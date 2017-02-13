#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#include "process.h"
#include "scheduler.h"

#include <vector>
using std::vector;

class ProcessManager {
public:
  //  constructors
  ProcessManager(const vector<Process*> &, Scheduler *);

  //  methods
  bool AllTerminated() const;
  int CountWaiting() const;
  Process *GetRunning() const;
  bool HasTerminated() const;
  void PrintContext(int) const;
  void PrintCurrentIO() const;
  void PrintProcessNames() const;
  void PrintResponseTimes() const;
  void PrintStats(int, int) const;
  void PrintTerminated() const;
  void PrintTurnaroundTimes() const;
  void PrintWaitTimes() const;
  void ProcessRunning(Process *, int, bool &);
  void ProcessWaiting(Process *, int, bool &);
  void SimulateScheduler();

  //  getters
  const vector<Process *> &jobs() const { return this->jobs_; }
  Scheduler *scheduler() const { return this->scheduler_; }

  //  setters
  void set_jobs(const vector<Process *> &jobs) { this->jobs_ = jobs; }
  void set_scheduler(Scheduler *scheduler) { this->scheduler_ = scheduler; }

private:
  Scheduler *scheduler_;    //  A pointer to the scheduler which is to be used for the simulation
  vector<Process *> jobs_;  //  A vector of pointers to the Process objects which are to be simulated
};

#endif  //  PROCESS_MANAGER_H_