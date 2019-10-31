#ifndef FIGHTCONTROLLER_H
#define FIGHTCONTROLLER_H

#include "LifeboatAPI_global.h"

#include "Utils/Callback.h"
#include "Utils/Utils.h"

using FC_Callback = Callback<int, const CharacterPtr&, const CharacterPtr&, const CharacterPtr&>;
using FC_CallbackPtr = std::shared_ptr<FC_Callback>;

class LIFEBOAT_API FightController {
	
	CharacterPtr m_winner;
	
	std::vector<CharacterPtr> m_aTeam;
	std::vector<CharacterPtr> m_bTeam;
	
	std::vector<CharacterPtr> m_characters;
	std::vector<CharacterPtr> m_fighters;
	
	FC_CallbackPtr m_callback;
	
public:
	
	FightController(const std::vector<CharacterPtr> & characters);
	~FightController() = default;
	
	void startFight(const CharacterPtr & subject,
					const CharacterPtr & object);
	
	void setCharacters(const std::vector<CharacterPtr> & characters);
	
	const std::vector<CharacterPtr> & getFighters() const;
	const CharacterPtr & getWinner() const;
	
	FC_CallbackPtr getCallback() const;
	
	void clear();
	
private:
	
	int getTeamTotal(const std::vector<CharacterPtr> & team);
	void fightTotal();
};

#endif // FIGHTCONTROLLER_H
