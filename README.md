# Museum Heist

Museum Heist is a Computer Science I project using Python, Pygame, and a
standalone C++ engine. Python and C++ communicate through JSON files in
`game/data/`.

## Controls

- Arrow keys or WASD: move the player
- R: restart the game

Collected items disappear from the map. Reaching the exit ends the game and
shows the final score, collected item count, movement count, and route length.

## Stealth Rules

- Trying to move into a wall keeps the player in place and raises the alarm by 1.
- Moving into a camera or guard cell causes game over immediately.
- Ending a move next to a guard raises the alarm by 1.
- Collecting an item near a guard raises the alarm by 1.
- If the alarm reaches 3, the player is caught.
- If movement count reaches 35 before escaping, the player is caught.
- Press R at any time to restart the game.
