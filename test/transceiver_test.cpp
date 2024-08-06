
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test_common.h"

extern "C" {
#include "otai.h"
}

using namespace std;

extern otai_object_id_t           gLinecardId;

otai_transceiver_api_t*                    otai_transceiver_api;
otai_object_id_t                  gTransceiverId = OTAI_NULL_OBJECT_ID;


otai_test_u32_data_t test_set_transceiver_u32_data[] = {
    {OTAI_TRANSCEIVER_ATTR_FEC_MODE,        OTAI_TRANSCEIVER_FEC_MODE_FEC_AUTO,   OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FEC_MODE,        OTAI_TRANSCEIVER_FEC_MODE_FEC_ENABLED,   OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FEC_MODE,        OTAI_TRANSCEIVER_FEC_MODE_FEC_DISABLED,   OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ETHERNET_PMD_PRECONF,             OTAI_TRANSCEIVER_ETHERNET_PMD_ETH_400GBASE_ZR,  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ETHERNET_PMD_PRECONF,             OTAI_TRANSCEIVER_ETHERNET_PMD_ETH_AUTO,  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_POWER_MODE,                       OTAI_TRANSCEIVER_POWER_MODE_NORMAL,     OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_POWER_MODE,                       OTAI_TRANSCEIVER_POWER_MODE_LOW_POWER,     OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_SWITCH_FLASH_PARTITION,           OTAI_TRANSCEIVER_FLASH_PARTITION_A,                       OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_SWITCH_FLASH_PARTITION,           OTAI_TRANSCEIVER_FLASH_PARTITION_B,                       OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_BACKUP_FLASH_PARTITION,           OTAI_TRANSCEIVER_FLASH_PARTITION_A,                       OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_BACKUP_FLASH_PARTITION,           OTAI_TRANSCEIVER_FLASH_PARTITION_B,                       OTAI_STATUS_SUCCESS},
};

otai_test_d64_data_t test_set_transceiver_d64_data[] = {
    {OTAI_TRANSCEIVER_ATTR_TEMP_HIGH_ALARM_THRESHOLD,              60.0,           OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_HIGH_WARN_THRESHOLD,              50.0,           OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_LOW_ALARM_THRESHOLD,              10.0,           OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_LOW_WARN_THRESHOLD,              15.0,           OTAI_STATUS_SUCCESS},
};

otai_test_char_data_t test_set_transceiver_char_data[] = {
    {OTAI_TRANSCEIVER_ATTR_VENDOR_EXPECT,           {'T', 'E', 'S', 'T'},         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FREQUENCY_RANGE,           {'T', 'E', 'S', 'T'},         OTAI_STATUS_SUCCESS},
};

otai_test_bool_data_t test_set_transceiver_bool_data[] = {
    {OTAI_TRANSCEIVER_ATTR_ENABLED,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ENABLED,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RESET,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RESET,           false,         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_UPGRADE_DOWNLOAD,           true,         OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_UPGRADE_DOWNLOAD,           false,         OTAI_STATUS_SUCCESS},
};

otai_get_attr_t test_get_transceiver_attr[] = {
    {OTAI_TRANSCEIVER_ATTR_ENABLED,                 OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OPER_STATUS,                    OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FEC_MODE,                    OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_PRESENT,                    OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FORM_FACTOR,                       OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VENDOR_EXPECT,                       OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VENDOR,             OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VENDOR_PART,                      OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_PART_NO,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VENDOR_REV,              OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_DATE_CODE,                          OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MFG_NAME,                      OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MFG_DATE,                     OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_HARDWARE_VERSION,             OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FIRMWARE_VERSION,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ETHERNET_PMD,                   OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ETHERNET_PMD_PRECONF,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_REMOVABLE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_CONNECTOR_TYPE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_POWER_MODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_NUM_OF_LANE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_UPGRADE_STATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_LASER_AGE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OPERATE_TIME,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_LOT_CODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_CLEI_CODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_POWER_CLASS,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_NEWWORK_BIT_RATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_HOST_BIT_RATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_SM_FIBER_LEN,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MM_FIBER_LEN,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_COPPER_CABLE_LEN,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MAX_WAVELENGTH,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MIN_WAVELENGTH,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MAX_TX_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MAX_RX_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MAX_OPER_TEMP,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_MIN_OPER_TEMP,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VCC_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VCC_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VCC_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_VCC_LOW_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RX_TOTAL_POWER_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RX_TOTAL_POWER_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RX_TOTAL_POWER_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RX_TOTAL_POWER_LOW_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OA_PUMP_CURRENT_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OA_PUMP_CURRENT_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OA_PUMP_CURRENT_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OA_PUMP_CURRENT_LOW_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_BAIS_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_BAIS_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_BAIS_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_BAIS_LOW_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_EXTEND_MODULE_CODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_DATA_STATUS,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_ENCODE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_BITRATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_EXTEND_BIT_RATE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_WAVELENGTH,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_WAVELENGTH_TOLERANCE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_OUI,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_RN,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_IDENTIFIER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_CMIS_REVISION,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_POWER_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_POWER_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_POWER_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TX_POWER_LOW_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_FREQUENCY_RANGE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_HIGH_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_HIGH_WARN_THRESHOLD,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_ATTR_TEMP_LOW_ALARM_THRESHOLD,                  OTAI_STATUS_SUCCESS},
};

otai_get_stat_t test_get_transceiver_stat[] = {
    {OTAI_TRANSCEIVER_STAT_FEC_UNCORRECTABLE_WORDS,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_INPUT_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_OUTPUT_POWER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_LASER_BIAS_CURRENT,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TEMPERATURE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_PRE_FEC_BER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_POST_FEC_BER,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_EDFA_BIAS_CURRENT,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_LASER_BIAS_VOLTAGE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_CASE_TEMPERATURE,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_POWER_CONSUMPTION,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_XI,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_XQ,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_YI,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_YQ,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_XPH,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_TX_MOD_BIAS_YPH,                  OTAI_STATUS_SUCCESS},
    {OTAI_TRANSCEIVER_STAT_LOSS_TIME,                  OTAI_STATUS_SUCCESS},
};

void init_transceiver() {
    ASSERT_EQ(OTAI_STATUS_SUCCESS, otai_api_query(OTAI_API_TRANSCEIVER,                   (void **)&otai_transceiver_api));
    ASSERT_TRUE(otai_transceiver_api != NULL);
}

void create_transceiver() {
    vector<otai_attribute_t> attrs;
    otai_attribute_t attr;
    otai_status_t status;

    attr.id = OTAI_TRANSCEIVER_ATTR_PORT_TYPE;
    attr.value.s32 = OTAI_PORT_TYPE_LINE;
    attrs.push_back(attr);

    attr.id = OTAI_TRANSCEIVER_ATTR_PORT_ID;
    attr.value.s32 = 1;
    attrs.push_back(attr);

    status = otai_transceiver_api->create_transceiver(&gTransceiverId, gLinecardId, (uint32_t)attrs.size(), attrs.data());
    ASSERT_EQ(OTAI_STATUS_SUCCESS, status);
    ASSERT_TRUE(gTransceiverId != OTAI_NULL_OBJECT_ID);
}

void set_transceiver_u32_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_transceiver_u32_data) {
        attr.id = data.otai_attr_id;
        attr.value.s32 = data.otai_attr_val;
        status = otai_transceiver_api->set_transceiver_attribute(gTransceiverId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_transceiver_d64_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_transceiver_d64_data) {
        attr.id = data.otai_attr_id;
        attr.value.d64 = data.otai_attr_val;
        status = otai_transceiver_api->set_transceiver_attribute(gTransceiverId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_transceiver_char_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_transceiver_char_data) {
        attr.id = data.otai_attr_id;
        memset(attr.value.chardata, 0, sizeof(attr.value.chardata));
        strncpy(attr.value.chardata, data.otai_attr_val, sizeof(attr.value.chardata) - 1);
        status = otai_transceiver_api->set_transceiver_attribute(gTransceiverId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_transceiver_bool_attribute() {
    otai_status_t status;
    otai_attribute_t attr;

    int counter = 0;
    for(auto &data : test_set_transceiver_bool_data) {
        attr.id = data.otai_attr_id;
        attr.value.booldata = data.otai_attr_val;
        status = otai_transceiver_api->set_transceiver_attribute(gTransceiverId, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" attr: "<<data.otai_attr_val<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_transceiver_attribute() {
    otai_attribute_t attr;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_transceiver_attr) {
        attr.id = data.otai_attr_id;
        status = otai_transceiver_api->get_transceiver_attribute(gTransceiverId, 1, &attr);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_attr_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void get_transceiver_statistics() {
    otai_stat_value_t value;
    otai_stat_id_t stat_id;
    otai_status_t status;

    int counter = 0;
    for(auto &data : test_get_transceiver_stat) {
        stat_id = data.otai_stat_id;
        status = otai_transceiver_api->get_transceiver_stats(gTransceiverId, 1, &stat_id, &value);
        Logg(DEBUG)<<"index: "<<counter<<" id: "<<data.otai_stat_id<<" result: "<<status;
        ASSERT_EQ(data.status, status);
        counter++;
    }
}

void set_transceiver_attribute() {
    Logg(INFO)<<"testing set_transceiver_u32_attribute";
    set_transceiver_u32_attribute();
    Logg(INFO)<<"testing set_transceiver_d64_attribute";
    set_transceiver_d64_attribute();
    Logg(INFO)<<"testing set_transceiver_char_attribute";
    set_transceiver_char_attribute();
    Logg(INFO)<<"testing set_transceiver_bool_attribute";
    set_transceiver_bool_attribute();
}

void test_transceiver() {
    Logg(INFO)<<"------testing otai transceiver------";
    Logg(INFO)<<"testing init_transceiver";
    init_transceiver();
    Logg(INFO)<<"testing create_transceiver";
    create_transceiver();
    Logg(INFO)<<"testing set_transceiver_attribute";
    set_transceiver_attribute();
    Logg(INFO)<<"testing get_transceiver_attribute";
    get_transceiver_attribute();
    Logg(INFO)<<"testing get_transceiver_statistics";
    get_transceiver_statistics();
}