#include "process.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

//  A Default Constructor for the Process class
//  Takes no inputs
//  Does not set values for CPU bursts, IO bursts, process name, or process state
//  Sets other fields to default values
Process::Process() {
  this->response_flag_ = false;
  this->response_time_ = 0;
  this->time_waiting_ = 0;
  this->turnaround_time_ = 0;
  this->priority_ = 1;
}

//  A Constructor for the Process class
//  First input: A constant reference to a vector of ints representing the CPU bursts in order to be executed
//  Second input: A constant reference to a vector of ints representing the IO bursts in order to be executed
//  Third input: A constant reference to a string representing the name of the process
//  Fourth input: A constant reference to a string representing the state of the process
//  Sets other fields to default values
Process::Process(const vector<int> &cpus,
  const vector<int> &io, const string &name, const string &state)
  : cpus_(cpus), ios_(io), name_(name), state_(state) {
  this->response_flag_ = false;
  this->response_time_ = 0;
  this->time_waiting_ = 0;
  this->turnaround_time_ = 0;
  this->priority_ = 1;
}

//  A method to return the remaining CPU burst time for the current CPU burst
//  Takes no inputs
//  Should only be called on processes in the "ready" or "running" process state
//  Returns the value of the first entry in the CPU bursts vector,
//  which represents the current remaining burst time
int Process::CurrentCPUTime() const {
  return this->cpus_.front();
}

//  A method to return the remaining IO burst time for the current IO burst
//  Takes no inputs
//  Should only be called on processes in the "waiting" process state
//  Returns the value of the first entry in the IO bursts vector,
//  which represents the current remaining burst time 
int Process::CurrentIOTime() const {
  return this->ios_.front();
}

//  A method to decrement the current CPU burst (the burst at the front of the CPU burst vector)
//  Takes no inputs
//  Should only be called on processes in the "running" process state
//  Returns nothing
void Process::DecrementCPU() {
  --this->cpus_.front();
}

//  A method to decrement the current IO burst (the burst at the front of the IO burst vector)
//  Takes no inputs
//  Should only be called on processes in the "waiting" process state
//  Returns nothing
void Process::DecrementIO() {
  --this->ios_.front();
}

//  A method to determine if the process has more CPU bursts to be executed
//  Takes no inputs
//  Returns true if the CPU burst vector is not empty
//  Returns false if the CPU burst vector is empty
bool Process::HasCPU() const {
  return !this->cpus_.empty();
}

//  A method to determine if the process has more IO bursts to be executed
//  Takes no inputs
//  Retruns true if the IO burst vector is not empty
//  Returns true if the IO burst vector is empty
bool Process::HasIO() const {
  return !this->ios_.empty();
}

//  A method to remove the current CPU burst of a Process 
//  (erases the burst at the front of the CPU burst vector)
//  Takes no inputs
//  Returns nothing
void Process::PopCPU() {
  this->cpus_.erase(this->cpus_.begin());
}

//  A method to remove the current IO burst of a Process
//  (erases the burst at the front of the IO burst vector)
//  Takes no inputs
//  Returns nothing
void Process::PopIO() {
  this->ios_.erase(this->ios_.begin());
}