//
//  LoadBalancer.hpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#ifndef LoadBalancer_hpp
#define LoadBalancer_hpp

#include <stdio.h>
#include <queue>
#include <random>
#include <vector>

#include "Simulator.hpp"

using std::vector;
using std::queue;
using std::shared_ptr;
using randomnessEngine = std::default_random_engine;
using exponentialDistribution = std::exponential_distribution<double>;

class Server;

class PacketArrivalEvent : public Event {
    Server* server;
public:
    PacketArrivalEvent(double arrival, Server* server);
    void process() const override;
};

class PacketDepartureEvent : public Event {
    const double waitTime;
    Server* server;
public:
    PacketDepartureEvent(double waitTime, double departureTime, Server* server);
    double getWaitTime() const;
    void process() const override;
};

class Server {
    queue<shared_ptr<PacketArrivalEvent>> packets;
    const unsigned int m_maxSize;
    const double m_serviceRate;
    
    randomnessEngine generator;
    exponentialDistribution serviceTime;
    
    bool busy;
    unsigned int tossedPackets;
    unsigned int servicedPackets;
    double totalWaitTime;

    Simulator& simulator;
public:
    Server(Simulator& sim, unsigned int maxSize, double serviceRate);
    ~Server() = default;
    
    bool isBusy() const;
    bool isFull() const;
    void tossPacket();
    int getTossedPacketsNum() const;
    int getServicedPacketsNum() const;
    void registerPacket(const PacketArrivalEvent& packet);
    void processPacket();
};

class LoadBalancerSim : public Simulator {
    vector<Server> servers;
    vector<double> probabilities;
    
    Server* selectServer();
    std::ostream& print(std::ostream& os) const override;
public:
    LoadBalancerSim(int simTime, const vector<int>& serversSize,
        const vector<double>& probabilityPerServer, double arrivalRate,
              const vector<double>& serviceRatePerServer);
    LoadBalancerSim(const LoadBalancerSim&) = delete;
    LoadBalancerSim& operator=(const LoadBalancerSim&) = delete;
    ~LoadBalancerSim() = default;
    
    int getTossedPacketsNum() const;
    int getTotalWaitTime() const;
    int getServicedPacketsNum() const;
};

#endif /* LoadBalancer_hpp */
