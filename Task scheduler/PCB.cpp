#include "commons.h"
using namespace std;

PCB::PCB(){
	state= State::baby;
}
PCB::PCB(unsigned int p_arrival_time, unsigned int p_burst_time, string p_task_name, unsigned int p_deadline){
	arrival_time= p_arrival_time;
	deadline= p_deadline;
	burst_time= p_burst_time;
	burst_time_copy= p_burst_time;
	task_name= p_task_name;
	PCB();
}
string PCB::get_info(){
	string result="";
	result+= get_task_name()+ "-";
	result+= to_string(get_arrival_time())+"-";
	result+= to_string(get_burst_time())+"-";
	result+= to_string(get_deadline());
	return result;
	
}
unsigned int PCB::get_arrival_time(){return arrival_time;}
unsigned int PCB::get_burst_time(){return burst_time;}
unsigned int PCB::get_burst_time_copy(){ return burst_time_copy;}
double PCB::get_response_time(){return response_time;}
double PCB::get_waiting_time(){return waiting_time;}
unsigned int PCB::get_deadline(){return deadline;}
double PCB::get_finishing_time(){return finishing_time;}
void PCB::set_finishing_time(double time){ finishing_time+= time+ accumulated_switching_time;}
void PCB::set_response_time(double t){response_time= t;}
string PCB::get_task_name(){
	//if(state== State::baby || state== State::finished) return "_";
	return task_name;
}
bool PCB::done(){ return burst_time==0;}
bool PCB::run(){
	if(!burst_time){ state= State::finished; return false;}
	
	if(!--burst_time) state= State::finished;
	return true;
}
void PCB::wait(){
	if(state== State::baby) response_time++ ;
	waiting_time++;
}

void PCB::add_switching_time(double p_switching_time){
	if(state== State::baby) response_time+= p_switching_time;
	waiting_time+= p_switching_time;
	accumulated_switching_time+= p_switching_time;
}

void PCB::add_accumulated_switching_time_to_finishing_time(){
	finishing_time+= accumulated_switching_time;
}