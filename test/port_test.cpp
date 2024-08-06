
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

extern otai_object_id_t           gLinecardId;

otai_port_api_t*                  otai_port_api;
otai_object_id_t                  gPortId = OTAI_NULL_OBJECT_ID;


otai_test_u32_data_t test_set_port_u32_data[] = {
    {OTAI_PORT_ATTR_ADMIN_STATE,        OTAI_ADMIN_STATE_ENABLED,   OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_ADMIN_STATE,        OTAI_ADMIN_STATE_DISABLED,  OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_ADMIN_STATE,        OTAI_ADMIN_STATE_MAINT,     OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_ROLL_OFF,           2000,                       OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LED_MODE,           OTAI_LED_MODE_AUTO,         OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LED_MODE,           OTAI_LED_MODE_FORCE_FLASH,  OTAI_STATUS_SUCCESS},
};

otai_test_d64_data_t test_set_port_d64_data[] = {
    {OTAI_PORT_ATTR_HIGH_THRESHOLD,         10.0,           OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LOS_THRESHOLD,         -30.0,           OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LOW_THRESHOLD,         -20.0,           OTAI_STATUS_SUCCESS}
};

otai_test_char_data_t test_set_port_char_data[] = {
    {OTAI_PORT_ATTR_LED_NAME,           {'C', '1'},         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_port_attr[] = {
    {OTAI_PORT_ATTR_HIGH_THRESHOLD,                 OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_OPER_STATUS,                    OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_ADMIN_STATE,                    OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_RX_CD_RANGE,                    OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_PORT_ATTR_ROLL_OFF,                       OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LED_MODE,                       OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LED_FLASH_INTERVAL,             OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LED_COLOR,                      OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LOS_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_OPTICAL_PORT_TYPE,              OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_EMPTY,                          OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_REMOVABLE,                      OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_EQUIPMENT_FAILURE,              OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_EQUIPMENT_MISMATCH,             OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_LOW_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_INPUT_OFFSET,                   OTAI_STATUS_SUCCESS},
    {OTAI_PORT_ATTR_OUTPUT_OFFSET,                  OTAI_STATUS_SUCCESS}
};

otai_get_stat_t test_get_port_stat[] = {
    {OTAI_PORT_STAT_INPUT_POWER,             OTAI_STATUS_SUCCESS},
    {OTAI_PORT_STAT_OUTPUT_POWER,             OTAI_STATUS_SUCCESS},
    {OTAI_PORT_STAT_OSC_INPUT_POWER,             OTAI_STATUS_SUCCESS},
    {OTAI_PORT_STAT_OSC_OUTPUT_POWER,             OTAI_STATUS_SUCCESS},
};

void init_port() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_PORT,                   (void **)&otai_port_api));
    ASSERT_TRUE(otai_port_api != NULL);
}

void create_port() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_PORT_ATTR_PORT_TYPE;
    attr.value.s32 = OTAI_PORT_TYPE_CLIENT;
    attrs.push_back(attr);

    attr.id = OTAI_PORT_ATTR_PORT_ID;
    attr.value.s32 = 1;
    attrs.push_back(attr);

    status = otai_port_api->create_port(&gPortId, gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gPortId != OTAI_NULL_OBJECT_ID);
}

void set_port_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_port_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_port_api->set_port_attribute(gPortId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_port_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_port_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_port_api->set_port_attribute(gPortId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_port_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_port_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_port_api->set_port_attribute(gPortId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_port_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_port_attr) {
        attr.id = data.otai_attr_id;
        status = otai_port_api->get_port_attribute(gPortId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_port_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_port_stat) {
        stat_id = data.otai_stat_id;
        status = otai_port_api->get_port_stats(gPortId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_port_attribute() {
    Logg(INFO)<<"testing set_port_u32_attribute";
    set_port_u32_attribute();
    Logg(INFO)<<"testing set_port_d64_attribute";
    set_port_d64_attribute();
    Logg(INFO)<<"testing set_port_char_attribute";
    set_port_char_attribute();
}

void test_port() {
    Logg(INFO)<<"------testing otai port------";
    Logg(INFO)<<"testing init_port";
    init_port();
    Logg(INFO)<<"testing create_port";
    create_port();
    Logg(INFO)<<"testing set_port_attribute";
    set_port_attribute();
    Logg(INFO)<<"testing get_port_attribute";
    get_port_attribute();
    Logg(INFO)<<"testing get_port_statistics";
    get_port_statistics();
}