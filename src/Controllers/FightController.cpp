#include "Controllers/FightController.h"
#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

FightController::FightController(const std::vector<CharacterPtr> & characters) :
	m_winner(nullptr),
	m_characters(characters),
	m_callback(std::make_shared<FC_Callback>("FC_Callback"))
{}

void FightController::startFight(const CharacterPtr & subject,
								 const CharacterPtr & object) {
	m_aTeam.push_back(subject);
	m_bTeam.push_back(object);
	
	if (std::find(m_fighters.begin(), m_fighters.end(), subject) != m_fighters.end())
		m_fighters.push_back(subject);
	
	if (std::find(m_fighters.begin(), m_fighters.end(), object) != m_fighters.end())
		m_fighters.push_back(object);
	
	auto characterIt = m_characters.begin();
	while (characterIt != m_characters.end()) {
		if (*characterIt == subject ||
				*characterIt == object) {
			characterIt++;
			continue;
		}
		
		m_callback->setReceiver([this, &characterIt](int side) {
			if (side == 1) {
				m_aTeam.push_back(*characterIt);
			} else if (side == 2) {
				m_bTeam.push_back(*characterIt);
			}
			
			if (side != 0) {
				if (std::find(m_fighters.begin(), m_fighters.end(), *characterIt) != m_fighters.end()) {
					m_fighters.push_back(*characterIt);
				}
			}
		});
		m_callback->send(*characterIt, subject, object);
	}
	
	fightTotal();
	m_aTeam.clear();
	m_bTeam.clear();
}

void FightController::setCharacters(const std::vector<CharacterPtr> & characters) {
	m_characters = characters;
}

const std::vector<CharacterPtr> & FightController::getFighters() const {
	return m_fighters;
}

const CharacterPtr & FightController::getWinner() const {
	return m_winner;
}

FC_CallbackPtr FightController::getCallback() const {
	return m_callback;
}

void FightController::clear() {
	m_aTeam.clear();
	m_bTeam.clear();
	m_fighters.clear();
}

int FightController::getTeamTotal(const std::vector<CharacterPtr> & team) {
	int total = 0;
	
	for (auto a : team) {
		total += a->getTotalSize();
		size_t numHandCards = a->getNumHandCards();
		std::vector<ProvisionCardPtr> toRemove;
		for (size_t i = 0; i < numHandCards; ++i) {
			auto aHandCard = a->getHandCard(i);
			if (aHandCard->getCardType() == provision_t::FlareGun)
				toRemove.push_back(aHandCard);
		}
		
		for (auto remove : toRemove)
			a->removeCardFromHand(remove);
	}
	
	return total;
}

void FightController::fightTotal() {
	int aSize = getTeamTotal(m_aTeam);
	int bSize = getTeamTotal(m_bTeam);
	
	m_winner = aSize > bSize ? *m_aTeam.begin() : *m_bTeam.begin();
}
