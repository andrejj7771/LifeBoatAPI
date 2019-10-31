#ifndef TOTALCONTROLLER_H
#define TOTALCONTROLLER_H

#include "Utils/Utils.h"

class TotalController {
	
public:
	
	TotalController() = default;
	~TotalController() = default;
	
	const CharacterPtr & start(const std::vector<CharacterPtr> & characters);
	
private:
	
	int countScore(const CharacterPtr & character);
	
};

#endif // TOTALCONTROLLER_H
