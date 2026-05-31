import pygame

import bridge


CELL_SIZE = 64
HUD_HEIGHT = 120

BACKGROUND_COLOR = (245, 242, 235)
HUD_COLOR = (230, 224, 212)
GRID_COLOR = (45, 45, 45)
PLAYER_COLOR = (40, 110, 210)
EXIT_COLOR = (40, 150, 80)
WALL_COLOR = (70, 70, 70)
CAMERA_COLOR = (210, 170, 40)
GUARD_COLOR = (190, 60, 60)
ITEM_COLOR = (145, 85, 190)
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


def draw_objects(screen, state):
    for wall in state["walls"]:
        draw_cell(screen, wall["row"], wall["col"], WALL_COLOR, 4)

    for camera in state["cameras"]:
        draw_cell(screen, camera["row"], camera["col"], CAMERA_COLOR, 14)

    for guard in state["guards"]:
        draw_cell(screen, guard["row"], guard["col"], GUARD_COLOR, 12)

    for item in state["items"]:
        draw_cell(screen, item["row"], item["col"], ITEM_COLOR, 18)

    exit_position = state["exit"]
    draw_cell(screen, exit_position["row"], exit_position["col"], EXIT_COLOR, 8)

    player = state["player"]
    draw_cell(screen, player["row"], player["col"], PLAYER_COLOR, 16)


def draw_hud(screen, font, state):
    player = state["player"]
    item_count = len(state["items"])
    movement_count = len(state["movement_history"])

    pygame.draw.rect(screen, HUD_COLOR, (0, 0, screen.get_width(), HUD_HEIGHT))

    lines = [
        "Museum Heist",
        f"Player position: row {player['row']}, col {player['col']}",
        f"Items: {item_count}",
        f"Movements: {movement_count}",
    ]

    for index, text in enumerate(lines):
        label = font.render(text, True, TEXT_COLOR)
        screen.blit(label, (20, 16 + index * 24))


def main():
    state = bridge.get_game_state()
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

        screen.fill(BACKGROUND_COLOR)
        draw_hud(screen, font, state)
        draw_grid(screen, rows, cols)
        draw_objects(screen, state)
        pygame.display.flip()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main()
