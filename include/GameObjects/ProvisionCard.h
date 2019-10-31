#ifndef PROVISIONCARD_H
#define PROVISIONCARD_H

#include "GameObject.h"

enum class provision_e : char {
	Unknown = -1,
	FlareGun = 0,
	LifePreserver = 1,
	Chum = 2,
	Oar = 3,
	Parasol = 4,
	Compass = 5,
	Painting_2 = 6,
	Painting_3 = 7,
	Jewels = 8,
	Money = 9,
	MedKit = 10,
	Water = 11,
	BlackJack = 12,
	Knife = 13,
	GaffingHook = 14
};

class LIFEBOAT_API ProvisionCard : public GameObject {
	
	provision_e m_cType;
	std::string m_name;
	
public:
	
	ProvisionCard();
	~ProvisionCard() override = default;
	
	provision_e getCardType() const;
	
	const std::string & getName() const;
	
protected:
	
	bool onLoad(const rapidjson::Value & doc) override;
	
};

#endif // PROVISIONCARD_H
