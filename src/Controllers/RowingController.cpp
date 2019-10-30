#include "Controllers/RowingController.h"
#include "GameObjects/Character.h"
#include "GameObjects/ProvisionCard.h"

RowingController::RowingController(const std::vector<NavigationCardPtr> & navCards) :
	m_character(nullptr),
	m_navigationCards(navCards),
	m_cardCallback(std::make_shared<RC_CardCallback>("RC_CardCallback")),
	m_gunCallback(std::make_shared<RC_GunCallback>("RC_GunCallback")) {}

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
	m_cardCallback->setReceiver([this, &cards](const std::vector<size_t> & indices){
		for (size_t index : indices) {
			m_currentCards.push_back(cards.at(index));
			for (auto card : cards) {
				m_navigationCards.push_back(card);
			}
		}
	});
	m_cardCallback->send(m_character, cards);
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
			m_gunCallback->setReceiver([this, &counter, &card](bool res){
				if (res) {
					m_character->removeCardFromTable(card);
					counter++;
				}
			});
			m_gunCallback->send(m_character);
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
