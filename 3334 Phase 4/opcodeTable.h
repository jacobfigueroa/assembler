/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 2
 //	File:               symbolTable.h
 // Description of File:
 // The opcode table contains a vector which holds opcodes
 // and their corresponding values
 ***************************************************************/

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#ifndef _334_Phase_2_opcodeTable_h
#define _334_Phase_2_opcodeTable_h

class opcodeTable
{
private:
    ifstream opTabSource; //file to be read which contains the values for the opcode
    
    //node which holds an opcode and a value
    class node
	{
	public:
        string opcode;
        int code;
        node(string opc, int value)
        {
            opcode = opc;
            code = value;
        }
	};
    vector<node*> table;
    
public:
	
    //construct the opcode table by reading in the optab.txt file
	opcodeTable()
	{
        opTabSource.open("optab.txt");
        if (opTabSource.is_open())
        {
            while ( !opTabSource.eof() )
            {
                string opc; //holds opcode
                string valuehex; //holds hex string which will later be converted to an it
                int value; //int value of the hex string
                
                opTabSource>>opc;
                opTabSource>>valuehex;
                
                //hex string to int function
                stringstream ss;
                ss<<valuehex;
                ss>>hex>>value;
                
                //store in table
                table.push_back(new node(opc, value));
            }
        }
        
    }
    
	~opcodeTable()
	{}
    
    //displays all opcodes and values stored in the opcode table
    void display()
    {
        for(int i=0;i<table.size(); i++)
        {
            cout<<table[i]->opcode<<" "<<hex<<table[i]->code<<endl;
        }
    }
    
    //given an opcode and a boolean variable which represents if the opcode is in the table
    //return the opcode value, if the opcode does not exist in the table return a -1
    int getValue(string opc, bool& valid)
    {
        for(int i=0;i<table.size();i++)
        {
            if(opc == table[i]->opcode)
            {
                valid=true;
                return table[i]->code;
            }
        }
        valid=false;
        return -1;
    }
};


#endif
