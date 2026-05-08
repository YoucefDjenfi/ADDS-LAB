#include "../headers/file_structure.h"
#include "../headers/trie_machine_abstract.h"
#include <stdlib.h>

void prep_file_node(file_node *node) {
  if (node == NULL)
    return;
  node->trie = create_trie();
  node->next = node->prev = NULL;
}

file_node *create_file_node() {
  file_node *node = malloc(sizeof(file_node));
  prep_file_node(node);
  return node;
}

file *create_file() {
  file *f = malloc(sizeof(file));
  if (f) {
    f->head = f->tail = NULL;
  }
  return f;
}

void free_file(file *file) {
  if (file == NULL) {
    return;
  }
  file_node *curr = file->head;
  while (curr != NULL) {
    file_node *next = curr->next;
    free_Trie(curr->trie);
    free(curr);
    curr = next;
  }
  free(file);
}

void fill_file(file *file, char *text, int s) {
  if (file == NULL || text == NULL) {
    return;
  }
  char word[1000];
  int j = 0;

  if (file->head == NULL) {
    file->head = file->tail = create_file_node();
  }

  file_node *node = file->tail;
  for (int i = 0; i < s; i++) {
    if (text[i] == ' ') {
      if (j > 0) {
        word[j] = '\0';
        insert(node->trie, word, 1);
        j = 0;
      }
    } else if (text[i] == '.') {
      if (j > 0) {
        word[j] = '\0';
        insert(node->trie, word, 1);
        j = 0;
      }
      // Only create a new node if there is potentially more text
      if (i < s - 1) {
        file_node *temp = create_file_node();
        node->next = temp;
        temp->prev = node;
        node = temp;
        file->tail = node;
      }
    } else {
      if (j < 999) {
        word[j] = text[i];
        j++;
      }
    }
  }
  // Handle last word if no period/space at end
  if (j > 0) {
    word[j] = '\0';
    insert(node->trie, word, 1);
  }
}