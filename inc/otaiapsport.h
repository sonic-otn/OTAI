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
 * @file    otaiapsport.h
 * @brief   This module defines the APSPORT for the OTAI
 */

#if !defined (__OTAIAPSPORT_H_)
#define __OTAIAPSPORT_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIAPSPORT OTAI - APSPORT specific API definitions
 *
 * @{
 */

/** @brief APSPORT port type */
typedef enum _otai_apsport_port_type_t
{
    OTAI_APSPORT_PORT_TYPE_LINE_PRIMARY_IN,
    OTAI_APSPORT_PORT_TYPE_LINE_SECONDARY_IN,
    OTAI_APSPORT_PORT_TYPE_COMMON_IN,
    OTAI_APSPORT_PORT_TYPE_LINE_PRIMARY_OUT,
    OTAI_APSPORT_PORT_TYPE_LINE_SECONDARY_OUT,
    OTAI_APSPORT_PORT_TYPE_COMMON_OUTPUT
} otai_apsport_port_type_t;

/**
 * @brief APSPORT attribute IDs
 */
typedef enum _otai_apsport_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_APSPORT_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_APSPORT_ATTR_ID = OTAI_APSPORT_ATTR_START,

    /**
     * @brief Port type
     *
     * @type otai_apsport_port_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_APSPORT_ATTR_PORT_TYPE,

    /**
     * @brief Power LOS threshold
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_APSPORT_ATTR_POWER_LOS_THRESHOLD,

    /**
     * @brief Power low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APSPORT_ATTR_POWER_LOW_THRESHOLD,

    /**
     * @brief Enabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    OTAI_APSPORT_ATTR_ENABLED,

    /**
     * @brief Target attenuation
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APSPORT_ATTR_TARGET_ATTENUATION,

    /**
     * @brief End of attributes
     */
    OTAI_APSPORT_ATTR_END,

    /** Custom range base value */
    OTAI_APSPORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_APSPORT_ATTR_CUSTOM_RANGE_END

} otai_apsport_attr_t;

/**
 * @brief APSPORT stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_apsport_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_APSPORT_STAT_START,

    /**
     * @brief Optical power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_APSPORT_STAT_OPTICAL_POWER = OTAI_APSPORT_STAT_START,

    /**
     * @brief Attenuation
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_APSPORT_STAT_ATTENUATION,

    /**
     * @brief End of statistics
     */
    OTAI_APSPORT_STAT_END,

} otai_apsport_stat_t;

/**
 * @brief Create APSPORT.
 *
 * Allocates and initializes a APSPORT.
 *
 * @param[out] apsport_id APSPORT id
 * @param[in] linecard_id Linecard id on which the APSPORT exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_apsport_fn)(
        _Out_ otai_object_id_t *apsport_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove APSPORT
 *
 * @param[in] apsport_id APSPORT id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_apsport_fn)(
        _In_ otai_object_id_t apsport_id);

/**
 * @brief Set APSPORT attribute
 *
 * @param[in] apsport_id APSPORT id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_apsport_attribute_fn)(
        _In_ otai_object_id_t apsport_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get APSPORT attribute
 *
 * @param[in] apsport_id APSPORT id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_apsport_attribute_fn)(
        _In_ otai_object_id_t apsport_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get APSPORT statistics.
 *
 * @param[in] apsport_id APSPORT id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_apsport_stats_fn)(
        _In_ otai_object_id_t apsport_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get APSPORT statistics extended.
 *
 * @param[in] apsport_id APSPORT id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_apsport_stats_ext_fn)(
        _In_ otai_object_id_t apsport_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear APSPORT statistics counters.
 *
 * @param[in] apsport_id APSPORT id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_apsport_stats_fn)(
        _In_ otai_object_id_t apsport_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_apsport_api_t
{
    otai_create_apsport_fn                create_apsport;
    otai_remove_apsport_fn                remove_apsport;
    otai_set_apsport_attribute_fn         set_apsport_attribute;
    otai_get_apsport_attribute_fn         get_apsport_attribute;
    otai_get_apsport_stats_fn             get_apsport_stats;
    otai_get_apsport_stats_ext_fn         get_apsport_stats_ext;
    otai_clear_apsport_stats_fn           clear_apsport_stats;
} otai_apsport_api_t;

/**
 * @}
 */
#endif /** __OTAIAPSPORT_H_ */
