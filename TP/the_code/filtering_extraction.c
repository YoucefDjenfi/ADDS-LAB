#include "../headers/filtering_extraction.h"
#include "../headers/sets_operations.h"
#include <stdlib.h>
#include <string.h>


void help_traverse_num(Trie_node *node, int *cpt) {
  if (node == NULL || cpt == NULL) {
    return;
  }
  if (node->is_end_of_word) {
    (*cpt) += node->count;
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      help_traverse_num(node->children[j], cpt);
    }
  }
}

int num_of_words(Trie *A) {
  if (A == NULL || A->Root == NULL) {
    return 0;
  }
  int cpt = 0;
  help_traverse_num(A->Root, &cpt);
  return cpt;
}

double calculateJaccard(Trie *a, Trie *b) {
  Trie *unionn = union_a_b(a, b);
  Trie *inter = intersection(a, b);
  int n1 = num_of_words(inter);
  int n2 = num_of_words(unionn);

  double result = 0.0;
  if (n2 != 0) {
    result = (double)n1 / (double)n2;
  }

  free_Trie(unionn);
  free_Trie(inter);
  return result;
}

void remove_stopwords(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *stopwords[] = {"the",  "is",   "at",    "which", "a",  "an",
                             "and",  "or",   "but",   "in",    "on", "to",
                             "for",  "with", "about", "as",    "by", "of",
                             "from", "that", "this"};
  int num_stopwords = sizeof(stopwords) / sizeof(stopwords[0]);
  for (int i = 0; i < num_stopwords; i++) {
    delete_word(&(trie->Root), (char *)stopwords[i]);
  }
}

void remove_conjunctions(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *conjunctions[] = {
      "however",     "therefore", "moreover", "thus",  "hence",  "nevertheless",
      "furthermore", "although",  "because",  "since", "unless", "while"};
  int num_conjunctions = sizeof(conjunctions) / sizeof(conjunctions[0]);
  for (int i = 0; i < num_conjunctions; i++) {
    delete_word(&(trie->Root), (char *)conjunctions[i]);
  }
}

void remove_pronouns(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *pronouns[] = {"he",   "she",   "they", "it",     "i",   "we",
                            "you",  "me",    "him",  "her",    "us",  "them",
                            "my",   "your",  "his",  "their",  "its", "our",
                            "mine", "yours", "hers", "theirs", "ours"};
  int num_pronouns = sizeof(pronouns) / sizeof(pronouns[0]);
  for (int i = 0; i < num_pronouns; i++) {
    delete_word(&(trie->Root), (char *)pronouns[i]);
  }
}

int check_similarity(Trie *a, Trie *b) {
  if (a == NULL || a->Root == NULL || b == NULL || b->Root == NULL) {
    return 0;
  }
  remove_conjunctions(a);
  remove_pronouns(a);
  remove_stopwords(a);
  remove_conjunctions(b);
  remove_pronouns(b);
  remove_stopwords(b);
  return (int)(calculateJaccard(a, b) * 100); // Return percentage
}

char *topic(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return NULL;
  }
  remove_conjunctions(trie);
  remove_pronouns(trie);
  remove_stopwords(trie);
  return most_common_roots(trie);
}

Trie_node *go_to(Trie *trie, char *root) {
  if (trie == NULL || trie->Root == NULL || root == NULL) {
    return NULL;
  }
  Trie_node *curr = trie->Root;
  int index;
  int size = strlen(root);
  for (int i = 0; i < size; i++) {
    index = root[i] - 'a';
    if (index < 0 || index >= 26)
      return NULL;
    if (curr->children[index] == NULL) {
      return NULL;
    } else {
      curr = curr->children[index];
    }
  }
  return curr;
}

void help_travers_2(Trie_node *node, char *word, int i, char **res, int *k) {
  if (node == NULL || res == NULL || k == NULL || *k >= 1000) {
    return;
  }
  if (node->is_end_of_word) {
    word[i] = '\0';
    res[(*k)++] = strdup(word);
    if (*k >= 1000)
      return; // Stop if we've reached the limit
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      word[i] = 'a' + j;
      help_travers_2(node->children[j], word, i + 1, res, k);
    }
  }
}

char **words_start_with(Trie *trie, char *s) {
  if (trie == NULL || trie->Root == NULL || s == NULL) {
    return NULL;
  }
  Trie_node *node = go_to(trie, s);
  if (node == NULL) {
    return NULL;
  }
  char **words =
      malloc(sizeof(char *) * 1001); // 1001 to include NULL terminator
  int k = 0;
  char word[256];
  strcpy(word, s);
  int size = strlen(s);
  help_travers_2(node, word, size, words, &k);
  words[k] = NULL; // NULL-terminate the array
  return words;
}

void helper_count(Trie_node *node, int *cpt) {
  if (node == NULL || cpt == NULL) {
    return;
  }
  if (node->is_end_of_word) {
    (*cpt) += node->count;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) {
      helper_count(node->children[i], cpt);
    }
  }
}

int countPrefixMatches(Trie *trie, char *prefix) {
  if (trie == NULL || trie->Root == NULL || prefix == NULL) {
    return 0;
  }
  Trie_node *node = go_to(trie, prefix);
  if (node == NULL) {
    return 0;
  }
  int cpt = 0;
  helper_count(node, &cpt);
  return cpt;
}
