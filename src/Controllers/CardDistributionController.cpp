#include "Controllers/CardDistributionController.h"
#include "GameObjects/Character.h"

#include <algorithm>
#include <functional>

CardDistributionController::CardDistributionController(const std::vector<ProvisionCardPtr> & provCards) :
	m_provisionCards(provCards)
{}

void CardDistributionController::setChracters(const std::vector<CharacterPtr> & characters) {
	m_characters = characters;
}

const std::vector<ProvisionCardPtr> & CardDistributionController::getProvisionCards() const {
	return m_provisionCards;
}

void CardDistributionController::execute() {
	auto cards = getCurrentCards();
	
	auto currentCharacter = m_characters.begin();
	
	while(currentCharacter != m_characters.end()) {
		if (cards.size() == 0)
			break;
		
		sendCards(*currentCharacter, cards,
				  [&cards, &currentCharacter](long index) mutable
		{
			if (index < 0) {
				currentCharacter++;
				return;
			}
			
			auto it = cards.begin() + index;
			
			(*currentCharacter)->appendCardToHand(*it);
			cards.erase(it);
			currentCharacter++;
		});
	}
	
}

void CardDistributionController::setSender(const senderFunction & func) {
	m_sender = func;
}

void CardDistributionController::sendCards(const CharacterPtr & to,
										   const std::vector<ProvisionCardPtr> & cards,
										   const callbackFunction & callback)
{
	callback(m_sender(to, cards));
}

std::vector<ProvisionCardPtr> CardDistributionController::getCurrentCards() {
	std::vector<ProvisionCardPtr> cards;
	
	for (size_t i = 0; i < m_characters.size() && !m_provisionCards.empty(); ++i) {
		auto begin = m_provisionCards.begin();
		cards.push_back(*begin);
		m_provisionCards.erase(begin);
	}
	
	return cards;
}
