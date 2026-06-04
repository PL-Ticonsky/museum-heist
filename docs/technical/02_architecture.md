# Architecture

Museum Heist is divided into four main parts: a C++ engine, Python algorithms, a Pygame UI, and a JSON bridge. The C++ engine owns the game rules and state generation. Python owns rendering, keyboard handling, algorithm visualization, and file-based communication.

## Architecture Diagram

```text
        Keyboard input
              |
              v
    +-------------------+
    | Pygame UI         |
    | game/ui/main.py   |
    +-------------------+
       |            ^
       | uses       | displays
       v            |
+----------------+  |
| Python         |  |
| algorithms     |  |
| greedy.py      |  |
| backtracking.py|  |
+----------------+  |
       ^            |
       | state      |
       |            |
    +-------------------+
    | JSON bridge       |
    | game/ui/bridge.py |
    +-------------------+
       |            ^
       | writes     | reads
       v            |
 game/data/input.json
       |            ^
       v            |
    +-------------------+
    | C++ engine        |
    | engine/main.cpp   |
    +-------------------+
       |
       | writes
       v
 game/data/state.json
```

## C++ Engine

The C++ engine is the rule-processing layer. It reads `game/data/input.json`, selects a hardcoded map from the requested difficulty, applies the requested action, validates movement, updates score, alarm, time, and end conditions, then writes `game/data/state.json`.

The engine also uses two required custom data structures:

- `MovementList` stores movement history.
- `ItemBST` writes remaining items sorted by value.

## Python Algorithms

The Python algorithms read the state dictionary loaded from `state.json`.

- `greedy.py` selects one item according to local criteria.
- `backtracking.py` searches for a safe route from the player to the exit.

The algorithms do not directly modify the game state. They produce information for visualization in the UI.

## Pygame UI

`game/ui/main.py` creates the window, handles keyboard input, tracks screen states, draws the board and sidebar, and shows final screens. It sends player actions to the bridge and redraws the newest state after each engine run.

## JSON Bridge

`game/ui/bridge.py` connects Python and C++ using files:

1. It writes `input.json`.
2. It compiles the C++ engine when `get_game_state()` is called.
3. It runs `engine/museum_engine`.
4. It reads `state.json` into Python.

## Execution Flow From `main.py`

1. `main()` calls `bridge.get_game_state()`.
2. `bridge.get_game_state()` compiles the C++ engine, runs it, and loads `state.json`.
3. The UI computes the greedy item and backtracking route from the loaded state.
4. The start screen lets the player choose Easy, Normal, or Hard.
5. During play, movement keys create a direction such as `up`, `down`, `left`, or `right`.
6. The UI writes the action through `bridge.write_input()`.
7. The bridge runs the C++ engine.
8. The engine writes updated `state.json`.
9. The UI reloads the state, recomputes algorithm outputs, and redraws the screen.
10. If the player escapes or is caught, the UI switches to the final screen.
