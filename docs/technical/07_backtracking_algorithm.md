# Backtracking Algorithm

The backtracking algorithm is implemented in `game/algorithms/backtracking.py`. It searches for a safe route from the current player position to the exit.

## State Representation

The algorithm uses the dictionary loaded from `state.json`.

Important state values:

- `grid.rows` and `grid.cols`: map size;
- `player`: starting cell;
- `exit`: goal cell;
- `walls`: blocked wall cells;
- `cameras`: blocked camera cells;
- `guards`: blocked guard cells;
- `vision_zones`: blocked watched cells.

The route is stored as a list of dictionaries:

```json
[
  {"row": 0, "col": 0},
  {"row": 0, "col": 1}
]
```

Visited cells are stored as `(row, col)` tuples to avoid repeating cells in the same search path.

## Valid And Invalid Cells

`is_valid_cell()` returns false when:

- the row is outside the grid;
- the column is outside the grid;
- the cell is in the blocked list;
- the cell was already visited.

The blocked list is built by adding positions from walls, cameras, guards, and vision zones.

## Avoiding Vision Zones

`find_escape_route()` adds `state.get("vision_zones", [])` to the blocked list before the recursive search begins. Because vision zone cells are blocked, the route search avoids cells watched by cameras or guards.

## Choose, Explore, Unchoose

The recursive function is `find_route_from()`.

It follows the backtracking pattern:

1. Choose the current cell by adding it to `visited` and `route`.
2. Check whether the current cell is the goal.
3. Explore valid neighboring cells.
4. If no neighbor leads to the goal, unchoose the current cell by removing it from `visited` and `route`.

The possible moves are up, down, left, and right. `get_ordered_moves()` sorts valid moves by Manhattan distance to the goal, so the search tries cells closer to the exit first.

## Pseudocode

```text
function find_escape_route(state):
    blocked = walls + cameras + guards + vision_zones
    route = empty list
    visited = empty list
    result = find_route_from(player row, player col)
    if result exists:
        return result
    return empty list

function find_route_from(row, col):
    if cell is not valid:
        return none

    choose cell:
        add cell to visited
        add cell to route

    if cell is exit:
        return copy of route

    moves = valid neighboring moves ordered by distance to exit
    for each move in moves:
        result = find_route_from(next row, next col)
        if result exists:
            return result

    unchoose cell:
        remove cell from visited
        remove cell from route

    return none
```

## Complexity Analysis

Let `n` be the number of cells in the grid. In the worst case, the search may explore many valid paths before finding the exit or proving that no route exists. A simple upper bound is exponential in the number of open cells because each cell can branch to multiple neighbors:

```text
O(4^n) worst case path exploration
```

The visited list prevents cycles inside one route. Because the project grids are 8x8 or 10x10 and contain blocked cells, the implemented search is practical for this game.

## UI Visualization

`main.py` calls `find_escape_route(state)` through `update_results(state)`. During the playing state, `draw_route()` draws small route markers on the board using `ROUTE_COLOR`. If the game is playing and no route is found, the sidebar displays `No safe route found`.
