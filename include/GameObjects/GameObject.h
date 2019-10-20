#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "LifeboatAPI_global.h"

#include <string>
#include <memory>

#include <rapidjson/document.h>

enum class obj_t : int {
	BaseObject = 0,
	Character = 1,
	NavigationCard = 2,
	ProvisionCard = 3
};

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

class LIFEBOAT_API GameObject {
	
	size_t m_id;
	obj_t m_type;
	
public:
	
	explicit GameObject(obj_t type);
	virtual ~GameObject() = default;
	
	inline size_t getID() const {
		return m_id;
	}
	
	inline obj_t getType() const {
		return m_type;
	}
	
	bool load(const rapidjson::Value & doc);
	
	template<typename type>
	inline std::shared_ptr<type> as() const {
		std::shared_ptr<type> cast_ptr = nullptr;
		try {
			cast_ptr =  std::dynamic_pointer_cast<type>(this);
		} catch (const std::bad_cast & error) {
			printf("%s -> %s", __FUNCTION__, error.what());
			return nullptr;
		}
		
		return cast_ptr;
	}
	
	template<typename type>
	inline bool is() const {
		std::shared_ptr<type> cast_ptr = nullptr;
		try {
			cast_ptr =  std::dynamic_pointer_cast<type>(this);
		} catch (const std::bad_cast & error) {
			printf("%s -> %s", __FUNCTION__, error.what());
			return false;
		}
		
		return cast_ptr != nullptr;
	}
	
	inline bool is(obj_t type) const {
		return m_type == type;
	}
	
protected:
	
	virtual bool onLoad(const rapidjson::Value & doc) = 0;
	
};

#endif // GAMEOBJECT_H
