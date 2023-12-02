#include "genetic_algo.h"

class Solver {
public:
    Solver()
        : gen_algo_(default_population_size_, default_chanse_use_close_, default_num_of_close) {
    }
    void Input(std::istream& input);
    void SolveTSP();
    void Output(std::ostream& output);

private:

    const size_t default_population_size_ = 15;
    const size_t default_chanse_use_close_ = 50;
    const size_t default_num_of_close = 8;
    GeneticManager gen_algo_;

    const std::pair<size_t, size_t> wr_size_grid = {4, 10};
    const std::pair<size_t, size_t> generation_grid = {5, 10};
    const std::pair<size_t, size_t> mut_chance_grid = {25, 75};
    const size_t chance_step = 5;
    const size_t iteration_num = 100;
};