//
//  Simulator.hpp
//  LoadBalancer
//
//  Created by Baraa Egbaria on 03/08/2024.
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <iostream>
#include <vector>
#include <queue>
#include <memory>

class Event {
protected:
    const double occuranceTime;
public:
    Event();
    Event(double time);
    virtual ~Event() = default;
    double getOccuranceTime() const;
    bool operator<(const Event& event);
    virtual void process() const = 0;
};

using EventPtr =  std::shared_ptr<Event> ;

struct EventComparator {
    bool operator() (const EventPtr& left, const EventPtr& right) const;
};

class Simulator {
protected:
    std::priority_queue<EventPtr, std::vector<EventPtr>, EventComparator> events;
    double simTime;
    double currentTime;
    
    virtual std::ostream& print(std::ostream& os) const = 0;
public:
    Simulator(double simTime);
    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    virtual ~Simulator() {};
    
    void scheduleEvent(EventPtr event);
    void runSim();
    
    friend std::ostream& operator<<(std::ostream& os, const Simulator& simulator);
};

#endif /* Simulator_hpp */
