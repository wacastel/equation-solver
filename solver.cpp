//
//  solver.cpp
//  EquationSolver
//
//  Created by William Castellano on 9/23/17.
//  Copyright © 2017 William Castellano. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "solver.h"

using namespace std;

int main(int argc, const char * argv[]) {
    vector<string> lhs;
    vector< vector<string> > rhsVariables;
    vector<long> sums;
    rhsLists lists;
    vector<solvedEquation> solvedList;
    string filename;
    
    // Read in the filename from the command line
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " FILENAME" << endl;
        return 1;
    } else {
        filename = argv[1];
        cout << endl << "Input Filename: " << filename << endl << endl;
    }
    
    // Read the list of equations from a file into a list of unparsed strings
    vector<string> equationList = getList(filename);
    cout << "Input Equations:" << endl;
    printInput(equationList);
    cout << endl;

    // Parse the input strings and create the parsed equation lists
    for (int i = 0; i < equationList.size(); i++) {
        string strippedLine = removeWhitespace(equationList.at(i));
        lhs.push_back(getLeft(strippedLine));
        string rightSide = getRight(strippedLine);
        lists = parseString(rightSide);
        rhsVariables.push_back(lists.vars);
        sums.push_back(lists.sum);
    }
    
    long safetyIndex = 0;
    
    // This is the main loop for the equation solver
    // Keep attempting to solve the equations until they're all solved or until 100 iterations are reached
    while (solvedList.size() < lhs.size() && safetyIndex < 100) {
        
        // Get the list of solved equations
        solvedList = checkSolved(lhs, rhsVariables, sums);
        
        if (solvedList.size() == lhs.size()) {
            break;
        }
        
        // Here is where we substitute solved values into the rest of the unsolved equations!
        // Match the LHS variable name with the RHS list of variables in each equation and
        // perform substitutions if a match is found
        
        for (int index = 0; index < solvedList.size(); index++) {
            string equName = solvedList.at(index).lhs;
            for (int i = 0; i < rhsVariables.size(); i++) {
                vector<string> tempList = rhsVariables.at(i);
                vector<string> newList;
                for (int j = 0; j < tempList.size(); j++) {
                    if (equName == tempList.at(j)) {
                        sums.at(i) += solvedList.at(index).rhs;
                        tempList.at(j) = "-1"; // mark the solved variable for deletion
                    }
                }
                rhsVariables.at(i) = tempList;
            }
        }
        
        // remove the marked entries from rhsVariables
        rhsVariables = removeMarkedForDeletion(rhsVariables);
        
        safetyIndex++;
    }
    
    cout << "Solved Equations:" << endl;
    printSolvedList(solvedList);
    cout << endl;

    return 0;
}

// Loop through lhs and check the rhsVariables size
// If size is zero, we have a solved equation
// Store in solved equations list
vector<solvedEquation> checkSolved(vector<string> lhs, vector< vector<string> > rhsVariables, vector<long> sums) {
    vector<solvedEquation> solvedList;
    vector<string> tempList;
    solvedEquation solved;
    for (int i = 0; i < lhs.size(); i++) {
        tempList = rhsVariables.at(i);
        if (tempList.size() == 0) {
            solved.lhs = lhs.at(i);
            solved.rhs = sums.at(i);
            solvedList.push_back(solved);
        }
    }
    return solvedList;
}

// Prints out the input equations
void printInput(vector<string> input) {
    for (int i = 0; i < input.size(); i++) {
        cout << input.at(i) << endl;
    }
}

void printVector(vector<string> input) {
    for (int i = 0; i < input.size(); i++) {
        cout << "[" << input.at(i) << "]" << endl;
    }
}

// Comparison function for the std::sort algorithm used in printSolvedList below
bool sortByName(const solvedEquation &equ1, const solvedEquation &equ2) {
    return (equ1.lhs < equ2.lhs);
}

// Sort and print the list of solved equations
void printSolvedList(vector<solvedEquation> input) {
    sort(input.begin(), input.end(), sortByName);
    for (int i = 0; i < input.size(); i++) {
        cout << input.at(i).lhs << " = " << input.at(i).rhs << endl;
    }
}

void printNumVector(vector<long> input) {
    for (int i = 0; i < input.size(); i++) {
        cout << "[" << input.at(i) << "]" << endl;
    }
}

void printVectorFormatted(vector<string> input) {
    for (int i = 0; i < input.size(); i++) {
        if (i < input.size() - 1) {
            cout << input.at(i) << ", ";
        } else {
            cout << input.at(i);
        }
    }
}

void printDoubleVector(vector< vector<string> > input) {
    for (int i = 0; i < input.size(); i++) {
        cout << "[";
        printVectorFormatted(input.at(i));
        cout << "]" << endl;
    }
}

string getLeft(string input) {
    string delimiter = "=";
    return input.substr(0, input.find(delimiter));
}

string getRight(string input) {
    string delimiter = "=";
    return input.substr(input.find(delimiter)+1, input.length());
}

rhsLists parseString(string input) {
    rhsLists tempLists;
    tempLists.sum = 0;
    string delimiter = "+";
    
    long start = 0;
    long end = input.find(delimiter);
    
    do {
        end = input.find(delimiter, start);
        if (end == string::npos) {
            end = input.length();
        }
        string currentVal = input.substr(start, end - start);
        int intVal = atoi(currentVal.c_str());
        if (intVal != 0) {
            tempLists.sum += intVal;
        } else {
            tempLists.vars.push_back(currentVal);
        }
        start = end + delimiter.length();
    }
    while (end < input.length() && start < input.length());
    return tempLists;
}

vector<string> getList(string filename) {
    string line;
    vector<string> equationList;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            equationList.push_back(line);
        }
        inputFile.close();
    } else {
        cout << "Unable to read input file!" << endl;
    }
    return equationList;
}

vector< vector<string> > removeMarkedForDeletion(vector< vector<string> > input) {
    vector< vector<string> > cleanVarList;
    vector<string> newList;
    vector<string> origList;
    for (int i = 0; i < input.size(); i++) {
        origList = input.at(i);
        for (int j = 0; j < origList.size(); j++) {
            if (origList.at(j) != "-1") {
                newList.push_back(origList.at(j));
            }
        }
        cleanVarList.push_back(newList);
        if (newList.size() > 0) {
            newList.clear();
        }
    }
    return cleanVarList;
}

// Strips one or more whitespace characters from the input array
string removeWhitespace(string input) {
    string::iterator endPos = remove(input.begin(), input.end(), ' ');
    input.erase(endPos, input.end());
    return input;
}
