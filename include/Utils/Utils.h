#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <memory>
#include <vector>

//GameObjects
class Character;
class ProvisionCard;
class NavigationCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;
using NavigationCardPtr = std::shared_ptr<NavigationCard>;

static CharacterPtr c_nullptr = nullptr;
static ProvisionCardPtr p_nullptr = nullptr;
static NavigationCardPtr n_nullptr = nullptr;

template<typename ret, typename ... args> 
class Callback;

struct ActionData;

using AC_ActionCallback = Callback<ActionData, const CharacterPtr&>;
using AC_FightCallback = Callback<bool, const CharacterPtr&, const CharacterPtr&>;
using DC_Callback = Callback<long, const CharacterPtr&, std::vector<ProvisionCardPtr>>;
using FC_Callback = Callback<int, const CharacterPtr&, const CharacterPtr&, const CharacterPtr&>;
using RC_CardCallback = Callback<std::vector<size_t>,
								 const CharacterPtr &,
								 const std::vector<NavigationCardPtr> &>;
using RC_GunCallback = Callback<bool, const CharacterPtr&>;
using ITC_CardCallback = Callback<size_t, const CharacterPtr&>;
using ITC_UsingCardCallback = Callback<ActionData, const CharacterPtr&>;
using ITC_WaterCallback = Callback<CharacterPtr, const CharacterPtr&>;
using ITC_PreserverCallback = Callback<CharacterPtr, const CharacterPtr&>;

using AC_ActionCallbackPtr = std::shared_ptr<AC_ActionCallback>;
using AC_FightCallbackPtr = std::shared_ptr<AC_FightCallback>;
using DC_CallbackPtr = std::shared_ptr<DC_Callback>;
using FC_CallbackPtr = std::shared_ptr<FC_Callback>;
using RC_CardCallbackPtr = std::shared_ptr<RC_CardCallback>;
using RC_GunCallbackPtr = std::shared_ptr<RC_GunCallback>;
using ITC_CardCallbackPtr = std::shared_ptr<ITC_CardCallback>;
using ITC_UsingCardCallbackPtr = std::shared_ptr<ITC_UsingCardCallback>;
using ITC_WaterCallbackPtr = std::shared_ptr<ITC_WaterCallback>;
using ITC_PreserverCallbackPtr = std::shared_ptr<ITC_PreserverCallback>;

enum class character_e : char;

enum class action_e : char {
	Unknown = -1,
	Robbing = 0,
	Swappin = 1,
	Rowing = 2,
	CardUsing = 3
};

enum class phase_e : char;

struct ActionData {
	
	CharacterPtr object;
	action_e action_type;
	size_t card_index;
	
	bool hand_card;
	
	ActionData() {
		clear();
	}
	
	void clear() {
		action_type = action_e::Unknown;
		object = nullptr;
		card_index = 0;
		hand_card = false;
	}
};

#endif
