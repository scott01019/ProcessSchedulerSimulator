#ifndef MLFQ_SCHEDULER_H_
#define MLFQ_SCHEDULER_H_

#include "process.h"
#include "scheduler.h"

#include <vector>
using std::vector;

//  A Multilevel Feedback Queue Scheduler which implements the Scheduler interface
class MLFQScheduler : public Scheduler {
public:
  //  constructors
  MLFQScheduler();
  MLFQScheduler(const vector<Process *> &);

  //  methods
  void AddProcess(Process *);
  void DispatchProcess(int);
  void ManageTimeQuantum(Process *, bool &);
  bool Preemption(Process *) const;
  void PrintAQueue(const vector<Process *> &) const;
  void PrintQueue() const;
  bool QueueEmpty() const;

  //  getters
  const vector<Process *> &fcfs_queue() const { return this->fcfs_queue_; }
  const vector<Process *> &low_priority_queue() const { return this->low_priority_rr_queue_; }
  const vector<Process *> &ready_queue() const { return this->high_priority_rr_queue_; }
  int time_quantum() const { return this->time_quantum_; }

  //  setters
  void set_fcfs_queue(const vector<Process *> &q) { this->fcfs_queue_ = q; }
  void set_low_priority_queue(const vector<Process *> &q) { this->low_priority_rr_queue_ = q; }
  void set_ready_queue(const vector<Process *> &q) { this->high_priority_rr_queue_ = q; }
  void set_time_quantum(int time) { this->time_quantum_ = time; }
private:
  vector<Process *> high_priority_rr_queue_;  //  Highest priority round robin queue (Tq = 6)
  vector<Process *> low_priority_rr_queue_;   //  Lower priority round robin queue (Tq = 11)
  vector<Process *> fcfs_queue_;              //  Lowest priority fcfs queue
  int time_quantum_;                          //  An integer to hold the remaining time left of time quantum
};

#endif  //  MLFQ_SCHEDULER_H_