#ifndef CARDDISTRIBUTIONCONTROLLER_H
#define CARDDISTRIBUTIONCONTROLLER_H

#include "LifeboatAPI_global.h"

#include <memory>
#include <vector>
#include <functional>

class Character;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

using senderFunction = std::function<long(const CharacterPtr &, const std::vector<ProvisionCardPtr> &)>;
using callbackFunction = std::function<void(long)>;

class LIFEBOAT_API CardDistributionController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<ProvisionCardPtr> m_provisionCards;
	
	/// this function should return the index of chosen card
	std::function<long(const CharacterPtr &, const std::vector<ProvisionCardPtr> &)> m_sender;
	
public:
	
	CardDistributionController(const std::vector<ProvisionCardPtr> & provCards);
	
	~CardDistributionController() = default;
	
	void setChracters(const std::vector<CharacterPtr> & characters);
	const std::vector<ProvisionCardPtr> & getProvisionCards() const;
	
	void execute();
	void setSender(const senderFunction & func);
	
private:
	
	std::vector<ProvisionCardPtr> getCurrentCards();
	void sendCards(const CharacterPtr & to,
				   const std::vector<ProvisionCardPtr> & cards,
				   const callbackFunction & callback);
	
	
};

#endif // CARDDISTRIBUTIONCONTROLLER_H
