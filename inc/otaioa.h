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
 * @file    otaioa.h
 * @brief   This module defines the OA for the OTAI
 */

#if !defined (__OTAIOA_H_)
#define __OTAIOA_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOA OTAI - OA specific API definitions
 *
 * @{
 */

/** @brief OA type */
typedef enum _otai_oa_type_t
{
    OTAI_OA_TYPE_EDFA,
    OTAI_OA_TYPE_FORWARD_RAMAN,
    OTAI_OA_TYPE_BACKWARD_RAMAN,
    OTAI_OA_TYPE_HYBRID,
} otai_oa_type_t;

/** @brief OA gain range */
typedef enum _otai_oa_gain_range_t
{
    OTAI_OA_GAIN_RANGE_LOW_GAIN_RANGE,
    OTAI_OA_GAIN_RANGE_MID_GAIN_RANGE,
    OTAI_OA_GAIN_RANGE_HIGH_GAIN_RANGE,
    OTAI_OA_GAIN_RANGE_FIXED_GAIN_RANGE,
} otai_oa_gain_range_t;

/** @brief OA amp mode */
typedef enum _otai_oa_amp_mode_t
{
    OTAI_OA_AMP_MODE_CONSTANT_POWER,
    OTAI_OA_AMP_MODE_CONSTANT_GAIN,
    OTAI_OA_AMP_MODE_DYNAMIC_GAIN,
    OTAI_OA_AMP_MODE_DYNAMIC_POWER,
} otai_oa_amp_mode_t;

/** @brief OA fiber type profile */
typedef enum _otai_oa_fiber_type_profile_t
{
    OTAI_OA_FIBER_TYPE_PROFILE_DSF,
    OTAI_OA_FIBER_TYPE_PROFILE_LEAF,
    OTAI_OA_FIBER_TYPE_PROFILE_SSMF,
    OTAI_OA_FIBER_TYPE_PROFILE_TWC,
    OTAI_OA_FIBER_TYPE_PROFILE_TWRS,
} otai_oa_fiber_type_profile_t;

/** @brief OA working state */
typedef enum _otai_oa_working_state_t
{
    OTAI_OA_WORKING_STATE_LOS_A,
    OTAI_OA_WORKING_STATE_LOS_N,
} otai_oa_working_state_t;

/**
 * @brief OA attribute IDs
 */
typedef enum _otai_oa_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_OA_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OA_ATTR_ID = OTAI_OA_ATTR_START,

    /**
     * @brief Type
     *
     * @type otai_oa_type_t
     * @flags READ_ONLY
     */
    OTAI_OA_ATTR_TYPE,

    /**
     * @brief Target gain
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 15.0
     */
    OTAI_OA_ATTR_TARGET_GAIN,

    /**
     * @brief Min gain
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OA_ATTR_MIN_GAIN,

    /**
     * @brief Max gain
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_OA_ATTR_MAX_GAIN,

    /**
     * @brief Target gain tilt
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    OTAI_OA_ATTR_TARGET_GAIN_TILT,

    /**
     * @brief Gain range
     *
     * @type otai_oa_gain_range_t
     * @flags CREATE_AND_SET
     * @default OTAI_OA_GAIN_RANGE_LOW_GAIN_RANGE
     */
    OTAI_OA_ATTR_GAIN_RANGE,

    /**
     * @brief Amp mode
     *
     * @type otai_oa_amp_mode_t
     * @flags CREATE_AND_SET
     * @default OTAI_OA_AMP_MODE_CONSTANT_GAIN
     */
    OTAI_OA_ATTR_AMP_MODE,

    /**
     * @brief Target output power
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 15.0
     */
    OTAI_OA_ATTR_TARGET_OUTPUT_POWER,

    /**
     * @brief Max output power
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 25.0
     */
    OTAI_OA_ATTR_MAX_OUTPUT_POWER,

    /**
     * @brief Enabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    OTAI_OA_ATTR_ENABLED,

    /**
     * @brief Fiber type profile
     *
     * @type otai_oa_fiber_type_profile_t
     * @flags CREATE_AND_SET
     * @default OTAI_OA_FIBER_TYPE_PROFILE_SSMF
     */
    OTAI_OA_ATTR_FIBER_TYPE_PROFILE,

    /**
     * @brief Ingress port
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OA_ATTR_INGRESS_PORT,

    /**
     * @brief Egress port
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OA_ATTR_EGRESS_PORT,

    /**
     * @brief Working state
     *
     * @type otai_oa_working_state_t
     * @flags CREATE_AND_SET
     * @default OTAI_OA_WORKING_STATE_LOS_A
     */
    OTAI_OA_ATTR_WORKING_STATE,

    /**
     * @brief Input LOS threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -30.0
     */
    OTAI_OA_ATTR_INPUT_LOS_THRESHOLD,

    /**
     * @brief Input LOS hysteresis
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 3.0
     */
    OTAI_OA_ATTR_INPUT_LOS_HYSTERESIS,

    /**
     * @brief Output LOS threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -10.0
     */
    OTAI_OA_ATTR_OUTPUT_LOS_THRESHOLD,

    /**
     * @brief Output LOS hysteresis
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 3.0
     */
    OTAI_OA_ATTR_OUTPUT_LOS_HYSTERESIS,

    /**
     * @brief Gain low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 5.0
     */
    OTAI_OA_ATTR_GAIN_LOW_THRESHOLD,

    /**
     * @brief Gain low hysteresis
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 1.0
     */
    OTAI_OA_ATTR_GAIN_LOW_HYSTERESIS,

    /**
     * @brief Input low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -20.0
     */
    OTAI_OA_ATTR_INPUT_LOW_THRESHOLD,

    /**
     * @brief Output low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    OTAI_OA_ATTR_OUTPUT_LOW_THRESHOLD,

    /**
     * @brief Enable APR node
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    OTAI_OA_ATTR_APR_NODE_ENABLE,

    /**
     * @brief APR node reflection threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -20.0
     */
    OTAI_OA_ATTR_APR_NODE_REFLECTION_THRESHOLD,

    /**
     * @brief Enable APR line
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    OTAI_OA_ATTR_APR_LINE_ENABLE,

    /**
     * @brief End of attributes
     */
    OTAI_OA_ATTR_END,

    /** Custom range base value */
    OTAI_OA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_OA_ATTR_CUSTOM_RANGE_END

} otai_oa_attr_t;

/**
 * @brief OA stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_oa_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_OA_STAT_START,

    /**
     * @brief Temperature
     *
     * @type otai_double_t
     * @precision precision1
     * @iscounter false
     */
    OTAI_OA_STAT_TEMPERATURE = OTAI_OA_STAT_START,

    /**
     * @brief Actual gain
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_ACTUAL_GAIN,

    /**
     * @brief Actual gain tilt
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_ACTUAL_GAIN_TILT,

    /**
     * @brief Input power total
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_INPUT_POWER_TOTAL,

    /**
     * @brief Input power C band
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_INPUT_POWER_C_BAND,

    /**
     * @brief Input power L band
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_INPUT_POWER_L_BAND,

    /**
     * @brief Output power total
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_OUTPUT_POWER_TOTAL,

    /**
     * @brief Output power C band
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_OUTPUT_POWER_C_BAND,

    /**
     * @brief Output power L band
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_OUTPUT_POWER_L_BAND,

    /**
     * @brief Laser bias current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_LASER_BIAS_CURRENT,

    /**
     * @brief Optical return loss
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_OPTICAL_RETURN_LOSS,

    /**
     * @brief Laser temperature
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_LASER_TEMPERATURE,

    /**
     * @brief Panel input power primary-line rx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_PANEL_INPUT_POWER_LINEP_RX,

    /**
     * @brief Panel output power primary-line tx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_PANEL_OUTPUT_POWER_LINEP_TX,

    /**
     * @brief Panel input power secondary-line rx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_PANEL_INPUT_POWER_LINES_RX,

    /**
     * @brief Panel output power secondary-line tx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_PANEL_OUTPUT_POWER_LINES_TX,

    /**
     * @brief Laser TEC current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_LASER_TEC_CURRENT,

    /**
     * @brief Ingress VOA attenuation
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_INGRESS_VOA_ATTEN,

    /**
     * @brief Monitor port output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OA_STAT_MON_OUTPUT_POWER,

    /**
     * @brief End of statistics
     */
    OTAI_OA_STAT_END,

} otai_oa_stat_t;

/**
 * @brief Create OA.
 *
 * Allocates and initializes a OA.
 *
 * @param[out] oa_id OA id
 * @param[in] linecard_id Linecard id on which the OA exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_oa_fn)(
        _Out_ otai_object_id_t *oa_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove OA
 *
 * @param[in] oa_id OA id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_oa_fn)(
        _In_ otai_object_id_t oa_id);

/**
 * @brief Set OA attribute
 *
 * @param[in] oa_id OA id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_oa_attribute_fn)(
        _In_ otai_object_id_t oa_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get OA attribute
 *
 * @param[in] oa_id OA id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_oa_attribute_fn)(
        _In_ otai_object_id_t oa_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get OA statistics.
 *
 * @param[in] oa_id OA id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_oa_stats_fn)(
        _In_ otai_object_id_t oa_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get OA statistics extended.
 *
 * @param[in] oa_id OA id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_oa_stats_ext_fn)(
        _In_ otai_object_id_t oa_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear OA statistics counters.
 *
 * @param[in] oa_id OA id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_oa_stats_fn)(
        _In_ otai_object_id_t oa_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_oa_api_t
{
    otai_create_oa_fn                create_oa;
    otai_remove_oa_fn                remove_oa;
    otai_set_oa_attribute_fn         set_oa_attribute;
    otai_get_oa_attribute_fn         get_oa_attribute;
    otai_get_oa_stats_fn             get_oa_stats;
    otai_get_oa_stats_ext_fn         get_oa_stats_ext;
    otai_clear_oa_stats_fn           clear_oa_stats;
} otai_oa_api_t;

/**
 * @}
 */
#endif /** __OTAIOA_H_ */
