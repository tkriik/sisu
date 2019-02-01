#ifndef DB_H
#define DB_H

#include <sqlite3.h>

#include "file.h"

struct db {
	sqlite3 *handle;
	struct {
		sqlite3_stmt *add_file_meta;
	} stmt;
};

void db_init(struct db *, const char *);
void db_add_file_meta(struct db *, struct file_meta *);

#endif
