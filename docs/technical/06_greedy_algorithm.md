# Greedy Algorithm

The greedy algorithm is implemented in `game/algorithms/greedy.py`. It selects one remaining item to highlight in the UI.

## Greedy Decision

`choose_greedy_item(state)` receives the current state from `state.json` and examines:

- remaining items;
- guards;
- vision zones.

The decision order is:

1. Select the highest-value item that is near a guard and not in a vision zone.
2. If none exists, select the highest-value safe item outside vision zones.
3. If every remaining item is in a vision zone, select the highest-value remaining item and return a warning reason.
4. If no items remain, return `None`.

An item is considered near a guard when its Manhattan distance to a guard is less than or equal to `NEAR_GUARD_DISTANCE`, which is `2`.

## Locally Optimal Choice

The algorithm is greedy because it makes one immediate decision based on the current map state. It prefers high-value items under simple local safety rules.

This is not globally guaranteed to produce the best full route or final score. It does not simulate future moves, future alarms, movement limits, or alternate paths. It only chooses the best-looking item according to the current local criteria.

## Pseudocode

```text
function choose_greedy_item(state):
    items = remaining uncollected items
    guards = state guards
    vision_zones = state vision zones

    if items is empty:
        return no item

    best_near_guard = none
    for each item in items:
        if item is near a guard and item is not in vision zone:
            if best_near_guard is none or item.value is greater:
                best_near_guard = item

    if best_near_guard exists:
        return best_near_guard

    best_safe = none
    for each item in items:
        if item is not in vision zone:
            if best_safe is none or item.value is greater:
                best_safe = item

    if best_safe exists:
        return best_safe

    return highest-value item from all remaining items
```

## Complexity Analysis

Let:

- `i` be the number of remaining items;
- `g` be the number of guards;
- `v` be the number of vision zone cells.

For each item, the algorithm may check vision zones and guards. The complexity is:

```text
O(i * (g + v))
```

The project maps are small, so this cost is acceptable for recomputing the result after each move.

## UI Visualization

`main.py` calls `choose_greedy_item(state)` through `update_results(state)`. The selected item is shown in two ways:

- the item tile is outlined with `SELECTED_ITEM_COLOR`;
- the sidebar displays text such as `Selected item: painting (100)`.
