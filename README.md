# Load Balancer Simulation

This repository contains a simple simulation of a load balancer that distributes incoming packets across multiple servers.

## Description

The simulation models a scenario where packets arrive at the load balancer at a Poisson rate. Each server has a unique probability of receiving a packet from the load balancer. After a packet is processed by a server, it leaves the server at a Poisson rate.

The Poisson process is a statistical model that represents how events occur in a fixed interval of time or space. In this case, it's used to model the random arrival of packets to the load balancer. The different probabilities of each server receiving a packet could represent a real-world scenario where some servers are more powerful or have more capacity than others, and thus can handle more packets.

The goal of this simulation is to help understand how different load balancing strategies can affect the performance of a system. By adjusting the arrival rate of packets and the probabilities of each server receiving a packet, you can see how these factors impact the overall system performance.

### Executing the Program

* There's a makefile for the project in the repo. All you need to do is run `make`, and an executable with the name `simulator` is created.
* All you need to do now is run the executable with the following parameters:
  * T - total run time of the Simulation
  * N - number of servers
  * P1, P2, ..., Pn - Probabilities of the servers, should accumulate to 1
  * lambda - arrival rate of the packets from the internet
  * Q1, Q2, ..., Qn - the size of the waiting Queue for each server.
  * M1, M2, ..., Mn - the service rate of each server in the system.

```bash
./simulator T N P1 P2 ... Pn 𝜆 𝑄1 𝑄2 ... 𝑄n  μ1 μ2 ... μn
./simulator 5000 2 0.2 0.8 200 2 10 20 190
