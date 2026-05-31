#include "linked_list.h"

#include <ostream>

MovementList::MovementList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

MovementList::~MovementList() {
    clear();
}

void MovementList::push_back(int row, int col, int step) {
    MovementNode* node = new MovementNode;
    node->row = row;
    node->col = col;
    node->step = step;
    node->next = nullptr;

    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }

    size++;
}

void MovementList::clear() {
    MovementNode* current = head;

    while (current != nullptr) {
        MovementNode* next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    tail = nullptr;
    size = 0;
}

void MovementList::write_json_array(std::ostream& out) const {
    out << "[";

    MovementNode* current = head;
    while (current != nullptr) {
        out << "{\"step\":" << current->step
            << ",\"row\":" << current->row
            << ",\"col\":" << current->col << "}";

        if (current->next != nullptr) {
            out << ",";
        }

        current = current->next;
    }

    out << "]";
}

int MovementList::get_size() const {
    return size;
}
