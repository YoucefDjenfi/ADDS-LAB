#include "../headers/file_structure.h"
#include "../headers/trie_machine_abstract.h"
#include <stdlib.h>

para *create_para() {
  para *p = malloc(sizeof(para));
  if (p == NULL)
    return NULL;
  p->trie = create_trie();
  p->next = p->prev = NULL;
  return p;
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
  para *p = file->head;
  while (p != NULL) {
    para *nxt = p->next;
    free_Trie(para_get_trie(p));
    free(p);
    p = nxt;
  }
  free(file);
}

void file_enqueue_para(file *file, char *text, int nb) {
  if (file == NULL || text == NULL) return;
  char word[1000];
  int j = 0;
  if (file->head == NULL) {
    file->head = file->tail = create_para();
  }

  para *p = file->tail;
  for (int i = 0; i < nb; i++) {
    // case01: new paragraph
    if (text[i] == '\n\n') {
      if (j > 0) {
        word[j] = '\0';
        insert(para_get_trie(p), word, 1);
        j = 0;
      }
      // Only create a new node if there is potentially more text
      if (i < nb - 1) {
        para *temp = create_para();
        p->next = temp;
        temp->prev = p;
        p = temp;
        file->tail = p;
      }
    }
    else if (text[i] == ' ' || text[i] == '\n' || text[i] == '\t' || 
    text[i] == '.' || text[i] == '!' || text[i] == '?') {
      if (j > 0) {
        word[j] = '\0';
        insert(para_get_trie(p), word, 1);
        j = 0;
      }
    }
    else {
      if (j < 999) {
        word[j] = text[i];
        j++;
      }
    }
  }
  // Handle last word if no period/space at end
  if (j > 0) {
    word[j] = '\0';
    insert(para_get_trie(p), word, 1);
  }
}

// paragraph-level functions
void para_ass_adr_trie(para *p, Trie *t) {
  if (p == NULL) return;
  p->trie = t;
  return;
}

void para_ass_adr_next(para *p, para *q) {
  if (p == NULL) return;
  p->next = q;
  return;
}

void para_ass_adr_prev(para *p, para *q) {
  if (p == NULL) return;
  p->prev = q;
  return;
}

Trie *para_get_trie(para *p) {
  if (p == NULL) return NULL;
  return p->trie;
}

para *para_get_next(para *p) {
  if (p == NULL) return NULL;
  return p->next;
}

para *para_get_prev(para *p) {
  if (p == NULL) return NULL;
  return p->prev;
}


// file-level functions
int file_is_empty(file *f) {
  if (f == NULL) return 1;
  return f->head == NULL;
}

Trie *file_get_trie_by_pos(file *f, int n) {     // pos is 1-indexed 
  if (file_is_empty(f)) return NULL;
  para *p = f->head;
  int i = 1;
  while (p != NULL && i < n) {
    p = para_get_next(p);
    i++;
  }
  return para_get_trie(p);
}

int file_count(file *f) {
  if (f == NULL) return 0;
  para *p = f->head;
  int n = 0;
  while (p != NULL) {
    n++;
    p = para_get_next(p);
  }
  return n;
}