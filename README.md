# {IN DEVELOPMENT}
# {DOC NOT UPDATED}

# EdgeML-Arduino

## Table of Contents
- [Pre-requistes](#Pre-requistes)
- [How to install](#How-to-install)
- [Github actions firmware compilation](#Github-actions-firmware-compilation)
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

#### `String get_name()`

Returns the current name of the device.

```c++
String deviceName = edge_ml.get_name();
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
void callback(int id, unsigned int timestamp, uint8_t* data, ReturnType r_type)
```

- `id` (integer): The ID of the sensor.
- `timestamp` (unsigned int): The timestamp of the sensor data.
- `data` (uint8_t*): A pointer to an array of `uint8_t` that contains the sensor data. Ensure that the first element of the casted array (`uint8_t` array to `int` or `float` array) indicates the number of sensor values stored starting from index 1.
- `r_type` (ReturnType): An argument indicating how the data array should be interpreted. Currently, it can be either `int` or `float`. (This may be subject to change)

```c++
void handleSensorData(int id, unsigned int timestamp, uint8_t* data, ReturnType r_type) {
  // Your custom logic here
}

// Setting the callback function
edge_ml.set_data_callback(handleSensorData);
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

#### Array of SensorConfig Structs

The `SensorConfig` struct defines the configuration for each sensor. It contains the following fields:

```cpp
struct SensorConfig {
    String name;
    int sensor_id;
    int module_id;
    ReturnType return_type;
    ParseScheme scheme;
    ParseType type;
};
```

- `name`: The name of the sensor.
- `sensor_id`: The ID of the sensor (defined in the `SensorID` enumeration).
- `module_id`: The ID of the module to which the sensor belongs (defined in the `ModuleID` enumeration).
- `return_type`: The data type returned by the sensor (enum `ReturnType`).
- `scheme`: The parsing scheme for the sensor data (enum `ParseScheme`).
- `type`: The parsing type for the sensor data (enum `ParseType`).

Here's an example of how the array of `SensorConfig` structs should be defined:

```cpp
const SensorConfig CONFIG[SENSOR_COUNT] = {
    {"ACC", IMU_ACCELERATION, MODULE_IMU, R_TYPE_FLOAT, SCHEME_XYZ, PARSE_TYPE_FLOAT},
    {"GYRO", IMU_GYROSCOPE, MODULE_IMU, R_TYPE_FLOAT, SCHEME_XYZ, PARSE_TYPE_FLOAT},
    {"PRESSURE", BARO_PRESSURE, MODULE_BARO, R_TYPE_FLOAT, SCHEME_VAL, PARSE_TYPE_FLOAT}
};
```

This array defines the sensor configurations, including their names, IDs, module IDs, return types, parsing schemes, and parsing types.

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

    void get_float_data(float floatArray[], int sensorID) override;
    void get_int_data(int intArray[], int sensorID) override;

    int get_sensor_count() override;

    const int sensor_count = 2;
};
```

In the `CustomSensor` class, the following methods need to be implemented:
- Constructor: Implement any necessary initialization of the sensor that should occur only once.
- `start`: This method is called each time when the sensor is started. Implement any necessary setup code within this method.
- `stop`: This method is called each time when the sensor is stopped. Implement any necessary cleanup or shutdown code within this method.
- `get_float_data`: This method retrieves the sensor data in floating-point format. It takes an array, `floatArray`, and a `sensorID` as parameters. Based on the `sensorID`, retrieve the corresponding sensor data and store it in the `floatArray`. Ensure that the first element of the array (`floatArray[0]`) indicates the number of sensor values stored starting from index 1.
- `get_int_data`: This method is similar to `get_float_data` but retrieves the sensor data in integer format. Follow the same approach as in `get_float_data`, but with the appropriate data type and indexing.
- `get_sensor_count`: This method returns the total number of sensors managed by the custom sensor. Set the value of `sensor_count` to the appropriate count.

---
## Attribution
This repository contains code from https://github.com/arduino-libraries/Arduino_BHY2
