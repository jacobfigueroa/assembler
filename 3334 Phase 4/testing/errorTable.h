/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 2
 //	File:               errorTable.h
 // Description of File:
 // The error table contains a vector which holds error messages
 // and its corresponding error codes
 ***************************************************************/

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#ifndef _334_Phase_2_errorTable_h
#define _334_Phase_2_errorTable_h

class errorTable
{
private:
    //node which holds an error message and its error code
    class node
	{
	public:
        string errorMessage;
        int errorCode;
        node(string errMessage, int errCode)
        {
            errorMessage=errMessage;
            errorCode=errCode;
        }
	};
    vector<node*> table;
    
public:
	
    //fill the table with error messages and error codes
	errorTable()
	{
        table.push_back(new node("Duplicate labels", 10));
        table.push_back(new node("Illegal label", 11));
        table.push_back(new node("Illegal operation", 12));
        table.push_back(new node("Missing or illegal operand on data storage directive", 13));
        table.push_back(new node("Missing or illegal operand on START directive", 14));
        table.push_back(new node("Missing or illegal operand on END directive", 15));
        table.push_back(new node("Too many symbols in source program", 16));
        table.push_back(new node("Program too long", 17));
        table.push_back(new node("Constant is more than 32 hex digits (16 bytes) long", 18));
        table.push_back(new node("Constant contains an odd ammount of hex digits", 19));
        table.push_back(new node("More than 30 characters in character string", 20));
        table.push_back(new node("RSUB does not require an operand",21));
        table.push_back(new node("Constant requires only hex digits",22));
        table.push_back(new node("Mising first single quote",23));
        table.push_back(new node("Mising second single quote",24));
        table.push_back(new node("Missing both single quotes",25));
    }
    
	~errorTable()
	{}
    
    //search through table and look for label
    //return label address, if label is not in table
    //return -1
    int getCode(string errMessage)
    {
        for(int i=0;i<table.size();i++)
        {
            if(table[i]->errorMessage==errMessage)
                return table[i]->errorCode;
        }
        return -1;
    }
    
    //return an error message given its error code
    string getMessage(int errCode)
    {
        for(int i=0;i<table.size();i++)
        {
            if(table[i]->errorCode==errCode)
                return table[i]->errorMessage;
        }
        return "";
    }
    
    //display all error codes and messages
    void display()
    {
        for(int i=0;i<table.size();i++)
        {
            cout<<dec<<table[i]->errorCode<<" "<<table[i]->errorMessage<<endl;
        }
    }
};



#endif
