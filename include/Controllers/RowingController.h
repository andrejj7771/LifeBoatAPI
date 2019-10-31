#ifndef ROWINGCONTROLLER_H
#define ROWINGCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

class LIFEBOAT_API RowingController {
	
	CharacterPtr m_character;
	std::vector<CharacterPtr> m_rowers;
	std::vector<NavigationCardPtr> m_navigationCards;
	std::vector<NavigationCardPtr> m_currentCards;
	
	RC_CardCallbackPtr m_cardCallback;
	RC_GunCallbackPtr m_gunCallback;
	
public:
	
	RowingController(const std::vector<NavigationCardPtr> & navCards);
	~RowingController() = default;
	
	RC_CardCallbackPtr getRC_CardCallback() const;
	RC_GunCallbackPtr getRC_GunCallback() const;
	
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	const std::vector<NavigationCardPtr> & getCurrentNavCards() const;
	
	const std::vector<CharacterPtr> & getRowers() const;
	
	void setCharacter(const CharacterPtr & character);
	
	void startRawing();
	
private:
	
	std::vector<NavigationCardPtr> getCurrentCards();
	
};

#endif // ROWINGCONTROLLER_H
