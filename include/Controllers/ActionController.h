#ifndef MOVINGCONTROLLER_H
#define MOVINGCONTROLLER_H

#include "LifeboatAPI_global.h"

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

using FightControllerPtr = std::shared_ptr<FightController>;
using RowingControllerPtr = std::shared_ptr<RowingController>;

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

class LIFEBOAT_API ActionController {
	
	FightControllerPtr m_fightController;
	RowingControllerPtr m_rowingCotroller;
	
	std::vector<CharacterPtr> m_characters;
	std::vector<CharacterPtr> m_newSequence;
	std::vector<NavigationCardPtr> m_navigationCards;
	
	std::function<void(const CharacterPtr &, ActionData & data)> m_actionQuery;
	std::function<bool(const CharacterPtr &, const CharacterPtr &)> m_fightQuery;
	
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
	
	void setActionQuery(const std::function<void(const CharacterPtr &, ActionData & data)>& query);
	void setFightQuery(const std::function<bool(const CharacterPtr &, const CharacterPtr &)> & query);
	
	void setRowingCardSender(const std::function<std::vector<size_t>(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender);
	void setRowingUsingGunQuery(const std::function<bool(const CharacterPtr &)> & query);
	void setFightingQuery(const std::function<int(const CharacterPtr &, const CharacterPtr &, const CharacterPtr &)> & query);
	
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
	
	void sendActionQuery(const CharacterPtr & to,
						 ActionData & data,
						 const std::function<void(ActionData &)> & callback);
	
	void sendFightQuery(const CharacterPtr & subject,
						const CharacterPtr & object,
						const std::function<void(bool)> & callback);
	
	void callback(ActionData & data, std::vector<CharacterPtr>::iterator & current);
	
};

#endif // MOVINGCONTROLLER_H
