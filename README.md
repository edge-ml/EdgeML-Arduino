# EdgeML-Arduino

## Table of Contents
- [Pre-requistes](#Pre-requistes)
- [How to install](#How-to-install)
- [Github actions firmware compilation](#Github-actions-firmware-compilation)
- [BLE Specification](#BLE-Specification)
- [Usage and functionality](#Usage-and-functionality)
- [Custom Board Configuration](#Custom-Board-Configuration)
  - [Custom SensorID](#Custom-SensorID)
  - [Custom Sensor Manager Interface](#Custom-Sensor-Manager-Interface)
  - [Custom Sensor](#Custom-Sensor)
- [Attribution](#Attribution)

---
## Pre-requistes
This project targets the Arduino Arduino Nano 33 BLE Sense and Nicla Sense ME. Before your start, please download the required support libraries in your Arduino IDE. Also, please make sure you have the latest Arduino IDE version installed before your start.

---
## How to install
1. Open the Arduino library manager
<img src="https://user-images.githubusercontent.com/11386075/143419661-5aa339dd-e5c2-4891-a28e-72831c764fa2.png" alt="alt text" width="500">

2. Install the edge-ml library using the integrated libary manager in the Arduino-IDE by searching for "EdgeML-Arduino".
<img src="https://user-images.githubusercontent.com/11386075/143421304-d4ab20ff-c1e8-48bf-a3cd-6134e3c5b25e.png" alt="alt text" width="500">

3. Once you are asked if you would like to install the required dependencies, select "Install all". (Note: if you already have the required dependencies installed at some point in the past this dialog will not show).
<img src="https://user-images.githubusercontent.com/11386075/143424434-37db4466-d471-4cae-9255-2247005d761e.png" alt="alt text" width="500">


4. Select your board from the boards manager (Arduino Nano 33 BLE or Nicla Sense ME)
<img src="https://user-images.githubusercontent.com/11386075/143422287-a209679a-7235-4249-9318-d7d7ad92a4c4.png" alt="alt text" width="500">

5. Connect your Arduino to your PC via Micro USB cable and select the port your Arduino is connected to (in this example the Nicla Sense ME, if you have a Nano 33 BLE Sense, then select the port it shows up on)
<img src="https://user-images.githubusercontent.com/11386075/143423313-cb7e4a89-c305-4ad8-9213-96d3bedc845d.png" alt="alt text" width="500">

6. Open the edge-ml firmware app by selecting it from the list of examples.
<img src="https://user-images.githubusercontent.com/11386075/143424987-a3ec925c-97e2-4a3b-bdf5-9425324a5603.png" alt="alt text" width="500">

7. Flash it onto your Arduino board by hitting the upload button (this may take a while).
<img src="https://user-images.githubusercontent.com/11386075/143425338-2db4c4f1-b4d1-455f-850a-fac7556417ef.png" alt="alt text" width="200">

8. You can now connect to your Arduino from edge-ml.

---
## Github actions firmware compilation
Currently, the firmware for the Nicla Sense ME, the Nano 33 BLE and the Seeed Xiao nRF52840 Sense is compiled using GitHub actions and are provided as build artifacts. Artifacts can be downloaded through the following links:

| Board                     | Firmware                                                                       |
|---------------------------|--------------------------------------------------------------------------------|
| Nicla Sense ME            | https://nightly.link/edge-ml/EdgeML-Arduino/workflows/build/main/nicla.bin.zip |
| Nano 33 BLE               | https://nightly.link/edge-ml/EdgeML-Arduino/workflows/build/main/nano.bin.zip  |
| Seeed Xiao nRF52840 Sense | https://nightly.link/edge-ml/EdgeML-Arduino/workflows/build/main/xiao.bin.zip  |

---
##BLE Specification

The following table contains the BLE specifications with the available Services and Characteristics as well as UUIDs.

| Service Name        | Service UUID                             | Characteristic Name  | Characteristic UUID                    |
|---------------------|------------------------------------------|----------------------|----------------------------------------|
| Sensor Service      | `34c2e3bb-34aa-11eb-adc1-0242ac120002`   | Sensor Configuration | `34c2e3bd-34aa-11eb-adc1-0242ac120002` |
|                     |                                          | Sensor Data          | `34c2e3bc-34aa-11eb-adc1-0242ac120002` |
| Device Info Service | `45622510-6468-465a-b141-0b9b0f96b468`   | Device Identifier    | `45622511-6468-465a-b141-0b9b0f96b468` |
|                     |                                          | Device Generation    | `45622512-6468-465a-b141-0b9b0f96b468` |
| Parse Info Service  | `caa25cb7-7e1b-44f2-adc9-e8c06c9ced43`   | Scheme               | `caa25cb8-7e1b-44f2-adc9-e8c06c9ced43` |

### Sensor Configuration Characteristic
Permissions: Write

This characteristic is used to send a sensor configuration to the Earable.

A configuration packet is an implemented struct:

```c++
struct SensorConfigurationPacket {
    uint8_t sensorId;
    float sampleRate;
    uint32_t latency;
};
```

Configuration Package structure:

| Byte 0    | Byte 1-4     | Byte 5-8  |
|-----------|--------------|-----------|
| sensorId  | sampleRate   | latency   |
| uint8     | float        | uint32    |

sensorId: ID of the sensor.<br>
sampleRate: Desired sample rate. <br>
latency: Legacy field which is mostly ignored.

Each sensor or audio IO can be enabled individually or together at the same time with predefined configurations.
It is recommended to use the predefined configurations.

### Sensor Data Characteristic
Permissions: Read/Notify

This Characteristic is responsible for sending data packages from the Earable to the connected device.

Data Package structure:

| Byte 0    | Byte 1-4    | Byte 5-X   |
|-----------|-------------|------------|
| SensorID  | Time Stamp  | Data Array |
| uint8     | uint32      | ---        |

SensorID: ID of the sensor.<br>
Time Stamp:  Timestamp in milliseconds.<br>
Data Array: Array of bytes, which need to be parsed according the sensors parsing scheme.

### Device Identifier Characteristic
Permissions: Read

This characteristic is used to get the Device Identifier string.

### Device Generation Characteristic
Permissions: Read

This characteristic is used to get the Device Generation string.

### Scheme Characteristic
Permissions: Read

With this characteristic the parsing scheme information can be requested from the device.
The parsing scheme is needed to convert a received data package to usable values.


The received buffer can be represented as such:

| Byte 0       | Byte 1 - Byte X        | Byte X+1 - Byte Y      | ... |
|--------------|------------------------|------------------------|-----|
| Scheme Count | Scheme Packet Sensor 0 | Scheme Packet Sensor 0 | ... |
| uint8        | Scheme Packet          | Scheme Packet          | ... |

Scheme Count is the total number Scheme Packets.<br>

Scheme Packet structure:

| Byte 0   | Byte 1             | Byte 2 - Byte X | Byte X+1        | Byte X+2 - Byte Y  | Byte Y+1 - Byte Z  | ...  |
|----------|--------------------|-----------------|-----------------|--------------------|--------------------|------|
| SensorID | Sensor Name Length | Sensor Name     | Component Count | Component Packet 0 | Component Packet 1 | ...  |
| uint8    | uint8              | char array      | uint8           | Component Packet   | Component Packet   | ...  |

SensorID is ID of sensor.<br>
Sensor Name Length is length of sensor name char array.<br>
Sensor Name is name char array.<br>
Component Count is count of total number of Component Packets of the sensor. 

Scheme Packet structure:

| Byte 0 | Byte 1            | Byte 2 - Byte X | Byte X+1              | Byte X+2 - Byte Y | Byte Y+1 - Byte Z | Byte Z+1 - Byte A |
|--------|-------------------|-----------------|-----------------------|-------------------|-------------------|-------------------|
| Type   | Group Name Length | Group Name      | Component Name Length | Component Name    | Unit Name Length  | Unit Name         |
| uint8  | uint8             | char array      | uint8                 | char array        | uint8             | char array        |

Type is the data type of component.<br>
Group Name Length is length of group name char array.<br>
Group Name is name char array.<br>
Component Name Length is length of component name char array.<br>
Component Name is name char array.<br>
Unit Name Length is length of unit name char array.<br>
Unit Name is name char array.

Data types:
```c++
enum ParseType {
    PARSE_TYPE_INT8,
    PARSE_TYPE_UINT8,

    PARSE_TYPE_INT16,
    PARSE_TYPE_UINT16,

    PARSE_TYPE_INT32,
    PARSE_TYPE_UINT32,

    PARSE_TYPE_FLOAT,
    PARSE_TYPE_DOUBLE
};
```
(Enums are integers in ascending order starting from 0)

---
## Usage and functionality
The easiest way to use edge-ml is with the provided `App` sketch.
The absolute minimum needed to run the code successfully is the following:

```c++
#include "EdgeML.h"

void setup() {
  edge_ml.begin();
}

void loop() {
  edge_ml.update();
}
```
However, there are a few more functionalities, which the basic edgeml offers to allow better integration.

(Note: The following is only applicable for non-Nicla boards or custom EdgeML implementations)

#### `configure_sensor(SensorConfigurationPacket& config)`

Manually send a Sensor Configuration Packet to EdgeML.

```c++
SensorConfigurationPacket config;
// Fill config with values

edge_ml.configure_sensor(config);
```


#### `String get_name()`

Returns the current name of the device.

```c++
String deviceName = edge_ml.get_name();
```

#### `void ble_manual_advertise()`

Stop EdgeML from automatically advertising BLE services during `edge_ml.begin()`.
This has to be use if custom BLE Services and Characteristics are defined outside EdgeML.
Make sure to then place the custom BLE Services and Characteristics after `edge_ml.begin()`.

```c++
edge_ml.ble_manual_advertise();

// Other BLE Services and Characteristics

BLE.advertise();
```

#### `void set_ble_config(String name, String version)`

Sets the name of the device as well as the current version string.

```c++
edge_ml.set_ble_config("MyDevice", "1.2.3");
```

#### `int get_active_count()`

Returns the number of currently active sensors.

```c++
int activeSensors = edge_ml.get_active_count();
```

#### `void set_data_callback(void(*callback)(int id, unsigned int timestamp, uint8_t* data, ReturnType r_type))`

Allows to set a custom callback function that is triggered when a sensor provides a new value. The callback function must have the following signature:

```c++
void callback(int id, unsigned int timestamp, uint8_t* data, int size)
```

- `id` (integer): The ID of the sensor.
- `timestamp` (unsigned int): The timestamp of the sensor data.
- `data` (uint8_t*): A pointer to an array of `uint8_t` that contains the sensor data. (Index 0: ID; Index 1: total size; Rest: data)
- `size` (int): Total size of data array. 

```c++
void handleSensorData(int id, unsigned int timestamp, uint8_t* data, int size) {
  // Your custom logic here
}

// Setting the callback function
edge_ml.set_data_callback(handleSensorData);
```

#### `void set_config_callback(void(*callback)(SensorConfigurationPacket *))`

Allows to set a custom callback function that is triggered when a ble configuration package is received. The callback function must have the following signature:

```c++
void callback(SensorConfigurationPacket * config)
```

```c++
void handleConfig(SensorConfigurationPacket * config) {
  // Your custom logic here
}

// Setting the callback function
edge_ml.set_config_callback(handleSensorData);
```

#### `void set_custom(SensorManagerInterface * sensorManager) `
Allows the user to set a custom Sensor Manager to implement a custom Board Configuration.
More on that in the chapter below.

---
## Custom Board Configuration

Users have the flexibility to utilize EdgeML with other boards and/or sensors by implementing their own Custom Board Configuration. This involves three parts:

- Custom SensorID
- Custom Sensor Manager Interface
- Custom Sensor

### Custom SensorID
The `SensorID.h` file is used to define the sensor configurations that will be injected into EdgeML. It specifies the identification and characteristics of the sensors to be integrated with the EdgeML framework. (Please note that this file does not contain the actual sensor implementations, which will be discussed later.)

The `SensorID.h` file should include the following components:

#### Import

```cpp
#include "EdgeML_Custom.h"
```

#### Number of Sensors and Physical Modules

Specify the total number of sensors and physical modules in the system using the following constants:

```cpp
const int SENSOR_COUNT = 3;
const int MODULE_COUNT_PHYSICAL = 2;
```

#### Enum of SensorIDs

Define the sensor IDs as an enumeration in ascending order:

```cpp
enum SensorID {
    IMU_ACCELERATION,
    IMU_GYROSCOPE,
    BARO_PRESSURE
};
```

#### Enum of Modules

Define the module IDs as an enumeration in ascending order:

```cpp
enum ModuleID {
    MODULE_IMU,
    MODULE_BARO
};
```

#### Sensor Components

Each sensor has components representing the structure of the data the sensor produces.

The `SensorComponent` struct looks the following:

```cpp
struct SensorComponent {
    String group_name;
    ParseType type;
    String component_name;
    String unit;
};
```
- `group_name`: The name of the group this component belongs to. (e.g. "ACC", "GYRO",...)
- `type`: The parsing type for the sensor data (enum `ParseType`).
- `component_name`: The name of the component (e.g. "X", "Y", "Z", "Value",...)
- `unit`: The name of the unit of the components value. The unit is optional and can be left empty.

Note that group_name only becomes relevant if the sensor provides data from several different sources that should not be interpreted together.
For example, if the sensor is an accelerometer and its acceleration and gyro data is supposed to be sent together. They can be given their own group under the same sensor.


The sensor components need to be defined as const array of `SensorComponent` structs, seen in the following example:

```cpp
const SensorComponent ACC_COMPONENTS[] = {
        {"ACC", PARSE_TYPE_FLOAT, "X", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Y", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Z", "g"}
};
```

#### Array of SensorConfig Structs

The `SensorConfig` struct defines the configuration for each sensor. It contains the following fields:

```cpp
struct SensorConfig {
    String name;
    int sensor_id;
    int module_id;
    int component_count;
    const SensorComponent * components;
};
```

- `name`: The name of the sensor.
- `sensor_id`: The ID of the sensor (defined in the `SensorID` enumeration).
- `module_id`: The ID of the module to which the sensor belongs (defined in the `ModuleID` enumeration).
- `component_count`: The number of components the sensor holds (can be 0).
- `components`: The pointer to the `SensorComponent` array.

Here's an example of how the array of `SensorConfig` structs should be defined:

```cpp
const SensorConfig CONFIG[SENSOR_COUNT] = {
    {"ACC", IMU_ACCELERATION, MODULE_IMU, 3, ACC_COMPONENTS},
    {"GYRO", IMU_GYROSCOPE, MODULE_IMU, 3, GYRO_COMPONENTS},
    {"PRESSURE", BARO_PRESSURE, MODULE_BARO, 1, PRESSURE_COMPONENTS}
};
```

This array defines the sensor configurations, including their names, IDs, module IDs, return types, parsing schemes, and parsing types.

#### Special Sensors

Special sensors are sensors that are ignored by the EdgeML framework.
This can be useful when a `config_callback` is used. 
The user can process the configuration package without EdgeML trying to initialize a sensor.


Special sensors are optional.

The special sensors get their own SensorID and Dummy Module in the enums.
They also get included into the CONFIG list. The component field can be set to 0.
They count towards the total number of sensors.

Here's an example:
```c++
const int SPECIAL_SENSOR_COUNT = 1;

enum SensorID {
    // Normal Sensors
    SPECIAL_SENSOR
};

enum ModuleID {
    // Normal Modules
    MODULE_DUMMY
};

const int SpecialSensors[SPECIAL_SENSOR_COUNT] = {
        SPECIAL_SENSOR
};

const SensorConfig CONFIG[SENSOR_COUNT] = 
    // Normal sensor configs
    {"Special Name", SPECIAL_SENSOR, MODULE_DUMMY, 0, {}}
};
```

### Custom Sensor Manager Interface

To create a custom sensor manager that integrates custom sensors with EdgeML, a class should be defined that inherits from the `SensorManagerInterface`. This class acts as the interface for managing the custom sensors and can be named `CustomSensorManager`.

#### Includes

Include the necessary headers for the `SensorID.h` and the custom sensors. For example:

```cpp
#include "SensorID.h"

#include "CustomIMUSensor.h"
#include "CustomBAROSensor.h"
```

#### Custom Sensor Manager Class

Next, define the `CustomSensorManager` class that inherits from `SensorManagerInterface`. This class implements the required setup method. Here's an example:

```cpp
class CustomSensorManager : public SensorManagerInterface {
public:
    void setup() override {
        // Create instances of the custom sensors
        CustomIMUSensor *sensorIMU = new CustomIMUSensor();
        CustomBAROSensor *sensorBARO = new CustomBAROSensor();

        // Create an array of SensorInterface pointers
        SensorInterface **modules = new SensorInterface *[MODULE_COUNT_PHYSICAL] {sensorIMU, sensorBARO};

        // Set the modules and sensor counts
        SensorManagerInterface::set_modules(modules);
        SensorManagerInterface::set_sensor_counts(SENSOR_COUNT, MODULE_COUNT_PHYSICAL);

        // Set the sensor configurations
        SensorManagerInterface::set_sensor_configs(CONFIG);
    }
};
```

In the `setup` method of the `CustomSensorManager` class, perform the following steps:

1. Create instances of the custom sensors.
2. Create an array of `SensorInterface` pointers, `modules`, containing the pointers to the custom sensors.
3. Use the `set_modules` method of `SensorManagerInterface` to set the `modules` array.
4. Use the `set_sensor_counts` method of `SensorManagerInterface` to set the total number of sensors (`SENSOR_COUNT`) and physical modules (`MODULE_COUNT_PHYSICAL`).
5. Use the `set_sensor_configs` method of `SensorManagerInterface` to set the sensor configurations (`CONFIG`).

If there are any special sensors they get included as follows:

```c++
class CustomSensorManager : public SensorManagerInterface {
public:
    void setup() override {
        // Create instances of the custom sensors
        CustomIMUSensor *sensorIMU = new CustomIMUSensor();
        CustomBAROSensor *sensorBARO = new CustomBAROSensor();
        
        // Dummy sensor
        DummySensor * dummy = new DummySensor();

        // Create an array of SensorInterface pointers
        SensorInterface **modules = new SensorInterface *[MODULE_COUNT_PHYSICAL] {sensorIMU, sensorBARO, dummy};

        // Set the modules and sensor counts
        SensorManagerInterface::set_modules(modules);
        SensorManagerInterface::set_sensor_counts(SENSOR_COUNT, MODULE_COUNT_PHYSICAL);
        
        // Set special sensors and special sensor count
        SensorManagerInterface::set_special_sensors(SpecialSensors, SPECIAL_SENSOR_COUNT);
        
        // Set the sensor configurations
        SensorManagerInterface::set_sensor_configs(CONFIG);
    }
};
```

---
### Custom Sensor

To integrate a custom sensor with EdgeML, a custom sensor class needs to be created that inherits from the `SensorInterface`. This class serves as the interface for EdgeML to communicate with the actual sensor.

#### Includes

Include the necessary headers. For example:

```cpp
#include "SensorID.h"
```
Add any further includes that are needed to drive the used sensor appropriately.

#### Custom Sensor Class

Next, define the `CustomSensor` class that inherits from `SensorInterface`. Here's an example:

```cpp
class CustomIMUSensor : public SensorInterface {
public:
    CustomIMUSensor();

    void start() override;
    void stop() override;
    
    void get_data(int sensorID, byte *data) override;

    int get_sensor_count() override;

    const int sensor_count = 2;
};
```

In the `CustomSensor` class, the following methods need to be implemented:
- Constructor: Implement any necessary initialization of the sensor that should occur only once.
- `start`: This method is called each time when the sensor is started. Implement any necessary setup code within this method.
- `stop`: This method is called each time when the sensor is stopped. Implement any necessary cleanup or shutdown code within this method.
- `get_data`: This method retrieves the sensor data. Based on the `sensorID`, retrieve the corresponding sensor data and store it in the `data` array. Cast `data` array pointer as needed (to int or float).
- `get_sensor_count`: This method returns the total number of sensors managed by the custom sensor. Set the value of `sensor_count` to the appropriate count.

---
## Attribution
This repository contains code from https://github.com/arduino-libraries/Arduino_BHY2
