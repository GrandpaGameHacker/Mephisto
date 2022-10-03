#include "Math/GameMath.h"
#include <chrono>
Mers64::Mers64()
{
    Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count() ^ 69 ^ rand();
    MT = std::mt19937_64(Seed);
}

Mers64::Mers64(unsigned long long seed)
{
    Seed = seed;
    MT = std::mt19937_64(Seed);
}

unsigned long long Mers64::Next()
{
    return MT();
}

void Mers64::Skip(unsigned long long n)
{
    MT.discard(n);
}

std::vector<unsigned long long> Mers64::Next(unsigned long long n)
{
    std::vector<unsigned long long> RandomNumbers;
    for (unsigned long long i = 0; i < n; i++) {
        RandomNumbers.push_back(MT());
    }
    return RandomNumbers;
}
