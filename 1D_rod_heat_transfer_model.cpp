#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>

// structure for grid properties

using namespace std;

struct grid {
    double length; // length of rod in m
    double time_interval; // size of time step in s
    double spatial_interval; // distance between grid 'points' in m
};

struct material {
    double thermal_diffusivity{}; // in m^2/ s
    double density{}; // in kg/ m^3
    double specific_heat_cap{}; // in J/ (kg * K) <Joules per kilogram-Kelvin>
};

int main() {
    
    ofstream data_file("node_temp_variation.csv"); // output file created for python to read
    if (!data_file.is_open()){
        cout << "Error! .csv file cannot be opened" << endl; 
        return 1; // error code returned 
    }

    grid rod;                     // create instances of structures 
    material aerospace_aluminium;

    aerospace_aluminium.density = 2810.0;
    aerospace_aluminium.specific_heat_cap = 960.0;
    aerospace_aluminium.thermal_diffusivity = 0.0000482;

    rod.length = 0.5;
    rod.spatial_interval = 0.001;
    rod.time_interval = 0.01;

    double fourier_stab_criterion = aerospace_aluminium.thermal_diffusivity * rod.time_interval / (rod.spatial_interval * rod.spatial_interval); // check for instability in calculation
    if (fourier_stab_criterion > 0.5){
        cout << "Fourier stability criterion is greater than 0.5, enter different intervals or diffusivity!" << "\n";
        exit(1);
    }

    int node_number = (rod.length / rod.spatial_interval) + 1; // number of nodes, add 1 for endpoints
    double ambient_temp{300};                                  // ambient temperature ---IN KELVIN!---
    double heat_source_temp{500};                              // dirichlet boundry condition on left side of rod in Kelvin

    vector<double> current_temp(node_number, ambient_temp); // create vectors with length equal to number of nodes, all with values of ambient temperature
    vector<double> next_temp(node_number); // vector containing temperatures in next time interval

    current_temp.at(0) = heat_source_temp; // placing values of heat source temperature on leftmost node
    
    // writing .csv headers: Time(s), x=0, x=0.001, x=0.002, ...
    data_file << "Time(s)";
    for (int i = 0; i < node_number; ++i) {
        data_file << ",x=" << rod.spatial_interval * i;
    }
    data_file << "\n";

    // simulation and downsampling parameters
    double sim_time = 1000.0;                                                   // time (real time) to simulate, in seconds
    int total_steps = static_cast<int>(sim_time / rod.time_interval);           // 100,000 steps
    int seconds_per_frame = 1;                                                  // the number of seconds in 'real' time each frame will simulate
    int write_stride = static_cast<int>(seconds_per_frame / rod.time_interval); // 100 steps per frame

    // --- MAIN TIME LOOP ---
    for (int step = 0; step <= total_steps; ++step) {
        
        double current_time = step * rod.time_interval;

        // export current frame to .csv only at stride intervals
        if (step % write_stride == 0) {
            data_file << current_time;
            for (int j = 0; j < node_number; ++j) {
                data_file << "," << current_temp[j];
            }
            data_file << "\n"; 
        }

        // finite difference spatial update
        for (int j = 1; j < node_number - 1; ++j) {
            next_temp[j] = current_temp[j] + fourier_stab_criterion * (
                current_temp[j + 1] - 2.0 * current_temp[j] + current_temp[j - 1]
            );
        }

        // apply bouncry conditions (Dirichlet)
        next_temp[0] = heat_source_temp;
        next_temp[node_number - 1] = ambient_temp;

        swap(current_temp, next_temp); // swap new temperatures to be the current ready for next loop 
    }


    data_file.close(); // close .csv file

    return 0;
}
