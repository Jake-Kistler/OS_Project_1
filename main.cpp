#include<iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

struct PCB
{
    int process_id;                                     // ID for the process
    int state;                                          // the state (encodeing)
    int max_memory_needed;                              // memory needed for the process
    int number_of_instructions_for_process;             // Number of instructions in the process
    int number_of_iterations;                           // number of iterations each instruction will take
    int CPU_Cycles_used;                                // Number of cpu cycles used for the operation
    int value_to_store;                                 // only needed in a load/store operation
    int memory_needed_for_operation;                    // Max memory needed for the operation to be done
    int system_memory_counter;                          // tracks how much memory used from main
    

};

void load_jobs_to_memory(queue <PCB> new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size);

void execute_CPU(int start_address, int *main_memory);

void print_PCB(const PCB *process_control_board);

int main(int argc, char **argv)
{

    int max_memory_size, process_count;
    cin >> max_memory_size;
    cin >> process_count;

    queue<PCB> new_job_queue;
    queue<int> ready_queue;
    int *main_memory = new int[max_memory_size];

    for(int i = 0; i < process_count; i++)
    {
        PCB process;
        cin >> process.process_id >> process.max_memory_needed >> process.number_of_instructions_for_process >> process.number_of_iterations 
        >> process.CPU_Cycles_used >> process.memory_needed_for_operation;
    }

    


}

void load_jobs_to_memory(queue <PCB> new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size)
{


}

void print_PCB(const PCB *process_control_board)
{
    if(process_control_board == NULL)
    {
        cout << "PCB is empty!\n";
        return;
    }
}
   