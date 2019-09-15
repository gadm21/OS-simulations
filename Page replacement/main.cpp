
#include "page_replacement.h"

// use srand
int main(){
	
	int main_memory_size; cout<<"main_memory_size:"; cin>>main_memory_size;
	int virtual_address_space;  cout<<"virtual_address_space:"; cin>>virtual_address_space;
	int lookup_table_size; cout<<"lookup_table_size:"; cin>>lookup_table_size;
	double poisson_distribution_mean; cout<<"poisson_distribution_mean:"; cin>>poisson_distribution_mean;
	
	Poisson_wrapper p(poisson_distribution_mean);
	
	OS os(main_memory_size, least_frequently_used);
	int p_id= os.create_process(lookup_table_size, virtual_address_space);
	
	int period= poisson_distribution_mean*5;
	vector<int> page_faults(period, 0);
	
	for(int i=0; i<1000; i++){
		int num= p.get_number(period-1);
		int response= os.processes[p_id]->run();
		if(response== PAGE_FAULT) page_faults[num]++;
	}
	
	Plotter plotter("th", "output.txt", 2, period);
	for(int i=0; i<period; i++) plotter.add_point(i, page_faults[i]);
	plotter.plot(0);

}
