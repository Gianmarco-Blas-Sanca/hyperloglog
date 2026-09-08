#include <iostream>
#include <unordered_set>
#include <string>
#include <cmath>
#include "hyperloglog.h"

int main() {
    HyperLogLog hll(14);
    std::unordered_set<std::string> exact_set;

    const uint32_t total = 1000000;
    const uint32_t distinct = 600000;

    for (uint32_t i = 1; i <= total; ++i) {
        std::string item = "data_" + std::to_string(i % distinct);
        hll.add(item);
        exact_set.insert(item);
    }

    double estimated = hll.estimate();
    double actual = static_cast<double>(exact_set.size());
    double rel_error = (std::abs(estimated - actual) / actual) * 100.0;
    double theo_bound = (1.04 / std::sqrt(hll.getRegisterCount())) * 100.0;

    std::cout << "Cardinalidad real      : " << static_cast<uint64_t>(actual) << "\n";
    std::cout << "Estimado HyperLogLog   : " << static_cast<uint64_t>(estimated) << "\n";
    std::cout << "Error relativo obtenido: " << rel_error << " %\n";
    std::cout << "Cota teorica esperada  : " << theo_bound << " %\n";

    return 0;
}