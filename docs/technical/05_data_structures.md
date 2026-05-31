# Data Structures

The project implements two required custom C++ data structures: a linked list for movement history and a binary search tree for item values. The required structures do not use `std::list`, `std::map`, or `std::set`.

## MovementList And MovementNode

`MovementNode` is defined in `engine/linked_list.h`:

```cpp
struct MovementNode {
    int row;
    int col;
    int step;
    MovementNode* next;
};
```

Each node stores one visited player position and the movement step number.

`MovementList` stores:

- `head`: first node;
- `tail`: last node;
- `size`: number of nodes.

The list supports:

- `push_back(row, col, step)`;
- `clear()`;
- `write_json_array(out)`;
- `get_size()`.

## Linked List Diagram

```text
MovementList
 head                                      tail
  |                                         |
  v                                         v
+--------+      +--------+      +--------+      null
| step 0 | ---> | step 1 | ---> | step 2 | --->
| row,col|      | row,col|      | row,col|
+--------+      +--------+      +--------+
```

## Why A Linked List Is Used For Movement History

Movement history grows one move at a time. The engine appends each valid position to the end and later traverses the sequence to write JSON. A linked list fits this behavior because each node points to the next movement and the `tail` pointer allows direct insertion at the end.

## ItemBST And ItemNode

`ItemNode` is defined in `engine/tree.h`:

```cpp
struct ItemNode {
    std::string id;
    int row;
    int col;
    int value;
    ItemNode* left;
    ItemNode* right;
};
```

`ItemBST` stores remaining items ordered by value. If two items have the same value, the id is used as a tie breaker.

The tree supports:

- `insert(id, row, col, value)`;
- `inorder_json(out)`;
- `clear()`;
- `get_size()`.

There is no implemented search function in the current `ItemBST` class.

## BST Diagram

```text
              value 60
             /        \
       value 25      value 100
                    /
               value 80
```

An inorder traversal writes items from smaller value to larger value.

## Why A BST Is Used For Item Values

The engine needs to output `items_sorted_by_value`. A binary search tree stores each remaining item according to its value, and inorder traversal naturally produces sorted output.

## Operation Complexity

### MovementList

| Operation | Complexity | Notes |
| --- | --- | --- |
| Insert at end | `O(1)` | `push_back()` uses the `tail` pointer. |
| Traversal | `O(n)` | `write_json_array()` visits each node once. |
| Clear | `O(n)` | `clear()` deletes each node. |
| Search | Not implemented | A search function is not present in the current class. |

### ItemBST

| Operation | Average Complexity | Worst Case | Notes |
| --- | --- | --- | --- |
| Insert | `O(log n)` | `O(n)` | Worst case occurs if the tree becomes unbalanced. |
| Traversal | `O(n)` | `O(n)` | `inorder_json()` visits every node once. |
| Clear | `O(n)` | `O(n)` | `clear()` deletes every node. |
| Search | Not implemented | Not implemented | A search method is not present in the current class. |

## Standard Library Containers

The required linked list and BST are manually implemented with nodes and pointers. The project does not use `std::list`, `std::map`, or `std::set` for these required structures.
