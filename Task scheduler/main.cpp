#include "commons.h"
using namespace std;

vector<PCB> processes;
vector<Schedular*> schedulars;


bool sort_for_AWT( Schedular* a, Schedular* b){ return a->AWT()< b->AWT();}
bool sort_for_ATT( Schedular* a, Schedular* b){ return a->ATT()< b->ATT();}
bool sort_for_ART( Schedular* a, Schedular* b){ return a->ART()< b->ART();}
bool sort_for_utilization( Schedular* a, Schedular* b){ return a->cpu_utilization()> b->cpu_utilization();}
bool sort_for_proportionality( Schedular* a, Schedular* b){ return a->proportionality()< b->proportionality();}

void initialize_schedulars(unsigned int quantum, double switching_time){
	schedulars.push_back(new RTEDF(processes, switching_time));
	schedulars.push_back(new SJF(processes, switching_time) );
	schedulars.push_back(new SRTF(processes, switching_time) );
	schedulars.push_back(new FCFS(processes, switching_time) );
	schedulars.push_back(new RR(processes, quantum, switching_time) );
}

void get_results(){
    freopen("output.out", "w", stdout);

	//for AWT:
	sort(schedulars.begin(), schedulars.end(), sort_for_AWT);
	printf("AWT:               ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->AWT()<<")   ";
	}
	cout<<endl;

	//for ART:
	sort(schedulars.begin(), schedulars.end(), sort_for_ART);
	printf("ART:               ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->ART()<<")   ";
	}
	cout<<endl;

	//for ATT:
	sort(schedulars.begin(), schedulars.end(), sort_for_ATT);
	printf("ATT:               ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->ATT()<<")   ";
	}
	cout<<endl;

	//for cpu utilization:
	sort(schedulars.begin(), schedulars.end(), sort_for_utilization);
	printf("CPU utilization:   ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->cpu_utilization()<<")   ";
	}
	cout<<endl;

	//for proportionality:
	sort(schedulars.begin(), schedulars.end(), sort_for_proportionality);
	printf("Proportionality:   ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->proportionality()<<")   ";
	}
    cout<<endl;
	//for throughput:
	sort(schedulars.begin(), schedulars.end(), sort_for_proportionality);
	printf("throughput:        ");
	for(unsigned int i=0; i<schedulars.size(); i++){
		cout<<schedulars[i]->get_policy_name()<<"("<<schedulars[i]->get_num_processed_processes()/schedulars[i]->get_current_time()<<")   ";
	}
	cout<<"\n\n\n\n";


    for(unsigned int i=0; i< schedulars.size(); i++){
        cout<<schedulars[i]->get_policy_name()<<":"<<endl;
        cout<<"timeline:";
        schedulars[i]->print_timeline();
        schedulars[i]->print_taskslist();
        cout<<endl<<endl;
    }

}
void start_schedulars(){
	for(unsigned int i=0; i<schedulars.size(); i++){

        schedulars[i]->start();
	}
}

void run_schedulars(){
	//for(int i=0; i<schedulars.size(); i++) schedulars[i].start();
}

bool is_number(const string &s){
	for(unsigned int i=0; i<s.length(); i++) if(!isdigit(s[i])) return false;
	return true;
}
bool is_double(const string &s){
	for(unsigned int i=0; i<s.length(); i++) if(!isdigit(s[i]) && s[i]!='.') return false;
	return true;
}

bool check_input_validity(int argc, char** argv){
	if(argc> 4) return false;
	ifstream file(argv[1]);
	if(!file) return false;
	file.close();
	if(!is_number(argv[2])) return false;
	return true;
}


bool space(char c){
	return c=='\t' || c=='\0' || c==' ';
}

bool extract_information(string line, string &process_name, unsigned int &arrival_time,
						unsigned int &burst_time, unsigned int &deadline){
	string temp="";
	if(line.empty()) return false;
	unsigned int i=0;

	while(space(line[i])){i++; if(i==line.length()) return false;}
	//cout<<"here:"<<i<<endl;
	while(!space(line[i])){process_name+=line[i]; i++; if(i==line.length()) return false;}
	//cout<<"again:"<<process_name<<endl;

	while(space(line[i])){i++; if(i==line.length()) return false;}
	//cout<<"here:"<<i<<endl;
	while(!space(line[i])){temp+=line[i]; i++; if(i==line.length()) return false;}
	arrival_time= atoi(temp.c_str());
	//cout<<"again:"<<atoi(temp.c_str())<<endl;
	temp="";

	while(space(line[i])){i++; if(i==line.length()) return false;}
	//cout<<"here:"<<i<<endl;
	while(!space(line[i])){temp+=line[i]; i++; if(i==line.length()) return false;}
	burst_time= atoi(temp.c_str());
	temp= "";

	while(space(line[i])){i++; if(i==line.length()) return false;}
	//cout<<"here:"<<i<<endl;
	while(!space(line[i])){temp+=line[i]; i++; if(i==line.length()) break;}
	deadline= atoi(temp.c_str());

	return true;
	//cout<<"again:"<<atoi(temp.c_str())<<endl;

//	while(space(line[i])){i++; if(i==line.length()) return false;}
//	while(!space(line[i])){
//		if(!isdigit(line[i])) return false;
//		else temp+= line[i];
//		if(i==line.length()) return false;
//	}
//	arrival_time= atoi(temp.c_str());
//	temp="";
//
//	while(space(line[i])){i++; if(i==line.length()) return false;}
//	while(!space(line[i])){
//		if(!isdigit(line[i])) return false;
//		else temp+= line[i];
//		if(i==line.length()) return false;
//	}
//	burst_time= atoi(temp.c_str());
//	return true;
}

bool fill_processes_list(const string& file_name){
	ifstream input_file(file_name);
	string line="";
	string process_name="";
	unsigned int arrival_time, burst_time, deadline;
	//int i=0;
	while(getline(input_file, line)){
		process_name="";
		if(extract_information(line, process_name, arrival_time, burst_time, deadline))
			processes.push_back(PCB(arrival_time, burst_time, process_name, deadline));
		else{
			cout<<"can't extract information"<<endl;
			return false;
		}
		//cout<<"process:"<<++i<<" added"<<endl;
	}
	return true;
}

bool initialize_processes_list(int argc, char** argv, long long &quantum, double &switching_time){
	//cout<<"initializing processes list"<<endl;
	if(!check_input_validity(argc, argv)) return false;
	quantum= atoll(argv[2]);
	if(argc==4){ if(is_double(argv[3]))switching_time= atof(argv[3]);  else return false;}

	if(!fill_processes_list(argv[1])) return false;
	//cout<<"processes list initialized"<<endl;
	return true;
}

int main(int argc, char **argv)
{
	long long quantum;
	double switching_time=0;
	bool initialized= initialize_processes_list( argc,  argv, quantum, switching_time);
	if(!initialized) return printf("a problem occured");

	initialize_schedulars(quantum, switching_time);
	start_schedulars();
	get_results();
}

//	PCB process1(1, 11,"a");
//	PCB process2(4, 3,"b");
//	vector<PCB> processes;
//	processes.push_back(process1);
//	processes.push_back(process2);
//	SJF schedular1(processes);
//	schedular1.print_processes_info();
//	schedular1.start();
//	schedular1.print_timeline();

