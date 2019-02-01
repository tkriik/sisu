#include <sys/types.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "db.h"
#include "file.h"
#include "log.h"
#include "scan.h"
#include "util.h"

static const char *ignore_paths[] = {
	".",
	"..",
	".sisu.db"
};

void
scan_file_meta(const char *path, struct db *db)
{
	assert(path != NULL);
	assert(db != NULL);

	DIR *dir;
	struct dirent *dirent;
	dir = opendir(path);
	if (dir == NULL)
		FATAL("failed to open directory at '.': %s", strerror(errno));

	while ((dirent = readdir(dir)) != NULL) {
		int skip = 0;
		for (size_t i = 0; i < ARRAY_LEN(ignore_paths); i++) {
			const char *ignore_path = ignore_paths[i];
			if (strcmp(ignore_path, dirent->d_name) == 0) {
				skip = 1;
				break;
			}
		};

		if (skip)
			continue;

		struct file_meta fm;
		if (!file_meta_init(&fm, dirent->d_name))
			continue;

		db_add_file_meta(db, &fm);

		if (dirent->d_type == DT_DIR)
			scan_file_meta(dirent->d_name, db);
	}

	closedir(dir);
}
