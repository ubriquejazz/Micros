#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_gatt_defs.h"

#define GATTS_TAG "           BLE SERVER"
#define SAMPLE_DEVICE_NAME          "GALAN_BIKE"
#define MANUFACTURER_NAME           "IDNEO"
#define MODEL_NUMBER                "1"
#define SERIAL_NUMBER               "SN0001"
#define HW_REVISION                 "RevA"
#define FW_REVISION                 "0.0.1"
#define SW_REVISION                 "0.0.1"

#define PROFILE_NUM                 2
#define PROFILE_A_APP_ID            0
#define PROFILE_B_APP_ID            1

/* The max length of characteristic value. When the GATT client performs a write or prepare write operation,
*  the data length must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX.
*/
#define GATTS_DEMO_CHAR_VAL_LEN_MAX 500
#define PREPARE_BUF_MAX_SIZE        1024
#define CHAR_DECLARATION_SIZE       (sizeof(uint8_t))

#define ADV_CONFIG_FLAG             (1 << 0)
#define SCAN_RSP_CONFIG_FLAG        (1 << 1)

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

/* Services */

static const uint16_t GATTS_SERVICE_UUID_TEST      = 0x00FF;
static const uint16_t GATTS_CHAR_UUID_TEST_A       = 0xFF01;
static const uint16_t GATTS_CHAR_UUID_TEST_B       = 0xFF02;
static const uint16_t GATTS_CHAR_UUID_TEST_C       = 0xFF03;

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

/* BLE Variables */

uint16_t rpm, pwr;
uint8_t  refresh_time = 3;
uint16_t cranck_mm = 180;
int16_t  pwr_offset = 0;

uint8_t dis_manufacturer[] = MANUFACTURER_NAME;
uint8_t dis_model_number[] = MODEL_NUMBER;
uint8_t dis_serial_number[] = SERIAL_NUMBER;
uint8_t dis_hardware_ver[] = HW_REVISION;
uint8_t dis_firmware_ver[] = FW_REVISION;
uint8_t dis_software_ver[] = SW_REVISION;

// characteristic notification
static bool rpm_connected = false;
static bool pwr_connected = false;
uint8_t  rpm_ccc[2] = {0x00, 0x00}; // notification - IDX_CHAR_CFG_RPM
uint8_t  pwr_ccc[2] = {0x00, 0x00}; // notification - IDX_CHAR_CFG_PWR



/* Attributes State Machine */
enum
{
    IDX_SVC1,

//    IDX_CHAR_A,
//    IDX_CHAR_VAL_A,
//    IDX_CHAR_CFG_A,
//
//    IDX_CHAR_B,
//    IDX_CHAR_VAL_B,
//
//    IDX_CHAR_C,
//    IDX_CHAR_VAL_C,

  IDX_CHAR_RPM,
  IDX_CHAR_VAL_RPM,
  IDX_CHAR_CFG_RPM,

  IDX_CHAR_POWER,
  IDX_CHAR_VAL_POWER,
  IDX_CHAR_CFG_POWER,

  IDX_CHAR_REFRESH_TIME,
  IDX_CHAR_VAL_REFRESH_TIME,

  IDX_CHAR_CRANCK_LENGTH,
  IDX_CHAR_VAL_CRANCK_LENGTH,

  IDX_CHAR_POWER_OFFSET,
  IDX_CHAR_VAL_POWER_OFFSET,    // could be negative

  IDX_SVC1_NB,
};

enum
{
    IDX_SVC2,
  IDX_CHAR_MANUFACTURER_NAME,
  IDX_CHAR_VAL_MANUFACTURER_NAME,

  IDX_CHAR_MODEL_NUMBER,
  IDX_CHAR_VAL_MODEL_NUMBER,

  IDX_CHAR_SERIAL_NUMBER,
  IDX_CHAR_VAL_SERIAL_NUMBER,

  IDX_CHAR_HW_REVISION,
  IDX_CHAR_VAL_HW_REVISION,

  IDX_CHAR_FW_REVISION,
  IDX_CHAR_VAL_FW_REVISION,

  IDX_CHAR_SW_REVISION,
  IDX_CHAR_VAL_SW_REVISION,

  IDX_SVC2_NB,
};

/* Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t gatt_db[IDX_SVC1_NB] =
{
//    // Service Declaration
//    [IDX_SVC1]        =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
//      sizeof(uint16_t), sizeof(GATTS_SERVICE_UUID_TEST), (uint8_t *)&GATTS_SERVICE_UUID_TEST}},
//
//    /* Characteristic Declaration */
//    [IDX_CHAR_A]     =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
//      CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},
//
//    /* Characteristic Value */
//    [IDX_CHAR_VAL_A] =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_TEST_A, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
//      GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t *)char_value}},
//
//    /* Client Characteristic Configuration Descriptor */
//    [IDX_CHAR_CFG_A]  =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
//      sizeof(uint16_t), sizeof(heart_measurement_ccc), (uint8_t *)heart_measurement_ccc}},
//
//    /* Characteristic Declaration */
//    [IDX_CHAR_B]      =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
//      CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},
//
//    /* Characteristic Value */
//    [IDX_CHAR_VAL_B]  =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_TEST_B, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
//      GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t *)char_value}},
//
//    /* Characteristic Declaration */
//    [IDX_CHAR_C]      =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
//      CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_write}},
//
//    /* Characteristic Value */
//    [IDX_CHAR_VAL_C]  =
//    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&GATTS_CHAR_UUID_TEST_C, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
//      GATTS_DEMO_CHAR_VAL_LEN_MAX, sizeof(char_value), (uint8_t *)char_value}},

};

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

