#ifndef LOG_H
#define LOG_H

enum log_level {
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

extern enum log_level LOG_LEVEL;

void _log(enum log_level, const char *, ...);

#define FATAL(fmt, ...) \
    _log(LOG_LEVEL_FATAL, "[FATAL] %s:%d (%s): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define ERROR(fmt, ...) \
    _log(LOG_LEVEL_ERROR, "[ERROR] %s:%d (%s): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define WARN(fmt, ...) \
    _log(LOG_LEVEL_WARN, "[WARN] %s:%d (%s): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define INFO(fmt, ...) \
    _log(LOG_LEVEL_INFO, "[INFO] %s:%d (%s): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define DEBUG(fmt, ...) \
    _log(LOG_LEVEL_DEBUG, "[DEBUG] %s:%d (%s): " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#endif
