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
 * @file    otaiocm.h
 * @brief   This module defines the OCM for the OTAI
 */

#if !defined (__OTAIOCM_H_)
#define __OTAIOCM_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOCM OTAI - OCM specific API definitions
 *
 * @{
 */

/**
 * @brief OCM attribute IDs
 */
typedef enum _otai_ocm_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_OCM_ATTR_START,

    /**
     * @brief Id
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OCM_ATTR_ID = OTAI_OCM_ATTR_START,

    /**
     * @brief Scan
     *
     * @type bool
     * @flags SET_ONLY
     * @isrecoverable false
     */
    OTAI_OCM_ATTR_SCAN,

    /**
     * @brief Frequency granularity in MHz
     *
     * @type otai_uint64_t
     * @flags CREATE_AND_SET
     */
    OTAI_OCM_ATTR_FREQUENCY_GRANULARITY,

    /**
     * @brief Serial No
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_SERIAL_NO,

    /**
     * @brief Part no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_PART_NO,

    /**
     * @brief Manufacture name
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_MFG_NAME,

    /**
     * @brief Manufacture date
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_MFG_DATE,

    /**
     * @brief Hardware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_HARDWARE_VERSION,

    /**
     * @brief The operational state of the OCM
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_OPER_STATUS,

    /**
     * @brief Whether the OCM is present or not
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_EMPTY,

    /**
     * @brief Whether the OCM is removable
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_REMOVABLE,

    /**
     * @brief Software version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_SOFTWARE_VERSION,

    /**
     * @brief Firmware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_FIRMWARE_VERSION,

    /**
     * @brief Insertion loss to panel port
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OCM_ATTR_INSERTION_LOSS_TO_PANEL_PORT,

    /**
     * @brief End of attributes
     */
    OTAI_OCM_ATTR_END,

    /** Custom range base value */
    OTAI_OCM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_OCM_ATTR_CUSTOM_RANGE_END

} otai_ocm_attr_t;

/**
 * @brief OCM stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_ocm_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_OCM_STAT_START,

    /**
     * @brief A placeholder
     *
     * @type otai_uint64_t
     */
    OTAI_OCM_STAT_NOT_USE = OTAI_OCM_STAT_START,

    /**
     * @brief End of statistics
     */
    OTAI_OCM_STAT_END,

} otai_ocm_stat_t;

/**
 * @brief Create OCM.
 *
 * Allocates and initializes a OCM.
 *
 * @param[out] ocm_id OCM id
 * @param[in] linecard_id Linecard id on which the OCM exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_ocm_fn)(
        _Out_ otai_object_id_t *ocm_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove OCM
 *
 * @param[in] ocm_id OCM id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_ocm_fn)(
        _In_ otai_object_id_t ocm_id);

/**
 * @brief Set OCM attribute
 *
 * @param[in] ocm_id OCM id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_ocm_attribute_fn)(
        _In_ otai_object_id_t ocm_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get OCM attribute
 *
 * @param[in] ocm_id OCM id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_ocm_attribute_fn)(
        _In_ otai_object_id_t ocm_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get OCM statistics.
 *
 * @param[in] ocm_id OCM id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_ocm_stats_fn)(
        _In_ otai_object_id_t ocm_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get OCM statistics extended.
 *
 * @param[in] ocm_id OCM id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_ocm_stats_ext_fn)(
        _In_ otai_object_id_t ocm_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear OCM statistics counters.
 *
 * @param[in] ocm_id OCM id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_ocm_stats_fn)(
        _In_ otai_object_id_t ocm_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_ocm_api_t
{
    otai_create_ocm_fn                create_ocm;
    otai_remove_ocm_fn                remove_ocm;
    otai_set_ocm_attribute_fn         set_ocm_attribute;
    otai_get_ocm_attribute_fn         get_ocm_attribute;
    otai_get_ocm_stats_fn             get_ocm_stats;
    otai_get_ocm_stats_ext_fn         get_ocm_stats_ext;
    otai_clear_ocm_stats_fn           clear_ocm_stats;
} otai_ocm_api_t;

/**
 * @}
 */
#endif /** __OTAIOCM_H_ */
