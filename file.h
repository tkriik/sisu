#ifndef FILE_META_H
#define FILE_META_H

#include <stdint.h>

enum file_type {
	FILE_TYPE_UNKNOWN = 0,
	FILE_TYPE_REGULAR = 1,
	FILE_TYPE_DIR     = 2
};

#define FILE_META_PATH_MAX 0x3FFF

struct file_meta {
	char     path[FILE_META_PATH_MAX];
	uint64_t path_len;
	enum     file_type type;
	int64_t  size;
	int64_t  atime;
	uint8_t  sha256[32];
};

int file_meta_init(struct file_meta *, const char *);

#endif
