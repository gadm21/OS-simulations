#ifndef PCBw
#define PCBw
#include "commons.h"
#endif

enum class State { running, ready, baby, finished};

class PCB{
private:
	unsigned int arrival_time=0, burst_time=0, burst_time_copy=0, deadline=0;
	double waiting_time=0, response_time=0, finishing_time=0, accumulated_switching_time=0;
	std::string task_name="";
public:
	PCB();
	PCB(unsigned int p_arrival_time, unsigned int p_burst_time, std::string p_task_name, unsigned int p_deadline);
	State state= State::baby;
	unsigned int get_arrival_time();
	unsigned int get_burst_time();
	unsigned int get_deadline();
	unsigned int get_burst_time_copy();
	double get_waiting_time();
	double get_response_time();
	void add_accumulated_switching_time_to_finishing_time();
	void add_switching_time(double switching_time=0);
	double get_finishing_time();
	void set_finishing_time(double time);
	void set_response_time(double t);
	std::string get_info();
	std::string get_task_name();
	bool run();
	void wait();
	bool done();
};