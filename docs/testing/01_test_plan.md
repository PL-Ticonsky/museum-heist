# Test Plan

The following tests are based on the implemented behavior in the current repository.

| ID | Test | Steps | Expected Result |
| --- | --- | --- | --- |
| T01 | Compilation | Run the bridge compile command or start the UI. | `engine/museum_engine` is created without compiler errors. |
| T02 | Pygame launch | Run `python game/ui/main.py`. | The Museum Heist window opens on the start screen. |
| T03 | Movement | Start a game and press a valid movement key. | Player moves one cell and movement history increases. |
| T04 | Invalid movement outside grid | At the top-left start, press up or left. | Player stays in place and the game continues. |
| T05 | Invalid wall movement | Move toward a wall and attempt to enter it. | Player stays in place and alarm increases by 1. |
| T06 | Item collection | Move onto an item cell. | Score increases and the item id appears in `collected_items`. |
| T07 | Item disappearance | Collect an item and reload state. | The collected item no longer appears in `state.json` `items`. |
| T08 | Vision zone game over | Move into a camera or guard vision zone. | `game_over` becomes true and status becomes `Caught`. |
| T09 | Alarm game over | Increase alarm until it reaches `max_alarm`. | Game ends with alarm-limit reason. |
| T10 | Movement limit game over | Use valid moves until movement history reaches `max_movements` without escaping. | Game ends with movement-limit reason. |
| T11 | Time limit game over | Let elapsed time reach `time_limit` while playing. | Game ends with reason `Time expired`. |
| T12 | Escape success | Move to the exit cell safely. | `escaped` becomes true, status becomes `Escaped`, and final metrics appear. |
| T13 | Restart | Press `R`. | Current difficulty resets to start position with score, alarm, and time reset. |
| T14 | Main menu return | Press `M`. | Game resets and UI returns to the start screen. |
| T15 | Greedy output | Load a state with remaining items and call `choose_greedy_item(state)`. | Function returns an item and a reason, or `None` if no items remain. |
| T16 | Backtracking route | Load a playable state and call `find_escape_route(state)`. | Function returns a route to the exit or an empty list if no safe route exists. |
| T17 | JSON bridge | Write input, run engine, and load state through `bridge.py`. | `state.json` reflects the requested action and can be parsed by Python. |
