# Test Results Template

Use this table to record manual or demonstrated test results.

| ID | Test | Expected Result | Observed Result | Pass/Fail | Evidence/Screenshot |
| --- | --- | --- | --- | --- | --- |
| T01 | Compilation | Engine compiles without errors. | Engine compiled successfully without warnings or errors. | PASS | <img width="1073" height="130" alt="image" src="https://github.com/user-attachments/assets/40214e43-d906-4cae-b09c-5a4cf164272e" />|
| T02 | Pygame launch | Start screen appears. |Start screen displayed correctly after launching the application. |PASS | <img width="1214" height="949" alt="image" src="https://github.com/user-attachments/assets/8beca623-be7a-42c5-b54b-2b0bd477fd83" />|
| T03 | Movement | Player moves one valid cell. | Player moved one cell to the selected valid position. | PASS | <img src="https://github.com/user-attachments/assets/9f9b46b0-b81b-41c7-a14f-f358779b0ee3" width="300">|
| T04 | Invalid movement outside grid | Player stays in place. |  |  |  |
| T05 | Invalid wall movement | Player stays in place and alarm increases. |  |  |  |
| T06 | Item collection | Score increases and item id is collected. |  |  |  |
| T07 | Item disappearance | Collected item disappears from visible items. |  |  |  |
| T08 | Vision zone game over | Status becomes `Caught`. |  |  |  |
| T09 | Alarm game over | Alarm limit causes game over. |  |  |  |
| T10 | Movement limit game over | Movement limit causes game over. |  |  |  |
| T11 | Time limit game over | Time limit causes game over. |  |  |  |
| T12 | Escape success | Status becomes `Escaped`. |  |  |  |
| T13 | Restart | Current difficulty resets. |  |  |  |
| T14 | Main menu return | UI returns to start screen. |  |  |  |
| T15 | Greedy output | Greedy function returns expected item/reason. |  |  |  |
| T16 | Backtracking route | Route is returned or empty if blocked. |  |  |  |
| T17 | JSON bridge | Input and state files update correctly. |  |  |  |
