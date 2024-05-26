#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_MAX 4096

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
  char *path;
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
    return;
  }
  list->tail->next = newNode;
  newNode->prev = list->tail;
  list->tail = newNode;
  list->size++;
}
// function to search node in the list
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
  return NULL;
}

// TODO: code for the tree

TreeNode *createTreeNode(char *name, char *path, int type, int spaces) {
  TreeNode *current = (TreeNode *)malloc(sizeof(TreeNode));
  current->name = name;
  current->path = path;
  current->type = type;
  current->visited = 0;
  current->spaces = spaces;
  current->childrens = createList();
  if (type == 1)
    current->color = 1;
  else
    current->color = 0;
  return current;
}

TreeNode *searchNodeParams(LList *list, char *value, int pos) {
  if (pos == -1)
    pos = 0;

  if (pos >= list->size)
    return NULL;

  TreeNode *current = getNodeIndex(list, pos);
  if (strcmp(current->path, value) == 0)
    return current;

  current = searchNodeParams(current->childrens, value, -1);
  if (current != NULL)
    return current;

  current = searchNodeParams(list, value, pos + 1);
  if (current != NULL)
    return current;

  return NULL;
}

int insertTreeNode(Tree *tree, char *pathParent, TreeNode *value) {
  if (strcmp(tree->root->path, pathParent) == 0) {
    insertList(value, tree->root->childrens);
    return 1;
  } else {
    TreeNode *tmpParent =
        searchNodeParams(tree->root->childrens, pathParent, -1);
    if (tmpParent == NULL)
      return 0;
    insertList(value, tmpParent->childrens);
    return 1;
  }
  return 0;
}

void printTree(TreeNode *node, int pos) {

  if (pos == -1)
    pos = 0;

  if (node == NULL)
    return;

  if (node->visited == 0) {
    char *spaces = (char *)malloc(node->spaces);
    for (int i = 0; i < node->spaces; i++) {
      spaces[i] = ' ';
    }
    printf("%s file Name: %s\n", spaces, node->name);
    node->visited = 1;
  }

  if (node->childrens->size == 0)
    return;

  TreeNode *current = getNodeIndex(node->childrens, pos);
  if (current == NULL)
    return;
  printTree(current, -1);
  printTree(node, pos + 1);
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

void loadChildren(char *path, Tree *tree, int spaces) {
  char *tmp2;
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(path))) {
    perror("ERROR");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
        strcmp(entry->d_name, "node_modules") == 0)
      continue;
    tmp2 = concatPath(path, entry->d_name);
    TreeNode *tmp1 =
        createTreeNode(entry->d_name, tmp2, isDirectory(tmp2), spaces);
    int res = insertTreeNode(tree, path, tmp1);
    if (tmp1->type == 1 && res == 1) {
      loadChildren(tmp2, tree, spaces + 2);
    }
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

  Tree *tree = malloc(sizeof(Tree));
  TreeNode *root = createTreeNode(".", name, isDirectory(name), 0);
  tree->root = root;
  loadChildren(name, tree, 1);
  printTree(tree->root, -1);
  char tmp[6] = "\e[31m";
  printf("%s \xe2\x94\x80 \n", tmp);
  printf("%s \xe2\x94\x8a \n", tmp);
  printf("%s \xe2\x94\x94 \n", tmp);
  printf("%s \xe2\x94\x9c \n", tmp);
  return 0;
}
