#pragma once

#include "unit.h"

class Hero : public Unit
{
protected:
	int deathBlowResist;
	bool deathsDoor;

	int bullwarkUtilityCountDown;
	int extraProt;

	bool canAttack(const std::shared_ptr<Unit>& targetUnit) override
	{
		if (!alive)
		{
			std::cout << "Hero " << name << " is not alive! Therefore, cannot attack." << std::endl;
			return false;
		}

		if (deathsDoor)
		{
			std::cout << "Hero " << name << " is in Death's Door state, he cannot attack." << std::endl;
			return false;
		}

		if (stunned)
		{
			std::cout << "Hero " << name << " is stunned, he cannot attack." << std::endl;
			stunned = false;
			return false;
		}

		if (!targetUnit->isAlive())
		{
			std::cout << "Monster " << targetUnit->getName() << " is not alive and can't be attacked." << std::endl;
			return false;
		}

		return true;
	}
public:
	Hero(const std::string& charName, const int& pos, const int& dbr) : Unit(charName, pos)
	{
		deathBlowResist = dbr;
		deathsDoor = false;
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
			std::cout << "Hero " << name << " is already dead. Cannot get healed." << std::endl;
			return;
		}

		hp = std::min(maxHp, hp + healingAmount);
		std::cout << "Hero " << name << " got healed by " << healingAmount << ". Current hp: " << hp << std::endl;
		if (hp > 0 && deathsDoor)
		{
			deathsDoor = false;
			std::cout << name << " is not in Death's Door state anymore!" << std::endl;
		}
	}

	void sustainDamage(int damage, bool isStunned) override
	{
		if (!alive)
		{
			return;
		}

		if (deathsDoor && damage >= 1)
		{
			RandomGenerator r{};
			int deathRandom = r.getRandom(0, 100);
			alive = deathRandom <= deathBlowResist;
			if (alive)
			{
				std::cout << "Hero " << name << " who is on Death's Door state survived the attack (deathBlowResist). What do we say to the god of death? Not today!"<< std::endl;
			}
			else
			{
				std::cout <<  "Hero " << name << " was on Death's Door state and has died due to the successful attack." << std::endl;
			}
		}
		if (hp > 0)
		{
			hp = std::max(0, hp - damage);
			std::cout << "Hero " << name << " sustained an attack of " << damage << " and his current hp is now at " << hp << std::endl;
			deathsDoor = hp == 0;
			if (deathsDoor)
			{
				std::cout << "Hero " << name << "'s hp is " << hp <<" now. He got into Death's Door state." << std::endl;
			}
			if (isStunned)
			{
				stunned = true;
				std::cout << "Hero " << name << " is stunned!" << std::endl;
			}
		}
	}

	void processTurn() override
	{
		if (bullwarkUtilityCountDown > 0)
		{
			bullwarkUtilityCountDown--;
			std::cout << "Winding down the bullwark effect. Remaining rounds: " << bullwarkUtilityCountDown << std::endl;
			if (bullwarkUtilityCountDown == 0)
			{
				prot -= extraProt;
				std::cout << "Prot decreased back to " + std::to_string(prot) << std::endl;
			}
		}

		std::cout << "Modifying " << name << "'s speed (" << speed << ") back to its original  (" << originalSpeed << ")" << std::endl;
		speed = originalSpeed;
	}

	virtual bool attackWithSkillsAndReturnIfMoveNeeded(std::shared_ptr<Unit> targetUnit) = 0;
	virtual void decideOnUtility(std::vector<std::shared_ptr<Hero>> allHeroes) {}
};

class Vestal : public Hero
{
public:
	Vestal(const std::string &charName, const int &pos) : Hero(charName, pos, 77)
	{
		maxHp = 24;
		hp = 24;
		dodge = 0;
		prot = 0.0;
		speed = 4;
		originalSpeed = 4;
		accMod = 0;
		baseCrit = 1;
		minDmg = 4;
		maxDmg = 8;
		stunResist = 30;

		extraProt = 0;
		bullwarkUtilityCountDown = 0;
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
			attack(targetUnit, DazzlingLight());
		}
		else
		{
			attack(targetUnit, MaceBash());
		}

		return false;
	}

	void decideOnUtility(std::vector<std::shared_ptr<Hero>> allHeroes) override
	{
		BullwarkOfFaith bullwarkOfFaith{};
		UtilityEffect effect = bullwarkOfFaith.getUtilityEffect();

		if (bullwarkUtilityCountDown > 0)
		{
			std::cout << bullwarkOfFaith.getName() << " already being used by Hero " << name <<
			", cannot re-use." << std::endl;
			return;
		}

		if (effect.type == UtilityTypeEnum::PROTCHANGE)
		{
			std::cout << "Hero " << name << " used " << bullwarkOfFaith.getName() << " which gave him "
					  << effect.amount << " prot change for " << effect.numberOfRounds << " rounds." << std::endl;
			bullwarkUtilityCountDown = effect.numberOfRounds;
			prot += effect.amount;
			extraProt = effect.amount;
		}
	}
};

class Crusader : public Hero
{
public:
	Crusader(const std::string &charName, const int &pos) : Hero(charName, pos, 67)
	{
		maxHp = 35;
		hp = 35;
		dodge = 5;
		prot = 0.0;
		speed = 1;
		originalSpeed = 1;
		accMod = 0;
		baseCrit = 3;
		minDmg = 6;
		maxDmg = 12;
		stunResist = 40;
		extraProt = 0;
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
			attack(targetUnit, HolyLance());
			return true;
		}
		else
		{
			RandomGenerator r{};
			int randNumber = r.getRandom(0, 100);
			if (randNumber < 60)
			{
				attack(targetUnit, StunningBlow());
			}
			else
			{
				attack(targetUnit, Smite());
			}
		}

		return false;
	}

	void decideOnUtility(std::vector<std::shared_ptr<Hero>> allHeroes) override
	{
		RandomGenerator r{};
		int randNum = r.getRandom(0, 100);
		if (randNum <= 50)
		{
			DivineGrace divineGrace{};
			UtilityEffect effect = divineGrace.getUtilityEffect();
			int randomHeroIndex = r.getRandom(0, 3);
			allHeroes[randomHeroIndex]->getHealed(effect.amount);
		}
		else
		{
			DivineComfort divineComfort{};
			UtilityEffect effect = divineComfort.getUtilityEffect();
			for (const auto& hero : allHeroes)
			{
				hero->getHealed(effect.amount);
			}
		}
	}

	bool operator< (const std::shared_ptr<Hero> &other) const {
		return speed < other->getSpeed();
	}

	bool operator> (const std::shared_ptr<Hero> &other) const {
		return speed > other->getSpeed();
	}
};