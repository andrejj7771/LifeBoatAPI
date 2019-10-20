#include "Controllers/ActionController.h"
#include "Controllers/RowingController.h"
#include "Controllers/FightController.h"
#include "Controllers/CardController.h"

#include "GameObjects/Character.h"
#include "GameObjects/NavigationCard.h"
#include "GameObjects/ProvisionCard.h"

#include <algorithm>

ActionController::ActionController(const std::vector<CharacterPtr> & characters,
								   const std::vector<NavigationCardPtr> & navCards) :
	m_fightController(nullptr),
	m_rowingCotroller(nullptr),
	m_characters(characters),
	m_newSequence(characters),
	m_navigationCards(navCards)
{}

const std::vector<CharacterPtr> & ActionController::getCheracters() const {
	return m_characters;
}

const std::vector<CharacterPtr> & ActionController::getFighters() const {
	return m_fightController->getFighters();
}

const std::vector<CharacterPtr> & ActionController::getRowers() const {
	return m_rowingCotroller->getRowers();
}

void ActionController::setNavigationCards(const std::vector<NavigationCardPtr> & navigationCards) {
	m_navigationCards = navigationCards;
}

const std::vector<NavigationCardPtr> & ActionController::getNavigationCards() const {
	return m_navigationCards;
}

const std::vector<NavigationCardPtr> & ActionController::getCurrentNavCards() const {
	return m_rowingCotroller->getCurrentNavCards();
}

void ActionController::setActionQuery(const std::function<void(const CharacterPtr &, ActionData &)> & query) {
	m_actionQuery = query;
}

void ActionController::setFightQuery(const std::function<bool(const CharacterPtr &, const CharacterPtr &)> & query) {
	m_fightQuery = query;
}

void ActionController::setRowingCardSender(const std::function<std::vector<size_t>(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender) {
	m_rowingCotroller->setCardSender(sender);
}

void ActionController::setRowingUsingGunQuery(const std::function<bool(const CharacterPtr &)> & query) {
	m_rowingCotroller->setUsingGunQuery(query);
}

void ActionController::setFightingQuery(const std::function<int(const CharacterPtr &, const CharacterPtr &, const CharacterPtr &)> & query) {
	m_fightController->setFightQuery(query);
}

bool ActionController::init() {
	if (!initFightController()) {
		return false;
	}
	
	if (!initRowingController()) {
		return false;
	}
	
	return true;
}

void ActionController::execute() {
	auto current = m_characters.begin();
	ActionData data;
	while (current != m_characters.end()) {
		sendActionQuery(*current, data, [this, &current](ActionData & data){
			if (data.actionType == 1) {
				RobCharacter(*current, data.object, data.cardIndex, data.hand);
			} else if (data.actionType == 2) {
				SwapPlaces(*current, data.object);
			} else if (data.actionType == 3) {
				Rowing(*current);
			} else if (data.actionType == 4) {
				bool end = false;
				UseCard(*current, data.object, data.cardIndex, end);
			}
			current++;
		});
		data.clear();
	}
	
	m_characters = m_newSequence;
}

void ActionController::RobCharacter(const CharacterPtr & subject,
									const CharacterPtr & object,
									size_t cardIndex, bool hand) {
	auto card = hand ? object->getHandCard(cardIndex) : object->getTableCard(cardIndex);
	if (object->isUnconscious() || object->isDead()) {
		if (hand == true) {
			object->removeCardFromHand(card);
		} else {
			object->removeCardFromTable(card);
		}
		subject->appendCardToHand(card);
	} else {
		if (subject->getCharacterType() == character_t::Kid && hand == true) {
			object->removeCardFromHand(card);
			subject->appendCardToHand(card);
		} else {
			sendFightQuery(subject, object, [this, &subject, &object, &card, hand](bool result) {
				if (!result) {
					if (hand == true) {
						object->removeCardFromHand(card);
					} else {
						object->removeCardFromTable(card);
					}
					subject->appendCardToHand(card);
				} else {
					m_fightController->startFight(subject, object);
					if (m_fightController->getWinner() == subject) {
						if (hand == true) {
							object->removeCardFromHand(card);
						} else {
							object->removeCardFromTable(card);
						}
						subject->appendCardToHand(card);
					}
				}
			});
		}
	}
}

void ActionController::SwapPlaces(const CharacterPtr & subject,
								  const CharacterPtr & object) {
	auto sIter = std::find(m_newSequence.begin(), m_newSequence.end(), subject);
	auto oIter = std::find(m_newSequence.begin(), m_newSequence.end(), object);
	
	if (object->isUnconscious() || object->isDead()) {
		std::swap(*sIter, *oIter);
	} else {
		sendFightQuery(subject, object, [this, &sIter, &oIter](bool result) {
			if (!result) {
				std::swap(*sIter, *oIter);
			} else {
				m_fightController->startFight(*sIter, *oIter);
				if (m_fightController->getWinner() == *sIter) {
					std::swap(*sIter, *oIter);
				}
			}
		});
	}
}

void ActionController::Rowing(const CharacterPtr & subject) {
	m_rowingCotroller->setCharacter(subject);
	m_rowingCotroller->startRawing();
	
	m_navigationCards = m_rowingCotroller->getNavigationCards();
}

void ActionController::UseCard(const CharacterPtr & subject,
							   const CharacterPtr & object,
							   size_t cardIndex, bool & end) {
	useCard(subject, object, cardIndex, end);
}

bool ActionController::initFightController() {
	try {
		m_fightController = std::make_shared<FightController>(m_characters);
	} catch (const std::exception & error) {
		printf("%s -> %s", __FUNCTION__, error.what());
		return false;
	}
	
	return true;
}

bool ActionController::initRowingController() {
	try {
		m_rowingCotroller = std::make_shared<RowingController>(m_navigationCards);
	} catch (const std::exception & error) {
		printf("%s -> %s", __FUNCTION__, error.what());
		return false;
	}
	
	return true;
}

void ActionController::sendActionQuery(const CharacterPtr & to,
									   ActionData & data,
									   const std::function<void(ActionData &)> & callback) {
	m_actionQuery(to, data);
	callback(data);
}

void ActionController::sendFightQuery(const CharacterPtr & subject,
									  const CharacterPtr & object,
									  const std::function<void(bool)> & callback) {
	callback(m_fightQuery(subject, object));
}

