#ifndef ROWINGCONTROLLER_H
#define ROWINGCONTROLLER_H

#include "LifeboatAPI_global.h"

#include <memory>
#include <vector>
#include <functional>

class Character;
class NavigationCard;

using CharacterPtr = std::shared_ptr<Character>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;

using Sender = std::function<std::vector<size_t>(const std::vector<NavigationCardPtr> &,
													const CharacterPtr &)>;
using Query = std::function<bool(const CharacterPtr &)>;

class LIFEBOAT_API RowingController {
	
	CharacterPtr m_character;
	std::vector<CharacterPtr> m_rowers;
	std::vector<NavigationCardPtr> m_navigationCards;
	std::vector<NavigationCardPtr> m_currentCards;
	
	Sender m_cardSender;
	Query m_usingGunQuery;
	
public:
	
	RowingController(const std::vector<NavigationCardPtr> & navCards);
	~RowingController() = default;
	
	void setCardSender(const Sender & sender);
	void setUsingGunQuery(const Query & query);
	
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	const std::vector<NavigationCardPtr> & getCurrentNavCards() const;
	
	const std::vector<CharacterPtr> & getRowers() const;
	
	void setCharacter(const CharacterPtr & character);
	
	void startRawing();
	
private:
	
	std::vector<NavigationCardPtr> getCurrentCards();
	
	void sendCards(const std::vector<NavigationCardPtr> & cards,
				   const CharacterPtr & to,
				   const std::function<void(const std::vector<size_t> &)> & callback);
	
	void sendUsingGunQuery(const CharacterPtr & sendTo,
						   const std::function<void(bool)> & callback);
	
};

#endif // ROWINGCONTROLLER_H
