#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"

//  An interface to be implemented by all scheduler classes
class Scheduler {
public:
  //  Add a process to the ready queue
  //  First input: A pointer to the Process which is to be added to the ready queue
  //  Returns nothing
  virtual void AddProcess(Process *) = 0;

  //  Dispatches the next Process in the ready queue with the highest priority
  //  Changes state of next Process from "ready" to "running"
  //  Also sets response flag/response time if next Process is entering "running" state for the firs time
  //  First input: an integer representing the current time of the process manager (used to set response time if necessary)
  //  Returns nothing
  virtual void DispatchProcess(int) = 0;

  //  Manages time quantum for round robin schedulers and determines whether a context switch is needed
  //  First input: A pointer to the Process which is currently in the "running" state
  //  Second input: A reference to a bool which is the context switch flag
  //  Returns nothing
  virtual void ManageTimeQuantum(Process *, bool &) = 0;

  //  Determines whether a preemption is needed for scheduler supporting preemption
  //  First input: A pointer to a Process which is in the "running" state
  //  Returns true if a preemptive context switch is necessary
  //  Returns false if preemptive context switch is not necessary
  virtual bool Preemption(Process *) const = 0;

  //  Prints the ready queue(s) for the scheduler
  //  Takes not inputs
  //  Returns nothing
  virtual void PrintQueue() const = 0;

  //  Determines whether the ready queue(s) are all empty
  //  Takes no inputs
  //  Returns true if all of the scheduler's ready queues are empty
  //  Returns false if any of the scheduler's ready queue are not empty
  virtual bool QueueEmpty() const = 0;
};

#endif // SCHEDULER_H_