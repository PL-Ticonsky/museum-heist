#include "linked_list.h"
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

const int GRID_ROWS = 8;
const int GRID_COLS = 8;
const int ITEM_COUNT = 3;
const int MAX_ALARM = 3;
const int MAX_MOVEMENTS = 35;

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

bool get_bool_value(const std::string& text, const std::string& key, bool default_value) {
    std::string search_key = "\"" + key + "\"";
    int key_position = text.find(search_key);

    if (key_position == -1) {
        return default_value;
    }

    int colon_position = text.find(":", key_position);
    int value_start = colon_position + 1;

    while (value_start < (int)text.length() && text[value_start] == ' ') {
        value_start++;
    }

    if (text.substr(value_start, 4) == "true") {
        return true;
    }

    if (text.substr(value_start, 5) == "false") {
        return false;
    }

    return default_value;
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

void reset_game(int& player_row, int& player_col, int& step, MovementList& history, int& collected_count, int& score, bool& escaped) {
    player_row = 0;
    player_col = 0;
    step = 0;
    collected_count = 0;
    score = 0;
    escaped = false;
    history.clear();
    history.push_back(0, 0, 0);
}

int manhattan_distance(int a_row, int a_col, int b_row, int b_col) {
    int row_distance = a_row - b_row;
    int col_distance = a_col - b_col;

    if (row_distance < 0) {
        row_distance = -row_distance;
    }

    if (col_distance < 0) {
        col_distance = -col_distance;
    }

    return row_distance + col_distance;
}

bool is_wall_cell(int row, int col) {
    if (row == 1 && col == 3) {
        return true;
    }

    if (row == 2 && col == 3) {
        return true;
    }

    if (row == 3 && col == 3) {
        return true;
    }

    return false;
}

bool is_camera_cell(int row, int col) {
    if (row == 0 && col == 6) {
        return true;
    }

    return false;
}

bool is_guard_cell(int row, int col) {
    if (row == 4 && col == 4) {
        return true;
    }

    return false;
}

bool is_next_to_guard(int row, int col) {
    return manhattan_distance(row, col, 4, 4) == 1;
}

bool item_is_near_guard(const ItemData& item) {
    return manhattan_distance(item.row, item.col, 4, 4) <= 2;
}

bool is_valid_move(int row, int col) {
    if (row < 0 || row >= GRID_ROWS) {
        return false;
    }

    if (col < 0 || col >= GRID_COLS) {
        return false;
    }

    if (is_wall_cell(row, col) || is_camera_cell(row, col) || is_guard_cell(row, col)) {
        return false;
    }

    return true;
}

void check_alarm_limit(int alarm, bool& game_over, std::string& game_over_reason) {
    if (alarm >= MAX_ALARM && !game_over) {
        game_over = true;
        game_over_reason = "Alarm reached the maximum level.";
    }
}

void collect_item_if_needed(
    int player_row,
    int player_col,
    std::string collected_items[],
    int& collected_count,
    int& score,
    int& alarm
) {
    for (int index = 0; index < ITEM_COUNT; index++) {
        ItemData item = game_items[index];

        if (player_row == item.row && player_col == item.col && !item_is_collected(collected_items, collected_count, item.id)) {
            if (collected_count < ITEM_COUNT) {
                collected_items[collected_count] = item.id;
                collected_count++;
                score += item.value;

                if (item_is_near_guard(item)) {
                    alarm++;
                }
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
    int& score,
    bool& escaped,
    bool& game_over,
    int& alarm,
    std::string& game_over_reason
) {
    if (!file_exists(input_path)) {
        history.push_back(0, 0, 0);
        return;
    }

    std::string input = read_file(input_path);
    rebuild_movement_history(input, history);
    read_collected_items(input, collected_items, collected_count);
    score = get_int_value(input, "score", 0);
    escaped = get_bool_value(input, "escaped", false);
    game_over = get_bool_value(input, "game_over", false);
    alarm = get_int_value(input, "alarm", 0);
    game_over_reason = get_string_value(input, "game_over_reason");

    std::string action = get_string_value(input, "action");
    std::string direction = get_string_value(input, "direction");

    if (action == "reset") {
        reset_game(player_row, player_col, step, history, collected_count, score, escaped);
        game_over = false;
        alarm = 0;
        game_over_reason = "";
        return;
    }

    player_row = get_int_value(input, "row", player_row);
    player_col = get_int_value(input, "col", player_col);

    if (escaped || game_over) {
        return;
    }

    if (action != "move") {
        return;
    }

    int current_row = player_row;
    int current_col = player_col;
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

    bool next_cell_is_inside = next_row >= 0 && next_row < GRID_ROWS && next_col >= 0 && next_col < GRID_COLS;

    if (!next_cell_is_inside) {
        return;
    }

    if (is_wall_cell(next_row, next_col)) {
        alarm++;
        check_alarm_limit(alarm, game_over, game_over_reason);
        return;
    }

    if (is_camera_cell(next_row, next_col)) {
        game_over = true;
        game_over_reason = "A camera spotted the player.";
        return;
    }

    if (is_guard_cell(next_row, next_col)) {
        game_over = true;
        game_over_reason = "A guard caught the player.";
        return;
    }

    if (is_valid_move(next_row, next_col)) {
        player_row = next_row;
        player_col = next_col;
        history.push_back(player_row, player_col, step);

        if (is_next_to_guard(player_row, player_col)) {
            alarm++;
        }

        collect_item_if_needed(player_row, player_col, collected_items, collected_count, score, alarm);

        if (player_row == 7 && player_col == 7) {
            escaped = true;
        }

        check_alarm_limit(alarm, game_over, game_over_reason);

        if (!escaped && !game_over && history.get_size() >= MAX_MOVEMENTS) {
            game_over = true;
            game_over_reason = "Movement limit reached.";
        }
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
    int score,
    bool escaped,
    bool game_over,
    int alarm,
    const std::string& game_over_reason
) {
    ItemBST items;
    for (int index = 0; index < ITEM_COUNT; index++) {
        if (!item_is_collected(collected_items, collected_count, game_items[index].id)) {
            items.insert(game_items[index].id, game_items[index].row, game_items[index].col, game_items[index].value);
        }
    }

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
    out << "  \"game_over\": " << (game_over ? "true" : "false") << ",\n";
    out << "  \"game_over_reason\": \"" << game_over_reason << "\",\n";
    out << "  \"alarm\": " << alarm << ",\n";
    out << "  \"max_alarm\": " << MAX_ALARM << ",\n";
    out << "  \"max_movements\": " << MAX_MOVEMENTS << ",\n";

    if (escaped) {
        out << "  \"status\": \"Escaped\",\n";
    } else if (game_over) {
        out << "  \"status\": \"Caught\",\n";
    } else {
        out << "  \"status\": \"Playing\",\n";
    }

    out << "  \"score\": " << score << ",\n";
    out << "  \"all_items_count\": " << ITEM_COUNT << ",\n";
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
    out << "  \"items\": [";

    bool first_item = true;
    for (int index = 0; index < ITEM_COUNT; index++) {
        ItemData item = game_items[index];

        if (!item_is_collected(collected_items, collected_count, item.id)) {
            if (!first_item) {
                out << ",";
            }

            out << "\n";
            out << "    {\"id\": \"" << item.id
                << "\", \"row\": " << item.row
                << ", \"col\": " << item.col
                << ", \"value\": " << item.value << "}";
            first_item = false;
        }
    }

    if (!first_item) {
        out << "\n";
    }

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
    bool escaped = false;
    bool game_over = false;
    int alarm = 0;
    std::string game_over_reason = "";
    int collected_count = 0;
    std::string collected_items[ITEM_COUNT];
    MovementList history;

    apply_input(
        input_path,
        player_row,
        player_col,
        step,
        history,
        collected_items,
        collected_count,
        score,
        escaped,
        game_over,
        alarm,
        game_over_reason
    );
    write_state_file(
        state_path,
        input_found,
        player_row,
        player_col,
        history,
        collected_items,
        collected_count,
        score,
        escaped,
        game_over,
        alarm,
        game_over_reason
    );

    std::cout << "Museum Heist engine wrote " << state_path << "\n";
    return 0;
}
