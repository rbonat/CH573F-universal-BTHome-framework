# Task

Analyze the current repository and convert it into a reusable framework repository called:

`ch57x-bthome-framework`

This is a repository restructuring task, NOT a firmware redesign task.

## Critical requirements

* Preserve all existing functionality.
* Preserve BLE behavior.
* Preserve BTHome payload generation.
* Preserve low-power and sleep behavior.
* Preserve sensor measurement algorithms.
* Do not change application logic unless required by restructuring.
* The resulting project must build after cloning and importing into MounRiver Studio.
* Use only relative paths.
* Keep CH573 compatibility.
* Prepare the structure for future CH582 support.

Before modifying files:

1. Analyze the existing repository.
2. Produce a migration plan.
3. Wait for approval.

---

# Target repository structure

```text
ch57x-bthome-framework/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── docs/
│   ├── architecture.md
│   ├── porting.md
│   └── examples.md
│
├── framework/
│   │
│   ├── core/
│   │   ├── include/
│   │   └── *.c
│   │
│   ├── ble/
│   │   ├── include/
│   │   └── *.c
│   │
│   ├── hal/
│   │   ├── include/
│   │   └── *.c
│   │
│   ├── sensors/
│   │   ├── include/
│   │   └── *.c
│   │
│   └── config/
│       └── default_config.h
│
├── vendor/
│   └── ch57x_sdk/
│       ├── StdPeriphDriver/
│       ├── Startup/
│       ├── RVMSIS/
│       ├── LIB/
│       └── Ld/
│
├── examples/
│   │
│   ├── soil_sensor/
│   │   ├── .project
│   │   ├── .cproject
│   │   ├── *.wvproj
│   │   ├── app/
│   │   ├── config/
│   │   └── README.md
│   │
│   ├── temp_hygro/
│   │   └── placeholder.md
│   │
│   └── scene_switch/
│       └── placeholder.md
│
├── templates/
│   └── new_sensor/
│
└── tools/
```

---

# Architecture rules

## Framework

Move reusable code into:

* framework/core
* framework/ble
* framework/hal
* framework/sensors

Examples:

### framework/core

* BTHome encoder
* advertisement payload builders
* battery helpers
* scheduler/helpers
* reusable BTHome logic

### framework/hal

Move reusable hardware abstraction:

* RTC
* sleep
* GPIO helpers
* LED helpers
* MCU utilities
* key/button helpers

### framework/sensors

Create reusable sensor drivers.

Move soil sensor measurement code into a reusable sensor module if practical.

Examples:

* soil_sensor.c
* soil_sensor.h

Future sensors should be added here.

### framework/ble

Move reusable BLE advertising and helper code.

---

## Vendor SDK

Move vendor-provided code unchanged into:

```text
vendor/ch57x_sdk/
```

Including:

* StdPeriphDriver
* RVMSIS
* Startup
* LIB
* Ld

Do not modify vendor code except include-path fixes.

---

## Example applications

The current soil sensor becomes:

```text
examples/soil_sensor/
```

Move application-specific code there.

The example should contain only:

* application logic
* GPIO assignments
* sensor configuration
* project files

Framework code must be shared.

---

# Configuration model

Do NOT introduce a board abstraction layer.

The framework targets a universal CH57x board.

GPIO configuration should be application-specific.

Create:

```text
examples/soil_sensor/config/app_config.h
```

Examples:

```c
#define SOIL_GPIO_PIN ...
#define BATTERY_ADC_CHANNEL ...
#define ADV_INTERVAL_MS ...
```

Framework modules should consume configuration from the example project.

---

# MounRiver Studio requirements

Each example must be a standalone MounRiver Studio project.

For example:

```text
examples/soil_sensor/
```

must contain:

* .project
* .cproject
* .wvproj

Requirements:

* imports directly into MounRiver Studio
* builds after clone
* uses only relative paths
* no workspace-specific paths
* no machine-specific paths

Verify:

* include paths
* linker paths
* library paths

---

# Documentation generation

Generate:

## README.md

Include:

* project overview
* features
* supported MCUs
* repository structure
* build instructions
* Home Assistant compatibility
* power-saving design
* roadmap

## docs/architecture.md

Describe:

* framework layer
* example layer
* vendor SDK layer

## docs/porting.md

Describe how to create a new sensor project.

## examples/soil_sensor/README.md

Describe:

* hardware
* GPIO configuration
* BTHome entities
* Home Assistant integration

---

# Template

Create:

```text
templates/new_sensor/
```

containing a minimal skeleton for new BTHome sensor projects.

---

# Git

Generate a proper .gitignore.

Ignore:

```text
obj/
bin/
Debug/
Release/
.mrs/
.settings/
*.log
*.tmp
```

Keep:

```text
.project
.cproject
*.wvproj
```

under version control.

---

# Validation

Before completing the migration:

1. Verify all include paths.
2. Verify all relative paths.
3. Verify all project references.
4. Verify the soil_sensor example builds.
5. Report all moved files.
6. Report all include-path changes.
7. Report all project configuration changes.

Do not leave TODO placeholders except for temp_hygro and scene_switch examples.
