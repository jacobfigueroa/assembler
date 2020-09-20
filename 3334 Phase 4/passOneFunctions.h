/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 2
 //	File:               passOneFunctions.h
 // Description of File:
 // Stores functions that were used in pass one
 // such as, string split function, a function to determine
 // which fields are empty and handleLineOne which is defined
 // in the pass one algorithm from the book
 ***************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include "symbolTable.h"
#include "opcodeTable.h"
#include "errorTable.h"
using namespace std;

#ifndef _334_Phase_2_functions_h
#define _334_Phase_2_functions_h

//determines whether the label, opcode or operand is missing
void emptyFieldChecker(bool& emptyLab, bool& emptyOpc, bool& emptyOper, string sourceLine)
{
    //variables to be used keep track of number of spaces and tabs
    //when determining if parameters are missing
    int numSpaces=0;
    int numTabs=0;
    
    bool emptyField[3]; //hold the boolean values for the passed in the bool parameters
    for(int i = 0; i<3; i++)
        emptyField[i]=false;
    
    //go through string find out which parameters are empty
    //set the corresponding array element to "empty" if a parameter is missing
    
    //see if label is missing.
    if(sourceLine[0]=='\t' || sourceLine[0]==' ')
    {
        emptyField[0]=true;
    }
    
    //in order to determine if the opcode or operand is missing, the following algorithm
    //counts the number of continuous spaces and tabs. if there are more than 7 uninterrupted spaces
    //or 2 tabs, then the field is determined to be missing.
    if(emptyField[0])
    {
        int index=1;
        for(int i=0;i<sourceLine.length();i++)
        {
            //if 7 spaces are found without a non-space character, set the field to be empty
            if(numSpaces>7)
            {
                emptyField[index]=true;
                numSpaces=0;
                index++;
            }
            //if 2 tabs are found without a non-space character, set the field to be empty
            if(numTabs>2)
            {
                emptyField[index]=true;
                numTabs=0;
                index++;
            }
            if(sourceLine[i]==' ')
                numSpaces++;
            if(sourceLine[i]=='\t')
                numTabs++;
            //if a non-space character is found, reset numSpaces and numTabs
            if(sourceLine[i]!='\t' && sourceLine[i]!=' ')
            {
                numTabs=0;
                numSpaces=0;
                
                //move on to the next index when a space character is found
                if(sourceLine[i+1]==' ' || sourceLine[i+1]=='\t')
                {
                    index++;
                }
            }
        }
    }
    else //the case where the label isn't missing
    {
        int index = 0;
        for(int i=0;i<sourceLine.length();i++)
        {
            //if 7 spaces are found without a non-space character, set the field to be empty
            if(numSpaces>7)
            {
                emptyField[index]=true;
                numSpaces=0;
                index++;
            }
            //if 2 tabs are found without a non-space character, set the field to be empty
            if(numTabs>2)
            {
                emptyField[index]=true;
                numTabs=0;
                index++;
            }
            if(sourceLine[i]==' ')
                numSpaces++;
            if(sourceLine[i]=='\t')
                numTabs++;
            //if a non-space character is found, reset numSpaces and numTabs
            if(sourceLine[i]!='\t' && sourceLine[i]!=' ')
            {
                numTabs=0;
                numSpaces=0;
                
                //move on to the next index when a space character is found
                if(sourceLine[i+1]==' ' || sourceLine[i+1]=='\t')
                {
                    index++;
                }
            }
        }
    }
    
    //update bool variables
    emptyLab=emptyField[0];
    emptyOpc=emptyField[1];
    emptyOper=emptyField[2];
}

//based on which fields are empty, split the string and store the tokens accordingly
void split(string *tokens, string sourceLine,string& lab, string& opc, string& oper)
{
    int j=0;
    for(int i=0;i<sourceLine.length();i++)
    {
        if(sourceLine[i]!='\t' && sourceLine[i]!=' ')
        {
            //concatenate characters to the variable if the field is not empty
            if(tokens[j]!="empty")
            {
                tokens[j]+=sourceLine[i];
            }
            else
            {
                j++; //move to the next element in the array
                i--; //go back a character since an element in the array was skipped
            }
            
            //move on to next character if a space is found
            if(sourceLine[i+1] == ' ' || sourceLine[i+1] == '\t')
            {
                j++;
            }
        }
    }
    lab=tokens[0];
    opc=tokens[1];
    oper=tokens[2];
}

int handleLineOne(string& lab, string& opc, string& oper,ofstream& intFile, int& locationCounter,symbolTable& symtab, opcodeTable& optab, vector<int>& errorList, errorTable& errtab)
{
    //search symtable to see if label already exists, if it already exists throw a duplicate label error
    //else insert the label and address into the symtab
    if(lab != "empty")//only insert into the symtable if lable is not empty
    {
        //error checks for label length greater than 6 characters and first character not a letter
        if(lab.length()>6)
            errorList.push_back(errtab.getCode("Illegal label"));
        if(!isalpha(lab[0]))
            errorList.push_back(errtab.getCode("Illegal label"));
        
        //check if label is alpha numeric
        bool validLabel=true;
        for(int i = 0; i<lab.length();i++)
        {
            if (!isalnum(lab[i]))
                validLabel=false;
        }
        if(!validLabel)
            errorList.push_back(errtab.getCode("Illegal label"));
        
        //check for duplicate labels
        if(symtab.exists(lab))
        {
            errorList.push_back(errtab.getCode("Duplicate labels"));
        }
        else
        {
            symtab.storeSymbol(lab,locationCounter);
        }
    }
    
    //update location counter based on opcode
    //check opcode table and verify that the opcode is valid
    bool validOpcode;
    int opcodeVal=optab.getValue(opc,validOpcode);
    if(validOpcode)
    {
        locationCounter += 3;
        
        //error check for RSUB opcode, verify that there is no operand
        if(opc=="RSUB" && oper !="empty")
            errorList.push_back(errtab.getCode("RSUB does not require an operand"));
    }
    else if(opc == "WORD")
    {
        locationCounter += 3;
    }
    else if(opc == "RESB")
    {
        if(oper=="empty")
            errorList.push_back(errtab.getCode("Missing or illegal operand on data storage directive"));
        
        //string to int method
        stringstream ss;
        int value;
        ss << oper;
        ss >> value;
        
        locationCounter+=value;
    }
    else if(opc == "RESW")
    {
        if(oper=="empty")
            errorList.push_back(errtab.getCode("Missing or illegal operand on data storage directive"));
        
        //string to int method
        stringstream ss;
        int value;
        ss << oper;
        ss >> value;
        
        locationCounter+=3*value;
    }
    else if(opc == "BYTE")
    {
        //variable to see if both single quotes are present
        bool firstSingleQuote=false, secondSingleQuote=false;
        string constant;
        //check if there is a singlequote after C or X
        if(oper[1]=='\'')
            firstSingleQuote=true;
        
        //check if there is a singlequote at the end of the operand string
        if(oper[oper.length()-1]=='\'')
            secondSingleQuote=true;
        
        if(firstSingleQuote && secondSingleQuote)
        {
            //store constant, constant is the substring between the two single quotes
            constant = oper.substr(2,oper.length()-3);
            
            //example C'EOF'
            if(oper[0]=='C')
            {
                if(constant.length() > 30)
                    errorList.push_back(errtab.getCode("More than 30 characters in character string"));
                else
                {
                    locationCounter+=constant.length();
                }
            }
            
            //example X'F1'
            if(oper[0]=='X')
            {
                //check that it is a hex value
                bool validHex=true;
                for(int i = 0; i<constant.length();i++)
                {
                    if (!isxdigit(constant[i]))
                        validHex=false;
                }
                if(!validHex)
                    errorList.push_back(errtab.getCode("Constant requires only hex digits"));
                
                //check that the constant length is an even number
                if(constant.length() % 2 != 0)
                    errorList.push_back(errtab.getCode("Constant contains an odd ammount of hex digits"));
                //constant cannot be more than 32 hex digits long
                if(constant.length() > 32)
                    errorList.push_back(errtab.getCode("Constant is more than 32 hex digits (16 bytes) long"));
                if(constant.length() <= 32 && constant.length() % 2 == 0 && validHex) //no error
                {
                    locationCounter+=(constant.length()/2);
                }
            }
        }
        //error messages depending on which single quotes are mssing
        else if(!firstSingleQuote)
        {
            errorList.push_back(errtab.getCode("Mising first single quote"));
        }
        else if(!secondSingleQuote)
        {
            errorList.push_back(errtab.getCode("Mising second single quote"));
        }
        else
        {
            errorList.push_back(errtab.getCode("Missing both single quotes"));
        }
    }
    else //if opcode is not in table, then it is an illegal operation
    {
        errorList.push_back(errtab.getCode("Illegal operation"));
    }
    
    return opcodeVal;
}

//print to intermediate file
void printToIntermediateFile(ofstream& intFile, string sourceLine,int address, string opcode, int opcodeValue, string operand, vector<int>& errorList)
{
    //print source line
    intFile<<sourceLine<<endl;
    
    //print address
    intFile<<hex<<address<<endl;
    
    //print opcode or value
    if(opcodeValue==-1)
        intFile<<dec<<-1<<" "<<opcode<<endl;
    else
        intFile<<hex<<opcodeValue<<endl;
    
    //print operand
    intFile<<operand<<endl;
    
    //print error codes
    if(errorList.size() != 0)
    {
        for(int i=0; i<errorList.size(); i++)
            intFile<<dec<<errorList[i]<<" ";
    }
    else
    {
        intFile<<0;
    }
    intFile<<endl;
    if(opcode != "END")
        intFile<<"--------------------------------------------------------------------------"<<endl;
    else
        intFile<<"*END OF INTERMEDIATE FILE*";
}

void printCommentToIntermediateFile(ofstream& intFile, string comment)
{
    intFile<<comment<<endl;
    intFile<<"--------------------------------------------------------------------------"<<endl;
}
#endif
