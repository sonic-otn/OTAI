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

#include "otaiobject.h"
#include "otailinecard.h"
#include "otaiport.h"
#include "otaitransceiver.h"
#include "otailogicalchannel.h"
#include "otaiotn.h"
#include "otaiethernet.h"
#include "otaiphysicalchannel.h"
#include "otaioch.h"
#include "otailldp.h"
#include "otaiassignment.h"
#include "otaistatus.h"
#include "otaitypes.h"
#include "otaiaps.h"
#include "otaiapsport.h"
#include "otaiattenuator.h"
#include "otaiinterface.h"
#include "otaioa.h"
#include "otaiosc.h"
#include "otaiwss.h"
#include "otaimediachannel.h"
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
    OTAI_API_LINECARD         =  1, /**< otai_linecard_api_t */
    OTAI_API_PORT             =  2, /**< otai_port_api_t */
    OTAI_API_TRANSCEIVER      =  3, /**< otai_transceiver_api_t */
    OTAI_API_LOGICALCHANNEL   =  4, /**< otai_logicalchannel_api_t */
    OTAI_API_OTN              =  5, /**< otai_otn_api_t */
    OTAI_API_ETHERNET         =  6, /**< otai_ethernet_api_t */
    OTAI_API_PHYSICALCHANNEL  =  7, /**< otai_physicalchannel_api_t */
    OTAI_API_OCH              =  8, /**< otai_och_api_t */
    OTAI_API_LLDP             =  9, /**< otai_lldp_api_t */
    OTAI_API_ASSIGNMENT       =  10, /**< otai_assignment_api_t */
    OTAI_API_INTERFACE        =  11, /**< otai_interface_api_t */
    OTAI_API_OA               =  12, /**< otai_oa_api_t */
    OTAI_API_OSC              =  13, /**< otai_osc_api_t */
    OTAI_API_APS              =  14, /**< otai_aps_api_t */
    OTAI_API_APSPORT          =  15, /**< otai_apsport_api_t */
    OTAI_API_ATTENUATOR       =  16, /**< otai_attenuator_api_t */
    OTAI_API_WSS              =  17, /**< otai_wss_api_t */
    OTAI_API_MEDIACHANNEL     =  18, /**< otai_mediachannel_api_t */
    OTAI_API_OCM              =  19, /**< otai_ocm_api_t */
    OTAI_API_OTDR             =  20, /**< otai_otdr_api_t */
    OTAI_API_MAX              =  21, /**< total number of APIs */
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
 * @brief Query OTAI linecard id.
 *
 * @param[in] object_id Object id
 *
 * @return #OTAI_NULL_OBJECT_ID when otai_object_id is not valid.
 * Otherwise, return a valid OTAI_OBJECT_TYPE_LINECARD object on which provided
 * object id belongs. If valid linecard id object is provided as input parameter
 * it should return itself.
 */
otai_object_id_t otai_linecard_id_query(
        _In_ otai_object_id_t object_id);

/**
 * @brief Link check.
 *
 * @param[out] up Whether link is up or not.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
otai_status_t otai_link_check(
        _Out_ bool *up);

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
 * @}
 */
#endif /** __OTAI_H_ */
