import json
import subprocess
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
ENGINE_PATH = PROJECT_ROOT / "engine" / "museum_engine"
INPUT_PATH = PROJECT_ROOT / "game" / "data" / "input.json"
STATE_PATH = PROJECT_ROOT / "game" / "data" / "state.json"


def compile_engine():
    command = [
        "g++",
        "-std=c++17",
        "engine/main.cpp",
        "engine/linked_list.cpp",
        "engine/tree.cpp",
        "-o",
        "engine/museum_engine",
    ]
    subprocess.run(command, cwd=PROJECT_ROOT, check=True)


def run_engine():
    subprocess.run([str(ENGINE_PATH)], cwd=PROJECT_ROOT, check=True)


def load_state():
    with STATE_PATH.open("r", encoding="utf-8") as file:
        return json.load(file)


def write_input(action, direction, state, elapsed_seconds=None):
    player = state["player"]
    movement_step = len(state["movement_history"])
    escaped = state.get("escaped", False)
    game_over = state.get("game_over", False)
    difficulty = state.get("difficulty", "Normal")
    if elapsed_seconds is None:
        elapsed_seconds = state.get("elapsed_seconds", 0)

    if action == "move" and (escaped or game_over):
        action = "none"
        direction = ""

    input_data = {
        "action": action,
        "direction": direction,
        "difficulty": difficulty,
        "player": {
            "row": player["row"],
            "col": player["col"],
        },
        "movement_step": movement_step,
        "movement_history": state["movement_history"],
        "collected_items": state.get("collected_items", []),
        "score": state.get("score", 0),
        "escaped": escaped,
        "game_over": game_over,
        "game_over_reason": state.get("game_over_reason", ""),
        "alarm": state.get("alarm", 0),
        "elapsed_seconds": elapsed_seconds,
    }

    with INPUT_PATH.open("w", encoding="utf-8") as file:
        json.dump(input_data, file, indent=2)


def write_reset_input(difficulty="Normal"):
    input_data = {
        "action": "reset",
        "direction": "",
        "difficulty": difficulty,
        "player": {
            "row": 0,
            "col": 0,
        },
        "movement_step": 0,
        "movement_history": [],
        "collected_items": [],
        "score": 0,
        "escaped": False,
        "game_over": False,
        "game_over_reason": "",
        "alarm": 0,
        "elapsed_seconds": 0,
    }

    with INPUT_PATH.open("w", encoding="utf-8") as file:
        json.dump(input_data, file, indent=2)


def get_game_state():
    compile_engine()
    run_engine()
    return load_state()
