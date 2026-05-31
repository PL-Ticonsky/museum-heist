def find_escape_route(state):
    rows = state["grid"]["rows"]
    cols = state["grid"]["cols"]
    start = state["player"]
    goal = state["exit"]

    blocked = []
    add_positions(blocked, state["walls"])
    add_positions(blocked, state["cameras"])
    add_positions(blocked, state["guards"])

    route = []
    visited = []

    if find_route_from(
        start["row"],
        start["col"],
        goal["row"],
        goal["col"],
        rows,
        cols,
        blocked,
        visited,
        route,
    ):
        return route

    return []


def add_positions(blocked, positions):
    for position in positions:
        blocked.append((position["row"], position["col"]))


def contains_position(positions, row, col):
    for position in positions:
        if position[0] == row and position[1] == col:
            return True

    return False


def is_valid_cell(row, col, rows, cols, blocked, visited):
    if row < 0 or row >= rows:
        return False

    if col < 0 or col >= cols:
        return False

    if contains_position(blocked, row, col):
        return False

    if contains_position(visited, row, col):
        return False

    return True


def find_route_from(row, col, goal_row, goal_col, rows, cols, blocked, visited, route):
    if not is_valid_cell(row, col, rows, cols, blocked, visited):
        return False

    # Choose this cell.
    visited.append((row, col))
    route.append({"row": row, "col": col})

    if row == goal_row and col == goal_col:
        return True

    moves = [
        (-1, 0),
        (1, 0),
        (0, -1),
        (0, 1),
    ]

    for move in moves:
        next_row = row + move[0]
        next_col = col + move[1]

        # Explore the next possible cell.
        if find_route_from(
            next_row,
            next_col,
            goal_row,
            goal_col,
            rows,
            cols,
            blocked,
            visited,
            route,
        ):
            return True

    # Unchoose this cell.
    visited.pop()
    route.pop()
    return False
