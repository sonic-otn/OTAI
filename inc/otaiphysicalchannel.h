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
 * @file    otaiphysicalchannel.h
 * @brief   This module defines the physical channel for the OTAI
 */

#if !defined (__OTAIPHYSICALCHANNEL_H_)
#define __OTAIPHYSICALCHANNEL_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIPHYSICALCHANNEL OTAI - Physical channel specific API definitions
 *
 * @{
 */

/**
 * @brief Physical channel attribute IDs
 */
typedef enum _otai_physicalchannel_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_PHYSICALCHANNEL_ATTR_START,

    /**
     * @brief Client port or line port.
     *
     * @type otai_port_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_PHYSICALCHANNEL_ATTR_PORT_TYPE = OTAI_PHYSICALCHANNEL_ATTR_START,

    /**
     * @brief Port ID.
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_PHYSICALCHANNEL_ATTR_PORT_ID,

    /**
     * @brief Lane ID.
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_PHYSICALCHANNEL_ATTR_LANE_ID,

    /**
     * @brief Tx laser
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_PHYSICALCHANNEL_ATTR_TX_LASER,

    /**
     * @brief The frequency in MHz of the individual physical channel
     *
     * @type otai_uint64_t
     * @flags READ_ONLY
     */
    OTAI_PHYSICALCHANNEL_ATTR_OUTPUT_FREQUENCY,

    /**
     * @brief End of attributes
     */
    OTAI_PHYSICALCHANNEL_ATTR_END,

    /** Custom range base value */
    OTAI_PHYSICALCHANNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_PHYSICALCHANNEL_ATTR_CUSTOM_RANGE_END

} otai_physicalchannel_attr_t;

/**
 * @brief Physical channel stat IDs in otai_get_physicalchannel_stats() call
 *
 * @flags Contains flags
 */
typedef enum _otai_physicalchannel_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_PHYSICALCHANNEL_STAT_START,

    /**
     * @brief Output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PHYSICALCHANNEL_STAT_OUTPUT_POWER = OTAI_PHYSICALCHANNEL_STAT_START,

    /**
     * @brief Input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PHYSICALCHANNEL_STAT_INPUT_POWER,

    /**
     * @brief Laser bias current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_PHYSICALCHANNEL_STAT_LASER_BIAS_CURRENT,

    /**
     * @brief End of statistics
     */
    OTAI_PHYSICALCHANNEL_STAT_END,

} otai_physicalchannel_stat_t;

/**
 * @brief Create physical channel.
 *
 * Allocates and initializes a physical channel.
 *
 * @param[out] physicalchannel_id Physical channel id
 * @param[in] linecard_id Linecard id on which the physical channel exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_physicalchannel_fn)(
        _Out_ otai_object_id_t *physicalchannel_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove physical channel
 *
 * @param[in] physicalchannel_id Physical channel id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_physicalchannel_fn)(
        _In_ otai_object_id_t physicalchannel_id);

/**
 * @brief Set physical channel attribute
 *
 * @param[in] physicalchannel_id Physical channel id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_physicalchannel_attribute_fn)(
        _In_ otai_object_id_t physicalchannel_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get physical channel attribute
 *
 * @param[in] physicalchannel_id Physical channel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_physicalchannel_attribute_fn)(
        _In_ otai_object_id_t physicalchannel_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get physical channel counters. Deprecated for backward compatibility.
 *
 * @param[in] physicalchannel_id Physical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_physicalchannel_stats_fn)(
        _In_ otai_object_id_t physicalchannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get physical channel statistics counters extended.
 *
 * @param[in] physicalchannel_id Physical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_physicalchannel_stats_ext_fn)(
        _In_ otai_object_id_t physicalchannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear physical channel statistics counters.
 *
 * @param[in] physicalchannel_id Physical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_physicalchannel_stats_fn)(
        _In_ otai_object_id_t physicalchannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_physicalchannel_api_t
{
    otai_create_physicalchannel_fn              create_physicalchannel;
    otai_remove_physicalchannel_fn              remove_physicalchannel;
    otai_set_physicalchannel_attribute_fn       set_physicalchannel_attribute;
    otai_get_physicalchannel_attribute_fn       get_physicalchannel_attribute;
    otai_get_physicalchannel_stats_fn           get_physicalchannel_stats;
    otai_get_physicalchannel_stats_ext_fn       get_physicalchannel_stats_ext;
    otai_clear_physicalchannel_stats_fn         clear_physicalchannel_stats;
} otai_physicalchannel_api_t;

/**
 * @}
 */
#endif /** __OTAIPHYSICALCHANNEL_H_ */
