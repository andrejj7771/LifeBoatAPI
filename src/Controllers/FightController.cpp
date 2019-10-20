#include "Controllers/FightController.h"
#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

FightController::FightController(const std::vector<CharacterPtr> & characters) :
	m_winner(nullptr),
	m_characters(characters)
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
		
		sendFightQuery(*characterIt, subject, object,
					   [this, &characterIt](int side, const CharacterPtr & character)
		{
			if (side == 1) {
				m_aTeam.push_back(character);
			} else if (side == 2) {
				m_bTeam.push_back(character);
			}
			
			if (side != 0) {
				if (std::find(m_fighters.begin(), m_fighters.end(), character) != m_fighters.end()) {
					m_fighters.push_back(character);
				}
			}
			
			characterIt++;
		});
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

void FightController::setFightQuery(const FightQuery &query) {
	m_fightQuery = query;
}

void FightController::clear() {
	m_aTeam.clear();
	m_bTeam.clear();
	m_fighters.clear();
}

void FightController::sendFightQuery(const CharacterPtr & to,
									 const CharacterPtr & subject,
									 const CharacterPtr & object,
									 const std::function<void(int, const CharacterPtr &)> & callback) {
	int resp = m_fightQuery(to, subject, object);
	callback(resp, to);
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
