//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_LINKED_LIST_H
#define SCALLION_LINKED_LIST_H

#include <strings.h>

struct NODE {
    void * contents;
    char contents_type[10];
    struct NODE * next;
    struct NODE * prev;
    unsigned long id;
};

struct NODE * ll_gen_head() {
    return (struct NODE *)calloc(1, sizeof(struct NODE));
}

void ll_add(struct NODE * head, void * contents_to_add, char * contents_type) {
    // make new node
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = 0;
    node_to_add->contents = contents_to_add;
    strcpy(node_to_add->contents_type, contents_type);
    // add it
    struct NODE * ptr = head;
    if (ptr->next == NULL && ptr->prev == NULL) {
        ptr->next = node_to_add;
        ptr->prev = node_to_add;
        node_to_add->next = ptr;
        node_to_add->prev = ptr;
    } else {
        struct NODE * below = ptr->prev;
        struct NODE * above = ptr;
        below->next = node_to_add;
        node_to_add->prev = below;
        above->prev = node_to_add;
        node_to_add->next = above;
    }
}

void ll_add_at(struct NODE * below, void * contents_to_add, char * contents_type) {
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = 0;
    node_to_add->contents = contents_to_add;
    strcpy(node_to_add->contents_type, contents_type);
    struct NODE * above = below->next;
    if (below->next == NULL && below->prev == NULL) {
        below->next = node_to_add;
        below->prev = node_to_add;
        node_to_add->next = below;
        node_to_add->prev = below;
    } else {
        below->next = node_to_add;
        node_to_add->prev = below;
        above->prev = node_to_add;
        node_to_add->next = above;
    }
}

void ll_add_with_id(struct NODE * head, void * contents_to_add, char * contents_type, unsigned long id) {
    // make new node
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = id;
    node_to_add->contents = contents_to_add;
    strcpy(node_to_add->contents_type, contents_type);
    // add it
    struct NODE * ptr = head;
    if (ptr->next == NULL && ptr->prev == NULL) {
        ptr->next = node_to_add;
        ptr->prev = node_to_add;
        node_to_add->next = ptr;
        node_to_add->prev = ptr;
    }
    do {
        if (ptr->id <= id && ptr->next->id >= id) {
            struct NODE *below = ptr;
            struct NODE *above = ptr->next;
            below->next = node_to_add;
            node_to_add->prev = below;
            above->prev = node_to_add;
            node_to_add->next = above;
            break;
        }
        ptr = ptr->next;
    } while (ptr != head);
}

// ERROR CODES
// -1 : attempting to remove non-existent node
void ll_remove(struct NODE * head, void * contents) {
    struct NODE * ptr = head;
    while (ptr->contents != contents) {
        ptr = ptr->next;
        if (ptr == head) break;
    }
    if (ptr->contents == contents) {
        struct NODE * below = ptr->prev;
        struct NODE * above = ptr->next;
        below->next = above;
        above->prev = below;
        free(ptr);
    } else {
        printf("attempted to remove a non-existent node\n");
        exit (-1);
    }
}

// ERROR CODES
// -1 : attempting to remove non-existent node
void ll_remove_with_id(struct NODE * head, unsigned long id) {
    struct NODE * ptr = head;
    while (ptr->id != id) {
        ptr = ptr->next;
        if (ptr == head) break;
    }
    if (ptr->id == id) {
        struct NODE * below = ptr->prev;
        struct NODE * above = ptr->next;
        below->next = above;
        above->prev = below;
        free(ptr);
    } else exit (-1);
}

void ll_print(struct NODE * head) {
    struct NODE * ptr = head;
    if (ptr == NULL) {
        printf("NULL\n"); return;
    }
    do {
        printf("[ <-{%p} (prev), loc:%p, contents:%p, contents_type:%s, id:%ld, (next) {%p}-> ]\n", ptr->prev, ptr, ptr->contents, ptr->contents_type, ptr->id, ptr->next);
        ptr = ptr->next;
    } while (ptr != head && ptr != NULL);
}


#endif //SCALLION_LINKED_LIST_H
