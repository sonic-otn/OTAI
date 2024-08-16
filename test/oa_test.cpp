
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

extern otai_object_id_t           gLinecardId;

otai_oa_api_t*                    otai_oa_api;
otai_object_id_t                  gOaId = OTAI_NULL_OBJECT_ID;


otai_test_u32_data_t test_set_oa_u32_data[] = {
    {OTAI_OA_ATTR_GAIN_RANGE,        OTAI_OA_GAIN_RANGE_FIXED_GAIN_RANGE,   OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_AMP_MODE,          OTAI_OA_AMP_MODE_CONSTANT_GAIN,  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_AMP_MODE,          OTAI_OA_AMP_MODE_CONSTANT_POWER,  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_FIBER_TYPE_PROFILE,        OTAI_OA_FIBER_TYPE_PROFILE_LEAF,     OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_WORKING_STATE,           OTAI_OA_WORKING_STATE_LOS_A,                       OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_WORKING_STATE,           OTAI_OA_WORKING_STATE_LOS_N,                       OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_LOS_ASE_DELAY,           2,                       OTAI_STATUS_SUCCESS},
};

otai_test_d64_data_t test_set_oa_d64_data[] = {
    {OTAI_OA_ATTR_TARGET_GAIN,              20.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_TARGET_GAIN_TILT,         -1.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_TARGET_OUTPUT_POWER,      10.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MAX_OUTPUT_POWER,         20.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOS_THRESHOLD,         -40.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOS_HYSTERESIS,         0.5,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOS_THRESHOLD,        -40.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOS_HYSTERESIS,         0.5,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_GAIN_LOW_THRESHOLD,         25.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_GAIN_LOW_HYSTERESIS,         1.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOW_THRESHOLD,         5.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOW_THRESHOLD,         25.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_NODE_REFLECTION_THRESHOLD,         10.0,           OTAI_STATUS_SUCCESS},
};

otai_test_char_data_t test_set_oa_char_data[] = {
};

otai_test_bool_data_t test_set_oa_bool_data[] = {
    {OTAI_OA_ATTR_ENABLED,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_ENABLED,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_NODE_ENABLE,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_NODE_ENABLE,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_ENABLE,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_ENABLE,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_VALID_LLDP,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_VALID_LLDP,           false,         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_oa_attr[] = {
    {OTAI_OA_ATTR_EMPTY,                 OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_REMOVABLE,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_SERIAL_NO,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_EQUIPMENT_FAILURE,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_EQUIPMENT_MISMATCH,                       OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_HARDWARE_VERSION,                       OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OPER_STATUS,             OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_PART_NO,                      OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MFG_NAME,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MFG_DATE,              OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_TYPE,                          OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_SOFTWARE_VERSION,                      OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_FIRMWARE_VERSION,              OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_TARGET_GAIN,             OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MIN_GAIN,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MAX_GAIN,                   OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_GAIN_RANGE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_AMP_MODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_TARGET_OUTPUT_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_MAX_OUTPUT_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_ENABLED,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_FIBER_TYPE_PROFILE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INGRESS_PORT,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_EGRESS_PORT,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_WORKING_STATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOS_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOS_HYSTERESIS,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOS_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOS_HYSTERESIS,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_GAIN_LOW_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_GAIN_LOW_HYSTERESIS,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_LOW_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_LOW_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_LOS_ASE_DELAY,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_OFFSET_LINEP_RX,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_OFFSET_LINEP_TX,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_INPUT_OFFSET_LINES_RX,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_OUTPUT_OFFSET_LINES_TX,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_NODE_ENABLE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_NODE_REFLECTION_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_ENABLE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OA_ATTR_APR_LINE_VALID_LLDP,                  OTAI_STATUS_SUCCESS},
};

otai_get_stat_t test_get_oa_stat[] = {
    {OTAI_OA_STAT_TEMPERATURE,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_ACTUAL_GAIN,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_ACTUAL_GAIN_TILT,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_INPUT_POWER_TOTAL,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_INPUT_POWER_C_BAND,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_INPUT_POWER_L_BAND,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_OUTPUT_POWER_TOTAL,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_OUTPUT_POWER_C_BAND,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_OUTPUT_POWER_L_BAND,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_LASER_BIAS_CURRENT,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_OPTICAL_RETURN_LOSS,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_LASER_TEMPERATURE,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_PANEL_INPUT_POWER_LINEP_RX,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_PANEL_OUTPUT_POWER_LINEP_TX,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_PANEL_INPUT_POWER_LINES_RX,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_PANEL_OUTPUT_POWER_LINES_TX,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_LASER_TEC_CURRENT,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_INGRESS_VOA_ATTEN,                    OTAI_STATUS_SUCCESS},
    {OTAI_OA_STAT_MON_OUTPUT_POWER,                    OTAI_STATUS_SUCCESS},
};

void init_oa() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_OA,                   (void **)&otai_oa_api));
    ASSERT_TRUE(otai_oa_api != NULL);
}

void create_oa() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_OA_ATTR_ID;
    attr.value.s32 = 1;
    attrs.push_back(attr);

    status = otai_oa_api->create_oa(&gOaId, gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gOaId != OTAI_NULL_OBJECT_ID);
}

void set_oa_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_oa_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_oa_api->set_oa_attribute(gOaId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_oa_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_oa_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_oa_api->set_oa_attribute(gOaId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_oa_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_oa_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_oa_api->set_oa_attribute(gOaId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_oa_bool_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_oa_bool_data) {
        attr.id = data.otai_attr_id;
        attr.value.booldata = data.otai_attr_val;
        status = otai_oa_api->set_oa_attribute(gOaId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_oa_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_oa_attr) {
        attr.id = data.otai_attr_id;
        status = otai_oa_api->get_oa_attribute(gOaId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_oa_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_oa_stat) {
        stat_id = data.otai_stat_id;
        status = otai_oa_api->get_oa_stats(gOaId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_oa_attribute() {
    Logg(INFO)<<"testing set_oa_u32_attribute";
    set_oa_u32_attribute();
    Logg(INFO)<<"testing set_oa_d64_attribute";
    set_oa_d64_attribute();
    Logg(INFO)<<"testing set_oa_char_attribute";
    set_oa_char_attribute();
    Logg(INFO)<<"testing set_oa_bool_attribute";
    set_oa_bool_attribute();
}

void test_oa() {
    Logg(INFO)<<"------testing otai oa------";
    Logg(INFO)<<"testing init_oa";
    init_oa();
    Logg(INFO)<<"testing create_oa";
    create_oa();
    Logg(INFO)<<"testing set_oa_attribute";
    set_oa_attribute();
    Logg(INFO)<<"testing get_oa_attribute";
    get_oa_attribute();
    Logg(INFO)<<"testing get_oa_statistics";
    get_oa_statistics();
}