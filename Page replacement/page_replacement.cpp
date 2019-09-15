
#include "page_replacement.h"


int* initialize_mem(int hd_size){
    int* ptr=(int*) calloc(sizeof(int), hd_size);
    return ptr;
}

int generate_address(int space){
//cout<<"generate address and done"<<endl;
    return rand()%space;
}

int get_page(int address, int source_space, int intries_size){
	return floor(((double) address/ source_space)* intries_size);

}

bool already_added(int p_n, vector<int> &generated_pages){
//cout<<"already added and done"<<endl;
    for(int i=0; i<generated_pages.size(); i++) if(generated_pages[i]== p_n) return true;
    return false;
}

int count_valid(MMU mmu){
	
	int result= 0;
	for(int i=0; i<mmu.entries.size(); i++) if(mmu.entries[i].valid) result++;
	return result;
}

void print_MMU(MMU mmu){
	for(int i=0; i<mmu.entries.size(); i++){
		Entry entry= mmu.entries[i];
		cout<<entry.target_frame<<"   "<<entry.valid<<endl;
	}
}

int least_recently_used(int address, Process* process){

	int target_main_frame= process->virtual_to_main_unit.choose_victim_lru();
	process->virtual_to_main_unit.set_entry(address, target_main_frame);
	return target_main_frame;
}

int fcfs(int address, Process* process){
	int target_main_frame= process->virtual_to_main_unit.choose_victim_fcfs();
	process->virtual_to_main_unit.set_entry(address, target_main_frame);
	return target_main_frame;
}

int least_frequently_used(int address, Process* process){
	int target_main_frame= process->virtual_to_main_unit.choose_victim_lfu();
	process->virtual_to_main_unit.set_entry(address, target_main_frame);
	return target_main_frame;	
}
Entry::Entry(int t_f, bool v){
//cout<<"entry adn done"<<endl;
    target_frame= t_f;
    valid= v;
	total_references=0;
	//last_reference= MMU::last_use++;
}

bool Entry::is_valid(){
	last_reference= MMU::last_use++;
	total_references++;
	return valid;
}

void Entry::set_valid(bool v){ valid= v;}
MMU::MMU(){}
MMU::MMU(int entries_size, int s_space, int t_size, bool fill_entries){
//cout<<"mmu"<<endl;
    source_space= s_space;
    target_size= t_size;
    if( fill_entries) {
        vector<int> generated_page_numbers;
        for(int i=0; i<entries_size; i++){
            int page_number= rand()% t_size;
            while(already_added(page_number, generated_page_numbers))
                page_number= rand()%t_size;
           entries.push_back(Entry(page_number, true));
        }
    }
    else for(int i=0; i<entries_size; i++) entries.push_back(Entry(-1, false));
    //cout<<"mmu done"<<endl;
}

int MMU::last_use= 0;
int MMU::arrive_time= 0;

int MMU::operator[](int address){
//cout<<"mmu operator"<<endl;
	//cout<<"add:"<<address<<"  sspace:"<<source_space<<endl;
    int page_reference= get_page(address, source_space, entries.size());
	//cout<<"page:"<<page_reference<<endl;
	//cout<<"page_r:"<<page_reference<<endl;
    if(page_reference>= entries.size()) return INVALID_REFERENCE;
	//cout<<"not an invalid refernece"<<endl;
    if(!entries[page_reference].valid) return PAGE_FAULT;
	//cout<<"mmu operator done"<<endl;
    return entries[page_reference].target_frame;
    ////cout<<"mmu operator done"<<endl;
}

int MMU::choose_victim_lru(){
	//if(entries.empty()){ cout<<"no intries"<<endl; return -1;}
	
	int least; int victim;
	bool empty= true;
	
	for(int i=1; i<entries.size(); i++) 
		if(entries[i].valid && (entries[i].last_reference< least || empty))
			{least= entries[i].last_reference; victim= i; empty= false;}
	entries[victim].valid= false;
	entries[victim].total_references= 0;
	return entries[victim].target_frame;
}

int MMU::choose_victim_lfu(){
	int least; int victim;
	bool empty= true;	
	for(int i=1; i<entries.size(); i++) 
		if(entries[i].valid && (entries[i].total_references< least || empty))
			{least= entries[i].total_references; victim= i; empty= false;}
	entries[victim].valid= false;
	entries[victim].total_references= 0;
	return entries[victim].target_frame;
}

int MMU::choose_victim_fcfs(){
	int least; int victim;
	bool empty= true;	
	for(int i=1; i<entries.size(); i++) 
		if(entries[i].valid && (entries[i].arrival_time< least || empty))
			{least= entries[i].arrival_time; victim= i; empty= false;}
	entries[victim].valid= false;
	entries[victim].total_references= 0;
	return entries[victim].target_frame;
}

void MMU::set_entry(int address, int assigned_page){
	int page_reference= get_page(address, source_space, entries.size());
	if(page_reference>= entries.size()) {cout<<"set_entry_reference_error"<<endl; return;}
	
	entries[page_reference].target_frame= assigned_page;
	entries[page_reference].valid= true;
	entries[page_reference].arrival_time= MMU::arrive_time++;
	entries[page_reference].last_reference= MMU::last_use++;
}

Process::Process(OS* Os, int p, int mmu_size,int vm_space, int mm_size){
//cout<<"process"<<endl;
	pid= p;
	os= Os;
    virtual_to_main_unit= MMU(mmu_size, vm_space, mm_size, false);
    virtual_memory_space= vm_space;
    MMU_size= mmu_size;
    main_memory_size= mm_size;
//cout<<"process"<<endl;
}

int Process::run(){
//cout<<"run"<<endl;
    int address= generate_address(virtual_memory_space);
    int response= virtual_to_main_unit[address];
	//cout<<"returned"<<endl;
    if(response== PAGE_FAULT || response== INVALID_REFERENCE) os->handle_error( response, address, pid);
   // else cout<<"thanks"<<endl;
//cout<<"run done"<<endl;
    return response;
}

int Process::run(int address){
	int response= virtual_to_main_unit[address];
	//cout<<"returned"<<endl;
    if(response== PAGE_FAULT || response== INVALID_REFERENCE) os->handle_error( response, address, pid);
    //else cout<<"thanks"<<endl;
	//cout<<"run done"<<endl;
    return response;
}
OS::OS(int mm_s, int (*replace_algorithm)(int, Process* process)){
//cout<<"os"<<endl;
    main_memory_size= mm_s;
	replace= replace_algorithm;
    initialize();
    //cout<<"os done"<<endl;
}

void OS::initialize(){
    //cout<<"0s_init"<<endl;
	
    for(int i=0; i<main_memory_size*100; i++) disk.push_back(i);
    for(int i=0; i<main_memory_size; i++) main_memory.push_back(-1);
    for(int i=0; i<main_memory_size; i++) free_frames.push_back(i);
    //cout<<"os_init done"<<endl;
}

int OS::create_process(int vm_size, int vm_space){
    //cout<<"create_process"<<endl;
	int pid= processes.size();
    MMU *mmu= new MMU(vm_size, vm_space, main_memory_size*100, true);
    Process *process= new Process(this, pid, vm_size, vm_space, main_memory_size);
    processes.push_back(process);
    virtual_to_disk_units.push_back(mmu);
    //cout<<"create_process done"<<endl;
    return pid;
}

void OS::handle_error(int response, int address, int pid){
	//cout<<"pid:"<<pid<<endl;
	//cout<<"size:"<<processes.size()<<endl;	Process *process= processes[pid];


    //cout<<"handle_page_fault"<<endl;
    if(response== PAGE_FAULT){
       // cout<<"address:"<<address<<"  caused a page_fault"<<endl;
		handle_page_fault(  address, pid);
    }else if(response== INVALID_REFERENCE){
       // cout<<"address:"<<address<<"  caused an INVALID ADDRESS"<<endl;
    }
}

void OS::handle_page_fault(int address, int pid){
	Process *process= processes[pid];
	int disk_page= (*virtual_to_disk_units[pid])[address];
	
	int free_page;
	if(!free_frames.empty()){
		free_page= free_frames[0];
		free_frames.erase(free_frames.begin());
		
		process->virtual_to_main_unit.set_entry(address, free_page);
	}
	else { free_page= replace( address, process);}
	
	main_memory[free_page]= disk_page;
	
}

void OS::show_main_memory(){
	for(int i=0; i<main_memory_size; i++)
		cout<<main_memory[i]<<"  ";
	
	cout<<endl;
}

Plotter::Plotter(string t, string f, int m, int s){
	title= t;
	size= s;
	mean=m;
	file_name= f;
	//for(int i=0; i<size; i++) graph.push_back(vector<int>(size, 0));
}

Plotter::~Plotter(){ }

void Plotter::plot(int destination){
	
	if(destination) freopen(file_name.c_str(), "w", stdout);
	for(int i=graph.size()-1; i>=0; i--){
		vector<int> current_row= graph[i];
		cout<<i<<(i>=100? "": (i<10? "  ": " "))<<"| ";
		for(int j= 0; j<current_row.size(); j++)
			if(!current_row[j]) cout<<"  "; else if(current_row[j]==1)cout<<"+ "; else cout<<"| "; 
		cout<<endl;
	}
	cout<<"    ";
	for(int i=0; i<size; i++) cout<<"___";
	cout<<endl; 
	cout<<"    ";
	for(int i=0; i<size; i++) cout<<i<<" ";
	cout<<endl;
}

void Plotter::add_point(int x, int y){
	if(x>= size) {cout<<"x:"<<x<<"  y:"<<y<<"   ";cout<<"out of range"<<endl; return;}
	//cout<<"x:"<<x<<"  y:"<<y<<endl;

	while(y>= graph.size()) {vector<int> newv(size, 0); graph.push_back(newv);}
	graph[y][x]= 1;
	
	for(int i= y-1; i>=0; i--) graph[i][x]= 2;
	
}

Poisson_wrapper::Poisson_wrapper(double m){
	mean= m;
	
	std::poisson_distribution<int> d(mean);
	distribution= d;
	
}

int Poisson_wrapper::get_number(int limit){
	double result= distribution(generator);
	int r= floor(result);
	if(r>= limit) return limit; else return r;
}