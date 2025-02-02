#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct PCB
{
    int process_id = 0;        // process id
    int state = 0;             // state as an integer
    int program_counter = 0;   // index of next instruction
    int instruction_base = 0;  // starting address of the instruction
    int data_base = 0;         // starting address of data section of the instruction
    int memory_limit = 0;      // limit of memory used by the instruction
    int cpu_cycles_used = 0;   // total number of CPU cycles used by the instruction
    int register_value = 0;    // value to store for read/write operations
    int max_memory_needed = 0; // maximum memory required
    int main_memory_base = 0;  // starting address of the main memory
};

void load_jobs_to_memory(queue<PCB> &new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size);

void execute_CPU(int start_address, int *main_memory);

void print_PCB(const PCB *process_control_board);

void print_main_memory(const vector<int> &main_memory);

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

        PCB process;
        process.process_id = processID;
        process.max_memory_needed = maxMemNeeded;

        for(int j = 0; j < numInstructions; j++)
        {
            int op_code;
            cin >> op_code;

            if(op_code == 1) // compute bring in 2 integers
            {
                int iterations, cycles;
                cin >> iterations >> cycles;
            }
            else if(op_code == 2) // print, bring in 1 integer
            {
                int cycles;
                cin >> cycles;
            }
            else if(op_code == 3) // store, bring in 2 integers
            {
                int value, address;
                cin >> value >> address;
            }
            else if(op_code == 4) // load bring in 1 integer
            {
                int address;
                cin >> address;
            }
            else
            {
                cerr << "Invalid opcode " << op_code << endl;
            }
        }

        new_job_queue.push(process); // push the process onto the queue
    }

    // load jobs into main memory and populate the ready queue
    load_jobs_to_memory(new_job_queue, ready_queue, main_memory, max_memory_size);

    while(!ready_queue.empty())
    {
        int start_address = ready_queue.front();
        ready_queue.pop();
        execute_CPU(start_address, main_memory.data());
    }

    cout << "\nMain memory after execution:\n";
    print_main_memory(main_memory);

    return 0;
}

void load_jobs_to_memory(queue<PCB> &new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size)
{
    int current_address = 0; // will act as a tracker for the location in main_memory

    while (!new_job_queue.empty())
    {
        PCB process = new_job_queue.front(); // grab the first PCB off the new_job_queue
        new_job_queue.pop();                 // pop the queue

        // Check if we have enough space to load the process into main memory
        if (current_address + process.max_memory_needed > max_memory_size)
        {
            cerr << "Not enough memory to load: " << process.process_id << endl;
            continue;
        }

        // Initialize all missing PCB values now
        process.state = 0; // this is encoded for NEW
        process.program_counter = 0;
        process.instruction_base = current_address + 10; // this is the size of the PCB in memory
        process.data_base = process.instruction_base + 1; // this is a placeholder for when we try to load into main memory
        process.memory_limit = process.max_memory_needed;
        process.cpu_cycles_used = 0;
        process.register_value = 0;
        process.main_memory_base = current_address;

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

void execute_CPU(int start_address, int *main_memory)
{
    cout << "\nExecuting process at address: " << start_address << "\n";

    int process_id = main_memory[start_address];
    int &state = main_memory[start_address + 1];
    int &program_counter = main_memory[start_address + 2];
    int &cpu_cycles_used = main_memory[start_address + 3];

    while(program_counter < 3)
    {
        int instruction = main_memory[start_address + 10 + program_counter];

        switch(instruction)
        {
            case 1: // compute
                cout << "Executing Compute\n";
                cpu_cycles_used += 5;
                break;
            case 2: // print
                cout << "Executing Print\n";
                break;
            case 3: // store
                cout << "Executing Store\n";
                break;
            case 4: // load
                cout << "Executing Load\n";
                break;
            default:
                cout << "Unknown instruction\n";
        }

        program_counter++;
    }

    // update process state to TERMINATED
    state = 3;
    cout << "Process " << process_id << " completed successfully\n";
}

void print_PCB(const PCB *process_control_board)
{
    if (process_control_board == NULL)
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
         << "Max Memory Needed: " << process_control_board->max_memory_needed << "\n"
         << "Main Memory Base: " << process_control_board->main_memory_base << "\n\n";
}

void print_main_memory(const vector<int> &main_memory)
{
    cout << "Address\tContent\n";
    for(int i = 0; i < main_memory.size(); i++)
    {
        cout << i << "\t" << main_memory[i] << "\n";
    }
}
