#ifndef STRUCTURE_SENTENCES_H
#define STRUCTURE_SENTENCES_H

#include "trie_sentences.h"

typedef struct file_node_S {
    Trie_S* paragraph_trie;
    struct file_node_S* next;
    struct file_node_S* prev;
} file_node_S;

typedef struct file_S {
    file_node_S* head;
    file_node_S* tail;
} file_S;

file_S* create_file_S();
void fill_file_S(file_S* f, char* text, int size);
void free_file_S(file_S* f);

#endif
