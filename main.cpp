//
//  main.cpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#include <iostream>
#include "LoadBalancer.hpp"
#include <vector>

const int T = 1;
const int NUM = 2;
const int PROABILITIES = 3;

int main(int argc, const char * argv[]) {
    // parse input from command line
    int totalSimTime = atoi(argv[T]);
    int serversNum = atoi(argv[NUM]);
    std::vector<double> probabilities;
    for(int i = 0; i < serversNum; i++) {
        probabilities.push_back(atof(argv[PROABILITIES+i]));
    }
    double arrivalRate = atof(argv[PROABILITIES+serversNum]);
    std::vector<int> serversQueueSizes;
    for(int i = 0; i < serversNum; i++) {
        serversQueueSizes.push_back(atoi(argv[4+serversNum+i]));
    }
    std::vector<double> serviceRate;
    for(int i = 0; i < serversNum; i++) {
        serviceRate.push_back(atoi(argv[4+2*serversNum+i]));
    }

    // run simulation
    LoadBalancerSim simulator(totalSimTime, serversQueueSizes, probabilities, arrivalRate, serviceRate);
    simulator.runSim();
    std::cout << simulator << std::endl;
    return 0;
}
