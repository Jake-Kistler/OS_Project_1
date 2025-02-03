#include<iostream>
#include<vector>
#include<sstream>
#include<string>
#include<queue>
#include<tuple>

using namespace std;

struct PCB{

    int process_id;         //The ID of the process
    int state = 1;          //The state of the process default is 1(NEW)
    int max_memory_needed;  // The maximum memory needed by the process

    int num_of_instructions; // The number of instructions the process has 
    //(1 is compute with iternations and cycles tailing, 2 is print with cycles following, 
    //3 is store with value and address following and lastly is load encoded as 4 with address following)

    vector<tuple<int, vector<int>>> instructions; // The instructions of the process
};

int main(int argv, char ** argc){
    int main_memory_size = 0; // The size of the main memory
    int num_of_processes = 0; // The number of processes

    vector<PCB> processes;    // The processes in the file
    vector<int> main_memory;  // The main memory 

    cin >> main_memory_size >> num_of_processes; // read in both numbers from the file

    for(int i = 0; i < num_of_processes; i++){
        PCB process;
        cin >> process.process_id;
        cin >> process.max_memory_needed;
        cin >> process.num_of_instructions;

        for(int j = 0; j < num_of_processes; j++){
            int opcode;
            cin >> opcode;
            vector<int> params;
            if(opcode == 1){ // compute
                int iterations, cycles;
                cin >> iterations >> cycles;
                params.push_back(iterations);
                params.push_back(cycles);
            }
            else if(opcode == 2){ // print
                int cycles;
                cin >> cycles;
                params.push_back(cycles);
            }
            else if(opcode == 3){ // store
                int value, address;
                cin >> value >> address;
                params.push_back(value);
                params.push_back(address);
            }
            else if(opcode == 4){ // load
                int address;
                cin >> address;
                params.push_back(address);
            }
            process.instructions.push_back(make_tuple(opcode, params));
        } // END J LOOP

        processes.push_back(process);

    } // END I LOOP

    // output the parsed processes for debugging
    for(const auto& processes : processes){
        cout << "Process ID: " << processes.process_id << endl;
        cout << "Max Memory Needed: " << processes.max_memory_needed << endl;
        cout << "Number of Instructions: " << processes.num_of_instructions << endl;

        for(const auto& instruction : processes.instructions){
            cout << "Opcode: " << get<0>(instruction) << endl;

            for(const auto& param : get<1>(instruction)){
                cout << "Param: " << param << endl;
            } // END PARAM LOOP
        } // END INSTRUCTION LOOP
    } // END PROCESSES LOOP

    return 0;

} // END MAIN
