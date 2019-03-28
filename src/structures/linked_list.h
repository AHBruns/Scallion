//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_LINKED_LIST_H
#define SCALLION_LINKED_LIST_H

struct NODE {
    void * contents;
    struct NODE * next;
    struct NODE * prev;
    unsigned long id;
};

struct NODE * ll_gen_head() {
    return (struct NODE *)calloc(1, sizeof(struct NODE));
}

void ll_add(struct NODE * head, void * contents_to_add) {
    // make new node
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = 0;
    node_to_add->contents = contents_to_add;
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

void ll_add_at(struct NODE * below, void * contents_to_add) {
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = 0;
    node_to_add->contents = contents_to_add;
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

void ll_add_with_id(struct NODE * head, void * contents_to_add, unsigned long id) {
    // make new node
    struct NODE * node_to_add = (struct NODE *)calloc(1, sizeof(struct NODE));
    node_to_add->id = id;
    node_to_add->contents = contents_to_add;
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
    } else exit (-1);
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


#endif //SCALLION_LINKED_LIST_H
