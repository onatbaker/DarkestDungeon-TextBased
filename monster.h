#pragma once

#include "unit.h"

class Monster : public Unit
{
protected:
	bool doesHaveAnyUtility;

	bool canAttack(const std::shared_ptr<Unit>& targetUnit) override
	{
		if (!alive)
		{
			std::cout << "Monster " << name << " is not alive! Therefore, cannot attack." << std::endl;
			return false;
		}

		if (stunned)
		{
			std::cout << "Monster " << name << " is stunned, he cannot attack." << std::endl;
			stunned = false;
			return false;
		}

		if (!targetUnit->isAlive())
		{
			std::cout << "Hero " << targetUnit->getName() << " is not alive and can't be attacked." << std::endl;
			return false;
		}

		return true;
	}

public:
	Monster(const std::string &charName, const int &pos) : Unit(charName, pos)
	{
	}

	void getHealed(int healingAmount) override
	{
		if (healingAmount <= 0)
		{
			std::cout << "Illegal healing amount " << healingAmount << "!" << std::endl;
			return;
		}

		if (!alive)
		{
			std::cout << "Monster " << name << " is already dead. Cannot get healed." << std::endl;
			return;
		}

		hp = std::min(maxHp, hp + healingAmount);
		std::cout << "Monster " << name << " got healed by " << healingAmount << ". Current hp: " << hp << std::endl;
	}

	void sustainDamage(int damage, bool isStunned) override
	{
		if (!alive)
		{
			return;
		}

		hp = std::max(0, hp - damage);
		std::cout << "Monster " << name << " sustained an attack of " << damage << " and his current hp is now at " << hp << std::endl;
		if (hp == 0)
		{
			alive = false;
			std::cout <<  "Monster " << name << " has died." << std::endl;
		}
		else if (isStunned)
		{
			stunned = true;
			std::cout << "Monster " << name << " is stunned!" << std::endl;
		}
	}

	bool doesHaveUtilityFunction()
	{
		return doesHaveAnyUtility;
	}

	void processTurn() override
	{
		std::cout << "Modifying " << name << "'s speed (" << speed << ") back to its original  (" << originalSpeed << ")" << std::endl;
		speed = originalSpeed;
	}

	virtual bool attackWithSkillsAndReturnIfMoveNeeded(std::shared_ptr<Unit> targetUnit) = 0;
	virtual void decideOnUtility(std::vector<std::shared_ptr<Monster>> allMonsters) {}
};

class BoneSoldier : public Monster
{
public:
	BoneSoldier(const std::string &charName, const int &pos) : Monster(charName, pos)
	{
		maxHp = 10;
		hp = 10;
		dodge = 10;
		prot = 15.0;
		speed = 2;
		originalSpeed = 2;
		accMod = 0;
		baseCrit = 0;
		minDmg = 3;
		maxDmg = 8;
		stunResist = 25;

		doesHaveAnyUtility = false;
	}

	bool attackWithSkillsAndReturnIfMoveNeeded(std::shared_ptr<Unit> targetUnit) override
	{
		if (!canAttack(targetUnit))
		{
			std::cout << "It is not viable for " << name << " to act this turn, passing.." << std::endl;
			return false;
		}

		if (targetUnit->getPosition() >= 2)
		{
			attack(targetUnit, GraveyardStumble());
			return true;
		}
		else
		{
			attack(targetUnit, GraveyardSlash());
		}

		return false;
	}
};

class BoneDefender : public Monster
{
public:
	BoneDefender(const std::string &charName, const int &pos) : Monster(charName, pos)
	{
		maxHp = 22;
		hp = 22;
		dodge = 8;
		prot = 45.0;
		speed = 1;
		originalSpeed  = 1;
		accMod = 0;
		baseCrit = 0;
		minDmg = 2;
		maxDmg = 4;
		stunResist = 45;

		doesHaveAnyUtility = true;
	}

	bool attackWithSkillsAndReturnIfMoveNeeded(std::shared_ptr<Unit> targetUnit) override
	{
		if (!canAttack(targetUnit))
		{
			std::cout << "It is not viable for " << name << " to act this turn, passing.." << std::endl;
			return false;
		}
		if (targetUnit->getPosition() >= 2)
		{
			attack(targetUnit, AxeBlade());
		}
		else
		{
			attack(targetUnit, DeadWeight());
		}

		return false;
	}

	virtual void decideOnUtility(std::vector<std::shared_ptr<Monster>> allMonsters) override
	{
		KnittingBones knittingBones{};
		UtilityEffect effect = knittingBones.getUtilityEffect();
		RandomGenerator r{};
		int randomMonsterIndex = r.getRandom(0, 3);
		allMonsters[randomMonsterIndex]->getHealed(effect.amount);
	}

	bool operator< (const std::shared_ptr<Monster> &other) const {
		return speed < other->getSpeed();
	}

	bool operator> (const std::shared_ptr<Monster> &other) const {
		return speed > other->getSpeed();
	}
};
