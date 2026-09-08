#include <iostream>
#include <unordered_set>
#include <string>
#include <cmath>
#include "hyperloglog.h"

int main() {
    uint32_t total = 0;
    uint32_t distinct = 0;

    std::cout << "Ingrese el total de elementos a procesar en el flujo: ";
    std::cin >> total;

    std::cout << "Ingrese la cardinalidad deseada (elementos unicos): ";
    std::cin >> distinct;

    if (distinct == 0 || total == 0) {
        std::cerr << "Los valores deben ser mayores a cero.\n";
        return 1;
    }

    if (distinct > total) {
        std::cerr << "La cardinalidad no puede ser mayor que el total de elementos.\n";
        return 1;
    }

    HyperLogLog hll(14);
    std::unordered_set<std::string> exact_set;

    // Generar el flujo con repeticiones usando el operador modulo
    for (uint32_t i = 1; i <= total; ++i) {
        std::string item = "data_" + std::to_string(i % distinct);
        hll.add(item);
        exact_set.insert(item);
    }

    double estimated = hll.estimate();
    double actual = static_cast<double>(exact_set.size());
    double rel_error = (std::abs(estimated - actual) / actual) * 100.0;
    double theo_bound = (1.04 / std::sqrt(hll.getRegisterCount())) * 100.0;

    std::cout << "\n RESULTADOS \n";
    std::cout << "Total de elementos procesados: " << total << "\n";
    std::cout << "Cardinalidad real             : " << static_cast<uint64_t>(actual) << "\n";
    std::cout << "Estimado HyperLogLog          : " << static_cast<uint64_t>(estimated) << "\n";
    std::cout << "Error relativo obtenido       : " << rel_error << " %\n";
    std::cout << "Cota teorica esperada         : " << theo_bound << " %\n";
    std::cout << "\n";

    return 0;
}