#include "tree.h"

#include <ostream>

ItemBST::ItemBST() {
    root = nullptr;
    size = 0;
}

ItemBST::~ItemBST() {
    clear();
}

ItemNode* ItemBST::insert_node(ItemNode* current, ItemNode* node) {
    if (current == nullptr) {
        return node;
    }

    bool goes_left = node->value < current->value;
    bool same_value_lower_id = node->value == current->value && node->id < current->id;

    if (goes_left || same_value_lower_id) {
        current->left = insert_node(current->left, node);
    } else {
        current->right = insert_node(current->right, node);
    }

    return current;
}

void ItemBST::insert(const std::string& id, int row, int col, int value) {
    ItemNode* node = new ItemNode;
    node->id = id;
    node->row = row;
    node->col = col;
    node->value = value;
    node->left = nullptr;
    node->right = nullptr;

    root = insert_node(root, node);
    size++;
}

void ItemBST::write_inorder(ItemNode* current, std::ostream& out, bool& first) const {
    if (current == nullptr) {
        return;
    }

    write_inorder(current->left, out, first);

    if (!first) {
        out << ",";
    }

    out << "{\"id\":\"" << current->id
        << "\",\"row\":" << current->row
        << ",\"col\":" << current->col
        << ",\"value\":" << current->value << "}";

    first = false;

    write_inorder(current->right, out, first);
}

void ItemBST::inorder_json(std::ostream& out) const {
    bool first = true;
    out << "[";
    write_inorder(root, out, first);
    out << "]";
}

void ItemBST::clear_node(ItemNode* current) {
    if (current == nullptr) {
        return;
    }

    clear_node(current->left);
    clear_node(current->right);
    delete current;
}

void ItemBST::clear() {
    clear_node(root);
    root = nullptr;
    size = 0;
}

int ItemBST::get_size() const {
    return size;
}
