#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints

    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open the file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int honour;
        if (!(iss >> name >> honour))
        {
            std::cerr << "Error: Invalid line format in file " << filename << ": " << line << std::endl;
            continue;
        }

        RealmShaper *newPlayer = new RealmShaper(name, honour);
        players.push_back(newPlayer);
    }
    file.close();
    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same
    return this->name == other.name;;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}
