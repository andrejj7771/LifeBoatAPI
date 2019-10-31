#ifndef CARDCONTROLLER_H
#define CARDCONTROLLER_H

#include "GameObjects/ProvisionCard.h"
#include "Utils/Utils.h"

bool useCard(const CharacterPtr & subject,
			 const CharacterPtr & object,
			 size_t cardIndex, bool & end);

bool hasCard(const CharacterPtr & character, provision_e type, bool hand);
ProvisionCardPtr findCardByType(const CharacterPtr & character, provision_e type, bool hand);

#endif // CARDCONTROLLER_H
