#include "linked_list.h"
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

const int MAX_WALLS = 16;
const int MAX_CAMERAS = 3;
const int MAX_GUARDS = 3;
const int MAX_ITEMS = 5;

struct Position {
    int row;
    int col;
};

struct ItemData {
    std::string id;
    int row;
    int col;
    int value;
};

struct CameraData {
    std::string id;
    int row;
    int col;
    std::string direction;
    int range;
};

struct GuardData {
    std::string id;
    int row;
    int col;
    int vision_range;
};

struct MapData {
    std::string difficulty;
    int rows;
    int cols;
    Position start;
    Position exit;
    int max_alarm;
    int max_movements;
    int time_limit;
    int wall_count;
    int camera_count;
    int guard_count;
    int item_count;
    Position walls[MAX_WALLS];
    CameraData cameras[MAX_CAMERAS];
    GuardData guards[MAX_GUARDS];
    ItemData items[MAX_ITEMS];
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

void clear_map(MapData& map) {
    map.wall_count = 0;
    map.camera_count = 0;
    map.guard_count = 0;
    map.item_count = 0;
}

void add_wall(MapData& map, int row, int col) {
    map.walls[map.wall_count] = {row, col};
    map.wall_count++;
}

void add_camera(MapData& map, const std::string& id, int row, int col, const std::string& direction, int range) {
    map.cameras[map.camera_count] = {id, row, col, direction, range};
    map.camera_count++;
}

void add_guard(MapData& map, const std::string& id, int row, int col, int vision_range) {
    map.guards[map.guard_count] = {id, row, col, vision_range};
    map.guard_count++;
}

void add_item(MapData& map, const std::string& id, int row, int col, int value) {
    map.items[map.item_count] = {id, row, col, value};
    map.item_count++;
}

void load_easy_map(MapData& map) {
    clear_map(map);
    map.difficulty = "Easy";
    map.rows = 8;
    map.cols = 8;
    map.start = {0, 0};
    map.exit = {7, 7};
    map.max_alarm = 4;
    map.max_movements = 45;
    map.time_limit = 90;

    add_wall(map, 2, 3);
    add_wall(map, 3, 3);
    add_camera(map, "camera_1", 0, 6, "down", 2);
    add_guard(map, "guard_1", 5, 4, 1);
    add_item(map, "painting", 2, 5, 100);
    add_item(map, "vase", 5, 1, 60);
    add_item(map, "coin", 6, 6, 25);
}

void load_normal_map(MapData& map) {
    clear_map(map);
    map.difficulty = "Normal";
    map.rows = 8;
    map.cols = 8;
    map.start = {0, 0};
    map.exit = {7, 7};
    map.max_alarm = 3;
    map.max_movements = 35;
    map.time_limit = 60;

    add_wall(map, 1, 3);
    add_wall(map, 2, 3);
    add_wall(map, 3, 3);
    add_camera(map, "camera_1", 0, 6, "down", 3);
    add_guard(map, "guard_1", 4, 4, 2);
    add_item(map, "painting", 2, 5, 100);
    add_item(map, "vase", 5, 1, 60);
    add_item(map, "coin", 6, 6, 25);
}

void load_hard_map(MapData& map) {
    clear_map(map);
    map.difficulty = "Hard";
    map.rows = 10;
    map.cols = 10;
    map.start = {0, 0};
    map.exit = {9, 9};
    map.max_alarm = 2;
    map.max_movements = 28;
    map.time_limit = 45;

    add_wall(map, 1, 3);
    add_wall(map, 2, 3);
    add_wall(map, 3, 3);
    add_wall(map, 5, 5);
    add_wall(map, 5, 6);
    add_wall(map, 6, 5);
    add_wall(map, 7, 2);
    add_wall(map, 7, 3);
    add_camera(map, "camera_1", 0, 7, "down", 3);
    add_camera(map, "camera_2", 8, 8, "left", 3);
    add_guard(map, "guard_1", 4, 4, 2);
    add_guard(map, "guard_2", 7, 7, 1);
    add_item(map, "painting", 2, 6, 100);
    add_item(map, "vase", 6, 1, 60);
    add_item(map, "coin", 8, 3, 25);
    add_item(map, "mask", 1, 8, 80);
}

void load_map_for_difficulty(const std::string& difficulty, MapData& map) {
    if (difficulty == "Easy") {
        load_easy_map(map);
    } else if (difficulty == "Hard") {
        load_hard_map(map);
    } else {
        load_normal_map(map);
    }
}

void rebuild_movement_history(const std::string& input, MovementList& history, const MapData& map) {
    int history_position = input.find("\"movement_history\"");

    if (history_position == -1) {
        history.push_back(map.start.row, map.start.col, 0);
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
        int row = get_int_value_after(input, "row", object_start, map.start.row);
        int col = get_int_value_after(input, "col", object_start, map.start.col);

        history.push_back(row, col, step);
        found_history = true;
        search_position = input.find("}", object_start);

        if (search_position != -1) {
            search_position++;
        }
    }

    if (!found_history) {
        history.push_back(map.start.row, map.start.col, 0);
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

    while (search_position != -1 && search_position < array_end && collected_count < MAX_ITEMS) {
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

void reset_game(
    const MapData& map,
    int& player_row,
    int& player_col,
    int& step,
    MovementList& history,
    int& collected_count,
    int& score,
    bool& escaped,
    bool& game_over,
    int& alarm,
    std::string& game_over_reason
) {
    player_row = map.start.row;
    player_col = map.start.col;
    step = 0;
    collected_count = 0;
    score = 0;
    escaped = false;
    game_over = false;
    alarm = 0;
    game_over_reason = "";
    history.clear();
    history.push_back(player_row, player_col, 0);
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

bool is_wall_cell(const MapData& map, int row, int col) {
    for (int index = 0; index < map.wall_count; index++) {
        if (map.walls[index].row == row && map.walls[index].col == col) {
            return true;
        }
    }

    return false;
}

bool is_camera_cell(const MapData& map, int row, int col) {
    for (int index = 0; index < map.camera_count; index++) {
        if (map.cameras[index].row == row && map.cameras[index].col == col) {
            return true;
        }
    }

    return false;
}

bool is_guard_cell(const MapData& map, int row, int col) {
    for (int index = 0; index < map.guard_count; index++) {
        if (map.guards[index].row == row && map.guards[index].col == col) {
            return true;
        }
    }

    return false;
}

bool is_next_to_guard(const MapData& map, int row, int col) {
    for (int index = 0; index < map.guard_count; index++) {
        if (manhattan_distance(row, col, map.guards[index].row, map.guards[index].col) == 1) {
            return true;
        }
    }

    return false;
}

bool item_is_near_guard(const MapData& map, const ItemData& item) {
    for (int index = 0; index < map.guard_count; index++) {
        if (manhattan_distance(item.row, item.col, map.guards[index].row, map.guards[index].col) <= 2) {
            return true;
        }
    }

    return false;
}

void get_direction_change(const std::string& direction, int& row_change, int& col_change) {
    row_change = 0;
    col_change = 0;

    if (direction == "up") {
        row_change = -1;
    } else if (direction == "down") {
        row_change = 1;
    } else if (direction == "left") {
        col_change = -1;
    } else if (direction == "right") {
        col_change = 1;
    }
}

bool camera_sees_cell(const MapData& map, int row, int col) {
    for (int camera_index = 0; camera_index < map.camera_count; camera_index++) {
        CameraData camera = map.cameras[camera_index];

        if (camera.row == row && camera.col == col) {
            return true;
        }

        int row_change;
        int col_change;
        get_direction_change(camera.direction, row_change, col_change);

        int vision_row = camera.row;
        int vision_col = camera.col;

        for (int step = 1; step <= camera.range; step++) {
            vision_row += row_change;
            vision_col += col_change;

            if (vision_row < 0 || vision_row >= map.rows || vision_col < 0 || vision_col >= map.cols) {
                break;
            }

            if (is_wall_cell(map, vision_row, vision_col)) {
                break;
            }

            if (vision_row == row && vision_col == col) {
                return true;
            }
        }
    }

    return false;
}

bool guard_sees_cell(const MapData& map, int row, int col) {
    std::string directions[4] = {"up", "down", "left", "right"};

    for (int guard_index = 0; guard_index < map.guard_count; guard_index++) {
        GuardData guard = map.guards[guard_index];

        if (guard.row == row && guard.col == col) {
            return true;
        }

        for (int direction_index = 0; direction_index < 4; direction_index++) {
            int row_change;
            int col_change;
            get_direction_change(directions[direction_index], row_change, col_change);

            int vision_row = guard.row;
            int vision_col = guard.col;

            for (int step = 1; step <= guard.vision_range; step++) {
                vision_row += row_change;
                vision_col += col_change;

                if (vision_row < 0 || vision_row >= map.rows || vision_col < 0 || vision_col >= map.cols) {
                    break;
                }

                if (is_wall_cell(map, vision_row, vision_col)) {
                    break;
                }

                if (vision_row == row && vision_col == col) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool is_vision_cell(const MapData& map, int row, int col) {
    if (is_wall_cell(map, row, col)) {
        return false;
    }

    return camera_sees_cell(map, row, col) || guard_sees_cell(map, row, col);
}

bool is_valid_move(const MapData& map, int row, int col) {
    if (row < 0 || row >= map.rows) {
        return false;
    }

    if (col < 0 || col >= map.cols) {
        return false;
    }

    if (is_wall_cell(map, row, col) || is_camera_cell(map, row, col) || is_guard_cell(map, row, col) || is_vision_cell(map, row, col)) {
        return false;
    }

    return true;
}

void check_alarm_limit(int alarm, int max_alarm, bool& game_over, std::string& game_over_reason) {
    if (alarm >= max_alarm && !game_over) {
        game_over = true;
        game_over_reason = "Alarm reached the maximum level.";
    }
}

void collect_item_if_needed(
    const MapData& map,
    int player_row,
    int player_col,
    std::string collected_items[],
    int& collected_count,
    int& score,
    int& alarm
) {
    for (int index = 0; index < map.item_count; index++) {
        ItemData item = map.items[index];

        if (player_row == item.row && player_col == item.col && !item_is_collected(collected_items, collected_count, item.id)) {
            if (collected_count < MAX_ITEMS) {
                collected_items[collected_count] = item.id;
                collected_count++;
                score += item.value;

                if (item_is_near_guard(map, item)) {
                    alarm++;
                }
            }
        }
    }
}

int total_item_value(const MapData& map) {
    int total = 0;

    for (int index = 0; index < map.item_count; index++) {
        total += map.items[index].value;
    }

    return total;
}

std::string calculate_rank(const MapData& map, bool escaped, bool game_over, int score, int movement_count, int elapsed_seconds, int alarm) {
    if (game_over && !escaped) {
        return "Caught";
    }

    if (!escaped) {
        return "In Progress";
    }

    int total_value = total_item_value(map);

    if (score >= total_value * 7 / 10 && movement_count <= map.max_movements / 2 && elapsed_seconds <= map.time_limit / 2 && alarm <= 1) {
        return "A";
    }

    if (score >= total_value * 4 / 10 && movement_count <= map.max_movements && elapsed_seconds <= map.time_limit) {
        return "B";
    }

    return "C";
}

void apply_input(
    const std::string& input_path,
    const MapData& map,
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
    std::string& game_over_reason,
    int& elapsed_seconds
) {
    if (!file_exists(input_path)) {
        history.push_back(map.start.row, map.start.col, 0);
        player_row = map.start.row;
        player_col = map.start.col;
        return;
    }

    std::string input = read_file(input_path);
    rebuild_movement_history(input, history, map);
    read_collected_items(input, collected_items, collected_count);
    score = get_int_value(input, "score", 0);
    escaped = get_bool_value(input, "escaped", false);
    game_over = get_bool_value(input, "game_over", false);
    alarm = get_int_value(input, "alarm", 0);
    elapsed_seconds = get_int_value(input, "elapsed_seconds", 0);
    game_over_reason = get_string_value(input, "game_over_reason");

    std::string action = get_string_value(input, "action");
    std::string direction = get_string_value(input, "direction");

    if (action == "reset") {
        reset_game(map, player_row, player_col, step, history, collected_count, score, escaped, game_over, alarm, game_over_reason);
        elapsed_seconds = 0;
        return;
    }

    player_row = get_int_value(input, "row", map.start.row);
    player_col = get_int_value(input, "col", map.start.col);

    if (!escaped && !game_over && elapsed_seconds >= map.time_limit) {
        game_over = true;
        game_over_reason = "Time expired";
        return;
    }

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

    bool next_cell_is_inside = next_row >= 0 && next_row < map.rows && next_col >= 0 && next_col < map.cols;

    if (!next_cell_is_inside) {
        return;
    }

    if (is_wall_cell(map, next_row, next_col)) {
        alarm++;
        check_alarm_limit(alarm, map.max_alarm, game_over, game_over_reason);
        return;
    }

    if (is_camera_cell(map, next_row, next_col) || is_guard_cell(map, next_row, next_col) || is_vision_cell(map, next_row, next_col)) {
        game_over = true;
        game_over_reason = "Caught in vision zone";
        return;
    }

    if (is_valid_move(map, next_row, next_col)) {
        player_row = next_row;
        player_col = next_col;
        history.push_back(player_row, player_col, step);

        if (is_next_to_guard(map, player_row, player_col)) {
            alarm++;
        }

        collect_item_if_needed(map, player_row, player_col, collected_items, collected_count, score, alarm);

        if (player_row == map.exit.row && player_col == map.exit.col) {
            escaped = true;
        }

        check_alarm_limit(alarm, map.max_alarm, game_over, game_over_reason);

        if (!escaped && !game_over && history.get_size() >= map.max_movements) {
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

void write_vision_cell(
    std::ostream& out,
    bool& first_cell,
    const std::string& source,
    const std::string& type,
    int row,
    int col
) {
    if (!first_cell) {
        out << ",";
    }

    out << "\n";
    out << "    {\"source\": \"" << source
        << "\", \"type\": \"" << type
        << "\", \"row\": " << row
        << ", \"col\": " << col << "}";
    first_cell = false;
}

void write_camera_vision(const MapData& map, std::ostream& out, bool& first_cell) {
    for (int camera_index = 0; camera_index < map.camera_count; camera_index++) {
        CameraData camera = map.cameras[camera_index];
        int row_change;
        int col_change;
        get_direction_change(camera.direction, row_change, col_change);

        int vision_row = camera.row;
        int vision_col = camera.col;

        for (int step = 1; step <= camera.range; step++) {
            vision_row += row_change;
            vision_col += col_change;

            if (vision_row < 0 || vision_row >= map.rows || vision_col < 0 || vision_col >= map.cols) {
                break;
            }

            if (is_wall_cell(map, vision_row, vision_col)) {
                break;
            }

            write_vision_cell(out, first_cell, camera.id, "camera", vision_row, vision_col);
        }
    }
}

void write_guard_vision(const MapData& map, std::ostream& out, bool& first_cell) {
    std::string directions[4] = {"up", "down", "left", "right"};

    for (int guard_index = 0; guard_index < map.guard_count; guard_index++) {
        GuardData guard = map.guards[guard_index];

        for (int direction_index = 0; direction_index < 4; direction_index++) {
            int row_change;
            int col_change;
            get_direction_change(directions[direction_index], row_change, col_change);

            int vision_row = guard.row;
            int vision_col = guard.col;

            for (int step = 1; step <= guard.vision_range; step++) {
                vision_row += row_change;
                vision_col += col_change;

                if (vision_row < 0 || vision_row >= map.rows || vision_col < 0 || vision_col >= map.cols) {
                    break;
                }

                if (is_wall_cell(map, vision_row, vision_col)) {
                    break;
                }

                write_vision_cell(out, first_cell, guard.id, "guard", vision_row, vision_col);
            }
        }
    }
}

void write_vision_zones(const MapData& map, std::ostream& out) {
    bool first_cell = true;

    out << "[";
    write_camera_vision(map, out, first_cell);
    write_guard_vision(map, out, first_cell);

    if (!first_cell) {
        out << "\n";
    }

    out << "]";
}

void write_walls(const MapData& map, std::ostream& out) {
    out << "[";

    for (int index = 0; index < map.wall_count; index++) {
        if (index > 0) {
            out << ",";
        }

        out << "\n";
        out << "    {\"row\": " << map.walls[index].row << ", \"col\": " << map.walls[index].col << "}";
    }

    if (map.wall_count > 0) {
        out << "\n";
    }

    out << "]";
}

void write_cameras(const MapData& map, std::ostream& out) {
    out << "[";

    for (int index = 0; index < map.camera_count; index++) {
        CameraData camera = map.cameras[index];

        if (index > 0) {
            out << ",";
        }

        out << "\n";
        out << "    {\"id\": \"" << camera.id
            << "\", \"row\": " << camera.row
            << ", \"col\": " << camera.col
            << ", \"direction\": \"" << camera.direction
            << "\", \"range\": " << camera.range << "}";
    }

    if (map.camera_count > 0) {
        out << "\n";
    }

    out << "]";
}

void write_guards(const MapData& map, std::ostream& out) {
    out << "[";

    for (int index = 0; index < map.guard_count; index++) {
        GuardData guard = map.guards[index];

        if (index > 0) {
            out << ",";
        }

        out << "\n";
        out << "    {\"id\": \"" << guard.id
            << "\", \"row\": " << guard.row
            << ", \"col\": " << guard.col
            << ", \"vision_range\": " << guard.vision_range << "}";
    }

    if (map.guard_count > 0) {
        out << "\n";
    }

    out << "]";
}

void write_visible_items(const MapData& map, std::ostream& out, const std::string collected_items[], int collected_count) {
    bool first_item = true;
    out << "[";

    for (int index = 0; index < map.item_count; index++) {
        ItemData item = map.items[index];

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

    out << "]";
}

void write_state_file(
    const std::string& path,
    bool input_found,
    const MapData& map,
    int player_row,
    int player_col,
    MovementList& history,
    const std::string collected_items[],
    int collected_count,
    int score,
    bool escaped,
    bool game_over,
    int alarm,
    const std::string& game_over_reason,
    int elapsed_seconds
) {
    ItemBST items;
    for (int index = 0; index < map.item_count; index++) {
        if (!item_is_collected(collected_items, collected_count, map.items[index].id)) {
            items.insert(map.items[index].id, map.items[index].row, map.items[index].col, map.items[index].value);
        }
    }

    std::string status = "Playing";
    if (escaped) {
        status = "Escaped";
    } else if (game_over) {
        status = "Caught";
    }

    std::string rank = calculate_rank(map, escaped, game_over, score, history.get_size(), elapsed_seconds, alarm);

    std::ofstream out(path);
    if (!out) {
        std::cerr << "Could not write " << path << "\n";
        return;
    }

    out << "{\n";
    out << "  \"input_found\": " << (input_found ? "true" : "false") << ",\n";
    out << "  \"difficulty\": \"" << map.difficulty << "\",\n";
    out << "  \"grid\": {\"rows\": " << map.rows << ", \"cols\": " << map.cols << "},\n";
    out << "  \"player\": {\"row\": " << player_row << ", \"col\": " << player_col << "},\n";
    out << "  \"exit\": {\"row\": " << map.exit.row << ", \"col\": " << map.exit.col << "},\n";
    out << "  \"escaped\": " << (escaped ? "true" : "false") << ",\n";
    out << "  \"game_over\": " << (game_over ? "true" : "false") << ",\n";
    out << "  \"game_over_reason\": \"" << game_over_reason << "\",\n";
    out << "  \"alarm\": " << alarm << ",\n";
    out << "  \"max_alarm\": " << map.max_alarm << ",\n";
    out << "  \"max_movements\": " << map.max_movements << ",\n";
    out << "  \"elapsed_seconds\": " << elapsed_seconds << ",\n";
    out << "  \"time_limit\": " << map.time_limit << ",\n";
    out << "  \"status\": \"" << status << "\",\n";
    out << "  \"rank\": \"" << rank << "\",\n";
    out << "  \"score\": " << score << ",\n";
    out << "  \"all_items_count\": " << map.item_count << ",\n";
    out << "  \"collected_items\": ";
    write_collected_items(out, collected_items, collected_count);
    out << ",\n";
    out << "  \"walls\": ";
    write_walls(map, out);
    out << ",\n";
    out << "  \"cameras\": ";
    write_cameras(map, out);
    out << ",\n";
    out << "  \"guards\": ";
    write_guards(map, out);
    out << ",\n";
    out << "  \"vision_zones\": ";
    write_vision_zones(map, out);
    out << ",\n";
    out << "  \"items\": ";
    write_visible_items(map, out, collected_items, collected_count);
    out << ",\n";
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
    std::string input = "";
    if (input_found) {
        input = read_file(input_path);
    }

    std::string difficulty = get_string_value(input, "difficulty");
    if (difficulty == "") {
        difficulty = "Normal";
    }

    MapData map;
    load_map_for_difficulty(difficulty, map);

    int player_row = map.start.row;
    int player_col = map.start.col;
    int step = 0;
    int score = 0;
    bool escaped = false;
    bool game_over = false;
    int alarm = 0;
    int elapsed_seconds = 0;
    std::string game_over_reason = "";
    int collected_count = 0;
    std::string collected_items[MAX_ITEMS];
    MovementList history;

    apply_input(
        input_path,
        map,
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
        game_over_reason,
        elapsed_seconds
    );
    write_state_file(
        state_path,
        input_found,
        map,
        player_row,
        player_col,
        history,
        collected_items,
        collected_count,
        score,
        escaped,
        game_over,
        alarm,
        game_over_reason,
        elapsed_seconds
    );

    std::cout << "Museum Heist engine wrote " << state_path << "\n";
    return 0;
}
