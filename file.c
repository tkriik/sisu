#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "log.h"

int
file_meta_init(struct file_meta *fm, const char *path)
{
	assert(fm != NULL);
	assert(path != NULL);
	assert(strlen(path) < FILE_META_PATH_MAX);

	struct stat sb;
	int rc;

	rc = lstat(path, &sb);
	if (rc == -1) {
		WARN("failed to get file status at path '%s': %s", path,
		    strerror(errno));
		return 0;
	}

	memset(fm, 0, sizeof(*fm));

	size_t path_len = strlen(path);
	assert(path_len < FILE_META_PATH_MAX);
	memcpy(fm->path, path, path_len);
	fm->path_len = (uint64_t)path_len;

	enum file_type type = FILE_TYPE_UNKNOWN;
	if (S_ISREG(sb.st_mode))
		type = FILE_TYPE_REGULAR;
	else if (S_ISDIR(sb.st_mode))
		type = FILE_TYPE_DIR;

	if (type == FILE_TYPE_UNKNOWN) {
		WARN("ignoring file with unsupported type at path '%s'", path);
		return 1;
	};

	fm->type = type;
	fm->size = (int64_t)sb.st_size;
	fm->atime = (int64_t)sb.st_atime;
	/* TODO: sha256 */

	return 1;
}
