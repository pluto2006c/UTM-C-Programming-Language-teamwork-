# UTM-C-Programming-Language-teamwork

A door-state sensing example project based on `STM32F103`. This project uses an ultrasonic rangefinder for distance measurement and controls LED indicators and alarms based on the results.

## Features

- **Ultrasonic Ranging**: High-precision distance detection based on timer capture.
- **LED Indication**: Integrated 6 user LEDs and 1 white LED for status display.
- **Key Recognition**: Implements key debouncing and multiple-press recognition.
- **Periodic Tasks**: Handles timing and status updates within the SysTick interrupt.
- **Mode Switching**: Supports switching operation modes by double-clicking the button twice per second.

## Project Structure

```text
teamwork/
├── Core/                                # Core code
│   ├── Inc/                             # Header files
│   │   ├── bsp.h                        # Board Support Package declarations
│   │   └── main.h                       # Main program header
│   └── Src/                             # Source files
│       ├── bsp.c                        # Board Support Package implementation
│       ├── main.c                       # Program entry point
│       └── stm32f1xx_it.c               # Interrupt handling
├── User_Drives/                         # User drivers
│   ├── Src/
│   │   └── user_led.c                   # LED driver implementation
│   └── user_led.h                       # LED driver interface
└── teamwork.ioc                         # CubeMX configuration file
```

## Quick Start

1. Open `teamwork.ioc` with STM32CubeMX to understand the pin configuration.
2. Check `Core/Src/main.c` for the initialization process.
3. View the core runtime logic in `Core/Src/stm32f1xx_it.c`.

## Development Environment

- STM32CubeMX
- Keil MDK / STM32CubeIDE / CMake (arm-none-eabi-gcc)
