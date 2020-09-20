/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 3
 //	File:               passTwo.h
 // Description of File:
 // Pass two of the assembler, this pass creates an object file
 // which contains the head, text and end records. In addition
 // a listing file is created which contains the object code,
 // address and source code. Also errors are displayed after
 // the line of code that produces the error.
 ***************************************************************/

#ifndef _passTwo_h
#define _passTwo_h

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include "symbolTable.h"
#include "opcodeTable.h"
#include "errorTable.h"
#include "passTwoFunctions.h"

using namespace std;

// pass two, the symboltable and opcode that were produced in pass one are
// passed in.
void passTwo(symbolTable& symtab, opcodeTable& optab, errorTable& errtab)
{
    // open the files that are needed for pass two
    ifstream intermediateFile;
    intermediateFile.open("intermediateFile.txt");
    ofstream objectFile;
    objectFile.open("objectFile.txt");
    ofstream listingFile;
    listingFile.open("listingFile.txt");
    
    bool error=false; // variable that checks if any errors are produced
    if (intermediateFile.is_open())
    {
        string programName, programStartingAddress, programLength;
        programName = symtab.programName;
        
        // conver the int value of starting address and progaram length into a string
        // of hex characters
        programStartingAddress = convertDecToHexString(symtab.programStartingAddress);
        programLength = convertDecToHexString(symtab.programLength);
        
        //construct header record
        if(!intermediateFile.eof())
        {
            //initalize header record with H and the program name
            string headerRecord = "H"+programName;
            
            //determine how many spaces to add to program name
            int spacesToAdd = 6 - programName.length();
            for(int i = 0; i<spacesToAdd; i++)
                headerRecord += " ";
            
            //add zeroes so that starting address takes up 6 columns
            addZeroes(6, programStartingAddress);
            headerRecord+=programStartingAddress;
            
            //add zeroes so that program length takes up 6 columns
            addZeroes(6, programLength);
            headerRecord+=programLength;
            
            headerRecord = toUpper(headerRecord);
            objectFile<<headerRecord<<endl;
        }
        //construct text and end records along with listing file
        string textRecord = "T";
        string objectCodeLine,oldObjectCodeLine;
        bool firstline = true;
        while ( !intermediateFile.eof() )
        {
            string sourceLine; // string to hold the source code
            getline(intermediateFile,sourceLine);
            
            // if statment to insure that any extraneous lines at the end of the intermediate
            // file are ignored
            if (sourceLine=="*END OF INTERMEDIATE FILE*")
            {
                break;
            }
            else if (sourceLine=="--------------------------------------------------------------------------")
            {
                //if the source line is "-----", ignored it
            }
            else if(sourceLine[0] == '.')
            {
                //write comment to listing file
                listingFile<<sourceLine<<endl;
            }
            else
            {
                
                string address,opcode,operand,errorLine,objectCode;
                objectCode=""; //initialize objectcode for the line
                
                //read in values from intermediate file
                intermediateFile>>address;
                intermediateFile>>opcode;
                
                bool validOpcode = true;
                if(opcode=="-1") // -1 indicated that the mneumonic was stored and not the value
                {
                    validOpcode = false;
                    intermediateFile>>opcode;
                }
                
                intermediateFile>>operand;
                intermediateFile.get(); //needed to read the next line
                getline(intermediateFile,errorLine,'\n');
                
                if(!validOpcode) //the case where the opcode doesnt have a value (eg. BYTE)
                {
                    if(opcode == "BYTE")
                    {
                        //object code = value of constant
                        string constant = operand.substr(2,operand.length()-3);
                        if(operand[0] == 'C')
                        {
                            //convert constant to hex string
                            stringstream ss;
                            for (int i = 0; i < constant.length(); i++)
                            {
                                char x = constant[i];
                                objectCode += convertDecToHexString(int(x));
                            }
                        }
                        else // eg. X'F1'
                            objectCode=constant;
                    }
                    if(opcode=="WORD")
                    {
                        objectCode = convertDecToHexString(stringToInt(operand));
                        addZeroes(6, objectCode);
                    }
                }
                else
                {
                    addZeroes(2, opcode);
                    
                    if(operand == "empty")
                    {
                        operand = "0000";
                    }
                    else if(operand[operand.length()-2] == ',' && operand[operand.length()-1] == 'X') // case where index bit is set
                    {
                        operand = operand.substr(0,operand.length()-2);
                        // get the index bit
                        // add 0x8000 since the index bit is set
                        operand = convertDecToHexString(symtab.getAddress(operand)+0x8000);
                    }
                    else
                    {
                        //get the address from the symbol table and and convert to a hex string
                        operand=convertDecToHexString(symtab.getAddress(operand));
                    }
                    addZeroes(4, operand);
                    objectCode=opcode+operand;
                }
                
                //change the object code to uppercase
                // eg. 107a -> 107A
                objectCode=toUpper(objectCode);
                
                // add zeroes
                string temp;
                int zeroesToAdd = 4 - address.length();
                for(int i = 0; i<zeroesToAdd; i++)
                    temp += "0";
                address=temp+address;
                
                //Output the generated information to the listing file
                //capitalize the address
                listingFile<<toUpper(address)<<"    ";
                
                if(objectCode=="")
                    listingFile<<"      ";
                else
                {
                    listingFile<<objectCode;
                    int spacesToAdd = 6 - objectCode.length();
                    for(int i = 0; i<spacesToAdd; i++)
                        listingFile<<" ";
                }
                
                // if the label is absent in the source line, add spaces to line up the columns
                if(isspace(sourceLine[0]))
                    listingFile<<"          "<<sourceLine<<endl;
                else
                    listingFile<<"      "<<sourceLine<<endl;
                
                //display error messages
                if(errorLine[0] != '0')
                {
                    error=true;
                    string tokens[10];
                    splitString(tokens,errorLine);
                    for(int i=0; i<10; i++)
                    {
                        if(tokens[i]!="")
                        {
                            listingFile<<"\t\t\t\t\tERROR: "<<errtab.getMessage(stringToInt(tokens[i]))<<endl;
                        }
                    }
                }
                
                // dont generate object code for the first line
                if(objectCode==""&&!firstline)
                {
                    objectCode="      ";
                }
                
                //store the old object code before adding to it
                oldObjectCodeLine=objectCodeLine;
                objectCodeLine+=objectCode;
                
                // if after adding new object code and the length is too long
                // revert back to the old object code line. Initialize a new text record if
                // the length is too long and write the old text record to the object file.
                if(objectCodeLine.length()>60)
                {
                    //rever to old object code
                    objectCodeLine=oldObjectCodeLine;
                    getRidOfSpaces(objectCodeLine);
                    
                    //find the length of object code, this will be stored in the text record
                    string lengthOfObjectCode = convertDecToHexString(objectCodeLine.length()/2);
                    
                    //add a zero the length if the hex string is only one digit
                    // eg. A -> 0A
                    if(lengthOfObjectCode.length()==1)
                        lengthOfObjectCode = "0" + lengthOfObjectCode;
                    
                    //add the length and the object code to the text record
                    textRecord+=lengthOfObjectCode;
                    textRecord+=objectCodeLine;
                    
                    //capitalize the text record
                    textRecord=toUpper(textRecord);
                    
                    //output to text record to object file
                    objectFile<<textRecord<<endl;
                    
                    //initialize the new text record
                    objectCodeLine=objectCode;
                    textRecord = "T00" + address;
                }
                else if(objectCodeLine.length()<=6)
                {
                    textRecord = "T00" + address;
                    oldObjectCodeLine="";
                }
            }
            firstline=false;
            
        }
        
        //generate last text record
        getRidOfSpaces(objectCodeLine);
        
        //find the length of object code, this will be stored in the text record
        string lengthOfObjectCode = convertDecToHexString(objectCodeLine.length()/2);
        
        //add a zero the length if the hex string is only one digit
        // eg. A -> 0A
        if(lengthOfObjectCode.length()==1)
            lengthOfObjectCode = "0" + lengthOfObjectCode;
        
        //add the length and the object code to the text record
        textRecord+=lengthOfObjectCode;
        textRecord+=objectCodeLine;
        
        //capitalize the text record
        textRecord=toUpper(textRecord);
        
        //output to text record to object file
        objectFile<<textRecord<<endl;
        
        //make end record
        string endRecord = "E";
        
        endRecord += programStartingAddress;
        endRecord = toUpper(endRecord);
        objectFile<<endRecord;
        
        //print symbol table end of listing file
        symtab.printTo(listingFile);
                
        //close all files
        intermediateFile.close();
        objectFile.close();
        listingFile.close();
        
        //if there is an error found in the object code, delete the file
        if(error)
            remove("objectFile.txt");
    }
}


#endif
