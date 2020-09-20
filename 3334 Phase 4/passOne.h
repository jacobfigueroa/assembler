/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 2
 //	File:               passOne.h
 // Description of File:
 // Pass One of the assembler. After the passOne function is
 // called, a symbol table and an intermediate files is produced
 // which will be used in pass two of the assemble.
 ***************************************************************/

#ifndef _334_Phase_2_passOne_h
#define _334_Phase_2_passOne_h

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include "passOneFunctions.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "errorTable.h"
using namespace std;



void passOne(symbolTable& symtab, opcodeTable& optab, errorTable& errtab,string filename)
{
    ifstream source;
    source.open(filename.c_str());
    ofstream intermediateFile;
    intermediateFile.open("intermediateFile.txt");
    
    string line; //holds the source line
    string programName;
    int LOCCTR,startingAddress,programLength;
    
    string label,opcode,operand;
    
    const int SIZE = 20;
    
    //words array holds the split up string. maximum of 20 words.
    //words[0] holds the label, words[1] holds the instruction and words[2] holds the operand
    string words[SIZE];
    
    int lineNumber=0; //keeps track of the number of lines in the program
    bool comment = false;
    
    if (source.is_open())
    {
        while ( !source.eof() )
        {
            getline(source,line,'\n');
            
            //initialize array that will hold up the split up string
            for(int k=0;k<SIZE;k++)
                words[k]="";
            
            //initialize label, opcode and operand
            label = "",opcode = "",operand = "";
            bool emptyLabel, emptyOpcode, emptyOperand;
            
            vector<int> errorList; //holds the errors found for a given line
            
            //check to see which fields are missing
            emptyFieldChecker(emptyLabel, emptyOpcode, emptyOperand, line);
            
            //after calling the function update the array that holds the split up text
            if(emptyLabel)
                words[0]="empty";
            if(emptyOpcode)
                words[1]="empty";
            if(emptyOperand)
                words[2]="empty";
            
            if(line[0]=='.') //if a comment, do nothing.
            {
                comment = true;
                printCommentToIntermediateFile(intermediateFile,line);
            }
            else if(line.length()>0)
            {
                lineNumber++;
                
                //split method
                split(words,line,label,opcode,operand);
                
                if(opcode=="START")
                {
                    //Perform error checks
                    if(label !="empty")
                    {
                        //check if lable is longer than 6 characters
                        if(label.length()>6)
                            errorList.push_back(errtab.getCode("Illegal label"));
                        //check to determing if the first character of the label is a letter
                        if(!isalpha(label[0]))
                            errorList.push_back(errtab.getCode("Illegal label"));
                        
                        //verify that the label is alpha numeric
                        bool validLabel=true;
                        for(int i = 0; i<label.length();i++)
                        {
                            if (!isalnum(label[i]))
                                validLabel=false;
                        }
                        if(!validLabel)
                            errorList.push_back(errtab.getCode("Illegal label"));
                        programName = label;
                        
                            
                    }
                    //check for valid operand after start make sure its not empty and that its a number
                    if(operand!="empty")
                    {
                        //check that the operand is a valid number
                        bool validAddress=true;
                        for(int i = 0; i<operand.length();i++)
                        {
                            if (!isxdigit(operand[i]))
                                validAddress=false;
                        }
                        
                        if(validAddress)
                        {
                            //convert hex string to an int and store in startingAddress variable
                            stringstream ss1;
                            ss1<<operand;
                            ss1>>hex>>startingAddress;
                        }
                        else
                        {
                            errorList.push_back(errtab.getCode("Missing or illegal operand on START directive"));
                            startingAddress=0; //set to 0 so that the assembler will still run
                        }
                    }
                    else
                    {
                        errorList.push_back(errtab.getCode("Missing or illegal operand on START directive"));
                        startingAddress=0; //set to 0 so that the assembler will still run
                    }
                    
                    LOCCTR=startingAddress;
                    
                    int x = -1; //-1 means that the opcode does not have a value
                    printToIntermediateFile(intermediateFile, line, LOCCTR, opcode, x, operand,errorList);
                }
                else if(opcode!="END")
                {
                    if(lineNumber==1)
                    {
                        LOCCTR=0;
                    }
                    //store value of locctr before its modified in the handleLineOne function
                    int oldLOCCTR=LOCCTR;
                    
                    int opcodeValue=handleLineOne(label, opcode, operand, intermediateFile, LOCCTR, symtab, optab, errorList, errtab);
                    
                    printToIntermediateFile(intermediateFile, line, oldLOCCTR, opcode, opcodeValue, operand,errorList);
                    
                }
                else if(opcode == "END")
                {
                    programLength = LOCCTR-startingAddress;
                    int x = -1; //-1 means that the opcode does not have a value
                    
                    //special end program error checks
                    if(symtab.getSize()>500)
                        errorList.push_back(errtab.getCode("Too many symbols in source program"));
                    if(LOCCTR > 32767)
                        errorList.push_back(errtab.getCode("Program too long"));
                    if(operand !=symtab.getLabel(0))
                        errorList.push_back(errtab.getCode("Missing or illegal operand on END directive"));
                    
                    printToIntermediateFile(intermediateFile, line, LOCCTR, opcode, x, operand,errorList);
                }
                
            }
        }
        
        symtab.programStartingAddress=startingAddress;
        symtab.programName=programName;
        symtab.programLength=programLength;
        source.close();
        intermediateFile.close();
        symtab.saveTable();
    }
    else
    {
        cout<<"Invalid filename, "<<filename<<" does not exist."<<endl;
    }
}


#endif
