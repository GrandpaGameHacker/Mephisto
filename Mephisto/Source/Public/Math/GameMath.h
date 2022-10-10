#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <random>
#include <vector>
#include <Types.h>

// planning on adding ray casting/picking

class Mersienne
{
public:
	Mersienne();
	Mersienne(uint64 seed);
	uint64 Next();
	uint64 Next(uint64 max, uint64 min);
	float NextFloat(float max, float min);
	double NextDouble(double max, double min);
	void Skip(uint64 n);
	std::vector<uint64> Next(uint64 n);
	std::vector<float> NextFloat(uint64 n, float max, float min);
	std::vector<double> NextDouble(uint64 n, float max, float min);
private:
	uint64 Seed;
	std::mt19937_64 MT;
};