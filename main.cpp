#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm> // for fill()
#include <string>
using namespace std;

// ----------------------------------------------------------------------
// PCB structure in memory (indices for reference in memory):
//
//  memory[mBase + 0]:  process_id
//  memory[mBase + 1]:  state        (1=NEW,2=READY,3=RUNNING,4=TERMINATED,...)
//  memory[mBase + 2]:  programCounter
//  memory[mBase + 3]:  num_of_instructions
//  memory[mBase + 4]:  placeholder or instructionBase (in a more advanced design)
//  memory[mBase + 5]:  max_memory_needed
//  memory[mBase + 6]:  cpuCyclesUsed
//  memory[mBase + 7]:  registerValue
//  memory[mBase + 8]:  maxMemoryNeeded (or dataBase, etc., in a more advanced design)
//  memory[mBase + 9]:  mainMemoryBase or placeholder
// ----------------------------------------------------------------------

struct PCB
{
    int process_id;
    int state; // 1=NEW
    int max_memory_needed;
    int num_of_instructions;
    vector<tuple<int, vector<int>>> instructions;
};

// ----------------------------------------------------------------------
// Write all processes to memory
// ----------------------------------------------------------------------
void writeToMemory(vector<int> &main_memory, const vector<PCB> &processes, int main_memory_size)
{
    fill(main_memory.begin(), main_memory.end(), -1); // Initialize to -1

    int memory_index = 0;
    for (auto &process : processes)
    {
        // The same formula you used:
        // 10 for the "PCB region" + num_of_instructions + 2*(num_of_instructions) for parameters
        // but in your code you used: process.instructions.size() * 2.
        // We'll keep it as in your original snippet:
        int required_memory = 10 + process.num_of_instructions + process.instructions.size() * 2;

        if (memory_index + required_memory > main_memory_size)
        {
            cerr << "Error: Not enough memory to allocate process "
                 << process.process_id << endl;
            continue;
        }

        // (1) PCB region
        main_memory[memory_index + 0] = process.process_id;
        main_memory[memory_index + 1] = process.state; // default=1=NEW
        main_memory[memory_index + 2] = 0;             // programCounter
        main_memory[memory_index + 3] = process.num_of_instructions;
        main_memory[memory_index + 4] = 13; // placeholder
        main_memory[memory_index + 5] = process.max_memory_needed;
        main_memory[memory_index + 6] = 0; // cpuCyclesUsed
        main_memory[memory_index + 7] = 0; // registerValue
        main_memory[memory_index + 8] = process.max_memory_needed;
        main_memory[memory_index + 9] = memory_index; // let's store mainMemoryBase here

        // (2) Write instruction opcodes
        int instr_base = memory_index + 10;
        // We'll define that the "data section" starts right after the opcodes
        int data_base = instr_base + process.num_of_instructions;

        for (auto &instr : process.instructions)
        {
            int opcode = get<0>(instr);
            main_memory[instr_base++] = opcode;
        }

        // (3) Write instruction parameters
        for (auto &instr : process.instructions)
        {
            auto &params = get<1>(instr);
            for (auto param : params)
            {
                main_memory[data_base++] = param;
            }
        }

        // advance memory_index
        memory_index += required_memory;
    }
}

// ----------------------------------------------------------------------
// Print the entire main memory array for debugging
// ----------------------------------------------------------------------
void printMemory(const vector<int> &main_memory)
{
    cout << "Main Memory:\n";
    for (size_t i = 0; i < main_memory.size(); ++i)
    {
        cout << i << ": " << main_memory[i] << "\n";
    }
}

// ----------------------------------------------------------------------
// A helper to safely read from main memory or detect out-of-bounds
// ----------------------------------------------------------------------
bool safeLoad(const vector<int> &memory, int address, int &outVal)
{
    if (address < 0 || address >= (int)memory.size())
    {
        return false; // out-of-bounds
    }
    outVal = memory[address];
    return true;
}

// ----------------------------------------------------------------------
// A helper to safely store a value into main memory
// ----------------------------------------------------------------------
bool safeStore(vector<int> &memory, int address, int value)
{
    if (address < 0 || address >= (int)memory.size())
    {
        return false; // out-of-bounds
    }
    memory[address] = value;
    return true;
}

// ----------------------------------------------------------------------
// Simulate CPU execution. We read each process's PCB from memory and
// interpret instructions (opcodes & parameters) from the place your
// code wrote them. Then we output the messages (compute/print/store/load)
// and once done, we set the process's state to TERMINATED and print
// final PCB details.
//
// NOTE: This is a simplified approach: we assume your code placed
// instructions at [base+10..base+10+numInstr-1], and parameters
// right after that.
// ----------------------------------------------------------------------
void simulateCPU(vector<int> &memory)
{
    int memSize = (int)memory.size();

    int currentIndex = 0;
    // We iterate over memory, looking for processes
    while (currentIndex < memSize)
    {
        int pid = memory[currentIndex + 0];
        if (pid == -1)
        {
            // means no more processes here or uninitialized
            // We can skip forward or break.
            break;
        }

        // Read PCB fields
        int state = memory[currentIndex + 1];
        int programCounter = memory[currentIndex + 2];
        int numInstructions = memory[currentIndex + 3];
        // memory[currentIndex + 4] is placeholder
        int maxMemNeeded = memory[currentIndex + 5];
        int cpuCyclesUsed = memory[currentIndex + 6];
        int registerValue = memory[currentIndex + 7];
        int maxMemNeeded2 = memory[currentIndex + 8];
        int mainMemBase = memory[currentIndex + 9]; // where process starts

        // If state is already terminated, skip
        if (pid == -1 || state == -1)
        {
            // means an empty slot or no process
            currentIndex += 1;
            continue;
        }
        // If state != 1 (NEW), that's okay, or we can set to RUNNING=3
        // For demonstration, let's force RUNNING=3 before execution
        memory[currentIndex + 1] = 3; // RUNNING

        // The instructions start at (base+10)
        int instrStart = currentIndex + 10;
        // The data start right after the instructions
        int dataStart = instrStart + numInstructions;

        // Now we "execute" each instruction in sequence
        for (int i = 0; i < numInstructions; i++)
        {
            int opcode = memory[instrStart + i];
            // The parameters for instruction i are located in data section,
            // but we have to figure out how many parameters each opcode uses.
            // Because your original code lumps all parameters in the same
            // order they appear. We'll keep a pointer into dataStart:
            // We'll do a simplified approach: just read them in the order
            // they appear, then move 'paramIndex' accordingly.

            // We'll keep a static pointer that moves as we read params.
            static int paramIndex = dataStart;
            // Ensure paramIndex is advanced properly for each new process
            // We'll reset it at the start of the process:
            if (i == 0)
            {
                paramIndex = dataStart;
            }

            switch (opcode)
            {
            case 1:
            {
                // compute => 2 params: iterations, cycles
                int iterations = memory[paramIndex++];
                int cycles = memory[paramIndex++];
                cout << "compute\n";
                // For example, we accumulate CPU cycles
                cpuCyclesUsed += cycles;
                // programCounter can be i+1 or something
                programCounter++;
                break;
            }
            case 2:
            {
                // print => 1 param: cycles
                int cycles = memory[paramIndex++];
                cout << "print\n";
                cpuCyclesUsed += cycles;
                programCounter++;
                break;
            }
            case 3:
            {
                // store => 2 params: value, address
                int value = memory[paramIndex++];
                int address = memory[paramIndex++];
                // check bounds
                if (safeStore(memory, address, value))
                {
                    cout << "store\n";
                }
                else
                {
                    cout << "store error!\n";
                }
                // maybe 1 CPU cycle cost, e.g.:
                cpuCyclesUsed += 1;
                programCounter++;
                break;
            }
            case 4:
            {
                // load => 1 param: address
                int address = memory[paramIndex++];
                int outVal = 0;
                if (safeLoad(memory, address, outVal))
                {
                    cout << "load\n";
                    registerValue = outVal;
                }
                else
                {
                    cout << "load error!\n";
                }
                cpuCyclesUsed += 1;
                programCounter++;
                break;
            }
            default:
                cout << "Unknown opcode " << opcode << "\n";
                programCounter++;
                break;
            }
        }

        // Once we finish all instructions for this process,
        // mark it TERMINATED in memory (state=4).
        memory[currentIndex + 1] = 4; // TERMINATED
        // Save updated fields back to memory
        memory[currentIndex + 2] = programCounter;
        memory[currentIndex + 6] = cpuCyclesUsed;
        memory[currentIndex + 7] = registerValue;

        // Now print the final PCB info
        cout << "\nPCB Contents (Stored in Main Memory):\n";
        cout << "Process ID:       " << memory[currentIndex + 0] << "\n";
        cout << "State:            TERMINATED\n";
        cout << "Program Counter:  " << memory[currentIndex + 2] << "\n";
        cout << "Instruction Base: " << (currentIndex + 10) << "\n";
        cout << "Data Base:        " << (currentIndex + 10 + numInstructions) << "\n";
        cout << "Memory Limit:     " << maxMemNeeded << "\n";
        cout << "CPU Cycles Used:  " << memory[currentIndex + 6] << "\n";
        cout << "Register Value:   " << memory[currentIndex + 7] << "\n";
        cout << "Max Memory Needed:" << memory[currentIndex + 5] << "\n";
        cout << "Main Memory Base: " << memory[currentIndex + 9] << "\n";
        cout << "Total CPU Cycles Consumed: " << memory[currentIndex + 6] << "\n";
        cout << "--------------------------------\n";

        // Move to the next block. The block size is 10 + #instr + 2*(#instr)
        int required_memory = 10 + numInstructions + numInstructions * 2;
        currentIndex += required_memory;
    }
}

// ----------------------------------------------------------------------
// MAIN
// ----------------------------------------------------------------------
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int main_memory_size = 0;
    int num_of_processes = 0;

    cin >> main_memory_size;
    cin >> num_of_processes;

    // main memory
    vector<int> main_memory(main_memory_size, -1);

    // read processes
    vector<PCB> processes;
    processes.reserve(num_of_processes);

    for (int i = 0; i < num_of_processes; i++)
    {
        PCB p;
        // p.state=1 is default (NEW).
        // read <process_id> <max_memory_needed> <num_of_instructions>
        cin >> p.process_id >> p.max_memory_needed >> p.num_of_instructions;
        p.state = 1; // NEW

        for (int j = 0; j < p.num_of_instructions; j++)
        {
            int opcode;
            cin >> opcode;
            vector<int> params;

            if (opcode == 1)
            {
                // compute => read 2 params: iterations, cycles
                int iters, cyc;
                cin >> iters >> cyc;
                params.push_back(iters);
                params.push_back(cyc);
            }
            else if (opcode == 2)
            {
                // print => read 1 param (cycles)
                int cyc;
                cin >> cyc;
                params.push_back(cyc);
            }
            else if (opcode == 3)
            {
                // store => read 2 params (value, address)
                int val, addr;
                cin >> val >> addr;
                params.push_back(val);
                params.push_back(addr);
            }
            else if (opcode == 4)
            {
                // load => read 1 param (address)
                int addr;
                cin >> addr;
                params.push_back(addr);
            }
            else
            {
                cerr << "Unknown opcode in input: " << opcode << "\n";
            }

            p.instructions.push_back(make_tuple(opcode, params));
        }

        processes.push_back(p);
    }

    // 1) Write them into memory
    writeToMemory(main_memory, processes, main_memory_size);

    // 2) Print all addresses in main memory
    //    in the format <address : content>
    printMemory(main_memory);

    // 3) Simulate CPU execution (printing instructions & final PCB)
    simulateCPU(main_memory);

    return 0;
}
