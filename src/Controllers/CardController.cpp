#include "Controllers/CardController.h"
#include "GameObjects/Character.h"

bool useCard(const CharacterPtr & user,
			 const CharacterPtr & target,
			 size_t cardIndex, bool & end)
{
	auto card = user->getHandCard(cardIndex);
	auto cardType = card->getCardType();
	switch (cardType) {
	case provision_e::FlareGun: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		user->setBonusSize(user->getBonusSize() + 8);
		end = true;
		break;
	}
	case provision_e::Parasol: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		end = true;
		break;
	}
	case provision_e::MedKit: {
		if (target == nullptr)
			return false;
		
		user->removeCardFromTable(card);
		target->decCriticalHit();
		end = true;
		break;
	}
	case provision_e::Chum: {
		if (target == nullptr)
			return false;
		
		user->removeCardFromHand(card);
		user->removeCardFromTable(card);
		target->incCriticalHit();
		break;
	}
	case provision_e::Water: {
		if (target == nullptr)
			return false;
		
		user->removeCardFromTable(card);
		target->decWaterHit();
		break;
	}
	case provision_e::LifePreserver: 
	case provision_e::Compass:
	{
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		break;
	}
	case provision_e::Oar: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		user->setBonusSize(user->getBonusSize() + 1);
		break;
	}
	case provision_e::BlackJack: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		user->setBonusSize(user->getBonusSize() + 2);
		break;
	}
	case provision_e::Knife: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		user->setBonusSize(user->getBonusSize() + 3);
		break;
	}
	case provision_e::GaffingHook: {
		user->removeCardFromHand(card);
		user->appendCardToTable(card);
		user->setBonusSize(user->getBonusSize() + 4);
		break;
	}
	default:
		return false;
	}
	
	return true;
}

bool hasCard(const CharacterPtr & character,
			 provision_e type, bool hand) {
	size_t size = hand ? character->getNumHandCards() : character->getNumTableCards();
	for (size_t i = 0; i < size; ++i) {
		auto card = hand ? character->getHandCard(i) : character->getTableCard(i);
		if (card->getCardType() == type)
			return true;
	}
	
	return false;
}

ProvisionCardPtr findCardByType(const CharacterPtr & character,
										provision_e type, bool hand) {
	size_t size = hand ? character->getNumHandCards() : character->getNumTableCards();
	for (size_t i = 0; i < size; ++i) {
		auto card = hand ? character->getHandCard(i) : character->getTableCard(i);
		if (card->getCardType() == type) {
			return card;
		}
	}
	
	return nullptr;
}

