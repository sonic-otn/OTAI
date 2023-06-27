/**
 * Copyright (c) 2021 Alibaba Group.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 * @file    otaimetadatalogger.h
 *
 * @brief   This module defines OTAI Metadata Logger
 */

#ifndef __OTAIMETADATALOGGER_H_
#define __OTAIMETADATALOGGER_H_

/**
 * @defgroup OTAIMETADATALOGGER OTAI - Metadata Logger Definitions
 *
 * @{
 */

/**
 * @brief Log level function definition.
 *
 * User can specify his own function that will be called when message log level
 * will be greater or equal to #otai_metadata_log_level.
 *
 * @param[in] log_level Log level
 * @param[in] file Source file
 * @param[in] line Line number in file
 * @param[in] function Function name
 * @param[in] format Format of logging
 * @param[in] ... Variable parameters
 */
typedef void (*otai_metadata_log_fn)(
        _In_ otai_log_level_t log_level,
        _In_ const char *file,
        _In_ int line,
        _In_ const char *function,
        _In_ const char *format,
        _In_ ...);

/**
 * @brief User specified log function.
 *
 * TODO: add a set function to update this?
 */
extern volatile otai_metadata_log_fn otai_metadata_log;

/**
 * @brief Log level for OTAI metadata macros.
 *
 * Log level can be changed by user at any time.
 *
 * TODO: add a set function to update this?
 */
extern volatile otai_log_level_t otai_metadata_log_level;

/**
 * @brief Helper log macro definition
 *
 * If logger function is NULL, stderr is used to print messages. Also, fprintf
 * function will validate parameters at compilation time.
 */
#define OTAI_META_LOG(loglevel,format,...)\
    if (loglevel >= otai_metadata_log_level)\
{\
    if (otai_metadata_log == NULL) /* or syslog? */ \
        fprintf(stderr, "%s:%d %s: " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    else\
        otai_metadata_log(loglevel, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__);\
}

/*
 * Helper macros.
 */

#define OTAI_META_LOG_ENTER()                OTAI_META_LOG(OTAI_LOG_LEVEL_DEBUG, ":> enter");
#define OTAI_META_LOG_DEBUG(format,...)      OTAI_META_LOG(OTAI_LOG_LEVEL_DEBUG, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_INFO(format,...)       OTAI_META_LOG(OTAI_LOG_LEVEL_INFO, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_NOTICE(format,...)     OTAI_META_LOG(OTAI_LOG_LEVEL_NOTICE, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_WARN(format,...)       OTAI_META_LOG(OTAI_LOG_LEVEL_WARN, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_ERROR(format,...)      OTAI_META_LOG(OTAI_LOG_LEVEL_ERROR, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_CRITICAL(format,...)   OTAI_META_LOG(OTAI_LOG_LEVEL_CRITICAL, ":- " format, ##__VA_ARGS__)
#define OTAI_META_LOG_EXIT()                 OTAI_META_LOG(OTAI_LOG_LEVEL_DEBUG, ":< exit");

/**
 * @}
 */
#endif /** __OTAIMETADATALOGGER_H_ */
