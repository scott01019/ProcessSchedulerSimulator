CXX = g++
CXXFLAGS = -Wall -g -std=c++11

RunProcessSimulator: main.o fcfs_scheduler.o process.o process_manager.o mlfq_scheduler.o sjf_scheduler.o
	$(CXX) $(CXXFLAGS) -o RunProcessSimulator main.o fcfs_scheduler.o process.o process_manager.o mlfq_scheduler.o sjf_scheduler.o

main.o: main.cpp fcfs_scheduler.h mlfq_scheduler.h process.h process_manager.h sjf_scheduler.h
	$(CXX) $(CXXFLAGS) -c main.cpp

fcfs_scheduler.o: fcfs_scheduler.h process.h
	$(CXX) $(CXXFLAGS) -c fcfs_scheduler.cpp

mlfq_scheduler.o: mlfq_scheduler.h process.h
	$(CXX) $(CXXFLAGS) -c mlfq_scheduler.cpp

sjf_scheduler.o: sjf_scheduler.h
	$(CXX) $(CXXFLAGS) -c sjf_scheduler.cpp

process_manager.o: process_manager.h
	$(CXX) $(CXXFLAGS) -c process_manager.cpp

process.o: process.h
	$(CXX) $(CXXFLAGS) -c process.cpp
