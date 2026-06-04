# Technical Questions And Answers

## Why did you use a linked list?

The C++ engine uses `MovementList` to store movement history. Each valid move is appended to the end, and later the list is traversed to write `movement_history` to `state.json`. The custom linked list satisfies the data structure requirement and matches the append-and-traverse behavior of the movement history.

## Why did you use a BST?

The C++ engine uses `ItemBST` to store remaining items ordered by value. Inorder traversal writes `items_sorted_by_value` from lower value to higher value. This demonstrates a binary search tree using item value as the ordering key and id as a tie breaker.

## Why did you use JSON?

JSON lets the Python UI and C++ engine communicate through readable files. Python writes `input.json`, C++ reads it and writes `state.json`, and Python reads the updated state. The files are easy to inspect during debugging and testing.

## Why did you use backtracking?

Backtracking is used to search for a safe route from the player to the exit. The algorithm chooses a cell, explores valid neighboring cells, and unchooses the cell if the path does not reach the goal. This fits route searching with blocked cells.

## Why did you use greedy?

The greedy algorithm highlights one item using immediate local criteria. It chooses a high-value item while checking simple danger conditions. It is useful for showing a decision strategy without simulating every possible future route.

## What is the complexity of the linked list operations?

`MovementList::push_back()` is `O(1)` because the class stores a tail pointer. Traversal and clear are `O(n)` because each node must be visited. Search is not implemented in the current linked list.

## What is the complexity of the BST operations?

BST insert is `O(log n)` on average and `O(n)` in the worst case if the tree becomes unbalanced. Inorder traversal and clear are `O(n)`. Search is not implemented in the current `ItemBST` class.

## What is the complexity of the greedy algorithm?

If `i` is the number of remaining items, `g` is the number of guards, and `v` is the number of vision zone cells, the greedy algorithm is `O(i * (g + v))`.

## What is the complexity of the backtracking algorithm?

In the worst case, backtracking can be exponential in the number of open cells because it may explore many possible paths. A simple upper bound is `O(4^n)`, where `n` is the number of grid cells.

## How does C++ communicate with Python?

Python writes `game/data/input.json` through `bridge.py`. The bridge runs the C++ executable. The C++ engine reads the input file, applies the rules, and writes `game/data/state.json`. Python then loads that state with `json.load()`.

## What happens when the player moves?

The UI writes a `move` action and direction to `input.json`. The C++ engine calculates the next cell, checks bounds, walls, cameras, guards, and vision zones, updates the state, writes `state.json`, and the UI reloads and redraws the result.

## What fields are stored in `state.json`?

`state.json` stores input status, difficulty, grid size, player position, exit, escaped flag, game over flag, game over reason, alarm, max alarm, max movements, elapsed seconds, time limit, status, rank, score, item counts, collected items, walls, cameras, guards, vision zones, visible items, movement history, and items sorted by value.

## What are the project limitations?

- The maps are hardcoded in C++.
- The C++ JSON reading uses simple string searches instead of a full JSON parser.
- The BST is not self-balancing.
- The greedy algorithm does not guarantee the best final score or safest complete route.
- The backtracking route avoids blocked and vision cells but does not optimize score, alarm, or time.
- The repository documentation files do not list real team member names.
