#ifndef Schedularw
#define Schedularw
#include "commons.h"
#endif

class Schedular{
protected:
	double current_time=0;
	double switching_time;
	std::vector<std::string> tasks_names;
	std::vector<PCB> initial_queue;
	std::vector<PCB> finished_queue;
	std::vector<PCB> ready_queue;
	void empty_processor();
	PCB running_process;
	virtual void preempt()=0;
	bool processor_is_empty= true;
public:
	Schedular( std::vector<PCB> processes, double switching_time);
	void remove_from_processor();
	void check_arrived_processes();
	void print_timeline();
	void print_taskslist();
	double AWT();
	double ART();
	double ATT();
	//unsigned int throughput();
	double cpu_utilization();
	double proportionality();
	double get_current_time();
	void print_statistics();
	void print_processes_info();
	void tell_ready_processes_to_wait();
    unsigned int get_num_processed_processes();
	virtual std::string get_policy_name()=0;
	virtual void start()=0;
};

class SJF: public Schedular{
private:
	void preempt();
	PCB get_SJ_process();
	void run();
public:
	SJF( std::vector<PCB> processes, double switching_time);
	std::string get_policy_name();
	void start();
};

class SRTF: public Schedular{
private:
	void preempt();
	PCB get_SRTF_process();
	void run();
public:
	SRTF( std::vector<PCB> processes, double switching_time);
	std::string get_policy_name();
	void start();
};

class FCFS: public Schedular{
private:
	void preempt();
	PCB get_FCFS_process();
	void run();
public:
	FCFS( std::vector<PCB> processes, double switching_time);
	std::string get_policy_name();
	void start();
};

class RR: public Schedular{
private:
	void preempt();
	PCB get_RR_process();
	void run();
	unsigned int quantum=0;
public:
	RR( std::vector<PCB> processes, unsigned int q, double switching_time);
	std::string get_policy_name();
	void set_quantum(unsigned int q);
	void start();
};

class RTEDF: public Schedular{
private:
	void preempt();
	PCB get_RTEDF_process();
	void run();
public:
	RTEDF( std::vector<PCB> processes, double switching_time);
	std::string get_policy_name();
	void start();
};
