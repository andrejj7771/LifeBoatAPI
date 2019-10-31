#include "GameObjects/ProvisionCard.h"

ProvisionCard::ProvisionCard() : GameObject(obj_e::ProvisionCard) {}

provision_e ProvisionCard::getCardType() const {
	return m_cType;
}

const std::string & ProvisionCard::getName() const {
	return m_name;
}

bool ProvisionCard::onLoad(const rapidjson::Value & doc) {
	m_name = doc["name"].GetString();
	m_cType = static_cast<provision_e>(doc["type"].GetInt());
	return true;
}
