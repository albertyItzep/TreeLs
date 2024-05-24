#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// structs  for tree N-ario
typedef struct NArioTree {
  struct TreeNode *root;
} Tree;

// structs for  double linked list
typedef struct LList {
  struct LNode *head;
  struct LNode *tail;
  int size;
} LList;

// structs for tree node
typedef struct TreeNode {
  char *name;
  char *extension;
  unsigned type : 1;
  unsigned visited : 1;
  int spaces;
  int color;
  struct LList *childrens;
} TreeNode;

// structs for node of double linked list
typedef struct LNode {
  struct LNode *next;
  struct LNode *prev;
  struct TreeNode *data;
} SNode;

LList *createList() {
  LList *list = (LList *)malloc(sizeof(LList));
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

SNode *createNode(TreeNode *data) {
  SNode *node = (SNode *)malloc(sizeof(SNode));
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

// TODO: code for the linked list
void insertList(TreeNode *data, LList *list) {
  SNode *newNode = createNode(data);
  if (list->head == NULL) {
    list->head = newNode;
    list->tail = newNode;
    list->size++;
  }
  list->tail->next = newNode;
  newNode->prev = list->tail;
  list->tail = newNode;
  list->size++;
}
// function to search node in the list
TreeNode *searchNodeList(LList *list, TreeNode *value) {
  if (list->head == NULL)
    return NULL;
  SNode *current = list->head;
  for (int i = 0; i < list->size; i++) {
    if (strcmp(current->data->name, value->name) != 0)
      return current->data;
    current = current->next;
  }
  return NULL;
}

TreeNode *getNodeIndex(LList *list, int index) {
  if (index >= list->size)
    return NULL;

  if (list->head == NULL)
    return NULL;

  SNode *current = list->head;
  for (int i = 0; i < list->size; i++) {
    if (i == index)
      return current->data;
    current = current->next;
  }
  return current->data;
}

// TODO: code for the tree

TreeNode *searchNodeParams(LList *list, TreeNode *value, int pos) {
  if (pos == -1)
    pos = 0;

  if (pos >= list->size)
    return NULL;

  TreeNode *current = getNodeIndex(list, pos);
  if (strcmp(current->name, value->name) == 0)
    if (current->type == value->type)
      return current;

  current = searchNodeParams(current->childrens, value, -1);
  if (current != NULL)
    return current;

  current = searchNodeParams(list, value, pos + 1);
  if (current != NULL)
    return current;

  return NULL;
}

TreeNode *searchNode(Tree *tree, TreeNode *value) {
  if (tree->root == NULL)
    return NULL;

  if (strcmp(tree->root->name, value->name) == 0)
    return tree->root;

  TreeNode *current = searchNodeParams(tree->root->childrens, value, -1);

  if (current != NULL)
    return current;

  return NULL;
}

int insertTreeNode(Tree *tree, TreeNode *parent, TreeNode *value) { return 0; }

int main(int argc, char *argv[]) {
  char *name;
  char buff[PATH_MAX];
  if (argc == 2) {
    name = argv[1];
  } else {
    name = getcwd(buff, PATH_MAX);
  }
  return 0;
}
