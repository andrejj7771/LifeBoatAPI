#ifndef TOTALCONTROLLER_H
#define TOTALCONTROLLER_H

#include <vector>
#include <memory>

class Character;
class ProvisionCard;

using CharacterPtr = std::shared_ptr<Character>;
using ProvisionCardPtr = std::shared_ptr<ProvisionCard>;

class TotalController {
	
public:
	
	TotalController() = default;
	~TotalController() = default;
	
	const CharacterPtr & start(const std::vector<CharacterPtr> & characters);
	
private:
	
	int countScore(const CharacterPtr & character);
	
};

#endif // TOTALCONTROLLER_H
