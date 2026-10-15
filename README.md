# SCORP Robot

Arduino Uno R3 firmware for a brush cleaning and abrasion test system. Built using the Arduino framework and PlatformIO.

## Features

- Two vertical axis motor outputs with top and bottom limit switches. (But only use One motor)
- Brush motor and DC pump control.
- Motor speed ramping.
- Automatic axis reversal.
- Cycle counting with EEPROM storage.
- 20×4 I2C LCD status display.
- Latched emergency stop.
- Axis movement timeout and watchdog timer.

## Operation

1. Press **Start** to start the brush, pump, and vertical axis.
2. The axis moves between the top and bottom limits.
3. At each limit, the axis stops and reverses after the configured delay.
4. An upward travel followed by arrival at the bottom counts as one cycle.
5. Press **Stop** to ramp all motors down.
6. Press **Start** again to resume in the remembered axis direction.

The brush and pump remain running during normal axis reversal delays.

## Emergency Stop

The E-stop hardware cuts 12 V to the motor drivers. A separate normally open contact connects Arduino **D12 to GND** to signal the firmware.

When activated, the firmware immediately zeros motor PWM outputs and displays an E-stop message. Releasing the button does not resume operation.

To recover, release E-stop and reset the Arduino. If E-stop is pressed at startup, the system remains latched until it is released and the Arduino is reset.

Do not connect 12 V directly to an Arduino input.

## Hardware

- Arduino Uno R3
- Motor drivers MDD20A From Cytron
- One vertical axis motors, Cytron IG42-04K
- Brush motor, Cytron IG42-04K
- DC pum, Seaflo 21 Series DC Diaphragm pump
- Top and bottom limit switches, Kozuka KZ-8111
- Start and Stop buttons
- E-stop with a separate auxiliary contact
- 20×4 I2C LCD, address `0x27`

See `src/config/PinMap.h` for pin assignments.


## Build and Upload

Install VS Code and the PlatformIO extension, then open this repository.

Build:

```bash
pio run
```

Upload:

```bash
pio run -t upload
```

Open the Serial Monitor:

```bash
pio device monitor -b 115200
```

PlatformIO installs the LCD library declared in `platformio.ini`.

## Code Organization

| Location | Purpose |
|---|---|
| `src/main.cpp` | Arduino entry point |
| `src/App.*` | Initializes and updates modules |
| `src/StateMachine.*` | Coordinates machine operation |
| `src/IO.*` | Reads buttons and switches |
| `src/Motor/` | Axis, brush, and pump control |
| `src/Display/` | LCD display |
| `src/CycleStorage.*` | EEPROM cycle storage |
| `src/config/PinMap.h` | Pin assignments |
| `src/config/Config.h` | Speeds, ramp rates, and timing |
| `src/config/Debug.h` | Debug output settings |

## Configuration

Adjust motor speeds, ramp rates, reversal delay, movement timeout, and cycle save interval in `src/config/Config.h`.

Motor speed commands use values from **0 to 255**. These are PWM duty commands, not measured motor speeds.

Completed cycles are saved periodically and when stopping or entering E-stop. Sudden power loss may lose cycles completed since the last save.