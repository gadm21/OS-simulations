#include <bits/stdc++.h>
using namespace std;

int process_num, resources_num;
vector<int> available_resources;


string get_available(){
    string result="";
    for(int i=0;i<resources_num; i++)
        result+= to_string(available_resources[i])+(i== resources_num-1?" ":",");
    return result;
}
int get_random(int s, int e){
int range= s-e;
int random=s+ rand()%range;
return random;
}
void print_vector(vector<int> v){
    cout<<endl<<"............................................................"<<endl;
    for(int i=0; i<v.size(); i++){
        cout<<v[i]<<" ";
    }
    cout<<endl<<"............................................................"<<endl;
}
class Process{
public:
    static int global_id;
    int id;
    vector<int> allocated, maxx, needed;

    Process(int Pid):id(Pid){ ++global_id;}
    Process(){ id= ++global_id;}
    string get_allocatedd(){
        string result="";
        for(int i=0; i<allocated.size(); i++)
            result+=to_string(allocated[i])+(i== allocated.size()-1?" ":",");
        return result;
    }
    string get_maxx(){
        string result="";
        for(int i=0; i<maxx.size(); i++)
            result+=to_string(maxx[i])+(i== maxx.size()-1?" ":",");
        return result;
    }
    string get_needed(){
        string result="";
        for(int i=0; i<needed.size(); i++)
            result+=to_string(needed[i])+(i== needed.size()-1?" ":",");
        return result;
    }
    void calculate_needed(){
        for(int i=0; i<allocated.size(); i++){
            needed.push_back( maxx[i]- allocated[i]);
        }
    }

};
int Process::global_id=0;
vector<Process> processes;

void generate_processes_and_resources(unsigned int seed=100){
    srand(seed);
    for(int i=0; i<resources_num; i++)
        available_resources.push_back(get_random(20, 27));

    for(int i=0; i<process_num; i++){
        Process process(i+1);
        for(int j=0; j<resources_num; j++){
            process.allocated.push_back(get_random(0, available_resources[j]/4));
            process.maxx.push_back(get_random(process.allocated[j], 15));
            available_resources[j]-= process.allocated[j];
        }
        process.calculate_needed();
        processes.push_back(process);
    }
}
void get_processes_and_resources_manually(){
    int resource=0;
    for(int i=0; i<resources_num; i++){
        cout<<"number of instances of resource type ("<<i+1<<") : "; cin>>resource;
        available_resources.push_back(resource);
    }
    cout<<"don't exceed total system resources instances when allocating to processes !"<<endl;
    for(int i=0; i<process_num; i++){
        cout<<endl<<endl<<"for process:"<<i+1<<endl;
        Process process;
        vector<int> maxx, allocted;
        int input= 0;
        for(int j=0; j<resources_num; j++){
            do{cout<<"maximum instances that will be requested of resource type ("<<j+1<<") : "; cin>>input; } while(input> available_resources[j]);
            maxx.push_back(input);
            do{cout<<"instances initially allocated of resource type ("<<j+1<<") : "; cin>>input; } while(input> maxx[j]);
            allocted.push_back(input);
            available_resources[j]-= input;
        }
        process.allocated= allocted;
        process.maxx= maxx;
        process.calculate_needed();
        processes.push_back(process);
    }
}

void get_input_1(){
cout<<"number of processes:"; cin>>process_num;
cout<<"number of resources:"; cin>>resources_num;
int input=0;
while(input!= 1 && input!= 2){
    cout<<"press 1 to generate randomly or 2 to enter processes and resources manually: ";
    cin>>input;
}
if(input==1){
    int seed =0;
    cout<<"enter a seed: "; cin>>seed;
    generate_processes_and_resources(seed);
}
else get_processes_and_resources_manually();
}


void print_table(){
cout<<"p_id   allocated   \tneeded   \tmax\t\tavailable"<<endl;
cout<<"......................................................"<<endl;
for(int i=0; i<process_num; i++){
        cout<<processes[i].id<<"        "<<processes[i].get_allocatedd()<<"         "<<processes[i].get_needed()<<
            "         "<<processes[i].get_maxx()<<"       "<<(i==0? get_available():" ")<<endl;

    }
    cout<<"....................................................."<<endl;
    cout<<"....................................................."<<endl;
    cout<<endl;
}
void print_processes(vector<Process> processes){
    cout<<endl<<"............................................................"<<endl;
    cout<<" a safe sequence is: < ";
    for(int i=0; i<processes.size(); i++){
        cout<<processes[i].id<<(i==processes.size()-1?"":", ");
    }
    cout<<"> ";
    cout<<endl<<"............................................................"<<endl;
}

void print_allocation_is_impossible(){
    cout<<endl<<"............................................................"<<endl;
    cout<<"allocation is impossible";
    cout<<endl<<"............................................................"<<endl;
}
void print_unsafe_region_will_happen(){
    cout<<endl<<"............................................................"<<endl;
    cout<<"this allocation will cause unsafe state";
    cout<<endl<<"............................................................"<<endl;
}


bool can_request(Process process, const vector<int> &available_resources){
    //cout<<"can_request::"<<endl;
    for(int i=0; i<available_resources.size(); i++){
        if(process.needed[i]> available_resources[i]){ return false;}
    }
    //cout<<"return true;"<<endl;
    return true;
}
void apply_request(const Process& process, vector<int> &available_resources){
    //cout<<"apply_request::"<<endl;
    for(int i=0; i<available_resources.size(); i++)
        available_resources[i]+= process.allocated[i];
}
void return_resources(const Process& process, vector<int> &available_resources){
    //cout<<"return_request::"<<endl;
    for(int i=0; i<available_resources.size(); i++)
        available_resources[i]-= (process.maxx[i]- process.needed[i]);
}

bool check_mode (vector<Process> processes1, vector<Process> &processes2, vector<int> available_resources){
    //cout<<"check_mode::"<<endl;
    //print_vector(available_resources);
    if(processes1.empty()) return true;

    for(int i=0; i<processes1.size(); i++){
        Process process= processes1[i];
        if(!can_request(process, available_resources)) continue;
        else apply_request(process, available_resources);
        processes1.erase(processes1.begin()+i);
        if(!check_mode(processes1, processes2, available_resources)){
            processes1.insert(processes1.begin()+i,  process);
            return_resources(process, available_resources);
        }
        else {processes2.insert(processes2.begin(),  process);  return true;}
    }
    return false;
}
void print_safe_sequence(){
    vector<Process> safe_sequence;
    if(check_mode(processes, safe_sequence, available_resources)) print_processes(safe_sequence);
    else print_allocation_is_impossible();
}

void handle_request(vector<Process> &processes, int id, vector<int> &available_resources, vector<int> &request){
    Process process= processes[id-1];
    int counter=0;
    for(int i=0; i<resources_num; i++){
        available_resources[i]-= request[i];
        process.needed[i]-= request[i];
        process.allocated[i]+= request[i];
        if(!process.needed[i]) counter++;
    }
    if(counter== process_num) return_resources(process, available_resources);
    processes[id-1]= process;
}
bool check_safety(vector<Process> processes, int id, vector<int> available_resources, vector<int> request){
    handle_request(processes, id, available_resources, request);
    vector<Process> processes2;
    if(check_mode(processes,processes2, available_resources)) return true;
    else return false;
}
void get_request(){
        int id=0; vector<int> request;

        do {cout<<"process id, (from1, to "<<process_num<<") "<<": "; cin>>id;}
        while( id> process_num);
        Process process= processes[id-1];
        cout<<"don't exceed available resources or needed resources of the process"<<endl;
        for(int i=0; i<resources_num; i++){
            int x=0;
            int trial=1;
            do{
                cout<<(trial>1? "available resources and/or your remaining limit are exceeded, try again!\n":"");
                cout<<"Instances to allocate of resource type "<<i+1<<", (available="<<available_resources[i]<<") "<<
                ", (remaining="<<process.needed[i]<<") "<<" : "; cin>>x;
                trial++;

            }while(available_resources[i]< x || process.needed[i]< x);
            request.push_back(x);
        }
        if(check_safety(processes, id, available_resources, request))
            handle_request(processes, id, available_resources, request);
        else print_unsafe_region_will_happen();
}
void get_input_2(int &input){
    cout<<" 0 to exit\n 1 to request an allocation\n 2 to get the safe sequence of execution\n 3 to print the resources allocation table \n"; cin>>input;
    if(input==1){
        get_request();
    }else if(input==2){
        print_safe_sequence();
    } else if(input==3) print_table();

}
int main(){
    get_input_1();

    print_table();

    int input=1;
    do{get_input_2(input);} while(input);
}
