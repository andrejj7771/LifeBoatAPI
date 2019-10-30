#ifndef ROWINGCONTROLLER_H
#define ROWINGCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"

#include <memory>
#include <vector>
#include <functional>

class Character;
class NavigationCard;

using CharacterPtr = std::shared_ptr<Character>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;

using RC_CardCallback = Callback<std::vector<size_t>,
								 const CharacterPtr &,
								 const std::vector<NavigationCardPtr> &>;
using RC_GunCallback = Callback<bool, const CharacterPtr&>;

using RC_CardCallbackPtr = std::shared_ptr<RC_CardCallback>;
using RC_GunCallbackPtr = std::shared_ptr<RC_GunCallback>;

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
	
	RC_CardCallbackPtr getCardCallback() const;
	RC_GunCallbackPtr getGunCallback() const;
	
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	const std::vector<NavigationCardPtr> & getCurrentNavCards() const;
	
	const std::vector<CharacterPtr> & getRowers() const;
	
	void setCharacter(const CharacterPtr & character);
	
	void startRawing();
	
private:
	
	std::vector<NavigationCardPtr> getCurrentCards();
	
};

#endif // ROWINGCONTROLLER_H
