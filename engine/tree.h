#ifndef TREE_H
#define TREE_H

#include <iosfwd>
#include <string>

struct ItemNode {
    std::string id;
    int row;
    int col;
    int value;
    ItemNode* left;
    ItemNode* right;
};

class ItemBST {
private:
    ItemNode* root;
    int size;

    ItemNode* insert_node(ItemNode* current, ItemNode* node);
    void clear_node(ItemNode* current);
    void write_inorder(ItemNode* current, std::ostream& out, bool& first) const;

public:
    ItemBST();
    ~ItemBST();

    void insert(const std::string& id, int row, int col, int value);
    void inorder_json(std::ostream& out) const;
    void clear();
    int get_size() const;
};

#endif
