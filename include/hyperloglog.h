#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <vector>
#include <string>
#include <cstdint>

class HyperLogLog {
private:
    uint8_t p;
    uint32_t m;
    double alpha_m;
    std::vector<uint8_t> M;

    static uint64_t hash64(const std::string& key);
    static uint8_t rho(uint64_t w);
    static double computeAlpha(uint32_t m);

public:
    explicit HyperLogLog(uint8_t precision = 14);
    void add(const std::string& key);
    double estimate() const;
    uint32_t getRegisterCount() const;
};

#endif