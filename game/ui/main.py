import sys
from pathlib import Path

import pygame

import bridge


GAME_DIR = Path(__file__).resolve().parents[1]
sys.path.append(str(GAME_DIR))

from algorithms.backtracking import find_escape_route
from algorithms.greedy import choose_greedy_item


GRID_SIZE = 10
TILE_SIZE = 54
MARGIN = 28
PANEL_WIDTH = 340
BOARD_SIZE = GRID_SIZE * TILE_SIZE
WINDOW_WIDTH = BOARD_SIZE + PANEL_WIDTH + MARGIN * 3
WINDOW_HEIGHT = 720
BOARD_LEFT = MARGIN
BOARD_TOP = MARGIN
PANEL_LEFT = BOARD_LEFT + BOARD_SIZE + MARGIN
PANEL_TOP = MARGIN
PANEL_HEIGHT = WINDOW_HEIGHT - MARGIN * 2

BACKGROUND_COLOR = (18, 20, 26)
PANEL_COLOR = (34, 37, 46)
PANEL_BORDER_COLOR = (82, 86, 102)
FLOOR_COLOR = (218, 205, 176)
FLOOR_ALT_COLOR = (205, 192, 166)
GRID_COLOR = (85, 78, 68)
WALL_COLOR = (54, 55, 60)
VISION_COLOR = (230, 92, 54, 92)
ROUTE_COLOR = (93, 190, 210)
PLAYER_COLOR = (55, 130, 230)
EXIT_COLOR = (75, 180, 98)
GUARD_COLOR = (210, 70, 76)
CAMERA_COLOR = (235, 190, 55)
ITEM_COLOR = (160, 92, 220)
SELECTED_ITEM_COLOR = (255, 220, 90)
TEXT_COLOR = (238, 236, 228)
MUTED_TEXT_COLOR = (176, 174, 166)
ALARM_COLOR = (255, 178, 82)
DANGER_COLOR = (215, 68, 70)
SUCCESS_COLOR = (82, 185, 106)


def tile_rect(row, col, padding=0):
    return pygame.Rect(
        BOARD_LEFT + col * TILE_SIZE + padding,
        BOARD_TOP + row * TILE_SIZE + padding,
        TILE_SIZE - padding * 2,
        TILE_SIZE - padding * 2,
    )


def tile_center(row, col):
    return (
        BOARD_LEFT + col * TILE_SIZE + TILE_SIZE // 2,
        BOARD_TOP + row * TILE_SIZE + TILE_SIZE // 2,
    )


def draw_text(screen, font, text, x, y, color=TEXT_COLOR):
    label = font.render(text, True, color)
    screen.blit(label, (x, y))


def draw_wrapped_text(screen, font, text, x, y, color=TEXT_COLOR, line_height=24):
    words = text.split()
    line = ""

    for word in words:
        test_line = word if line == "" else line + " " + word

        if font.size(test_line)[0] > PANEL_WIDTH - 44:
            draw_text(screen, font, line, x, y, color)
            y += line_height
            line = word
        else:
            line = test_line

    if line:
        draw_text(screen, font, line, x, y, color)
        y += line_height

    return y


def draw_grid(screen, rows, cols):
    board_rect = pygame.Rect(BOARD_LEFT, BOARD_TOP, cols * TILE_SIZE, rows * TILE_SIZE)
    pygame.draw.rect(screen, GRID_COLOR, board_rect, 3)

    for row in range(rows):
        for col in range(cols):
            color = FLOOR_COLOR
            if (row + col) % 2 == 1:
                color = FLOOR_ALT_COLOR

            rect = tile_rect(row, col)
            pygame.draw.rect(screen, color, rect)
            pygame.draw.rect(screen, GRID_COLOR, rect, 1)


def draw_vision_zones(screen, state):
    overlay = pygame.Surface((TILE_SIZE, TILE_SIZE), pygame.SRCALPHA)
    overlay.fill(VISION_COLOR)

    for zone in state.get("vision_zones", []):
        rect = tile_rect(zone["row"], zone["col"])
        screen.blit(overlay, rect)


def draw_route(screen, escape_route):
    for position in escape_route:
        center = tile_center(position["row"], position["col"])
        pygame.draw.circle(screen, ROUTE_COLOR, center, 5)


def draw_wall(screen, row, col):
    rect = tile_rect(row, col, 5)
    pygame.draw.rect(screen, WALL_COLOR, rect)
    pygame.draw.rect(screen, (26, 27, 30), rect, 2)


def draw_player(screen, row, col):
    pygame.draw.circle(screen, PLAYER_COLOR, tile_center(row, col), 16)


def draw_exit(screen, row, col):
    rect = tile_rect(row, col, 14)
    pygame.draw.rect(screen, EXIT_COLOR, rect)
    pygame.draw.rect(screen, (32, 95, 52), rect, 3)


def draw_guard(screen, row, col):
    pygame.draw.circle(screen, GUARD_COLOR, tile_center(row, col), 16)


def draw_camera(screen, row, col):
    center_x, center_y = tile_center(row, col)
    points = [
        (center_x, center_y - 17),
        (center_x - 17, center_y + 14),
        (center_x + 17, center_y + 14),
    ]
    pygame.draw.polygon(screen, CAMERA_COLOR, points)
    pygame.draw.polygon(screen, (98, 76, 18), points, 2)


def draw_item(screen, item):
    center_x, center_y = tile_center(item["row"], item["col"])
    points = [
        (center_x, center_y - 16),
        (center_x + 15, center_y),
        (center_x, center_y + 16),
        (center_x - 15, center_y),
    ]
    pygame.draw.polygon(screen, ITEM_COLOR, points)


def draw_selected_item(screen, selected_item, pulse):
    if selected_item is None:
        return

    padding = 10 - pulse
    rect = tile_rect(selected_item["row"], selected_item["col"], padding)
    pygame.draw.rect(screen, SELECTED_ITEM_COLOR, rect, 4, border_radius=4)


def draw_objects(screen, state, selected_item, pulse):
    for wall in state["walls"]:
        draw_wall(screen, wall["row"], wall["col"])

    for item in state["items"]:
        draw_item(screen, item)

    draw_selected_item(screen, selected_item, pulse)

    exit_position = state["exit"]
    draw_exit(screen, exit_position["row"], exit_position["col"])

    for camera in state["cameras"]:
        draw_camera(screen, camera["row"], camera["col"])

    for guard in state["guards"]:
        draw_guard(screen, guard["row"], guard["col"])

    player = state["player"]
    draw_player(screen, player["row"], player["col"])


def greedy_item_text(greedy_result):
    item = greedy_result["item"]

    if item is None:
        return "Selected item: none"

    return f"Selected item: {item['id']} ({item['value']})"


def draw_status_banner(screen, state, font):
    status = state.get("status", "Playing")

    if status == "Escaped":
        color = SUCCESS_COLOR
    elif status == "Caught":
        color = DANGER_COLOR
    else:
        color = PANEL_BORDER_COLOR

    rect = pygame.Rect(PANEL_LEFT + 18, PANEL_TOP + 112, PANEL_WIDTH - 36, 34)
    pygame.draw.rect(screen, color, rect, border_radius=6)
    draw_text(screen, font, f"Status: {status}", rect.x + 12, rect.y + 7)


def draw_metric(screen, font, name, value, x, y, value_color=TEXT_COLOR):
    draw_text(screen, font, name, x, y, MUTED_TEXT_COLOR)
    draw_text(screen, font, str(value), x + 150, y, value_color)


def format_time(seconds):
    return f"{seconds}s"


def draw_legend_item(screen, font, x, y, color, label, shape="square"):
    if shape == "circle":
        pygame.draw.circle(screen, color, (x + 9, y + 9), 8)
    elif shape == "triangle":
        points = [(x + 9, y), (x, y + 17), (x + 18, y + 17)]
        pygame.draw.polygon(screen, color, points)
    else:
        pygame.draw.rect(screen, color, (x, y, 18, 18), border_radius=3)

    draw_text(screen, font, label, x + 28, y - 1, MUTED_TEXT_COLOR)


def draw_sidebar(screen, fonts, state, greedy_result, escape_route):
    title_font, font, small_font = fonts
    panel = pygame.Rect(PANEL_LEFT, PANEL_TOP, PANEL_WIDTH, PANEL_HEIGHT)
    pygame.draw.rect(screen, PANEL_COLOR, panel, border_radius=8)
    pygame.draw.rect(screen, PANEL_BORDER_COLOR, panel, 2, border_radius=8)

    x = PANEL_LEFT + 20
    y = PANEL_TOP + 18
    draw_text(screen, title_font, "Museum Heist", x, y)
    y += 42
    y = draw_wrapped_text(screen, small_font, "Escape without being caught", x, y, MUTED_TEXT_COLOR, 22)

    draw_status_banner(screen, state, font)

    total_items = state.get("all_items_count", len(state["items"]))
    collected_items = len(state.get("collected_items", []))
    movements = len(state["movement_history"])
    max_movements = state.get("max_movements", 0)
    elapsed = state.get("elapsed_seconds", 0)
    time_limit = state.get("time_limit", 0)
    alarm = state.get("alarm", 0)
    max_alarm = state.get("max_alarm", 3)
    alarm_color = ALARM_COLOR
    if alarm >= max_alarm - 1:
        alarm_color = DANGER_COLOR

    y = PANEL_TOP + 164
    draw_metric(screen, font, "Status", state.get("status", "Playing"), x, y)
    y += 28
    draw_metric(screen, font, "Difficulty", state.get("difficulty", "Normal"), x, y)
    y += 28
    draw_metric(screen, font, "Score", state.get("score", 0), x, y)
    y += 28
    draw_metric(screen, font, "Items", f"{collected_items} / {total_items}", x, y)
    y += 28
    draw_metric(screen, font, "Moves", f"{movements} / {max_movements}", x, y)
    y += 28
    draw_metric(screen, font, "Time", f"{format_time(elapsed)} / {format_time(time_limit)}", x, y)
    y += 28
    draw_metric(screen, font, "Alarm", f"{alarm} / {max_alarm}", x, y, alarm_color)
    y += 28
    draw_metric(screen, font, "Route length", len(escape_route), x, y)
    y += 28
    draw_metric(screen, font, "Rank", state.get("rank", "In Progress"), x, y)
    y += 34
    draw_wrapped_text(screen, small_font, greedy_item_text(greedy_result), x, y, TEXT_COLOR, 22)

    if state.get("status", "Playing") == "Playing" and len(escape_route) == 0:
        draw_text(screen, small_font, "No safe route found", x, y + 48, DANGER_COLOR)

    y = PANEL_TOP + 584
    draw_text(screen, small_font, "Press H for tutorial", x, y, MUTED_TEXT_COLOR)
    y += 24
    draw_text(screen, small_font, "Press R to restart", x, y, MUTED_TEXT_COLOR)
    y += 24
    draw_text(screen, small_font, "Main Menu: M", x, y, MUTED_TEXT_COLOR)


def draw_board(screen, state, greedy_result, escape_route, pulse):
    rows = state["grid"]["rows"]
    cols = state["grid"]["cols"]

    draw_grid(screen, rows, cols)
    draw_vision_zones(screen, state)

    if state.get("status", "Playing") == "Playing":
        draw_route(screen, escape_route)

    draw_objects(screen, state, greedy_result["item"], pulse)


def draw_center_panel(screen, fonts, title, lines, banner_color):
    title_font, font, small_font = fonts
    rect = pygame.Rect(170, 120, WINDOW_WIDTH - 340, WINDOW_HEIGHT - 240)
    pygame.draw.rect(screen, PANEL_COLOR, rect, border_radius=10)
    pygame.draw.rect(screen, banner_color, rect, 3, border_radius=10)

    y = rect.y + 42
    title_label = title_font.render(title, True, TEXT_COLOR)
    screen.blit(title_label, (rect.centerx - title_label.get_width() // 2, y))
    y += 62

    for line in lines:
        label = font.render(line, True, MUTED_TEXT_COLOR)
        screen.blit(label, (rect.centerx - label.get_width() // 2, y))
        y += 34

    return small_font


def draw_start_screen(screen, fonts, selected_difficulty):
    screen.fill(BACKGROUND_COLOR)
    lines = [
        "Escape, collect items, avoid vision zones.",
        f"Selected difficulty: {selected_difficulty}",
        "Press 1 Easy, 2 Normal, 3 Hard",
        "Press Enter to start",
        "Press H for tutorial",
        "Main Menu: M",
    ]
    draw_center_panel(screen, fonts, "Museum Heist", lines, SELECTED_ITEM_COLOR)


def draw_tutorial_screen(screen, fonts):
    screen.fill(BACKGROUND_COLOR)
    title_font, font, small_font = fonts
    panel = pygame.Rect(90, 54, WINDOW_WIDTH - 180, WINDOW_HEIGHT - 108)
    pygame.draw.rect(screen, PANEL_COLOR, panel, border_radius=10)
    pygame.draw.rect(screen, SELECTED_ITEM_COLOR, panel, 3, border_radius=10)

    x = panel.x + 38
    y = panel.y + 28
    draw_text(screen, title_font, "Tutorial", x, y)
    y += 54
    draw_text(screen, font, "Objective", x, y)
    y += 30
    draw_text(screen, small_font, "Escape, collect items, avoid vision zones.", x, y, MUTED_TEXT_COLOR)

    y += 48
    draw_text(screen, font, "Controls", x, y)
    y += 30
    draw_text(screen, small_font, "Move: WASD / Arrows", x, y, MUTED_TEXT_COLOR)
    y += 24
    draw_text(screen, small_font, "Restart: R", x, y, MUTED_TEXT_COLOR)
    y += 24
    draw_text(screen, small_font, "Main Menu: M", x, y, MUTED_TEXT_COLOR)
    y += 24
    draw_text(screen, small_font, "Start: Enter", x, y, MUTED_TEXT_COLOR)

    y += 44
    draw_text(screen, font, "Objects", x, y)
    y += 30
    draw_legend_item(screen, small_font, x, y, PLAYER_COLOR, "Player", "circle")
    y += 24
    draw_legend_item(screen, small_font, x, y, EXIT_COLOR, "Exit")
    y += 24
    draw_legend_item(screen, small_font, x, y, WALL_COLOR, "Wall")
    y += 24
    draw_legend_item(screen, small_font, x, y, GUARD_COLOR, "Guard", "circle")
    y += 24
    draw_legend_item(screen, small_font, x, y, CAMERA_COLOR, "Camera", "triangle")
    y += 24
    draw_legend_item(screen, small_font, x, y, ITEM_COLOR, "Item")
    y += 24
    draw_legend_item(screen, small_font, x, y, (218, 96, 62), "Vision zone")
    y += 24
    draw_legend_item(screen, small_font, x, y, ROUTE_COLOR, "Backtracking route", "circle")
    y += 24
    draw_legend_item(screen, small_font, x, y, SELECTED_ITEM_COLOR, "Greedy selected item")

    x = panel.x + 430
    y = panel.y + 112
    draw_text(screen, font, "Rules", x, y)
    y += 32
    rules = [
        "Avoid vision zones.",
        "Collect items for score.",
        "Reach the exit to win.",
        "Alarm reaching the limit causes game over.",
        "Movement limit causes game over.",
        "Time limit causes game over.",
    ]

    for rule in rules:
        draw_text(screen, small_font, rule, x, y, MUTED_TEXT_COLOR)
        y += 28

    y += 30
    draw_text(screen, small_font, "Press B to go back", x, y, TEXT_COLOR)
    y += 26
    draw_text(screen, small_font, "Press Enter to start", x, y, TEXT_COLOR)
    y += 26
    draw_text(screen, small_font, "Main Menu: M", x, y, TEXT_COLOR)


def draw_end_screen(screen, fonts, state):
    screen.fill(BACKGROUND_COLOR)
    total_items = state.get("all_items_count", len(state["items"]))
    collected_items = len(state.get("collected_items", []))
    movements = len(state["movement_history"])
    max_movements = state.get("max_movements", 0)
    elapsed = state.get("elapsed_seconds", 0)
    time_limit = state.get("time_limit", 0)

    if state.get("escaped", False):
        title = "Escape successful!"
        banner = SUCCESS_COLOR
        reason = "You reached the exit."
    else:
        title = "Game Over!"
        banner = DANGER_COLOR
        reason = state.get("game_over_reason", "")

    lines = [
        reason,
        f"Difficulty: {state.get('difficulty', 'Normal')}",
        f"Final score: {state.get('score', 0)}",
        f"Collected items: {collected_items} / {total_items}",
        f"Movements: {movements} / {max_movements}",
        f"Time: {format_time(elapsed)} / {format_time(time_limit)}",
        f"Rank: {state.get('rank', 'In Progress')}",
        "Press R to restart",
        "Main Menu: M",
    ]
    draw_center_panel(screen, fonts, title, lines, banner)


def update_results(state):
    return choose_greedy_item(state), find_escape_route(state)


def return_to_main_menu(selected_difficulty):
    bridge.write_reset_input(selected_difficulty)
    bridge.run_engine()
    state = bridge.load_state()
    greedy_result, escape_route = update_results(state)
    start_ticks = pygame.time.get_ticks()
    last_tick_second = 0
    return state, greedy_result, escape_route, start_ticks, last_tick_second


def current_elapsed_seconds(start_ticks):
    return (pygame.time.get_ticks() - start_ticks) // 1000


def main():
    state = bridge.get_game_state()
    greedy_result, escape_route = update_results(state)
    screen_state = "start"
    previous_screen_state = "start"
    selected_difficulty = state.get("difficulty", "Normal")

    pygame.init()
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("Museum Heist")
    fonts = (
        pygame.font.Font(None, 42),
        pygame.font.Font(None, 28),
        pygame.font.Font(None, 23),
    )
    clock = pygame.time.Clock()
    start_ticks = pygame.time.get_ticks()
    last_tick_second = state.get("elapsed_seconds", 0)

    running = True
    while running:
        pulse = (pygame.time.get_ticks() // 180) % 4

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                direction = None

                if event.key == pygame.K_m:
                    selected_difficulty = state.get("difficulty", selected_difficulty)
                    state, greedy_result, escape_route, start_ticks, last_tick_second = return_to_main_menu(selected_difficulty)
                    screen_state = "start"
                    previous_screen_state = "start"
                elif screen_state == "start":
                    if event.key == pygame.K_1:
                        selected_difficulty = "Easy"
                        bridge.write_reset_input(selected_difficulty)
                        bridge.run_engine()
                        state = bridge.load_state()
                        greedy_result, escape_route = update_results(state)
                        start_ticks = pygame.time.get_ticks()
                        last_tick_second = 0
                    elif event.key == pygame.K_2:
                        selected_difficulty = "Normal"
                        bridge.write_reset_input(selected_difficulty)
                        bridge.run_engine()
                        state = bridge.load_state()
                        greedy_result, escape_route = update_results(state)
                        start_ticks = pygame.time.get_ticks()
                        last_tick_second = 0
                    elif event.key == pygame.K_3:
                        selected_difficulty = "Hard"
                        bridge.write_reset_input(selected_difficulty)
                        bridge.run_engine()
                        state = bridge.load_state()
                        greedy_result, escape_route = update_results(state)
                        start_ticks = pygame.time.get_ticks()
                        last_tick_second = 0
                    elif event.key == pygame.K_RETURN:
                        start_ticks = pygame.time.get_ticks() - state.get("elapsed_seconds", 0) * 1000
                        last_tick_second = state.get("elapsed_seconds", 0)
                        screen_state = "playing"
                    elif event.key == pygame.K_h:
                        previous_screen_state = "start"
                        screen_state = "tutorial"
                    elif event.key == pygame.K_r:
                        bridge.write_reset_input(selected_difficulty)
                        bridge.run_engine()
                        state = bridge.load_state()
                        greedy_result, escape_route = update_results(state)
                        start_ticks = pygame.time.get_ticks()
                        last_tick_second = 0
                elif screen_state == "tutorial":
                    if event.key == pygame.K_b:
                        screen_state = previous_screen_state
                    elif event.key == pygame.K_RETURN:
                        start_ticks = pygame.time.get_ticks() - state.get("elapsed_seconds", 0) * 1000
                        last_tick_second = state.get("elapsed_seconds", 0)
                        screen_state = "playing"
                    elif event.key == pygame.K_r:
                        bridge.write_reset_input(selected_difficulty)
                        bridge.run_engine()
                        state = bridge.load_state()
                        greedy_result, escape_route = update_results(state)
                        start_ticks = pygame.time.get_ticks()
                        last_tick_second = 0
                        screen_state = "playing"
                elif event.key == pygame.K_r:
                    selected_difficulty = state.get("difficulty", selected_difficulty)
                    bridge.write_reset_input(selected_difficulty)
                    bridge.run_engine()
                    state = bridge.load_state()
                    greedy_result, escape_route = update_results(state)
                    start_ticks = pygame.time.get_ticks()
                    last_tick_second = 0
                    screen_state = "playing"
                elif event.key == pygame.K_h:
                    previous_screen_state = screen_state
                    screen_state = "tutorial"
                elif state.get("escaped", False) or state.get("game_over", False):
                    direction = None
                elif event.key == pygame.K_UP or event.key == pygame.K_w:
                    direction = "up"
                elif event.key == pygame.K_DOWN or event.key == pygame.K_s:
                    direction = "down"
                elif event.key == pygame.K_LEFT or event.key == pygame.K_a:
                    direction = "left"
                elif event.key == pygame.K_RIGHT or event.key == pygame.K_d:
                    direction = "right"

                if direction is not None:
                    elapsed = current_elapsed_seconds(start_ticks)
                    state["elapsed_seconds"] = elapsed
                    bridge.write_input("move", direction, state, elapsed)
                    bridge.run_engine()
                    state = bridge.load_state()
                    greedy_result, escape_route = update_results(state)
                    last_tick_second = state.get("elapsed_seconds", elapsed)

                    if state.get("escaped", False):
                        screen_state = "escaped"
                    elif state.get("game_over", False):
                        screen_state = "caught"

        if screen_state == "playing" and state.get("status", "Playing") == "Playing":
            elapsed = current_elapsed_seconds(start_ticks)
            state["elapsed_seconds"] = elapsed

            if elapsed != last_tick_second:
                bridge.write_input("tick", "", state, elapsed)
                bridge.run_engine()
                state = bridge.load_state()
                greedy_result, escape_route = update_results(state)
                last_tick_second = state.get("elapsed_seconds", elapsed)

                if state.get("game_over", False):
                    screen_state = "caught"

        if screen_state == "start":
            draw_start_screen(screen, fonts, selected_difficulty)
        elif screen_state == "tutorial":
            draw_tutorial_screen(screen, fonts)
        elif screen_state == "escaped" or screen_state == "caught":
            draw_end_screen(screen, fonts, state)
        else:
            screen.fill(BACKGROUND_COLOR)
            draw_board(screen, state, greedy_result, escape_route, pulse)
            draw_sidebar(screen, fonts, state, greedy_result, escape_route)

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main()
