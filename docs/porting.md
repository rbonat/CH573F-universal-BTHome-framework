# Porting a New Sensor

Create a new example by copying the structure from `templates/new_sensor` or `examples/soil_sensor`.

## 1. Create an Example Project

Place the new project under `examples/<sensor_name>/` with:

- `.project`
- `.cproject`
- `*.wvproj`
- `app/`
- `config/`
- `README.md`

Use relative linked resources for the repository-level `framework/` and `vendor/` folders.

## 2. Define Application Configuration

Put GPIO assignments, ADC channels, advertisement timing, sensor calibration, and feature flags in `config/app_config.h`.

The example should provide `config/CONFIG.h` that includes `app_config.h` before `framework/config/default_config.h`. MounRiver Studio on Windows resolves the framework's existing `config.h` includes to that same file.

## 3. Add or Reuse a Sensor Driver

Reusable sensor measurement code belongs in `framework/drivers/<driver_name>`.

Application code should call driver functions and keep only orchestration logic: scheduling, deciding what to publish, and building the BTHome payload.

## 4. Build the BTHome Payload

Use `framework/core/include/bthome_v2.h` to initialize a builder and add objects in ascending object-id order.

Keep payload buffers small enough for BLE legacy advertising when using `bthome_v2_build_legacy_advertisement`.

## 5. Validate the Project

Before publishing a new example:

- Verify all include paths are relative.
- Verify linker and library paths are relative.
- Check for stale machine-specific paths.
- Build the example in MounRiver Studio after a clean import.
