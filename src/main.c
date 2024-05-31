#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_MAX 4096
// colors
#define RED "\x1b[31m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

#define horizontalChild "\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
#define finalHorizontalChild "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
#define verticalLine "\xe2\x94\x82"
#define simpleLine "\xe2\x94\x80"
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
  char *color;
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
  if (type == 1) {
    current->color = YELLOW;
  } else {
    current->color = CYAN;
  }
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

void printTree(TreeNode *node, char *prefix, int isLast) {
  if (node == NULL)
    return;

  printf("%s", prefix);
  if (strcmp(node->name, ".") == 0) {
    printf("%s%s%s\n", RED, node->path, RESET);
  } else if (isLast) {
    printf("%s%s %s%s%s\n", WHITE, finalHorizontalChild, node->color,
           node->name, RESET);
  } else {
    printf("%s%s %s%s%s\n", WHITE, horizontalChild, node->color, node->name,
           RESET);
  }

  char newPrefix[PATH_MAX];
  strcpy(newPrefix, prefix);
  if (isLast) {
    strcat(newPrefix, "    ");
  } else {
    if (strcmp(node->name, ".") == 0) {
      strcat(newPrefix, "    ");
    } else {

      strcat(newPrefix, verticalLine "   ");
    }
  }

  SNode *current = node->childrens->head;
  while (current != NULL) {
    printTree(current->data, newPrefix, current->next == NULL);
    current = current->next;
  }
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
      loadChildren(tmp2, tree, spaces + 7);
    }
  }
}

int main(int argc, char *argv[]) {
  char *initialPath;
  char *tmp[argc - 2];
  char *fileNames;
  printf("%d\n", argc);
  if (argc == 2) {
    if (strcmp(argv[1], "-h") == 0) {
      printf("Usage: main                             shows the content of the "
             "current path\n");
      printf("Usage: main [path]                      shows the content of the "
             "entered route \n");
      printf(
          "Usage: main [path] -ef [nameDirectory]  displays the contents of "
          "the entered path and excludes directories with the entered names\n");
      return 0;
    } else {
      initialPath = argv[1];
      tmp[0] = "node_modules";
      tmp[1] = ".git";
    }
  } else if (argc > 2) {
    exit(0);
  } else {
    char buff[PATH_MAX];
    initialPath = getcwd(buff, PATH_MAX);
    // fileNames[0] = "node_modules";
    // fileNames[1] = ".git";
  }

  Tree *tree = malloc(sizeof(Tree));
  TreeNode *root =
      createTreeNode(".", initialPath, isDirectory(initialPath), 0);
  tree->root = root;
  loadChildren(initialPath, tree, 1);
  printTree(tree->root, "", 0);
  return 0;
}
