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


def write_input(action, direction, state):
    player = state["player"]
    movement_step = len(state["movement_history"])

    input_data = {
        "action": action,
        "direction": direction,
        "player": {
            "row": player["row"],
            "col": player["col"],
        },
        "movement_step": movement_step,
    }

    with INPUT_PATH.open("w", encoding="utf-8") as file:
        json.dump(input_data, file, indent=2)


def get_game_state():
    compile_engine()
    run_engine()
    return load_state()
