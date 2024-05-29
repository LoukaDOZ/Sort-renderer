#include <stdlib.h>
#include "api.h"
#include "sorts.h"

#define IGNORED -1

static void tournament(Data* data, int* tree, int tree_len) {
    for (int i = data->array_len - 1; i > 0; i--) {
        int p1 = 2 * i;
        int p2 = p1 + 1;

        if (p2 < tree_len)
            tree[i] = tree[p2] == IGNORED || (tree[p1] != IGNORED && tree[p1] < tree[p2]) ? tree[p1] : tree[p2];
        else
            tree[i] = tree[p1];
    }
}

static void build_tree(Data* data, int* tree, int tree_len) {
    for(int i = 0; i < data->array_len && run(data); i++) {
        tree[data->array_len + i] = data->array[i];
        data->cursor = i;
        tick(data);
    }

    tournament(data, tree, tree_len);
}

static int get_winner(Data* data, int* tree, int tree_len) {
    int winner = tree[1];

    for(int i = data->array_len; i < tree_len && run(data); i++) {
        if(winner == tree[i]) {
            tree[i] = IGNORED;
            break;
        }
    }

    tournament(data, tree, tree_len);
    return winner;
}

short run_tournament_sort(Data* data) {
    int tree_len = data->array_len * 2;
    int tree[tree_len];
    
    build_tree(data, tree, tree_len);

    for(int i = 0; i < data->array_len && run(data); i++) {
        data->array[i] = get_winner(data, tree, tree_len);
        data->cursor = i;
        tick(data);
    }

    return SORT_SUCCESS;
}