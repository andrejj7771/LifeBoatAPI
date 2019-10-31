#include "Controllers/ActionController.h"
#include "Controllers/RowingController.h"
#include "Controllers/FightController.h"
#include "Controllers/CardController.h"

#include "GameObjects/Character.h"
#include "GameObjects/NavigationCard.h"
#include "GameObjects/ProvisionCard.h"

ActionController::ActionController(const std::vector<CharacterPtr> & characters,
								   const std::vector<NavigationCardPtr> & navCards) :
	m_fightController(nullptr),
	m_rowingCotroller(nullptr),
	m_characters(characters),
	m_newSequence(characters),
	m_navigationCards(navCards),
	m_actionCallback(std::make_shared<AC_ActionCallback>("AC_ActionCallback")),
	m_fightCallback(std::make_shared<AC_FightCallback>("AC_FightCallback")) {}

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

AC_ActionCallbackPtr ActionController::getAC_ActionCallback() const {
	return m_actionCallback;
}

AC_FightCallbackPtr ActionController::getAC_FightCallback() const {
	return m_fightCallback;
}

RC_CardCallbackPtr ActionController::getRC_CardCallback() const {
	return m_rowingCotroller->getRC_CardCallback();
}

RC_GunCallbackPtr ActionController::getRC_GunCallback() const {
	return m_rowingCotroller->getRC_GunCallback();
}

FC_CallbackPtr ActionController::getFC_Callback() const {
	return m_fightController->getCallback();
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
		m_actionCallback->setReceiver([this, &current](ActionData & data){
			callback(data, current);
		});
		m_actionCallback->send(*current);
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
		if (subject->getCharacterType() == character_e::Kid && hand == true) {
			object->removeCardFromHand(card);
			subject->appendCardToHand(card);
		} else {
			m_fightCallback->setReceiver([this, &subject, &object, &card, hand](bool res){
				if (!res) {
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
			m_fightCallback->send(subject, object);
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
		m_fightCallback->setReceiver([this, &sIter, &oIter](bool result) {
			if (!result) {
				std::swap(*sIter, *oIter);
			} else {
				m_fightController->startFight(*sIter, *oIter);
				if (m_fightController->getWinner() == *sIter) {
					std::swap(*sIter, *oIter);
				}
			}
		});
		m_fightCallback->send(subject, object);
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

void ActionController::callback(ActionData & data, std::vector<CharacterPtr>::iterator & current) {
	if (data.action_type == action_e::Robbing) {
		RobCharacter(*current, data.object, data.card_index, data.hand_card);
	} else if (data.action_type == action_e::Swappin) {
		SwapPlaces(*current, data.object);
	} else if (data.action_type == action_e::Rowing) {
		Rowing(*current);
	} else if (data.action_type == action_e::CardUsing) {
		bool end = false;
		UseCard(*current, data.object, data.card_index, end);
		if (!end) {
			m_actionCallback->setReceiver([this, &current](ActionData & data){
				callback(data, current);
			});
			m_actionCallback->send(*current);
		}
	}
	current++;
}
