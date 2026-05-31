# JSON Bridge

The JSON bridge is the file-based communication layer between Python and C++. Python writes `game/data/input.json`, the C++ engine reads it, applies the game rules, and writes `game/data/state.json`. Python then reads `state.json` to update the UI.

## Why JSON Is Used

JSON is used because both Python and C++ can read and write text files, and JSON is easy to inspect during testing. In this project, Python uses the standard `json` module. The C++ engine uses simple string searches to read the fields it needs and manually writes JSON output with `std::ofstream`.

## `input.json`

`input.json` stores the current action request and the previous state values needed by the engine.

Important fields:

| Field | Meaning |
| --- | --- |
| `action` | Requested action, such as `reset`, `move`, `tick`, or `none`. |
| `direction` | Movement direction for a move action: `up`, `down`, `left`, or `right`. |
| `difficulty` | Selected map difficulty: `Easy`, `Normal`, or `Hard`. |
| `player.row`, `player.col` | Player position before applying the next action. |
| `movement_step` | Current movement step value from the UI. |
| `movement_history` | Previous movement history preserved between engine runs. |
| `collected_items` | Item ids already collected. |
| `score` | Current score. |
| `escaped` | Whether the player has reached the exit. |
| `game_over` | Whether the player has already been caught. |
| `game_over_reason` | Explanation for the caught state. |
| `alarm` | Current alarm level. |
| `elapsed_seconds` | Time elapsed in the current run. |

Current example fragment:

```json
{
  "action": "reset",
  "direction": "",
  "difficulty": "Hard",
  "player": {
    "row": 0,
    "col": 0
  },
  "movement_step": 0,
  "movement_history": [],
  "collected_items": [],
  "score": 0,
  "escaped": false,
  "game_over": false,
  "alarm": 0,
  "elapsed_seconds": 0
}
```

## `state.json`

`state.json` is the complete state written by the C++ engine for the UI and algorithms.

Important fields:

| Field | Meaning |
| --- | --- |
| `input_found` | Whether the engine found `input.json`. |
| `difficulty` | Difficulty selected by the engine. |
| `grid.rows`, `grid.cols` | Map size. |
| `player.row`, `player.col` | Current player position. |
| `exit.row`, `exit.col` | Exit position. |
| `escaped` | Whether the player has won. |
| `game_over` | Whether the player has lost. |
| `game_over_reason` | Reason for the loss, if any. |
| `alarm`, `max_alarm` | Current alarm and alarm limit. |
| `max_movements` | Movement limit for the difficulty. |
| `elapsed_seconds`, `time_limit` | Current time and time limit. |
| `status` | `Playing`, `Escaped`, or `Caught`. |
| `rank` | `In Progress`, `A`, `B`, `C`, or `Caught`. |
| `score` | Score from collected items. |
| `all_items_count` | Number of items defined for the map. |
| `collected_items` | Item ids collected so far. |
| `walls` | Wall cells. |
| `cameras` | Camera cells, directions, and ranges. |
| `guards` | Guard cells and vision ranges. |
| `vision_zones` | Cells watched by cameras or guards. |
| `items` | Visible uncollected items. |
| `movement_history` | Linked-list movement history written as JSON. |
| `items_sorted_by_value` | Remaining items written by BST inorder traversal. |

Current example fragments:

```json
{
  "difficulty": "Hard",
  "grid": {"rows": 10, "cols": 10},
  "player": {"row": 0, "col": 0},
  "exit": {"row": 9, "col": 9},
  "status": "Playing",
  "rank": "In Progress",
  "score": 0
}
```

```json
{
  "vision_zones": [
    {"source": "camera_1", "type": "camera", "row": 1, "col": 7},
    {"source": "guard_1", "type": "guard", "row": 3, "col": 4}
  ],
  "movement_history": [
    {"step": 0, "row": 0, "col": 0}
  ],
  "items_sorted_by_value": [
    {"id": "coin", "row": 8, "col": 3, "value": 25}
  ]
}
```

## Python To C++ Communication

The UI calls `bridge.write_input()` or `bridge.write_reset_input()`. These functions build a Python dictionary and write it to `input.json` with `json.dump(..., indent=2)`.

For movement, the UI sends:

- the action `move`;
- the direction;
- the previous player position;
- previous movement history;
- collected items;
- score;
- alarm;
- elapsed time;
- current game flags.

Then the bridge runs `engine/museum_engine`.

## C++ To Python Communication

The C++ engine writes `state.json` using `write_state_file()`. The bridge reads it with `json.load()` in `load_state()`. `main.py` then sends that state to:

- the Pygame rendering functions;
- `choose_greedy_item(state)`;
- `find_escape_route(state)`.
