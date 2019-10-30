#ifndef CARDDISTRIBUTIONCONTROLLER_H
#define CARDDISTRIBUTIONCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"

#include <memory>
#include <vector>
#include <functional>

class Character;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

using callback = Callback<long, const CharacterPtr&, std::vector<ProvisionCardPtr>>;
using CallbackPtr = std::shared_ptr<callback>;

class LIFEBOAT_API CardDistributionController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<ProvisionCardPtr> m_provisionCards;
	
	CallbackPtr m_callback;
	
public:
	
	CardDistributionController(const std::vector<ProvisionCardPtr> & provCards);
	
	~CardDistributionController() = default;
	
	void setChracters(const std::vector<CharacterPtr> & characters);
	const std::vector<ProvisionCardPtr> & getProvisionCards() const;
	
	CallbackPtr getCallback() const;
	
	void execute();
	
private:
	
	std::vector<ProvisionCardPtr> getCurrentCards();
	
};

#endif // CARDDISTRIBUTIONCONTROLLER_H
