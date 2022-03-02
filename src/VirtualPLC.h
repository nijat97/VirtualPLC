/* 
    A simple library for simulating general PLC functionality.
*/
#define STRING_NAME_LEN 20

/* Variables and enums */
enum VariableType { BOOL, BYTE, INT, FLOAT };
enum GPIOMode { INPUT, OUTPUT };

/* Function pointers */
typedef int (*PeripheryType_Create)(const char[]);
typedef int (*PeripheryType_Set)(PeripheryType*);
typedef int (*PeripheryInstance_Create)(const char[]);
typedef int (*PeripheryInstance_Set)(PeripheryInstance*);
typedef int (*PeripheryInstance_Loop)(const char[]);
typedef int (*Instruction_Parse)(CPU, Line);


typedef struct {
    int pin;
    GPIOMode mode;
    Variable *var;
} GPIO;

typedef struct {
    char name[STRING_NAME_LEN];
    PeripheryType_Create create;
    PeripheryType_Set set;
} PeripheryType;

typedef struct {
    char name[STRING_NAME_LEN];
    VariableType type;
    int ivalue;
    float fvalue;
} Variable;

typedef struct {
    char name[STRING_NAME_LEN];
    PeripheryType type;
    PeripheryInstance_Create create;
    PeripheryInstance_Set set;
} PeripheryInstance;

typedef struct {
    char label[20];
    char instruction[20];
    char args[5];
    int args_count;
} Line;

typedef struct {
    int PC;
    Variable vars[15];
    PeripheryType periph_types[15];
    PeripheryInstance instances[15];
    Instruction instructions[15];
} CPU;

typedef struct 
{
    char name[20];
    Instruction_Parse instr;
} Instruction;









