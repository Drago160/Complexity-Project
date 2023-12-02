#include <iostream>
#include "random.h"
#include <string>

const size_t kRandomSeedForGenerator = 42;

class GraphGenerator {
public:
    GraphGenerator() : generator_(kRandomSeedForGenerator) {}
    void CreateGraphOnFile(size_t size, const std::string& path);
private:
    void Generate(size_t size);
    void WriteGraphToFile(const std::string& filename);

    std::vector<std::pair<int, int>> integer_graph_;
    std::mt19937 generator_;
};
