#include "Controllers/LogicController.h"
#include "Controllers/ActionController.h"
#include "Controllers/IterationTotalController.h"
#include "Controllers/CardDistributionController.h"

LogicController::LogicController(const std::vector<CharacterPtr> & characters,
								 const std::vector<NavigationCardPtr> & navCards,
								 const std::vector<ProvisionCardPtr> & provCards) :
	m_characters(characters),
	m_navigationCards(navCards),
	m_provisionCards(provCards),
	m_distributionController(nullptr),
	m_actionController(nullptr),
	m_iterationTotalController(nullptr)
{
	m_currentPhase = phase_t::Initialization;
}

bool LogicController::init() {
	if (m_characters.empty() ||
			m_provisionCards.empty() ||
			m_navigationCards.empty()) {
		return false;
	}
	
	if (!initDistributionController(m_provisionCards)) {
		return false;
	}
	
	if (!initActionController(m_characters,
							  m_navigationCards)) {
		return false;
	}
	
	if (!initIterationTotalController()) {
		return false;
	}
	
	m_currentPhase = phase_t::CardDistributionPhase;
	
	return true;
}

void LogicController::nextPhase() {
	switch (m_currentPhase) {
		case phase_t::CardDistributionPhase: {
			m_distributionController->setChracters(m_characters);
			m_distributionController->execute();
			
			m_provisionCards = m_distributionController->getProvisionCards();
			m_currentPhase = phase_t::MovingPhase;
			break;
		}
		case phase_t::MovingPhase: {
			m_actionController->setNavigationCards(m_navigationCards);
			m_actionController->execute();
			
			m_characters = m_actionController->getCheracters();
			m_navigationCards = m_actionController->getNavigationCards();
			m_currentPhase = phase_t::TotalPhase;
			break;
		}
		case phase_t::TotalPhase: {
			m_iterationTotalController->setCharacters(m_characters);
			m_iterationTotalController->setNavigationCards(m_navigationCards);
			m_iterationTotalController->setFighters(m_actionController->getFighters());
			m_iterationTotalController->setRowers(m_actionController->getRowers());
			m_iterationTotalController->setCurrentNavCards(m_actionController->getCurrentNavCards());
			m_iterationTotalController->execute();
			
			m_navigationCards = m_iterationTotalController->getNavigationCards();
			m_currentPhase = phase_t::CardDistributionPhase;
			break;
		}
		default:
			return;
	}
}

const std::vector<CharacterPtr> & LogicController::getCharacters() const {
	return m_characters;
}

void LogicController::setDistributionCardSender(const std::function<long(const CharacterPtr &, const std::vector<ProvisionCardPtr> &)> & sender) {
	if (m_distributionController == nullptr)
		return;
	
	m_distributionController->setSender(sender);
}

void LogicController::setRowingCardSender(const std::function<std::vector<size_t>(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender) {
	m_actionController->setRowingCardSender(sender);
}

void LogicController::setIterTotalCardSender(const std::function<size_t(const CharacterPtr &, const std::vector<NavigationCardPtr> &)> & sender) {
	m_iterationTotalController->setCardSender(sender);
}

void LogicController::setActionQuery(const std::function<void(const CharacterPtr &, ActionData & data)> & query) {
	m_actionController->setActionQuery(query);
}

void LogicController::setFightQuery(const std::function<bool(const CharacterPtr &, const CharacterPtr &)> & query) {
	m_actionController->setFightQuery(query);
}

void LogicController::setFightControllerFightQuery(const std::function<int(const CharacterPtr &, const CharacterPtr &, const CharacterPtr &)> & query) {
	m_actionController->setFightingQuery(query);
}

void LogicController::setRowingUsingGunQuery(const std::function<bool(const CharacterPtr &)> & query) {
	m_actionController->setRowingUsingGunQuery(query);
}

void LogicController::setIterTotalUsingCardQuery(const std::function<ProvisionCardPtr(const CharacterPtr &, totalPhase)> & query) {
	m_iterationTotalController->setUsingCardQuery(query);
}

void LogicController::setIterTotalHealQuery(const std::function<CharacterPtr(const CharacterPtr &)> & query) {
	m_iterationTotalController->setHealQuery(query);
}

bool LogicController::initDistributionController(const std::vector<ProvisionCardPtr> & provCards) {
	try {
		m_distributionController = std::make_shared<CardDistributionController>(provCards);
	}
	catch (const std::exception & error) {
		printf("%s -> %s", __FUNCTION__, error.what());
		return false;
	}
	
	return true;
}

bool LogicController::initActionController(const std::vector<CharacterPtr> & characters,
										   const std::vector<NavigationCardPtr> & navCards)
{
	try {
		m_actionController = std::make_shared<ActionController>(characters, navCards);
		if (!m_actionController->init())
			return false;
	}
	catch (const std::exception & error) {
		printf("%s -> %s", __FUNCTION__, error.what());
		return false;
	}
	
	return true;
}

bool LogicController::initIterationTotalController() {
	try {
		m_iterationTotalController = std::make_shared<IterationTotalController>();
	}
	catch (const std::exception & error) {
		printf("%s -> %s", __FUNCTION__, error.what());
		return false;
	}
	
	return true;
}
