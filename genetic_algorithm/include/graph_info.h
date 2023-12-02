#pragma once
#include <vector>
#include "utils.h"
#include <iostream>

class GeomGraphData {
public:
    GeomGraphData() = default;
    GeomGraphData(std::istream& input);
    void Input(std::istream& input);
    std::vector<size_t> GetClosests(size_t vertix, size_t closest_num) const;
    double DistBetween(size_t from, size_t to) const;
    size_t GetClosestNbr(size_t vertix) const { return sortded_nbrs_[vertix][0]; }
    double TourCost(const std::vector<size_t> tour) const;
    size_t Size() const { return size_; }
private:
    size_t size_;
    std::vector<Cord> vertices_;
    std::vector<std::vector<size_t>> sortded_nbrs_;
};