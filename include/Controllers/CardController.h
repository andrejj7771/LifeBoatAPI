#ifndef CARDCONTROLLER_H
#define CARDCONTROLLER_H

#include "GameObjects/ProvisionCard.h"

#include <memory>

class Character;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

bool useCard(const CharacterPtr & subject,
			 const CharacterPtr & object,
			 size_t cardIndex, bool & end);

bool hasCard(const CharacterPtr & character, provision_t type, bool hand);
ProvisionCardPtr findCardByType(const CharacterPtr & character, provision_t type, bool hand);

#endif // CARDCONTROLLER_H
