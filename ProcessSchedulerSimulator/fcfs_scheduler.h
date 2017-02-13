#ifndef FCFS_SCHEDULER_H_
#define FCFS_SCHEDULER_H_

#include "process.h"
#include "scheduler.h"

#include <vector>
using std::vector;

//  First Come First Serve Scheduler which implements the Scheduler interface
class FCFSScheduler : public Scheduler {
public:
  //  constructors
  FCFSScheduler();
  FCFSScheduler(const vector<Process *> &);

  //  methods
  void AddProcess(Process *);
  void DispatchProcess(int);
  void ManageTimeQuantum(Process *, bool &);    //  this method will do nothing  
  bool Preemption(Process *) const;             //  this method will always return false
  void PrintQueue() const;
  bool QueueEmpty() const;

  //  getters
  const vector<Process*> &ready_queue() const { return this->ready_queue_; }

  //  setters
  void set_ready_queue(const vector<Process*> &ready_queue) { this->ready_queue_ = ready_queue; }
private:
  //  ready queue for the FCFS Scheduler
  vector<Process *> ready_queue_;
};

#endif  //  FCFS_SCHEDULER_H_