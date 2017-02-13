#include "fcfs_scheduler.h"
#include "mlfq_scheduler.h"
#include "process.h"
#include "process_manager.h"
#include "sjf_scheduler.h"

#include <iostream>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

int main() {
  bool running = true;
  while (running) {
    //  CPU bursts for the processes
    vector<int> p1_cpu({ 4, 5, 3, 5, 4, 6, 4, 5, 2 });
    vector<int> p2_cpu({ 18, 19, 11, 18, 19, 18, 17, 19, 10 });
    vector<int> p3_cpu({ 6, 4, 7, 4, 5, 7, 8, 6, 5 });
    vector<int> p4_cpu({ 17, 19, 20, 17, 15, 12, 15, 14 });
    vector<int> p5_cpu({ 5, 4, 5, 3, 5, 4, 3, 4, 3, 5 });
    vector<int> p6_cpu({ 10, 12, 14, 11, 15, 13, 11 });
    vector<int> p7_cpu({ 21, 23, 24, 22, 21, 20 });
    vector<int> p8_cpu({ 11, 14, 15, 17, 16, 12, 13, 15 });

    //  IO bursts for the processes
    vector<int> p1_io({ 24, 73, 31, 27, 33, 43, 64, 19 });
    vector<int> p2_io({ 31, 35, 42, 43, 47, 43, 51, 32 });
    vector<int> p3_io({ 18, 21, 19, 16, 29, 21, 22, 24 });
    vector<int> p4_io({ 42, 55, 54, 52, 67, 72, 66 });
    vector<int> p5_io({ 81, 82, 71, 61, 62, 51, 77, 61, 42 });
    vector<int> p6_io({ 35, 41, 33, 32, 41, 29 });
    vector<int> p7_io({ 51, 53, 61, 31, 43 });
    vector<int> p8_io({ 52, 42, 31, 21, 43, 31, 32 });

    //  Initialize the processes
    Process P1(p1_cpu, p1_io, "P1", "ready");
    Process P2(p2_cpu, p2_io, "P2", "ready");
    Process P3(p3_cpu, p3_io, "P3", "ready");
    Process P4(p4_cpu, p4_io, "P4", "ready");
    Process P5(p5_cpu, p5_io, "P5", "ready");
    Process P6(p6_cpu, p6_io, "P6", "ready");
    Process P7(p7_cpu, p7_io, "P7", "ready");
    Process P8(p8_cpu, p8_io, "P8", "ready");

    //  A vector of all processes
    vector<Process *> jobs = { &P1, &P2, &P3, &P4, &P5, &P6, &P7, &P8 };

    cout << "Process Scheduler Simulator\n\n";
    cout << "1. Shortest Job First Simulation\n";
    cout << "2. First Come First Server Simulation\n";
    cout << "3. Multi Level Feedback Queue Simulation\n";
    cout << "4. Exit\n\n";
    cout << "Input: ";

    int input;

    cin >> input;

    switch (input) {
    case 1: {
      //  A shortest job first scheduler which is passed the jobs list
      SJFScheduler *sjf_scheduler = new SJFScheduler(jobs);

      //  A process manager which is passed the jobs list and the scheduler to be simulated
      ProcessManager simulator(jobs, sjf_scheduler);

      //  Run the simulation
      simulator.SimulateScheduler();
    } break;
    case 2: {
      //  A first come first serve scheduler which is passed the jobs list
      FCFSScheduler *fcfs_scheduler = new FCFSScheduler(jobs);

      //  A process manager which is passed the jobs list and the scheduler to be simulated
      ProcessManager simulator(jobs, fcfs_scheduler);

      //  Run the simulation
      simulator.SimulateScheduler();
    } break;
    case 3: {
      //  A multilevel feedback queue which is passed the jobs list
      MLFQScheduler *mlfq_scheduler = new MLFQScheduler(jobs);

      //  A process manager which is passed the jobs list and the scheduler to be simulated
      ProcessManager simulator(jobs, mlfq_scheduler);
      
      //  Run the simulation
      simulator.SimulateScheduler();
    } break;
    case 4: {
      running = false;
    } break;
    }
  }
  return 0;
}