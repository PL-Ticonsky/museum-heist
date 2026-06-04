# Pygame UI

The Pygame UI is implemented in `game/ui/main.py`. It opens the game window, handles keyboard input, calls the JSON bridge, runs the algorithms, and renders the board, sidebar, tutorial, and final screens.

## Screen States

The UI uses the `screen_state` variable.

| State | Purpose |
| --- | --- |
| `start` | Main start screen with difficulty selection. |
| `tutorial` | Tutorial screen with objective, controls, objects, and rules. |
| `playing` | Active game board and sidebar. |
| `caught` | Final game over screen. |
| `escaped` | Final escape successful screen. |

## Start Screen

The start screen shows the title, selected difficulty, controls for difficulty selection, Enter to start, and H for tutorial. Keys 1, 2, and 3 reset the game with Easy, Normal, or Hard.

## Tutorial

The tutorial explains:

- objective;
- movement controls;
- restart and menu controls;
- objects on the board;
- vision zones;
- backtracking route;
- greedy selected item;
- alarm, movement, and time loss conditions.

## Playing State

During play, movement keys write a move action through the bridge, run the C++ engine, reload state, and recompute the greedy item and backtracking route.

The UI also updates elapsed time. When a new second is reached, it writes a `tick` action so the C++ engine can enforce the time limit.

## Game Over And Escape Screens

`draw_end_screen()` displays:

- game over or escape title;
- reason;
- difficulty;
- final score;
- collected item count;
- movement count;
- elapsed time;
- rank;
- restart and main menu options.

## Rendering Order

In `draw_board()`, the board is rendered in this order:

1. floor and grid;
2. vision zones;
3. backtracking route while playing;
4. walls;
5. items;
6. selected greedy item outline;
7. exit;
8. cameras;
9. guards;
10. player.

The HUD/sidebar is rendered separately by `draw_sidebar()`, after the board.

## Keyboard Controls

| Key | Behavior |
| --- | --- |
| Arrow keys or WASD | Move the player while playing. |
| `1` | Select Easy on the start screen. |
| `2` | Select Normal on the start screen. |
| `3` | Select Hard on the start screen. |
| Enter | Start the game from the start or tutorial screen. |
| `H` | Open the tutorial. |
| `B` | Return from tutorial to the previous screen. |
| `R` | Restart the current difficulty. |
| `M` | Return to the main menu and reset the current difficulty. |

## Timer And Final Metrics

The timer is based on `pygame.time.get_ticks()`. The UI calculates elapsed seconds and sends them to the engine. The engine writes the resulting elapsed time and time limit to `state.json`.

The sidebar displays:

- status;
- difficulty;
- score;
- collected items;
- movements;
- time;
- alarm;
- route length;
- rank;
- selected greedy item.

The final screen displays score, item count, movement count, elapsed time, and rank.
