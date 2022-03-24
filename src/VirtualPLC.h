/* 
    A simple library for simulating general PLC functionality.
*/
#include <string>
#define STRING_NAME_LEN 20

/* Variables and enums */
enum VariableType { BOOL, BYTE, INT, FLOAT };
enum GPIOMode { INPUT, OUTPUT };

typedef struct sVariable Variable;
typedef struct sGPIO GPIO;
typedef struct sPeripheryType PeripheryType;
typedef struct sPeripheryInstance PeripheryInstance;
typedef struct sLine Line;
typedef struct sLineReader LineReader;
typedef struct sInstruction Instruction;
typedef struct sCPU CPU;

/* Function pointers */
typedef int (*PeripheryType_Create)(CPU *cpu,Line *line);
typedef int (*PeripheryType_Set)(PeripheryType *periph_type);
typedef int (*PeripheryInstance_Create)(const char[]);
typedef int (*PeripheryInstance_Set)(PeripheryInstance *periph_instance);
typedef int (*PeripheryInstance_Loop)(const char[]);
typedef int (*Instruction_Parse)(CPU *cpu,Line *line);

typedef int (*GetLineNumber)(const char src[]);
typedef int (*GetNextLine)(std::ifstream& src,  Line* curr_line);


typedef struct sVariable {
    char name[STRING_NAME_LEN];
    VariableType type;
    int ivalue;
    float fvalue;
} Variable;

typedef struct sGPIO{
    int pin;
    GPIOMode mode;
    Variable *var;
} GPIO;

typedef struct sTimer {
    int presetTime;
    int actualTime;
    int lastTimeMillis; //how much time passed since last call
    int timerMode; //onDelay, offDelay, pulse
    int *input; //input pin
    int *output; //output pin
}Timer;

typedef struct sESPNOW {
    //not implemented yet
}ESPNOW;

//CREATE GPIO iButton
//CREATE func will go thorugh PeriphTypes in cpu and look for GPIO and call its function to 
//create new Periph instance
//There is periph instances in cpu and it wil setup new element
//name would be ibutton, type reference will point to periphtype struct
//it can setup loop function, gpio_loop 
// sets peripherydata

//SET iButton MODE Out 
//SET function is called and looks for iButton periphery instance
//it will call periphtype set function
//set function in periphtype will use periphdata to access gpioMode and set gpioMode

//same with> SET iButton pin 5
// SET ibutton var ibutton
//it should create/find new var in cpu 
//and set it to Variable in GPIO

//SETUPDONE instruction f.e before it all config , after it the main program
typedef struct sPeripheryType {
    char name[STRING_NAME_LEN];
    PeripheryType_Create create;
    PeripheryType_Set set;
    PeripheryInstance_Loop loop;
    //variable to state how often the loop should called
    //it should periodically call loop for all instances and update them
    //if input read from pin and set to Variable in GPIO
    //if output use variable to update output digitalWrite
    //cal in everycycle for GPIO
    //loop here?
} PeripheryType;

typedef struct sPeripheryInstance {
    char name[STRING_NAME_LEN];
    PeripheryType *type;
    //PeripheryInstance_Set set;
    
    void* peripheryData; ///--- GPIO structure point to GPIO struct instance
} PeripheryInstance;

typedef struct sLine{
    int line_number;
    char instruction[20];
    char args[10][15];
    int args_count;
} Line;

typedef struct sLineReader{
    char sourceName[20];
    int lineNumber;
    Line curr_line;
    GetLineNumber get_ln_num;
    GetNextLine get_next_line;
} LineReader;

typedef struct sInstruction {
    char name[20];
    Instruction_Parse instr;
} Instruction;

typedef struct sCPU {
    int PC;
    Variable vars[15];
    PeripheryType periph_types[15];
    PeripheryInstance periph_instances[15];
    Instruction instructions[15];
} CPU;








