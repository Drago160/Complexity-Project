#include "graph_info.h"

GeomGraphData::GeomGraphData(std::istream& input) {
    Input(input);
}

void GeomGraphData::Input(std::istream& input) {
    input >> size_;
    vertices_.reserve(size_);
    for (size_t i = 0; i < size_; ++i) {
        double x, y;
        input >> x >> y;
        vertices_.emplace_back(x, y);
    }
    sortded_nbrs_.resize(size_);
    std::vector<size_t> default_num = GetDefaultNumerating(size_);
    for (size_t i = 0; i < size_; ++i) {
        sortded_nbrs_[i] = default_num;
        std::swap(sortded_nbrs_[i][i], sortded_nbrs_[i][size_ - 1]);
        sortded_nbrs_[i].pop_back();
        auto curr_cord = vertices_[i];
        std::sort(sortded_nbrs_[i].begin(), sortded_nbrs_[i].end(),
                  [this, &curr_cord](const size_t from, const size_t to) {
                      return SquareDistBetween(vertices_[from], curr_cord) <
                             SquareDistBetween(vertices_[to], curr_cord);
                  });
    }
}

std::vector<size_t> GeomGraphData::GetClosests(size_t vertix, size_t closests_num) const {
    if (closests_num >= vertices_.size()) {
        throw std::invalid_argument("numbers of closests must be lower");
    }
    std::vector<size_t> closests(closests_num);
    std::copy(sortded_nbrs_[vertix].begin(), sortded_nbrs_[vertix].begin() + closests_num, closests.begin());
    return closests;
}

double GeomGraphData::DistBetween(size_t from, size_t to) const {
    if (from >= vertices_.size() || to >= vertices_.size()) {
        throw std::invalid_argument("no appropriate vertices");
    }
    return SquareDistBetween(vertices_[from], vertices_[to]);
}

double GeomGraphData::TourCost(const std::vector<size_t> tour) const {
    double cost = 0;
    for (size_t i = 0; i < tour.size()-1; ++i) {
        cost += DistBetween(tour[i], tour[i + 1]);
    }
    return cost;
}