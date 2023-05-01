// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Project2.h"
using namespace std;

const int tableRow = 5;
const int tableColumn = 8;
const string predictiveParsingTable[tableRow][tableColumn] = {
    //         a    +      -      *      /       (      )    $
            { "TQ", "",    "",    "",    "",    "TQ",  "",  ""  },    // E
            { "",   "+TQ", "-TQ", "",    "",    "",    "0", "0" },    // Q
            { "FR", "",    "",    "",    "",    "FR",  "",  ""  },    // T
            { "",   "0",   "0",   "*FR", "/FR", "",    "0", "0" },    // R
            { "a",  "",    "",    "",    "",    "(E)", "",  ""  },    // F 
};

void printStack(string stack) {
    string s = "Stack = ['$'";

    if (stack.length() == 0) {
        cout << s << "]" << endl;
    }
    else {
        cout << s << ", ";
        for (int i = stack.length() - 1; i >= 0; i--) {
            if (i != 0) {
                cout << "'" << stack[i] << "', ";
            }
            else {
                cout << "'" << stack[i] << "'";
            }
        }
        cout << "]" << endl;
    }
}

void firstCheck(vector<char>::iterator& firstVal, vector<char>& Terminal, string& terminalVal, vector<char>& NonTerminal)
{
    for (int row = 0; row < tableRow; row++) {
        if (predictiveParsingTable[row][firstVal - Terminal.begin()] != "" && terminalVal == "") {
            terminalVal += NonTerminal[row];
            printStack(terminalVal);
            terminalVal.erase(terminalVal.begin());
            terminalVal.append(predictiveParsingTable[row][firstVal - Terminal.begin()]);
            printStack(terminalVal);
        }
    }
}


int checkValidity(string& userInput, vector<char>& Terminal, vector<char>& NonTerminal)
{
    bool valid = true;
    int increment = 0;
    char currentValue = userInput[increment];
    string currentValue_string;
    string stringHolder = "";
    string terminalVal;

    // Iterators
    vector<char>::iterator terminalIterator;
    vector<char>::iterator currentValIterator;
    vector<char>::iterator nonTerminalIterator;
    vector<char>::iterator firstVal = find(Terminal.begin(), Terminal.end(), currentValue);

    firstCheck(firstVal, Terminal, terminalVal, NonTerminal);

    while (true) {
        terminalIterator = find(Terminal.begin(), Terminal.end(), terminalVal[0]);
        currentValIterator = find(Terminal.begin(), Terminal.end(), currentValue);
        nonTerminalIterator = find(NonTerminal.begin(), NonTerminal.end(), terminalVal[0]);

        if (terminalVal.length() == 0) {
            stringHolder += "$";
            if (stringHolder != userInput) {
                valid = false;
            }
            else {
                cout << "The string " << userInput << " is accepted and valid" << endl;
                return 0;
            }
        }
        else if (terminalIterator != Terminal.end()) {
            if (terminalVal[0] == currentValue) {
                increment += 1;
                currentValue = userInput[increment];
                stringHolder += terminalVal[0];
                terminalVal.erase(terminalVal.begin());
                printStack(terminalVal);
            }
            else {
                valid = false;
            }
        }
        else {
            int nonTerminal_Position = nonTerminalIterator - NonTerminal.begin();
            int currentValue_Position = currentValIterator - Terminal.begin();
            int terminalEpsilon_Position = 7;

            if (predictiveParsingTable[nonTerminal_Position][currentValue_Position] != "") {
                currentValue_string += currentValue;

                if (predictiveParsingTable[nonTerminal_Position][currentValue_Position] == currentValue_string) {
                    terminalVal.replace(0, 1, predictiveParsingTable[nonTerminal_Position][currentValue_Position]);
                    printStack(terminalVal);
                    stringHolder += currentValue;
                    increment += 1;
                    currentValue = userInput[increment];
                    terminalVal.erase(terminalVal.begin());
                    printStack(terminalVal);
                }
                else if (predictiveParsingTable[nonTerminal_Position][currentValue_Position] == "0") {
                    terminalVal.erase(terminalVal.begin());
                    printStack(terminalVal);
                }
                else {
                    terminalVal.replace(0, 1, predictiveParsingTable[nonTerminal_Position][currentValue_Position]);
                    printStack(terminalVal);
                }

                currentValue_string = "";
            }

            else if (predictiveParsingTable[nonTerminal_Position][currentValue_Position] == "" && predictiveParsingTable[nonTerminal_Position][terminalEpsilon_Position] != "") {
                terminalVal.replace(0, 1, predictiveParsingTable[nonTerminal_Position][terminalEpsilon_Position]);
                terminalVal.erase(terminalVal.begin());
                printStack(terminalVal);
            }

            else if (predictiveParsingTable[nonTerminal_Position][currentValue_Position] == "" && predictiveParsingTable[nonTerminal_Position][terminalEpsilon_Position] == "") {
                printStack(terminalVal);
                valid = false;
            }
        }

        if (!valid) {
            cout << "The string " << userInput << " is not accepted.";
            return 0;
        }
    }
}


int main()
{
    vector<char> NonTerminal = { 'E', 'Q', 'T', 'R', 'F' };
    vector<char> Terminal = { 'a', '+', '-', '*', '/', '(', ')', '$'};
    
    string input1 = "(a+a)*a$";
    string input2 = "a*(a/a)$";
    string input3 = "a(a+a)$";

    checkValidity(input1, Terminal, NonTerminal);
    checkValidity(input2, Terminal, NonTerminal);
    checkValidity(input3, Terminal, NonTerminal);
    return 0;
}
