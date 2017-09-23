//
//  solver.h
//  EquationSolver
//
//  Created by William Castellano on 9/23/17.
//  Copyright © 2017 William Castellano. All rights reserved.
//

#ifndef solver_h
#define solver_h

#include <vector>
using namespace std;

// Struct Declarations
struct rhsLists {
    vector<string> vars;
    long sum;
};

struct solvedEquation {
    string lhs;
    long rhs;
};

// Class Declaration
class Solver {
private:
    vector<string> lhs;
    vector< vector<string> > rhsVariables;
    vector<long> sums;
    rhsLists lists;
    vector<solvedEquation> solvedList;
    string filename;
    
    string removeWhitespace(string input);
    vector<string> getList(string filename);
    string getLeft(string input);
    string getRight(string input);
    void printVector(vector<string> input);
    void printInput(vector<string> input);
    void printSolvedList(vector<solvedEquation> input);
    void printNumVector(vector<long> input);
    void printVectorFormatted(vector<string> input);
    void printDoubleVector(vector< vector<string> > input);
    rhsLists parseString(string input);
    vector< vector<string> > removeMarkedForDeletion(vector< vector<string> > input);
    vector<solvedEquation> checkSolved();
    void substituteValues();
    void parseEquationStrings(vector<string>);
public:
    Solver(string filename);
    void solveIt();
};

// Function Declarations
bool sortByName(const solvedEquation &equ1, const solvedEquation &equ2);

#endif /* solver_h */
