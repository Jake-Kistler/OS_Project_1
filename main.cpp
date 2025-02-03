#include<iostream>
#include<vector>
#include<sstream>
#include<string>
#include<queue>

using namespace std;

struct PCB{

    int process_id;         //The ID of the process
    int state = 1;          //The state of the process default is 1(NEW)
    int max_memory_needed;  // The maximum memory needed by the process

    int num_of_instructions; // The number of instructions the process has 
    //(1 is compute with iternations and cycles tailing, 2 is print with cycles following, 
    //3 is store with value and address following and lastly is load encoded as 4 with address following)

    vector<int> instructions; // The instructions of the process
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

        for(int j = 0; j < process.num_of_instructions; j++){
            int instruction;
            cin >> instruction;
            process.instructions.push_back(instruction);
        }

        processes.push_back(process);
        
    } // END I LOOP

    // print the processes vector and the subvector that makes up each process
    for(const auto& process : processes){
        cout << "Process ID: " << process.process_id << endl;
        cout << "Max Memory Needed: " << process.max_memory_needed << endl;
        cout << "Number of Instructions: " << process.num_of_instructions << endl;
        cout << "Instructions: ";
        for(const auto& instruction : process.instructions){
            cout << instruction << " ";
        }
        cout << endl;
    }

}
