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

/* Global variables */
LineReader ln_reader;
CPU myCPU;
GPIO gpio_vars[15];
Timer timer_vars[3];

/*
Create virtual peripheries by types GPIO, timer, comm
*/

/*
 Pass file_name instead of ifstream reference
*/


/* Parsing the next line in the file */
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

    curr_line->line_number++;
    return SUCCESS;
}


/*
********************************************
    Function definitions for instructions
********************************************
*/
int Create(CPU *cpu, Line *line)
{
    // Looks for the first argument(GPIO,TIMER)
    // in periph_types. If found, it will call
    //corresponding create function for the periph type
    cout << "Creating: " << line->args[0]<< endl;
    for(int i=0;i<sizeof(cpu->periph_types)/sizeof(cpu->periph_types[0]);i++)
    {
        if(strcmp(cpu->periph_types[i].name,line->args[0]) == 0)
        {
            cpu->periph_types[i].create(cpu,line);
        } 
    }
}

int Set(CPU *cpu, Line *line)
{
    for(int i=0;i<sizeof(cpu->periph_types)/sizeof(cpu->periph_types[0]);i++)
    {
        if(cpu->periph_types[i].name == line->args[0])
        {
            cpu->periph_types[i].set(&cpu->periph_types[i]);
        } 
    }
}

/**********************************************/

/* ********************************************
 *  CREATE functions for peripherals
 * ********************************************/
int CreateGpio(CPU *cpu, Line *line)
{
    cout << "Creating GPIO: " << line->args[1] << endl;
    strcpy(cpu->periph_instances[0].name,line->args[1]);
    cpu->periph_instances[0].type = &cpu->periph_types[0];

    cpu->periph_instances[0].peripheryData = &gpio_vars[0];
}

int CreateTimer()
{
    //not implemented yet
}
/***********************************************/

/* *********************************************
 * SET functions for peripherals
 * ********************************************/

int setGpio(CPU *cpu, Line *line)
{
    if(line->args[1] == "MODE")
    {

    }
}

int setTimer()
{

}

int setESPNOW()
{

}
/* Calls cooresponding function for given line of code */
int InstrParse(CPU *cpu, Line *line)
{
    for(int i=0; i<sizeof(cpu->instructions)/sizeof(cpu->instructions[0]);i++ )
    {
        if(strcmp(line->instruction,cpu->instructions[i].name)==0)
        {
            cout << line->instruction << " is being executed.." << endl;
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

    //assign functions to instructions
    strcpy(myCPU.instructions[0].name,"CREATE");
    myCPU.instructions[0].instr= &Create;

    strcpy(myCPU.instructions[1].name,"SET");
    myCPU.instructions[1].instr= &Set;


    //assign function to GPIO periph type
    strcpy(myCPU.periph_types[0].name, "GPIO");
    myCPU.periph_types[0].create = &CreateGpio;

    ln_reader.get_next_line = &getNext;
    Line current;
    current.line_number = 0;

    while (1)
    {

        int res = ln_reader.get_next_line(source_code, &current);
        if (res == SUCCESS)
        {

            cout << current.instruction << endl;

            for (int i = 0; i < current.args_count; i++)
            {
                cout << current.args[i] << " ";
            }
            cout << endl
                 << "Arg number: " << current.args_count << endl;
            cout << "Line number: " << current.line_number << endl;

            InstrParse(&myCPU,&current);
        }
        
        sleep(1);
    }
}