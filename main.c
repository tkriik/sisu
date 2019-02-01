#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "db.h"
#include "file.h"
#include "log.h"
#include "scan.h"
#include "util.h"

static const char *PROGNAME = NULL;

static void
usage(void)
{
	assert(PROGNAME != NULL);

	fprintf(stderr, "usage: %s\n", PROGNAME);
	exit(EXIT_FAILURE);
}

static const char *paths[] = {
	"main.c",
	".git/",
	"Makefile"
};

int
main(int argc, char **argv)
{
	PROGNAME = argv[0];

	struct db db;
	const char *db_path = ".sisu.db";

	INFO("initializing database");
	db_init(&db, db_path);

	INFO("testing add_file_meta");
	for (size_t i = 0; i < ARRAY_LEN(paths); i++) {
		const char *path = paths[i];
		struct file_meta fm;
		if (!file_meta_init(&fm, path))
			continue;
		db_add_file_meta(&db, &fm);
	}

	scan_file_meta(".", &db);

	return EXIT_SUCCESS;
}
