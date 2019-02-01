#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"

enum log_level LOG_LEVEL = LOG_LEVEL_DEBUG;

void
_log(enum log_level level, const char *fmt, ...)
{
	assert(fmt != NULL);

	if (LOG_LEVEL < level)
		return;

	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (level == LOG_LEVEL_FATAL)
		exit(EXIT_FAILURE);
}
