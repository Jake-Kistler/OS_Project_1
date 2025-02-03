#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

struct PCB
{
    int process_id = 0;
    int state = 0;
    int program_counter = 0;
    int instruction_base = 0;
    int data_base = 0;
    int memory_limit = 0;
    int cpu_cycles_used = 0;
    int register_value = 0;
    int max_memory_needed = 0;
    int main_memory_base = 0;
    queue<int> instructions;
    queue<int> data;
};

void load_jobs_to_memory(queue<PCB> &new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size);
void execute_CPU(int start_address, vector<int> &main_memory);
void print_main_memory(const vector<int> &main_memory);

int main()
{
    int max_memory_size, process_count;
    cin >> max_memory_size >> process_count;

    queue<PCB> new_job_queue;
    queue<int> ready_queue;
    vector<int> main_memory(max_memory_size, -1); // Initialize all memory to -1

    for (int i = 0; i < process_count; i++)
    {
        int processID, maxMemNeeded, numInstructions;
        cin >> processID >> maxMemNeeded >> numInstructions;

        PCB process;
        process.process_id = processID;
        process.max_memory_needed = maxMemNeeded;

        for (int j = 0; j < numInstructions; j++)
        {
            int op_code;
            cin >> op_code;
            process.instructions.push(op_code);

            if (op_code == 1) // Compute: 2 extra integers (iterations, CPU cycles)
            {
                int iterations, cycles;
                cin >> iterations >> cycles;
                process.data.push(iterations);
                process.data.push(cycles);
            }
            else if (op_code == 2) // Print: 1 extra integer (CPU cycles)
            {
                int cycles;
                cin >> cycles;
                process.data.push(cycles);
            }
            else if (op_code == 3) // Store: 2 extra integers (value, address)
            {
                int value, address;
                cin >> value >> address;
                process.data.push(value);
                process.data.push(address);
            }
            else if (op_code == 4) // Load: 1 extra integer (address)
            {
                int address;
                cin >> address;
                process.data.push(address);
            }
        }

        new_job_queue.push(process);
    }

    load_jobs_to_memory(new_job_queue, ready_queue, main_memory, max_memory_size);

    while (!ready_queue.empty())
    {
        int start_address = ready_queue.front();
        ready_queue.pop();
        execute_CPU(start_address, main_memory);
    }

    cout << "\nMain memory after execution:\n";
    print_main_memory(main_memory);

    return 0;
}

void load_jobs_to_memory(queue<PCB> &new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size)
{
    int current_address = 0; // Tracks where to store in main memory

    while (!new_job_queue.empty())
    {
        PCB process = new_job_queue.front();
        new_job_queue.pop();

        if (current_address + process.max_memory_needed > max_memory_size)
        {
            cout << "Not enough memory to load process: " << process.process_id << endl;
            continue;
        }

        process.state = 1;
        process.program_counter = 0;
        process.instruction_base = current_address + 10;
        process.data_base = process.instruction_base + process.instructions.size();
        process.memory_limit = process.max_memory_needed;
        process.cpu_cycles_used = 0;
        process.register_value = 0;
        process.main_memory_base = current_address;

        // **Store PCB in main memory**
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

        int instruction_start = process.instruction_base;
        int data_start = process.data_base;

        while (!process.instructions.empty())
        {
            main_memory[instruction_start++] = process.instructions.front();
            process.instructions.pop();
        }

        while (!process.data.empty())
        {
            main_memory[data_start++] = process.data.front();
            process.data.pop();
        }

        ready_queue.push(current_address);
        current_address += process.max_memory_needed;
    }
}

void execute_CPU(int start_address, vector<int> &main_memory)
{
    int process_id = main_memory[start_address];
    int &state = main_memory[start_address + 1];
    int &program_counter = main_memory[start_address + 2];
    int instruction_base = main_memory[start_address + 3];
    int data_base = main_memory[start_address + 4];

    while (program_counter < 10)
    {
        int instruction_address = instruction_base + program_counter;
        int instruction = main_memory[instruction_address];

        if (instruction == -1)
            break; // Stop at empty instruction slot

        switch (instruction)
        {
        case 1: // Compute
        {
            int iterations = main_memory[data_base++];
            int cycles = main_memory[data_base++];
            cout << "Executing Compute: " << iterations << " iterations, " << cycles << " cycles\n";
            break;
        }
        case 2: // Print
        {
            int cycles = main_memory[data_base++];
            cout << "Executing Print, " << cycles << " cycles\n";
            break;
        }
        case 3: // Store
        {
            int value = main_memory[data_base++];
            int logical_address = main_memory[data_base++];
            int physical_address = start_address + logical_address; // Convert logical to physical
            main_memory[physical_address] = value;
            cout << "Executing Store: Value " << value << " stored at logical address " << logical_address << "\n";
            break;
        }
        case 4: // Load
        {
            int address = main_memory[data_base++];
            cout << "Executing Load from logical address " << address << "\n";
            break;
        }
        default:
            cout << "Unknown instruction at memory address: " << instruction_address << "\n";
            return;
        }

        program_counter++;
    }

    state = 3;
    cout << "Process " << process_id << " completed successfully\n";
}

void print_main_memory(const vector<int> &main_memory)
{
    ofstream out("out.txt");
    if (!out)
    {
        cerr << "Can't open output file\n";
        return;
    }

    for (int i = 0; i < main_memory.size(); i++)
    {
        if (main_memory[i] != -1)
        {
            cout << i << " : " << main_memory[i] << "\n";
            out << i << "\t" << main_memory[i] << "\n";
        }
    }

    out.close();
    cout << "Main memory contents saved to out.txt\n";
}
