#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "LifeboatAPI_global.h"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include <rapidjson/document.h>

const static std::string DATA_PATH = "/home/andrey/LifeBoat/data/";

class Character;
class NavigationCard;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

class LIFEBOAT_API StartController {
	
	int m_numPlayers;
	
	std::vector<CharacterPtr> m_characters;
	std::vector<NavigationCardPtr> m_navigationCards;
	std::vector<ProvisionCardPtr> m_provisionCards;
	
public:
	
	StartController(int numPlayers = 4);
	~StartController() = default;
	
	StartController(StartController & c) = delete;
	StartController & operator=(const StartController & c) = delete;
	
	bool init();
	
	const std::vector<CharacterPtr> & getCharacters() const;
	const std::vector<NavigationCardPtr> & getNavigationCards() const;
	const std::vector<ProvisionCardPtr> & getProvisionCards() const;
	
private:
	
	bool initCharacters();
	bool initNavigationCards();
	bool initProvisionCards();
	
	template<typename type>
	bool loadFromFile(std::vector<std::shared_ptr<type>> & vector,
					  const std::string & memberName) {
		const std::string & path = DATA_PATH + memberName + ".json";
		const std::string & data = readFile(path);
		if (data == path)
			return false;
		
		rapidjson::Document doc;
		doc.Parse(data.data());
		if (!doc.HasMember(memberName.data()))
			return false;
		
		auto arrayJson = doc[memberName.data()].GetArray();
		for (unsigned i = 0; i < arrayJson.Size(); ++i) {
			auto item = arrayJson[i].GetObject();
			auto object = std::make_shared<type>();
			object->load(item);
			vector.push_back(object);
		}
		
		return true;
	}
	
	bool loadCharacters(std::vector<CharacterPtr> & characters);
	bool loadNavigationCards(std::vector<NavigationCardPtr> & navigationCards);
	bool loadProvisionCards(std::vector<ProvisionCardPtr> & provisionCards);
	
	template<typename type>
	void shuffle(std::vector<type> & vector) {
		std::random_device rd;
		std::mt19937_64 g(rd());
		
		std::shuffle(vector.begin(), vector.end(), g);
	}
	
	std::string readFile(const std::string & path);
	
};

#endif // STARTCONTROLLER_H
