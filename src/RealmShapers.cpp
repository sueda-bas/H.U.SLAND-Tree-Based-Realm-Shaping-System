#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (RealmShaper* shaper : realmShapers)
    {
        if (shaper != nullptr)
        {
            delete shaper;
        }
    }

    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    realmShapers.resize(shapers.size());

    for (int i = 0; i < shapers.size(); ++i)
    {
        realmShapers[i] = shapers[i];
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    int counter = 0;
    for (auto shaper : realmShapers)
    {
        if (shaper != nullptr)
        {
            counter++;
        }
    }
    return counter;
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (index >= 0 && index < realmShapers.size())
    {
        if (realmShapers[index] != nullptr)
        {
            isValid = true;
        }
    }
    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    for (int i = 0; i < realmShapers.size(); ++i)
    {
        if (*realmShapers[i] == *shaper)
        {
            realmShapers.erase(realmShapers.begin() + i);
            return i;
        }
    }
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == *shaper) {
            return i;
        }
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    if (realmShapers.empty()) {
        return 0;
    }
    return static_cast<int>(std::log2(findIndex(shaper))) + 1;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    if (realmShapers.empty()) {
        return 0;
    }
    RealmShaper *realmShaper = realmShapers[0];
    int depth = getDepth(realmShaper);
    return depth;
}

RealmShaper ShaperTree::findShaper(int index) {
    if (index >= 0 && index < realmShapers.size()) {
        return *realmShapers[index];
    }
    throw std::out_of_range("Index out of range");
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{

    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
    RealmShaper *parent = getParent(challenger);
    if(parent == nullptr) {
        return *challenger;
    }

    RealmShaper *victor = nullptr;
    RealmShaper *loser = nullptr;
    if (result) {
        victor = challenger;
        loser = parent;
        replace(victor, parent);
    }else {
        victor = parent;
        loser= challenger;
    }

    victor->gainHonour();
    loser->loseHonour();

    std::cout << "[Duel] " << victor->getName() << " won the duel" << std::endl;
    std::cout << "[Honour] " << "New honour points: ";
    std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
    std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;

    if (loser->getHonour() <= 0) {
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
        remove(loser);
    }

    return *victor;
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper

    int index = findIndex(shaper);

    if (index == -1 || index == 0) {
        return nullptr;
    }

    parent = realmShapers[(index - 1)/2];
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int player_low_index = findIndex(player_low);
    int player_high_index = findIndex(player_high);

    if (player_low_index == -1 || player_high_index == -1) {
        std::cerr << "Invalid players!" << std::endl;
        return;
    }

    RealmShaper *temp = realmShapers[player_low_index];
    realmShapers[player_low_index] = realmShapers[player_high_index];
    realmShapers[player_high_index] = temp;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == shaper) {
            foundShaper = realmShapers[i];
            break;
        }
    }
    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (realmShapers[i]->getName() == name) {
            foundShaper = realmShapers[i];
            break;
        }
    }
    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    if (!isValidIndex(index)) {
        return result;
    }
    int leftIndex = 2 * index + 1;
    if (isValidIndex(leftIndex)) {
        std::vector<std::string> leftResult = inOrderTraversal(leftIndex);
        result.reserve(result.size() + leftResult.size());
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    result.push_back(realmShapers[index]->getName());

    int rightIndex = 2 * index + 2;
    if (isValidIndex(rightIndex)) {

        std::vector<std::string> rightResult = inOrderTraversal(rightIndex);
        result.reserve(result.size() + rightResult.size());
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    if (!isValidIndex(index)) {
        return result;
    }
    result.push_back(realmShapers[index]->getName());

    int leftIndex = 2 * index + 1;
    if (isValidIndex(leftIndex)) {
        std::vector<std::string> leftResult = preOrderTraversal(leftIndex);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    int rightIndex = 2 * index + 2;
    if (isValidIndex(rightIndex)) {
        std::vector<std::string> rightResult = preOrderTraversal(rightIndex);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }
    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    if (!isValidIndex(index)) {
        return result;
    }

    int leftIndex = 2 * index + 1;
    if (isValidIndex(leftIndex)) {
        std::vector<std::string> leftResult = postOrderTraversal(leftIndex);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    int rightIndex = 2 * index + 2;
    if (isValidIndex(rightIndex)) {
        std::vector<std::string> rightResult = postOrderTraversal(rightIndex);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }

    result.push_back(realmShapers[index]->getName());
    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation

    if (!isValidIndex(index)) {
        return;
    }

    outFile << realmShapers[index]->getName() << std::endl;

    int leftIndex = 2 * index + 1;
    int rightIndex = 2 * index + 2;

    preOrderTraversal(leftIndex, outFile);
    preOrderTraversal(rightIndex, outFile);

    outFile.close();

}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
    if (realmShapers.empty()) {
        return;
    }

    std::queue<RealmShaper*>realmShapersQueue;
    realmShapersQueue.push(realmShapers[0]);

    while (!realmShapersQueue.empty()) {
        RealmShaper *realmShaper = realmShapersQueue.front();
        realmShapersQueue.pop();
        outFile << realmShaper->getName() << std::endl;

        int index = findIndex(realmShaper);
        int leftIndex = 2 * index + 1;
        if (leftIndex < realmShapers.size()) {
            RealmShaper shaper = findShaper(leftIndex);
            realmShapersQueue.push(&shaper);
        }

        int rightIndex = 2 * index + 2;
        if (rightIndex < realmShapers.size()) {
            RealmShaper shaper = findShaper(rightIndex);
            realmShapersQueue.push(&shaper);
        }
    }

    outFile.close();
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    //int left = 0;  // TODO: Calculate left index
    //int right = 0; // TODO: Calculate right index

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "[Error] Failed to open file for writing!" << std::endl;
        return;
    }

    if (realmShapers.empty()) {
        outFile.close();
        return;
    }

    std::queue<RealmShaper*> shaperQueue;
    shaperQueue.push(realmShapers[0]);

    while (!shaperQueue.empty()) {
        RealmShaper *currentShaper = shaperQueue.front();
        shaperQueue.pop();

        outFile << currentShaper->getName() << std::endl;

        int index = findIndex(currentShaper);
        int leftIndex = 2 * index + 1;
        int rightIndex = 2 * index + 2;

        if (leftIndex < realmShapers.size()) {
            shaperQueue.push(realmShapers[leftIndex]);
        }

        if (rightIndex < realmShapers.size()) {
            shaperQueue.push(realmShapers[rightIndex]);
        }
    }

    outFile.close();
    std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;

}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "[Error] Failed to open file for writing!" << std::endl;
        return;
    }
    writeToFile(0, outFile);
    outFile.close();

    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}

void ShaperTree::writeToFile(int index, std::ofstream &filename) {
    if (index < 0 || index >= realmShapers.size()) {
        return;
    }
    filename << realmShapers[index]->getName() << std::endl;

    int leftIndex = 2 * index + 1;
    int rightIndex = 2 * index + 2;

    writeToFile(leftIndex, filename);
    writeToFile(rightIndex, filename);
}