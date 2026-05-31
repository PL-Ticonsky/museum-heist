#include "linked_list.h"
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

const int GRID_ROWS = 8;
const int GRID_COLS = 8;

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

void apply_input(const std::string& input_path, int& player_row, int& player_col, int& step) {
    if (!file_exists(input_path)) {
        return;
    }

    std::string input = read_file(input_path);
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
    step = current_step + 1;

    if (is_valid_move(next_row, next_col)) {
        player_row = next_row;
        player_col = next_col;
    }
}

void write_state_file(const std::string& path, bool input_found, int player_row, int player_col, int step) {
    MovementList history;
    history.push_back(0, 0, 0);
    history.push_back(player_row, player_col, step);

    ItemBST items;
    items.insert("painting", 2, 5, 100);
    items.insert("vase", 5, 1, 60);
    items.insert("coin", 6, 6, 25);

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

    apply_input(input_path, player_row, player_col, step);
    write_state_file(state_path, input_found, player_row, player_col, step);

    std::cout << "Museum Heist engine wrote " << state_path << "\n";
    return 0;
}
