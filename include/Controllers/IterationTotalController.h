#ifndef ITERATIONTOTALCONTROLLER_H
#define ITERATIONTOTALCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"

#include <functional>
#include <memory>
#include <vector>

enum class character_t : int;

class Character;
class ProvisionCard;
class NavigationCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;

enum class totalPhase : int {
	Unknown = -1,
	Overboard = 0,
	Dehydration = 1
};

using ITC_CardCallback = Callback<size_t, const CharacterPtr&>;
using UsingCardQuery = std::function<ProvisionCardPtr(const CharacterPtr &, totalPhase)>;
using HealQuery = std::function<CharacterPtr(const CharacterPtr &)>;

using ITC_CardCallbackPtr = std::shared_ptr<ITC_CardCallback>;

class LIFEBOAT_API IterationTotalController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<NavigationCardPtr> m_navigationCards;
	
	std::vector<CharacterPtr> m_fighters;
	std::vector<CharacterPtr> m_rowers;
	std::vector<NavigationCardPtr> m_currentCards;
	
	std::vector<CharacterPtr> m_overboard;
	std::vector<CharacterPtr> m_dehydrated;
	
	ITC_CardCallbackPtr m_cardCallback;
	UsingCardQuery m_usingCardQuery;
	HealQuery m_healQuery;
	
	std::vector<NavigationCardPtr> m_birdCards;
	
	totalPhase m_currentPhase;
	
public:
	
	IterationTotalController();
	~IterationTotalController() = default;
	
	size_t getNumBirds() const;
	
	void setCharacters(const std::vector<CharacterPtr> & characters);
	void setNavigationCards(const std::vector<NavigationCardPtr> & navigationCards);
	
	void setFighters(const std::vector<CharacterPtr> & fighters);
	void setRowers(const std::vector<CharacterPtr> & rowers);
	void setCurrentNavCards(const std::vector<NavigationCardPtr> & cards);
	
	ITC_CardCallbackPtr getITC_CardCallback() const;
	
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	
	void execute();
	
private:
	
	void convertTypesToCharacters(const std::vector<character_t> & types,
								  std::vector<CharacterPtr> & characters);
	
	void usingCardQueryCallback(const CharacterPtr & from,
								const ProvisionCardPtr & card);
	
	void sendUsingCardQuery(const CharacterPtr & to,
							const std::function<void(const CharacterPtr &,
													 const ProvisionCardPtr &)> & usingCardQueryCallback);
	void sendQuery(const CharacterPtr & to, const std::function<void(const CharacterPtr &)> & callback);
	
};

#endif // ITERATIONTOTALCONTROLLER_H
