#include "Controllers/RowingController.h"
#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

RowingController::RowingController(const std::vector<NavigationCardPtr> & navCards) :
	m_character(nullptr),
	m_navigationCards(navCards)
{}

void RowingController::setCardSender(const Sender & sender) {
	m_cardSender = sender;
}

void RowingController::setUsingGunQuery(const Query & query) {
	m_usingGunQuery = query;
}

const std::vector<NavigationCardPtr> & RowingController::getNavigationCards() const {
	return m_navigationCards;
}

const std::vector<NavigationCardPtr> & RowingController::getCurrentNavCards() const {
	return m_currentCards;
}

const std::vector<CharacterPtr> & RowingController::getRowers() const {
	return m_rowers;
}

void RowingController::setCharacter(const CharacterPtr & character) {
	m_character = character;
}

void RowingController::startRawing() {
	std::vector<NavigationCardPtr> cards = getCurrentCards();
	sendCards(cards, m_character, [this, &cards](const std::vector<size_t> & indices) mutable {
		for (size_t index : indices) {
			m_currentCards.push_back(cards.at(index));
			cards.erase(cards.begin() + static_cast<long>(index));
			for (auto card : cards) {
				m_navigationCards.push_back(card);
			}
		}
	});
}

std::vector<NavigationCardPtr> RowingController::getCurrentCards() {
	std::vector<NavigationCardPtr> cards;
	size_t numCards = m_character->getNumTableCards();
	int counter = 2;
	for (size_t i = 0; i < numCards; ++i) {
		auto card = m_character->getTableCard(i);
		auto cardType = card->getCardType();
		switch (cardType) {
			case provision_t::Oar:
			case provision_t::Compass: {
				counter++;
				break;
			}
			case provision_t::FlareGun: {
				sendUsingGunQuery(m_character, [this, &counter, &card](bool result){
					if (result) {
						m_character->removeCardFromTable(card);
						counter++;
					}
				});
				break;
			}
			default:
				continue;
		}
	}
	
	for (int i = 0; i < counter; ++i) {
		auto iter = m_navigationCards.begin();
		cards.push_back(*iter);
		m_navigationCards.erase(iter);
	}
	
	return cards;
}

void RowingController::sendCards(const std::vector<NavigationCardPtr> & cards,
								 const CharacterPtr & to,
								 const std::function<void(const std::vector<size_t> &)> & callback) {
	callback(m_cardSender(cards, to));
}

void RowingController::sendUsingGunQuery(const CharacterPtr & sendTo,
										 const std::function<void(bool)> & callback) {
	callback(m_usingGunQuery(sendTo));
}
