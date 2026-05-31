#include "linked_list.h"
#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>

bool file_exists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

void write_state_file(const std::string& path, bool input_found) {
    MovementList history;
    history.push_back(0, 0, 0);

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
    out << "  \"player\": {\"row\": 0, \"col\": 0},\n";
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
    write_state_file(state_path, input_found);

    std::cout << "Museum Heist engine wrote " << state_path << "\n";
    return 0;
}
