//
//  LoadBalancer.cpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#include "LoadBalancer.hpp"

int totalCount=0;

using std::make_shared;

PacketArrivalEvent::PacketArrivalEvent(double arrival, Server* server) : Event(arrival), server(server) {}

void PacketArrivalEvent::process() const {
    server->registerPacket(*this);
}

PacketDepartureEvent::PacketDepartureEvent(double arrivalTime, double departureTime, double serviceTime, Server* server) : Event(departureTime), packetArrivalTime(arrivalTime), serviceTime(serviceTime), server(server) {}

double PacketDepartureEvent::getServiceTime() const {
    return serviceTime;
}

double PacketDepartureEvent::getWaitTime() const {
    return occuranceTime - serviceTime - packetArrivalTime;
}

void PacketDepartureEvent::process() const  {
    server->processPacket(*this);
}


Server::Server(Simulator& sim, unsigned int maxSize, double serviceRate) : m_maxSize(maxSize), m_serviceRate(serviceRate), serviceTime(m_serviceRate), busy(false), tossedPackets(0), servicedPackets(0), totalWaitTime(0.0), simulator(sim) {
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed((unsigned int) seed);
}

bool Server::isFull() const {
    return packets.size() >= m_maxSize;
}

bool Server::isBusy() const {
    return busy;
}

void Server::tossPacket() {
    tossedPackets++;
}

int Server::getTossedPacketsNum() const {
    return tossedPackets;
}

int Server::getServicedPacketsNum() const {
    return servicedPackets;
}

double Server::getTotalWaitTime() const {
    return totalWaitTime;
}

double Server::getTotalServingTime() const {
    return totalServingTime;
}

void Server::registerPacket(const PacketArrivalEvent& packet){
    if(isBusy())
    {
        if(isFull()){
            tossPacket();
        }
        else
            packets.push(make_shared<PacketArrivalEvent>(packet));
    }
    else
    {
        busy = true;
        double arrivalTime = packet.getOccuranceTime();
        double servingTime = serviceTime(generator);
        double departureTime = arrivalTime + servingTime;
        std::shared_ptr<Event> newEvent = make_shared<PacketDepartureEvent>(arrivalTime, departureTime, servingTime, this);
        simulator.scheduleEvent((newEvent));
    }
}

void Server::processPacket(const PacketDepartureEvent& packet)
{
    servicedPackets++;
    totalWaitTime += packet.getWaitTime();
    totalServingTime += packet.getServiceTime();
    if(packets.size() == 0)
    {
        busy = false;
        return;
    }
    shared_ptr<PacketArrivalEvent> toServe = packets.front();
    packets.pop();
    double time = toServe->getOccuranceTime();
    double servingTime = serviceTime(generator);
    double departureTime = packet.getOccuranceTime() + servingTime;
    std::shared_ptr<Event> newEvent = make_shared<PacketDepartureEvent>(time, departureTime, servingTime, this);
    simulator.scheduleEvent(newEvent);
    
}

Server* LoadBalancerSim::selectServer() {
    // Generate a random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_number = dis(gen);

    // Select an element
    for (size_t i = 0; i < servers.size(); ++i) {
        if (random_number < probabilities[i]) {
            return &servers[i];
        }
        random_number -= probabilities[i];
    }
    // This point should never be reached if the probabilities are normalized
    throw std::runtime_error("Failed to select an element");
}

LoadBalancerSim::LoadBalancerSim(int simTime, const vector<int>& serversSizes,
    const vector<double>& probabilityPerServer, double arrivalRate,
                     const vector<double>& serviceRatePerServer) :
    Simulator(simTime), probabilities(probabilityPerServer)
{
    exponentialDistribution arrivalDistribution(arrivalRate);
    randomnessEngine generator;
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed((unsigned int) seed);
    
    for(int i=0; i< serversSizes.size(); i++)
        servers.push_back(Server(*this, serversSizes[i], serviceRatePerServer[i]));
    
    double time = 0;
    std::shared_ptr<Event> newEvent = make_shared<PacketArrivalEvent>(time, selectServer());
    scheduleEvent(newEvent);
    while(time < simTime){
        time += arrivalDistribution(generator);
        newEvent = make_shared<PacketArrivalEvent>(time, selectServer());
        scheduleEvent(newEvent);
    }
}

int LoadBalancerSim::getTossedPacketsNum() const {
    int count = 0;
    for(const Server& server : servers)
        count += server.getTossedPacketsNum();
    return count;
}

int LoadBalancerSim::getServicedPacketsNum() const {
    int count = 0;
    for(const Server& server : servers)
        count += server.getServicedPacketsNum();
    return count;
}

double LoadBalancerSim::getTotalWaitTime() const {
    double count = 0;
    for(const Server& server : servers)
        count += server.getTotalWaitTime();
    return count;
}

double LoadBalancerSim::getTotalServiceTime() const {
    double count = 0;
    for(const Server& server : servers)
        count += server.getTotalServingTime();
    return count;
}

double LoadBalancerSim::getPacketAvgWaitTime() const {
    return getTotalWaitTime() / getServicedPacketsNum();
}

double LoadBalancerSim::getAvgServiceTime() const {
    return getTotalServiceTime() / getServicedPacketsNum();
}

double LoadBalancerSim::getSimTiming() const {
    return currentTime;
}

std::ostream& LoadBalancerSim::print(std::ostream& os) const {
    os << getServicedPacketsNum() << " " << getTossedPacketsNum() << " ";
    os << getSimTiming() << " " << getAvgServiceTime() << " " << getPacketAvgWaitTime() ;
    return os;
}
