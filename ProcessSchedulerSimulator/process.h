#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

class Process {
public:
  //  constructors
  Process();
  Process(const vector<int> &, const vector<int> &,
    const string &, const string &);

  //  methods
  int CurrentCPUTime() const;
  int CurrentIOTime() const;
  void DecrementCPU();
  void DecrementIO();
  bool HasCPU() const;
  bool HasIO() const;
  void PopCPU();
  void PopIO();

  //  getters
  const vector<int> &cpus() const { return this->cpus_; }
  const vector<int> &ios() const { return this->ios_; }
  const string &name() const { return this->name_; }
  int priority() const { return this->priority_; }
  bool response_flag() const { return this->response_flag_; }
  int response_time() const { return this->response_time_; }
  const string &state() const { return this->state_; }
  int turnaround_time() const { return this->turnaround_time_; }
  int time_waiting() const { return this->time_waiting_; }

  //  setters
  void set_cpus(const vector<int> &cpus) { this->cpus_ = cpus; }
  void set_ios(const vector<int> &ios) { this->ios_ = ios; }
  void set_name(const string &name) { this->name_ = name; }
  void set_priority(int priority) { this->priority_ = priority; }
  void set_response_flag(bool flag) { this->response_flag_ = flag; }
  void set_response_time(int time) { this->response_time_ = time; }
  void set_state(const string &state) { this->state_ = state; }
  void set_turnaround_time(int time) { this->turnaround_time_ = time; }
  void set_time_waiting(int time) { this->time_waiting_ = time; }

private:
  vector<int> cpus_;    //  vector of ints specifying the CPU bursts
  vector<int> ios_;       //  vector of ints specifying the IO bursts
  string name_;           //  string specifying the name of the process
  int priority_;          //  integer to keep track of the current priority of the process
  bool response_flag_;    //  flag to check if response time has been set
  int response_time_;     //  integer to keep track of the first time the process is in running state
  string state_;          //  string for the current state of the process
  int turnaround_time_;   //  integer to keep track of when process finishes all CPU and IO bursts
  int time_waiting_;      //  integer to keep track of time spent in ready queue
};

#endif // PROCESS_H_