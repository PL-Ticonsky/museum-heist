# Test Results Template

Use this table to record manual or demonstrated test results.

NOTE: GIF evidence may require a few seconds to load on GitHub. If an animation is not displayed immediately, please refresh the page or wait for the content to finish loading.

| ID | Test | Expected Result | Observed Result | Pass/Fail | Evidence/Screenshot |
| --- | --- | --- | --- | --- | --- |
| T01 | Compilation | Engine compiles without errors. | Engine compiled successfully without warnings or errors. | PASS | <img width="1073" height="130" alt="image" src="https://github.com/user-attachments/assets/40214e43-d906-4cae-b09c-5a4cf164272e" />|
| T02 | Pygame launch | Start screen appears. |Start screen displayed correctly after launching the application. |PASS | <img width="1214" height="949" alt="image" src="https://github.com/user-attachments/assets/8beca623-be7a-42c5-b54b-2b0bd477fd83" />|
| T03 | Movement | Player moves one valid cell. | Player moved one cell to the selected valid position. | PASS | <img src="https://github.com/user-attachments/assets/9f9b46b0-b81b-41c7-a14f-f358779b0ee3" width="300">|
| T04 | Invalid movement outside grid | Player stays in place. | Player remained in the same position when attempting to leave the grid. | PASS | <img src="https://github.com/user-attachments/assets/3e4078a0-aa3d-4274-b6e8-823b05b177c2" width="300" />|
| T05 | Invalid wall movement | Player stays in place and alarm increases. | Player remained in place and alarm counter increased by one. | PASS | <img width="300" src="https://github.com/user-attachments/assets/b7bc1596-8d98-460a-a4d0-64369e75770c" />|
| T06 | Item collection | Score increases and item id is collected. | Score increased by the item value and item ID was added to collected items. | PASS | <img width="300" src="https://github.com/user-attachments/assets/bd23b815-8a58-407f-95ad-b0c5041f99a8" />|
| T07 | Item disappearance | Collected item disappears from visible items. | Collected item was removed from the visible items list. | PASS | <img width="300" src="https://github.com/user-attachments/assets/7f7a7b4b-150e-44e9-bd55-ac4d0de00f28" />|
| T08 | Vision zone game over | Status becomes `Caught`. | Game status changed to Caught and game-over screen was displayed. | PASS | <img width="300" src="https://github.com/user-attachments/assets/a2622dfd-654a-454d-8299-96bfd47fe889" />|
| T09 | Alarm game over | Alarm limit causes game over. | The game displays the game over screen for exceeding the alarm limit  | PASS |  |
| T10 | Movement limit game over | Movement limit causes game over. |  |  |  |
| T11 | Time limit game over | Time limit causes game over. |  |  |  |
| T12 | Escape success | Status becomes `Escaped`. |  |  |  |
| T13 | Restart | Current difficulty resets. |  |  |  |
| T14 | Main menu return | UI returns to start screen. |  |  |  |
| T15 | Greedy output | Greedy function returns expected item/reason. |  |  |  |
| T16 | Backtracking route | Route is returned or empty if blocked. |  |  |  |
| T17 | JSON bridge | Input and state files update correctly. |  |  |  |
