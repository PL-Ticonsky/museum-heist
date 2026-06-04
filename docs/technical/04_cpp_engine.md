# C++ Engine

The C++ engine is implemented in `engine/main.cpp`. It is a standalone executable that reads `game/data/input.json`, applies the game rules, and writes `game/data/state.json`.

## Compile Command

The Python bridge uses this compile command:

```bash
g++ -std=c++17 engine/main.cpp engine/linked_list.cpp engine/tree.cpp -o engine/museum_engine
```

## Main Execution

`main()` uses these paths:

- input: `game/data/input.json`
- output: `game/data/state.json`

The engine checks whether the input file exists, reads the difficulty, selects a map, initializes state variables, applies the input, and writes the output file.

If the input does not contain a difficulty, the engine defaults to `Normal`.

## Difficulty Map Selection

`load_map_for_difficulty()` selects one of three hardcoded maps:

| Difficulty | Grid | Start | Exit | Alarm Limit | Movement Limit | Time Limit |
| --- | --- | --- | --- | --- | --- | --- |
| Easy | 8x8 | `(0, 0)` | `(7, 7)` | 4 | 45 | 90 seconds |
| Normal | 8x8 | `(0, 0)` | `(7, 7)` | 3 | 35 | 60 seconds |
| Hard | 10x10 | `(0, 0)` | `(9, 9)` | 2 | 28 | 45 seconds |

Each map defines walls, cameras, guards, and items using fixed arrays in the C++ code.

## Movement Validation

The engine receives the current position and requested direction. It calculates the next cell, then validates it:

1. If the next cell is outside the grid, the player stays in place.
2. If the next cell is a wall, the player stays in place and the alarm increases by 1.
3. If the next cell is a camera, guard, or vision cell, the game ends immediately.
4. If the move is valid, the player moves and the position is added to `MovementList`.

`is_valid_move()` rejects cells outside the grid, walls, camera cells, guard cells, and vision cells.

## Walls

Walls are stored as `Position` values in the selected `MapData`. The function `is_wall_cell()` checks whether a row and column match a wall. Walls block movement and also stop camera and guard vision.

## Cameras

Cameras are stored with:

- `id`
- `row`
- `col`
- `direction`
- `range`

`camera_sees_cell()` checks the camera's direction one step at a time until it reaches the range, the grid boundary, or a wall.

## Guards

Guards are stored with:

- `id`
- `row`
- `col`
- `vision_range`

`guard_sees_cell()` checks four directions: up, down, left, and right. Like cameras, guard vision stops at walls or boundaries.

## Vision Zones

`is_vision_cell()` returns true when a cell is seen by a camera or guard. It returns false for wall cells. Entering a vision zone causes game over with the reason `Caught in vision zone`.

## Alarm

The alarm increases when:

- the player attempts to move into a wall;
- the player ends a move next to a guard;
- the player collects an item near a guard.

`check_alarm_limit()` ends the game when `alarm >= max_alarm`.

## Score And Items

`collect_item_if_needed()` checks whether the player is on an uncollected item. If so, the item id is added to `collected_items`, the score increases by the item value, and the item will no longer be written in the visible `items` array.

## Time

The UI sends `elapsed_seconds` through `input.json`. If the run is not escaped or over and elapsed time reaches the map limit, the engine sets game over with reason `Time expired`.

## Game Over And Escape Logic

The player escapes when the current position equals the exit position after a valid move. The player is caught when:

- the player enters a camera, guard, or vision cell;
- the alarm reaches the difficulty limit;
- the movement history size reaches the movement limit before escaping;
- elapsed time reaches the time limit before escaping.

The rank is calculated by `calculate_rank()`:

- `Caught` if game over without escape;
- `In Progress` if the player has not escaped and is not caught;
- `A`, `B`, or `C` after escape, based on score, movement count, elapsed time, and alarm.

## Writing `state.json`

`write_state_file()` writes the complete state for Python. It includes map data, player state, score, alarms, time, status, rank, objects, vision zones, movement history, and items sorted by value.

The movement history is written by the custom linked list:

```cpp
history.write_json_array(out);
```

Remaining items are inserted into `ItemBST` and written through inorder traversal:

```cpp
items.inorder_json(out);
```
