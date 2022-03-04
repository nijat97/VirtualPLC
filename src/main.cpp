#include "VirtualPLC.h"
#include "fstream"
#include "iostream"
#include "string"
#include "sstream"

using namespace std;

LineReader ln_reader;

Line getNext(const char source[], Line curr_line)
{
    Line line;
    string str,word;
    std::ifstream source_code;
    int i;

    line.args_count = 0;
    line.line_number = curr_line.line_number;

    source_code.open(source);
    if(!source_code)
    {
        cout << "Unable to open source code" << endl;
        exit(1);
    }

    for(i = 0; i < line.line_number; i++)
    {
       getline(source_code, str);
    }
    getline(source_code,str);

    istringstream ss(str);
    i=0;
    while (ss >> word) 
    {
        if(i==0)
        {
            line.instruction = word;
        }
        else
        {
            line.args[line.args_count] = word;
            line.args_count++;
        }
        i++;
    }
    line.line_number++;

    source_code.close();
    return line;
}

int main()
{
    char source[20] = "source.txt";
    Line current;
    current.line_number=0;
    ln_reader.get_next_line = &getNext;

    for(int i=0;i<2;i++)
    {
        current = ln_reader.get_next_line(source,current);

        cout << current.instruction << endl;

        for(int i=0; i < current.args_count; i++) {
            cout << current.args[i] << endl; 
        }
        cout << current.args_count<< endl;
        cout << current.line_number;
        
    }
}