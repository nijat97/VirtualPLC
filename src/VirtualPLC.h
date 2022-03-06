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
typedef int (*PeripheryType_Create)(const char[]);
typedef int (*PeripheryType_Set)(PeripheryType *periph_type);
typedef int (*PeripheryInstance_Create)(const char[]);
typedef int (*PeripheryInstance_Set)(PeripheryInstance *periph_instance);
typedef int (*PeripheryInstance_Loop)(const char[]);
typedef int (*Instruction_Parse)(CPU *cpu, Line *line);

typedef int (*GetLineNumber)(const char src[]);
typedef Line (*GetNextLine)(const char src[],  Line curr_line);


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

typedef struct sPeripheryType {
    char name[STRING_NAME_LEN];
    PeripheryType_Create create;
    PeripheryType_Set set;
} PeripheryType;

typedef struct sPeripheryInstance {
    char name[STRING_NAME_LEN];
    PeripheryType type;
    PeripheryInstance_Create create;
    PeripheryInstance_Set set;
} PeripheryInstance;

typedef struct sLine{
    int line_number;
    std::string instruction;
    std::string args[10];
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
    PeripheryInstance instances[15];
    Instruction instructions[15];
} CPU;








