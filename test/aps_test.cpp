
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

extern otai_object_id_t           gLinecardId;

otai_aps_api_t*                    otai_aps_api;
otai_object_id_t                  gApsId = OTAI_NULL_OBJECT_ID;


otai_test_u32_data_t test_set_aps_u32_data[] = {
    {OTAI_APS_ATTR_TYPE,        OTAI_APS_TYPE_APS,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_WAIT_TO_RESTORE_TIME,        60,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_HOLD_OFF_TIME,        60,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_FORCE_TO_PORT,        OTAI_APS_FORCE_TO_PORT_NONE,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_FORCE_TO_PORT,        OTAI_APS_FORCE_TO_PORT_PRIMARY,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_FORCE_TO_PORT,        OTAI_APS_FORCE_TO_PORT_SECONDARY,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ACTIVE_PATH,        OTAI_APS_ACTIVE_PATH_PRIMARY,   OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ACTIVE_PATH,        OTAI_APS_ACTIVE_PATH_SECONDARY,   OTAI_STATUS_SUCCESS},
};

otai_test_d64_data_t test_set_aps_d64_data[] = {
    {OTAI_APS_ATTR_PRIMARY_SWITCH_THRESHOLD,               5.0,           OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_PRIMARY_SWITCH_HYSTERESIS,               1.0,           OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_SECONDARY_SWITCH_THRESHOLD,              5.0,           OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD,              7.0,           OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD_OFFSET,              1.0,           OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ALARM_HYSTERESIS,              1.0,           OTAI_STATUS_SUCCESS},
};

otai_test_char_data_t test_set_aps_char_data[] = {
};

otai_test_bool_data_t test_set_aps_bool_data[] = {
    {OTAI_APS_ATTR_REVERTIVE,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_REVERTIVE,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_COLLECT_SWITCH_INFO,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_COLLECT_SWITCH_INFO,           false,         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_aps_attr[] = {
    {OTAI_APS_ATTR_EMPTY,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_REMOVABLE,                 OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_SERIAL_NO,                    OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_EQUIPMENT_FAILURE,                    OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_EQUIPMENT_MISMATCH,                    OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_HARDWARE_VERSION,                       OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ADMIN_STATE,                       OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_OPER_STATUS,                       OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_PART_NO,             OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_MFG_NAME,                      OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_MFG_DATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_SOFTWARE_VERSION,                          OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_FIRMWARE_VERSION,                      OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_TYPE,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_REVERTIVE,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_WAIT_TO_RESTORE_TIME,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_HOLD_OFF_TIME,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_PRIMARY_SWITCH_THRESHOLD,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_PRIMARY_SWITCH_HYSTERESIS,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_SECONDARY_SWITCH_THRESHOLD,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_RELATIVE_SWITCH_THRESHOLD_OFFSET,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_FORCE_TO_PORT,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ACTIVE_PATH,              OTAI_STATUS_SUCCESS},
    {OTAI_APS_ATTR_ALARM_HYSTERESIS,              OTAI_STATUS_SUCCESS},
};

otai_get_stat_t test_get_aps_stat[] = {
    
};

void test_otai_aps_report_switch_info_fn(
        _In_ otai_object_id_t aps_id,
        _In_ otai_olp_switch_t switch_info) {
    UNREFERENCED_PARAMETER(aps_id);
    UNREFERENCED_PARAMETER(switch_info);
    Logg(DEBUG)<<"Call test_otai_aps_report_switch_info_fn";
}

void init_aps() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_APS,                   (void **)&otai_aps_api));
    ASSERT_TRUE(otai_aps_api != NULL);
}

void create_aps() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_APS_ATTR_ID;
    attr.value.s32 = 1;
    attrs.push_back(attr);

    attr.id = OTAI_APS_ATTR_SWITCH_INFO_NOTIFY;
    attr.value.ptr = (void*)test_otai_aps_report_switch_info_fn;
    attrs.push_back(attr);

    status = otai_aps_api->create_aps(&gApsId, gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gApsId != OTAI_NULL_OBJECT_ID);
}

void set_aps_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_aps_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_aps_api->set_aps_attribute(gApsId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_aps_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_aps_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_aps_api->set_aps_attribute(gApsId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_aps_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_aps_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_aps_api->set_aps_attribute(gApsId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_aps_bool_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_aps_bool_data) {
        attr.id = data.otai_attr_id;
        attr.value.booldata = data.otai_attr_val;
        status = otai_aps_api->set_aps_attribute(gApsId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_aps_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_aps_attr) {
        attr.id = data.otai_attr_id;
        status = otai_aps_api->get_aps_attribute(gApsId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_aps_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_aps_stat) {
        stat_id = data.otai_stat_id;
        status = otai_aps_api->get_aps_stats(gApsId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_aps_attribute() {
    Logg(INFO)<<"testing set_aps_u32_attribute";
    set_aps_u32_attribute();
    Logg(INFO)<<"testing set_aps_d64_attribute";
    set_aps_d64_attribute();
    Logg(INFO)<<"testing set_aps_char_attribute";
    set_aps_char_attribute();
    Logg(INFO)<<"testing set_aps_bool_attribute";
    set_aps_bool_attribute();
}

void test_aps() {
    Logg(INFO)<<"------testing otai aps------";
    Logg(INFO)<<"testing init_aps";
    init_aps();
    Logg(INFO)<<"testing create_aps";
    create_aps();
    Logg(INFO)<<"testing set_aps_attribute";
    set_aps_attribute();
    Logg(INFO)<<"testing get_aps_attribute";
    get_aps_attribute();
    Logg(INFO)<<"testing get_aps_statistics";
    get_aps_statistics();
}