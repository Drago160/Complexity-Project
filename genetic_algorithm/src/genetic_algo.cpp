#include "genetic_algo.h"
#include <unordered_set>
#include <unordered_map>

void GeneticManager::Input(std::istream& input) {
    graph_info_.Input(input);
}

size_t GetFirstNotUsed(const std::vector<bool>& used, const std::vector<size_t>& get_from) {
    for (size_t elem : get_from) {
        if (!used[elem]) {
            return elem;
        }
    }
    return used.size();
}

std::vector<size_t> GeneticManager::ShiftTourToStandart(const std::vector<size_t>& tour) {
    std::vector<size_t> ret;
    ret.reserve(tour.size());
    size_t i = 0;
    for (; tour[i] != 0; ++i) {
    }

    for (; i < tour.size(); ++i) {
        ret.push_back(tour[i]);
    }
    for (i = 0; tour[i] != 0; ++i) {
        ret.push_back(tour[i]);
    }
    ret.push_back(0);
    return ret;
}

void GeneticManager::CreateNewTour() {
    static std::vector<bool> used(graph_info_.Size());
    std::fill(used.begin(), used.end(), false);

    std::vector<size_t> tour;
    // tour.reserve(graph_info_.Size() + 1);

    size_t start_from = RandomInt(0, graph_info_.Size() - 1, generator_);
    tour.push_back(start_from);
    used[start_from] = true;

    while (tour.size() < graph_info_.Size()) {
        if (RandomInt(0, 100, generator_) < static_cast<int>(chance_use_close_city_)) {
            auto closest_not_used = GetFirstNotUsed(
                used, graph_info_.GetClosests(tour[tour.size() - 1], number_of_close_cities_));
            if (closest_not_used >= graph_info_.Size()) {
                size_t not_used = GetRandomNotUsed(used, tour.size(), generator_);
                used[not_used] = true;
                tour.push_back(not_used);
                continue;
            }
            used[closest_not_used] = true;
            tour.push_back(closest_not_used);
        } else {
            size_t not_used = GetRandomNotUsed(used, tour.size(), generator_);
            used[not_used] = true;
            tour.push_back(not_used);
        }
    }
    population_.tours.push_back(ShiftTourToStandart(tour));
}

void GeneticManager::CreateStartPopulation() {
    population_.tours.reserve(population_size_);

    for (size_t i = 0; i < population_size_; ++i) {
        CreateNewTour();
    }

    population_.CountDists(graph_info_);
}

void GeneticManager::Population::CountDists(const GeomGraphData& graph_info) {
    distances.reserve(tours.size());
    for (const auto& tour : tours) {
        distances.push_back(graph_info.TourCost(tour));
    }
}

void UpdatePrecountInfo(std::vector<size_t>& lhs_table, std::vector<size_t>& rhs_table,
                        std::unordered_map<size_t, size_t>& common_connections,
                        const std::vector<size_t>& lhs, const std::vector<size_t>& rhs) {
    common_connections.clear();
    for (size_t i = 0; i < lhs.size() - 2; ++i) {
        lhs_table[lhs[i]] = lhs[i + 1];
        common_connections[lhs[i]] = lhs[i + 1];
    }
    lhs_table[lhs[lhs.size() - 2]] = lhs.size();

    for (size_t i = 0; i < rhs.size() - 2; ++i) {
        rhs_table[rhs[i]] = rhs[i + 1];
        if (!common_connections.contains(rhs[i])) {
            continue;
        }
        if (common_connections[rhs[i]] != rhs[i + 1]) {
            common_connections.erase(rhs[i]);
        }
    }
    rhs_table[rhs[rhs.size() - 2]] = rhs.size();
}

std::vector<size_t> CrossTours(const std::vector<size_t>& lhs, const std::vector<size_t>& rhs) {
    static std::vector<size_t> lhs_table(lhs.size(), lhs.size());
    static std::vector<size_t> rhs_table(rhs.size(), lhs.size());
    static std::unordered_map<size_t, size_t> common_connections;

    UpdatePrecountInfo(lhs_table, rhs_table, common_connections, lhs, rhs);

    std::vector<size_t> result_tour;
    static std::vector<bool> used(lhs.size() - 1, false);
    std::fill(used.begin(), used.end(), false);
    for (const auto& [from, to] : common_connections) {
        used[to] = true;
    }

    result_tour.reserve(lhs.size());
    result_tour.push_back(0);
    size_t curr = 0;

    used[0] = true;

    while (result_tour.size() != rhs.size() - 1) {
        if (common_connections.contains(curr)) {
            curr = common_connections[curr];
            result_tour.push_back(curr);
            used[curr] = true;
            continue;
        }
        if (curr % 2 == 0) {
            if (lhs_table[curr] >= lhs.size()) {
                curr = GetFirstNotUsed(used, lhs);
            } else if (!used[lhs_table[curr]]) {
                curr = lhs_table[curr];
            } else if (rhs_table[curr] < rhs.size() && !used[rhs_table[curr]]) {
                curr = rhs_table[curr];
            } else {
                curr = GetFirstNotUsed(used, lhs);
            }
        } else {
            if (rhs_table[curr] >= rhs.size()) {
                curr = GetFirstNotUsed(used, lhs);
            } else if (!used[rhs_table[curr]]) {
                curr = rhs_table[curr];
            } else if (lhs_table[curr] < lhs.size() && !used[lhs_table[curr]]) {
                curr = lhs_table[curr];
            } else {
                curr = GetFirstNotUsed(used, lhs);
            }
        }
        used[curr] = true;
        result_tour.push_back(curr);
    }
    result_tour.push_back(0);
    return result_tour;
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
    for (size_t i = 1; i < tour.size() - 1; ++i) {
        if (copy[i] != i - 1) {
            return false;
        }
    }
    return true;
}

void GeneticManager::MutateWithCond(std::vector<size_t>& tour, bool cond) {
    if (cond) {
        Mutate(tour);
    }
}

void GeneticManager::MutateWithChance(std::vector<size_t>& tour, size_t chance) {
    MutateWithCond(tour, RandomInt(0, 100, generator_) < chance);
}

void GeneticManager::EvalutionIter() {
    // create and sort group (only indexes)
    std::vector<size_t> wr_group = 
        RandomDiffNumbers(wr_group_size_, population_.tours.size() - 1, generator_);
    const auto& dists = population_.distances;
    BubbleSort(wr_group.begin(), wr_group.end(),
               [&dists](size_t lhs, size_t rhs) { return dists[lhs] < dists[rhs]; });

    for (size_t i = 0; i < generation_; ++i) {
        auto child = CrossTours(population_.tours[wr_group[0]], population_.tours[wr_group[1]]);
        if (!CheckIsTourCorrect(child)) {  ////////////// DEBUG ARTEFACT
            CrossTours(population_.tours[wr_group[0]], population_.tours[wr_group[1]]);
        }
        // optimization: may not save parents
        MutateWithCond(child, child == population_.tours[wr_group[0]] ||
                                  child == population_.tours[wr_group[1]]);
        MutateWithChance(child, chance_mutate_);

        population_.tours[wr_group[wr_group.size() - 1]] = child;
        auto cost = graph_info_.TourCost(child);
        population_.distances[wr_group[wr_group.size() - 1]] = cost;

        if (cost < best_cost) {
            best_cost = cost;
            best_tour = child;
        }

        BubbleSort(wr_group.begin(), wr_group.end(),
                   [&dists](size_t lhs, size_t rhs) { return dists[lhs] < dists[rhs]; });
    }
}

void GeneticManager::Mutate(std::vector<size_t>& tour) {
    size_t swap_from = RandomInt(1, tour.size() - 2, generator_);
    size_t swap_to = RandomInt(1, tour.size() - 2, generator_);
    std::swap(tour[swap_from], tour[swap_to]);
}

void GeneticManager::SolveTSP() {
    MakeIters(max_iters_);
}

void GeneticManager::MakeIters(size_t iters_num) {
    for (size_t i = 0; i < iters_num; ++i) {
        EvalutionIter();
    }
}

void GeneticManager::SetParams(size_t group_size, size_t generation_cnt, size_t chance_mutate) {
    if (group_size > population_size_) {
        wr_group_size_ = population_size_ - 2;
    } else {
        wr_group_size_ = group_size;
    }
    generation_ = generation_cnt;
    chance_mutate_ = chance_mutate;
}

void GeneticManager::PrintAns(std::ostream& output) {
    for (const auto& elem : best_tour) {
        output << elem << ',';
    }
    output << "\ncost: " << best_cost;
}

std::vector<size_t> GreedyAlgorithm(const GeomGraphData& graph_data) {
    std::vector<size_t> result;
    static std::vector<bool> used(graph_data.Size());
    std::fill(used.begin(), used.end(), false);
    result.reserve(graph_data.Size() + 1);
    result.push_back(0);
    used[0] = true;
    while (result.size() != graph_data.Size()) {
        auto closests = graph_data.GetClosests(result[result.size() - 1], graph_data.Size() - 1);
        auto next = GetFirstNotUsed(used, closests);
        result.push_back(next);
        used[next] = true;
    }
    result.push_back(0);
    return result;
}

std::vector<size_t> GeneticManager::MakeGreedyTour() {
    return GreedyAlgorithm(graph_info_);
}