#ifndef CARDDISTRIBUTIONCONTROLLER_H
#define CARDDISTRIBUTIONCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

class LIFEBOAT_API CardDistributionController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<ProvisionCardPtr> m_provisionCards;
	
	DC_CallbackPtr m_callback;
	
public:
	
	CardDistributionController(const std::vector<ProvisionCardPtr> & provCards);
	
	~CardDistributionController() = default;
	
	void setChracters(const std::vector<CharacterPtr> & characters);
	const std::vector<ProvisionCardPtr> & getProvisionCards() const;
	
	DC_CallbackPtr getCallback() const;
	
	void execute();
	
private:
	
	std::vector<ProvisionCardPtr> getCurrentCards();
	
};

#endif // CARDDISTRIBUTIONCONTROLLER_H
