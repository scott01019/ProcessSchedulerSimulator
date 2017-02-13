#include "fcfs_scheduler.h"
#include "process.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

//  A default constructor for the FCFSScheduler class
FCFSScheduler::FCFSScheduler() {}

//  A constructor for the FCFSScheduler class
//  First input: A constant reference to a vector of pointers to Process objects,
//  which represent the processes to be simulated
FCFSScheduler::FCFSScheduler(const vector<Process *> &jobs) : ready_queue_(jobs) {}

//  Adds a process to the ready queue
//  First input: A pointer to the Process object to be added to the ready queue
//  Returns nothing
void FCFSScheduler::AddProcess(Process *process) {
  this->ready_queue_.push_back(process);
}

//  Dispatches the next Process in the ready queue
//  Changes the state of the of the dispatched process from "ready" to "running"
//  Also sets response flag/response time for process if it is the first time
//  entering "running" process state
//  First input: an integer representing the current time of the process manager
//  Returns nothing
void FCFSScheduler::DispatchProcess(int current_time) {
  //  if the ready queue is not empty
  if (!this->ready_queue_.empty()) {

    //  if this is the first time the next Process has entered the "running" process state  
    if (!this->ready_queue_.front()->response_flag()) {
      //  set response flag to true
      this->ready_queue_.front()->set_response_flag(true);

      //  set response time to current time
      this->ready_queue_.front()->set_response_time(current_time);
    }

    //  set the next Process to "running" process state and erase it from ready queue
    this->ready_queue_.front()->set_state("running");
    this->ready_queue_.erase(this->ready_queue_.begin());
  }
}

//  The FCFS Scheduler does not have a round robin queue and hence
//  does not need to do anything when ManageTimeQuantum is called.
//  This is only implemented, because it is required by the Scheduler
//  interface.
//  First input: A pointer to a Process object which is in the "running" process state
//  Second input: A reference to a boolean value which represents the context switch flag
//  Returns nothing.
void FCFSScheduler::ManageTimeQuantum(Process *running, bool &context_switch) {}

//  The FCFS Scheduler does not support preemption and hence will
//  always return false. This is only implemented, because it is required by
//  the Scheduler interface
//  Takes no inputs
//  Always returns false
bool FCFSScheduler::Preemption(Process *running) const { return false; }

//  Prints the ready queue to the console
//  Takes no inputs
//  Returns nothing
//  Outputs the ready queue to the console
void FCFSScheduler::PrintQueue() const {
  cout << "Ready Queue:\tProcess\t\tBurst" << endl;
  if (this->ready_queue_.begin() == this->ready_queue_.end())
    cout << "\t\t[empty]" << endl;
  else {
    for (vector<Process *>::const_iterator it = this->ready_queue_.begin();
      it != this->ready_queue_.end(); ++it) {
      cout << "\t\t" << (*it)->name() << "\t\t" << (*it)->CurrentCPUTime() << endl;
    }
  }
}

//  Determines if the ready queue for the FCFS scheduler is empty
//  Takes no inputs
//  Returns true if the ready queue is empty
//  Returns false if the ready queue is not empty
bool FCFSScheduler::QueueEmpty() const {
  return this->ready_queue_.empty();
}