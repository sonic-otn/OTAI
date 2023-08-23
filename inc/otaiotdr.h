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
 * @file    otaiotdr.h
 * @brief   This module defines the OTDR for the OTAI
 */

#if !defined (__OTAIOTDR_H_)
#define __OTAIOTDR_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOTDR OTAI - OTDR specific API definitions
 *
 * @{
 */

/**
 * @brief OTDR attribute IDs
 */
typedef enum _otai_otdr_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_OTDR_ATTR_START,

    /**
     * @brief Id
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OTDR_ATTR_ID = OTAI_OTDR_ATTR_START,

    /**
     * @brief Refractive index
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 1.4679
     */
    OTAI_OTDR_ATTR_REFRACTIVE_INDEX,

    /**
     * @brief Backscatter index
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 1.4679
     */
    OTAI_OTDR_ATTR_BACKSCATTER_INDEX,

    /**
     * @brief Reflection threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -40.0
     */
    OTAI_OTDR_ATTR_REFLECTION_THRESHOLD,

    /**
     * @brief Splice loss threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 2.5
     */
    OTAI_OTDR_ATTR_SPLICE_LOSS_THRESHOLD,

    /**
     * @brief End of fiber threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -40.0
     */
    OTAI_OTDR_ATTR_END_OF_FIBER_THRESHOLD,

    /**
     * @brief Distance range
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     * @default 60
     */
    OTAI_OTDR_ATTR_DISTANCE_RANGE,

    /**
     * @brief Pulse width
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     * @default 3000
     */
    OTAI_OTDR_ATTR_PULSE_WIDTH,

    /**
     * @brief Average time
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     * @default 60
     */
    OTAI_OTDR_ATTR_AVERAGE_TIME,

    /**
     * @brief Output frequency
     *
     * @type otai_uint64_t
     * @flags CREATE_AND_SET
     * @default 193414489
     */
    OTAI_OTDR_ATTR_OUTPUT_FREQUENCY,

    /**
     * @brief Enabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    OTAI_OTDR_ATTR_ENABLED,

    /**
     * @brief Scan
     *
     * @type bool
     * @flags SET_ONLY
     * @isrecoverable false
     * @default true
     */
    OTAI_OTDR_ATTR_SCAN,

    /**
     * @brief Start time
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    OTAI_OTDR_ATTR_START_TIME,

    /**
     * @brief Period
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    OTAI_OTDR_ATTR_PERIOD,

    /**
     * @brief Dynamic range
     *
     * @type otai_uint32_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_DYNAMIC_RANGE,

    /**
     * @brief Distance accuracy
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_DISTANCE_ACCURACY,

    /**
     * @brief Sampling resolution
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_SAMPLING_RESOLUTION,

    /**
     * @brief Loss dead zone
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_LOSS_DEAD_ZONE,

    /**
     * @brief Reflection dead zone
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_REFLECTION_DEAD_ZONE,

    /**
     * @brief Scanning status
     *
     * @type otai_scanning_status_t
     * @flags READ_ONLY
     */
    OTAI_OTDR_ATTR_SCANNING_STATUS,

    /**
     * @brief End of attributes
     */
    OTAI_OTDR_ATTR_END,

    /** Custom range base value */
    OTAI_OTDR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_OTDR_ATTR_CUSTOM_RANGE_END

} otai_otdr_attr_t;

/**
 * @brief OTDR stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_otdr_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_OTDR_STAT_START,

    /**
     * @brief A placeholder
     *
     * @type otai_uint64_t
     */
    OTAI_OTDR_STAT_NOT_USE = OTAI_OTDR_STAT_START,

    /**
     * @brief End of statistics
     */
    OTAI_OTDR_STAT_END,

} otai_otdr_stat_t;

/**
 * @brief Create OTDR.
 *
 * Allocates and initializes a OTDR.
 *
 * @param[out] otdr_id OTDR id
 * @param[in] linecard_id Linecard id on which the OTDR exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_otdr_fn)(
        _Out_ otai_object_id_t *otdr_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove OTDR
 *
 * @param[in] otdr_id OTDR id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_otdr_fn)(
        _In_ otai_object_id_t otdr_id);

/**
 * @brief Set OTDR attribute
 *
 * @param[in] otdr_id OTDR id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_otdr_attribute_fn)(
        _In_ otai_object_id_t otdr_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get OTDR attribute
 *
 * @param[in] otdr_id OTDR id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_otdr_attribute_fn)(
        _In_ otai_object_id_t otdr_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get OTDR statistics.
 *
 * @param[in] otdr_id OTDR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_otdr_stats_fn)(
        _In_ otai_object_id_t otdr_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get OTDR statistics extended.
 *
 * @param[in] otdr_id OTDR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_otdr_stats_ext_fn)(
        _In_ otai_object_id_t otdr_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear OTDR statistics counters.
 *
 * @param[in] otdr_id OTDR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_otdr_stats_fn)(
        _In_ otai_object_id_t otdr_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_otdr_api_t
{
    otai_create_otdr_fn                create_otdr;
    otai_remove_otdr_fn                remove_otdr;
    otai_set_otdr_attribute_fn         set_otdr_attribute;
    otai_get_otdr_attribute_fn         get_otdr_attribute;
    otai_get_otdr_stats_fn             get_otdr_stats;
    otai_get_otdr_stats_ext_fn         get_otdr_stats_ext;
    otai_clear_otdr_stats_fn           clear_otdr_stats;
} otai_otdr_api_t;

/**
 * @}
 */
#endif /** __OTAIOTDR_H_ */
