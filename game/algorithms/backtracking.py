def find_escape_route(state):
    rows = state["grid"]["rows"]
    cols = state["grid"]["cols"]
    start = state["player"]
    goal = state["exit"]

    blocked = []
    add_positions(blocked, state["walls"])
    add_positions(blocked, state["cameras"])
    add_positions(blocked, state["guards"])
    add_positions(blocked, state.get("vision_zones", []))

    route = []
    visited = []

    route = find_route_from(
        start["row"],
        start["col"],
        goal["row"],
        goal["col"],
        rows,
        cols,
        blocked,
        visited,
        route,
    )

    if route is not None:
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


def manhattan_distance(row, col, goal_row, goal_col):
    return abs(row - goal_row) + abs(col - goal_col)


def get_ordered_moves(row, col, goal_row, goal_col, rows, cols, blocked, visited):
    moves = [
        (-1, 0),
        (1, 0),
        (0, -1),
        (0, 1),
    ]
    valid_moves = []

    for move in moves:
        next_row = row + move[0]
        next_col = col + move[1]

        if is_valid_cell(next_row, next_col, rows, cols, blocked, visited):
            valid_moves.append(move)

    valid_moves.sort(
        key=lambda move: manhattan_distance(
            row + move[0],
            col + move[1],
            goal_row,
            goal_col,
        )
    )

    return valid_moves


def find_route_from(row, col, goal_row, goal_col, rows, cols, blocked, visited, route):
    if not is_valid_cell(row, col, rows, cols, blocked, visited):
        return None

    # Choose this cell.
    visited.append((row, col))
    route.append({"row": row, "col": col})

    if row == goal_row and col == goal_col:
        return route.copy()

    moves = get_ordered_moves(row, col, goal_row, goal_col, rows, cols, blocked, visited)

    for move in moves:
        next_row = row + move[0]
        next_col = col + move[1]

        # Explore the next possible cell.
        result = find_route_from(
            next_row,
            next_col,
            goal_row,
            goal_col,
            rows,
            cols,
            blocked,
            visited,
            route,
        )

        if result is not None:
            return result

    # Unchoose this cell.
    visited.pop()
    route.pop()
    return None
