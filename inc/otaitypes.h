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
 * @file    otaitypes.h
 *
 * @brief   This module defines OTAI portable types
 */

#if !defined (__OTAITYPES_H_)
#define __OTAITYPES_H_

/**
 * @defgroup OTAITYPES OTAI - Types definitions
 *
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

typedef int32_t  otai_status_t;
typedef uint32_t otai_linecard_profile_id_t;
typedef uint32_t otai_attr_id_t;
typedef uint32_t otai_stat_id_t;

#define _In_
#define _Out_
#define _Inout_
#define _In_reads_z_(_LEN_)
#define _In_reads_opt_z_(_LEN_)

/*
 * New common definitions
 */

typedef uint64_t otai_uint64_t;
typedef int64_t otai_int64_t;
typedef uint32_t otai_uint32_t;
typedef int32_t otai_int32_t;
typedef uint16_t otai_uint16_t;
typedef int16_t otai_int16_t;
typedef uint8_t otai_uint8_t;
typedef int8_t otai_int8_t;
typedef float otai_float_t;
typedef double otai_double_t;
typedef size_t otai_size_t;
typedef uint64_t otai_object_id_t;
typedef void *otai_pointer_t;

typedef struct _otai_spectrum_power_t
{
    otai_uint64_t lower_frequency;
    otai_uint64_t upper_frequency;
    otai_double_t power;
} otai_spectrum_power_t;

/**
 * @def OTAI_NULL_OBJECT_ID
 * OTAI NULL object ID
 */
#define OTAI_NULL_OBJECT_ID 0L

/**
 * @brief Defines a list of OTAI object ids used as OTAI attribute value.
 *
 * In set attribute function call, the count member defines the number of
 * objects.
 *
 * In get attribute function call, the function call returns a list of objects
 * to the caller in the list member. The caller is responsible for allocating
 * the buffer for the list member and set the count member to the size of
 * allocated object list. If the size is large enough to accommodate the list of
 * object id, the callee will then fill the list member and set the count member
 * to the actual number of objects. If the list size is not large enough, the
 * callee will set the count member to the actual number of object id and return
 * #OTAI_STATUS_BUFFER_OVERFLOW. Once the caller gets such return code, it should
 * use the returned count member to re-allocate list and retry.
 */
typedef struct _otai_object_list_t
{
    uint32_t count;
    otai_object_id_t *list;
} otai_object_list_t;

/**
 * @brief OTAI common API type
 */
typedef enum _otai_common_api_t
{
    OTAI_COMMON_API_CREATE      = 0,
    OTAI_COMMON_API_REMOVE      = 1,
    OTAI_COMMON_API_SET         = 2,
    OTAI_COMMON_API_GET         = 3,
    OTAI_COMMON_API_BULK_CREATE = 4,
    OTAI_COMMON_API_BULK_REMOVE = 5,
    OTAI_COMMON_API_BULK_SET    = 6,
    OTAI_COMMON_API_BULK_GET    = 7,
    OTAI_COMMON_API_MAX         = 8,
} otai_common_api_t;

/**
 * @brief OTAI object type
 */
typedef enum _otai_object_type_t
{
    OTAI_OBJECT_TYPE_NULL                     =  0, /**< invalid object type */
    OTAI_OBJECT_TYPE_OA                       =  1,
    OTAI_OBJECT_TYPE_OSC                      =  2,
    OTAI_OBJECT_TYPE_ATTENUATOR               =  3,
    OTAI_OBJECT_TYPE_OCM                      =  4,
    OTAI_OBJECT_TYPE_OTDR                     =  5,
    OTAI_OBJECT_TYPE_MAX                      =  6,
} otai_object_type_t;

typedef struct _otai_u8_list_t
{
    uint32_t count;
    uint8_t *list;
} otai_u8_list_t;

typedef struct _otai_s8_list_t
{
    uint32_t count;
    int8_t *list;
} otai_s8_list_t;

typedef struct _otai_u16_list_t
{
    uint32_t count;
    uint16_t *list;
} otai_u16_list_t;

typedef struct _otai_s16_list_t
{
    uint32_t count;
    int16_t *list;
} otai_s16_list_t;

typedef struct _otai_u32_list_t
{
    uint32_t count;
    uint32_t *list;
} otai_u32_list_t;

typedef struct _otai_s32_list_t
{
    uint32_t count;
    int32_t *list;
} otai_s32_list_t;

typedef struct _otai_spectrum_power_list_t
{
    uint32_t count;
    otai_spectrum_power_t *list;
} otai_spectrum_power_list_t;

typedef struct _otai_u32_range_t
{
    uint32_t min;
    uint32_t max;
} otai_u32_range_t;

typedef struct _otai_s32_range_t
{
    int32_t min;
    int32_t max;
} otai_s32_range_t;

/**
 * @brief Data Type
 *
 * To use enum values as attribute value is otai_int32_t s32
 *
 * @extraparam const otai_attr_metadata_t *meta
 */
typedef union _otai_attribute_value_t
{
    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_BOOL */
    bool booldata;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_CHARDATA */
    char chardata[512];

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT8 */
    otai_uint8_t u8;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT8 */
    otai_int8_t s8;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT16 */
    otai_uint16_t u16;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT16 */
    otai_int16_t s16;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT32 */
    otai_uint32_t u32;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT32 */
    otai_int32_t s32;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT64 */
    otai_uint64_t u64;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT64 */
    otai_int64_t s64;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_DOUBLE */
    otai_double_t d64;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_POINTER */
    otai_pointer_t ptr;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_OBJECT_ID */
    otai_object_id_t oid;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_OBJECT_LIST */
    otai_object_list_t objlist;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT8_LIST */
    otai_u8_list_t u8list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT8_LIST */
    otai_s8_list_t s8list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT16_LIST */
    otai_u16_list_t u16list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT16_LIST */
    otai_s16_list_t s16list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT32_LIST */
    otai_u32_list_t u32list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT32_LIST */
    otai_s32_list_t s32list;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_UINT32_RANGE */
    otai_u32_range_t u32range;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_INT32_RANGE */
    otai_s32_range_t s32range;

    /** @validonly meta->attrvaluetype == OTAI_ATTR_VALUE_TYPE_SPECTRUM_POWER_LIST */
    otai_spectrum_power_list_t spectrumpowerlist;

} otai_attribute_value_t;

typedef struct _otai_attribute_t
{
    otai_attr_id_t id;
    otai_attribute_value_t value;
} otai_attribute_t;

typedef union _otai_stat_value_t
{
    /** @validonly meta->statvaluetype == OTAI_STAT_VALUE_TYPE_INT32 */
    otai_int32_t s32;

    /** @validonly meta->statvaluetype == OTAI_STAT_VALUE_TYPE_UINT32 */
    otai_uint32_t u32;

    /** @validonly meta->statvaluetype == OTAI_STAT_VALUE_TYPE_INT64 */
    otai_int64_t s64;

    /** @validonly meta->statvaluetype == OTAI_STAT_VALUE_TYPE_UINT64 */
    otai_uint64_t u64;

    /** @validonly meta->statvaluetype == OTAI_STAT_VALUE_TYPE_DOUBLE */
    otai_double_t d64;
} otai_stat_value_t;

typedef enum _otai_bulk_op_error_mode_t
{
    /**
     * @brief Bulk operation error handling mode where operation stops on the first failed creation
     *
     * Rest of objects will use OTAI_STATUS_NON_EXECUTED return status value.
     */
    OTAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,

    /**
     * @brief Bulk operation error handling mode where operation ignores the failures and continues to create other objects
     */
    OTAI_BULK_OP_ERROR_MODE_IGNORE_ERROR,
} otai_bulk_op_error_mode_t;

/**
 * @brief OTAI statistics modes
 *
 * Used in get statistics extended or query statistics capabilities
 * Note enum values must be powers of 2 to be used as bit mask for query statistics capabilities
 *
 * @flags Contains flags
 */
typedef enum _otai_stats_mode_t
{
    /**
     * @brief Read statistics
     */
    OTAI_STATS_MODE_READ = 1 << 0,

    /**
     * @brief Read and clear after reading
     */
    OTAI_STATS_MODE_READ_AND_CLEAR = 1 << 1,
} otai_stats_mode_t;

/** @brief Operational status */
typedef enum _otai_oper_status_t
{
    OTAI_OPER_STATUS_ACTIVE,
    OTAI_OPER_STATUS_INACTIVE,
    OTAI_OPER_STATUS_DISABLED,
} otai_oper_status_t;

/** @brief Scanning status */
typedef enum _otai_scanning_status_t
{
    OTAI_SCANNING_STATUS_ACTIVE,
    OTAI_SCANNING_STATUS_INACTIVE,
} otai_scanning_status_t;

/** @brief Admin state */
typedef enum _otai_admin_state_t
{
    OTAI_ADMIN_STATE_ENABLED,
    OTAI_ADMIN_STATE_DISABLED,
    OTAI_ADMIN_STATE_MAINT,
} otai_admin_state_t;

/** @brief Admin status */
typedef enum _otai_admin_status_t
{
    OTAI_ADMIN_STATUS_ENABLED,
    OTAI_ADMIN_STATUS_DISABLED,
    OTAI_ADMIN_STATUS_MAINT,
} otai_admin_status_t;

/**
 * @brief The type of the port
 */
typedef enum _otai_port_type_t
{
    OTAI_PORT_TYPE_INVALID,
    OTAI_PORT_TYPE_CLIENT,
    OTAI_PORT_TYPE_LINE,
    OTAI_PORT_TYPE_LINE_IN,
    OTAI_PORT_TYPE_LINE_OUT,
    OTAI_PORT_TYPE_EDFA_IN,
    OTAI_PORT_TYPE_EDFA_OUT,
    OTAI_PORT_TYPE_MD_IN,
    OTAI_PORT_TYPE_MD_OUT,
    OTAI_PORT_TYPE_MD_EXP_IN,
    OTAI_PORT_TYPE_MD_EXP_OUT,
    OTAI_PORT_TYPE_OLP_PRI_IN,
    OTAI_PORT_TYPE_OLP_PRI_OUT,
    OTAI_PORT_TYPE_OLP_SEC_IN,
    OTAI_PORT_TYPE_OLP_SEC_OUT,
    OTAI_PORT_TYPE_OLP_COM_IN,
    OTAI_PORT_TYPE_OLP_COM_OUT,
    OTAI_PORT_TYPE_CLIENT_IN,
    OTAI_PORT_TYPE_CLIENT_OUT,

} otai_port_type_t;

/**
 * @brief Led mode
 */
typedef enum _otai_led_mode_t
{
    OTAI_LED_MODE_AUTO,
    OTAI_LED_MODE_FORCE_FLASH,
} otai_led_mode_t;

/**
 * @brief Led color
 */
typedef enum _otai_led_color_t
{
    OTAI_LED_COLOR_RED,
    OTAI_LED_COLOR_GREEN,
    OTAI_LED_COLOR_YELLOW,
    OTAI_LED_COLOR_RED_FLASH,
    OTAI_LED_COLOR_GREEN_FLASH,
    OTAI_LED_COLOR_RED_GREEN_FLASH,
    OTAI_LED_COLOR_OFF,
    OTAI_LED_COLOR_ABNORMAL,
    OTAI_LED_COLOR_YELLOW_FLASH,
} otai_led_color_t;

/**
 * @brief Enum defining OTDR event types.
 */
typedef enum _otai_otdr_event_type_t
{
    OTAI_OTDR_EVENT_TYPE_START,
    OTAI_OTDR_EVENT_TYPE_END,
    OTAI_OTDR_EVENT_TYPE_REFLECTION,
    OTAI_OTDR_EVENT_TYPE_NON_REFLECTION,
    OTAI_OTDR_EVENT_TYPE_FIBER_SECTION,
    OTAI_OTDR_EVENT_TYPE_UNKOWN,
} otai_otdr_event_type_t;

typedef struct _otai_otdr_event_t
{
    otai_otdr_event_type_t type;

    /** Event distance or fiber section length in km */
    otai_double_t length;

    /** Event loss in dB */
    otai_double_t loss;

    /** Event reflection in dB */
    otai_double_t reflection;

    /** Accumulated loss at the event point */
    otai_double_t accumulate_loss;

} otai_otdr_event_t;

typedef struct _otai_otdr_event_list_t
{
    uint32_t count;
    otai_otdr_event_t *list;
} otai_otdr_event_list_t;

typedef struct _otai_otdr_events_t
{
    /** Total length in km */
    otai_double_t span_distance;

    /** Total loss in dB */
    otai_double_t span_loss;

    otai_otdr_event_list_t events;

} otai_otdr_events_t;

typedef struct _otai_otdr_scanning_profile_t
{
    otai_uint64_t scan_time;

    /** Distance range in km */
    otai_uint32_t distance_range;

    /** Pulse width in nanosecond */
    otai_uint32_t pulse_width;

    /** Average time of each scanning in second */
    otai_uint32_t average_time;

    /** The output frequency in MHz of the OTDR */
    otai_uint64_t output_frequency;

} otai_otdr_scanning_profile_t;

typedef struct _otai_otdr_result_trace_t
{
    otai_uint64_t update_time;
    otai_u8_list_t data;
} otai_otdr_result_trace_t;

typedef struct _otai_otdr_result_t
{
    otai_otdr_scanning_profile_t scanning_profile;
    otai_otdr_events_t events;
    otai_otdr_result_trace_t trace;
} otai_otdr_result_t;

/**
 * @brief Alarm type of the linecard
 */
typedef enum _otai_alarm_type_t
{
    OTAI_ALARM_TYPE_BOARD_INIT,
    OTAI_ALARM_TYPE_BOARD_LOAD_FILE_FAILED,
    OTAI_ALARM_TYPE_BOARD_LOADING,
    OTAI_ALARM_TYPE_BOARD_LOAD_FAILED,
    OTAI_ALARM_TYPE_BOARD_LOAD_ACTIVE,
    OTAI_ALARM_TYPE_BOARD_MIN_FPGA_MISSING,
    OTAI_ALARM_TYPE_BOARD_READY,
    OTAI_ALARM_TYPE_BOARD_NEED_POWER_CYCLE,
    OTAI_ALARM_TYPE_BOARD_MAX_FPGA_MISSING,
    OTAI_ALARM_TYPE_BOARD_BOOT_FAILED,
    OTAI_ALARM_TYPE_BOARD_CLEAN_UP_FOLDER,
    OTAI_ALARM_TYPE_BOARD_RESET_CONSOLE,
    OTAI_ALARM_TYPE_HARD_BAD,
    OTAI_ALARM_TYPE_FPGA_MAX_TEMPERATURE_HIGH_ALARM,
    OTAI_ALARM_TYPE_FPGA_MAX_TEMPERATURE_HIGH_WARN,
    OTAI_ALARM_TYPE_FPGA_MAX_TEMPERATURE_LOW_WARN,
    OTAI_ALARM_TYPE_FPGA_MAX_TEMPERATURE_LOW_ALARM,
    OTAI_ALARM_TYPE_BOARD_MODE_SWITCH,
    OTAI_ALARM_TYPE_BOARD_UPGRADE_DONE,
    OTAI_ALARM_TYPE_AC1200_MISSING,
    OTAI_ALARM_TYPE_AC1200_COMMUNICATION_FAILED,
    OTAI_ALARM_TYPE_HIGH_TEMPERATURE_ALARM,
    OTAI_ALARM_TYPE_LOW_TEMPERATURE_ALARM,
    OTAI_ALARM_TYPE_HIGH_TEMPERATURE_WARN,
    OTAI_ALARM_TYPE_LOW_TEMPERATURE_WARN,
    OTAI_ALARM_TYPE_WL5E_FAULT,
    OTAI_ALARM_TYPE_WL5E_ABNORMAL,
    OTAI_ALARM_TYPE_LOOPBACK_TERMINAL,
    OTAI_ALARM_TYPE_LOOPBACK_FACILITY,
    OTAI_ALARM_TYPE_TRANSCEIVER_FAILED,
    OTAI_ALARM_TYPE_RX_LOS,
    OTAI_ALARM_TYPE_RX_LOL,
    OTAI_ALARM_TYPE_TX_FAIL,
    OTAI_ALARM_TYPE_TX_LOS,
    OTAI_ALARM_TYPE_TX_PWR_LOW,
    OTAI_ALARM_TYPE_TX_PWR_HIGH,
    OTAI_ALARM_TYPE_RX_PWR_LOW,
    OTAI_ALARM_TYPE_RX_PWR_HIGH,
    OTAI_ALARM_TYPE_PORT_INIT,
    OTAI_ALARM_TYPE_XCVR_MISSING,
    OTAI_ALARM_TYPE_XCVR_UNSUPPORTED,
    OTAI_ALARM_TYPE_XCVR_MISMATCH,
    OTAI_ALARM_TYPE_XCVR_COMMUNICATION_FAILED,
    OTAI_ALARM_TYPE_INVALID_CROSS_BAR,
    OTAI_ALARM_TYPE_INVALID_WORKING_MODE,
    OTAI_ALARM_TYPE_VENDOR_MISMATCH,
    OTAI_ALARM_TYPE_FLEXO_RX_SM_BIP8,
    OTAI_ALARM_TYPE_FLEXO_RX_SM_BEI,
    OTAI_ALARM_TYPE_FLEXO_RX_OTN_LOF,
    OTAI_ALARM_TYPE_FLEXO_RX_OTN_OOF,
    OTAI_ALARM_TYPE_FLEXO_RX_OTN_LOM,
    OTAI_ALARM_TYPE_FLEXO_RX_OTN_OOM,
    OTAI_ALARM_TYPE_FLEXO_RX_SM_BDI,
    OTAI_ALARM_TYPE_FLEXO_RX_ODU_AIS,
    OTAI_ALARM_TYPE_FLEXO_RX_ODU_LCK,
    OTAI_ALARM_TYPE_FLEXO_RX_FEC_UNCORRECT,
    OTAI_ALARM_TYPE_FLEXO_RX_FEC_SD,
    OTAI_ALARM_TYPE_FLEXO_RX_FEC_SF,
    OTAI_ALARM_TYPE_FLEXO_TX_FEC_SD,
    OTAI_ALARM_TYPE_FLEXO_TX_FEC_SF,
    OTAI_ALARM_TYPE_FLEXO_RX_ALGNMENT_ERR,
    OTAI_ALARM_TYPE_FLEXO_RX_CD_DEG,
    OTAI_ALARM_TYPE_FLEXO_RX_PDL_DEG,
    OTAI_ALARM_TYPE_FLEXO_RX_PMD_DEG,
    OTAI_ALARM_TYPE_FLEXO_RX_FOFF_DEG,
    OTAI_ALARM_TYPE_FLEXO_RX_SOP_CHANGE_RATE_DEG,
    OTAI_ALARM_TYPE_FLEXO_TX_FORCE_AIS,
    OTAI_ALARM_TYPE_FLEXO_TX_FORCE_LCK,
    OTAI_ALARM_TYPE_FLEXO_ABNORMAL_STATE,
    OTAI_ALARM_TYPE_FLEXO_LINE_PRBS_ENABLE,
    OTAI_ALARM_TYPE_FLEXO_LINE_RX_TTI_MISMATCH,
    OTAI_ALARM_TYPE_FLEXO_TX_FORCE_OCI,
    OTAI_ALARM_TYPE_FLEXO_RX_BER_FDD,
    OTAI_ALARM_TYPE_FLEXO_RX_BER_FED,
    OTAI_ALARM_TYPE_FLEXO_RX_GIDM,
    OTAI_ALARM_TYPE_FLEXO_RX_LDI_LD,
    OTAI_ALARM_TYPE_FLEXO_RX_LDI_RD,
    OTAI_ALARM_TYPE_FLEXO_RX_LOFLOM,
    OTAI_ALARM_TYPE_FLEXO_RX_PMM,
    OTAI_ALARM_TYPE_FLEXO_RX_RPF,
    OTAI_ALARM_TYPE_RX_DISPERSION_POST_COMP_OOR,
    OTAI_ALARM_TYPE_CLIENT_LASER_OFF_BY_ALS,
    OTAI_ALARM_TYPE_CLIENT_LASER_OFF_BY_DISABLED,
    OTAI_ALARM_TYPE_CLIENT_POWER_MODE_ERROR,
    OTAI_ALARM_TYPE_CLIENT_QSFP28_PNCODE_ERR,
    OTAI_ALARM_TYPE_CLIENT_RX_PLL_FLT,
    OTAI_ALARM_TYPE_CLIENT_SERDES_PRBS_ENABLE,
    OTAI_ALARM_TYPE_CRYPT_KEY_ID,
    OTAI_ALARM_TYPE_CLIENT_MODULE_HOST_PRBS_ENABLE,
    OTAI_ALARM_TYPE_CLIENT_MODULE_MEDIA_PRBS_ENABLE,
    OTAI_ALARM_TYPE_CLIENT_MODULE_HOST_LOOPBACK,
    OTAI_ALARM_TYPE_CLIENT_MODULE_MEDIA_LOOPBACK,
    OTAI_ALARM_TYPE_CLIENT_RX_FEC_BER_SD_ALARM,
    OTAI_ALARM_TYPE_CLIENT_RX_FEC_BER_SF_ALARM,
    OTAI_ALARM_TYPE_GE_TX_FORCE_LF,
    OTAI_ALARM_TYPE_GE_TX_FORCE_RF,
    OTAI_ALARM_TYPE_GE_TX_FORCE_IDLE,
    OTAI_ALARM_TYPE_GE_RX_NO_PACKET,
    OTAI_ALARM_TYPE_GE_RX_LF,
    OTAI_ALARM_TYPE_GE_TX_LF,
    OTAI_ALARM_TYPE_GE_RX_RF,
    OTAI_ALARM_TYPE_GE_TX_RF,
    OTAI_ALARM_TYPE_GE_RX_BLK_LCK_FLT,
    OTAI_ALARM_TYPE_GE_RX_AM_LCK_FLT,
    OTAI_ALARM_TYPE_GE_TX_BLK_LCK_FLT,
    OTAI_ALARM_TYPE_GE_TX_AM_LCK_FLT,
    OTAI_ALARM_TYPE_GE_RX_PCS_ERR_FLT,
    OTAI_ALARM_TYPE_GE_RX_PCS_BIP_FLT,
    OTAI_ALARM_TYPE_GE_TX_PCS_ERR_FLT,
    OTAI_ALARM_TYPE_GE_TX_PCS_BIP_FLT,
    OTAI_ALARM_TYPE_GE_RX_PCS_LOA,
    OTAI_ALARM_TYPE_GE_TX_PCS_LOA,
    OTAI_ALARM_TYPE_GE_RX_PCS_HI_BER,
    OTAI_ALARM_TYPE_GE_RX_LOSS_OF_SYNC,
    OTAI_ALARM_TYPE_GE_TX_LOSS_OF_SYNC,
    OTAI_ALARM_TYPE_OTUCN_IAE,
    OTAI_ALARM_TYPE_OTUCN_LOFLOM,
    OTAI_ALARM_TYPE_OTUCN_ODU_AIS,
    OTAI_ALARM_TYPE_OTUCN_ODU_LCK,
    OTAI_ALARM_TYPE_OTUCN_ODU_OCI,
    OTAI_ALARM_TYPE_OTUCN_ODU_SD,
    OTAI_ALARM_TYPE_OTUCN_OOF,
    OTAI_ALARM_TYPE_OTUCN_OOM,
    OTAI_ALARM_TYPE_OTUCN_OPU_AIS,
    OTAI_ALARM_TYPE_OTUCN_OPU_CRC5,
    OTAI_ALARM_TYPE_OTUCN_OPU_CRC8,
    OTAI_ALARM_TYPE_OTUCN_OPU_CSF,
    OTAI_ALARM_TYPE_OTUCN_OPU_LOCM,
    OTAI_ALARM_TYPE_OTUCN_OPU_PLM,
    OTAI_ALARM_TYPE_OTUCN_OTU_SD,
    OTAI_ALARM_TYPE_OTUCN_PM_BDI,
    OTAI_ALARM_TYPE_OTUCN_PM_BEI_ALARM,
    OTAI_ALARM_TYPE_OTUCN_PM_BIP8_ALARM,
    OTAI_ALARM_TYPE_OTUCN_PM_SSF,
    OTAI_ALARM_TYPE_OTUCN_PM_TSF,
    OTAI_ALARM_TYPE_OTUCN_SM_BDI,
    OTAI_ALARM_TYPE_OTUCN_SM_BEI_ALARM,
    OTAI_ALARM_TYPE_OTUCN_SM_BIAE,
    OTAI_ALARM_TYPE_OTUCN_SM_BIP8_ALARM,
    OTAI_ALARM_TYPE_OTUCN_SM_SSF,
    OTAI_ALARM_TYPE_OTUCN_SM_TSF,
    OTAI_ALARM_TYPE_OTN_AM_SF_0,
    OTAI_ALARM_TYPE_OTN_AM_SF_2,
    OTAI_ALARM_TYPE_OTN_ASSERTING_AM_SF_1,
    OTAI_ALARM_TYPE_OTN_IAE,
    OTAI_ALARM_TYPE_OTN_LOFLOM,
    OTAI_ALARM_TYPE_OTN_LRC,
    OTAI_ALARM_TYPE_OTN_ODU_AIS,
    OTAI_ALARM_TYPE_OTN_ODU_LCK,
    OTAI_ALARM_TYPE_OTN_ODU_OCI,
    OTAI_ALARM_TYPE_OTN_ODU_SD,
    OTAI_ALARM_TYPE_OTN_OOF,
    OTAI_ALARM_TYPE_OTN_OOM,
    OTAI_ALARM_TYPE_OTN_OPU_AIS,
    OTAI_ALARM_TYPE_OTN_OPU_CRC5,
    OTAI_ALARM_TYPE_OTN_OPU_CRC8,
    OTAI_ALARM_TYPE_OTN_OPU_CSF,
    OTAI_ALARM_TYPE_OTN_OPU_LOCM,
    OTAI_ALARM_TYPE_OTN_OPU_PLM,
    OTAI_ALARM_TYPE_OTN_OTU_SD,
    OTAI_ALARM_TYPE_OTN_PM_BDI,
    OTAI_ALARM_TYPE_OTN_PM_BEI_ALARM,
    OTAI_ALARM_TYPE_OTN_PM_BIP8_ALARM,
    OTAI_ALARM_TYPE_OTN_PM_SSF,
    OTAI_ALARM_TYPE_OTN_PM_TSF,
    OTAI_ALARM_TYPE_OTN_RECEIVE_CDL_1,
    OTAI_ALARM_TYPE_OTN_SM_BDI,
    OTAI_ALARM_TYPE_OTN_SM_BEI_ALARM,
    OTAI_ALARM_TYPE_OTN_SM_BIAE,
    OTAI_ALARM_TYPE_OTN_SM_BIP8_ALARM,
    OTAI_ALARM_TYPE_OTN_SM_SSF,
    OTAI_ALARM_TYPE_OTN_SM_TSF,
    OTAI_ALARM_TYPE_OLP_SWITCH_TO_PRIMARY,
    OTAI_ALARM_TYPE_OLP_SWITCH_TO_SECONDARY,
    OTAI_ALARM_TYPE_OLP_FORCE_TO_PRIMARY,
    OTAI_ALARM_TYPE_OLP_FORCE_TO_SECONDARY,
    OTAI_ALARM_TYPE_EDFA_SATURATED,
    OTAI_ALARM_TYPE_EDFA_GAIN_LOW,
    OTAI_ALARM_TYPE_OTDR_LOS_CHANGE,
    OTAI_ALARM_TYPE_OTDR_TEST_FAILURE,
    OTAI_ALARM_TYPE_OTDR_REPORT_IN,
    OTAI_ALARM_TYPE_OTDR_LENGTH_BENEATH,
    OTAI_ALARM_TYPE_OTDR_LOSS_EXCEED,
    OTAI_ALARM_TYPE_WSS_TRIB_INPUT_LOS,
    OTAI_ALARM_TYPE_WSS_TRIB_OUTPUT_LOS,
    OTAI_ALARM_TYPE_WSS_TRIB_INPUT_LOW,
    OTAI_ALARM_TYPE_WSS_TRIB_INPUT_HIGH,
    OTAI_ALARM_TYPE_WSS_PORT_MISMATCH,
    OTAI_ALARM_TYPE_MEDIACHANNEL_TARGETPOWER_OUTOFREACH,
    OTAI_ALARM_TYPE_MEDIACHANNEL_INPUT_LOS,
    OTAI_ALARM_TYPE_ASE_TARGET_PSD_NOT_REACHABLE,
    OTAI_ALARM_TYPE_CRD_REBOOT,
    OTAI_ALARM_TYPE_TRANS_FAIL,
    OTAI_ALARM_TYPE_UPGRADE_IP,
    OTAI_ALARM_TYPE_UPGRADE_FAIL,
    OTAI_ALARM_TYPE_UPGRADE_ACTIVE,
    OTAI_ALARM_TYPE_CRD_READY,
    OTAI_ALARM_TYPE_SLOT_COMM_FAIL,
    OTAI_ALARM_TYPE_TX_PWR_NOT_MATCH,
    OTAI_ALARM_TYPE_EDFA_CURRENT_TOO_HIGH,
    OTAI_ALARM_TYPE_MODULE_ABSENT,
    OTAI_ALARM_TYPE_PLUGGABLE_BIASCURRENT_TOO_HIGH,
    OTAI_ALARM_TYPE_PLUGGABLE_BIASCURRENT_TOO_LOW,
    OTAI_ALARM_TYPE_PLUGGABLE_FAIL,
    OTAI_ALARM_TYPE_PLUGGABLE_VCC_TOO_HIGH,
    OTAI_ALARM_TYPE_PLUGGABLE_VCC_TOO_LOW,
    OTAI_ALARM_TYPE_PUMP_TEMP_ABNORMAL,
    OTAI_ALARM_TYPE_APR_TRIGGER,
    OTAI_ALARM_TYPE_APR_INSTATIN_TRIGGER,
    OTAI_ALARM_TYPE_TARGET_POWER_NOT_REACHABLE,
    OTAI_ALARM_TYPE_MC_ASE_LOAD,
    OTAI_ALARM_TYPE_OTDR_TSET_FAILED,
    OTAI_ALARM_TYPE_OTDR_REPORT_IN_BASELINE,
    OTAI_ALARM_TYPE_OTDR_SPAN_DISTANCE_CHANGE,
    OTAI_ALARM_TYPE_OTDR_SPAN_DISTANCE_CHANGE_BASELINE,
    OTAI_ALARM_TYPE_OTDR_SPAN_LOSS_CHANGE,
    OTAI_ALARM_TYPE_OTDR_SPAN_LOSS_CHANGE_BASELINE,
    OTAI_ALARM_TYPE_PLUG_IN,
    OTAI_ALARM_TYPE_PLUG_OFF,

    OTAI_ALARM_TYPE_MAX,
} otai_alarm_type_t;

/**
 * @brief Alarm status
 */
typedef enum _otai_alarm_status_t
{
    OTAI_ALARM_STATUS_ACTIVE,          /**< Alarm is active */
    OTAI_ALARM_STATUS_INACTIVE,        /**< Alarm is inactive */
    OTAI_ALARM_STATUS_TRANSIENT,       /**< Alarm is transient */
    OTAI_ALARM_STATUS_MAX,             /**< Number of alarm states */
} otai_alarm_status_t;

/**
 * @brief Alarm severity
 */
typedef enum _otai_alarm_severity_t
{
    OTAI_ALARM_SEVERITY_UNKNOWN,          /**< Unknown */
    OTAI_ALARM_SEVERITY_NOT_ALARMED,      /**< Not alarmed */
    OTAI_ALARM_SEVERITY_CLEARED,          /**< Cleared */
    OTAI_ALARM_SEVERITY_NOT_REPORT,       /**< Not report */
    OTAI_ALARM_SEVERITY_MINOR,            /**< Minor */
    OTAI_ALARM_SEVERITY_WARNING,          /**< Warning */
    OTAI_ALARM_SEVERITY_MAJOR,            /**< Major */
    OTAI_ALARM_SEVERITY_CRITICAL,         /**< Critical */
} otai_alarm_severity_t;

/**
 * @brief Structure for alarm info
 */
typedef struct _otai_alarm_info_t
{
    /**
     * @brief Status
     */
    otai_alarm_status_t status;

    /**
     * @brief Time created
     */
    uint64_t time_created;

    /**
     * @brief Text
     */
    otai_s8_list_t text;

    /**
     * @brief Resource object id
     */
    otai_object_id_t resource_oid;

    /**
     * @brief Severity
     */
    otai_alarm_severity_t severity;

} otai_alarm_info_t;

/**
 * @}
 */
#endif /** __OTAITYPES_H_ */
