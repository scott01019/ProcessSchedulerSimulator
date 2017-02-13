#include "mlfq_scheduler.h"
#include "process.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

//  A default constructor for the MLFQScheduler class
MLFQScheduler::MLFQScheduler() {}

//  A constructor for the MLFQScheduler class
//  First input: A constant reference to a vector of pointers to Process objects,
//  which represent the processes to be simulated
MLFQScheduler::MLFQScheduler(const vector<Process *> & jobs) 
  : high_priority_rr_queue_(jobs) {
  //  For each Process in the initial highest priority ready queue
  //  set that Process' priority to 1
  for (vector<Process *>::iterator it = this->high_priority_rr_queue_.begin();
    it != this->high_priority_rr_queue_.end(); ++it)
    (*it)->set_priority(1);
}

//  Adds a process to the ready queue taking priority into account
//  First input: A pointer to the Process object to be added to the ready queue
//  Priority 1 Processes are added to the highest priority round robin (Tq=6) queue
//  Priority 2 Processes are added to the lower priority round robin (Tq=11) queue
//  Priority greater than 2 are added to the lowest priority FCFS queue
//  Returns nothing
void MLFQScheduler::AddProcess(Process *process) {
  //  increment the priority of the Process
  process->set_priority(process->priority() + 1);

  //  if the Process has a priority of 1 add it to the highest
  //  priority ready queue
  if (process->priority() == 1)
    this->high_priority_rr_queue_.push_back(process);

  //  if the Process has a priority of 2 add it to the lower (teir 2)
  //  priority ready queue
  else if (process->priority() == 2)
    this->low_priority_rr_queue_.push_back(process);

  //  otherwise add the Process to the lowest priority FCFS ready queue
  else
    this->fcfs_queue_.push_back(process);
}

//  Dispatches the next Process in the ready queue with the highest priority
//  Changes the state of the of the dispatched process from "ready" to "running"
//  Also sets response flag/response time for process if it is the first time
//  entering "running" process state
//  First input: an integer representing the current time of the process manager
//  Returns nothing
void MLFQScheduler::DispatchProcess(int current_time) {
  //  if a Process exists in the highest priority ready queue
  if (!this->high_priority_rr_queue_.empty()) {
    //  if this is the first time the next Process has entered the "running" process state
    if (!this->high_priority_rr_queue_.front()->response_flag()) {
      //  set response flag to true
      this->high_priority_rr_queue_.front()->set_response_flag(true);

      //  set response time to the current time
      this->high_priority_rr_queue_.front()->set_response_time(current_time);
    }
    //  set the Process state to "running" and erase it from ready queue
    this->high_priority_rr_queue_.front()->set_state("running");
    this->high_priority_rr_queue_.erase(this->high_priority_rr_queue_.begin());

    //  set current time quantum to 6
    this->set_time_quantum(6);
  } 
  
  //  else if a Process exists in the lower priority round robin ready queue
  else if (!this->low_priority_rr_queue_.empty()) {
    //  set the Process state to "running" and erase it from ready queue
    this->low_priority_rr_queue_.front()->set_state("running");
    this->low_priority_rr_queue_.erase(this->low_priority_rr_queue_.begin());

    //  set current time quantum to 11
    this->set_time_quantum(11);
  } 
  
  //  else if a Process exists in the lowest priority FCFS ready queue
  else if (!this->fcfs_queue_.empty()) {
    //  set the Process state to "running" and erase it from ready queue
    this->fcfs_queue_.front()->set_state("running");
    this->fcfs_queue_.erase(this->fcfs_queue_.begin());
  }
}

//  Decrements the current time remaining for a running process in a round robin queue
//  and checks to see if a context switch is necessary
//  First input: A pointer to the Process which is currently in the "running" state
//  Second input: A reference to a bool which is the context switch flag
//  Returns nothing
void MLFQScheduler::ManageTimeQuantum(Process *running, bool &context_switch) {
  //  decrement the time remaining on time quantum
  --this->time_quantum_;

  //  if there exists a Process in the "running" process state which is from a round robin ready queue
  //  and time quantum has reached zero then a context switch is needed
  if (this->time_quantum_ == 0 && running != nullptr && running->priority() <= 2) {
    //  set context switch flag to true
    context_switch = true;

    //  if the "running" Process has not completed the current CPU burst then
    //  set its process state to "ready" and add it to the ready queue
    if (running->CurrentCPUTime() != 0) {
      running->set_state("ready");
      this->AddProcess(running);
    } 
    
    //  otherwise the Process has completed its current CPU burst
    else {
      //  remove the current burst, which has just been completed
      running->PopCPU();

      //  if the Process has more IO bursts to complete set its process state to "waiting"
      if (running->HasIO()) {
        running->set_state("waiting");
      }
      
      //  else if the Process has more CPU bursts to complete then set the process state to
      //  "ready" and add it to the ready queue
      else if (running->HasCPU()) {
        running->set_state("ready");
        this->AddProcess(running);
      } 
      
      //  else the Process has no more CPU bursts or IO bursts and should have its process state
      //  set to "terminated"
      else {
        running->set_state("terminated");
      }
    }
  }
}

//  Determines whether a preemption is needed, which occurs when a Process
//  in a higher priority queue is in the "ready" process state
//  First input: A pointer to a Process which is in the "running" state
//  Returns true if a preemptive context switch is necessary
//  Returns false if preemptive context switch is not necessary
bool MLFQScheduler::Preemption(Process *running) const {

  //  if the current Procces has a priority greater than 2 and a Process
  //  exists in a queue with a higher priority (priority = 1 or priority = 2)
  //  then preemption is needed return true
  if ((running != nullptr && running->priority() > 2) &&
    (!this->high_priority_rr_queue_.empty() || !this->low_priority_rr_queue_.empty()))
    return true;

  //  else if the current Process has a priority of 2 and a Process
  //  exists in the highest priority queue (priority = 1)
  //  then preemption is needed return true
  else if ((running != nullptr && running->priority() == 2) &&
    !this->high_priority_rr_queue_.empty())
    return true;

  //  else there is no preemption return false
  else
    return false;
}

//  Prints a given queue to the console
//  First input: A constant reference to a vector of pointers to Process objects,
//  which represent the queue to be printed
//  Returns nothing
//  Outputs the input queue to the console
void MLFQScheduler::PrintAQueue(const vector<Process *> &queue) const {
  if (queue.begin() == queue.end())
    cout << "\t\t[empty]" << endl;
  else {
    for (vector<Process *>::const_iterator it = queue.begin(); it != queue.end(); ++it)
      cout << "\t\t" << (*it)->name() << "\t\t" << (*it)->CurrentCPUTime() << endl;
  } cout << endl;
}

//  Prints the ready queues for the MLFQ scheduler
//  Takes not inputs
//  Returns nothing
void MLFQScheduler::PrintQueue() const {
  cout << "Ready Queue 1:\tProcess\t\tBurst" << endl;
  this->PrintAQueue(this->high_priority_rr_queue_);
  cout << "Ready Queue 2:\tProcess\t\tBurst" << endl;
  this->PrintAQueue(this->low_priority_rr_queue_);
  cout << "Ready Queue 3:\tProcess\t\tBurst" << endl;
  this->PrintAQueue(this->fcfs_queue_);
}

//  Determines whether the ready queues are all empty
//  Takes no inputs
//  Returns true if all of the scheduler's ready queues are empty
//  Returns false if any of the scheduler's ready queue are not empty
bool MLFQScheduler::QueueEmpty() const {
  return (this->high_priority_rr_queue_.empty() &&
    this->low_priority_rr_queue_.empty() &&
    this->fcfs_queue_.empty());
}