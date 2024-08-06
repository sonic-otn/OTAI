
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

extern otai_object_id_t           gLinecardId;

otai_osc_api_t*                    otai_osc_api;
otai_object_id_t                  gOscId = OTAI_NULL_OBJECT_ID;


otai_test_u32_data_t test_set_osc_u32_data[] = {
};

otai_test_d64_data_t test_set_osc_d64_data[] = {
    {OTAI_OSC_ATTR_RX_LOW_THRESHOLD,              20.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_RX_HIGH_THRESHOLD,              50.0,           OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_TX_LOW_THRESHOLD,              -10.0,           OTAI_STATUS_SUCCESS},
};

otai_test_char_data_t test_set_osc_char_data[] = {
};

otai_test_bool_data_t test_set_osc_bool_data[] = {
    {OTAI_OSC_ATTR_ENABLED,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_ENABLED,           false,         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_osc_attr[] = {
    {OTAI_OSC_ATTR_REMOVABLE,                 OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_SERIAL_NO,                    OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_EQUIPMENT_FAILURE,                    OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_EQUIPMENT_MISMATCH,                    OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_HARDWARE_VERSION,                       OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_OPER_STATUS,                       OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_PART_NO,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_MFG_NAME,                      OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_MFG_DATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_EMPTY,              OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_SOFTWARE_VERSION,                          OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_FIRMWARE_VERSION,                      OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_OUTPUT_FREQUENCY,              OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_ENABLED,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_RX_LOW_THRESHOLD,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_RX_HIGH_THRESHOLD,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_ATTR_TX_LOW_THRESHOLD,             OTAI_STATUS_SUCCESS},
};

otai_get_stat_t test_get_osc_stat[] = {
    {OTAI_OSC_STAT_INPUT_POWER,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_TEMPERATURE,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_OUTPUT_POWER,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_INPUT_OFFSET_LINEP_RX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_INPUT_OFFSET_LINES_RX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_OUTPUT_OFFSET_LINEP_TX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_OUTPUT_OFFSET_LINES_TX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_LASER_BIAS_CURRENT,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_PANEL_INPUT_POWER_LINEP_RX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_PANEL_OUTPUT_POWER_LINEP_TX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_PANEL_INPUT_POWER_LINES_RX,             OTAI_STATUS_SUCCESS},
    {OTAI_OSC_STAT_PANEL_OUTPUT_POWER_LINES_TX,             OTAI_STATUS_SUCCESS},
};

void init_osc() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_OSC,                   (void **)&otai_osc_api));
    ASSERT_TRUE(otai_osc_api != NULL);
}

void create_osc() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_OSC_ATTR_ID;
    attr.value.s32 = 1;
    attrs.push_back(attr);

    status = otai_osc_api->create_osc(&gOscId, gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gOscId != OTAI_NULL_OBJECT_ID);
}

void set_osc_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_osc_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_osc_api->set_osc_attribute(gOscId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_osc_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_osc_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_osc_api->set_osc_attribute(gOscId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_osc_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_osc_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_osc_api->set_osc_attribute(gOscId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_osc_bool_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_osc_bool_data) {
        attr.id = data.otai_attr_id;
        attr.value.booldata = data.otai_attr_val;
        status = otai_osc_api->set_osc_attribute(gOscId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_osc_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_osc_attr) {
        attr.id = data.otai_attr_id;
        status = otai_osc_api->get_osc_attribute(gOscId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_osc_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_osc_stat) {
        stat_id = data.otai_stat_id;
        status = otai_osc_api->get_osc_stats(gOscId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_osc_attribute() {
    Logg(INFO)<<"testing set_osc_u32_attribute";
    set_osc_u32_attribute();
    Logg(INFO)<<"testing set_osc_d64_attribute";
    set_osc_d64_attribute();
    Logg(INFO)<<"testing set_osc_char_attribute";
    set_osc_char_attribute();
    Logg(INFO)<<"testing set_osc_bool_attribute";
    set_osc_bool_attribute();
}

void test_osc() {
    Logg(INFO)<<"------testing otai osc------";
    Logg(INFO)<<"testing init_osc";
    init_osc();
    Logg(INFO)<<"testing create_osc";
    create_osc();
    Logg(INFO)<<"testing set_osc_attribute";
    set_osc_attribute();
    Logg(INFO)<<"testing get_osc_attribute";
    get_osc_attribute();
    Logg(INFO)<<"testing get_osc_statistics";
    get_osc_statistics();
}