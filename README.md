# Museum Heist

Museum Heist is a Computer Science I project using Python, Pygame, and a
standalone C++ engine. Python and C++ communicate through JSON files in
`game/data/`.

## Controls

- Arrow keys or WASD: move the player
- R: restart the game
- H: open the tutorial screen
- B: return from the tutorial screen
- 1: select Easy on the start screen
- 2: select Normal on the start screen
- 3: select Hard on the start screen
- Enter: start the game

Collected items disappear from the map. Reaching the exit ends the game and
shows the final score, collected item count, movement count, time, and rank.

## Difficulty

- Easy: 8x8 grid, alarm limit 4, movement limit 45, time limit 90 seconds.
- Normal: 8x8 grid, alarm limit 3, movement limit 35, time limit 60 seconds.
- Hard: 10x10 grid, alarm limit 2, movement limit 28, time limit 45 seconds.

The selected difficulty is sent to the C++ engine through `input.json`. The
engine chooses a predefined hardcoded map and writes it to `state.json`.

## Stealth Rules

- Trying to move into a wall keeps the player in place and raises the alarm by 1.
- Moving into a camera or guard cell causes game over immediately.
- Cameras see in one direction for a limited range.
- Guards see up, down, left, and right for a limited range.
- Walls block camera and guard vision.
- Entering a camera or guard vision zone causes game over immediately.
- Ending a move next to a guard raises the alarm by 1.
- Collecting an item near a guard raises the alarm by 1.
- If the alarm reaches the difficulty alarm limit, the player is caught.
- If movement count reaches the difficulty movement limit before escaping, the player is caught.
- If time reaches the difficulty time limit before escaping, the player is caught.
- Press R at any time to restart the game.

## Tutorial And Metrics

Press H to open the tutorial screen. It explains the objective, controls,
objects, vision zones, the greedy selected item, and the backtracking route.

During play, the sidebar shows compact metrics: status, difficulty, score,
items collected, moves used out of max movements, elapsed time out of the time
limit, alarm, selected item, and route length.

The final screen shows score, collected items, movements, elapsed time, and
rank.

## Ranking

- A: escaped with high score, low movement count, low time, and low alarm.
- B: escaped with acceptable score, movement count, and time.
- C: escaped with low score or many movements.
- Caught: the player did not escape.
