/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 1/4
 //	File:               main.cpp
 // Description of File:
 // This file contains the code for a command interpreter.
 // When the assemble command is called, pass one and two of the
 // assembler is called. The load command is used to load an
 // object file into memory. Dump displays the contents of
 // the memory. Execute runs the code that was put into memory.
 ***************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctype.h>
#include <cctype>
#include "passOne.h"
#include "passTwo.h"
#include "passTwoFunctions.h"
#include "sicengine.c"

using namespace std;

/* Function prototypes that are used to interact with sicengine.c */
int load(string filename);
void dump(int address1, int address2);

/* Function prototypes */
void split(string * tokens, string line);
int hexStringToInt(string s);
bool isValidHex(string hex);

int main()
{

	//initialize sic engine
	SICInit();

    string command;
    unsigned long startAddress;
    
    bool running=true;
    
    while(running)
    {
        bool validCommand=false;
        cout << "Command> ";

        //get command
        getline (cin,command);
        

        string tokens[10]; //string array to hold the tokenized command
        split(tokens,command);	//tokenize the string
        
        if(tokens[0]=="load")
        {
            validCommand=true;
            if(tokens[1] != "")
            {
                int s_a = load(tokens[1]);
                startAddress = (unsigned long)s_a;
            }
            else
                cout<<tokens[0]<<" requires a parameter"<<endl;
        }
        
        if(tokens[0]=="execute")
        {
            validCommand=true;
            if(tokens[1] == "")
                SICRun(&startAddress, 0);
            else
                cout<<tokens[0]<<" does not require a parameter"<<endl;
        }
        
        if(tokens[0]=="debug")
        {
            validCommand=true;
            if(tokens[1] == "")
            {
                cout<<"At the moment the "<<tokens[0]<<" command has not been implemented."<<endl;
                //debug();
            }
            else
                cout<<tokens[0]<<" does not require a parameter"<<endl;
        }
        
        if(tokens[0]=="dump")
        {
            validCommand=true;
            
            if(tokens[1] != "" && tokens[2] != "")
            {
                bool validHex1=isValidHex(tokens[1]);
                bool validHex2=isValidHex(tokens[2]);

                if(!validHex1 || !validHex2)
                    cout<<"dump requires two hexadecimal parameters"<<endl;
                else
                {
                	int addr1 = hexStringToInt(tokens[1]);
    				int addr2 = hexStringToInt(tokens[2]);

   		 			if(addr1 > addr2)
    					cout<<"The second hex address must not be bigger than the first address."<<endl;
    				else
                    	dump(addr1, addr2);
                }
            }
            else
                cout<<tokens[0]<<" requires two hexadecimal parameters"<<endl;
                
        }
        
        if(tokens[0]=="help")
        {
            validCommand=true;
            if(tokens[1] == "")
            {
                cout<<endl<<"Available commands:"<<endl
                <<"load <filename>"<<endl
                <<"execute"<<endl
                <<"debug"<<endl
                <<"dump <start> <end>"<<endl
                <<"help"<<endl
                <<"assemble <filename>"<<endl
                <<"directory"<<endl
                <<"exit"<<endl<<endl
                <<"NOTE: Commands must be lowercase, parameters are case dependent"<<endl;
            }
            else
                cout<<tokens[0]<<" does not require a parameter"<<endl;
        }
        
        
        if(tokens[0]=="assemble")
        {
            validCommand=true;
            if(tokens[1] != "")
            {
                string filename = tokens[1];
                
                //assemble
                symbolTable symtab;
                opcodeTable optab;
                errorTable errtab;
                passOne(symtab,optab,errtab,filename);
                passTwo(symtab,optab,errtab);
            }
            else
                cout<<tokens[0]<<" requires a parameter"<<endl;
        }
        
        if(tokens[0]=="directory")
        {
            validCommand=true;
            if(tokens[1] == "")
                system("ls");
            else
                cout<<tokens[0]<<" does not require a parameter"<<endl;
        }
        
        if(tokens[0]=="exit")
        {
            validCommand=true;
            running=false;
        }
        			        	
        if(!validCommand)
            cout<<"Enter help for a list of available commands and examples of correct syntax"<<endl;
        
    }
    
    return 0;
}

/* objectFile.txt is opened and the objectcode is loaded into memory using the PutMem function from sicengine.c */
int load(string filename)
{
	ifstream objectFile;
    objectFile.open(filename.c_str());
    int startingaddr;
    
    if (objectFile.is_open())
    {
        while ( !objectFile.eof() )
        {
            string line;
            getline(objectFile,line);
            if(line[0] == 'T')
            {
                string address = line.substr(1,6);
                int addr = hexStringToInt(address);

                string objectCode = line.substr(9);

                //go through the object code, 2 characters at a time and load them into memory
                for(int i = 0; i<objectCode.length(); i+=2)
                {	
                    string s_byte;	//string to hold the 2 characters 
                    s_byte += objectCode[i];	//store first character
                    s_byte += objectCode[i+1];	//store second character

                    int byte = hexStringToInt(s_byte);
                    unsigned char c = byte;
                    PutMem(addr,&c,0);
                    addr++;
                }
            }
            if(line[0] == 'E')
            {
                startingaddr = hexStringToInt(line.substr(1,6));
            }

        }
    }
    objectFile.close();
    return startingaddr;
}

/* Displays the memory address, followed by the contents for each address between address 1 and 2. */
void dump(int address1, int address2)
{
    unsigned char byte; //used to store the contents

    int display = 0; //variable used to nice formatting.
    for(int i = address1; i<address2; i++)
    {
    	// display bytes in groups of 16
    	if(display==0)
    		cout<<hex<<i<<": ";
    	else 
    	{	
    		if(display % 24 == 0)
        		cout<<endl<<hex<<i<<": ";
    		else if(display % 8 == 0)
        		cout<<" "<<hex<<i<<": ";
    	}

    	//get memory and store in byte
        GetMem (i, &byte, 0);
        cout<<hex<<setfill ('0') <<setw (2)<<int((&byte)[0])<<"";

        display++;
    }
    cout<<endl;
}

/* Given a string, the function tokenizes the string by removing spaces and tabs*/
void split(string * tokens, string line)
{
    int j=0;
    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] != ' ' && line[i] != '\t')
        {
            tokens[j]+=line[i];
            if(line[i+1] == ' ')
                j++;
        }
    }
}

/* A hex string is passed as a parameter and converted to an int which is then returned */
int hexStringToInt(string s)
{
    int n;
    stringstream ss;
    ss<<s;
    ss>>hex>>n;
    return n;
}

/* returns true if a given hex string is comprised of valid hex digits */
bool isValidHex(string hex)
{
	//loop through each character in hex string
	//if the chararacter is not a valid hex digit return false
	for(int i = 0; i<hex.length(); i++)
    {
        if(!isxdigit(hex[i]))
        	return false;
    }
    return true;
}
