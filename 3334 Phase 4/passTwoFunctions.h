/****************************************************************
 // Name:               Jacob Figueroa
 // ID:                 20056963
 // CS Lab Username:    jafigueroa
 //	Assignment:         Phase 3
 //	File:               passTwoFunctions.h
 // Description of File:
 // Stores functions that were used in pass one
 // such as, string split function, a function to determine
 // which fields are empty and handleLineOne which is defined
 // in the pass one algorithm from the book
 ***************************************************************/

#ifndef _passTwoFunctions_h
#define _passTwoFunctions_h

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


//capitalizes a string
string toUpper(string s)
{
    int i = 0;
    string temp = "";
    while(s[i])
    {
        temp += toupper(s[i]);
        i++;
    }
    
    return temp;
}

//given an int n, convert it to a string of hex characters
string convertDecToHexString(int n)
{
    stringstream ss;
    string hexString;
    ss<<hex<<n;
    ss>>hexString;
    return hexString;
}

//given a string and a string array
//tokenize the string
void splitString(string * tokens, string line)
{
    int j=0;
    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] != ' ')
        {
            tokens[j]+=line[i];
            if(line[i+1] == ' ')
                j++;
        }
    }
}

//remove spaces from a given string
void getRidOfSpaces(string& s)
{
    string temp = "";
    for(int i = 0; i<s.length(); i++)
    {
        if(s[i]!=' ')
            temp+=s[i];
    }
    s = temp;
}

//convert a string to and int
int stringToInt(string& s)
{
    stringstream ss(s);
    int result;
    ss >> result;
    return result;
}

//add zeroes to the front of a string
void addZeroes(int n, string & s)
{
    string temp;
    int zeroesToAdd = n - s.length();
    for(int i = 0; i<zeroesToAdd; i++)
        temp += "0";
    s=temp+s;
}

#endif
