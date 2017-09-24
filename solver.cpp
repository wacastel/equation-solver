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

Solver::Solver(string fname)
    :filename(fname)
{}

/**
    Solves the list of equations by parsing them, substituting in values and checking for solved equations.
    The algorithm reads a text file of equations and outputs the solution to the console.
 */
void Solver::solveIt() {
    // Read the list of equations from a file into a list of unparsed strings
    vector<string> equationList = getList(filename);
    
    if (equationList.size() == 0) {
        return;
    }
    
    parseEquationStrings(equationList);
    
    long safetyIndex = 0;
    solvedList.clear();
    
    // This is the main loop for the equation solver
    // Keep attempting to solve the equations until they're all solved or until 100 iterations are reached
    while (solvedList.size() < lhs.size() && safetyIndex < 100) {
        solvedList = checkSolved(); // Get the list of solved equations
        if (solvedList.size() < lhs.size()) {
            substituteValues(); // Substitute known variables back into the unsolved equations
            safetyIndex++;
        }
    }
    // Check if safetyIndex is at the limit and print message if equations are unsolvable
    if (safetyIndex == 100) {
        printInput(equationList);
        cout << endl;
    } else {
        printSolvedList(solvedList);
        cout << endl;
    }
}

/**
   Parse the input strings and create the parsed equation lists
   @param equationList the list of equation strings
 */
void Solver::parseEquationStrings(vector<string> equationList) {
    for (int i = 0; i < equationList.size(); i++) {
        string strippedLine = removeWhitespace(equationList.at(i));
        lhs.push_back(getLeft(strippedLine));
        string rightSide = getRight(strippedLine);
        lists = parseString(rightSide);
        rhsVariables.push_back(lists.vars);
        sums.push_back(lists.sum);
    }
}

/**
   Here is where we substitute solved values into the rest of the unsolved equations!
   Match the LHS variable name with the RHS list of variables in each equation and
   perform substitutions if a match is found
 */
void Solver::substituteValues() {
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
    rhsVariables = removeMarkedForDeletion(rhsVariables); // remove the marked entries
}

/**
   Loop through lhs and check the rhsVariables size
   If size is zero, we have a solved equation
   Store in solved equations list
   @return the vector of solved equations
 */
vector<solvedEquation> Solver::checkSolved() {
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

/**
   Prints out the input equations
   @param input the list of equations to solve
 */
void Solver::printInput(vector<string> input) {
    for (int i = 0; i < input.size(); i++) {
        cout << input.at(i) << endl;
    }
}

/**
   Comparison function for the std::sort algorithm used in printSolvedList below
   @param equ1 the first solved equation struct
   @param equ2 the second solved equation struct
   @return bool the results of the comparison
 */
bool sortByName(const solvedEquation &equ1, const solvedEquation &equ2) {
    return (equ1.lhs < equ2.lhs);
}

/**
   Sort and print the list of solved equations
   @param input the list of solved equations
 */
void Solver::printSolvedList(vector<solvedEquation> input) {
    sort(input.begin(), input.end(), sortByName);
    for (int i = 0; i < input.size(); i++) {
        cout << input.at(i).lhs << " = " << input.at(i).rhs << endl;
    }
}

/**
   Parses an equation string to get the left hand side of the equation
   @param input the input equation to be parsed
   @return a string containing the LHS of the equation
 */
string Solver::getLeft(string input) {
    string delimiter = "=";
    return input.substr(0, input.find(delimiter));
}

/**
 Parses an equation string to get the right hand side of the equation
 @param input the input equation to be parsed
 @return a string containing the RHS of the equation
 */
string Solver::getRight(string input) {
    string delimiter = "=";
    return input.substr(input.find(delimiter)+1, input.length());
}

/**
   Parses an input string into a list of variable names and a sum of integers
   @param input the input equation to be parsed
   @return a struct containing the list of variable names and sum
 */
rhsLists Solver::parseString(string input) {
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

/**
   Gets the list of unparsed equations from an input file
   @param filename the name of the input file containing the set of equations
   @return a list containing the set of input equations
 */
vector<string> Solver::getList(string filename) {
    string line;
    vector<string> equationList;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            equationList.push_back(line);
        }
        inputFile.close();
    } else {
        cerr << "Unable to read input file!" << endl << endl;
    }
    return equationList;
}

/**
   Removes the RHS variables from the list that have already been substituted and processed
   @param input list of string vectors containing equation variable names
   @return the list of processed string vectors with the marked entries removed
 */
vector< vector<string> > Solver::removeMarkedForDeletion(vector< vector<string> > input) {
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

/**
   Strips one or more whitespace characters from the input array
   @param input the input string with whitespaces
   @return the string with whitespaces removed
 */
string Solver::removeWhitespace(string input) {
    string::iterator endPos = remove(input.begin(), input.end(), ' ');
    input.erase(endPos, input.end());
    return input;
}
