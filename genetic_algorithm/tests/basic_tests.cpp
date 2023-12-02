#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "utils.h"
#include <unordered_map>

#define private public
#define protected public

#include "genetic_algo.h"

const std::string pref_way = "../../examples/";
std::string GetFullPath(const std::string& filename) {
    return pref_way + filename;
}

TEST(Basic, CheckCorrectRead) {
    std::fstream fs(GetFullPath("simple.txt"));

    GeneticManager genetic(8, 50, 3);
    genetic.Input(fs);

    // check sizes
    ASSERT_EQ(genetic.graph_info_.size_, 5);
    ASSERT_EQ(genetic.graph_info_.sortded_nbrs_.size(), 5);

    // check correct sorting
    size_t j = 0;
    const auto& vertices = genetic.graph_info_.vertices_;
    for (const auto& nbrs : genetic.graph_info_.sortded_nbrs_) {
        for (size_t i = 0; i < nbrs.size() - 1; ++i) {
            bool is_lower = (SquareDistBetween(vertices[nbrs[i]], vertices[j]) <=
                             SquareDistBetween(vertices[nbrs[i + 1]], vertices[j]));
            ASSERT_TRUE(is_lower);
        }
        ++j;
    }

    fs.close();
}

bool CheckIsTourCorrect(const std::vector<size_t> tour) {
    if (tour[0] != tour[tour.size() - 1] || tour[0] != 0) {
        return false;
    }
    // check if not equals
    auto copy = tour;
    std::sort(copy.begin(), copy.end());
    if (copy[0] != copy[1] || copy[0] != 0) {
        return false;
    }
    // check if contains all
    for (size_t i = 1; i < tour.size()-1; ++i) {
        if (copy[i] != i-1) {
            return false;
        }
    }
    return true;
}

TEST(Basic, CheckCorrectStartPopulation) {
    std::fstream fs(GetFullPath("simple.txt"));

    GeneticManager genetic(8, 50, 3);
    genetic.Input(fs);
    genetic.CreateStartPopulation();

    const auto& population = genetic.population_.tours;

    // check size
    ASSERT_EQ(population.size(), 8);

    // check is ways correct
    for (const auto& tour : population) {
        ASSERT_TRUE(CheckIsTourCorrect(tour));
    }

    fs.close();
}
