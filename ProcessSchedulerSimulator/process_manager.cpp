#include "fcfs_scheduler.h"
#include "process_manager.h"

#include <iostream>
using std::cout;
using std::endl;

//  A Constructor for the ProcessManager class
//  First input: A constant reference to a vector of Process pointers to each 
//  of the Processes to be simulated
//  Second input: A pointer to a scheduler object which is the scheduler used 
//  during the simulation
ProcessManager::ProcessManager(const vector<Process*> &jobs, Scheduler *scheduler)
  : jobs_(jobs), scheduler_(scheduler) { }

//  Determines if the simulation is completed by check if all Processes in the 
//  jobs queue are in the "terminated" process state
//  Takes no inputs
//  Returns true if all processes in the jobs queue are in the "terminated" 
//  process state
//  Returns false if any of the processes in the jobs queue are not in the 
//  "terminated" process state
bool ProcessManager::AllTerminated() const {
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    if ((*it)->state() != "terminated")
      return false;
  }
  return true;
}

//  Determines if there are any Processes in the jobs queue in the "terminated"
//  process state
//  Takes no inputs
//  Returns true if the jobs queue contains a Process in the "terminated" process
//  state
//  Returns false if the jobs queue does not contain a Process in the "terminated"
//  process state
bool ProcessManager::HasTerminated() const {
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    if ((*it)->state() == "terminated")
      return true;
  }
  return false;
}

//  Determines the number of Processes in the jobs queue in the "waiting" process state
//  Takes no inputs
//  Returns an integer representing the number of Process objects in the jobs queue which
//  are in the "waiting" process state
int ProcessManager::CountWaiting() const {
  int count = 0;
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    if ((*it)->state() == "waiting") ++count;
  }
  return count;
}

//  Finds the Process in the jobs queue in the "running" process state
//  Takes no inputs
//  Returns a pointer to the Process object in the jobs queue which is in the "running"
//  process state
Process *ProcessManager::GetRunning() const {
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    if ((*it)->state() == "running")
      return *it;
  }
  return nullptr;
}

//  Runs the actual simulation
//  Takes no inputs
//  Returns nothing
//  Outputs all context switches and result statistics to console
void ProcessManager::SimulateScheduler() {
  int current_time = 0;                               //  track current time of simulation
  int idle_time = 0;                                  //  track time CPU is idle

  //  get the first Process from the scheduler
  this->scheduler_->DispatchProcess(current_time);
  
  //  print the first context switch
  this->PrintContext(current_time);

  //  while any of the Process' are not in the "terminated" process state
  while (!this->AllTerminated()) {
    bool context_switch = false;    //  create a context switch flag for the current iteration

    //  increment the current time of simulation
    ++current_time;

    //  if there is currently no Process in the "running" process state then increment idle time
    if (this->GetRunning() == nullptr) ++idle_time;

    //  for each Process in the jobs queue
    for (vector<Process *>::iterator it = this->jobs_.begin();
      it != this->jobs_.end(); ++it) {
      //  if the Process is in the "running" process state call ProcessRunning
      if ((*it)->state() == "running") {
        this->ProcessRunning(*it, current_time, context_switch);
      } 
      
      //  else if the Process is in the "waiting" process state call ProcessWaiting
      else if ((*it)->state() == "waiting") {
        this->ProcessWaiting(*it, current_time, context_switch);
      } 
      
      //  else if the Process is in the "ready" process state increment waiting time for the Process
      else if ((*it)->state() == "ready") {
        (*it)->set_time_waiting((*it)->time_waiting() + 1);
      }
    }

    //  Check to see if preemption is required
    //  NOTE: Only returns true for our MLFQScheduler class
    if (this->scheduler_->Preemption(this->GetRunning())) {
      //  set context switch flag true
      context_switch = true;
      
      //  since preempted Process' which are running return to the ready queue
      //  from which they came we must first decrement that Process' priority
      //  as it will be incremented when we call AddProcess
      this->GetRunning()->set_priority(this->GetRunning()->priority() - 1);

      //  add the preempted Process back to the ready queue and set its process state to "ready"
      this->scheduler_->AddProcess(this->GetRunning());
      this->GetRunning()->set_state("ready");
    }

    //  Handle time quantum and determine if a conext switch is needed
    //  NOTE: Only does something for Schedulers which implement a round robin queue
    //  in our case that is only Schedulers from the MLFQScheduler class
    this->scheduler_->ManageTimeQuantum(this->GetRunning(), context_switch);

    //  if a context switch is required
    if (context_switch) {
      //  have the scheduler dispatch a new "running" Process
      this->scheduler_->DispatchProcess(current_time);

      //  Print the context switch details to console
      this->PrintContext(current_time);
    }
  }

  //  When simulation is completed print results
  this->PrintStats(current_time, idle_time);
}

//  Prints the statistics for the results of the simulation to the console
//  First input: An integer representing the total time of the simulation
//  Second input: An integer representing the total idle time of the simulation
//  Returns nothing
//  Ouputs simulation results to console
void ProcessManager::PrintStats(int time, int idle) const {
  cout << endl << endl << "Finished" << endl << endl;
  cout << "Total Time:\t\t" << time << endl;
  cout << "CPU Utilization:\t" << 100.0 * (time - idle) / time << "%";
  cout << endl << endl;
  this->PrintWaitTimes();
  this->PrintTurnaroundTimes();
  this->PrintResponseTimes();
}

//  Prints response times for each process and the average response time to the console
//  Takes no inputs
//  Returns nothing
//  Outputs response times for each process and average response time to the console
void ProcessManager::PrintResponseTimes() const {
  cout << "Reponse Times:\t\t";
  this->PrintProcessNames();
  cout << "\t\t\t";
  int average_response_time = 0;
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    average_response_time += (*it)->response_time();
    cout.width(5);
    cout << std::left << (*it)->response_time();
  } cout << endl;
  cout << "Average Reponse:\t" << average_response_time / 8.0 << endl << endl;
}

//  Prints turnaround times for each process and the average turnaround time to the console
//  Takes no inputs
//  Returns nothing
//  Outputs turnaround times for each process and average turnaround time to the console
void ProcessManager::PrintTurnaroundTimes() const {
  cout << "Turnaround Times:\t";
  this->PrintProcessNames();
  cout << "\t\t\t";
  int average_turnaround_time = 0;
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    average_turnaround_time += (*it)->turnaround_time();
    cout.width(5);
    cout << std::left << (*it)->turnaround_time();
  } cout << endl;
  cout << "Average Turnaround:\t" << average_turnaround_time / 8.0 << endl << endl;
}

//  Prints waiting times for each process and the average waiting time to the console
//  Takes no inputs
//  Returns nothing
//  Outputs waiting times for each process and average waiting time to the console
void ProcessManager::PrintWaitTimes() const {
  cout << "Waiting Times:\t\t";
  this->PrintProcessNames();
  cout << "\t\t\t";
  int average_waiting_time = 0;
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    average_waiting_time += (*it)->time_waiting();
    cout.width(5);
    cout << std::left << (*it)->time_waiting();
  } cout << endl;
  cout << "Average Wait:\t\t" << average_waiting_time / 8.0 << endl << endl;
}

//  Prints process names to the console
//  Takes no inputs
//  Returns nothing
//  Outputs process names to the console
void ProcessManager::PrintProcessNames() const {
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    cout.width(5);
    cout << std::left << (*it)->name();
  } cout << endl;
}

//  Decrements current CPU burst of the current Process in "running" process state
//  and determines if the a context switch is necessary
//  First input: A pointer to the Process object in the "running" process state
//  Second input: An integer representing the current run time of the simulation
//  Third input: A reference to a boolean value representing the context switch flag
//  Returns nothing
void ProcessManager::ProcessRunning(Process *process, int current_time
  , bool &context_switch) {
  //  decrement the current CPU burst
  process->DecrementCPU();

  //  if the burst is completed
  if (process->CurrentCPUTime() == 0) {
    //  set context switch flag to true
    context_switch = true;

    //  pop completed CPU burst form CPU burst vector
    process->PopCPU();
    
    //  if the Process needs to complete an IO burst
    if (process->HasIO()) {
      //  set Process state to "waiting" and initialize priority to 0 for next potential CPU burst
      process->set_state("waiting");
      process->set_priority(0);
    } 
    
    //  else if the Process needs to complete a CPU burst
    //  NOTE: this should never happen for our case, because all of our Processes
    //  follow a CPU, IO, CPU, IO, ... pattern, however, this is implemented for the
    //  general purpose
    else if (process->HasCPU()) {
      //  set Process state to "ready" and use scheduler to add Process to ready queue
      process->set_state("ready");
      this->scheduler_->AddProcess(process);
    } 
    
    //  else the Process has no more CPU bursts or IO bursts
    else {
      //  set Process state to "terminated" and set turnaround time to current time
      process->set_state("terminated");
      process->set_turnaround_time(current_time);
    }
  }
}

//  Decrements current IO burst of the a Process in the "waiting" process state
//  and determines if the a context switch is necessary
//  First input: A pointer to a Process object in the "waiting" process state
//  Second input: An integer representing the current run time of the simulation
//  Third input: A reference to a boolean value representing the context switch flag
//  Returns nothing
void ProcessManager::ProcessWaiting(Process *process, int current_time,
  bool &context_switch) {
  //  decrement the current IO burst
  process->DecrementIO();

  //  if the current IO burst is completed
  if (process->CurrentIOTime() == 0) {

    //  removed completed IO burst from the IO burst vector
    process->PopIO();

    //  if Process has another CPU burst to complete
    if (process->HasCPU()) {

      //  if there is no "running" Process and the ready queue is empty
      if (this->GetRunning() == nullptr && this->scheduler_->QueueEmpty()) {

        //  set context switch flag to true (the Process we are about to add will become
        //  the new "running" Process)
        context_switch = true;
      }

      //  set the process state to "ready" and add it to the ready queue
      process->set_state("ready");
      this->scheduler_->AddProcess(process);
    } 
    
    //  else if the Process has another IO burst to complete
    //  NOTE: this should never happen for our case, because all of our Processes
    //  follow a CPU, IO, CPU, IO, ... pattern, however, this is implemented for the
    //  general purpose
    else if (process->HasIO()) {
      //  set the Process state to "waiting" and initialize the Process priority to
      //  zero for any potential future times it will be added to ready queue
      process->set_state("waiting");
      process->set_priority(0);
    } 
    
    //  else the Process has no more CPU or IO bursts
    else {
      
      //  set the Process state to "terminated"
      process->set_state("terminated");

      //  set the turnarond time of the Process to the current time
      process->set_turnaround_time(current_time);
    }
  }
}

//  Prints the context switch to the console
//  First input: An integer which represents the current time of the simulation
//  Returns nothing
//  Outputs the details of a context switch to the console
void ProcessManager::PrintContext(int current_time) const {
  cout << endl << endl << "Current Time:\t" << current_time << endl << endl;
  cout << "Now Running:\t";
  if (this->GetRunning()) cout << this->GetRunning()->name() << " (" << this->GetRunning()->CurrentCPUTime() << ")" << endl;
  else cout << "[idle]" << endl;
  cout << ".................................................." << endl << endl;
  this->scheduler_->PrintQueue();
  cout << ".................................................." << endl << endl;
  cout << "Now in I/O:\tProcess\t\tRemaining I/O time" << endl;
  this->PrintCurrentIO();
  cout << ".................................................." << endl;
  if (this->HasTerminated()) this->PrintTerminated();
  cout << ".................................................." << endl << endl;
}

//  Prints the processes which are currently waiting for IO to the console
//  Takes no inputs
//  Returns nothing
//  Outputs the names and time remaining in IO for the processes which are currently
//  in the "waiting" process state to the console
void ProcessManager::PrintCurrentIO() const {
  if (this->CountWaiting() == 0) cout << "\t\t[empty]" << endl << endl;
  else {
    for (vector<Process *>::const_iterator it = this->jobs_.begin();
      it != this->jobs_.end(); ++it) {
      if ((*it)->state() == "waiting")
        cout << "\t\t" << (*it)->name() << "\t\t" << (*it)->CurrentIOTime() << endl;
    }
  }
}

//  Prints the names of processes which are currently in the "terminated" process state
//  Takes no inputs
//  Returns nothing
//  Outputs the names of processes which are currently in the "terminated" process state
void ProcessManager::PrintTerminated() const {
  cout << endl << "Completed:\t";
  for (vector<Process *>::const_iterator it = this->jobs_.begin();
    it != this->jobs_.end(); ++it) {
    if ((*it)->state() == "terminated") {
      cout.width(5);
      cout << std::left << (*it)->name();
    }
  }
  cout << endl << endl;
  cout << ".................................................." << endl;
}