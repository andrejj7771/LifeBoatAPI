#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"

#include <map>
#include <vector>

class ProvisionCard;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

class Character;
using CharacterPtr = std::shared_ptr<Character>;

const static ProvisionCardPtr NullProvision = nullptr;


enum class character_e : char {
	Unknown = 0,
	Lauren = 1,
	Stephen = 2,
	Captain = 3,
	FirstMate = 4,
	Frenchy = 5,
	Kid = 6
};

static std::map<character_e, std::string> characterNames =
{
	{	character_e::Lauren, "Lady Lauren"		},
	{	character_e::Stephen, "Sir Stephen"		},
	{	character_e::Captain, "The Captain"		},
	{	character_e::FirstMate, "First Mate"	},
	{	character_e::Frenchy, "Frenchy"			},
	{	character_e::Kid, "The Kid"				}
};

class LIFEBOAT_API Character : public GameObject {
	//strength and health
	int m_size;
	//score for surviving
	int m_survivalValue;
	//additional strength. useful during a fight
	int m_bonusSize;
	
	int m_waterHit;
	int m_criticalHit;
	
	bool m_isUnconscious;
	bool m_isDead;
	
	std::vector<ProvisionCardPtr> m_table;
	std::vector<ProvisionCardPtr> m_hand;
	
	CharacterPtr m_enemy;
	CharacterPtr m_friend;
	
	character_e m_cType;
	
	std::string m_name;
	
public:
	
	Character();
	~Character() override = default;
	
	void setName(const std::string & name);
	const std::string & getName() const;
	
	int getSize() const;
	int getSurvivalValue() const;
	
	void setBonusSize(int size);
	int getBonusSize() const;
	int getTotalSize() const;
	
	void incWaterHit();
	void decWaterHit();
	void setWaterHit(int value);
	int getWaterHit() const;
	
	void incCriticalHit();
	void decCriticalHit();
	void setCriticalHit(int value);
	int getCriticalHit() const;
	
	bool isUnconscious() const;
	bool isDead() const;
	
	void appendCardToTable(const ProvisionCardPtr & card);
	void removeCardFromTable(const ProvisionCardPtr & card);
	const ProvisionCardPtr & getTableCard(size_t index) const;
	size_t getNumTableCards() const;
	
	void appendCardToHand(const ProvisionCardPtr & card);
	void removeCardFromHand(const ProvisionCardPtr & card);
	const ProvisionCardPtr & getHandCard(size_t index) const;
	size_t getNumHandCards() const;
	
	void setEnemy(CharacterPtr character);
	CharacterPtr getEnemy() const;
	
	void setFriend(CharacterPtr character);
	CharacterPtr getFriend() const;
	
	character_e getCharacterType() const;
	
	void looseCards();
	
protected:
	
	bool onLoad(const rapidjson::Value & doc) override {
		m_cType = static_cast<character_e>(doc["type"].GetInt());
		m_name = characterNames[m_cType];
		m_size = doc["size"].GetInt();
		m_survivalValue = doc["survival"].GetInt();
		return true;
	}
	
};

#endif // CHARACTER_H
