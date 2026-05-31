NEAR_GUARD_DISTANCE = 2


def manhattan_distance(a_row, a_col, b_row, b_col):
    return abs(a_row - b_row) + abs(a_col - b_col)


def choose_greedy_item(state):
    items = state["items"]
    guards = state["guards"]

    if len(items) == 0:
        return {
            "item": None,
            "reason": "There are no items to choose.",
        }

    best_near_guard = None

    for item in items:
        item_is_near_guard = False

        for guard in guards:
            distance = manhattan_distance(
                item["row"],
                item["col"],
                guard["row"],
                guard["col"],
            )

            if distance <= NEAR_GUARD_DISTANCE:
                item_is_near_guard = True

        if item_is_near_guard:
            if best_near_guard is None or item["value"] > best_near_guard["value"]:
                best_near_guard = item

    if best_near_guard is not None:
        return {
            "item": best_near_guard,
            "reason": "Highest-value item near a guard.",
        }

    best_overall = items[0]

    for item in items:
        if item["value"] > best_overall["value"]:
            best_overall = item

    return {
        "item": best_overall,
        "reason": "No item is near a guard, so the highest-value item was chosen.",
    }
