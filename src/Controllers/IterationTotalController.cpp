#include "Controllers/IterationTotalController.h"
#include "Controllers/CardController.h"
#include "GameObjects/NavigationCard.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

IterationTotalController::IterationTotalController() :
	m_cardCallback(std::make_shared<ITC_CardCallback>("ITC_CardCallback"))
{
	m_currentPhase = totalPhase::Unknown;
}

size_t IterationTotalController::getNumBirds() const {
	return m_birdCards.size();
}

void IterationTotalController::setCharacters(const std::vector<CharacterPtr> & characters) {
	m_characters = characters;
}

void IterationTotalController::setNavigationCards(const std::vector<NavigationCardPtr> & navigationCards) {
	m_navigationCards = navigationCards;
}

void IterationTotalController::setFighters(const std::vector<CharacterPtr> & fighters) {
	m_fighters = fighters;
}

void IterationTotalController::setRowers(const std::vector<CharacterPtr> & rowers) {
	m_rowers = rowers;
}

void IterationTotalController::setCurrentNavCards(const std::vector<NavigationCardPtr> & cards) {
	m_currentCards = cards;
}

ITC_CardCallbackPtr IterationTotalController::getITC_CardCallback() const {
	return m_cardCallback;
}

const std::vector<NavigationCardPtr> & IterationTotalController::getNavigationCards() const {
	return m_navigationCards;
}

void IterationTotalController::usingCardQueryCallback(const CharacterPtr & from,
													  const ProvisionCardPtr & card)
{
	if (card == nullptr)
		return;
	
	auto cardType = card->getCardType();
	auto bind = std::bind(&IterationTotalController::usingCardQueryCallback,
						  this, std::placeholders::_1, std::placeholders::_2);
	if (m_currentPhase == totalPhase::Overboard) {
		if (cardType != provision_t::Chum && cardType != provision_t::LifePreserver) {
			sendUsingCardQuery(from, bind);
		} else {
			if (cardType == provision_t::Chum) {
				for (const CharacterPtr & c : m_overboard) {
					auto chum = findCardByType(from, provision_t::Chum, false);
					from->removeCardFromTable(chum);
					c->incCriticalHit();
				}
			} else if (cardType == provision_t::LifePreserver) {
				sendQuery(from, [&from](const CharacterPtr & c){
					auto preserver = findCardByType(from, provision_t::LifePreserver, false);
					from->removeCardFromTable(preserver);
					c->decCriticalHit();
				});
			}
		}
	} else if (m_currentPhase == totalPhase::Dehydration) {
		if (cardType != provision_t::Water) {
			sendUsingCardQuery(from, bind);
		} else {
			sendQuery(from, [&from](const CharacterPtr & c){
				auto water = findCardByType(from, provision_t::Water, true);
				from->removeCardFromTable(water);
				c->decWaterHit();
			});
		}
	}
}

void IterationTotalController::execute() {
	if (m_currentCards.size() == 0) {
		auto firstCard = m_navigationCards.begin();
		m_currentCards.push_back(*firstCard);
		m_navigationCards.erase(firstCard);
	}
	
	auto solve = [this](const NavigationCardPtr & card, size_t index){
		if (card->plusBird()) {
			m_birdCards.push_back(card);
			m_currentCards.erase(m_currentCards.begin() + static_cast<long>(index));
		} else if (card->loseBirds()) {
			for (auto bird : m_birdCards) {
				m_navigationCards.push_back(bird);
			}
			m_birdCards.clear();
		}
		
		//overboard characters
		{
			m_currentPhase = totalPhase::Overboard;
			convertTypesToCharacters(card->getOverboard(), m_overboard);
			for (auto character : m_overboard) {
				if (character->getCharacterType() != character_t::Frenchy &&
						hasCard(character, provision_t::LifePreserver, true)) {
					character->incCriticalHit();
				}
				
				character->looseCards();
			}
			
			for (auto character : m_characters) {
				if (std::find(m_overboard.begin(), m_overboard.end(), character) != m_overboard.end())
					continue;
				
				sendUsingCardQuery(character, std::bind(&IterationTotalController::usingCardQueryCallback,
														this, std::placeholders::_1, std::placeholders::_2));
			}
		}
		
		//dehydrated characters
		{
			m_currentPhase = totalPhase::Dehydration;
			convertTypesToCharacters(card->getThirsty(), m_dehydrated);
			
			auto find = [](const std::vector<CharacterPtr> & src, const CharacterPtr & target) -> bool {
				return std::find(src.begin(), src.end(), target) != src.end();
			};
			
			for (const CharacterPtr & character : m_characters) {
				if (find(m_dehydrated, character)) {
					character->incWaterHit();
				}
				
				if (card->isFighters() && find(m_fighters, character)) {
					character->incWaterHit();
				}
				
				if (card->isRowers() && find(m_rowers, character))
				{
					character->incWaterHit();
				}
			}
			
			for (const CharacterPtr & character : m_dehydrated) {
				sendUsingCardQuery(character, std::bind(&IterationTotalController::usingCardQueryCallback,
														this, std::placeholders::_1, std::placeholders::_2));
			}
			
			m_currentPhase = totalPhase::Unknown;
		}
	};
	if (m_currentCards.size() == 1) {
		solve(*m_currentCards.begin(), 0);
	} else {
		auto characterIter = m_characters.end() - 1;
		m_cardCallback->setReceiver([this, solve](size_t index) {
			auto card = m_currentCards.at(index);
			solve(card, index);
		});
		m_cardCallback->send(*characterIter);
	}
	
	m_fighters.clear();
	m_rowers.clear();
	m_overboard.clear();
	m_dehydrated.clear();
	m_characters.clear();
	for (auto card : m_currentCards)
		m_navigationCards.push_back(card);
}

void IterationTotalController::convertTypesToCharacters(const std::vector<character_t> & types,
														std::vector<CharacterPtr> & characters) {
	for (auto type : types) {
		for (const CharacterPtr & character : m_characters) {
			if (character->getCharacterType() == type) {
				characters.push_back(character);
				break;
			}
		}
	}
}

void IterationTotalController::sendUsingCardQuery(const CharacterPtr & to,
												  const std::function<void(const CharacterPtr &,
																		   const ProvisionCardPtr &)> & callback) {
	if (m_currentPhase == totalPhase::Unknown)
		return;
	
	callback(to, m_usingCardQuery(to, m_currentPhase));
}

void IterationTotalController::sendQuery(const CharacterPtr & to,
				const std::function<void(const CharacterPtr &)> & callback) {
	callback(m_healQuery(to));
}
