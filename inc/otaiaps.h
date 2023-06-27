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
 * @file    otaiaps.h
 * @brief   This module defines the APS for the OTAI
 */

#if !defined (__OTAIAPS_H_)
#define __OTAIAPS_H_

#include <otaitypes.h>

/** @brief APS type */
typedef enum _otai_aps_type_t
{
    OTAI_APS_TYPE_APS,
} otai_aps_type_t;

/** @brief APS force to port */
typedef enum _otai_aps_force_to_port_t
{
    OTAI_APS_FORCE_TO_PORT_NONE,
    OTAI_APS_FORCE_TO_PORT_PRIMARY,
    OTAI_APS_FORCE_TO_PORT_SECONDARY,
} otai_aps_force_to_port_t;

/** @brief APS active path */
typedef enum _otai_aps_active_path_t
{
    OTAI_APS_ACTIVE_PATH_PRIMARY,
    OTAI_APS_ACTIVE_PATH_SECONDARY,

} otai_aps_active_path_t;

typedef enum _otai_olp_switch_reason_t
{
    OTAI_OLP_SWITCH_REASON_AUTO_ABSOLUTE,
    OTAI_OLP_SWITCH_REASON_AUTO_RELATIVE,
    OTAI_OLP_SWITCH_REASON_MANUAL_CMD,
    OTAI_OLP_SWITCH_REASON_FORCE_CMD,
    OTAI_OLP_SWITCH_REASON_BUTTON_TRIGGER,

} otai_olp_switch_reason_t;

typedef enum _otai_olp_switch_operate_t
{
    OTAI_OLP_SWITCH_OPERATE_PRIMARY_TO_SECONDARY,
    OTAI_OLP_SWITCH_OPERATE_SECONDARY_TO_PRIMARY,

} otai_olp_switch_operate_t;

typedef enum _otai_olp_switch_interval_t
{
    OTAI_OLP_SWITCH_INTERVAL_1MS,
    OTAI_OLP_SWITCH_INTERVAL_2MS,
    OTAI_OLP_SWITCH_INTERVAL_10MS,

} otai_olp_switch_interval_t;

typedef struct _otai_olp_switch_power_info_t
{
    otai_double_t common_out;
    otai_double_t primary_in;
    otai_double_t secondary_in;
} otai_olp_switch_power_info_t;

typedef struct _otai_olp_switch_info_t
{
    otai_uint16_t index;
    otai_olp_switch_reason_t reason;
    otai_olp_switch_operate_t operate;
    otai_uint64_t time_stamp;
    otai_olp_switch_power_info_t before[40];
    otai_olp_switch_power_info_t switching;
    otai_olp_switch_power_info_t after[40];
} otai_olp_switch_info_t;

typedef struct _otai_olp_switch_t
{
    otai_uint8_t num;
    otai_uint8_t type;
    otai_uint8_t interval;
    otai_uint16_t pointers;
    otai_uint8_t channel_id;
    otai_olp_switch_info_t info[10];
} otai_olp_switch_t;

/**
 * @brief APS report switch info notification
 *
 * @param[in] aps_id APS Id
 * @param[in] switch_info Switch info
 */
typedef void (*otai_aps_report_switch_info_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ otai_olp_switch_t switch_info);

/**
 * @defgroup OTAIAPS OTAI - APS specific API definitions
 *
 * @{
 */

/**
 * @brief APS attribute IDs
 */
typedef enum _otai_aps_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_APS_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_APS_ATTR_ID = OTAI_APS_ATTR_START,

    /**
     * @brief Empty
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_EMPTY,

    /**
     * @brief Removable
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_REMOVABLE,

    /**
     * @brief Serial no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_SERIAL_NO,

    /**
     * @brief Equipment failure
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_EQUIPMENT_FAILURE,

    /**
     * @brief Equipment mismatch
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_EQUIPMENT_MISMATCH,

    /**
     * @brief Hardware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_HARDWARE_VERSION,

    /**
     * @brief Admin state
     *
     * @type otai_admin_state_t
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_ADMIN_STATE,

    /**
     * @brief Operational status
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_OPER_STATUS,

    /**
     * @brief Part no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_PART_NO,

    /**
     * @brief Mfg name
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_MFG_NAME,

    /**
     * @brief Manufacture date
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_MFG_DATE,

    /**
     * @brief Software version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_SOFTWARE_VERSION,

    /**
     * @brief Firmware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_APS_ATTR_FIRMWARE_VERSION,

    /**
     * @brief Type
     *
     * @type otai_aps_type_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_TYPE,

    /**
     * @brief Revertive
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_REVERTIVE,

    /**
     * @brief Wait to restore time
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_WAIT_TO_RESTORE_TIME,

    /**
     * @brief Hold off time
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_HOLD_OFF_TIME,

    /**
     * @brief Primary switch threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_PRIMARY_SWITCH_THRESHOLD,

    /**
     * @brief Primary switch hysteresis
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_PRIMARY_SWITCH_HYSTERESIS,

    /**
     * @brief Secondary switch threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_SECONDARY_SWITCH_THRESHOLD,

    /**
     * @brief Relative switch threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD,

    /**
     * @brief Relative switch threshold offset
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD_OFFSET,

    /**
     * @brief Force to port
     *
     * @type otai_aps_force_to_port_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_FORCE_TO_PORT,

    /**
     * @brief Active path
     *
     * @type otai_aps_active_path_t
     * @flags CREATE_AND_SET
     * @isrecoverable false
     */
    OTAI_APS_ATTR_ACTIVE_PATH,

    /**
     * @brief Alarm hysteresis
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_APS_ATTR_ALARM_HYSTERESIS,

    /**
     * @brief Switch info notify
     *
     * @type otai_pointer_t otai_aps_report_switch_info_fn
     * @flags CREATE_ONLY
     * @default NULL
     */
    OTAI_APS_ATTR_SWITCH_INFO_NOTIFY,

    /**
     * @brief Collect switch info
     *
     * @type bool
     * @flags SET_ONLY
     * @isrecoverable false
     */
    OTAI_APS_ATTR_COLLECT_SWITCH_INFO,

    /**
     * @brief End of attributes
     */
    OTAI_APS_ATTR_END,

    /** Custom range base value */
    OTAI_APS_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_APS_ATTR_CUSTOM_RANGE_END

} otai_aps_attr_t;

/**
 * @brief APS stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_aps_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_APS_STAT_START,

    /**
     * @brief A placeholder
     *
     * @type otai_uint64_t
     */
    OTAI_APS_STAT_NOT_USE = OTAI_APS_STAT_START,

    /**
     * @brief End of statistics
     */
    OTAI_APS_STAT_END,

} otai_aps_stat_t;

/**
 * @brief Create APS.
 *
 * Allocates and initializes a APS.
 *
 * @param[out] aps_id APS id
 * @param[in] linecard_id Linecard id on which the APS exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_aps_fn)(
        _Out_ otai_object_id_t *aps_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove APS
 *
 * @param[in] aps_id APS id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_aps_fn)(
        _In_ otai_object_id_t aps_id);

/**
 * @brief Set APS attribute
 *
 * @param[in] aps_id APS id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_aps_attribute_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get APS attribute
 *
 * @param[in] aps_id APS id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_aps_attribute_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get APS statistics.
 *
 * @param[in] aps_id APS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_aps_stats_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get APS statistics extended.
 *
 * @param[in] aps_id APS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_aps_stats_ext_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear APS statistics counters.
 *
 * @param[in] aps_id APS id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_aps_stats_fn)(
        _In_ otai_object_id_t aps_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_aps_api_t
{
    otai_create_aps_fn                create_aps;
    otai_remove_aps_fn                remove_aps;
    otai_set_aps_attribute_fn         set_aps_attribute;
    otai_get_aps_attribute_fn         get_aps_attribute;
    otai_get_aps_stats_fn             get_aps_stats;
    otai_get_aps_stats_ext_fn         get_aps_stats_ext;
    otai_clear_aps_stats_fn           clear_aps_stats;
} otai_aps_api_t;

/**
 * @}
 */
#endif /** __OTAIAPS_H_ */
