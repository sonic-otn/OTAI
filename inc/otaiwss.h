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
 * @file    otaiwss.h
 * @brief   This module defines the WSS for the OTAI
 */

#if !defined (__OTAIWSS_H_)
#define __OTAIWSS_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIWSS OTAI - WSS specific API definitions
 *
 * @{
 */

/**
 * @brief WSS attribute IDs
 */
typedef enum _otai_wss_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_WSS_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_WSS_ATTR_ID = OTAI_WSS_ATTR_START,

    /**
     * @brief Serial No
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_SERIAL_NO,

    /**
     * @brief Part no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_PART_NO,

    /**
     * @brief Manufacture name
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_MFG_NAME,

    /**
     * @brief Manufacture date
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_MFG_DATE,

    /**
     * @brief Hardware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_HARDWARE_VERSION,

    /**
     * @brief The operational state of the WSS
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_OPER_STATUS,

    /**
     * @brief Whether the WSS is present or not
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_EMPTY,

    /**
     * @brief Whether the WSS is removable
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_REMOVABLE,

    /**
     * @brief Software version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_WSS_ATTR_SOFTWARE_VERSION,

    /**
     * @brief End of attributes
     */
    OTAI_WSS_ATTR_END,

    /** Custom range base value */
    OTAI_WSS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_WSS_ATTR_CUSTOM_RANGE_END

} otai_wss_attr_t;

/**
 * @brief WSS stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_wss_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_WSS_STAT_START,

    /**
     * @brief A placeholder
     *
     * @type otai_uint64_t
     */
    OTAI_WSS_STAT_NOT_USE = OTAI_WSS_STAT_START,

    /**
     * @brief End of statistics
     */
    OTAI_WSS_STAT_END,

} otai_wss_stat_t;

/**
 * @brief Create WSS.
 *
 * Allocates and initializes a WSS.
 *
 * @param[out] wss_id WSS id
 * @param[in] linecard_id Linecard id on which the WSS exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_wss_fn)(
        _Out_ otai_object_id_t *wss_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove WSS
 *
 * @param[in] wss_id WSS id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_wss_fn)(
        _In_ otai_object_id_t wss_id);

/**
 * @brief Set WSS attribute
 *
 * @param[in] wss_id WSS id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_wss_attribute_fn)(
        _In_ otai_object_id_t wss_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get WSS attribute
 *
 * @param[in] wss_id WSS id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_wss_attribute_fn)(
        _In_ otai_object_id_t wss_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get WSS statistics.
 *
 * @param[in] wss_id WSS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_wss_stats_fn)(
        _In_ otai_object_id_t wss_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get WSS statistics extended.
 *
 * @param[in] wss_id WSS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_wss_stats_ext_fn)(
        _In_ otai_object_id_t wss_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear WSS statistics counters.
 *
 * @param[in] wss_id WSS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_wss_stats_fn)(
        _In_ otai_object_id_t wss_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_wss_api_t
{
    otai_create_wss_fn                create_wss;
    otai_remove_wss_fn                remove_wss;
    otai_set_wss_attribute_fn         set_wss_attribute;
    otai_get_wss_attribute_fn         get_wss_attribute;
    otai_get_wss_stats_fn             get_wss_stats;
    otai_get_wss_stats_ext_fn         get_wss_stats_ext;
    otai_clear_wss_stats_fn           clear_wss_stats;
} otai_wss_api_t;

/**
 * @}
 */
#endif /** __OTAIWSS_H_ */
