#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>
#include <tuple>

using namespace std;

struct PCB {
    int process_id;         // The ID of the process
    int state = 1;          // The state of the process default is 1 (NEW)
    int max_memory_needed;  // The maximum memory needed by the process
    int num_of_instructions; // The number of instructions the process has
    vector<tuple<int, vector<int>>> instructions; // The instructions of the process
};

int main(int argc, char **argv) {
    int main_memory_size = 0; // The size of the main memory
    int num_of_processes = 0; // The number of processes

    vector<PCB> processes;    // The processes in the file
    vector<int> main_memory;  // The main memory 

    // Read from standard input (assuming redirection from a file)
    cin >> main_memory_size;
    cin >> num_of_processes;

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

    // Output the parsed processes for verification
    for (const auto& process : processes) {
        cout << "Process ID: " << process.process_id << endl;
        cout << "Max Memory Needed: " << process.max_memory_needed << endl;
        cout << "Number of Instructions: " << process.num_of_instructions << endl;
        for (const auto& instr : process.instructions) {
            int opcode = get<0>(instr);
            const auto& params = get<1>(instr);
            if (opcode == 1) {
                cout << "Operation: Compute, Iterations: " << params[0] << ", Cycles: " << params[1] << endl;
            } else if (opcode == 2) {
                cout << "Operation: Print, Cycles: " << params[0] << endl;
            } else if (opcode == 3) {
                cout << "Operation: Store, Value: " << params[0] << ", Address: " << params[1] << endl;
            } else if (opcode == 4) {
                cout << "Operation: Load, Address: " << params[0] << endl;
            }
        }
        cout << endl;
    }

    return 0;
}
