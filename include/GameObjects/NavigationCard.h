#ifndef CARD_H
#define CARD_H

#include "GameObject.h"
#include "Character.h"

#include <rapidjson/document.h>

class LIFEBOAT_API NavigationCard : public GameObject {
	
	std::vector<character_e> m_overboard;
	std::vector<character_e> m_thirsty;
	
	bool m_fighters;
	bool m_rowers;
	
	/// 0 - has no bird
	/// 1 - plus bird
	/// 2 - minus bird
	char m_bird;
	
public:
	
	NavigationCard();
	virtual ~NavigationCard() override = default;
	
	const std::vector<character_e> & getOverboard() const;
	const std::vector<character_e> & getThirsty() const;
	
	bool isFighters() const;
	bool isRowers() const;
	
	bool plusBird() const;
	bool loseBirds() const;
	
protected:
	
	bool onLoad(const rapidjson::Value & doc) override;
	
};

#endif // CARD_H
