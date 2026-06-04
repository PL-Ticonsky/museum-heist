# Museum Heist Project Overview

## Project Title

Museum Heist

## Team Members

The repository files reviewed for this documentation do not list team member names. For that reason, this document does not invent or assign names.

## Game Objective

Museum Heist is a grid-based stealth game. The player starts at the entrance of a museum map, collects valuable items, avoids walls, cameras, guards, and vision zones, and tries to reach the exit before being caught.

When the player reaches the exit, the game ends successfully and shows final metrics: score, collected items, movement count, elapsed time, and rank.

## Main Mechanics

- The player moves one cell at a time using the keyboard.
- Walls block movement.
- Cameras and guards occupy dangerous cells.
- Cameras see in one configured direction for a limited range.
- Guards see up, down, left, and right for a limited range.
- Walls stop camera and guard vision.
- Entering a camera cell, guard cell, or vision zone causes game over.
- Attempting to move into a wall increases the alarm.
- Ending a move next to a guard increases the alarm.
- Collecting an item near a guard increases the alarm.
- Collected items disappear from the map.
- A movement limit, alarm limit, and time limit are enforced by difficulty.
- The UI shows a greedy-selected item and a backtracking escape route.

## Technologies Used

- C++17 for the game engine in `engine/main.cpp`.
- Custom C++ linked list in `engine/linked_list.h` and `engine/linked_list.cpp`.
- Custom C++ binary search tree in `engine/tree.h` and `engine/tree.cpp`.
- Python for the UI bridge and algorithms.
- Pygame for the graphical user interface.
- JSON files in `game/data/` for communication between Python and C++.
- `g++` for compiling the C++ engine.

## Course Requirements Coverage

Museum Heist satisfies the Computer Science I requirements through implemented code:

| Requirement | Implementation |
| --- | --- |
| Greedy algorithm | `game/algorithms/greedy.py` selects one item using local value and danger criteria. |
| Backtracking algorithm | `game/algorithms/backtracking.py` recursively searches for an escape route. |
| Linked list | `MovementList` stores movement history in insertion order. |
| Binary search tree | `ItemBST` stores remaining items ordered by value, with id used as a tie breaker. |
| C++ engine | `engine/main.cpp` validates movement, applies rules, and writes state output. |
| Python interface | `game/ui/main.py` renders the game and handles keyboard input. |
| JSON bridge | `game/ui/bridge.py` writes `input.json`, runs the C++ engine, and reads `state.json`. |
