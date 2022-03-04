#include "VirtualPLC.h"
#include "fstream"
#include "iostream"
#include "cstring"

using namespace std;

LineReader ln_reader;

Line getNext(const char source[], Line curr_line)
{
    Line line;
    char str[20];
    char *ptr;
    std::ifstream source_code;

    line.args_count = 0;
    source_code.open(source);
    if(!source_code)
    {
        cout << "Unable to open source code" << endl;
        exit(1);
    }

    for(int i = 0; i < curr_line.line_number; ++i)
    {
       source_code.getline(str, 0);
    }
    source_code.getline(str,20);

    ptr = strtok(str," ");

    if(ptr != NULL)
    {
        line.instruction = ptr;
        ptr = strtok(NULL, " ");
    }

    while(ptr != NULL)
    {
        line.args[line.args_count] = ptr;
        line.args_count++;
        ptr = strtok(NULL, " ");
    }

    cout << line.instruction << endl;

    for(int i=0; i < line.args_count; i++) {
        cout << line.args[i] << endl; 
    }
    line.line_number++;
    return line;
}

int main()
{
    char source[20] = "source.txt";
    Line current;
    current.line_number=0;
    ln_reader.get_next_line = &getNext;

    current = ln_reader.get_next_line(source,current);

    cout << current.instruction << endl;

    for(int i=0; i < current.args_count; i++) {
        cout << current.args[i] << endl; 
    }
    cout << current.args_count;
    cout << current.line_number;

    ln_reader.get_next_line(source, current);
}