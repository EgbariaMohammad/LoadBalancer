//
//  Simulator.cpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#include "Simulator.hpp"
#include <iostream>

Event::Event(double time) : occuranceTime(time) {};

double Event::getOccuranceTime() const {
    return occuranceTime;
}

bool Event::operator<(const Event& event) {
    return occuranceTime < event.occuranceTime;
}

bool EventComparator::operator()(const EventPtr& left, const EventPtr& right) const
{
    return left->getOccuranceTime() > right->getOccuranceTime();
}

Simulator::Simulator(double simTime) : simTime(simTime), currentTime(0.0) {}

void Simulator::scheduleEvent(EventPtr event){
    events.push(event);
}

void Simulator::runSim(){
    while(events.size() != 0 && currentTime < simTime)
    {
        const auto& event = events.top();
        events.pop();
        currentTime = event->getOccuranceTime();
        event->process();
    }
    std::cout << "endTime: " << currentTime << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Simulator& simulator) {
    return simulator.print(os);
}
