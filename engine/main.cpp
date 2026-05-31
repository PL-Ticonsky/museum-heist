#include "linked_list.h"
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

const int GRID_ROWS = 8;
const int GRID_COLS = 8;
const int ITEM_COUNT = 3;

struct ItemData {
    std::string id;
    int row;
    int col;
    int value;
};

ItemData game_items[ITEM_COUNT] = {
    {"painting", 2, 5, 100},
    {"vase", 5, 1, 60},
    {"coin", 6, 6, 25},
};

bool file_exists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    std::string content;
    std::string line;

    while (std::getline(file, line)) {
        content += line;
    }

    return content;
}

std::string get_string_value(const std::string& text, const std::string& key) {
    std::string search_key = "\"" + key + "\"";
    int key_position = text.find(search_key);

    if (key_position == -1) {
        return "";
    }

    int colon_position = text.find(":", key_position);
    int first_quote = text.find("\"", colon_position + 1);
    int second_quote = text.find("\"", first_quote + 1);

    if (colon_position == -1 || first_quote == -1 || second_quote == -1) {
        return "";
    }

    return text.substr(first_quote + 1, second_quote - first_quote - 1);
}

int get_int_value(const std::string& text, const std::string& key, int default_value) {
    std::string search_key = "\"" + key + "\"";
    int key_position = text.find(search_key);

    if (key_position == -1) {
        return default_value;
    }

    int colon_position = text.find(":", key_position);
    int number_start = colon_position + 1;

    while (number_start < (int)text.length() && text[number_start] == ' ') {
        number_start++;
    }

    int number_end = number_start;
    while (number_end < (int)text.length() && text[number_end] >= '0' && text[number_end] <= '9') {
        number_end++;
    }

    if (colon_position == -1 || number_start == number_end) {
        return default_value;
    }

    return std::stoi(text.substr(number_start, number_end - number_start));
}

int get_int_value_after(const std::string& text, const std::string& key, int start_position, int default_value) {
    std::string search_key = "\"" + key + "\"";
    int key_position = text.find(search_key, start_position);

    if (key_position == -1) {
        return default_value;
    }

    int colon_position = text.find(":", key_position);
    int number_start = colon_position + 1;

    while (number_start < (int)text.length() && text[number_start] == ' ') {
        number_start++;
    }

    int number_end = number_start;
    while (number_end < (int)text.length() && text[number_end] >= '0' && text[number_end] <= '9') {
        number_end++;
    }

    if (colon_position == -1 || number_start == number_end) {
        return default_value;
    }

    return std::stoi(text.substr(number_start, number_end - number_start));
}

void rebuild_movement_history(const std::string& input, MovementList& history) {
    int history_position = input.find("\"movement_history\"");

    if (history_position == -1) {
        history.push_back(0, 0, 0);
        return;
    }

    int array_start = input.find("[", history_position);
    int array_end = input.find("]", array_start);
    int search_position = array_start;
    bool found_history = false;

    while (search_position != -1 && search_position < array_end) {
        int object_start = input.find("{", search_position);

        if (object_start == -1 || object_start > array_end) {
            break;
        }

        int step = get_int_value_after(input, "step", object_start, 0);
        int row = get_int_value_after(input, "row", object_start, 0);
        int col = get_int_value_after(input, "col", object_start, 0);

        history.push_back(row, col, step);
        found_history = true;
        search_position = input.find("}", object_start);

        if (search_position != -1) {
            search_position++;
        }
    }

    if (!found_history) {
        history.push_back(0, 0, 0);
    }
}

void read_collected_items(const std::string& input, std::string collected_items[], int& collected_count) {
    collected_count = 0;
    int collected_position = input.find("\"collected_items\"");

    if (collected_position == -1) {
        return;
    }

    int array_start = input.find("[", collected_position);
    int array_end = input.find("]", array_start);
    int search_position = array_start;

    while (search_position != -1 && search_position < array_end && collected_count < ITEM_COUNT) {
        int first_quote = input.find("\"", search_position);

        if (first_quote == -1 || first_quote > array_end) {
            break;
        }

        int second_quote = input.find("\"", first_quote + 1);

        if (second_quote == -1 || second_quote > array_end) {
            break;
        }

        collected_items[collected_count] = input.substr(first_quote + 1, second_quote - first_quote - 1);
        collected_count++;
        search_position = second_quote + 1;
    }
}

bool item_is_collected(const std::string collected_items[], int collected_count, const std::string& item_id) {
    for (int index = 0; index < collected_count; index++) {
        if (collected_items[index] == item_id) {
            return true;
        }
    }

    return false;
}

bool is_blocked_cell(int row, int col) {
    if (row == 1 && col == 3) {
        return true;
    }

    if (row == 2 && col == 3) {
        return true;
    }

    if (row == 3 && col == 3) {
        return true;
    }

    if (row == 0 && col == 6) {
        return true;
    }

    if (row == 4 && col == 4) {
        return true;
    }

    return false;
}

bool is_valid_move(int row, int col) {
    if (row < 0 || row >= GRID_ROWS) {
        return false;
    }

    if (col < 0 || col >= GRID_COLS) {
        return false;
    }

    if (is_blocked_cell(row, col)) {
        return false;
    }

    return true;
}

void collect_item_if_needed(int player_row, int player_col, std::string collected_items[], int& collected_count, int& score) {
    for (int index = 0; index < ITEM_COUNT; index++) {
        ItemData item = game_items[index];

        if (player_row == item.row && player_col == item.col && !item_is_collected(collected_items, collected_count, item.id)) {
            if (collected_count < ITEM_COUNT) {
                collected_items[collected_count] = item.id;
                collected_count++;
                score += item.value;
            }
        }
    }
}

void apply_input(
    const std::string& input_path,
    int& player_row,
    int& player_col,
    int& step,
    MovementList& history,
    std::string collected_items[],
    int& collected_count,
    int& score
) {
    if (!file_exists(input_path)) {
        history.push_back(0, 0, 0);
        return;
    }

    std::string input = read_file(input_path);
    rebuild_movement_history(input, history);
    read_collected_items(input, collected_items, collected_count);
    score = get_int_value(input, "score", 0);

    std::string action = get_string_value(input, "action");
    std::string direction = get_string_value(input, "direction");

    if (action != "move") {
        return;
    }

    int current_row = get_int_value(input, "row", player_row);
    int current_col = get_int_value(input, "col", player_col);
    int current_step = get_int_value(input, "movement_step", step);

    int next_row = current_row;
    int next_col = current_col;

    if (direction == "up") {
        next_row--;
    } else if (direction == "down") {
        next_row++;
    } else if (direction == "left") {
        next_col--;
    } else if (direction == "right") {
        next_col++;
    }

    player_row = current_row;
    player_col = current_col;
    step = current_step;

    if (is_valid_move(next_row, next_col)) {
        player_row = next_row;
        player_col = next_col;
        history.push_back(player_row, player_col, step);
        collect_item_if_needed(player_row, player_col, collected_items, collected_count, score);
    }
}

void write_collected_items(std::ostream& out, const std::string collected_items[], int collected_count) {
    out << "[";

    for (int index = 0; index < collected_count; index++) {
        out << "\"" << collected_items[index] << "\"";

        if (index < collected_count - 1) {
            out << ", ";
        }
    }

    out << "]";
}

void write_state_file(
    const std::string& path,
    bool input_found,
    int player_row,
    int player_col,
    MovementList& history,
    const std::string collected_items[],
    int collected_count,
    int score
) {
    ItemBST items;
    items.insert(game_items[0].id, game_items[0].row, game_items[0].col, game_items[0].value);
    items.insert(game_items[1].id, game_items[1].row, game_items[1].col, game_items[1].value);
    items.insert(game_items[2].id, game_items[2].row, game_items[2].col, game_items[2].value);

    bool escaped = player_row == 7 && player_col == 7;

    std::ofstream out(path);
    if (!out) {
        std::cerr << "Could not write " << path << "\n";
        return;
    }

    out << "{\n";
    out << "  \"input_found\": " << (input_found ? "true" : "false") << ",\n";
    out << "  \"grid\": {\"rows\": 8, \"cols\": 8},\n";
    out << "  \"player\": {\"row\": " << player_row << ", \"col\": " << player_col << "},\n";
    out << "  \"exit\": {\"row\": 7, \"col\": 7},\n";
    out << "  \"escaped\": " << (escaped ? "true" : "false") << ",\n";
    out << "  \"score\": " << score << ",\n";
    out << "  \"collected_items\": ";
    write_collected_items(out, collected_items, collected_count);
    out << ",\n";
    out << "  \"walls\": [\n";
    out << "    {\"row\": 1, \"col\": 3},\n";
    out << "    {\"row\": 2, \"col\": 3},\n";
    out << "    {\"row\": 3, \"col\": 3}\n";
    out << "  ],\n";
    out << "  \"cameras\": [\n";
    out << "    {\"id\": \"camera_1\", \"row\": 0, \"col\": 6}\n";
    out << "  ],\n";
    out << "  \"guards\": [\n";
    out << "    {\"id\": \"guard_1\", \"row\": 4, \"col\": 4}\n";
    out << "  ],\n";
    out << "  \"items\": [\n";
    out << "    {\"id\": \"painting\", \"row\": 2, \"col\": 5, \"value\": 100},\n";
    out << "    {\"id\": \"vase\", \"row\": 5, \"col\": 1, \"value\": 60},\n";
    out << "    {\"id\": \"coin\", \"row\": 6, \"col\": 6, \"value\": 25}\n";
    out << "  ],\n";
    out << "  \"movement_history\": ";
    history.write_json_array(out);
    out << ",\n";
    out << "  \"items_sorted_by_value\": ";
    items.inorder_json(out);
    out << "\n";
    out << "}\n";
}

int main() {
    const std::string input_path = "game/data/input.json";
    const std::string state_path = "game/data/state.json";

    bool input_found = file_exists(input_path);
    int player_row = 0;
    int player_col = 0;
    int step = 0;
    int score = 0;
    int collected_count = 0;
    std::string collected_items[ITEM_COUNT];
    MovementList history;

    apply_input(input_path, player_row, player_col, step, history, collected_items, collected_count, score);
    write_state_file(state_path, input_found, player_row, player_col, history, collected_items, collected_count, score);

    std::cout << "Museum Heist engine wrote " << state_path << "\n";
    return 0;
}
