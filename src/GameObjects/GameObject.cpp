#include "GameObjects/GameObject.h"

#include <fstream>

GameObject::GameObject(obj_t type) {
	static size_t id = 0;
	
	m_type = type;
	m_id = id++;
}

bool GameObject::load(const rapidjson::Value & doc) {
	if (!doc.IsObject()) {
		return false;
	}
	
	return onLoad(doc);
}
