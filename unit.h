#pragma once

#include "common_types.h"
#include "skills.h"
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Unit
{
protected:
	std::string name;
	int position;
	bool alive;
	bool stunned;
	int maxHp;
	int hp;
	int dodge;
	double prot;
	int originalSpeed;
	int speed;
	int accMod;
	int baseCrit;
	int minDmg;
	int maxDmg;
	int stunResist;

public:
	Unit(const std::string& charName, const int& pos)
	{
		name = charName;
		position = pos;
		alive = true;
		stunned = false;
	}

	std::string getName()
	{
		return name;
	}

	int getPosition()
	{
		return position;
	}

	bool isAlive()
	{
		return alive;
	}

	double getProt()
	{
		return prot;
	}

	int getDodge()
	{
		return dodge;
	}

	int getSpeed()
	{
		return speed;
	}

	int getStunResist()
	{
		return stunResist;
	}

	int getHp()
	{
		return hp;
	}

	void increaseSpeedBy(int increment)
	{
		speed += increment;
	}

	void updatePosition(int newPosition)
	{
		position = newPosition;
	}

	/*
	 * NOTE: when actualDamage is less than 1, we don't want to deal 0 damage
	 * so in that case we ceil the actualDamage to 1.
	 */
	void attack(std::shared_ptr<Unit> targetUnit, AttackSkill skill)
	{
		TryHit attackStats = skill.useAttackSkill(accMod, baseCrit, targetUnit->getDodge(), targetUnit->getStunResist());
		std::cout << name << " attacked " << targetUnit->getName() << " with " << skill.getName() << std::endl;
		if (!attackStats.isHit)
		{
			std::cout << name << " missed!" << std::endl;
			return;
		}
		else if (attackStats.isHit)
		{
			double dmg;
			if (attackStats.isCrit)
			{
				std::cout << "Critical damage!" << std::endl;
				dmg = maxDmg * 1.5;
			}
			else
			{
				RandomGenerator r{};
				dmg = r.getRandom(minDmg, maxDmg);
			}
			double rawDmg = dmg * (100.0 + skill.getDmgMod()) / 100.0;
			double actualDmg = rawDmg - (rawDmg * (targetUnit->getProt() / 100.0));
//			std::cout << "dmg: " << dmg << ", rawDmg: " << rawDmg << ", actualDmg: " << actualDmg << std::endl;
			if (actualDmg < 1.0)
			{
				targetUnit->sustainDamage((int)ceil(actualDmg), attackStats.isStun);
			}
			else
			{
				targetUnit->sustainDamage((int)floor(actualDmg), attackStats.isStun);
			}
		}
	}

	virtual bool canAttack(const std::shared_ptr<Unit>& targetUnit) = 0;
	virtual void processTurn() {}
	virtual void getHealed(int healingAmount) {}
	virtual void sustainDamage(int damage, bool isStunned) {}

};
