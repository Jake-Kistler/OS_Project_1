#include<iostream>
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

    // define and read in the first two lines from the file

    queue<PCB> new_job_queue;
    queue<int> ready_queue;

    // both queues defined

    vector<int> main_memory;
    main_memory.resize(max_memory_size, -1); // all elements in main_memory are -1

    for(int i = 0; i < process_count; i++)
    {
        PCB process;

        cin >> process.process_id 
        >> process.max_memory_needed 
        >> process.number_of_instructions_for_process 
        >> process.number_of_iterations 
        >> process.CPU_Cycles_used 
        >> process.memory_needed_for_operation;

        process.system_memory_counter = 0;
        process.state = 0; // Defaulting to new

        new_job_queue.push(process);
    }

    // load jobs into main memory and populate the ready queue
    load_jobs_to_memory(new_job_queue, ready_queue, main_memory, max_memory_size);

    


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
   