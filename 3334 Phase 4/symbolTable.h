/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 2
 //	File:               symbolTable.h
 // Description of File:
 // The symbol table contains a vector which holds labels
 // and their corresponding address.
 ***************************************************************/

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#ifndef _334_Phase_2_symbolTable_h
#define _334_Phase_2_symbolTable_h

class symbolTable
{
private:
    ofstream symbolTableFile; //file to be created to save symbol table
    //node which holds a label (symbol) and its address value
    class node
	{
	public:
        string symbol;
        int address;
        node(string sym, int addr)
        {
            symbol=sym;
            address=addr;
        }
	};
    vector<node*> table;
    
public:
    string programName;
    int programStartingAddress, programLength;
	
	symbolTable()
	{}
    
	~symbolTable()
	{}
    
    //search through table and look for label
    //return label address, if label is not in table
    //return -1
    int getAddress(string label)
    {
        for(int i=0;i<table.size();i++)
        {
            if(table[i]->symbol==label)
                return table[i]->address;
        }
        return -1;
    }
    
    //return the label stored at a given index
    string getLabel(int index)
    {
        return table[index]->symbol;
    }
    
    //return size of the table
    int getSize()
    {
        return table.size();
    }
    
    //boolean function to check if a label already exists in the table
    bool exists(string label)
    {
        for(int i=0;i<table.size();i++)
        {
            if(table[i]->symbol==label)
                return true;
        }
        return false;
    }
    
    //insert a label and labelAddress into symbol table
    void storeSymbol(string label, int labelAddress)
    {
        table.push_back(new node(label, labelAddress));
    }
    
    //save table as a text document
    void saveTable()
    {
        symbolTableFile.open("symbolTableFile.txt");
        for(int i=0;i<table.size();i++)
        {
            symbolTableFile<<table[i]->symbol<<" "<<hex<<table[i]->address<<endl;
        }
        symbolTableFile.close();
    }
    
    //displays all labels and addresses stored in the symbol table
    void display()
    {
        for(int i=0;i<table.size();i++)
        {
            cout<<table[i]->symbol<<" "<<hex<<table[i]->address<<endl;
        }
    }
    
    void printTo(ofstream& file)
    {
        file<<endl<<"Symbol Table"<<endl;
        for(int i=0;i<table.size();i++)
        {
            /*
            if(table[i]->symbol.length()<4)
                file<<table[i]->symbol<<"\t\t"<<hex<<table[i]->address<<endl;
            else*/
                file<<table[i]->symbol<<"\t"<<hex<<table[i]->address<<endl;
        }
    }
};

#endif
