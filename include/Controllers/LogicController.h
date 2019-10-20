#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include "LifeboatAPI_global.h"

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
	
	void setDistributionCardSender(const std::function<long(const CharacterPtr &, const std::vector<ProvisionCardPtr> &)> & sender);
	void setRowingCardSender(const std::function<std::vector<size_t>( const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender);
	void setIterTotalCardSender(const std::function<size_t(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender);
	
	void setActionQuery(const std::function<void(const CharacterPtr &, ActionData & data)> & query);
	void setFightQuery(const std::function<bool(const CharacterPtr &, const CharacterPtr &)> & query);
	void setFightControllerFightQuery(const std::function<int(const CharacterPtr &, const CharacterPtr &, const CharacterPtr &)> & query);
	void setRowingUsingGunQuery(const std::function<bool(const CharacterPtr &)> & query);
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
