#include "Map.h"
#include <stack>

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    delete root;
    root = nullptr;

}



void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    for (Isle *isle : isles) {
        isle->setItem(Item::EMPTY);
        insert(isle);
    }

    populateWithItems();

}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->left) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* newRoot = current->left;

    current->left = newRoot->right;

    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;

}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->right) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* newRoot = current->right;

    current->right = newRoot->left;

    newRoot->left = current;

    current->height = std::max(height(current->right), height(current->left)) + 1;
    newRoot->height = std::max(height(newRoot->right), height(newRoot->left)) + 1;

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node == nullptr)
        return 0;
    return node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;
    // TODO: Recursively insert isle to the tree
    // returns inserted node
    if (node == nullptr) {
        MapNode *newNode = new MapNode(isle);
        newNode->height = 1;
        return newNode;
    }

    if (*isle < *node->isle) {
        node->left = insert(node->left, isle);
    }

    else {
        node->right = insert(node->right, isle);
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balanceValue = height(node->left) - height(node->right);

    if (balanceValue >= 2) {
        if (*isle < *node->left->isle && node->left) {
            rebalanceCounter ++;
            if (rebalanceCounter % 3 == 0) {
                populateWithItems();
            }
            return rotateRight(node);
        }
        else if (node->left && *isle > *node->left->isle) {
            node->left = rotateLeft(node->left);
            rebalanceCounter ++;
            if (rebalanceCounter % 3 == 0) {
                populateWithItems();
            }
            return rotateRight(node);
        }
    }

    else if (balanceValue <= -2) {
        if (*isle > *node->right->isle && node->right) {
            rebalanceCounter ++;
            if (rebalanceCounter % 3 == 0) {
                populateWithItems();
            }
            return rotateLeft(node);
        }
        else if (node->right && *isle < *node->right->isle) {
            node->right = rotateRight(node->right);
            rebalanceCounter ++;
            if (rebalanceCounter % 3 == 0) {
                populateWithItems();
            }
            return rotateLeft(node);
        }
    }

    return node;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
    if (node == nullptr) {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return node;
    }

    if (*isle < *node->isle) {
        node->left = remove(node->left, isle);
    }
    else if (*isle > *node->isle) {
        node->right = remove(node->right, isle);
    }
    else {

        if (node->left == nullptr || node->right == nullptr) {

            MapNode *temp;
            if (node->left != nullptr) {
                temp = node->left;
            }
            else {
                temp = node->right;
            }
            if (temp == nullptr) {
                delete node;
                return nullptr;
            }
            delete node;
            node = temp;
        }
        else {

            MapNode *temp = findInOrderSuccessor(node);

            if (temp == nullptr) {
                std::cerr << "[Remove] Error: In-order successor not found." << std::endl;
                return node;
            }

            node->isle = temp->isle;

            node->right = remove(node->right, temp->isle);
        }
    }

    if (node == nullptr) {
        return node;
    }


    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = height(node->left) - height(node->right);

    if (balance > 1 && node->left != nullptr && height(node->left->left) >= height(node->left->right)) {
        rebalanceCounter ++;
        if (rebalanceCounter % 3 == 0) {
            populateWithItems();
        }
        return rotateRight(node);
    }

    if (balance < -1 && node->right != nullptr && height(node->right->right) >= height(node->right->left)) {
        rebalanceCounter ++;
        if (rebalanceCounter % 3 == 0) {
            populateWithItems();
        }
        return rotateLeft(node);
    }

    if (balance > 1 && node->left != nullptr && height(node->left->left) < height(node->left->right)) {
        node->left = rotateLeft(node->left);
        rebalanceCounter ++;
        if (rebalanceCounter % 3 == 0) {
            populateWithItems();
        }
        return rotateRight(node);
    }

    if (balance < -1 && node->right != nullptr && height(node->right->right) < height(node->right->left)) {
        node->right = rotateRight(node->right);
        rebalanceCounter ++;
        if (rebalanceCounter % 3 == 0) {
            populateWithItems();
        }
        return rotateLeft(node);
    }
    return node;
}


MapNode *Map::findInOrderSuccessor(MapNode *node) {
    if (node == nullptr || node->right == nullptr) {
        return nullptr;
    }

    MapNode* current = node->right;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr) {
        return;
    }

    count++;
    if (count % 5 == 0) {
        current->isle->setItem(Item :: EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);

    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr) {
        return;
    }

    postOrderItemDrop(current->left, count);

    postOrderItemDrop(current->right, count);

    count++;
    if (count % 3 == 0 && current->isle->getItem() == Item::EMPTY) {
        current->isle->setItem(Item::GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item
    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty()) {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getItem() == Item::EMPTY) {
            return current;
        }

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    if (root == nullptr) {
        return;
    }

    if (rebalanceCounter % 3 != 0 || rebalanceCounter == 0) {
        return;
    }
    MapNode *emptyIsle = findFirstEmptyIsle(root);
    if (emptyIsle == nullptr) {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }
    else {
        emptyIsle->isle->setItem(Item::AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << emptyIsle->isle->getName() << std::endl;
    }

}
void Map::clearAllItems(MapNode* current)
{
    if (current == nullptr) {
        return;
    }

    clearAllItems(current->left);
    current->isle->setItem(Item::EMPTY);
    clearAllItems(current->right);
}

void Map::clearAllItems()
{
    clearAllItems(root);
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    if (node == nullptr) {
        return 0;
    }

    int leftDepth = getDepth(node->left);
    int rightDepth = getDepth(node->right);

    return std::max(leftDepth, rightDepth) + 1;

}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    MapNode *node = findNode(*isle);
    return getDepth(node);
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    if (root == nullptr) {
        return 0;
    }

    return getDepth(root);
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM

    clearAllItems();

    int goldiumCount = 0;
    postOrderItemDrop(root, goldiumCount);

    int einsteinumCount = 0;
    preOrderItemDrop(root, einsteinumCount);

    dropItemBFS();
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    return findIsleFromNode(root, isle);
}

Isle* Map::findIsleFromNode(MapNode* node, Isle isle) {
    if (node == nullptr) {
        return nullptr;
    }

    if (*node->isle == isle) {
        return node->isle;
    }
    else if (*node->isle > isle) {
        return findIsleFromNode(node->left, isle);
    }
    else {
        return findIsleFromNode(node->right, isle);
    }
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    return findIsleFromNodeName(root, name);}

Isle *Map::findIsleFromNodeName(MapNode* node ,std::string name) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->isle->getName() == name) {
        return node->isle;
    }
    else if (node->isle->getName() > name) {
        return findIsleFromNodeName(node->left, name);
    }
    else {
        return findIsleFromNodeName(node->right, name);
    }
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode *current = root;
    while (current != nullptr) {
        if (*current->isle == isle) {
            return current;
        }
        else if (*current->isle > isle) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *current = root;

    while (current != nullptr) {
        if (current->isle->getName() == name) {
            return current;
        } else if (name < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
    if (root == nullptr) {
        return;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open file: " << filename << std::endl;
        return;
    }

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            MapNode *current = q.front();
            q.pop();
            if (current != nullptr) {
                if (current->isle != nullptr) {
                    file << current->isle->getName() << " ";
                }
                q.push(current->left);
                q.push(current->right);
            } else {
                file << "NULL ";
            }
        }
        file << "\n";
    }
    file.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;

    if (root == nullptr) {
        return;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open file: " << filename << std::endl;
        return;
    }

    std::stack<MapNode *> stack;
    MapNode *current = root;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();

        file << current->isle->getName() << "\n";

        current = current->right;
    }

    file.close();
    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
}

