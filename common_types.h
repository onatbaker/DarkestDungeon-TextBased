#pragma once
#include <chrono>
#include <random>

class RandomGenerator
{
public:
	RandomGenerator()
	{
	}
	~RandomGenerator() = default;

	int getRandom(int min, int max)
	{
		if (min > max)
		{
			std::swap(min, max);
		}

		std::mt19937 rng(static_cast<unsigned int>(
			std::chrono::high_resolution_clock::now().time_since_epoch().count()));

		std::uniform_int_distribution<int> dist(min, max);

		return dist(rng);
	}
};

struct TryHit
{
	bool isHit = false;
	bool isCrit = false;
	bool isStun = false;
};

enum class SkillType {ATTACK, UTILITY, MOVE};

enum class UtilityTypeEnum {HEAL, PROTCHANGE};
enum class UtilityTargetEnum {SELF, ALL, ANY};

struct UtilityEffect
{
	UtilityTypeEnum type;
	UtilityTargetEnum target;
	int amount;
	int numberOfRounds;
};
