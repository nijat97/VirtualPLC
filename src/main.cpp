#include "VirtualPLC.h"
#include "fstream"
#include "iostream"
#include "string.h"
#include "sstream"
#include <unistd.h>

using namespace std;

enum
{
    SUCCESS = 0,
    EMPTY
};

LineReader ln_reader;

/*
Create virtual peripheries by types GPIO, timer, comm
*/

/*
 Pass file_name instead of ifstream reference
*/

CPU myCPU;

int getNext(std::ifstream &src, Line *curr_line)
{
    string line;
    string word = "";
    string instruction, args[10];
    int i=0;

    curr_line->instruction[0]= '\0';
    for(int i=0;i<10;i++)
    {
        curr_line->args[i][0] = '\0';
    }

    getline(src, line);
    
    if(line == "")
    {
        return EMPTY;
    }
    for (auto x : line) 
    {
        if (x == ' ')
        {
            if(i==0)
            {
                strcpy(curr_line->instruction,word.c_str());
                curr_line->instruction[word.length()]='\0';
            }
            else
            {
                strcpy(curr_line->args[i-1],word.c_str());
                curr_line->args[i-1][word.length()]='\0';
            }
            i++;
            word = "";
        }
        else 
        {
            word = word + x;
        }
    }
    curr_line->args_count = i;
    cout << "Arg count: " << i << endl;
    if(i==0)
    {
        strcpy(curr_line->instruction,word.c_str());
        curr_line->instruction[word.length()]='\0';
    }
    else
    {
        strcpy(curr_line->args[i-1],word.c_str());
        curr_line->args[i-1][word.length()]='\0';
    }
    cout << "Instruction: " << curr_line->instruction << endl;
    cout << "Args: " << endl;
    for(int i=0;i<curr_line->args_count;i++)
    {
        cout << curr_line->args[i] << endl;
    }
    return SUCCESS;
}

int CREATE(CPU *cpu, Line *line)
{
    cout << "EXECUTE CREATE\n";
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

    strcpy(myCPU.instructions[0].name,"CREATE");
    myCPU.instructions[0].instr= &CREATE;
    ln_reader.get_next_line = &getNext;
    Line current;
    current.line_number = 0;

    while (1)
    {

        int res = ln_reader.get_next_line(source_code, &current);
        std::cout << std::endl;
        // if (res == SUCCESS)
        // {
        //     InstrParse(&myCPU,&current);
        //     cout << current.instruction << endl;

        //     for (int i = 0; i < current.args_count; i++)
        //     {
        //         cout << current.args[i] << " ";
        //     }
        //     cout << endl
        //          << "Arg number: " << current.args_count << endl;
        //     cout << "Line number: " << current.line_number << endl;
        // }
        
        sleep(1);
    }
}