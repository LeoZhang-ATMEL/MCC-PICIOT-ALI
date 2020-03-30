#ifndef IOT_SENSOR_NODE_CONFIG_H
#define IOT_SENSOR_NODE_CONFIG_H

// <h> Application Configuration

// <o> Send Interval <0-100000>
// <i> Send interval in seconds
// <id> application_send_interval
#define CFG_SEND_INTERVAL 1

// <o> Timeout <0-100000>
// <i> Timeout
// <id> application_timeout
#define CFG_TIMEOUT 5000

// </h>

// <h> Cloud Configuration

// <s> project id
// <i> Google Cloud Platform project id
// <id> project_id
#define CFG_PRODUCT_KEY "a12SBAhLjnt"

// <s> registry id
// <i> Google Cloud Platform registry id
// <id> registry_id
#define CFG_DEVICE_NAME "D804"

#define CFG_DEVICE_SECRET "Zv4yGHpUN3fGWljc2OAndunvtRUyGg7g"

#define CFG_WRITE_DEVICE_SECRET 1

#define MAX_PRODUCT_KEY_LENGTH 11
#define MAX_DEVICE_NAME_LENGTH 30
#define DEVICE_SECRET_LENGTH 32
// <s> mqtt host
// <i> mqtt host address
// <id> mqtt_host
#define CFG_MQTT_HOST_SUFFIX ".iot-as-mqtt.cn-shanghai.aliyuncs.com"

#define CFG_MQTT_HOST CFG_PRODUCT_KEY CFG_MQTT_HOST_SUFFIX
// <s> mqtt port
// <i> mqtt port value
// <id> mqtt_port
#define CFG_MQTT_PORT 1883

// <s> mqtt connect timeout
// <i> mqtt connect timeout length in seconds
// <id> mqtt_conn_timeout
#define CFG_MQTT_CONN_TIMEOUT 10

// </h>

// <h> Debug Configuration

// <q> Enable debug messages:
// <i> Check to enable debug messages
// <id> debug_msg
#define CFG_DEBUG_MSG 0

// </h>

// <h> CLI Support

// <q> Enable CLI:
// <i> Check to enable cli
// <id> enable_cli
#define CFG_ENABLE_CLI 1

// </h>


#endif // IOT_SENSOR_NODE_CONFIG_H
