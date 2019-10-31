#include "Controllers/IterationTotalController.h"
#include "Controllers/CardController.h"
#include "GameObjects/NavigationCard.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

IterationTotalController::IterationTotalController() :
	m_cardCallback(std::make_shared<ITC_CardCallback>("ITC_CardCallback")),
	m_waterCallback(std::make_shared<ITC_WaterCallback>("ITC_WaterCallback")),
	m_preserverCallback(std::make_shared<ITC_PreserverCallback>("ITC_PreserverCallback")),
	m_usingCardCallback(std::make_shared<ITC_UsingCardCallback>("ITC_UsingCardCallback"))
{
	m_currentPhase = TotalPhase::Unknown;
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

ITC_WaterCallbackPtr IterationTotalController::getITC_WaterCallback() const {
	return m_waterCallback;
}

ITC_PreserverCallbackPtr IterationTotalController::getITC_PreserverCallback() const {
	return m_preserverCallback;
}

ITC_UsingCardCallbackPtr IterationTotalController::getITC_UsingCardCallback() const {
	return m_usingCardCallback;
}

const std::vector<NavigationCardPtr> & IterationTotalController::getNavigationCards() const {
	return m_navigationCards;
}

void IterationTotalController::usingCardQueryCallback(const ActionData & data) {
	auto card = data.object->getHandCard(data.card_index);
	if (card == nullptr)
		return;
	
	auto cardType = card->getCardType();
	auto bind = std::bind(&IterationTotalController::usingCardQueryCallback,
						  this, std::placeholders::_1);
	if (m_currentPhase == TotalPhase::Overboard) {
		if (cardType != provision_e::Chum && cardType != provision_e::LifePreserver) {
			m_usingCardCallback->setReceiver(bind);
			m_usingCardCallback->send(data.object);
		} else {
			if (cardType == provision_e::Chum) {
				for (const CharacterPtr & c : m_overboard) {
					auto chum = findCardByType(data.object, provision_e::Chum, false);
					data.object->removeCardFromTable(chum);
					c->incCriticalHit();
				}
			} else if (cardType == provision_e::LifePreserver) {
				m_preserverCallback->setReceiver([&data](const CharacterPtr & c) {
					auto preserver = findCardByType(data.object, provision_e::LifePreserver, false);
					data.object->removeCardFromTable(preserver);
					c->decCriticalHit();
				});
				m_preserverCallback->send(data.object);
			}
		}
	} else if (m_currentPhase == TotalPhase::Dehydration) {
		if (cardType != provision_e::Water) {
			m_usingCardCallback->send(data.object);
		} else {
			m_waterCallback->setReceiver([&data](const CharacterPtr & c){
				auto water = findCardByType(data.object, provision_e::Water, true);
				data.object->removeCardFromTable(water);
				c->decWaterHit();
			});
			m_waterCallback->send(data.object);
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
			m_currentPhase = TotalPhase::Overboard;
			convertTypesToCharacters(card->getOverboard(), m_overboard);
			for (auto character : m_overboard) {
				if (character->getCharacterType() != character_e::Frenchy &&
						hasCard(character, provision_e::LifePreserver, true)) {
					character->incCriticalHit();
				}
				
				character->looseCards();
			}
			
			for (auto character : m_characters) {
				if (std::find(m_overboard.begin(), m_overboard.end(), character) != m_overboard.end())
					continue;
				
				m_usingCardCallback->send(character);
			}
		}
		
		//dehydrated characters
		{
			m_currentPhase = TotalPhase::Dehydration;
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
				m_usingCardCallback->send(character);
			}
			
			m_currentPhase = TotalPhase::Unknown;
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
		m_cardCallback->send(*characterIter, m_currentCards);
	}
	
	m_fighters.clear();
	m_rowers.clear();
	m_overboard.clear();
	m_dehydrated.clear();
	m_characters.clear();
	for (auto card : m_currentCards)
		m_navigationCards.push_back(card);
}

void IterationTotalController::convertTypesToCharacters(const std::vector<character_e> & types,
														std::vector<CharacterPtr> & characters)
{
	for (auto type : types) {
		for (const CharacterPtr & character : m_characters) {
			if (character->getCharacterType() == type) {
				characters.push_back(character);
				break;
			}
		}
	}
}
