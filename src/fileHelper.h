#ifndef FILEHELPER_H
#define FILEHELPER_H

typedef struct {
  char *(*readFile)(const char *path);
} FileHelper;

FileHelper *fileHelper_load(void);


#endif
