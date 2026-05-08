#ifndef FILE_STRUCTURE_H
#define FILE_STRUCTURE_H

#include "trie_machine_abstract.h"

typedef struct file_node {
  Trie *trie;
  struct file_node *next;
  struct file_node *prev;
} file_node;

typedef struct file {
  file_node *head;
  file_node *tail;
} file;

void prep_file_node(file_node *node);
file_node *create_file_node();
file *create_file();
void free_file(file *f);
void fill_file(file *f, char *text, int s);

#endif // FILE_STRUCTURE_H
