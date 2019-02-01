#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

#include "db.h"
#include "file.h"
#include "log.h"
#include "util.h"

static const char *init_ddls[] = {
	"CREATE TABLE IF NOT EXISTS file_meta ("                           "\n"
	"  path  TEXT PRIMARY KEY NOT NULL,"                               "\n"
	"  type  INTEGER NOT NULL CHECK (type IN (1, 2)),"                 "\n"
	"  size  INTEGER NOT NULL,"                                        "\n"
	"  atime INTEGER NOT NULL"                                         "\n"
	")"                                                                "\n"
};

static const char ADD_FILE_META_DDL[] =
	"INSERT OR REPLACE INTO file_meta VALUES (?, ?, ?, ?)";

static void
assert_db_ok(struct db *db)
{
	assert(db->handle != NULL);
	assert(db->stmt.add_file_meta != NULL);
};

void
db_init(struct db *db, const char *path)
{
	assert(db != NULL);
	assert(path != NULL);

	int rc;

	DEBUG("opening database at '%s'", path);

	rc = sqlite3_open(path, &db->handle);
	if (rc != 0)
		FATAL("failed to open database at '%s': %s",
		    sqlite3_errmsg(db->handle));

	DEBUG("initializing tables");

	for (size_t i = 0; i < ARRAY_LEN(init_ddls); i++) {
		const char *ddl = init_ddls[i];
		char *errmsg = NULL;
		rc = sqlite3_exec(db->handle, ddl, NULL, 0, &errmsg);
		if (rc != SQLITE_OK)
			FATAL("failed to initialize table: %s", errmsg);
	}

	DEBUG("compiling prepared statements");

	rc = sqlite3_prepare_v2(db->handle, ADD_FILE_META_DDL,
	    sizeof(ADD_FILE_META_DDL), &db->stmt.add_file_meta, NULL);
	if (rc != SQLITE_OK)
		FATAL("failed to compile prepared statement: %s",
		    sqlite3_errmsg(db->handle));

	assert_db_ok(db);
}

void
db_add_file_meta(struct db *db, struct file_meta *fm)
{
	assert_db_ok(db);
	assert(fm != NULL);

	int rc;

	DEBUG("adding file metadata with path '%s'", fm->path);

	rc = sqlite3_bind_text64(db->stmt.add_file_meta, 1, fm->path,
	    fm->path_len, SQLITE_TRANSIENT, SQLITE_UTF8);
	if (rc != SQLITE_OK)
		FATAL("failed to bind file path '%s' to prepared statement: %s",
		    fm->path, sqlite3_errmsg(db->handle));

	rc = sqlite3_bind_int64(db->stmt.add_file_meta, 2, fm->type);
	if (rc != SQLITE_OK)
		FATAL("failed to bind file type %" PRId64 " to prepared statement: %s",
		    fm->type, sqlite3_errmsg(db->handle));

	rc = sqlite3_bind_int64(db->stmt.add_file_meta, 3, fm->size);
	if (rc != SQLITE_OK)
		FATAL("failed to bind file size %" PRId64 " to prepared statement: %s",
		    fm->size, sqlite3_errmsg(db->handle));

	rc = sqlite3_bind_int64(db->stmt.add_file_meta, 4, fm->atime);
	if (rc != SQLITE_OK)
		FATAL("failed to bind file access time %" PRId64 " to prepared statement: %s",
		    fm->atime, sqlite3_errmsg(db->handle));

	rc = sqlite3_step(db->stmt.add_file_meta);
	if (rc != SQLITE_DONE)
		FATAL("failed to add file metadata with path '%s': %s",
		    fm->path, sqlite3_errmsg(db->handle));

	rc = sqlite3_reset(db->stmt.add_file_meta);
	if (rc != SQLITE_OK)
		FATAL("failed to reset prepared statement: %s",
		    sqlite3_errmsg(db->handle));
}
