#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "logl.h"

/**
 * @brief Entry in the tag list containing the tag and the log level
 */
typedef struct {
    const char* tag;
    log_level_t level;
} tag_entry_t;

static tag_entry_t tag_entries[LOGL_MAX_TAG_ENTRIES];
static size_t tag_entries_count = 0;
static const char* TAG = "logl";

void logl_set_level(const char* tag, log_level_t level) {
    // Check if the tag is already in the list
    for (int i = 0; i < tag_entries_count; i++) {
        if (tag_entries[i].tag == tag) {
            tag_entries[i].level = level;
            return;
        }
    }

    // The tag is not in the list yet, so add it
    if (tag_entries_count < LOGL_MAX_TAG_ENTRIES) {
        tag_entries[tag_entries_count].tag = tag;
        tag_entries[tag_entries_count].level = level;
        tag_entries_count++;
    } else {
        LOG_WARN(TAG, "Could not add tag %s to list: List is full. Try increasing LOGL_MAX_TAG_ENTRIES", tag);
    }
}

log_level_t logl_get_level(const char* tag) {
    log_level_t level = LOGL_MIN_LOG_LEVEL;

    // Try to find the tag in the list
    for (int i = 0; i < tag_entries_count; i++) {
        if (tag_entries[i].tag == tag) {
            level = tag_entries[i].level;
            break;
        }
    }

    return level;
}

/**
 * @brief Get the current timestamp to be used in the logger
 *
 * @return The current timestamp in milliseconds since boot
 */
uint32_t logl_get_timestamp(void) {
    return time(NULL);
}

/**
 * @brief Write a log message to the console
 * For now, this is just a wrapper around printf.
 *
 * @param[in] format The format string (see printf)
 * @param[in] ...
 */
void logl_write(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
