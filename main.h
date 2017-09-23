//
//  main.h
//  EquationSolver
//
//  Created by William Castellano on 9/23/17.
//  Copyright © 2017 William Castellano. All rights reserved.
//

#ifndef main_h
#define main_h

using namespace std;

struct rhsLists {
    vector<string> vars;
    long sum;
};

struct solvedEquation {
    string lhs;
    long rhs;
};

// Function Declarations
string removeWhitespace(string input);
vector<string> getList(string filename);
string getLeft(string input);
string getRight(string input);
void printVector(vector<string> input);
void printInput(vector<string> input);
bool sortByName(const solvedEquation &equ1, const solvedEquation &equ2);
void printSolvedList(vector<solvedEquation> input);
void printNumVector(vector<long> input);
void printVectorFormatted(vector<string> input);
void printDoubleVector(vector< vector<string> > input);
rhsLists parseString(string input);
vector< vector<string> > removeMarkedForDeletion(vector< vector<string> > input);
vector<solvedEquation> checkSolved(vector<string> lhs, vector< vector<string> > rhsVariables, vector<long> sums);

#endif /* main_h */
