# Game Rules

This document summarizes the rules implemented by the current code.

## Movement

The player moves one cell at a time using arrow keys or WASD. The C++ engine validates each requested move.

- Moving outside the grid keeps the player in place.
- Moving into a wall keeps the player in place and increases alarm by 1.
- Moving into a camera, guard, or vision zone causes game over.
- A valid move updates the player position and appends the position to movement history.

## Item Collection

Items have an id, position, and value. When the player moves onto an uncollected item:

- the item id is added to `collected_items`;
- the score increases by the item value;
- the item disappears from the visible `items` array in the next state;
- if the item is near a guard, alarm increases by 1.

## Vision Zones

Cameras and guards create vision zones.

- Cameras look in one configured direction for a limited range.
- Guards look up, down, left, and right for a limited range.
- Walls block vision.
- Entering a vision zone causes immediate game over.

## Alarm

Alarm increases when:

- the player attempts to move into a wall;
- the player ends a move next to a guard;
- the player collects an item near a guard.

When alarm reaches the difficulty's `max_alarm`, the player is caught.

## Time Limit

The UI sends elapsed seconds to the engine. If the player has not escaped and time reaches the difficulty time limit, the engine ends the game with reason `Time expired`.

## Movement Limit

Movement history includes the starting position and valid moves. If the movement history size reaches the difficulty movement limit before escaping, the player is caught with reason `Movement limit reached.`

## Difficulty Levels

| Difficulty | Grid | Alarm Limit | Movement Limit | Time Limit |
| --- | --- | --- | --- | --- |
| Easy | 8x8 | 4 | 45 | 90 seconds |
| Normal | 8x8 | 3 | 35 | 60 seconds |
| Hard | 10x10 | 2 | 28 | 45 seconds |

## Win Condition

The player wins when a valid move places the player on the exit cell.

## Lose Conditions

The player loses when:

- the player moves into a camera, guard, or vision zone;
- alarm reaches the alarm limit;
- movement history reaches the movement limit before escape;
- elapsed time reaches the time limit before escape.

## Restart And Main Menu

Pressing `R` writes a reset input for the current difficulty and starts a fresh run.

Pressing `M` writes a reset input, reloads the map, and returns to the start screen so the player can choose a difficulty again.
