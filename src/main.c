// vamos a crear un programa que muestre los archivos de un directorio
// en forma de arbol
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// proceso para saber si una ruta es un directorio
int idDir(char *path) {
  struct stat buffer;
  int status;
  status = stat(path, &buffer);
  if (status != 0) {
    perror("ERROR");
    exit(1);
  }
  if (!S_ISDIR(buffer.st_mode)) {
    printf("ERROR: %s Not is a directory\n", path);
    return 0;
  }
  return 1;
}
// proceso para obtener el contenido de un directorio
void getDirContent(char *path) {
  DIR *folder;
  struct dirent *directory;
  int n = 0;

  folder = opendir(path);
  if (folder == NULL) {
    perror("ERROR");
    exit(1);
  }
  while ((directory = readdir(folder)) != NULL) {
    if (strcmp(directory->d_name, ".") != 0 &&
        strcmp(directory->d_name, "..") != 0) {
      if (directory->d_type == DT_DIR) {
        printf("%s\n", directory->d_name);
      }
    }
  }
}
int main(int argc, char *argv[], char *envp[]) {

  // proceso para obtener la direccion ya sea por parametro o por defecto
  char *buf;
  buf = (char *)malloc(100 * sizeof(char));
  if (argc == 1) {
    getcwd(buf, 100);
  } else {
    buf = argv[1];
    printf("Directorio: %s\n", buf);
  }

  // proceso para verificar que la ruta sea un directorio y existe
  if (!idDir(buf)) {
    exit(1);
  }

  getDirContent(buf);
  return 0;
}
