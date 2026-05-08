#include "../headers/structure_sentences.h"
#include <stdlib.h>

file_S *create_file_S() {
  file_S *f = malloc(sizeof(file_S));
  if (f) {
    f->head = f->tail = NULL;
  }
  return f;
}

file_node_S *create_file_node_S() {
  file_node_S *node = malloc(sizeof(file_node_S));
  if (node) {
    node->paragraph_trie = create_trie_S();
    node->next = node->prev = NULL;
  }
  return node;
}

void fill_file_S(file_S *f, char *text, int size) {
  if (!f || !text)
    return;

  char sentence[1000];
  int j = 0;
  if (f->head == NULL) {
    f->head = f->tail = create_file_node_S();
  }

  file_node_S *curr_node = f->tail;

  for (int i = 0; i < size; i++) {
    if (text[i] == '.') {
      sentence[j] = '\0';
      if (j > 0) {
                insert_S(curr_node->paragraph_trie, sentence, 1);
                j = 0;
      }
    } else if (text[i] == '\n') {
      sentence[j] = '\0';
      if (j > 0) {
        insert_S(curr_node->paragraph_trie, sentence, 1);
      }
      file_node_S *new_node = create_file_node_S();
      curr_node->next = new_node;
      new_node->prev = curr_node;
      curr_node = new_node;
      f->tail = curr_node;
      j = 0;
    } else {
      if (j < 999) {
        sentence[j++] = text[i];
      }
    }
  }
}

void free_file_S(file_S *f) {
  if (!f)
    return;
  file_node_S *curr = f->head;
  while (curr) {
    file_node_S *next = curr->next;
    free_Trie_S(curr->paragraph_trie);
    free(curr);
    curr = next;
  }
  free(f);
}
