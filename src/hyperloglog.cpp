#include "hyperloglog.h"
#include <cmath>

HyperLogLog::HyperLogLog(uint8_t precision) 
    : p(precision), m(1 << precision), alpha_m(computeAlpha(1 << precision)), M(1 << precision, 0) {}

double HyperLogLog::computeAlpha(uint32_t m) {
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / m);
}

uint64_t HyperLogLog::hash64(const std::string& key) {
    const uint64_t m_const = 0xc6a4a7935bd1e995ULL;
    const int r = 47;
    uint64_t h = 0x9747b28cULL ^ (key.length() * m_const);

    const uint64_t* data = reinterpret_cast<const uint64_t*>(key.data());
    const size_t n_blocks = key.length() / 8;

    for (size_t i = 0; i < n_blocks; ++i) {
        uint64_t k = data[i];
        k *= m_const;
        k ^= k >> r;
        k *= m_const;
        h ^= k;
        h *= m_const;
    }

    const uint8_t* tail = reinterpret_cast<const uint8_t*>(key.data() + n_blocks * 8);
    switch (key.length() & 7) {
        case 7: h ^= static_cast<uint64_t>(tail[6]) << 48; [[fallthrough]];
        case 6: h ^= static_cast<uint64_t>(tail[5]) << 40; [[fallthrough]];
        case 5: h ^= static_cast<uint64_t>(tail[4]) << 32; [[fallthrough]];
        case 4: h ^= static_cast<uint64_t>(tail[3]) << 24; [[fallthrough]];
        case 3: h ^= static_cast<uint64_t>(tail[2]) << 16; [[fallthrough]];
        case 2: h ^= static_cast<uint64_t>(tail[1]) << 8;  [[fallthrough]];
        case 1: h ^= static_cast<uint64_t>(tail[0]);
                h *= m_const;
    }

    h ^= h >> r;
    h *= m_const;
    h ^= h >> r;
    return h;
}

uint8_t HyperLogLog::rho(uint64_t w) {
    if (w == 0) return 64;
    return static_cast<uint8_t>(__builtin_clzll(w) + 1);
}

void HyperLogLog::add(const std::string& key) {
    uint64_t x = hash64(key);
    uint32_t idx = static_cast<uint32_t>(x >> (64 - p));
    uint64_t w = x << p;
    uint8_t r = rho(w);

    if (r > M[idx]) {
        M[idx] = r;
    }
}

double HyperLogLog::estimate() const {
    double z_sum = 0.0;
    uint32_t empty_registers = 0;

    for (uint32_t j = 0; j < m; ++j) {
        z_sum += std::ldexp(1.0, -static_cast<int>(M[j]));
        if (M[j] == 0) {
            empty_registers++;
        }
    }

    double E = alpha_m * m * m / z_sum;

    if (E <= 2.5 * m && empty_registers > 0) {
        return static_cast<double>(m) * std::log(static_cast<double>(m) / empty_registers);
    }
    return E;
}

uint32_t HyperLogLog::getRegisterCount() const {
    return m;
}