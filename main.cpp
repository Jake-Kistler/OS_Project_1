#include<iostream>
#include <string>
#include <queue>
#include <fstream>

using namespace std;

struct PCB
{
    int max_memory_size; // first line in file
    int process_count; // second line in file
    int process_id; // first number in line post process count
    string state = "NEW"; // state, defaulted to NEW
    int memory_needed_for_process; // 2nd number in the post process line
    int number_of_instructions; // 3rd number in the post process line
    int instruction; // the instruction encoded
    int operand_count; // dynamically allocation occurs from this number
}; // END OF PCB

void load_jobs_to_memory(queue <PCB> new_job_queue, queue<int> &ready_queue, vector<int> &main_memory, int max_memory_size);

void execute_CPU(int start_address, int *main_memory);

void print_PCB(const PCB *process_control_board);

int main(int argc, char **argv)
{
    int max_memory_size, process_count;
    queue<PCB> new_job_queue;
    queue<int> ready_queue;
    int *main_memory;
    fstream file ("PJ_1_inp.txt");

    // check if the file is open 
    if(!file.is_open())
    {
        cerr << "Error opening the file!\n";
        return 1;
    }

    PCB process_control_board;
    // read in data from the file into the PCB
    file >> process_control_board.max_memory_size;
    //cout << "MAX MEM FROM FILE: " << process_control_board.max_memory_size << endl;

    file >> process_control_board.process_count;
    file >> process_control_board.process_id;
    // skip state as is constant at the moment 
    file >> process_control_board.memory_needed_for_process;
    file >> process_control_board.number_of_instructions;
    file >> process_control_board.instruction;
    file >> process_control_board.operand_count;

    print_PCB(&process_control_board);
 
 // Test

}

void print_PCB(const PCB *process_control_board)
{
    if(process_control_board == NULL)
    {
        cout << "PCB is empty!\n";
        return;
    }

    cout << "Max memory needed: " << process_control_board->max_memory_size << endl;
    cout << "Process count: " << process_control_board->process_count << endl;
    cout << "Process ID: " << process_control_board->process_id << endl;
    cout << "Process State: " << process_control_board->state << endl;
    cout << "Memory needed for process: " << process_control_board->memory_needed_for_process << endl;
    cout << "Number of instructions: " << process_control_board->number_of_instructions << endl;
    cout << "Specific instruction: " << process_control_board->instruction << endl;
    cout << "Operand count: " << process_control_board->operand_count << endl;
}
   