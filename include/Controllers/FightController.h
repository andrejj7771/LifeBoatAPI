#ifndef FIGHTCONTROLLER_H
#define FIGHTCONTROLLER_H

#include "LifeboatAPI_global.h"

#include <functional>
#include <vector>
#include <memory>

class Character;
using CharacterPtr = std::shared_ptr<Character>;

using FightQuery = std::function<int(const CharacterPtr &,
										const CharacterPtr &,
										const CharacterPtr &)>;

class LIFEBOAT_API FightController {
	
	CharacterPtr m_winner;
	
	std::vector<CharacterPtr> m_aTeam;
	std::vector<CharacterPtr> m_bTeam;
	
	std::vector<CharacterPtr> m_characters;
	std::vector<CharacterPtr> m_fighters;
	
	FightQuery m_fightQuery;
	
public:
	
	FightController(const std::vector<CharacterPtr> & characters);
	~FightController() = default;
	
	void startFight(const CharacterPtr & subject,
					const CharacterPtr & object);
	
	void setCharacters(const std::vector<CharacterPtr> & characters);
	
	const std::vector<CharacterPtr> & getFighters() const;
	const CharacterPtr & getWinner() const;
	
	void setFightQuery(const FightQuery & query);
	
	void clear();
	
private:
	
	void sendFightQuery(const CharacterPtr & to,
						const CharacterPtr & subject,
						const CharacterPtr & object,
						const std::function<void(int, const CharacterPtr &)> & callback);
	
	int getTeamTotal(const std::vector<CharacterPtr> & team);
	void fightTotal();
};

#endif // FIGHTCONTROLLER_H
