#include "VirtualPLC.h"
#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
#include <unistd.h>

using namespace std;

enum
{
    SUCCESS = 0,
    EMPTY
};

LineReader ln_reader;

CPU myCPU;
int getNext(std::ifstream &src, Line *curr_line)
{
    string str, word;
    int i;

    curr_line->args_count = 0;

    getline(src, str);

    if(str == "")
        return EMPTY;
    istringstream ss(str);
    i = 0;
    while (ss >> word)
    {
        if (i == 0)
        {
            curr_line->instruction = word;
        }
        else
        {
            curr_line->args[curr_line->args_count] = word;
            curr_line->args_count++;
        }
        i++;
    }
    curr_line->line_number++;

    return SUCCESS;
}

int CREATE(CPU *cpu, Line *line)
{
    cout << "EXECUTE CREATE";
}

int InstrParse(CPU *cpu, Line *line)
{
    for(int i=0; i<20;i++ )
    {
        if(line->instruction == cpu->instructions[i].name)
        {
            cpu->instructions[i].instr(cpu,line);
            return 0;
        }
    }
}

int main()
{
    std::ifstream source_code;
    source_code.open("source.txt");
    if (!source_code)
    {
        cout << "Unable to open source code" << endl;
        exit(1);
    }

    myCPU.instructions[0].name = "CREATE";
    myCPU.instructions[0].instr= &CREATE;
    ln_reader.get_next_line = &getNext;

    while (1)
    {
        Line current;
        current.line_number = 0;
        int res = ln_reader.get_next_line(source_code, &current);
        if (res == SUCCESS)
        {
            InstrParse(&myCPU,&current);
            cout << current.instruction << endl;

            for (int i = 0; i < current.args_count; i++)
            {
                cout << current.args[i] << " ";
            }
            cout << endl
                 << "Arg number: " << current.args_count << endl;
            cout << "Line number: " << current.line_number << endl;
        }
        
        sleep(1);
    }
}