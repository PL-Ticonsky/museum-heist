# Demo Script

This script divides the presentation among three speakers. The repository does not specify real team member names, so the script uses role labels.

## Speaker Roles

- Team Member 1: C++ engine and data structures.
- Team Member 2: Python algorithms and JSON bridge.
- Team Member 3: Pygame UI and game mechanics.

## 5-Minute Version

### 0:00-0:40 - Introduction

Team Member 3:

"This project is Museum Heist, a grid-based stealth game. The objective is to collect items, avoid guards, cameras, and vision zones, and reach the exit before the alarm, movement limit, or time limit ends the run."

### 0:40-1:50 - C++ Engine And Data Structures

Team Member 1:

"The game rules are processed by a C++ engine. The engine reads `input.json`, selects a difficulty map, validates movement, updates alarm, score, collected items, time, and end conditions, then writes `state.json`."

"We implemented a custom linked list called `MovementList` to store movement history. Each node stores row, column, step, and a pointer to the next node. We also implemented `ItemBST`, a binary search tree that stores remaining items by value and writes them in sorted order using inorder traversal."

### 1:50-3:00 - Algorithms And JSON Bridge

Team Member 2:

"Python communicates with C++ through JSON files. The UI writes the action and current state to `input.json`. The C++ engine writes the new full state to `state.json`, and Python loads it again."

"The greedy algorithm selects one item using local criteria: first the highest-value safe item near a guard, then the highest-value safe item, and finally the highest-value item if all are dangerous. The backtracking algorithm recursively searches for a route from the player to the exit while avoiding walls, cameras, guards, and vision zones."

### 3:00-4:30 - UI And Game Mechanics

Team Member 3:

"The Pygame UI shows the start screen, tutorial, playing screen, and final screens. The board renders floor, vision zones, route, walls, items, cameras, guards, player, and the sidebar. The sidebar shows score, items, movements, time, alarm, route length, rank, and selected greedy item."

Demonstrate:

- choose a difficulty;
- start the game;
- move the player;
- show the route and selected item;
- collect or approach an item;
- show restart or main menu.

### 4:30-5:00 - Closing

Team Member 1:

"The project combines C++ rule processing, Python visualization, JSON communication, greedy selection, backtracking search, and custom data structures in one playable game."

## 10-Minute Version

### 0:00-1:00 - Introduction

Team Member 3:

"Museum Heist is a Computer Science I project built with Python, Pygame, and C++. The player moves through a museum grid, collects items for score, avoids walls and surveillance, and tries to escape."

### 1:00-3:00 - C++ Engine

Team Member 1:

"The C++ engine is responsible for the official game state. It contains three hardcoded maps: Easy, Normal, and Hard. Each difficulty changes grid size, alarm limit, movement limit, and time limit."

"When a movement request arrives, the engine checks boundaries, walls, cameras, guards, and vision zones. Valid moves are stored in movement history. Invalid wall moves increase alarm. Vision-zone moves end the game. The engine also checks alarm, movement, time, and escape conditions."

### 3:00-4:20 - Data Structures

Team Member 1:

"The linked list stores the movement history in order. `push_back()` appends a new node at the tail in constant time, and traversal writes the JSON array."

"The BST stores remaining items by value. Smaller values go left, larger values go right, and equal values use id as a tie breaker. Inorder traversal produces `items_sorted_by_value`."

### 4:20-6:10 - JSON Bridge And Algorithms

Team Member 2:

"The bridge compiles and runs the C++ engine, writes input, and loads output. This keeps the C++ engine separate from the Python UI while allowing both parts to share game state."

"The greedy algorithm highlights a selected item. It is locally optimal because it chooses from the current state only. The backtracking algorithm uses choose, explore, and unchoose recursion to find a safe escape route."

### 6:10-8:50 - UI Demonstration

Team Member 3:

Demonstrate:

- start screen and difficulty selection;
- tutorial screen;
- movement with WASD or arrow keys;
- vision zones and route markers;
- item collection and score update;
- alarm change after risky actions;
- restart with `R`;
- main menu with `M`;
- final escaped or caught screen.

### 8:50-10:00 - Summary

Team Member 2:

"The project demonstrates course topics through an interactive system: C++ logic, Python UI, JSON file communication, greedy and backtracking algorithms, linked list movement history, and BST item ordering."
