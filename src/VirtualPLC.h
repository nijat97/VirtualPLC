/* 
    A simple library for simulating general PLC functionality.
*/
#include <string>

/* Variables and enums */
enum VariableType { BOOL, BYTE, INT, FLOAT };
enum GPIOMode { INPUT, OUTPUT };

/* Function pointers */
typedef int (*PeripheryType_Create)(std::string name);
typedef int (*PeripheryType_Set)(PeripheryType*);
typedef int (*PeripheryInstance_Create)(std::string name);
typedef int (*PeripheryInstance_Set)(PeripheryInstance*);

typedef struct {
    uint8_t pin;
    GPIOMode mode;
    Variable var;
} GPIO;

typedef struct {
    std::string name;
    PeripheryType_Create create;
    PeripheryType_Set set;
}PeripheryType;

/* Structures */
typedef struct {
    std::string name;
    VariableType type;
    uint32_t value;
} Variable;

typedef struct {
    std::string name;
    PeripheryType type;
    PeripheryInstance_Create create;
    PeripheryInstance_Set set;
} PeripheryInstance;

typedef struct {

} Line;

typedef struct {
    uint8_t PC;
    Variable vars[15];
    PeripheryType periph_types[15];
    PeripheryInstance instances[15];
    Instruction instructions[15];
} CPU;

typedef struct 
{

} Instruction;









