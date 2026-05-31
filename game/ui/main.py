import sys
from pathlib import Path

import pygame

import bridge


GAME_DIR = Path(__file__).resolve().parents[1]
sys.path.append(str(GAME_DIR))

from algorithms.backtracking import find_escape_route
from algorithms.greedy import choose_greedy_item


CELL_SIZE = 64
HUD_HEIGHT = 320

BACKGROUND_COLOR = (245, 242, 235)
HUD_COLOR = (230, 224, 212)
GRID_COLOR = (45, 45, 45)
ROUTE_COLOR = (60, 150, 170)
PLAYER_COLOR = (40, 110, 210)
EXIT_COLOR = (40, 150, 80)
WALL_COLOR = (70, 70, 70)
CAMERA_COLOR = (210, 170, 40)
GUARD_COLOR = (190, 60, 60)
ITEM_COLOR = (145, 85, 190)
SELECTED_ITEM_COLOR = (255, 215, 70)
TEXT_COLOR = (25, 25, 25)


def draw_grid(screen, rows, cols):
    for row in range(rows):
        for col in range(cols):
            rect = pygame.Rect(
                col * CELL_SIZE,
                HUD_HEIGHT + row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
            )
            pygame.draw.rect(screen, GRID_COLOR, rect, 1)


def draw_cell(screen, row, col, color, padding=10):
    rect = pygame.Rect(
        col * CELL_SIZE + padding,
        HUD_HEIGHT + row * CELL_SIZE + padding,
        CELL_SIZE - padding * 2,
        CELL_SIZE - padding * 2,
    )
    pygame.draw.rect(screen, color, rect)


def draw_route(screen, escape_route):
    for position in escape_route:
        center_x = position["col"] * CELL_SIZE + CELL_SIZE // 2
        center_y = HUD_HEIGHT + position["row"] * CELL_SIZE + CELL_SIZE // 2
        pygame.draw.circle(screen, ROUTE_COLOR, (center_x, center_y), 7)


def draw_objects(screen, state, selected_item):
    for wall in state["walls"]:
        draw_cell(screen, wall["row"], wall["col"], WALL_COLOR, 4)

    for camera in state["cameras"]:
        draw_cell(screen, camera["row"], camera["col"], CAMERA_COLOR, 14)

    for guard in state["guards"]:
        draw_cell(screen, guard["row"], guard["col"], GUARD_COLOR, 12)

    for item in state["items"]:
        draw_cell(screen, item["row"], item["col"], ITEM_COLOR, 18)

    if selected_item is not None:
        rect = pygame.Rect(
            selected_item["col"] * CELL_SIZE + 12,
            HUD_HEIGHT + selected_item["row"] * CELL_SIZE + 12,
            CELL_SIZE - 24,
            CELL_SIZE - 24,
        )
        pygame.draw.rect(screen, SELECTED_ITEM_COLOR, rect, 4)

    exit_position = state["exit"]
    draw_cell(screen, exit_position["row"], exit_position["col"], EXIT_COLOR, 8)

    player = state["player"]
    draw_cell(screen, player["row"], player["col"], PLAYER_COLOR, 16)


def draw_hud(screen, font, state, greedy_result, escape_route):
    player = state["player"]
    total_item_count = state.get("all_items_count", len(state["items"]))
    movement_count = len(state["movement_history"])
    score = state.get("score", 0)
    collected_count = len(state.get("collected_items", []))
    escaped = state.get("escaped", False)
    game_over = state.get("game_over", False)
    alarm = state.get("alarm", 0)
    max_alarm = state.get("max_alarm", 3)
    status = state.get("status", "Playing")
    selected_item = greedy_result["item"]

    if selected_item is None:
        greedy_text = "Selected item: none"
    else:
        greedy_text = (
            f"Selected item: {selected_item['id']} "
            f"(value {selected_item['value']})"
        )

    pygame.draw.rect(screen, HUD_COLOR, (0, 0, screen.get_width(), HUD_HEIGHT))

    lines = [
        "Museum Heist",
        f"Player position: row {player['row']}, col {player['col']}",
        f"Score: {score}",
        f"Collected items: {collected_count} / {total_item_count}",
        f"Movements: {movement_count}",
        f"Alarm: {alarm} / {max_alarm}",
        f"Status: {status}",
        greedy_text,
        f"Route length: {len(escape_route)}",
    ]

    if escaped:
        lines.append("Escape successful!")
        lines.append("Press R to restart")
    elif game_over:
        lines.append("Game Over!")
        lines.append(f"Reason: {state.get('game_over_reason', '')}")
        lines.append("Press R to restart")
    else:
        lines.append("Use arrows or WASD to move. Press R to restart.")

    for index, text in enumerate(lines):
        label = font.render(text, True, TEXT_COLOR)
        screen.blit(label, (20, 16 + index * 24))


def main():
    state = bridge.get_game_state()
    greedy_result = choose_greedy_item(state)
    escape_route = find_escape_route(state)

    rows = state["grid"]["rows"]
    cols = state["grid"]["cols"]
    window_width = cols * CELL_SIZE
    window_height = rows * CELL_SIZE + HUD_HEIGHT

    pygame.init()
    screen = pygame.display.set_mode((window_width, window_height))
    pygame.display.set_caption("Museum Heist")
    font = pygame.font.Font(None, 28)
    clock = pygame.time.Clock()

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                direction = None

                if event.key == pygame.K_r:
                    bridge.write_reset_input()
                    bridge.run_engine()
                    state = bridge.load_state()
                    greedy_result = choose_greedy_item(state)
                    escape_route = find_escape_route(state)
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
                    bridge.write_input("move", direction, state)
                    bridge.run_engine()
                    state = bridge.load_state()
                    greedy_result = choose_greedy_item(state)
                    escape_route = find_escape_route(state)

        screen.fill(BACKGROUND_COLOR)
        draw_hud(screen, font, state, greedy_result, escape_route)
        draw_grid(screen, rows, cols)
        if state.get("status", "Playing") == "Playing":
            draw_route(screen, escape_route)
        draw_objects(screen, state, greedy_result["item"])
        pygame.display.flip()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main()
