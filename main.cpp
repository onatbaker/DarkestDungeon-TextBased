#include "hero.h"
#include "monster.h"
#include <map>

std::vector<std::shared_ptr<Hero>> heroes;
std::vector<std::shared_ptr<Monster>> monsters;

bool checkMonstersEnd()
{
	for (const auto& m : monsters)
	{
		if (m->isAlive())
			return false;
	}

	std::cout << std::endl << "All monsters are dead! Heroes won!" << std::endl;

	return true;
}

bool checkHeroesEnd()
{
	for (const auto& h : heroes)
	{
		if (h->isAlive())
			return false;
	}

	std::cout << std::endl << "All heroes are dead! Monsters won!" << std::endl;

	return true;
}

void swap(std::shared_ptr<Unit> unit1, std::shared_ptr<Unit> unit2)
{

	std::shared_ptr<Unit> temp = unit1;
	unit1 = unit2;
	unit2 = temp;
}

void moveHero(std::shared_ptr<Hero>& currHero, int moveIdx)
{
	if (moveIdx > currHero->getPosition())
	{
		std::cout << "Moving " << currHero->getName() << " back." << std::endl;
		for (int i = currHero->getPosition(); i < moveIdx; i++)
		{
			swap(heroes[i], heroes[moveIdx]);
			heroes[i]->updatePosition(heroes[moveIdx]->getPosition());
		}
	}
	else
	{
		std::cout << "Moving " << currHero->getName() << " forward." << std::endl;
		for (int i = currHero->getPosition(); i < moveIdx; i--)
		{
			swap(heroes[i], heroes[moveIdx]);
			heroes[i]->updatePosition(heroes[moveIdx]->getPosition());
		}
	}
}

void moveMonster(std::shared_ptr<Monster>& currMonster, int moveIdx)
{
	if (moveIdx > currMonster->getPosition())
	{
		for (int i = currMonster->getPosition(); i < moveIdx; i++)
		{
			std::cout << "Moving " << currMonster->getName() << " back." << std::endl;
			swap(monsters[i], monsters[moveIdx]);
			monsters[i]->updatePosition(monsters[moveIdx]->getPosition());
		}
	}
	else
	{
		std::cout << "Moving " << currMonster->getName() << " forward." << std::endl;
		for (int i = currMonster->getPosition(); i < moveIdx; i--)
		{
			swap(monsters[i], monsters[moveIdx]);
			monsters[i]->updatePosition(monsters[moveIdx]->getPosition());
		}
	}
}

void heroMakeDecisionRandom(std::shared_ptr<Hero> currentHero)
{
	std::cout << "Current Hero is " + currentHero->getName() + " (hp: " << currentHero->getHp() << ") at position " + std::to_string(currentHero->getPosition()) << std::endl;
	currentHero->processTurn();
	RandomGenerator r{};
	int randomChoice = r.getRandom(0, 100);
	if (randomChoice < 75)
	{
		int randomMonsterIndex = r.getRandom(0, 3);
		auto targetMonster = monsters[randomMonsterIndex];
		bool moveNeeded = currentHero->attackWithSkillsAndReturnIfMoveNeeded(targetMonster);
		moveNeeded &= currentHero->getPosition() > 0;
		if (moveNeeded)
		{
			moveHero(currentHero, currentHero->getPosition() - 1);
		}
	}
	else if (randomChoice >= 75 && randomChoice < 90)
	{
		currentHero->decideOnUtility(heroes);
	}
	else
	{
		// Ugly move code. Too bad!
		MoveSkill moveSkill{};
		int randNum = r.getRandom(-100, 100);
		int moveDir = randNum < 0 ? -1 : 1;
		int moveIndex = moveSkill.getSwapIndexForMove(currentHero->getPosition(), moveDir);
		moveHero(currentHero, moveIndex);
	}
}

void monsterMakeDecisionRandom(std::shared_ptr<Monster> currentMonster)
{
	std::cout << "Current Monster is " + currentMonster->getName() + " (hp: " << currentMonster->getHp() << ") at position " + std::to_string(currentMonster->getPosition()) << std::endl;
	RandomGenerator r{};
	currentMonster->processTurn();
	int randomChoice = r.getRandom(0, 100);
	if (randomChoice < 75)
	{
		int randomHeroIndex = r.getRandom(0, 3);
		auto targetHero = heroes[randomHeroIndex];
		bool moveNeeded = currentMonster->attackWithSkillsAndReturnIfMoveNeeded(targetHero);
		moveNeeded &= currentMonster->getPosition() > 0; // same things as: attackWithSkillsAndReturnIfMoveNeeded(targetHero) && currentMonster->getPosition() > 0;
		if (moveNeeded)
		{
			moveMonster(currentMonster, currentMonster->getPosition() - 1);
		}
	}
	else if (randomChoice >= 75 && randomChoice < 90)
	{
		if (currentMonster->doesHaveUtilityFunction())
		{
			currentMonster->decideOnUtility(monsters);
		}
		else
		{
			int randomHeroIndex = r.getRandom(0, 3);
			auto targetHero = heroes[randomHeroIndex];
			bool moveNeeded = currentMonster->attackWithSkillsAndReturnIfMoveNeeded(targetHero);
			moveNeeded &= currentMonster->getPosition() < 3;
			if (moveNeeded)
			{
				moveMonster(currentMonster, 1);
			}
		}
	}
	else
	{
		// Ugly move code. Too bad!
		MoveSkill moveSkill{};
		int randNum = r.getRandom(-100, 100);
		int moveDir = randNum < 0 ? -1 : 1;
		int moveIndex = moveSkill.getSwapIndexForMove(currentMonster->getPosition(), moveDir);
		moveMonster(currentMonster, currentMonster->getPosition() + 1);
	}
}

int main()
{
	heroes.reserve(4);
	heroes.push_back(std::make_shared<Crusader>("Crusader #1", 0));
	heroes.push_back(std::make_shared<Crusader>("Crusader #2", 1));
	heroes.push_back(std::make_shared<Vestal>("Vestal #1", 2));
	heroes.push_back(std::make_shared<Vestal>("Vestal #2", 3));

	monsters.reserve(4);
	monsters.push_back(std::make_shared<BoneSoldier>("BoneSoldier #1", 0));
	monsters.push_back(std::make_shared<BoneSoldier>("BoneSoldier #2", 1));
	monsters.push_back(std::make_shared<BoneDefender>("BoneDefender #1", 2));
	monsters.push_back(std::make_shared<BoneDefender>("BoneDefender #2", 3));


	bool allMonstersDead = false;
	bool allHeroesDead = false;
	int turn = 0;
	while (!allHeroesDead && !allMonstersDead)
	{
		RandomGenerator r{};
		for (const auto& hero : heroes)
		{
			int speedIncrement = r.getRandom(0, 8);
			hero->increaseSpeedBy(speedIncrement);
		}
		sort(heroes.begin(), heroes.end(), [](const std::shared_ptr<Hero>& lhs, const std::shared_ptr<Hero>& rhs) {
			return lhs->getSpeed() > rhs->getSpeed();
		});

		for (const auto& monster : monsters)
		{
			int speedIncrement = r.getRandom(0, 8);
			monster->increaseSpeedBy(speedIncrement);
		}
		sort(monsters.begin(), monsters.end(), [](const std::shared_ptr<Monster>& lhs, const std::shared_ptr<Monster>& rhs) {
			return lhs->getSpeed() > rhs->getSpeed();
		});

		turn++;
		std::cout << "-------------------------------------------------" << std::endl << std::endl;
		std::cout << "TURN " + std::to_string(turn) << ": " << std::endl;

		for (int i = heroes.size() - 1; i >= 0; i--)
		{
			std::cout << heroes[i]->getName() << " (speed: " << heroes[i]->getSpeed() << ", hp: " << heroes[i]->getHp() << "), ";
		}
		std::cout << "  |   ";
		for (const auto& monster : monsters)
		{
			std::cout << monster->getName() << " (speed: " << monster->getSpeed() << ", hp: "  << monster->getHp() << "), ";
		}
		std::cout << std::endl;

		std::cout << "Heroes' turn:" << std::endl;
		int heroPos = 0;
		for (const auto& hero : heroes)
		{
			hero->updatePosition(heroPos++);
			heroMakeDecisionRandom(hero);
		}

		int monsterPos = 0;
		std::cout << std::endl << "Monsters' turn:" << std::endl;
		for (const auto& monster : monsters)
		{
			monster->updatePosition(monsterPos);
			monsterMakeDecisionRandom(monster);
		}

		allHeroesDead = checkHeroesEnd();
		allMonstersDead = checkMonstersEnd();
	}
}