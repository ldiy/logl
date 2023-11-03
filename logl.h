/**
 * @file logl.h
 * @brief LOGL - A simple logging library
 *
 * This logging library provides a simple logging interface with different verbosity levels.
 * Each tag can have its own log level.
 * Only messages with a log level greater or equal to the log level of the tag and the global log level will be printed.
 */

#ifndef LOGL_H
#define LOGL_H

#include <stdint.h>

/**
 * @brief Log levels
 */
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} log_level_t;

/* For internal use only.
 * Use the LOG_* macros instead e.g., LOG_DEBUG(TAG, "Debug message");
 */
uint32_t logl_get_timestamp(void);
void logl_write(const char* format, ...);

/**
 * @brief Set the log level for a tag
 * If the tag is not already in the list, it will be added.
 *
 * @param[in] tag The tag to set the log level for
 * @param[in] level The log level to set (member of @ref log_level_t)
 */
void logl_set_level(const char* tag, log_level_t level);

/**
 * @brief Get the log level for a tag
 *
 * @param[in] tag The tag to get the log level for
 * @return The log level for the tag (member of @ref log_level_t)
 */
log_level_t logl_get_level(const char* tag);

/**
 * @brief The global minimum log level.
 * Messages with a log level lower than this will not be logged.
 * And log statements with a log level lower than this will optimized away by the compiler.
 * @ref log_level_t
 */
#define LOGL_MIN_LOG_LEVEL LOG_LEVEL_DEBUG

/**
 * @brief The maximum number of tags that can be used.
 */
#define LOGL_MAX_TAG_ENTRIES 20

/**
 * @brief Whether to use color in the log output.
 * This is only supported in terminals that support ANSI escape codes.
 */
#define LOGL_USE_COLOR 1

#if LOGL_USE_COLOR
#define LOGL_RESET_COLOR "\033[0m"
#define LOGL_COLOR_E     "\033[0;31m" // Red
#define LOGL_COLOR_W     "\033[0;33m" // Brown
#define LOGL_COLOR_I     "\033[0;32m" // Green
#define LOGL_COLOR_D
#else
#define LOGL_RESET_COLOR
#define LOGL_COLOR_E
#define LOGL_COLOR_W
#define LOGL_COLOR_I
#define LOGL_COLOR_D
#endif

#define LOGL_FORMAT(letter, format) LOGL_COLOR_##letter #letter " (%lu) %s: " format LOGL_RESET_COLOR "\r\n"

#define LOGL_LOG(level, tag, format, ...)                                                   \
    do {                                                                                      \
        if (level >= LOGL_MIN_LOG_LEVEL && level >= logl_get_level(tag)) {                    \
            if (level == LOG_LEVEL_DEBUG) {                                                   \
                logl_write(LOGL_FORMAT(D, format), logl_get_timestamp(), tag, ##__VA_ARGS__); \
            } else if (level == LOG_LEVEL_INFO) {                                             \
                logl_write(LOGL_FORMAT(I, format), logl_get_timestamp(), tag, ##__VA_ARGS__); \
            } else if (level == LOG_LEVEL_WARN) {                                             \
                logl_write(LOGL_FORMAT(W, format), logl_get_timestamp(), tag, ##__VA_ARGS__); \
            } else if (level == LOG_LEVEL_ERROR) {                                            \
                logl_write(LOGL_FORMAT(E, format), logl_get_timestamp(), tag, ##__VA_ARGS__); \
            }                                                                                 \
        }                                                                                     \
    } while (0)

/**
 * @brief Macro to log a debug message (LOG_LEVEL_DEBUG)
 */
#define LOG_DEBUG(tag, format, ...) LOGL_LOG(LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

/**
 * @brief Macro to log an info message (LOG_LEVEL_INFO)
 */
#define LOG_INFO(tag, format, ...) LOGL_LOG(LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

/**
 * @brief Macro to log a warning message (LOG_LEVEL_WARN)
 */
#define LOG_WARN(tag, format, ...) LOGL_LOG(LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

/**
 * @brief Macro to log an error message (LOG_LEVEL_ERROR)
 */
#define LOG_ERROR(tag, format, ...) LOGL_LOG(LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#endif // LOGL_H
