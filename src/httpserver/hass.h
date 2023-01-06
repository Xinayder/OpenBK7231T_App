
#include "new_http.h"
#include "../cJSON/cJSON.h"
#include "../new_pins.h"
#include "../mqtt/new_mqtt.h"

typedef enum {
	/// @brief Switch
	ENTITY_RELAY = 0,

	/// @brief Single PWM
	ENTITY_LIGHT_PWM,

	/// @brief 2 PWM setup (brightness and temperature)
	ENTITY_LIGHT_PWMCW,

	/// @brief RGB (3 PWM)
	ENTITY_LIGHT_RGB,

	/// @brief RGB + temperature (5 PWM or LED driver)
	ENTITY_LIGHT_RGBCW,

	/// @brief Sensor (voltage, current, power)
	ENTITY_SENSOR,

	/// @Brief Binary Sensor
	ENTITY_BINARY_SENSOR
} ENTITY_TYPE;

//unique_id is defined in hass_populate_unique_id and is based on CFG_GetDeviceName() whose size is CGF_DEVICE_NAME_SIZE.
//Sample unique_id would be deviceName_entityType_index.
//Currently supported entityType is `relay` or `light` - 5 char.
#define HASS_UNIQUE_ID_SIZE     (CGF_DEVICE_NAME_SIZE + 1 + 5 + 1 + 4)

//channel is based on unique_id (see hass_populate_device_config_channel)
#define HASS_CHANNEL_SIZE       (HASS_UNIQUE_ID_SIZE + 32)

//Size of JSON (1 less than MQTT queue holding)
#define HASS_JSON_SIZE          (MQTT_PUBLISH_ITEM_VALUE_LENGTH - 1)

/// @brief HomeAssistant device discovery information
typedef struct HassDeviceInfo_s {
	char unique_id[HASS_UNIQUE_ID_SIZE];
	char channel[HASS_CHANNEL_SIZE];
	char json[HASS_JSON_SIZE];

	cJSON* root;
	cJSON* device;
	cJSON* ids;
} HassDeviceInfo;

void hass_print_unique_id(http_request_t* request, const char* fmt, ENTITY_TYPE type, int index);
HassDeviceInfo* hass_init_relay_device_info(int index);
HassDeviceInfo* hass_init_light_device_info(ENTITY_TYPE type);
HassDeviceInfo* hass_init_sensor_device_info(int index);
HassDeviceInfo* hass_init_binary_sensor_device_info(int index);
char* hass_build_discovery_json(HassDeviceInfo* info);
void hass_free_device_info(HassDeviceInfo* info);
