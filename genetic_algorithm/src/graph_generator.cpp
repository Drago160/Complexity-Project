#include "graph_generator.h"
#include <fstream>

void GraphGenerator::Generate(size_t size) {
    integer_graph_.reserve(size);
    integer_graph_.clear();
    for (size_t i = 0; i < size; ++i) {
        int x,y;
        x = RandomInt(-100, 100, generator_);
        y = RandomInt(-100, 100, generator_);
        integer_graph_.emplace_back(x, y);
    }
}

void GraphGenerator::WriteGraphToFile(const std::string& filename) {
    std::ofstream output(filename);
    output << integer_graph_.size() << '\n';
    for (const auto& [x, y] : integer_graph_) {
        output << x << ' ' << y << '\n';
    }
    output.close();
}

void GraphGenerator::CreateGraphOnFile(size_t size, const std::string& path) {
    Generate(size);
    WriteGraphToFile(path);
}