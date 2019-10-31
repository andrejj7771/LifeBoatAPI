#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

Character::Character() :
	GameObject (obj_e::Character),
	m_cType(character_e::Unknown)
{
	m_size = 3;
	m_survivalValue = 9;
	m_bonusSize = 0;
	
	m_waterHit = 0;
	m_criticalHit = 0;
	
	m_isDead = false;
	m_isUnconscious = false;
	
	m_enemy = nullptr;
	m_friend = nullptr;
}

void Character::setName(const std::string & name) {
	m_name = name;
}

const std::string & Character::getName() const {
	return m_name;
}

int Character::getSize() const {
	return m_size;
}

int Character::getSurvivalValue() const {
	return m_survivalValue;
}

void Character::setBonusSize(int size) {
	m_bonusSize = size;
}

int Character::getBonusSize() const {
	return m_bonusSize;
}

int Character::getTotalSize() const {
	return m_bonusSize + m_size;
}

void Character::incWaterHit() {
	m_waterHit++;
}

void Character::decWaterHit() {
	if (m_waterHit != 0)
		m_waterHit--;
}

void Character::setWaterHit(int value) {
	if (value > 0)
		m_waterHit = value;
}

int Character::getWaterHit() const {
	return m_waterHit;
}

void Character::incCriticalHit() {
	if (m_isDead) {
		return;
	}
	
	m_criticalHit++;
	
	if (m_criticalHit == m_size) {
		m_isUnconscious = true;
		return;
	}
	
	if (m_isUnconscious) {
		m_isDead = true;
	}
}

void Character::decCriticalHit() {
	if (m_isDead) {
		return;
	}
	
	if (m_criticalHit != 0)
		m_criticalHit--;
	
	if (m_isUnconscious) {
		m_isUnconscious = false;
	}
}

void Character::setCriticalHit(int value) {
	if (m_isDead) {
		return;
	}
	
	if (value >= 0)
		m_criticalHit = value;
	
	if (m_criticalHit >= m_size) {
		m_isUnconscious = true;
		if (m_criticalHit > m_size) {
			m_isDead = true;
		}
		
		return;
	}
	
	if (m_isUnconscious) {
		m_isUnconscious = false;
	}
}

int Character::getCriticalHit() const {
	return m_criticalHit;
}

bool Character::isUnconscious() const {
	return m_isUnconscious;
}

bool Character::isDead() const {
	return m_isDead;
}

void Character::appendCardToTable(const ProvisionCardPtr & card) {
	if (card == nullptr) {
		return;
	}
	
	if (std::find(m_table.begin(), m_table.end(), card) == m_table.end())
		m_table.push_back(card);
}

void Character::removeCardFromTable(const ProvisionCardPtr & card) {
	auto cardIter = std::find(m_table.begin(), m_table.end(), card);
	if (cardIter != m_table.end())
		m_table.erase(cardIter);
}

const ProvisionCardPtr & Character::getTableCard(size_t index) const {
	if (index >= m_table.size()) {
		return NullProvision;
	}
	
	return m_table.at(index);
}

size_t Character::getNumTableCards() const {
	return m_table.size();
}

void Character::appendCardToHand(const ProvisionCardPtr & card) {
	if (card == nullptr) {
		return;
	}
	
	if (std::find(m_hand.begin(), m_hand.end(), card) == m_hand.end())
		m_hand.push_back(card);
}

void Character::removeCardFromHand(const ProvisionCardPtr & card) {
	auto cardIter = std::find(m_hand.begin(), m_hand.end(), card);
	if (cardIter != m_hand.end())
		m_hand.erase(cardIter);
}

const ProvisionCardPtr & Character::getHandCard(size_t index) const {
	if (index >= m_hand.size()) {
		return NullProvision;
	}
	
	return m_hand.at(index);
}

size_t Character::getNumHandCards() const {
	return m_hand.size();
}

void Character::setEnemy(CharacterPtr character) {
	m_enemy = character;
}

CharacterPtr Character::getEnemy() const {
	return m_enemy;
}

void Character::setFriend(CharacterPtr character) {
	m_friend = character;
}

CharacterPtr Character::getFriend() const {
	return m_friend;
}

character_e Character::getCharacterType() const {
	return m_cType;
}

void Character::looseCards() {
	auto cardIterrator = m_table.begin();
	bool stop = false;
	while (!stop) {
		if (m_table.size() == 0) {
			stop = true;
			continue;
		}
		
		if ((*cardIterrator)->getCardType() == provision_e::LifePreserver) {
			if (m_table.size() == 1) {
				stop = true;
				continue;
			}
		} else {
			m_table.erase(cardIterrator);
		}
		
		++cardIterrator;
	}
}
