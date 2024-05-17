#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

typedef struct Card Card;
struct Card {
    Card* next;
    int value;
};

typedef struct Pile Pile;
struct Pile {
    Pile* next;
    Card* cards;
};

static void free_cards(Card* start) {
    while(start != NULL) {
        Card* tmp = start->next;
        free(start);
        start = tmp;
    }
}

static void free_piles(Pile* start) {
    while(start != NULL) {
        Pile* tmp = start->next;
        free_cards(start->cards);
        free(start);
        start = tmp;
    }
}

static Card* create_card(int value) {
    Card* card = (Card*) malloc(sizeof(Card));
    if(card == NULL)
        return NULL;

    card->next = NULL;
    card->value = value;
    return card;
}

static Pile* create_pile() {
    Pile* pile = (Pile*) malloc(sizeof(Pile));
    if(pile == NULL)
        return NULL;

    pile->next = NULL;
    pile->cards = NULL;
    return pile;
}

static bool place_card(Data* data, Pile** start, int value) {
    Card* card = create_card(value);
    if(card == NULL)
        return false;

    Pile* pile = *start;
    while(pile != NULL && pile->cards != NULL && pile->cards->value < value && run(data))
        pile = pile->next;

    if(!run(data))
        return true;

    if(pile != NULL) {
        card->next = pile->cards;
        pile->cards = card;
        return true;
    }

    pile = create_pile();
    if(pile == NULL) {
        free_cards(card);
        return false;
    }

    pile->cards = card;

    if((*start) == NULL) {
        *start = pile;
        return true;
    }

    Pile* previous = *start;
    while(previous->next != NULL && run(data))
        previous = previous->next;

    previous->next = pile;
    return true;
}

short run_patience_sort(Data* data) {
    Pile* piles = NULL;

    for(int i = 0; i < data->array_len && run(data); i++) {
        if(!place_card(data, &piles, data->array[i])) {
            free_piles(piles);
            return SORT_FAILURE;
        }

        data->cursor = i;
        tick(data);
    }

    for(int i = 0; i < data->array_len && run(data); i++) {
        Pile* pile = piles;
        Pile* min = piles;

        while(pile != NULL && run(data)) {
            if(min->cards == NULL || (pile->cards != NULL && pile->cards->value < min->cards->value))
                min = pile;

            pile = pile->next;
        }

        if(run(data)) {
            Card* card = min->cards;
            min->cards = card->next;
            data->array[i] = card->value;
            free(card);

            data->cursor = i;
            tick(data);
        }
    }

    free_piles(piles);
    return SORT_SUCCESS;
}