#ifndef SJF_SCHEDULER_H_
#define SJF_SCHEDULER_H_

#include "process.h"
#include "scheduler.h"

#include <vector>
using std::vector;

//  Shortest Job First Scheduler which implements the Scheduler interface
class SJFScheduler : public Scheduler {
public:
  //  constructors
  SJFScheduler();
  SJFScheduler(const vector<Process *> &);

  //  methods
  void AddProcess(Process *);
  void DispatchProcess(int);
  void ManageTimeQuantum(Process *, bool &);  //  this method will do nothing
  bool Preemption(Process *) const;           //  this method will always return false
  void PrintQueue() const;
  bool QueueEmpty() const;

  //  getters
  const vector<Process *> &ready_queue() const { return this->ready_queue_; }

  //  setters
  void set_ready_queue(const vector<Process *> &ready_queue) { this->ready_queue_ = ready_queue; }
private:
  //  ready queue for the SJF Scheduler (is always sorted by priority)
  vector<Process *> ready_queue_;
};

#endif  //  SJF_SCHEDULER_H_