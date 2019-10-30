#ifndef MOVINGCONTROLLER_H
#define MOVINGCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"

#include <functional>
#include <memory>
#include <vector>

class Character;
class NavigationCard;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

class FightController;
class RowingController;

struct ActionData {
	int actionType;
	CharacterPtr object;
	size_t cardIndex;
	bool hand;
	
	ActionData() {
		clear();
	}
	
	void clear() {
		actionType = 0;
		object = nullptr;
		cardIndex = 0;
		hand = false;
	}
};

using FightControllerPtr = std::shared_ptr<FightController>;
using RowingControllerPtr = std::shared_ptr<RowingController>;

using AC_ActionCallback = Callback<ActionData, const CharacterPtr&>;
using AC_FightCallback = Callback<bool, const CharacterPtr&, const CharacterPtr&>;
using FC_Callback = Callback<int, const CharacterPtr&, const CharacterPtr&, const CharacterPtr&>;
using RC_CardCallback = Callback<std::vector<size_t>,
								 const CharacterPtr &,
								 const std::vector<NavigationCardPtr> &>;
using RC_GunCallback = Callback<bool, const CharacterPtr&>;

using AC_ActionCallbackPtr = std::shared_ptr<AC_ActionCallback>;
using AC_FightCallbackPtr = std::shared_ptr<AC_FightCallback>;
using FC_CallbackPtr = std::shared_ptr<FC_Callback>;
using RC_CardCallbackPtr = std::shared_ptr<RC_CardCallback>;
using RC_GunCallbackPtr = std::shared_ptr<RC_GunCallback>;

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
