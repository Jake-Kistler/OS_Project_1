#include<iostream>
#include <queue>
#include <vector>

using namespace std;

struct PCB
{
    int process_id = 0;             // process id
    int state = 0;                  // state as an integer
    int program_counter = 0;        // index of next instruction
    int instruction_base = 0;       // starting address of the instruction
    int data_base = 0;              // starting address of data section of the instruction
    int memory_limit = 0;           // limit of memory used by the instruction
    int cpu_cycles_used = 0;        // total number of CPU cycles used by the instruction
    int register_value = 0;         // value to store for read/write operations
    int max_memory_needed = 0;      // maximum memory required
    int main_memory_base = 0;       // starting address of the main memory
    

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


    for (int i = 0; i < process_count; i++)
    {
        int processID, maxMemNeeded, numInstructions;
        cin >> processID >> maxMemNeeded >> numInstructions;

        // For this example we focus on the PCB. In a complete implementation,
        // you would read the instructions and store them in main_memory after the PCB.
        for (int j = 0; j < numInstructions; j++)
        {
            int instruction;
            cin >> instruction; // Read and later store/process the instruction.
        }

        // Create a PCB instance for the process.
        PCB process;
        process.process_id = processID;
        process.max_memory_needed = maxMemNeeded;
        // The remaining PCB fields will be initialized in load_jobs_to_memory.
        new_job_queue.push(process);
    }

   
    // load jobs into main memory and populate the ready queue
    load_jobs_to_memory(new_job_queue, ready_queue, main_memory, max_memory_size);

    

    


}

void load_jobs_to_memory(queue <PCB> new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size)
{
    int current_address = 0; // will act as a tracker for the location in main_memory

    while(!new_job_queue.empty())
    {
        PCB process = new_job_queue.front(); // grab the first PCB off the new_job_queue
        new_job_queue.pop(); // pop the queue

        // Check if we have enough space to load the process into main memory
        if(current_address + process.max_memory_needed > max_memory_size)
        {
            cout << "Not enough memory to load: " << process.process_id << endl;
            continue;
        }

        // walk across the PCB loading it's data member by member into sequential memory locations

        main_memory[current_address + 0] = process.process_id;
        main_memory[current_address + 1] = process.state;
        main_memory[current_address + 2] = process.program_counter;
        main_memory[current_address + 3] = process.instruction_base;
        main_memory[current_address + 4] = process.data_base;
        main_memory[current_address + 5] = process.memory_limit;
        main_memory[current_address + 6] = process.cpu_cycles_used;
        main_memory[current_address + 7] = process.register_value;
        main_memory[current_address + 8] = process.max_memory_needed;
        main_memory[current_address + 9] = process.main_memory_base;

        // push to the ready queue
        ready_queue.push(current_address);

        // move the current address forward by the total memory for the current process
        current_address += process.max_memory_needed;

        // print the PCB for testing purposes
        print_PCB(&process);
    }

}

void print_PCB(const PCB *process_control_board)
{
    if(process_control_board == NULL)
    {
        cout << "PCB is empty!\n";
        return;
    }

    cout << "Process ID: " << process_control_board->process_id << "\n"
         << "State: " << process_control_board->state << "\n"
         << "Program Counter: " << process_control_board->program_counter << "\n"
         << "Instruction Base: " << process_control_board->instruction_base << "\n"
         << "Data Base: " << process_control_board->data_base << "\n"
         << "Memory Limit: " << process_control_board->memory_limit << "\n"
         << "CPU Cycles Used: " << process_control_board->cpu_cycles_used << "\n"
         << "Register Value: " << process_control_board->register_value << "\n"
         << "Max Memory Needed: " <<  process_control_board->max_memory_needed << "\n"
         << "Main Memory Base: " << process_control_board->main_memory_base << "\n";
}
   