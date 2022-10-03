#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <random>
#include <vector>

// planning on adding ray casting/picking
class Mers64
{
public:
	Mers64();
	Mers64(unsigned long long seed);
	unsigned long long Next();
	void Skip(unsigned long long n);
	std::vector<unsigned long long> Next(unsigned long long n);
private:
	unsigned long long Seed;
	std::mt19937_64 MT;
};