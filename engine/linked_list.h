#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iosfwd>

struct MovementNode {
    int row;
    int col;
    int step;
    MovementNode* next;
};

class MovementList {
private:
    MovementNode* head;
    MovementNode* tail;
    int size;

public:
    MovementList();
    ~MovementList();

    void push_back(int row, int col, int step);
    void clear();
    void write_json_array(std::ostream& out) const;
    int get_size() const;
};

#endif
