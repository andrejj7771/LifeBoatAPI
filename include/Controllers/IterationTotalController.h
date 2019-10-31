#ifndef ITERATIONTOTALCONTROLLER_H
#define ITERATIONTOTALCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

enum class TotalPhase : int {
	Unknown = -1,
	Overboard = 0,
	Dehydration = 1
};

class LIFEBOAT_API IterationTotalController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<NavigationCardPtr> m_navigationCards;
	
	std::vector<CharacterPtr> m_fighters;
	std::vector<CharacterPtr> m_rowers;
	std::vector<NavigationCardPtr> m_currentCards;
	
	std::vector<CharacterPtr> m_overboard;
	std::vector<CharacterPtr> m_dehydrated;
	
	ITC_CardCallbackPtr m_cardCallback;
	ITC_WaterCallbackPtr m_waterCallback;
	ITC_PreserverCallbackPtr m_preserverCallback;
	ITC_UsingCardCallbackPtr m_usingCardCallback;
	
	std::vector<NavigationCardPtr> m_birdCards;
	
	TotalPhase m_currentPhase;
	
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
	ITC_WaterCallbackPtr getITC_WaterCallback() const;
	ITC_PreserverCallbackPtr getITC_PreserverCallback() const;
	ITC_UsingCardCallbackPtr getITC_UsingCardCallback() const;
	
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	
	void execute();
	
private:
	
	void convertTypesToCharacters(const std::vector<character_e> & types,
								  std::vector<CharacterPtr> & characters);
	
	void usingCardQueryCallback(const ActionData & data);
	
};

#endif // ITERATIONTOTALCONTROLLER_H
