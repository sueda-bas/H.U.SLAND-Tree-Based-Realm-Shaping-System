#include "GameWorld.h"

#include <complex>

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;

    if(realmShaper == nullptr || isle == nullptr) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return hasAccess;
    }

    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalMapDepth = mapTree.getDepth();
    int totalShaperTreeHeight = shaperTree.getDepth();
    int minAccessDepth = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);
    int isleDepth = mapTree.getIsleDepth(isle);

    if (isleDepth >= minAccessDepth) {
        hasAccess = true;
    } else {
        hasAccess = false;    }

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation

    if (realmShaper == nullptr) {
        return;
    }
    isle = mapTree.findIsle(*isle);
    if(isle == nullptr) {
        return;
    }
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << isle->getName() << "." << std::endl;
        return;
    }
    isle->addShaper(realmShaper);
    if (isle->increaseShaperCount()) {
        std::cout << "[Overcrowding] " << isle->getName() << " self-destructed, it will be removed from the map." << std::endl;
        mapTree.remove(isle);
        return;
    }
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << "." << std::endl;
    realmShaper->collectItem(isle->getItem());
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << "." << std::endl;
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    if (shaper == nullptr) {
        return;
    }

    if (shaper->hasEnoughEnergy()) {
        shaper->loseEnergy();
        Isle *newIsle = new Isle(isleName);
        mapTree.insert(newIsle);

        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}



/*void GameWorld::displayGameState()
{
    mapTree.displayMap();
    shaperTree.displayTree();
}*/

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open()) {
        std::cerr << "[Error] Could not open access log file: " << accessLogs << std::endl;
        return;
    }
    if (!duelFile.is_open()) {
        std::cerr << "[Error] Could not open duel log file: " << duelLogs << std::endl;
        return;
    }

    std::string line;
    int accessCount = 0;

    // İşlem: Access loglarını işleme
    while (std::getline(accessFile, line)) {
        std::istringstream ss(line);
        std::string shaperName, isleName;
        ss >> shaperName >> isleName;

        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);

        if (shaper == nullptr) {
            continue;
        }
        if (isle == nullptr) {
            if (shaper->hasEnoughEnergy()) {
                craft(shaper, isleName);
                Isle *newIsle = mapTree.findIsle(isleName);
                if (newIsle != nullptr) {
                    exploreArea(shaper, newIsle);
                }
            } else {
                std::cerr << "[Error] Isle " << isleName << " not found and " << shaper->getName() << " does not have enough energy to craft it." << std::endl;
            }
        } else {
            exploreArea(shaper, isle);
        }

        accessCount ++;




        if (accessCount % 5 == 0 && std::getline(duelFile, line)) {
            std::istringstream duelSS(line);
            std::string shaper1Name;
            int result;
            duelSS >> shaper1Name >> result;
            bool iswinner = false;

            RealmShaper *shaper1 = shaperTree.findPlayer(shaper1Name);
            if(shaper1 != nullptr) {
                iswinner = result == 1;
                RealmShaper duelResult = shaperTree.duel(shaper1, iswinner);
            }else {
                std::cerr << "[Error] Could not find player " << shaper1Name << std::endl;
            }
        }
    }

    //displayGameState();
    accessFile.close();
    duelFile.close();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}