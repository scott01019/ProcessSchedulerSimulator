#include "functions.h"
#include "sjf_scheduler.h"

#include <algorithm>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::sort;
using std::vector;

//  A default constructor for the SJFScheduler class
SJFScheduler::SJFScheduler() {}

//  A constructor for the SJFScheduler class
//  First input: A constant reference to a vector of pointers to Process objects,
//  which represent the processes to be simulated
SJFScheduler::SJFScheduler(const vector<Process *> &jobs) {
  //  For each Process in the jobs queue add it to the ready queue
  for (vector<Process *>::const_iterator it = jobs.begin(); it != jobs.end(); ++it)
    this->AddProcess(*it);
}

//  Adds a process to the ready queue taking priority into account
//  First input: A pointer to the Process object to be added to the ready queue
//  Returns nothing
void SJFScheduler::AddProcess(Process *process) {
  //  set priority of process equal to its current burst time
  process->set_priority(process->CurrentCPUTime());

  //  add to the ready queue
  this->ready_queue_.push_back(process);

  //  sort ready queue by priority using the ProcessCmp comparator
  //  highest priority will be in the front of the queue
  sort(this->ready_queue_.begin(), this->ready_queue_.end(), ProcessCmp);
}

//  Dispatches the next Process in the ready queue with the highest priority
//  Changes the state of the of the dispatched process from "ready" to "running"
//  Also sets response flag/response time for process if it is the first time
//  entering "running" process state
//  First input: an integer representing the current time of the process manager
//  Returns nothing
void SJFScheduler::DispatchProcess(int current_time) {
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

//  The SJF Scheduler does not have a round robin queue and hence
//  does not need to do anything when ManageTimeQuantum is called.
//  This is only implemented, because it is required by the Scheduler
//  interface.
//  First input: A pointer to a Process object which is in the "running" process state
//  Second input: A reference to a boolean value which represents the context switch flag
//  Returns nothing.
void SJFScheduler::ManageTimeQuantum(Process *running, bool &context_switch) { }

//  The SJF Scheduler does not support preemption and hence will
//  always return false. This is only implemented, because it is required by
//  the Scheduler interface
//  Takes no inputs
//  Always returns false
bool SJFScheduler::Preemption(Process *) const { return false; }

//  Prints the ready queue to the console
//  Takes no inputs
//  Returns nothing
//  Outputs the ready queue to the console with processes in order by highest priority
void SJFScheduler::PrintQueue() const {
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

//  Determines if the ready queue for the SJF scheduler is empty
//  Takes no inputs
//  Returns true if the ready queue is empty
//  Returns false if the ready queue is not empty
bool SJFScheduler::QueueEmpty() const {
  return this->ready_queue_.empty();
}