#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// PCB Structure
struct PCB
{
    int processID;
    int state; // 0=NEW, 1=READY, 2=RUNNING, 3=TERMINATED
    int programCounter;
    int instructionBase;
    int dataBase;
    int memoryLimit;
    int cpuCyclesUsed;
    int registerValue;
    int maxMemoryNeeded;
    int mainMemoryBase;
    vector<int> instructions;
};

// Load Jobs into Memory
void loadJobsToMemory(queue<PCB> &newJobQueue, queue<int> &readyQueue, vector<int> &mainMemory, int maxMemory)
{
    int currentAddress = 0;
    while (!newJobQueue.empty())
    {
        PCB process = newJobQueue.front();
        newJobQueue.pop();

        process.mainMemoryBase = currentAddress;
        process.instructionBase = currentAddress + 10;
        process.dataBase = process.instructionBase + process.instructions.size();

        // Store PCB in mainMemory
        mainMemory[currentAddress] = process.processID;
        mainMemory[currentAddress + 1] = 1; // READY
        mainMemory[currentAddress + 2] = process.programCounter;
        mainMemory[currentAddress + 3] = process.instructionBase;
        mainMemory[currentAddress + 4] = process.dataBase;
        mainMemory[currentAddress + 5] = process.memoryLimit;
        mainMemory[currentAddress + 6] = process.cpuCyclesUsed;
        mainMemory[currentAddress + 7] = process.registerValue;
        mainMemory[currentAddress + 8] = process.maxMemoryNeeded;
        mainMemory[currentAddress + 9] = process.mainMemoryBase;

        // Store Instructions in mainMemory
        int instructionAddress = process.instructionBase;
        int dataAddress = process.dataBase;
        for (size_t i = 0; i < process.instructions.size(); i++)
        {
            int opcode = process.instructions[i];
            mainMemory[instructionAddress++] = opcode;

            if (opcode == 1) // Compute
            {
                mainMemory[dataAddress++] = process.instructions[++i]; // Iterations
                mainMemory[dataAddress++] = process.instructions[++i]; // Cycles
            }
            else if (opcode == 2) // Print
            {
                mainMemory[dataAddress++] = process.instructions[++i]; // Cycles
            }
            else if (opcode == 3) // Store
            {
                mainMemory[dataAddress++] = process.instructions[++i]; // Value
                mainMemory[dataAddress++] = process.instructions[++i]; // Address
            }
            else if (opcode == 4) // Load
            {
                mainMemory[dataAddress++] = process.instructions[++i]; // Address
            }
        }
        currentAddress += (10 + process.memoryLimit);
        readyQueue.push(process.mainMemoryBase);
    }
}

void executeCPU(queue<int> &readyQueue, vector<int> &mainMemory)
{
    while (!readyQueue.empty())
    {
        int baseAddress = readyQueue.front();
        readyQueue.pop();

        int instructionBase = mainMemory[baseAddress + 3];
        int dataBase = mainMemory[baseAddress + 4];
        int programCounter = 0;
        int cpuCyclesUsed = mainMemory[baseAddress + 6];
        int registerValue = 0;

        while (programCounter < (dataBase - instructionBase))
        {
            int opCode = mainMemory[instructionBase + programCounter];

            if (opCode == 1) // Compute
            {
                cout << "compute" << endl;
                programCounter += 3;
            }
            else if (opCode == 2) // Print
            {
                cout << "print" << endl;
                programCounter += 2;
            }
            else if (opCode == 3) // Store
            {
                cout << "stored" << endl;
                programCounter += 3;
            }
            else if (opCode == 4) // Load
            {
                cout << "loaded" << endl;
                programCounter += 2;
            }
        }
        mainMemory[baseAddress + 1] = 3; // TERMINATED
        cout << "Process ID: " << mainMemory[baseAddress] << endl;
        cout << "State: TERMINATED" << endl;
        cout << "Program Counter: " << programCounter << endl;
        cout << "Instruction Base: " << instructionBase << endl;
        cout << "Data Base: " << dataBase << endl;
        cout << "Memory Limit: " << mainMemory[baseAddress + 5] << endl;
        cout << "CPU Cycles Used: " << cpuCyclesUsed << endl;
        cout << "Register Value: " << registerValue << endl;
        cout << "Max Memory Needed: " << mainMemory[baseAddress + 8] << endl;
        cout << "Main Memory Base: " << mainMemory[baseAddress + 9] << endl;
        cout << "Total CPU Cycles Consumed: " << cpuCyclesUsed << endl;
    }
}

int main()
{
    int maxMemory, numProcesses;
    vector<int> mainMemory;
    queue<int> readyQueue;
    queue<PCB> newJobQueue;

    cin >> maxMemory >> numProcesses;
    mainMemory.resize(maxMemory, -1);

    for (int i = 0; i < numProcesses; i++)
    {
        PCB process;
        cin >> process.processID >> process.maxMemoryNeeded;
        process.state = 0;
        process.programCounter = 0;
        process.memoryLimit = process.maxMemoryNeeded;
        process.cpuCyclesUsed = 0;
        process.registerValue = 0;
        int numInstructions;
        cin >> numInstructions;
        for (int j = 0; j < numInstructions; j++)
        {
            int opcode;
            cin >> opcode;
            process.instructions.push_back(opcode);
        }
        newJobQueue.push(process);
    }
    loadJobsToMemory(newJobQueue, readyQueue, mainMemory, maxMemory);
    executeCPU(readyQueue, mainMemory);
    return 0;
}
