/**
 * @file logger.h
 * @author waterproofpatch (waterproofpatch@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

/**
 * @brief Debug wrapper around DEBUG
 *
 */
#define LOG_DEBUG(...)                                        \
    do                                                        \
    {                                                         \
        fprintf(stdout, "%s:%d:DEBUG: ", __FILE__, __LINE__); \
        fprintf(stdout, ##__VA_ARGS__);                       \
    } while (0)

/**
 * @brief Debug wrapper around ERROR
 *
 */
#define LOG_ERROR(...)                                        \
    do                                                        \
    {                                                         \
        fprintf(stdout, "%s:%d:ERROR: ", __FILE__, __LINE__); \
        fprintf(stdout, ##__VA_ARGS__);                       \
    } while (0)

/**
 * @brief Debug wrapper around LOG_INFO
 *
 */
#define LOG_INFO(...)                                        \
    do                                                       \
    {                                                        \
        fprintf(stdout, "%s:%d:INFO: ", __FILE__, __LINE__); \
        fprintf(stdout, ##__VA_ARGS__);                      \
    } while (0)

#endif /* _LOGGER_H_ */
