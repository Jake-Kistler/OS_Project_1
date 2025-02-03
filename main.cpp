#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>
#include <tuple>
#include <fstream>

using namespace std;

fstream fin ("PJ_1_inp.txt");

struct PCB {
    int process_id;         // The ID of the process
    int state = 1;          // The state of the process default is 1 (NEW)
    int max_memory_needed;  // The maximum memory needed by the process
    int num_of_instructions; // The number of instructions the process has
    vector<tuple<int, vector<int>>> instructions; // The instructions of the process
};

void writeToMemory(vector<int>& main_memory, const vector<PCB>& processes, int main_memory_size) {
    fill(main_memory.begin(), main_memory.end(), -1); // Initialize all elements to -1

    int memory_index = 0;
    for (const auto& process : processes) {
        int required_memory = 10 + process.num_of_instructions + process.instructions.size() * 2; // PCB metadata + opcodes + data
        if (memory_index + required_memory > main_memory_size) {
            cerr << "Error: Not enough memory to allocate process " << process.process_id << endl;
            continue;
        }

        // Allocate memory for the PCB metadata
        main_memory[memory_index] = process.process_id;
        main_memory[memory_index + 1] = process.state;
        main_memory[memory_index + 2] = 0; // Placeholder for future use
        main_memory[memory_index + 3] = process.num_of_instructions;
        main_memory[memory_index + 4] = 13; // Placeholder for future use
        main_memory[memory_index + 5] = process.max_memory_needed;
        main_memory[memory_index + 6] = 0; // Placeholder for future use
        main_memory[memory_index + 7] = 0; // Placeholder for future use
        main_memory[memory_index + 8] = process.max_memory_needed;
        main_memory[memory_index + 9] = 0; // Placeholder for future use

        // Allocate memory for the instruction opcodes
        int instr_base = memory_index + 10;
        int data_base = instr_base + process.num_of_instructions;
        for (const auto& instr : process.instructions) {
            int opcode = get<0>(instr);
            main_memory[instr_base++] = opcode;
        }

        // Allocate memory for the instruction data
        for (const auto& instr : process.instructions) {
            const auto& params = get<1>(instr);
            for (const auto& param : params) {
                main_memory[data_base++] = param;
            }
        }

        // Move to the next memory block
        memory_index += required_memory;
    }
}

void printMemory(const vector<int>& main_memory) {
    cout << "Main Memory:" << endl;
    for (size_t i = 0; i < main_memory.size(); ++i) {
        cout << i << ": " << main_memory[i] << endl;
    }
}

int main(int argc, char **argv) {
    int main_memory_size = 0; // The size of the main memory
    int num_of_processes = 0; // The number of processes

    vector<PCB> processes;    // The processes in the file
    vector<int> main_memory;  // The main memory 

    // Read from standard input (assuming redirection from a file)
    cin >> main_memory_size;
    cin >> num_of_processes;

    main_memory.resize(main_memory_size, -1); // Initialize main memory with -1 (indicating empty)

    for (int i = 0; i < num_of_processes; i++) {
        PCB process;
        cin >> process.process_id;
        cin >> process.max_memory_needed;
        cin >> process.num_of_instructions;

        for (int j = 0; j < process.num_of_instructions; ++j) {
            int opcode;
            cin >> opcode;
            vector<int> params;
            if (opcode == 1) { // Compute
                int iterations, cycles;
                cin >> iterations >> cycles;
                params.push_back(iterations);
                params.push_back(cycles);
            } else if (opcode == 2) { // Print
                int cycles;
                cin >> cycles;
                params.push_back(cycles);
            } else if (opcode == 3) { // Store
                int value, address;
                cin >> value >> address;
                params.push_back(value);
                params.push_back(address);
            } else if (opcode == 4) { // Load
                int address;
                cin >> address;
                params.push_back(address);
            }
            process.instructions.push_back(make_tuple(opcode, params));
        }

        processes.push_back(process);
    }

    // Write processes into main memory
    writeToMemory(main_memory, processes, main_memory_size);

    // Print the main memory for verification
    printMemory(main_memory);

    return 0;
}
