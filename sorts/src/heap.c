#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "heap.h"

typedef struct Node Node;
struct Node {
    Node* parent;
    Node* left_child;
    Node* right_child;
    int index, explored_number;
    bool explored;
};

typedef struct Other_info {
    Node* tree;
    Node* current_node;
    Node* last_index_node;
    int explored_number;
} Other_info;

Node* create_node(int index) {
    Node* node = (Node*) malloc(sizeof(Node));
    if(node == NULL)
        return NULL;

    node->parent = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->index = index;
    node->explored_number = 0;
    node->explored = false;
    return node;
}

void free_node(Node* node) {
    if(node->left_child != NULL)
        free_node(node->left_child);

    if(node->right_child != NULL)
        free_node(node->right_child);

    free(node);
}

Node* build_tree(int array_len, int i, bool* success) {
    if(i >= array_len) {
        *success = true;
        return NULL;
    }

    bool child_success;
    Node* node = create_node(i);
    if(node == NULL) {
        *success = false;
        return NULL;
    }

    node->left_child = build_tree(array_len, i * 2 + 1, &child_success);
    if(!child_success) {
        *success = false;
        free_node(node);
        return NULL;
    }

    node->right_child = build_tree(array_len, i * 2 + 2, &child_success);
    if(!child_success) {
        *success = false;
        free_node(node);
        return NULL;
    }

    if(node->left_child != NULL)
        node->left_child->parent = node;

    if(node->right_child != NULL)
        node->right_child->parent = node;

    *success = true;
    return node;
}

Node* find_last_node_and_reset(Node* start) {
    Node* last = start;

    if(start->left_child != NULL) {
        Node* left = find_last_node_and_reset(start->left_child);
        if(left->index > last->index) last = left;
    }

    if(start->right_child != NULL) {
        Node* right = find_last_node_and_reset(start->right_child);
        if(right->index > last->index) last = right;
    }

    start->explored = false;
    return last;
}

void remove_node(Node* node) {
    Node* parent = node->parent;

    if(parent->left_child == node) {
        free_node(parent->left_child);
        parent->left_child = NULL;
    } else {
        free_node(parent->right_child);
        parent->right_child = NULL;
    }
}

short init_heap_sort(Sort_info* info) {
    bool success;
    Node* tree = build_tree(info->array_len, 0, &success);
    if(!success)
        return SORT_FAILURE;

    Other_info* other_info = (Other_info*) malloc(sizeof(Other_info));
    if(other_info == NULL) {
        free_node(tree);
        return SORT_FAILURE;
    }

    other_info->tree = tree;
    other_info->current_node = tree;
    other_info->last_index_node = tree;
    other_info->explored_number = 1;
    info->cursor = 0;
    info->other = (void*) other_info;
    return SORT_SUCCESS;
}

short heap_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    Node* current = other_info->current_node;

    if(other_info->tree->explored_number == other_info->explored_number && other_info->last_index_node->index == 0)
        return SORT_FINISHED;

    if(current == NULL) {
        swap(info, other_info->tree->index, other_info->last_index_node->index);
        remove_node(other_info->last_index_node);

        other_info->current_node = other_info->tree;
        other_info->last_index_node = other_info->tree;
        other_info->explored_number++;
        return SORT_SUCCESS;
    }

    if(current->index > other_info->last_index_node->index)
        other_info->last_index_node = current;

    if(current->left_child != NULL && current->left_child->explored_number != other_info->explored_number)
        other_info->current_node = current->left_child;
    else if(current->right_child != NULL && current->right_child->explored_number != other_info->explored_number)
        other_info->current_node = current->right_child;
    else {
        int c = info->array[current->index];
        int l = current->left_child != NULL ? info->array[current->left_child->index] : -1;
        int r = current->right_child != NULL ? info->array[current->right_child->index] : -1;

        if(l > c && l > r)
            swap(info, current->left_child->index, current->index);
        else if(r > c && r > l)
            swap(info, current->right_child->index, current->index);

        current->explored_number = other_info->explored_number;
        other_info->current_node = current->parent;
    }

    info->cursor = current->index;
    return SORT_SUCCESS;
}

void free_heap_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    free_node(other_info->tree);
    free(other_info);
}