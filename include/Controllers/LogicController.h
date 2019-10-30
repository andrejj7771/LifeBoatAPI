#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"

#include <functional>
#include <memory>
#include <vector>

class Character;
class NavigationCard;
class ProvisionCard;

class ActionController;
class IterationTotalController;
class CardDistributionController;

struct ActionData;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;

using ActionControllerPtr = std::shared_ptr<ActionController>;
using IterationTotalControllerPtr = std::shared_ptr<IterationTotalController>;
using CardDistributionControllerPtr = std::shared_ptr<CardDistributionController>;

using AC_ActionCallback = Callback<ActionData, const CharacterPtr&>;
using AC_FightCallback = Callback<bool, const CharacterPtr&, const CharacterPtr&>;
using DC_Callback = Callback<long, const CharacterPtr&, std::vector<ProvisionCardPtr>>;
using FC_Callback = Callback<int, const CharacterPtr&, const CharacterPtr&, const CharacterPtr&>;
using RC_CardCallback = Callback<std::vector<size_t>,
								 const CharacterPtr &,
								 const std::vector<NavigationCardPtr> &>;
using RC_GunCallback = Callback<bool, const CharacterPtr&>;

using AC_ActionCallbackPtr = std::shared_ptr<AC_ActionCallback>;
using AC_FightCallbackPtr = std::shared_ptr<AC_FightCallback>;
using DC_CallbackPtr = std::shared_ptr<DC_Callback>;
using FC_CallbackPtr = std::shared_ptr<FC_Callback>;
using RC_CardCallbackPtr = std::shared_ptr<RC_CardCallback>;
using RC_GunCallbackPtr = std::shared_ptr<RC_GunCallback>;

enum class totalPhase : int;

enum class phase_t : int {
	Initialization = -1,
	CardDistributionPhase = 0,
	MovingPhase = 1,
	TotalPhase = 2
};

class LIFEBOAT_API LogicController {
	
	std::vector<CharacterPtr> m_characters;
	std::vector<NavigationCardPtr> m_navigationCards;
	std::vector<ProvisionCardPtr> m_provisionCards;
	
	CardDistributionControllerPtr m_distributionController;
	ActionControllerPtr m_actionController;
	IterationTotalControllerPtr m_iterationTotalController;
	
	phase_t m_currentPhase;
	size_t m_birdsCounter;
	
public:
	
	explicit LogicController(const std::vector<CharacterPtr> & characters,
							 const std::vector<NavigationCardPtr> & navCards,
							 const std::vector<ProvisionCardPtr> & provCards);
	
	~LogicController() = default;
	
	bool init();
	void run();
	
	const std::vector<CharacterPtr> & getCharacters() const;
	
	
	AC_ActionCallbackPtr getAC_ActionCallback() const;
	AC_FightCallbackPtr getAC_FightCallback() const;
	DC_CallbackPtr getDC_Callback() const;
	FC_CallbackPtr getFC_Callback() const;
	RC_CardCallbackPtr getRC_CardCallback() const;
	RC_GunCallbackPtr getRC_GunCallback() const;
	void setIterTotalCardSender(const std::function<size_t(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender);
	void setIterTotalUsingCardQuery(const std::function<ProvisionCardPtr(const CharacterPtr &, totalPhase)> & query);
	void setIterTotalHealQuery(const std::function<CharacterPtr(const CharacterPtr &)> & query);
	
private:
	
	void nextPhase();
	
	bool initDistributionController(const std::vector<ProvisionCardPtr> & provCards);
	
	bool initActionController(const std::vector<CharacterPtr> & characters,
							  const std::vector<NavigationCardPtr> & navCards);
	
	bool initIterationTotalController();
	
};

#endif // LOGICCONTROLLER_H
