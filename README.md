# 🏰 H.U.SLAND-Tree-Based-Realm-Shaping-System

Realm Shapers is a **C++-based RPG simulation** that introduces a **new playable class** where players can reshape the game world dynamically. Using **AVL trees for world generation** and **Shaper Trees for ranking players**, this project simulates **duels, crafting new islands, and strategic world-building**.

## 📌 Features
- **Shaper Tree (Player Rankings)**:
  - Implemented using a **complete binary tree with dynamic insertion & deletion**.
  - Players can **challenge superior ranks in duels** to climb the hierarchy.
  - Players with **0 Honour Points are removed** from the tree.
- **Self-Balancing AVL Tree (World Map)**:
  - Stores **game world’s Isles dynamically**.
  - New Isles are inserted **while maintaining AVL balance**.
  - Overcrowded Isles **self-destruct**, triggering a re-balancing.
- **Item System & Energy Management**:
  - Isles contain special items (e.g., **Goldium, Einsteinium, Amazonite**).
  - Players collect items to **craft new Isles and expand the game world**.
- **File-Based Input & Output**:
  - Loads **initial world and player data** from text files.
  - Stores **updated player rankings and world state** after every action.

## 🎮 How It Works
1. **Game starts with players ranked in the Shaper Tree** and Isles placed in an AVL tree.
2. **Players perform actions** based on log files (`access.log`, `duels.log`).
3. **Every 5 access records**, a **duel is processed**.
4. **Players can challenge superior ranks** to gain Honour Points.
5. **Isles dynamically change** as players interact and craft new locations.
6. **The world map and rankings update in real-time**.

## 📂 Input & Output
### **Input Files**
- **`initial_realm_shapers.txt`** → Initial player rankings & Honour Points.
- **`initial_world.txt`** → Initial list of Isles.
- **`access.log`** → Player attempts to visit or craft Isles.
- **`duels.log`** → Records of duels between players.

### **Output Files**
- **`current_realm_shapers.txt`** → Updated player rankings.
- **`current_shaper_tree.txt`** → Pre-order traversal of the Shaper Tree.
- **`current_isles.txt`** → List of existing Isles in **in-order traversal**.
- **`current_map.txt`** → AVL tree structure of the current world.

## 🚀 Running the Program
Compile and run the program using:
```bash
g++ -std=c++11 *.cpp *.h -o HUSLAND
./HUSLAND initial_world.txt initial_realm_shapers.txt access.log duels.log current_isles.txt current_map.txt current_realm_shapers.txt current_shaper_tree.txt

