#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "heap.h"

typedef struct Node Node;
struct Node {
    Node* parent;
    Node* child1;
    Node* child2;
    int index;
};

typedef struct List_element List_element;
struct List_element {
    List_element* next;
    Node* node;
};

void free_node(Node* node) {
    if(node->child1 != NULL) free_node(node->child1);
    if(node->child2 != NULL) free_node(node->child2);
    free(node);
}

Node* create_node(int index) {
    Node* node = (Node*) malloc(sizeof(Node));
    if(node == NULL)
        return NULL;

    node->parent = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->index = index;
    return node;
}

bool is_leaf(Node* node) {
    return node->child1 == NULL && node->child2 == NULL ? true : false;
}

List_element* create_list_element(Node* node) {
    List_element* elt = (List_element*) malloc(sizeof(List_element));
    if(elt == NULL)
        return NULL;

    elt->node = node;
    elt->next = NULL;
    return elt;
}

List_element* add_leaf_node(List_element* start, Node* node) {
    List_element* elt = create_list_element(node);
    if(elt == NULL)
        return NULL;

    if(start == NULL)
        return elt;

    if(elt->node->index > start->node->index) {
        elt->next = start;
        return elt;
    }

    List_element* current = start;
    while(current->next != NULL && elt->node->index < current->next->node->index)
        current = current->next;

    if(current->next != NULL) {
        if(elt->node->index > current->next->node->index) {
            elt->next = current->next;
            current->next = elt;
        } else {
            current->next->next = elt;
            elt->next = NULL;
        }
    } else {
        current->next = elt;
        elt->next = NULL;
    }

    return start;
}

List_element* get_last_index_node(List_element* start, Node** last_index_node) {
    if(start != NULL) {
        *last_index_node = start->node;
        List_element* next = start->next;
        free(start);
        return next;
    }

    return NULL;
}

void free_leaf_list(List_element* start) {
    List_element* current = start;

    while(current != NULL) {
        List_element* next = current->next;
        free(current);
        current = next;
    }
}

Node* build_tree(List_element** leaf_list, int array_len, int i) {
    Node* node = create_node(i);
    if(node == NULL)
        return NULL;

    int child1_i = i * 2 + 1;
    int child2_i = i * 2 + 2;

    if(child1_i < array_len) {
        node->child1 = build_tree(leaf_list, array_len, child1_i);

        if(node->child1 == NULL) {
            free_node(node);
            return NULL;
        }

        node->child1->parent = node;
    }

    if(child2_i < array_len) {
        node->child2 = build_tree(leaf_list, array_len, child2_i);
        
        if(node->child2 == NULL) {
            free_node(node);
            return NULL;
        }

        node->child2->parent = node;
    }

    if(is_leaf(node)) {
        List_element* tmp = add_leaf_node(*leaf_list, node);

        if(tmp == NULL)
            return false;

        *leaf_list = tmp;
    }

    return node;
}

void build_max_tree(Data* data, Node* node) {
    int child1_i = -1, child2_i = -1;
    Node* max = NULL;

    if(is_leaf(node) || !run(data))
        return;

    if(node->child1 != NULL) {
        build_max_tree(data, node->child1);
        child1_i = node->child1->index;
        max = node->child1;
    }

    if(node->child2 != NULL) {
        build_max_tree(data, node->child2);
        child2_i = node->child2->index;

        if(max == NULL || data->array[node->child2->index] > data->array[max->index])
            max = node->child2;
    }

    if(!run(data))
        return;

    if(max != NULL && data->array[node->index] < data->array[max->index])
        swap(data, node->index, max->index);

    data->cursor = node->index;
    tick(data);
}

bool remove_node(Node* node, List_element** leaf_list) {
    Node* parent = node->parent;

    if(parent->child1 == node)
        parent->child1 = NULL;
    else
        parent->child2 = NULL;

    free_node(node);

    if(is_leaf(parent)) {
        List_element* tmp = add_leaf_node(*leaf_list, parent);

        if(tmp == NULL)
            return false;

        *leaf_list = tmp;
    }

    return true;
}

short run_heap_sort(Data* data) {
    List_element* leaf_list = NULL;
    Node* tree = build_tree(&leaf_list, data->array_len, 0);

    if(tree == NULL) {
        free_leaf_list(leaf_list);
        return SORT_FAILURE;
    }

    while(!is_leaf(tree) && run(data)) {
        build_max_tree(data, tree);

        if(!run(data))
            break;

        Node* last_node;
        leaf_list = get_last_index_node(leaf_list, &last_node);
        
        swap(data, tree->index, last_node->index);
        data->cursor = last_node->index;

        if(!remove_node(last_node, &leaf_list)) {
            free_leaf_list(leaf_list);
            free_node(tree);
            return SORT_FAILURE;
        }

        tick(data);
    }

    free_leaf_list(leaf_list);
    free_node(tree);
    return SORT_SUCCESS;
}