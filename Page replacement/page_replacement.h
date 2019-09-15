
#include <bits/stdc++.h>
#include <time.h>
using namespace std;
#define INVALID_REFERENCE -2
#define PAGE_FAULT -1

class OS;
class Process;
class MMU;

int generate_address(int space);
int get_page(int, int, int);
int poisson_distribution();

int least_recently_used(int, Process*);
int least_frequently_used(int, Process*);
int fcfs(int , Process*);

int count_valid(MMU mmu);
void print_MMU(MMU);

class Entry{
public:
    int target_frame;
    bool valid;
	int last_reference;
	int arrival_time;
	int total_references;
	
    Entry(int t_f, bool v);
	bool is_valid();
	void set_valid(bool v);
};

class MMU{
public:
    vector<Entry> entries;
    int source_space, target_size;
	
	static int last_use;
	static int arrive_time;

    MMU();
    MMU(int entries_size, int s_space, int t_ssize, bool fill_entries);
    int operator[](int address);
	void set_entry(int address, int assigned_page);
	
	int choose_victim_lru();
	int choose_victim_lfu();
	int choose_victim_fcfs();
};

class Process{
public:
	int pid;
    MMU virtual_to_main_unit;
    OS* os;
    int virtual_memory_space;
    int MMU_size;
    int main_memory_size;

    Process(OS* Os, int pid, int mmu_size, int vm_space, int mm_size);
    int run();
	int run(int address);
};

class OS{
public:
    vector<int> main_memory, free_frames, disk;
	int main_memory_size;
    vector<MMU*> virtual_to_disk_units;
    vector<Process*> processes;
	int (*replace)( int, Process*);

    OS(int mm_s, int (*replace_algorithm)(int, Process*));
    void initialize();
    int create_process(int vm_size, int vm_space);
    void handle_page_fault( int address, int pid);
    void handle_error(int response, int address, int pid);
	void show_main_memory();
};

class Plotter{
	string file_name;
	int size;
	int max_value;
	int mean;
	vector< vector<int> > graph;
	string title;
public:
	Plotter(string t, string f, int m, int s= 100  );
	void plot(int destination=0);
	void add_point(int x, int y);
	~Plotter();
};


class Poisson_wrapper{
public:
	double mean;
	std::default_random_engine generator;
	std::poisson_distribution<int> distribution;
	Poisson_wrapper(double m);
	int get_number(int limit);
};