#pragma once

#include "graph_info.h"
#include "utils.h"
#include "random.h"

const size_t kRandomSeed = 42;

std::vector<size_t> GreedyAlgorithm(const GeomGraphData& graph_info);

class GeneticManager {
public:
    GeneticManager(size_t population_size, size_t chanse_use_close, size_t num_of_close)
        : population_size_(population_size),
          chance_use_close_city_(chanse_use_close),
          number_of_close_cities_(num_of_close),
          generator_(kRandomSeed) {
    }

    void SetParams(size_t group_size, size_t generation_cnt, size_t chance_mutate);
    std::vector<size_t> MakeGreedyTour();
    const GeomGraphData& GetGraphInfo() const {
        return graph_info_;
    }

    void CreateStartPopulation();
    void SolveTSP();
    void MakeIters(size_t iters_num);
    void Input(std::istream& input);
    std::vector<size_t> GetBestTour() const {
        return best_tour;
    }
    double GetBestTourCost() const {
        return best_cost;
    }
    void PrintAns(std::ostream& ouput);

private:
    struct Population {
        std::vector<std::vector<size_t>> tours;
        std::vector<double> distances;
        void CountDists(const GeomGraphData& graph_info);
    };
    void EvalutionIter();
    void MutateWithChance(std::vector<size_t>& tour, size_t chance);
    void MutateWithCond(std::vector<size_t>& tour, bool condition);
    void Mutate(std::vector<size_t>& tour);
    void CreateNewTour();
    std::vector<size_t> ShiftTourToStandart(const std::vector<size_t>&);

    GeomGraphData graph_info_;
    Population population_;

    const size_t population_size_;
    const size_t chance_use_close_city_;
    const size_t number_of_close_cities_;

    size_t wr_group_size_ = 4;  // needs lower then ~ 20 to
    size_t generation_ = 5;     // not very big
    size_t chance_mutate_ = 50;

    const size_t max_iters_ = 1000;

    std::vector<size_t> best_tour;
    double best_cost = 1e9;

    std::mt19937 generator_;
};