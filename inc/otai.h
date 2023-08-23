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
 * @file    otai.h
 *
 * @brief   This module defines an entry point into OTAI-2.0
 */

#if !defined (__OTAI_H_)
#define __OTAI_H_

#include "otaistatus.h"
#include "otaitypes.h"
#include "otaiattenuator.h"
#include "otaioa.h"
#include "otaiosc.h"
#include "otaiocm.h"
#include "otaiotdr.h"

/**
 * @defgroup OTAI OTAI - Entry point specific API definitions.
 *
 * @{
 */

/**
 * @brief Defined API sets have assigned IDs.
 *
 * If specific API method table changes in any way (method signature, number of
 * methods), a new ID needs to be created (e.g. PORT2) and old API still may
 * need to be supported for compatibility with older adapter hosts.
 */
typedef enum _otai_api_t
{
    OTAI_API_UNSPECIFIED      =  0, /**< unspecified API */
    OTAI_API_OA               =  1, /**< otai_oa_api_t */
    OTAI_API_OSC              =  2, /**< otai_osc_api_t */
    OTAI_API_ATTENUATOR       =  3, /**< otai_attenuator_api_t */
    OTAI_API_OCM              =  4, /**< otai_ocm_api_t */
    OTAI_API_OTDR             =  5, /**< otai_otdr_api_t */
    OTAI_API_MAX              =  6, /**< total number of APIs */
} otai_api_t;

/**
 * @brief Defines the logging level.
 */
typedef enum _otai_log_level_t
{
    OTAI_LOG_LEVEL_DEBUG,        /**< Debug logging level */
    OTAI_LOG_LEVEL_INFO,         /**< Info logging level */
    OTAI_LOG_LEVEL_NOTICE,       /**< Notice logging level */
    OTAI_LOG_LEVEL_WARN,         /**< Warning logging level */
    OTAI_LOG_LEVEL_ERROR,        /**< Error logging level */
    OTAI_LOG_LEVEL_CRITICAL,     /**< Critical logging level */
    OTAI_LOG_LEVEL_MAX           /**< Number of logging levels */
} otai_log_level_t;

typedef const char* (*otai_profile_get_value_fn)(
        _In_ otai_linecard_profile_id_t profile_id,
        _In_ const char *variable);

typedef int (*otai_profile_get_next_value_fn)(
        _In_ otai_linecard_profile_id_t profile_id,
        _Out_ const char **variable,
        _Out_ const char **value);

/**
 * @brief Method table that contains function pointers for services exposed by
 * adapter host for adapter.
 */
typedef struct _otai_service_method_table_t
{
    /**
     * @brief Get variable value given its name
     */
    otai_profile_get_value_fn        profile_get_value;

    /**
     * @brief Enumerate all the K/V pairs in a profile.
     *
     * Pointer to NULL passed as variable restarts enumeration. Function
     * returns 0 if next value exists, -1 at the end of the list.
     */
    otai_profile_get_next_value_fn   profile_get_next_value;

} otai_service_method_table_t;

/**
 * @brief Adapter linecard initialization call
 *
 * This is NOT for SDK initialization. This function allows the adapter to
 * initialize any data/control structures that may be necessary during
 * subsequent OTAI operations.
 *
 * @param[in] flags Reserved for future use, must be zero
 * @param[in] services Methods table with services provided by adapter host
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_api_initialize(
        _In_ uint64_t flags,
        _In_ const otai_service_method_table_t *services);

/**
 * @brief Retrieve a pointer to the C-style method table for desired OTAI
 * functionality as specified by the given otai_api_id.
 *
 * @param[in] api The API ID whose method table is being retrieved.
 * @param[out] api_method_table Caller allocated method table. The table must
 * remain valid until the otai_api_uninitialize() is called.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_api_query(
        _In_ otai_api_t api,
        _Out_ void **api_method_table);

/**
 * @brief Uninitialize adapter linecard. OTAI functionalities,
 * retrieved via otai_api_query() cannot be used after this call.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_api_uninitialize(void);

/**
 * @brief Set log level for OTAI API module
 *
 * The default log level is #OTAI_LOG_LEVEL_WARN.
 *
 * @param[in] api The API ID whose logging level is being set
 * @param[in] log_level Log level
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_log_set(
        _In_ otai_api_t api,
        _In_ otai_log_level_t log_level);

/**
 * @brief Query OTAI object type.
 *
 * @param[in] object_id Object id
 *
 * @return #OTAI_OBJECT_TYPE_NULL when otai_object_id is not valid.
 * Otherwise, return a valid OTAI object type OTAI_OBJECT_TYPE_XXX.
 */
otai_object_type_t otai_object_type_query(
        _In_ otai_object_id_t object_id);

/**
 * @brief Generate dump file. The dump file may include OTAI state information
 *        and vendor SDK information.
 *
 * @param[in] dump_file_name Full path for dump file
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_dbg_generate_dump(
        _In_ const char *dump_file_name);

/**
 * @brief Get OTAI object type resource availability.
 *
 * @param[in] linecard_gid OTAI Linecard object id
 * @param[in] object_type OTAI object type
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list List of attributes that to distinguish resource
 * @param[out] count Available objects left
 *
 * @return #OTAI_STATUS_NOT_SUPPORTED if the given object type does not support resource accounting.
 * Otherwise, return #OTAI_STATUS_SUCCESS.
 */
otai_status_t otai_object_type_get_availability(
        _In_ otai_object_id_t linecard_gid,
        _In_ otai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list,
        _Out_ uint64_t *count);

/**
 * @}
 */
#endif /** __OTAI_H_ */
