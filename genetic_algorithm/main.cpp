#include <iostream>
#include <fstream>
#include "solver.h"
#include "graph_generator.h"

int main(){
    /*
    GraphGenerator generator;
    for (size_t i = 20; i < 50; i+=5) {
        std::string name = "example_s";
        name += std::to_string(i);
        name += ".txt";
        generator.CreateGraphOnFile(i, "../examples/" + name);
    }
    */

    std::fstream fs;
    fs.open("../examples/example_s30.txt");

    Solver solver;
    solver.Input(fs);
    solver.SolveTSP();

    std::ofstream of("../solves/example_s30_solve.txt");
    solver.Output(of);
    of.close();


    fs.close();
}
