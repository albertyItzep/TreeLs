#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
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

TreeNode *createTreeNode(char *name, char *extencion, int type) {
  TreeNode *current = (TreeNode *)malloc(sizeof(TreeNode));
  current->name = name;
  current->extension = name;
  current->type = type;
  return current;
}

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

int insertTreeNode(Tree *tree, TreeNode *parent, TreeNode *value) {
  if (tree->root == NULL) {
    tree->root = value;
    return 1;
  }

  if (strcmp(parent->name, tree->root->name) == 0) {
    if (parent->type == tree->root->type) {
      parent = tree->root;
      return 1;
    }
  } else {
    parent = searchNodeParams(tree->root->childrens, parent, -1);
  }

  if (parent != NULL) {
    insertList(value, parent->childrens);
    return 1;
  }
  return 0;
}

void printTree(TreeNode *root, int pos) {
  if (pos == -1)
    pos = 0;

  if (root == NULL)
    return;
  printf("%s\n", root->name);

  if (root->childrens->size == 0)
    return;

  TreeNode *current = getNodeIndex(root->childrens, pos);
  if (current == NULL)
    return;
  printTree(current, -1);

  printTree(root, pos + 1);
}

// TODO: Code for the validation data beofre insert in tree

int isDirectory(char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

char *concatPath(char *path, char *name) {
  char *tmp = (char *)malloc(strlen(path) + strlen(name) + 2);
  strcpy(tmp, path);
  strcat(tmp, "/");
  strcat(tmp, name);
  return tmp;
}
// TODO: code for manipulation of directories and insert in tree

void loadChildren(char *path, Tree *tree) {
  char *tmp2;
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(path))) {
    perror("ERROR");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
        strcmp(entry->d_name, ".git") == 0 ||
        strcmp(entry->d_name, "node_modules") == 0)
      continue;
    tmp2 = concatPath(path, entry->d_name);
  }
}

int main(int argc, char *argv[]) {
  char *name;
  char buff[PATH_MAX];
  if (argc == 2) {
    name = argv[1];
  } else {
    name = getcwd(buff, PATH_MAX);
  }

  printf("%s\n", name);
  Tree *tree;
  loadChildren(name, tree);

  return 0;
}
