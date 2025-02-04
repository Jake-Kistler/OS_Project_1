#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm> // for fill()
#include <fstream>   // for file output
using namespace std;

// Output file stream
ofstream outputFile("output.txt");

// ----------------------------------------------------------------------
// PCB structure in memory
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
        main_memory[memory_index + 9] = memory_index; // mainMemoryBase

        // (2) Write instruction opcodes
        int instr_base = memory_index + 10;
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
// Print the entire main memory array to console and file
// ----------------------------------------------------------------------
void printMemory(const vector<int> &main_memory)
{
    outputFile << "Main Memory:\n";
    cout << "Main Memory:\n";
    for (size_t i = 0; i < main_memory.size(); ++i)
    {
        outputFile << i << ": " << main_memory[i] << "\n";
        cout << i << ": " << main_memory[i] << "\n";
    }
}

// ----------------------------------------------------------------------
// Simulate CPU execution and write log to file
// ----------------------------------------------------------------------
void simulateCPU(vector<int> &memory)
{
    int memSize = (int)memory.size();
    int currentIndex = 0;

    while (currentIndex < memSize)
    {
        int pid = memory[currentIndex + 0];
        if (pid == -1)
            break;

        // Read PCB fields
        int state = memory[currentIndex + 1];
        int programCounter = memory[currentIndex + 2];
        int numInstructions = memory[currentIndex + 3];
        int maxMemNeeded = memory[currentIndex + 5];
        int cpuCyclesUsed = memory[currentIndex + 6];
        int registerValue = memory[currentIndex + 7];
        int mainMemBase = memory[currentIndex + 9];

        memory[currentIndex + 1] = 3; // RUNNING
        int instrStart = currentIndex + 10;
        int dataStart = instrStart + numInstructions;

        int paramIndex = dataStart;
        for (int i = 0; i < numInstructions; i++)
        {
            int opcode = memory[instrStart + i];

            switch (opcode)
            {
            case 1:
            {
                int iterations = memory[paramIndex++];
                int cycles = memory[paramIndex++];
                outputFile << "compute\n";
                cout << "compute\n";
                cpuCyclesUsed += cycles;
                programCounter++;
                break;
            }
            case 2:
            {
                int cycles = memory[paramIndex++];
                outputFile << "print\n";
                cout << "print\n";
                cpuCyclesUsed += cycles;
                programCounter++;
                break;
            }
            case 3:
            {
                int value = memory[paramIndex++];
                int address = memory[paramIndex++];
                if (address >= 0 && address < memSize)
                {
                    memory[address] = value;
                    outputFile << "store\n";
                    cout << "store\n";
                }
                else
                {
                    outputFile << "store error!\n";
                    cout << "store error!\n";
                }
                cpuCyclesUsed += 1;
                programCounter++;
                break;
            }
            case 4:
            {
                int address = memory[paramIndex++];
                if (address >= 0 && address < memSize)
                {
                    registerValue = memory[address];
                    outputFile << "load\n";
                    cout << "load\n";
                }
                else
                {
                    outputFile << "load error!\n";
                    cout << "load error!\n";
                }
                cpuCyclesUsed += 1;
                programCounter++;
                break;
            }
            default:
                outputFile << "Unknown opcode " << opcode << "\n";
                cout << "Unknown opcode " << opcode << "\n";
                programCounter++;
                break;
            }
        }

        memory[currentIndex + 1] = 4; // TERMINATED
        memory[currentIndex + 2] = programCounter;
        memory[currentIndex + 6] = cpuCyclesUsed;
        memory[currentIndex + 7] = registerValue;

        // Write final PCB contents to file
        outputFile << "\nPCB Contents (Stored in Main Memory):\n";
        outputFile << "Process ID:       " << memory[currentIndex + 0] << "\n";
        outputFile << "State:            TERMINATED\n";
        outputFile << "Program Counter:  " << memory[currentIndex + 2] << "\n";
        outputFile << "Instruction Base: " << (currentIndex + 10) << "\n";
        outputFile << "Data Base:        " << (currentIndex + 10 + numInstructions) << "\n";
        outputFile << "Memory Limit:     " << maxMemNeeded << "\n";
        outputFile << "CPU Cycles Used:  " << memory[currentIndex + 6] << "\n";
        outputFile << "Register Value:   " << memory[currentIndex + 7] << "\n";
        outputFile << "Max Memory Needed:" << memory[currentIndex + 5] << "\n";
        outputFile << "Main Memory Base: " << memory[currentIndex + 9] << "\n";
        outputFile << "Total CPU Cycles Consumed: " << memory[currentIndex + 6] << "\n";
        outputFile << "--------------------------------\n";

        currentIndex += 10 + numInstructions + numInstructions * 2;
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

    vector<int> main_memory(main_memory_size, -1);
    vector<PCB> processes;
    processes.reserve(num_of_processes);

    for (int i = 0; i < num_of_processes; i++)
    {
        PCB p;
        cin >> p.process_id >> p.max_memory_needed >> p.num_of_instructions;
        p.state = 1;

        for (int j = 0; j < p.num_of_instructions; j++)
        {
            int opcode;
            cin >> opcode;
            vector<int> params;

            if (opcode == 1)
            {
                int iters, cyc;
                cin >> iters >> cyc;
                params.push_back(iters);
                params.push_back(cyc);
            }
            else
            {
                int val;
                cin >> val;
                params.push_back(val);
            }
            p.instructions.push_back(make_tuple(opcode, params));
        }
        processes.push_back(p);
    }

    writeToMemory(main_memory, processes, main_memory_size);
    printMemory(main_memory);
    simulateCPU(main_memory);

    outputFile.close();
    return 0;
}
