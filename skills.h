#pragma once

#include "common_types.h"
#include <unordered_set>
#include <vector>
#include <string>

class Skill
{
protected:
	std::string name;
	std::unordered_set<int> usablePositions;
public:
	explicit Skill(const std::string& skillName, const std::vector<int>& usablePosList)
	{
		name = skillName;
		for (int p : usablePosList)
			usablePositions.insert(p);
	}

	std::string getName()
	{
		return name;
	}

	auto getUsablePositions()
	{
		return usablePositions;
	}
};

class AttackSkill : public Skill
{
protected:
	std::unordered_set<int> targetPositions;
	int dmgMod;
	int baseAcc;
	double critMod;
	bool stunEffect;
	int stunBase;
	int moveDir;

public:
	AttackSkill(const std::string& skillName, const std::vector<int> &usablePosList) : Skill(skillName, usablePosList)
	{
	}

	int getDmgMod()
	{
		return dmgMod;
	}

	TryHit useAttackSkill(const int& selfAccMode, const int& selfBaseCrit, const int& targetDodge, const int& targetStunResist)
	{
		RandomGenerator r{};
		TryHit hitResult{};
		int hitChance = (baseAcc + selfAccMode) - targetDodge;
		bool isHit = r.getRandom(0, 100) <= hitChance;
		if (!isHit)
		{
			return hitResult;
		}
		hitResult.isHit = isHit;

		int critChance = selfBaseCrit + critMod;
		bool isCrit = r.getRandom(0, 100) <= critChance;
		hitResult.isCrit = isCrit;

		bool isStun = false;
		if (stunEffect) //todo learn
		{
			int stunChance = stunBase - targetStunResist;
			if (isCrit)
			{
				stunChance += 20;
			}

			isStun = r.getRandom(0, 100) <= stunChance;
		}
		hitResult.isStun = isStun;

		return hitResult;
	}
};

class UtilitySkill : public Skill
{
protected:
	UtilityEffect effect;

public:
	explicit UtilitySkill(const std::string &skillName, const std::vector<int> &usablePosList) : Skill(skillName, usablePosList)
	{
	}

	UtilityEffect getUtilityEffect()
	{
		return effect;
	}
};


class MoveSkill : public Skill
{
protected:

public:
	explicit MoveSkill() : Skill("MoveSkill", {0, 1, 2, 3})
	{
	}

	int getSwapIndexForMove(const int& currentIndex, const int& moveDir)
	{
		int swapIndex = currentIndex + moveDir;
		if (swapIndex < 0)
		{
			return 0; //return 0
		}
		else if (swapIndex > 3)
		{
			return 3;
		}

		return swapIndex;
	}
};

class Smite : public AttackSkill
{
public:
	Smite() : AttackSkill("Smite", {0, 1})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = 0;
		baseAcc = 85;
		critMod = 0;
		stunEffect = false;
		stunBase = 0;
		moveDir = 0;
	}
};

class StunningBlow : public AttackSkill
{
public:
	StunningBlow() : AttackSkill("StunningBlow", {0, 1})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = -50;
		baseAcc = 90;
		critMod = 0;
		stunEffect = true;
		stunBase = 100;
		moveDir = 0;
	}
};

class HolyLance : public AttackSkill
{
public:
	HolyLance() : AttackSkill("HolyLance", {0, 1})
	{
		for (int p : {0, 1, 2}) targetPositions.insert(p);
		dmgMod = 0;
		baseAcc = 85;
		critMod = 6.5;
		stunEffect = false;
		stunBase = 0;
		moveDir = -1; // negative = forward
	}
};

class MaceBash : public AttackSkill
{
public:
	MaceBash() : AttackSkill("MaceBash", {0, 1})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = 0;
		baseAcc = 85;
		critMod = 0;
		stunEffect = false;
		stunBase = 0;
		moveDir = 0;
	}
};

class DazzlingLight : public AttackSkill
{
public:
	DazzlingLight() : AttackSkill("DazzlingLight", {1, 2, 3})
	{
		for (int p : {0, 1, 2}) targetPositions.insert(p);
		dmgMod = -75;
		baseAcc = 90;
		critMod = 5;
		stunEffect = true;
		stunBase = 100;
		moveDir = 0;
	}
};

class GraveyardSlash : public AttackSkill
{
public:
	GraveyardSlash() : AttackSkill("GraveyardSlash", {1, 2, 3})
	{
		for (int p : {1, 2, 3}) targetPositions.insert(p);
		dmgMod = 0;
		baseAcc = 85;
		critMod = 6;
		stunEffect = false;
		stunBase = 0;
		moveDir = 0;
	}
};

class GraveyardStumble : public AttackSkill
{
public:
	GraveyardStumble() : AttackSkill("GraveyardStumble", {3})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = -50;
		baseAcc = 45;
		critMod = 6.5;
		stunEffect = false;
		stunBase = 0;
		moveDir = -1; // negative = forward
	}
};

class AxeBlade : public AttackSkill
{
public:
	AxeBlade() : AttackSkill("AxeBlade", {0, 1})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = 0;
		baseAcc = 72;
		critMod = 6;
		stunEffect = false;
		stunBase = 0;
		moveDir = 0;
	}
};

class DeadWeight : public AttackSkill
{
public:
	DeadWeight() : AttackSkill("DeadWeight", {0, 1, 2})
	{
		for (int p : {0, 1}) targetPositions.insert(p);
		dmgMod = -25;
		baseAcc = 82;
		critMod = 6;
		stunEffect = true;
		stunBase = 100;
		moveDir = 0;
	}
};

class BullwarkOfFaith : public UtilitySkill
{
public:
	BullwarkOfFaith() : UtilitySkill("BullwarkOfFaith", {0, 1})
	{
		effect.target = UtilityTargetEnum::SELF;
		effect.type = UtilityTypeEnum::PROTCHANGE;
		effect.amount = 20;
		effect.numberOfRounds = 3;
	}
};

class DivineGrace : public UtilitySkill
{
public:
	DivineGrace() : UtilitySkill("DivineGrace", {2, 3})
	{
		RandomGenerator r{};
		effect.target = UtilityTargetEnum::ANY;
		effect.type = UtilityTypeEnum::HEAL;
		effect.amount = r.getRandom(4, 5);
		effect.numberOfRounds = 0;
	}
};

class DivineComfort : public UtilitySkill
{
public:
	DivineComfort() : UtilitySkill("DivineComfort", {1, 2, 3})
	{
		RandomGenerator r{};
		effect.target = UtilityTargetEnum::ALL;
		effect.type = UtilityTypeEnum::HEAL;
		effect.amount = r.getRandom(1, 3);
		effect.numberOfRounds = 0;
	}
};

class KnittingBones : public UtilitySkill
{
public:
	KnittingBones() : UtilitySkill("KnittingBones", {2, 3})
	{
		RandomGenerator r{};
		effect.target = UtilityTargetEnum::ANY;
		effect.type = UtilityTypeEnum::HEAL;
		effect.amount = r.getRandom(2, 3);
		effect.numberOfRounds = 0;
	}
};