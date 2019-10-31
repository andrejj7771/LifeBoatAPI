#include "Controllers/TotalController.h"

#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>
#include <cmath>

const CharacterPtr & TotalController::start(const std::vector<CharacterPtr> & characters) {
	std::vector<int> scores;
	for (auto character : characters) {
		scores.push_back(countScore(character));
	}
	
	auto it = std::max_element(scores.begin(), scores.end());
	auto index = std::distance(scores.begin(), it);
	return *(characters.begin() + index);
}

int TotalController::countScore(const CharacterPtr & character) {
	int score = 0;
	if (!character->isDead())
		score += character->getSurvivalValue();
	
	auto eCharacter = character->getEnemy();
	auto fCharacter = character->getFriend();
	
	if (eCharacter->isDead())
		score += eCharacter->getSize();
	
	if (!fCharacter->isDead())
		score += fCharacter->getSurvivalValue();
	
	
	int paintings = 0;
	int jewels = 0;
	int money = 0;
	
	size_t tableSize = character->getNumTableCards();
	for (size_t i = 0; i < tableSize; ++i) {
		auto card = character->getTableCard(i);
		auto type = card->getCardType();
		if (type == provision_e::Money) {
			money += 1;
		} else if (type == provision_e::Jewels) {
			jewels += 1;
		} else if (type == provision_e::Painting_3) {
			paintings += 3;
		} else if (type == provision_e::Painting_2) {
			paintings += 2;
		}
	}
	
	jewels = static_cast<int>(std::pow(2, jewels == 1 ? jewels - 1 : jewels));
	
	auto type = character->getCharacterType();
	score += type == character_e::Captain ? money * 2 : money;
	score += type == character_e::Lauren ? jewels * 2 : jewels;
	score += type == character_e::Stephen ? paintings * 2 : paintings;
	
	return score;
}
