//
//  main.cpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#include <iostream>
#include "LoadBalancer.hpp"
#include <vector>
int main(int argc, const char * argv[]) {
    LoadBalancerSim sim(5000, {2, 10}, {0.2, 0.8}, 200, {20, 190});
    sim.runSim();
    std::cout << sim << std::endl;
    return 0;
}
