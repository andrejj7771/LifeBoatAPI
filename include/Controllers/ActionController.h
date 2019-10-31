#ifndef MOVINGCONTROLLER_H
#define MOVINGCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

class FightController;
class RowingController;

using FightControllerPtr = std::shared_ptr<FightController>;
using RowingControllerPtr = std::shared_ptr<RowingController>;


class LIFEBOAT_API ActionController {
	
	FightControllerPtr m_fightController;
	RowingControllerPtr m_rowingCotroller;
	
	std::vector<CharacterPtr> m_characters;
	std::vector<CharacterPtr> m_newSequence;
	std::vector<NavigationCardPtr> m_navigationCards;
	
	AC_ActionCallbackPtr m_actionCallback;
	AC_FightCallbackPtr m_fightCallback;
	
public:
	
	ActionController(const std::vector<CharacterPtr> & characters,
					 const std::vector<NavigationCardPtr> & navCards);
	
	~ActionController() = default;
	
	const std::vector<CharacterPtr> & getCheracters() const;
	const std::vector<CharacterPtr> & getFighters() const;
	const std::vector<CharacterPtr> & getRowers() const;
	const std::vector<NavigationCardPtr> & getCurrentNavCards() const;
	
	void setNavigationCards(const std::vector<NavigationCardPtr> & navigationCards);
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	
	AC_ActionCallbackPtr getAC_ActionCallback() const;
	AC_FightCallbackPtr getAC_FightCallback() const;
	RC_CardCallbackPtr getRC_CardCallback() const;
	RC_GunCallbackPtr getRC_GunCallback() const;
	FC_CallbackPtr getFC_Callback() const;
	
	bool init();
	
	void execute();
	
	void RobCharacter(const CharacterPtr & subject,
					  const CharacterPtr & object,
					  size_t cardIndex, bool hand);
	
	void SwapPlaces(const CharacterPtr & subject,
					const CharacterPtr & object);
	
	void Rowing(const CharacterPtr & subject);
	
	void UseCard(const CharacterPtr & subject,
				 const CharacterPtr & object,
				 size_t cardIndex, bool & end);
	
private:
	
	bool initFightController();
	bool initRowingController();
	
	void callback(ActionData & data, std::vector<CharacterPtr>::iterator & current);
};

#endif // MOVINGCONTROLLER_H
