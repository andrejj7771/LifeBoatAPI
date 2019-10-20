#include "GameObjects/NavigationCard.h"

NavigationCard::NavigationCard() : GameObject (obj_t::NavigationCard)
{
	m_fighters = false;
	m_rowers = false;
	m_bird = false;
}

const std::vector<character_t> & NavigationCard::getOverboard() const {
	return m_overboard;
}

const std::vector<character_t> & NavigationCard::getThirsty() const {
	return m_thirsty;
}

bool NavigationCard::isFighters() const {
	return m_fighters;
}

bool NavigationCard::isRowers() const {
	return m_rowers;
}

bool NavigationCard::plusBird() const {
	return m_bird == 1;
}

bool NavigationCard::loseBirds() const {
	return m_bird == 2;
}

bool NavigationCard::onLoad(const rapidjson::Value & doc) {
	auto overboard = doc["overboard"].GetArray();
	auto thirsty = doc["thirsty"].GetArray();
	
	for (unsigned i = 0; i < overboard.Size(); ++i) {
		auto type = overboard[i].GetInt();
		m_overboard.push_back(static_cast<character_t>(type));
	}
	
	for (unsigned i = 0; i < thirsty.Size(); ++i) {
		auto type = thirsty[i].GetInt();
		m_thirsty.push_back(static_cast<character_t>(type));
	}
	
	m_bird = static_cast<char>(doc["bird"].GetInt());
	m_fighters = doc["fighters"].GetBool();
	m_rowers = doc["rowers"].GetBool();
	
	return true;
}
