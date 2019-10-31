#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

class ActionController;
class IterationTotalController;
class CardDistributionController;

using ActionControllerPtr = std::shared_ptr<ActionController>;
using IterationTotalControllerPtr = std::shared_ptr<IterationTotalController>;
using CardDistributionControllerPtr = std::shared_ptr<CardDistributionController>;

enum class phase_e : char {
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
	
	phase_e m_currentPhase;
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
	ITC_CardCallbackPtr getITC_CardCallback() const;
	ITC_UsingCardCallbackPtr getITC_UsingCardCallback() const;
	ITC_WaterCallbackPtr getITC_WaterCallback() const;
	ITC_PreserverCallbackPtr getITC_PreserverCallback() const;
	
private:
	
	void nextPhase();
	
	bool initDistributionController(const std::vector<ProvisionCardPtr> & provCards);
	
	bool initActionController(const std::vector<CharacterPtr> & characters,
							  const std::vector<NavigationCardPtr> & navCards);
	
	bool initIterationTotalController();
	
};

#endif // LOGICCONTROLLER_H
