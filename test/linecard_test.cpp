#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

otai_object_id_t gLinecardId = OTAI_NULL_OBJECT_ID;
otai_linecard_api_t*              otai_linecard_api;

otai_test_u32_data_t test_set_linecard_u32_data[] = {
    {OTAI_LINECARD_ATTR_ADMIN_STATE,                OTAI_ADMIN_STATE_ENABLED,                   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_ADMIN_STATE,                OTAI_ADMIN_STATE_DISABLED,                  OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_ADMIN_STATE,                OTAI_ADMIN_STATE_MAINT,                     OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_RESET,                      OTAI_LINECARD_RESET_WARM,                   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_RESET,                      OTAI_LINECARD_RESET_COLD,                   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_POWER_ADMIN_STATE,          OTAI_LINECARD_POWER_ADMIN_STATE_ENABLED,    OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_POWER_ADMIN_STATE,          OTAI_LINECARD_POWER_ADMIN_STATE_DISABLED,   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_MODE,                   OTAI_LED_MODE_FORCE_FLASH,                  OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_FLASH_INTERVAL,         100,                                        OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_COLOR,                  OTAI_LED_COLOR_RED_FLASH,                   OTAI_STATUS_INVALID_PARAMETER},
    {OTAI_LINECARD_ATTR_BAUD_RATE,                  115200,                                     OTAI_STATUS_INVALID_PARAMETER},
    {OTAI_LINECARD_ATTR_HOST_IP,                    3232235876,                                 OTAI_STATUS_SUCCESS},
};

otai_test_d64_data_t test_set_linecard_d64_data[] = {
    {OTAI_LINECARD_ATTR_TEMP_HIGH_ALARM_THRESHOLD,         50.0,           OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_TEMP_HIGH_WARN_THRESHOLD,         40.0,           OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_TEMP_LOW_ALARM_THRESHOLD,         10.0,           OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_TEMP_LOW_WARN_THRESHOLD,         0.0,           OTAI_STATUS_SUCCESS},
};

otai_test_char_data_t test_set_linecard_char_data[] = {
    {OTAI_LINECARD_ATTR_BOARD_MODE,           {'L','1','_', '4','0','0','G','_','C','A','_','1','0','0','G','E'},         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_HOSTNAME,             {'T','E','S', 'T'},                                                         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_USER_NAME,             {'A','d','m', 'i', 'n'},                                                   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_USER_PASSWORD,         {'A','d','m', 'i', 'n'},                                                   OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_FILE_NAME,     {'t','e','s', 't', '.','b','i','n'},                                       OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_FILE_PATH,     {'/','t','m','p'},                                                         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_NAME,               {'a','l','r','m'},                                                        OTAI_STATUS_SUCCESS},
};

otai_test_bool_data_t test_set_linecard_bool_data[] = {
    {OTAI_LINECARD_ATTR_COLLECT_LINECARD_ALARM,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_COLLECT_LINECARD_ALARM,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_COLLECT_LINECARD_LOG,             true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_COLLECT_LINECARD_LOG,             false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_DOWNLOAD,                 true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_DOWNLOAD,                 false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT,                   true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT,                   false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT_PAUSE,             true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT_PAUSE,             false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT_RESUME,            true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_COMMIT_RESUME,            false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_ROLLBACK,                 true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_ROLLBACK,                 false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_REBOOT,                   true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_REBOOT,                   false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_AUTO,                     true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_UPGRADE_AUTO,                     false,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_START_PRE_CONFIGURATION,          true,         OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_STOP_PRE_CONFIGURATION,           true,         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_linecard_attr[] = {
    {OTAI_LINECARD_ATTR_ADMIN_STATE,                    OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_BOARD_MODE,                     OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_OPER_STATUS,                    OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_RELAY,                          OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_EMPTY,                          OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_REMOVABLE,                      OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_POWER_ADMIN_STATE,              OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_SERIAL_NO,                      OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_PART_NO,                        OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_HARDWARE_VERSION,               OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_SOFTWARE_VERSION,               OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_CPLD_VERSION,                   OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_SCRIPT_VERSION,                 OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_MFG_DATE,                       OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_MFG_NAME,                       OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_LED_MODE,                       OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_FLASH_INTERVAL,             OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_LED_COLOR,                      OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_BAUD_RATE,                      OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_UPGRADE_STATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_FPGA_VERSION,                   OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_TEMP_HIGH_ALARM_THRESHOLD,      OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_ATTR_SLOT_ID,                        OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_EQUIPMENT_FAILURE,              OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_EQUIPMENT_MISMATCH,             OTAI_STATUS_NOT_IMPLEMENTED},
    {OTAI_LINECARD_ATTR_LED_NAME,                       OTAI_STATUS_SUCCESS},
};

otai_get_stat_t test_get_linecard_stat[] = {
    {OTAI_LINECARD_STAT_MEMORY_AVAILABLE,                    OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_STAT_MEMORY_UTILIZED,                     OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_STAT_CPU_UTILIZATION,                     OTAI_STATUS_SUCCESS},
    {OTAI_LINECARD_STAT_TEMPERATURE,                         OTAI_STATUS_SUCCESS},
};

const char* test_profile_get_value(
    _In_ otai_linecard_profile_id_t profile_id,
    _In_ const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    // return default type on ot-vs platform
    return "OTAI_VS_LINECARD_TYPE_P230C";
}

int test_profile_get_next_value(
    _In_ otai_linecard_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    UNREFERENCED_PARAMETER(value);

    return -1;
}

const otai_service_method_table_t test_services =
{
    test_profile_get_value,
    test_profile_get_next_value
};

void test_otai_linecard_alarm_notification_fn(
        _In_ otai_object_id_t linecard_id,
        _In_ otai_alarm_type_t alarm_type,
        _In_ otai_alarm_info_t alarm_info) {
    UNREFERENCED_PARAMETER(linecard_id);
    UNREFERENCED_PARAMETER(alarm_type);
    UNREFERENCED_PARAMETER(alarm_info);
    Logg(DEBUG)<<"Call test_otai_linecard_alarm_notification_fn";
}

void test_otai_linecard_ocm_spectrum_power_notification_fn(
        _In_ otai_object_id_t linecard_id,
        _In_ otai_object_id_t ocm_id,
        _In_ otai_spectrum_power_list_t ocm_result) {
    UNREFERENCED_PARAMETER(linecard_id);
    UNREFERENCED_PARAMETER(ocm_id);
    UNREFERENCED_PARAMETER(ocm_result);
    Logg(DEBUG)<<"Call test_otai_linecard_ocm_spectrum_power_notification_fn";        
}

void test_otai_linecard_otdr_result_notification_fn (
        _In_ otai_object_id_t linecard_id,
        _In_ otai_object_id_t otdr_id,
        _In_ otai_otdr_result_t otdr_result) {
    UNREFERENCED_PARAMETER(linecard_id);
    UNREFERENCED_PARAMETER(otdr_id);
    UNREFERENCED_PARAMETER(otdr_result);
    Logg(DEBUG)<<"Call test_otai_linecard_otdr_result_notification_fn"; 
}

void test_otai_linecard_state_change_notification_fn (
        _In_ otai_object_id_t linecard_id,
        _In_ otai_oper_status_t linecard_oper_status) {
    UNREFERENCED_PARAMETER(linecard_id);
    UNREFERENCED_PARAMETER(linecard_oper_status);
    Logg(DEBUG)<<"Call test_otai_linecard_state_change_notification_fn";
}

void init_linecard() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_initialize(0, (otai_service_method_table_t *)&test_services));

    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_LINECARD,               (void **)&otai_linecard_api));
    ASSERT_TRUE(otai_linecard_api != NULL);

    bool isLinkUp = false;
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_link_check(&isLinkUp));
    ASSERT_TRUE(isLinkUp); 
}

void create_linecard() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_LINECARD_ATTR_LINECARD_TYPE;
    memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
    strncpy(attr.value.chardata, "P230C", sizeof(attr.value.chardata) - 1);
    attrs.push_back(attr);

    memset(&attr, 0, sizeof(attr));
    attr.id = OTAI_LINECARD_ATTR_INIT_LINECARD;
    attr.value.booldata = true;
    attrs.push_back(attr);

    memset(&attr, 0, sizeof(attr));
    attr.id = OTAI_LINECARD_ATTR_LINECARD_ALARM_NOTIFY;
    attr.value.ptr = (void*)test_otai_linecard_alarm_notification_fn;
    attrs.push_back(attr);

    memset(&attr, 0, sizeof(attr));
    attr.id = OTAI_LINECARD_ATTR_LINECARD_OCM_SPECTRUM_POWER_NOTIFY;
    attr.value.ptr = (void*)test_otai_linecard_ocm_spectrum_power_notification_fn;
    attrs.push_back(attr);

    memset(&attr, 0, sizeof(attr));
    attr.id = OTAI_LINECARD_ATTR_LINECARD_OTDR_RESULT_NOTIFY;
    attr.value.ptr = (void*)test_otai_linecard_ocm_spectrum_power_notification_fn;
    attrs.push_back(attr);

    memset(&attr, 0, sizeof(attr));
    attr.id = OTAI_LINECARD_ATTR_LINECARD_STATE_CHANGE_NOTIFY;
    attr.value.ptr = (void*)test_otai_linecard_state_change_notification_fn;
    attrs.push_back(attr);

    status = otai_linecard_api->create_linecard(&gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gLinecardId != OTAI_NULL_OBJECT_ID);
}

void set_linecard_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_linecard_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_linecard_api->set_linecard_attribute(gLinecardId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_linecard_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_linecard_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_linecard_api->set_linecard_attribute(gLinecardId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_linecard_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_linecard_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_linecard_api->set_linecard_attribute(gLinecardId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_linecard_bool_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_linecard_bool_data) {
        attr.id = data.otai_attr_id;
        attr.value.booldata = data.otai_attr_val;
        status = otai_linecard_api->set_linecard_attribute(gLinecardId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_linecard_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_linecard_attr) {
        attr.id = data.otai_attr_id;
        status = otai_linecard_api->get_linecard_attribute(gLinecardId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_linecard_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_linecard_stat) {
        stat_id = data.otai_stat_id;
        status = otai_linecard_api->get_linecard_stats(gLinecardId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_linecard_attribute() {
    Logg(INFO)<<"testing set_linecard_u32_attribute";
    set_linecard_u32_attribute();
    Logg(INFO)<<"testing set_linecard_d64_attribute";
    set_linecard_d64_attribute();
    Logg(INFO)<<"testing set_linecard_char_attribute";
    set_linecard_char_attribute();
    Logg(INFO)<<"testing set_linecard_bool_attribute";
    set_linecard_bool_attribute();
}

void test_linecard() {
    Logg(INFO)<<"------testing otai linecard------";
    Logg(INFO)<<"testing init_linecard";
    init_linecard();
    Logg(INFO)<<"testing create_linecard";
    create_linecard();
    Logg(INFO)<<"testing set_linecard_attribute";
    set_linecard_attribute();
    Logg(INFO)<<"testing get_linecard_attribute";
    get_linecard_attribute();
    Logg(INFO)<<"testing get_linecard_statistics";
    get_linecard_attribute();
}

