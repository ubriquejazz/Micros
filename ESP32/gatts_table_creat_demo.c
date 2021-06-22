/****************************************************************************
*
* This demo showcases creating a GATT database using a predefined attribute table.
* It acts as a GATT server and can send adv data, be connected by client.
* Run the gatt_client demo, the client demo will automatically connect to the gatt_server_service_table demo.
* Client demo will enable GATT server's notify after connection. The two devices will then exchange data
* When the GATT client performs a write operation, the data length < GATTS_DEMO_CHAR_VAL_LEN_MAX.
*
****************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include <math.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "gatts_table_creat_demo.h"
#include "esp_gatt_common_api.h"

#define GATTS_TAG "           BLE SERVER"
#define SAMPLE_DEVICE_NAME          "GALAN_BIKE"
#define MANUFACTURER_NAME           "IDNEO"
#define MODEL_NUMBER                "1"
#define SERIAL_NUMBER               "SN0001"
#define HW_REVISION                 "RevA"
#define FW_REVISION                 "0.0.1"
#define SW_REVISION                 "0.0.1"

#define DEFAULT_VREF                    1100    // use adc2_vref_to_gpio() to obtain a better estimate
#define GATTS_DEMO_CHAR_VAL_LEN_MAX     500     // max length of characteristic value
#define PREPARE_BUF_MAX_SIZE            1024
#define CHAR_DECLARATION_SIZE           (sizeof(uint8_t))
#define ADV_CONFIG_FLAG                 (1 << 0)
#define SCAN_RSP_CONFIG_FLAG            (1 << 1)

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
static prepare_type_env_t b_prepare_write_env;

//#define CONFIG_SET_RAW_ADV_DATA
#ifdef CONFIG_SET_RAW_ADV_DATA
static uint8_t raw_adv_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power*/
        0x02, 0x0a, 0xeb,
        /* service uuid */
        0x03, 0x03, 0xFF, 0x00,
        /* device name */
        0x0f, 0x09, 'G', 'A', 'L', 'A', 'N', '_', 'B', 'I', 'K', 'E'
};
static uint8_t raw_scan_rsp_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power */
        0x02, 0x0a, 0xeb,
        /* service uuid */
        0x03, 0x03, 0xFF,0x00
};

#else
static uint8_t service_uuid[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 32bit, [12],[13] is the value for CP "0000-1818-0000-1000-80 00-00805f9b34fb
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00,     0x00, 0x80, 0x00, 0x10,     0x00, 0x00,     0x18, 0x18, 0x00, 0x00,
    //second uuid, for DIS "0000-180A-0000-1000-8000-00805f9b34fb"
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00,     0x00, 0x80, 0x00, 0x10,     0x00, 0x00,     0x0A, 0x18, 0x00, 0x00,
};

/* The length of adv data must be less than 31 bytes */
static uint8_t test_manufacturer[] =  {0x12, 0x23, 0x45, 0x56};
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp        = false,
    .include_name        = true,
    .include_txpower     = true,
    .min_interval        = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval        = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance          = ESP_BLE_APPEARANCE_CYCLING_COMPUTER,
    .manufacturer_len    = sizeof(test_manufacturer),
    .p_manufacturer_data = test_manufacturer,
    .service_data_len    = 0,
    .p_service_data      = NULL,
    .service_uuid_len    = sizeof(service_uuid),
    .p_service_uuid      = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

// scan response data
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp        = true,
    .include_name        = true,
    .include_txpower     = true,
    .min_interval        = 0x0006,
    .max_interval        = 0x0010,
    .appearance          = ESP_BLE_APPEARANCE_CYCLING_COMPUTER,
    .manufacturer_len    = sizeof(test_manufacturer),
    .p_manufacturer_data = test_manufacturer,
    .service_data_len    = 0,
    .p_service_data      = NULL,
    .service_uuid_len    = sizeof(service_uuid),
    .p_service_uuid      = service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
#endif /* CONFIG_SET_RAW_ADV_DATA */

static esp_ble_adv_params_t adv_params = {
    .adv_int_min         = 0x20,
    .adv_int_max         = 0x40,
    .adv_type            = ADV_TYPE_IND,
    .own_addr_type       = BLE_ADDR_TYPE_PUBLIC,
    .channel_map         = ADV_CHNL_ALL,
    .adv_filter_policy   = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

/* Private Functions */

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

static float app_GetRPM(int adc_reading);
static float app_GetPower(int adc_reading, float angular_speed);

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);
void example_prepare_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

/* Profiles */

#define PROFILE_NUM                 2
#define PROFILE_A_APP_ID            0
#define PROFILE_B_APP_ID            1

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },

    [PROFILE_B_APP_ID] = {
        .gatts_cb = gatts_profile_b_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

/* Services */

uint16_t cycling_power_handle_table[IDX_SVC1_NB];
uint16_t device_info_handle_table[IDX_SVC2_NB];

static const uint16_t GATTS_SERVICE_UUID_CPS         = ESP_GATT_UUID_CYCLING_POWER_SVC;
static const uint16_t GATTS_CHAR_UUID_RPM            = ESP_GATT_UUID_CSC_MEASUREMENT;
static const uint16_t GATTS_CHAR_UUID_PWR            = 0x2A63;  // Cycling Power Measurement
static const uint16_t GATTS_CHAR_UUID_REFRESH        = 0xFF01;  // Custom
static const uint16_t GATTS_CHAR_UUID_CRANCK_LENGTH  = 0x2AAF;  // Longitude
static const uint16_t GATTS_CHAR_UUID_PWR_OFFSET     = 0xFF02;  // Custom

static const uint16_t GATTS_SERVICE_UUID_DIS         = ESP_GATT_UUID_DEVICE_INFO_SVC;
static const uint16_t GATTS_CHAR_UUID_MANU_NAME      = ESP_GATT_UUID_MANU_NAME;
static const uint16_t GATTS_CHAR_UUID_MODEL_NUM      = ESP_GATT_UUID_MODEL_NUMBER_STR;
static const uint16_t GATTS_CHAR_UUID_SERIAL_NUM     = ESP_GATT_UUID_SERIAL_NUMBER_STR;
static const uint16_t GATTS_CHAR_UUID_HW_VERSION     = ESP_GATT_UUID_HW_VERSION_STR;
static const uint16_t GATTS_CHAR_UUID_FW_VERSION     = ESP_GATT_UUID_FW_VERSION_STR;
static const uint16_t GATTS_CHAR_UUID_SW_VERSION     = ESP_GATT_UUID_SW_VERSION_STR;

static const uint16_t primary_service_uuid          = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid    = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid  = ESP_GATT_UUID_CHAR_CLIENT_CONFIG; // 0x2902

static const uint8_t char_prop_read                 = ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_read_write           = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;
static const uint8_t char_prop_read_notify          = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;

uint16_t rpm;       // characteristic value - IDX_CHAR_VAL_RPM
uint16_t pwr;       // characteristic value - IDX_CHAR_VAL_PWR
static bool rpm_connected = false;
static bool pwr_connected = false;
uint8_t  rpm_ccc[2] = {0x00, 0x00}; // characteristic notification - IDX_CHAR_CFG_RPM
uint8_t  pwr_ccc[2] = {0x00, 0x00}; // characteristic notification - IDX_CHAR_CFG_PWR

uint8_t  refresh_time = 3;
uint16_t cranck_mm = 180;
int16_t  pwr_offset = 0;

uint8_t dis_manufacturer[] = MANUFACTURER_NAME;
uint8_t dis_model_number[] = MODEL_NUMBER;
uint8_t dis_serial_number[] = SERIAL_NUMBER;
uint8_t dis_hardware_ver[] = HW_REVISION;
uint8_t dis_firmware_ver[] = FW_REVISION;
uint8_t dis_software_ver[] = SW_REVISION;

/* Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t gatt_db_a[IDX_SVC1_NB] =
{
    // Service Declaration
    [IDX_SVC1]        =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
      sizeof(uint16_t), sizeof(GATTS_SERVICE_UUID_CPS), (uint8_t *)&GATTS_SERVICE_UUID_CPS}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_RPM]     =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_notify}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_RPM] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_RPM, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(rpm), (uint8_t *)&rpm}},

    /* Client Characteristic Configuration Descriptor */
    [IDX_CHAR_CFG_RPM]  =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    sizeof(uint16_t), sizeof(rpm_ccc), (uint8_t *)rpm_ccc}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_POWER]     =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_notify}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_POWER] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_PWR, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(pwr), (uint8_t *)&pwr}},

    /* Client Characteristic Configuration Descriptor */
    [IDX_CHAR_CFG_POWER]  =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    sizeof(uint16_t), sizeof(pwr_ccc), (uint8_t *)pwr_ccc}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_REFRESH_TIME] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_REFRESH_TIME] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_REFRESH, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(refresh_time), (uint8_t *)&refresh_time}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_CRANCK_LENGTH] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_CRANCK_LENGTH] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_CRANCK_LENGTH, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(cranck_mm), (uint8_t *)&cranck_mm}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_POWER_OFFSET] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_POWER_OFFSET] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_PWR_OFFSET, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(pwr_offset), (uint8_t *)&pwr_offset}},
};

/* Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t gatt_db_b[IDX_SVC2_NB] =
{
    // Service Declaration
    [IDX_SVC2]        =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
    sizeof(uint16_t), sizeof(GATTS_SERVICE_UUID_DIS), (uint8_t *)&GATTS_SERVICE_UUID_DIS}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_MANUFACTURER_NAME] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_MANUFACTURER_NAME] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_MANU_NAME, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_manufacturer), dis_manufacturer}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_MODEL_NUMBER] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_MODEL_NUMBER] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_MODEL_NUM, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_model_number), dis_model_number}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_SERIAL_NUMBER] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_SERIAL_NUMBER] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_SERIAL_NUM, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_serial_number), dis_serial_number}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_HW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_FW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_HW_VERSION, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_hardware_ver), dis_hardware_ver}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_FW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_HW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_FW_VERSION, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_firmware_ver), dis_firmware_ver}},

    /* Characteristic Declaration ------------------------------------------------------------------------- */
    [IDX_CHAR_SW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
    CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    /* Characteristic Value */
    [IDX_CHAR_VAL_SW_REVISION] =
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_SW_VERSION, ESP_GATT_PERM_READ,
    GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(dis_software_ver), dis_software_ver}},
};

/* Advertisement Handler */

static uint8_t adv_config_done = 0;

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    #ifdef CONFIG_SET_RAW_ADV_DATA
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
            adv_config_done &= (~ADV_CONFIG_FLAG);
            if (adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
            adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
            if (adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
    #else
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~ADV_CONFIG_FLAG);
            if (adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
            if (adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;
    #endif
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            /* advertising start complete event to indicate advertising start successfully or failed */
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(GATTS_TAG, "advertising start failed");
            }else{
                ESP_LOGI(GATTS_TAG, "advertising start successfully");
            }
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(GATTS_TAG, "Advertising stop failed");
            }
            else {
                ESP_LOGI(GATTS_TAG, "Stop adv successfully\n");
            }
            break;
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI(GATTS_TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                  param->update_conn_params.status,
                  param->update_conn_params.min_int,
                  param->update_conn_params.max_int,
                  param->update_conn_params.conn_int,
                  param->update_conn_params.latency,
                  param->update_conn_params.timeout);
            break;
        default:
            break;
    }
}

/* Profile(s) Handler */

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event)
    {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(GATTS_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
            gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_CPS;

            esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(SAMPLE_DEVICE_NAME);
            if (set_dev_name_ret){
                ESP_LOGE(GATTS_TAG, "set device name failed, error code = %x", set_dev_name_ret);
            }
    #ifdef CONFIG_SET_RAW_ADV_DATA
            esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
            if (raw_adv_ret){
                ESP_LOGE(GATTS_TAG, "config raw adv data failed, error code = %x ", raw_adv_ret);
            }
            adv_config_done |= ADV_CONFIG_FLAG;
            esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
            if (raw_scan_ret){
                ESP_LOGE(GATTS_TAG, "config raw scan rsp data failed, error code = %x", raw_scan_ret);
            }
            adv_config_done |= SCAN_RSP_CONFIG_FLAG;
    #else
            //config adv data
            esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
            if (ret){
                ESP_LOGE(GATTS_TAG, "config adv data failed, error code = %x", ret);
            }
            adv_config_done |= ADV_CONFIG_FLAG;
            //config scan response data
            ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
            if (ret){
                ESP_LOGE(GATTS_TAG, "config scan response data failed, error code = %x", ret);
            }
            adv_config_done |= SCAN_RSP_CONFIG_FLAG;
    #endif
            esp_err_t create_attr_ret = esp_ble_gatts_create_attr_tab(gatt_db_a, gatts_if, IDX_SVC1_NB, PROFILE_A_APP_ID);
            if (create_attr_ret){
                ESP_LOGE(GATTS_TAG, "create attr table failed, error code = %x", create_attr_ret);
            }
            break;
        case ESP_GATTS_READ_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_READ_EVT");
            break;
        case ESP_GATTS_WRITE_EVT:
            if (!param->write.is_prep){
                // the data length of gattc write  must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
                ESP_LOGI(GATTS_TAG, "GATT_WRITE_EVT, handle = %d, value len = %d, value :", param->write.handle, param->write.len);
                esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);

                // IDX_CHAR_CFG_RPM
                if (cycling_power_handle_table[IDX_CHAR_CFG_RPM] == param->write.handle && param->write.len == 2){
                    uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
                    if (descr_value == 0x0001){
                        ESP_LOGI(GATTS_TAG, "rpm notify enable");
                        rpm_connected = true;
                    }else if (descr_value == 0x0000){
                        ESP_LOGI(GATTS_TAG, "rpm notify disable ");
                        rpm_connected = false;
                    }else{
                        ESP_LOGE(GATTS_TAG, "unknown descr value");
                        esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
                    }
                }

                // IDX_CHAR_CFG_POWER
                if (cycling_power_handle_table[IDX_CHAR_CFG_POWER] == param->write.handle && param->write.len == 2){
                    uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
                    if (descr_value == 0x0001){
                        ESP_LOGI(GATTS_TAG, "pwr notify enable");
                        pwr_connected = true;
                    }else if (descr_value == 0x0000){
                        ESP_LOGI(GATTS_TAG, "pwr notify disable ");
                        pwr_connected = false;
                    }else{
                        ESP_LOGE(GATTS_TAG, "unknown descr value");
                        esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
                    }
                }

                // IDX_CHAR_VAL_REFRESH_TIME
                if (cycling_power_handle_table[IDX_CHAR_VAL_REFRESH_TIME] == param->write.handle && param->write.len == 1){
                    refresh_time = param->write.value[0];
                    ESP_LOGI(GATTS_TAG, "New refresh time");
                }

                // IDX_CHAR_VAL_CRANCK_LENGTH
                if (cycling_power_handle_table[IDX_CHAR_VAL_CRANCK_LENGTH] == param->write.handle && param->write.len == 1){
                    cranck_mm = param->write.value[0];
                    ESP_LOGI(GATTS_TAG, "New cranck length");
                }

                /* send response when param->write.need_rsp is true*/
                if (param->write.need_rsp){
                    esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
                }
            }else{
                /* handle prepare write */
                example_prepare_write_event_env(gatts_if, &a_prepare_write_env, param);
            }
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            // the length of gattc prepare write data must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
            example_exec_write_event_env(&a_prepare_write_env, param);
            break;
        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
            break;
        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONF_EVT, status = %d, attr_handle %d", param->conf.status, param->conf.handle);
            break;
        case ESP_GATTS_START_EVT:
            ESP_LOGI(GATTS_TAG, "SERVICE_START_EVT, status %d, service_handle %d", param->start.status, param->start.service_handle);
            break;
        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONNECT_EVT, conn_id = %d", param->connect.conn_id);
            esp_log_buffer_hex(GATTS_TAG, param->connect.remote_bda, 6);
            esp_ble_conn_update_params_t conn_params = {0};
            memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            /* For the iOS system, please refer to Apple official documents about the BLE connection parameters restrictions. */
            conn_params.latency = 0;
            conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
            conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
            conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
            //start sent the update connection parameters to the peer device.
            esp_ble_gap_update_conn_params(&conn_params);
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_DISCONNECT_EVT, reason = 0x%x", param->disconnect.reason);
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:{
            if (param->add_attr_tab.status != ESP_GATT_OK){
                ESP_LOGE(GATTS_TAG, "create attribute table failed, error code=0x%x", param->add_attr_tab.status);
            }
            else if (param->add_attr_tab.num_handle != IDX_SVC1_NB){
                ESP_LOGE(GATTS_TAG, "create attribute table abnormally, num_handle (%d) \
                        doesn't equal to IDX_SVC1_NB(%d)", param->add_attr_tab.num_handle, IDX_SVC1_NB);
            }
            else {
                ESP_LOGI(GATTS_TAG, "create attribute table, handle = %d\n",param->add_attr_tab.num_handle);
                memcpy(cycling_power_handle_table, param->add_attr_tab.handles, sizeof(cycling_power_handle_table));
                esp_ble_gatts_start_service(cycling_power_handle_table[IDX_SVC1]);
            }
            break;
        }
        case ESP_GATTS_STOP_EVT:
        case ESP_GATTS_OPEN_EVT:
        case ESP_GATTS_CANCEL_OPEN_EVT:
        case ESP_GATTS_CLOSE_EVT:
        case ESP_GATTS_LISTEN_EVT:
        case ESP_GATTS_CONGEST_EVT:
        case ESP_GATTS_UNREG_EVT:
        case ESP_GATTS_DELETE_EVT:
        default:
            break;
    }
}


static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(GATTS_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
            gl_profile_tab[PROFILE_B_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_DIS;

            esp_err_t create_attr_ret = esp_ble_gatts_create_attr_tab(gatt_db_b, gatts_if, IDX_SVC2_NB, PROFILE_B_APP_ID);
            if (create_attr_ret){
                ESP_LOGE(GATTS_TAG, "create attr table failed, error code = %x", create_attr_ret);
            }
            break;
        case ESP_GATTS_READ_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_READ_EVT");
            break;
        case ESP_GATTS_WRITE_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_WRITE_EVT");
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            // the length of gattc prepare write data must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
            example_exec_write_event_env(&b_prepare_write_env, param);
            break;
        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
            break;
        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONF_EVT, status = %d, attr_handle %d", param->conf.status, param->conf.handle);
            break;
        case ESP_GATTS_START_EVT:
            ESP_LOGI(GATTS_TAG, "SERVICE_START_EVT, status %d, service_handle %d", param->start.status, param->start.service_handle);
            break;
    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONNECT_EVT, conn_id = %d", param->connect.conn_id);
        esp_log_buffer_hex(GATTS_TAG, param->connect.remote_bda, 6);
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the iOS system, please refer to Apple official documents about the BLE connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
        //start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:{
        if (param->add_attr_tab.status != ESP_GATT_OK){
            ESP_LOGE(GATTS_TAG, "create attribute table failed, error code=0x%x", param->add_attr_tab.status);
        }
        else if (param->add_attr_tab.num_handle != IDX_SVC2_NB){
            ESP_LOGE(GATTS_TAG, "create attribute table abnormally, num_handle (%d) \
                    doesn't equal to IDX_SVC2_NB(%d)", param->add_attr_tab.num_handle, IDX_SVC2_NB);
        }
        else {
            ESP_LOGI(GATTS_TAG, "create attribute table, handle = %d\n",param->add_attr_tab.num_handle);
            memcpy(device_info_handle_table, param->add_attr_tab.handles, sizeof(device_info_handle_table));
            esp_ble_gatts_start_service(device_info_handle_table[IDX_SVC2]);
        }
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

/* Att Handler */

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{

    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        } else {
            ESP_LOGE(GATTS_TAG, "reg app failed, app_id %04x, status %d",
                    param->reg.app_id,
                    param->reg.status);
            return;
        }
    }
    do {
        int idx;
        for (idx = 0; idx < PROFILE_NUM; idx++) {
            /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
            if (gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[idx].gatts_if) {
                if (gl_profile_tab[idx].gatts_cb) {
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    } while (0);
}

/* Application */

#define SAMPLES     64
#define LOOPTIME    20
#define AVG         3

float ACC[SAMPLES] = {0};

static const float FIR[SAMPLES] = {
  -0.032324782,0.044535824,0.027708853,0.019761315,
  0.015734866, 0.013160858,0.010829337,0.00818429,
  0.005036703,0.001394091,-0.002604021,-0.006730702,
  -0.010706668,-0.014213031,-0.016915365,-0.018506349,
  -0.018727439,-0.017367625,-0.014281782,-0.009427987,
  -0.00287441,0.005225689,0.014635419,0.025013476,
  0.035936691,0.046954758,0.057597117,0.067362902,
  0.075789171,0.082514315,0.087171213,0.0895621,
  0.0895621,0.087171213,0.082514315,0.075789171,
  0.067362902,0.057597117,0.046954758,0.035936691,
  0.025013476,0.014635419,0.005225689,-0.00287441,
  -0.009427987,-0.014281782,-0.017367625,-0.018727439,
  -0.018506349,-0.016915365,-0.014213031,-0.010706668,
  -0.006730702,-0.002604021,0.001394091,0.005036703,
  0.00818429,0.010829337,0.013160858,0.015734866,
  0.019761315,0.027708853,0.044535824,-0.032324782
  };

TickType_t record_time = 0;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11;

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        ESP_LOGI(GATTS_TAG, "Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        ESP_LOGI(GATTS_TAG, "Characterized using eFuse Vref\n");
    } else {
        ESP_LOGI(GATTS_TAG, "Characterized using Default Vref\n");
    }
}

static void convert_float_u16(float x, uint16_t* result)
{
    *result = (uint16_t)x;
}

static void convert_int_u16(int x, uint16_t* result)
{
    *result = (uint16_t)x;
}

void app_main(void)
{
    esp_err_t ret;

    /* Initialize NVS. */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", ret);
        return;
    }

    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", ret);
        return;
    }

    ret = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
        return;
    }

    ret = esp_ble_gatts_app_register(PROFILE_B_APP_ID);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
        return;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret){
        ESP_LOGE(GATTS_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(ADC_CHANNEL_6, atten);
    adc1_config_channel_atten(ADC_CHANNEL_7, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);



    uint16_t millisecond = 0;
    int adc_ch6, adc_ch7;
    for (;;) {

        adc_ch6 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_6);
        adc_ch7 = adc1_get_raw((adc1_channel_t)ADC_CHANNEL_7);

        if (millisecond > refresh_time * 1000 / LOOPTIME)
        {
            if (rpm_connected) {

                convert_int_u16(adc_ch6, &rpm);
                ESP_LOGI(GATTS_TAG, "rpm = %d, 0x%04X", rpm, rpm);
                esp_ble_gatts_send_indicate(
                                     gl_profile_tab[PROFILE_A_APP_ID].gatts_if,
                                     gl_profile_tab[PROFILE_A_APP_ID].app_id,
                                     cycling_power_handle_table[IDX_CHAR_VAL_RPM],
                                     sizeof(rpm), (uint8_t *)&rpm, false);
            }

            if (pwr_connected) {

                convert_int_u16(adc_ch7, &pwr);
                ESP_LOGI(GATTS_TAG, "power = 0x%04X", pwr);
                esp_ble_gatts_send_indicate(
                                     gl_profile_tab[PROFILE_A_APP_ID].gatts_if,
                                     gl_profile_tab[PROFILE_A_APP_ID].app_id,
                                     cycling_power_handle_table[IDX_CHAR_VAL_POWER],
                                     sizeof(pwr), (uint8_t *)&pwr, false);

            }
            millisecond = 0;
        }
        else
        {
            millisecond++;
        }
        vTaskDelay(LOOPTIME/portTICK_RATE_MS);
    }
}

/* Private Functions */

static float app_GetRPM(int adc_reading)
{
    float oldf, result = 0;
    static float filt = 0;          // keep value from previous iteration
    static float sample[AVG];

    ACC[0] = adc_reading - 2295;    // get the sample and remove offset
    oldf = filt;

    /* FIR output */
    filt=0;
    for (int i=SAMPLES; i>1; i--)
    {
        ACC[i-1] = ACC[i-2];                    // shift register
        filt += (float) ACC[i]*FIR[i];          // getting FIR output
    }

    /* timeout for low speed - less than 30 rpm */
    if ((xTaskGetTickCount()- record_time) > 2000) result=0;

    /* zero cross detection + hyst (5) */
    if( (filt>5 && oldf<-5) || (filt<-5 && oldf>5) )
    {
        sample[0]= 30000 / (xTaskGetTickCount()- record_time);  // calculate rpms
        record_time = xTaskGetTickCount();                          // restart time counter

        result = 0;
        for (int i=AVG; i>1; i--) {
            sample[i-1] = sample[i-2];          // shift sample array
            result += sample[i-1] / (AVG-1);    // calculate RPM average
        }
    }
    return result;
}

static float app_GetPower(int adc_reading, float angular_speed)
{
    float watts = 1.0;
    float newtons;

    newtons = adc_reading - 2295;   // get the sample and remove offset

    /* FIR output */

    watts *= newtons;
    watts *= cranck_mm * 0.001;
    watts *= angular_speed;
    watts += pwr_offset;
    return watts;
}

void example_prepare_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    ESP_LOGI(GATTS_TAG, "prepare write, handle = %d, value len = %d", param->write.handle, param->write.len);
    esp_gatt_status_t status = ESP_GATT_OK;
    if (prepare_write_env->prepare_buf == NULL) {
        prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE * sizeof(uint8_t));
        prepare_write_env->prepare_len = 0;
        if (prepare_write_env->prepare_buf == NULL) {
            ESP_LOGE(GATTS_TAG, "%s, Gatt_server prep no mem", __func__);
            status = ESP_GATT_NO_RESOURCES;
        }
    } else {
        if(param->write.offset > PREPARE_BUF_MAX_SIZE) {
            status = ESP_GATT_INVALID_OFFSET;
        } else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
            status = ESP_GATT_INVALID_ATTR_LEN;
        }
    }
    /*send response when param->write.need_rsp is true */
    if (param->write.need_rsp){
        esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
        if (gatt_rsp != NULL){
            gatt_rsp->attr_value.len = param->write.len;
            gatt_rsp->attr_value.handle = param->write.handle;
            gatt_rsp->attr_value.offset = param->write.offset;
            gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
            memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
            esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
            if (response_err != ESP_OK){
               ESP_LOGE(GATTS_TAG, "Send response error");
            }
            free(gatt_rsp);
        }else{
            ESP_LOGE(GATTS_TAG, "%s, malloc failed", __func__);
        }
    }
    if (status != ESP_GATT_OK){
        return;
    }
    memcpy(prepare_write_env->prepare_buf + param->write.offset,
           param->write.value,
           param->write.len);
    prepare_write_env->prepare_len += param->write.len;

}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC && prepare_write_env->prepare_buf){
        esp_log_buffer_hex(GATTS_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }else{
        ESP_LOGI(GATTS_TAG,"ESP_GATT_PREP_WRITE_CANCEL");
    }
    if (prepare_write_env->prepare_buf) {
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}
