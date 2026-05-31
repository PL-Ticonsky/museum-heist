NEAR_GUARD_DISTANCE = 2


def manhattan_distance(a_row, a_col, b_row, b_col):
    return abs(a_row - b_row) + abs(a_col - b_col)


def choose_greedy_item(state):
    items = get_remaining_items(state)
    guards = state["guards"]
    vision_zones = state.get("vision_zones", [])

    if len(items) == 0:
        return {
            "item": None,
            "reason": "There are no items to choose.",
        }

    best_near_guard = None

    for item in items:
        item_is_near_guard = False
        item_is_dangerous = is_in_vision_zone(item, vision_zones)

        for guard in guards:
            distance = manhattan_distance(
                item["row"],
                item["col"],
                guard["row"],
                guard["col"],
            )

            if distance <= NEAR_GUARD_DISTANCE:
                item_is_near_guard = True

        if item_is_near_guard and not item_is_dangerous:
            if best_near_guard is None or item["value"] > best_near_guard["value"]:
                best_near_guard = item

    if best_near_guard is not None:
        return {
            "item": best_near_guard,
            "reason": "Highest-value item near a guard.",
        }

    best_safe = None

    for item in items:
        if not is_in_vision_zone(item, vision_zones):
            if best_safe is None or item["value"] > best_safe["value"]:
                best_safe = item

    if best_safe is not None:
        return {
            "item": best_safe,
            "reason": "No safe item near a guard, so the highest-value safe item was chosen.",
        }

    best_overall = items[0]

    for item in items:
        if item["value"] > best_overall["value"]:
            best_overall = item

    return {
        "item": best_overall,
        "reason": "Warning: all remaining items are inside a vision zone.",
    }


def get_remaining_items(state):
    collected_items = state.get("collected_items", [])
    remaining_items = []

    for item in state["items"]:
        if item["id"] not in collected_items:
            remaining_items.append(item)

    return remaining_items


def is_in_vision_zone(item, vision_zones):
    for zone in vision_zones:
        if item["row"] == zone["row"] and item["col"] == zone["col"]:
            return True

    return False
