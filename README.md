# MyArduinoLibrary

MyArduinoLibrary is a collection of utility functions designed for calculations and transformations in Arduino projects. This library provides functions for mapping values, constraining values, calculating angles, and rotating roll-pitch structures.

## Features

- **Value Mapping**: Map a float value from one range to another.
- **Value Constraining**: Constrain a float value within a specified range.
- **Angle Calculations**: Calculate angles using trigonometric functions.
- **Roll-Pitch Rotation**: Rotate roll and pitch values by a specified angle.

## Installation

To install MyArduinoLibrary, you can use the Arduino Library Manager or download the library and place it in your Arduino libraries folder.

### Using Arduino Library Manager

1. Open the Arduino IDE.
2. Go to Sketch > Include Library > Manage Libraries.
3. Search for "MyArduinoLibrary" and install it.

### Manual Installation

1. Download the library from the repository.
2. Extract the contents to your Arduino libraries folder (usually located in `Documents/Arduino/libraries`).
3. Restart the Arduino IDE.

## Usage

To use MyArduinoLibrary in your Arduino project, include the header file in your sketch:

```cpp
#include <utils.h>
```

### Example

Here is a simple example demonstrating how to use the library:

```cpp
#include <Arduino.h>
#include <utils.h>

void setup() {
    Serial.begin(9600);
    
    float mappedValue = Utils::Calcs::map_float(5.0, 0.0, 10.0, 0.0, 100.0);
    Serial.println(mappedValue);
}

void loop() {
    // Your code here
}
```

## Documentation

For detailed documentation on the functions and structures provided by this library, please refer to the header file `utils.h`.

## Contributing

Contributions are welcome! If you have suggestions or improvements, please submit a pull request or open an issue.

## License

This library is released under the MIT License. See the LICENSE file for more details.