#include "commons.h"
using namespace std;

Schedular::Schedular( std::vector<PCB> processes, double p_switching_time){
	initial_queue.swap(processes);
	switching_time= p_switching_time;
}

double Schedular::ART(){
	//cout<<"artname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	unsigned int i=0;
	unsigned int processes_number= finished_queue.size();
	double total_response_time=0;
	for(; i<processes_number; i++) total_response_time+= finished_queue[i].get_response_time();
	return total_response_time/ processes_number;
}
double Schedular::ATT(){
	//cout<<"attname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	unsigned int i=0;
	double total_turnaround_time=0;
	unsigned int processes_number=finished_queue.size();
	for(; i<processes_number; i++)
		total_turnaround_time+= finished_queue[i].get_waiting_time()+ finished_queue[i].get_burst_time_copy();
	return total_turnaround_time/ processes_number;
}
double Schedular::AWT(){
	//cout<<"awtname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	unsigned int i=0;
	double total_waiting_time=0;
	unsigned int processes_number= finished_queue.size();
	for(; i<processes_number; i++){
		total_waiting_time+= finished_queue[i].get_waiting_time();
	}
	return total_waiting_time/ processes_number;
}
double Schedular::cpu_utilization(){
	unsigned int i=0;
	double time= tasks_names.size();
	unsigned int used=0;
	for(; i<time; i++) if(tasks_names[i]!="_") used++;
	return (used/time)*100;

}
double Schedular::proportionality(){
	unsigned int i=0;
	double result=0;
	for(; i<finished_queue.size(); i++){
		double time_taken= finished_queue[i].get_finishing_time()- finished_queue[i].get_arrival_time();
		result+= ((finished_queue[i].get_burst_time_copy()>0)?time_taken/ finished_queue[i].get_burst_time_copy()
				: time_taken);
	}
	return result;
}
double Schedular::get_current_time(){
    return current_time;
}
unsigned int Schedular::get_num_processed_processes(){
    return finished_queue.size();
}
void Schedular::print_taskslist(){
	printf("<task>\t<start time>\t<end time>\t<duration>\n");
	for(unsigned int i=0; i<finished_queue.size(); i++){
		PCB process= finished_queue[i];
		double time_taken= process.get_finishing_time()- process.get_arrival_time();
		cout<<process.get_task_name()<<'\t'<<process.get_response_time()+ process.get_arrival_time()
			<<"\t\t"<<process.get_finishing_time()<<"\t\t"
			<<process.get_burst_time_copy()+process.get_waiting_time()<<endl;
	}
}
void Schedular::print_statistics(){
	print_timeline();
	printf("AWT:%f\tART:%f\tATT:%f\n", AWT(), ART(), ATT());
	printf("CPU UTILIZATION:%f %%\n", cpu_utilization());
}
void Schedular::print_processes_info(){
	if(!initial_queue.empty()){
		cout<<"initial queue:"<<endl;
		for(unsigned int i=0; i<initial_queue.size(); i++) cout<<initial_queue[i].get_info()<<"  ";
		cout<<endl;
	}

	if(!ready_queue.empty()){
		cout<<"ready queue:"<<endl;
		for(unsigned int i=0; i<ready_queue.size(); i++) cout<<ready_queue[i].get_info()<<"  ";
		cout<<endl;
	}

	if(!finished_queue.empty()){
		cout<<"finished queue:"<<endl;
		for(unsigned int i=0; i<finished_queue.size(); i++) cout<<finished_queue[i].get_info()<<"  ";
		cout<<endl;
	}

}
void Schedular::print_timeline(){
	for(unsigned int i=0; i<tasks_names.size(); i++) cout<<tasks_names[i]<<"  ";
	cout<<endl;
}

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
void Schedular::empty_processor(){
	processor_is_empty= true;
	running_process.state= State::baby;
}
void Schedular::remove_from_processor(){
	//cout<<"srename:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	//cout<<"removing"<<endl;
	if(running_process.state==State::running){
		ready_queue.push_back(running_process);
		ready_queue[ready_queue.size()-1].state= State::ready;
		ready_queue[ready_queue.size()-1].add_switching_time(switching_time);
	}
	else if(running_process.state==State::finished){
		//cout<<"pushing to finshed:"<<running_process.get_info()<<endl;
		running_process.set_finishing_time(current_time);
		finished_queue.push_back(running_process);
		//cout<<"rename:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	}
	empty_processor();
}
void Schedular::check_arrived_processes(){
	//cout<<"chename:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	//cout<<"checking"<<endl;
	for(unsigned int i=0; i<initial_queue.size(); i++)
		if(initial_queue[i].get_arrival_time()<= current_time){
			ready_queue.push_back(initial_queue[i]);
			initial_queue.erase(initial_queue.begin()+ i);
		}
}
void Schedular::tell_ready_processes_to_wait(){
	for(unsigned int i=0; i<ready_queue.size(); i++){
		ready_queue[i].wait();
		//cout<<"srname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	}
}

//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
SJF::SJF( std::vector<PCB> processes, double switching_time): Schedular(processes, switching_time){}
//@override
string SJF::get_policy_name(){ return "SJF";}
//@override
void SJF::preempt(){
	//cout<<"prname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	remove_from_processor();
	if(ready_queue.empty()) return;
	//cout<<"preempting"<<endl;
	running_process= get_SJ_process();
	//cout<<"impname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;

}

PCB SJF::get_SJ_process(){
	//cout<<"gsname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	unsigned int i=0, del=0;
	for(; i<ready_queue.size(); i++)
		if(ready_queue[i].get_burst_time()< ready_queue[del].get_burst_time()) del= i;
	PCB process= ready_queue[del];
	ready_queue.erase(ready_queue.begin()+ del);
	process.state= State::running;
	processor_is_empty= false;
	return process;
}
void SJF::run(){
	//cout<<"running"<<endl;
	//int x; cin>>x;
	//print_processes_info();
	//print_timeline();

	label:

	check_arrived_processes();
	if(processor_is_empty)preempt();

	if(running_process.state==State::running ){
		if(running_process.run()) tasks_names.push_back(running_process.get_task_name());
		else goto label;
	}
	current_time++;
	tell_ready_processes_to_wait();
	if( running_process.state== State::finished) preempt();

}

//@override
void SJF::start(){
	while(!initial_queue.empty() || !ready_queue.empty() || !processor_is_empty)
		run();
}


//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
SRTF::SRTF( std::vector<PCB> processes, double switching_time): Schedular(processes, switching_time){}
string SRTF::get_policy_name(){ return "SRTF";}
void SRTF::preempt(){
	//cout<<"prname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	remove_from_processor();
	if(ready_queue.empty()) return;
	//cout<<"preempting"<<endl;
	running_process= get_SRTF_process();
	//cout<<"impname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;

}

PCB SRTF::get_SRTF_process(){
	unsigned int i=0, del=0;
	for(; i<ready_queue.size(); i++)
		if(ready_queue[i].get_burst_time()< ready_queue[del].get_burst_time()) del= i;
	PCB process= ready_queue[del];
	ready_queue.erase(ready_queue.begin()+ del);
	process.state= State::running;
	processor_is_empty= false;
	return process;
}
void SRTF::run(){
	//cout<<"running"<<endl;
	//int x; cin>>x;
	//print_processes_info();
	//print_timeline();

	label:

	check_arrived_processes();
	preempt();

	if(running_process.state==State::running ){
		if(running_process.run()) tasks_names.push_back(running_process.get_task_name());
		else goto label;
	}else tasks_names.push_back("_");
	current_time++;
	tell_ready_processes_to_wait();
	if( running_process.state== State::finished) preempt();

}

void SRTF::start(){
	while(!initial_queue.empty() || !ready_queue.empty() || !processor_is_empty)
		run();
}


//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
FCFS::FCFS( std::vector<PCB> processes, double switching_time): Schedular(processes, switching_time){}
string FCFS::get_policy_name(){ return "FCFS";}
void FCFS::preempt(){
	//cout<<"prname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	remove_from_processor();
	if(ready_queue.empty()) return;
	//cout<<"preempting"<<endl;
	running_process= get_FCFS_process();
	//cout<<"impname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;

}

PCB FCFS::get_FCFS_process(){

	PCB process= ready_queue[0];
	ready_queue.erase(ready_queue.begin());
	process.state= State::running;
	processor_is_empty= false;
	return process;
}
void FCFS::run(){
	//cout<<"running"<<endl;
	//int x; cin>>x;
	//print_processes_info();
	//print_timeline();

	label:

	check_arrived_processes();
	if(processor_is_empty)preempt();

	if(running_process.state==State::running ){
		if(running_process.run()) tasks_names.push_back(running_process.get_task_name());
		else{
			remove_from_processor();
			goto label;
		}
	} else tasks_names.push_back("_");
	current_time++;
	tell_ready_processes_to_wait();
	if( running_process.state== State::finished) preempt();

}

void FCFS::start(){
	while(!initial_queue.empty() || !ready_queue.empty() || !processor_is_empty)
		run();
}

//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
RR::RR( std::vector<PCB> processes, unsigned int q, double switching_time): Schedular(processes, switching_time){quantum=q;}
string RR::get_policy_name(){ return "RR";}
void RR::preempt(){
	//cout<<"prname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	remove_from_processor();
	if(ready_queue.empty()) return;
	//cout<<"preempting"<<endl;
	running_process= get_RR_process();
	//cout<<"impname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;

}

PCB RR::get_RR_process(){

	PCB process= ready_queue[0];
	ready_queue.erase(ready_queue.begin());
	process.state= State::running;
	processor_is_empty= false;
	return process;
}
void RR::run(){
	static unsigned i=0;

	label:


	if(processor_is_empty ){check_arrived_processes();  preempt();}

	if(running_process.state==State::running ){
		if(running_process.run()) tasks_names.push_back(running_process.get_task_name());
		else{
			remove_from_processor();
			goto label;
		}
	} else tasks_names.push_back("_");
	current_time++;
	i++;
	tell_ready_processes_to_wait();
	check_arrived_processes();
	if( running_process.state== State::finished || i==quantum){i=0; remove_from_processor();}


}

void RR::start(){
	if(quantum<1){
		cout<<"qunatum can't be less than 1"<<endl;
		return;
	}
	while(!initial_queue.empty() || !ready_queue.empty() || !processor_is_empty)
		run();
}

void RR::set_quantum(unsigned int q){quantum= q;}


//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
//...............................................................................
RTEDF::RTEDF( std::vector<PCB> processes, double switching_time): Schedular(processes, switching_time){}
string RTEDF::get_policy_name(){ return "RTEDF";}
void RTEDF::preempt(){
	//cout<<"prname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;
	remove_from_processor();
	if(ready_queue.empty()) return;
	//cout<<"preempting"<<endl;
	running_process= get_RTEDF_process();
	//cout<<"impname:"<<running_process.get_task_name()<<"  w:"<<running_process.get_waiting_time()<<endl;

}

PCB RTEDF::get_RTEDF_process(){
	unsigned int i=0, del=0;
	for(; i<ready_queue.size(); i++)
		if(ready_queue[i].get_deadline()< ready_queue[del].get_deadline()) del= i;
	PCB process= ready_queue[del];
	ready_queue.erase(ready_queue.begin()+ del);
	process.state= State::running;
	processor_is_empty= false;
	return process;
}

void RTEDF::run(){
	//cout<<"running"<<endl;
	//int x; cin>>x;
	//print_processes_info();
	//print_timeline();

	label:

	check_arrived_processes();
	preempt();

	if(running_process.state==State::running ){
		if(running_process.run()) tasks_names.push_back(running_process.get_task_name());
		else goto label;
	}else tasks_names.push_back("_");
	current_time++;
	tell_ready_processes_to_wait();
	if( running_process.state== State::finished) preempt();

}

void RTEDF::start(){

	while(!initial_queue.empty() || !ready_queue.empty() || !processor_is_empty)
		run();
}
