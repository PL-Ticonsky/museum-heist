# Poster Content

## Introduction

Museum Heist is a Computer Science I project that combines a playable Pygame interface with a C++ rule engine. The game uses a grid-based museum map where the player must avoid surveillance and escape.

## Goal

The goal is to collect valuable items, avoid guards, cameras, and vision zones, and reach the exit before the alarm, movement limit, or time limit ends the run.

## Proposed Solution

The project separates responsibilities:

- C++ processes game rules and writes official state.
- Python renders the UI and handles controls.
- JSON files connect both languages.
- Greedy and backtracking algorithms provide visual decision support.

## Architecture

```text
Pygame UI -> input.json -> C++ Engine -> state.json -> Pygame UI
                    |                         |
                    v                         v
              Player action            Algorithms and rendering
```

## Algorithms

Greedy algorithm:

- Selects one remaining item.
- Prioritizes high value and simple danger checks.
- Locally optimal, not globally guaranteed.

Backtracking algorithm:

- Searches from player to exit.
- Avoids walls, cameras, guards, and vision zones.
- Uses choose, explore, and unchoose recursion.

## Data Structures

Linked list:

- `MovementList` stores movement history.
- Appends valid positions and writes the route to JSON.

Binary search tree:

- `ItemBST` stores remaining items by value.
- Inorder traversal writes sorted item output.

## Results

The project implements:

- three difficulty levels;
- keyboard movement;
- item collection and score;
- camera and guard vision;
- alarm, movement, and time loss conditions;
- escape success state;
- final metrics and rank;
- visible greedy selection and backtracking route.

## Conclusions

Museum Heist demonstrates core programming concepts through a complete interactive game. It connects C++ and Python with JSON, applies greedy and backtracking algorithms, and uses custom linked list and BST structures in the engine.
