#include "solver.h"

void Solver::Input(std::istream& input) {
    gen_algo_.Input(input);
}

void Solver::Output(std::ostream& output) {
    for (const auto& elem : gen_algo_.GetBestTour()) {
        output << elem << ',';
    }
    output << "\ncost: " << gen_algo_.GetBestTourCost();
}

void Solver::SolveTSP() {
    gen_algo_.CreateStartPopulation();
    for (size_t wr_group_size = wr_size_grid.first;
         wr_group_size < std::min(wr_size_grid.second, default_population_size_); ++wr_group_size) {
        for (size_t generation = generation_grid.first; generation < generation_grid.second;
             ++generation) {
            for (size_t mut_chance = mut_chance_grid.first; mut_chance < mut_chance_grid.second;
                 mut_chance += chance_step) {
                gen_algo_.SetParams(wr_group_size, generation, mut_chance);
                gen_algo_.MakeIters(iteration_num);
            }
        }
    }
}