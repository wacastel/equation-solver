//
//  main.cpp
//  EquationSolver
//
//  Created by William Castellano on 9/23/17.
//  Copyright © 2017 William Castellano. All rights reserved.
//

#include <iostream>
#include "solver.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // Read in the filename from the command line
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " FILENAME" << endl;
        return 1;
    } else {
        string filename = argv[1];
        cout << endl << "Input Filename: " << filename << endl << endl;
        Solver solver(filename);
        solver.solveIt();
    }
}


