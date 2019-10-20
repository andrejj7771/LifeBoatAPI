#include "Controllers/StartController.h"

#include "GameObjects/Character.h"
#include "GameObjects/NavigationCard.h"
#include "GameObjects/ProvisionCard.h"

#include <fstream>
#include <sstream>

StartController::StartController(int numPlayers) {
	m_numPlayers = numPlayers;
}

bool StartController::init() {
	if (initCharacters() == false)
		return false;
	
	if (initNavigationCards() == false)
		return false;
	
	if (initProvisionCards()  == false)
		return false;
	
	return true;
}

const std::vector<CharacterPtr> & StartController::getCharacters() const {
	return m_characters;
}

const std::vector<NavigationCardPtr> & StartController::getNavigationCards() const {
	return m_navigationCards;
}

const std::vector<ProvisionCardPtr> & StartController::getProvisionCards() const {
	return m_provisionCards;
}

bool StartController::initCharacters() {
	if (!loadCharacters(m_characters)) {
		printf("%s -> Error: can't load characters\n", __FUNCTION__);
		return false;
	}
	
	std::random_device rd;
	std::mt19937 g(rd());
	
	std::vector<size_t> f;
	std::vector<size_t> e;
	for (size_t i = 0; i < m_characters.size(); ++i) {
		f.push_back(i);
		e.push_back(i);
	}
	
	for (auto c : m_characters) {
		std::uniform_int_distribution<size_t> uni(0, f.size() - 1);
		
		size_t randomValue = uni(g);
		c->setEnemy(m_characters.at(e.at(randomValue)));
		e.erase(e.begin() + static_cast<long>(randomValue));
		
		randomValue = uni(g);
		c->setFriend(m_characters.at(f.at(randomValue)));
		f.erase(f.begin() + static_cast<long>(randomValue));
	}
	
	return true;
}

bool StartController::initNavigationCards() {
	if (!loadNavigationCards(m_navigationCards)) {
		printf("%s -> Error: can't load navigation cards\n", __FUNCTION__);
		return false;
	}
	
	shuffle<NavigationCardPtr>(m_navigationCards);
	return true;
}

bool StartController::initProvisionCards() {
	if (!loadProvisionCards(m_provisionCards)) {
		printf("%s -> Error: can't load navigation cards\n", __FUNCTION__);
		return false;
	}
	
	shuffle<ProvisionCardPtr>(m_provisionCards);
	return true;
}

bool StartController::loadCharacters(std::vector<CharacterPtr> & characters) {
	if (!loadFromFile<Character>(characters, "characters"))
		return false;
	
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<size_t> uni(0, m_characters.size() - 1);
	
	for (size_t i = 0; i <= m_characters.size() - size_t(m_numPlayers); ++i) {
		size_t randomValue = uni(g);
		m_characters.erase(m_characters.begin() + unsigned(randomValue));
	}
	
	return true;
}

bool StartController::loadNavigationCards(std::vector<NavigationCardPtr> & navigationCards) {
	return loadFromFile<NavigationCard>(navigationCards, "navigation");
}

bool StartController::loadProvisionCards(std::vector<ProvisionCardPtr> & provisionCards) {
	return loadFromFile<ProvisionCard>(provisionCards, "provision");
}

std::string StartController::readFile(const std::string & path) {
	if (path.empty())
		return path;
	
	std::fstream fs(path);
	if (!fs.is_open())
		return path;
	
	std::ostringstream buffer;
	buffer << fs.rdbuf();
	
	return buffer.str();
}
