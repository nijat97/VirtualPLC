#include "VirtualPLC.h"
#include "fstream"
#include "iostream"
#include "string.h"
#include "sstream"
#include <unistd.h>
#include <ctime>

using namespace std;

enum
{
    SUCCESS = 0,
    EMPTY,
    FAIL
};

/* Global variables */
LineReader ln_reader;
CPU myCPU;
GPIO gpio_vars[15];
Timer timer_vars[3];

int gpio_index=0;
int timer_index=0;

/*
Create virtual peripheries by types GPIO, timer, comm
*/

/*
 Pass file_name instead of ifstream reference
*/

/* *******************************************
 * Parses the line and fills instruction
  and args to curr_line 
 *********************************************/
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
/*******************************************/

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

    for(int i=0;i<sizeof(cpu->periph_types)/sizeof(cpu->periph_types[0]);i++)
    {
        if(strcmp(cpu->periph_types[i].name,line->args[0]) == 0)
        {
            if(cpu->periph_types[i].create(cpu,line, &cpu->periph_types[i]) == SUCCESS)
            {
                return SUCCESS;
            }
        } 
    }
    cout << line->args[0] << " is not a valid periphery type" << endl;
    return FAIL;
}


int Set(CPU *cpu, Line *line)
{
    for(int i=0;i<sizeof(cpu->periph_instances)/sizeof(cpu->periph_instances[0]);i++)
    {
        if(strcmp(cpu->periph_instances[i].name,line->args[0])==0)
        {
            cpu->periph_instances[i].type->set(cpu,line, &cpu->periph_instances[i]);
        } 
    }
}

int Print(CPU *cpu, Line *line)
{
    for(int i=0;i<sizeof(cpu->periph_instances)/sizeof(cpu->periph_instances[0]);i++)
    {
        if(strcmp(cpu->periph_instances[i].name,line->args[0])==0)
        {
            cout << line->args[0] << " found" << endl;
            cout << "TYPE: " << cpu->periph_instances[i].type->name << endl;
            if(strcmp(cpu->periph_instances[i].type->name,"GPIO")==0)
            {
                cout << "MODE: ";
                (((GPIO*)cpu->periph_instances[i].peripheryData)->mode == INPUT) ? cout << "INPUT": cout << "OUTPUT";
                cout << endl;
                cout << "PIN: " << ((GPIO*)cpu->periph_instances[i].peripheryData)->pin << endl;
                cout << "VARIABLE: " << ((GPIO*)cpu->periph_instances[i].peripheryData)->var->name << endl;
                cout << endl;
            }
            else if(strcmp(cpu->periph_instances[i].type->name,"TIMER")==0)
            {
                cout << "PRESET TIME: ";
                cout << ((Timer*)cpu->periph_instances[i].peripheryData)->presetTime;
                cout << endl;
                cout << "TIMER MODE: ";
                (((Timer*)cpu->periph_instances[i].peripheryData)->timerMode == ONDelay) ? cout << "ONDelay": cout << "OFFDelay";
                cout << endl;
                cout << "INPUT GPIO: " << ((Timer*)cpu->periph_instances[i].peripheryData)->input->pin << endl;
                cout << "OUTPUT GPIO: " << ((Timer*)cpu->periph_instances[i].peripheryData)->output->pin << endl;
                cout << endl;
            }
        } 
    }
}
/**********************************************/

/* ********************************************
 *  CREATE functions for peripherals
 * ********************************************/
int CreateGpio(CPU *cpu, Line *line, PeripheryType *p_type)
{

    //Check if there is already instance with the same name
    for(int i=0;i<sizeof(cpu->periph_instances)/sizeof(cpu->periph_instances[0]);i++)
    {
        if(strcmp(cpu->periph_instances[i].name,line->args[1]) == 0)
        {
            cout << "Instance is already there" << endl;
            return FAIL;
        }
    }
    strcpy(cpu->periph_instances[cpu->instance_index].name,line->args[1]);
    cpu->periph_instances[cpu->instance_index].type = p_type;
    cpu->periph_instances[cpu->instance_index].peripheryData = &gpio_vars[gpio_index];
    cpu->instance_index++;
    gpio_index++;
    return SUCCESS;
}

int CreateTimer(CPU *cpu, Line *line, PeripheryType *p_type)
{
    //Check if there is already instance with the same name
    for(int i=0;i<sizeof(cpu->periph_instances)/sizeof(cpu->periph_instances[0]);i++)
    {
        if(strcmp(cpu->periph_instances[i].name,line->args[1]) == 0)
        {
            cout << "Instance is already there" << endl;
            return FAIL;
        }
    }
    
    strcpy(cpu->periph_instances[cpu->instance_index].name,line->args[1]);
    cpu->periph_instances[cpu->instance_index].type = p_type;
    cpu->periph_instances[cpu->instance_index].peripheryData = &timer_vars[timer_index];
    cpu->instance_index++;
    timer_index++;
    return SUCCESS;
}


/***********************************************/

/* *********************************************
 * SET functions for peripherals
 * ********************************************/

int setGpio(CPU *cpu, Line *line, PeripheryInstance *p_instance)
{
    if(strcmp(line->args[1],"MODE")==0)
    {
        if(strcmp(line->args[2],"INPUT")==0)
        {
            ((GPIO*)(p_instance->peripheryData))->mode = INPUT;
        }
        else
        {
            ((GPIO*)(p_instance->peripheryData))->mode = OUTPUT;
        }
    }
    else if(strcmp(line->args[1],"PIN")==0)
    {
        ((GPIO*)(p_instance->peripheryData))->pin = strtol(line->args[2],nullptr,10);
    }
    else if(strcmp(line->args[1],"VAR")==0)
    {
        strcpy(cpu->vars[cpu->variable_index].name,line->args[2]);
        ((GPIO*)(p_instance->peripheryData))->var = &cpu->vars[cpu->variable_index];
        cpu->variable_index++;
    }
}

int setTimer(CPU *cpu, Line *line, PeripheryInstance *p_instance)
{
    if(strcmp(line->args[1],"MODE")==0)
    {
        if(strcmp(line->args[2],"ONDelay")==0)
        {
            ((Timer*)(p_instance->peripheryData))->timerMode = ONDelay;
        }
        else if(strcmp(line->args[2],"OFFDelay")==0)
        {
            ((Timer*)(p_instance->peripheryData))->timerMode = OFFDelay;
        }
    }
    else if(strcmp(line->args[1],"TIME")==0)
    {
        ((Timer*)(p_instance->peripheryData))->presetTime = strtol(line->args[2],nullptr,10);
    }
    else if(strcmp(line->args[1],"INPUT")==0)
    {
        for (int i = 0; i < sizeof(cpu->periph_instances) / sizeof(cpu->periph_instances[0]); i++)
        {
            if (strcmp(cpu->periph_instances[i].name, line->args[2]) == 0)
            {
                 ((Timer *)(p_instance->peripheryData))->input = (GPIO*)cpu->periph_instances[i].peripheryData;
            }
        }
    }
    else if(strcmp(line->args[1],"OUTPUT")==0)
    {
        for (int i = 0; i < sizeof(cpu->periph_instances) / sizeof(cpu->periph_instances[0]); i++)
        {
            if (strcmp(cpu->periph_instances[i].name, line->args[2]) == 0)
            {
                 ((Timer *)(p_instance->peripheryData))->output = (GPIO*)cpu->periph_instances[i].peripheryData;
            }
        }
    }
    else if(strcmp(line->args[1],"ENABLE")==0)
    {
        ((Timer *)(p_instance->peripheryData))->EN = true;
        ((Timer *)(p_instance->peripheryData))->lastMillis = time(nullptr);
        ((Timer *)(p_instance->peripheryData))->lastInputState = ((Timer *)(p_instance->peripheryData))->input->var->ivalue;
    }
}

int setESPNOW()
{

}

/**********************************/
/* Loop functions for peripherals */
/**********************************/

void loopGpio()
{
    if(gpio_index == 0)
    {
        cout << "NO GPIO Registered!" << endl;
        return;
    }

    for(int i=0; i < gpio_index; i++)
    {
        if(gpio_vars[i].mode == INPUT)
        {
            cout << "GPIO Pin " << gpio_vars[i].pin << " is read" << endl;
            //gpio_vars[i].var->ivalue = digitalRead(gpio_vars[i].pin);
        }
        else if(gpio_vars[i].mode == OUTPUT)
        {
            cout << "GPIO Pin " << gpio_vars[i].pin << " is written" << endl;
            //digitalWrite(gpio_vars[i].pin,gpio_vars[i].var->ivalue);
        }
    }
}

void loopTimer()
{
    for (int i = 0; i < timer_index; i++)
    {
        if (timer_vars[i].timerMode == ONDelay)
        {
            time_t now = time(nullptr);
            if (timer_vars[i].EN == false)
                continue;

            if (timer_vars[i].accumulatedTime > timer_vars[i].presetTime)
            {
                timer_vars[i].output->var->ivalue = 1;
                timer_vars[i].accumulatedTime = 0;
                timer_vars[i].running = false;
            }
            if (timer_vars[i].lastInputState == 0 && timer_vars[i].input->var->ivalue == 1)
            {
                timer_vars[i].running = true;
            }

            if (timer_vars[i].running)
            {
                timer_vars[i].accumulatedTime += (now - timer_vars[i].lastMillis);
                cout << timer_vars[i].accumulatedTime << endl;
            }

            timer_vars[i].lastMillis = time(nullptr);
            timer_vars[i].lastInputState = timer_vars[i].input->var->ivalue;
        }
    }
}

/**********************************/
/* Calls cooresponding function for given line of code */
int ParseInstruction(CPU *cpu, Line *line)
{
    bool found=0;
    for(int i=0; i<sizeof(cpu->instructions)/sizeof(cpu->instructions[0]);i++ )
    {
        if(strcmp(line->instruction,cpu->instructions[i].name)==0)
        {
            cpu->instructions[i].instr(cpu,line);
            return SUCCESS;
        }
    }
    cout << line->instruction << " is not defined in CPU" << endl;
    return FAIL;
}

int main()
{
    Line current;
    myCPU.instance_index=0;
    current.line_number = 0;
    std::ifstream source_code;
    source_code.open("source.txt");
    time_t now, lastMillis = 0;

 
    if (!source_code)
    {
        cout << "Unable to open source code" << endl;
        exit(1);
    }

    /* Line reader function */
    ln_reader.get_next_line = &getNext;


    /* CPU instructions */
    strcpy(myCPU.instructions[0].name,"CREATE");
    myCPU.instructions[0].instr= &Create;

    strcpy(myCPU.instructions[1].name,"SET");
    myCPU.instructions[1].instr= &Set;

    strcpy(myCPU.instructions[2].name,"PRINT");
    myCPU.instructions[2].instr = &Print;


    /* Periph types */
    strcpy(myCPU.periph_types[0].name, "GPIO");
    myCPU.periph_types[0].create = &CreateGpio;
    myCPU.periph_types[0].set = &setGpio;
    myCPU.periph_types[0].loop = &loopGpio;
 
    strcpy(myCPU.periph_types[1].name, "TIMER");
    myCPU.periph_types[1].create = &CreateTimer;
    myCPU.periph_types[1].set = &setTimer;
    myCPU.periph_types[1].loop = &loopTimer;

    while (1)
    {
        now = time(nullptr);
        if (now - lastMillis > 1)
        {
            int res = ln_reader.get_next_line(source_code, &current);
            if (res == SUCCESS)
            {
                cout << "Line " << current.line_number << ": " << current.instruction << " ";

                for (int i = 0; i < current.args_count; i++)
                {
                    cout << current.args[i] << " ";
                }
                cout << endl;

                ParseInstruction(&myCPU, &current);
            }
            lastMillis = now;
            myCPU.periph_types[0].loop();
            myCPU.periph_types[1].loop();
        }
 
    }
}