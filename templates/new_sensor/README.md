# New Sensor Template

Use this directory as a small reference for creating another standalone BTHome sensor example.

Recommended layout:

```text
examples/<sensor_name>/
|-- .project
|-- .cproject
|-- <sensor_name>.wvproj
|-- app/
|   `-- include/
|-- config/
|   |-- CONFIG.h
|   `-- app_config.h
`-- README.md
```

Copy the relative linked resource setup from `examples/soil_sensor/.project` so `framework/` and `vendor/` are available inside MounRiver Studio.
