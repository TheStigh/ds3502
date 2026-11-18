## External Component DS3502 for ESPHOME 2025.2.1 ->


Copy the folder `ds3502` which is located in `components` to the identical folder at your instance.
Create your YAML and smile again - DS3502 is back and working in ESPHOME :)


> Example YAML:
```
# Include the local components directory
external_components:
  - source: 
      type: local
      path: components

# I2C bus configuration
i2c:
  sda: GPIO6          # Replace with your GPIO configuration
  scl: GPIO7          # Replace with your GPIO configuration
  scan: true
  frequency: 100kHz   # Lower frequency for reliability

# Create the DS3502 base component
ds3502:
  id: my_ds3502
  address: 0x28

# Create the DS3502 output
output:
  - platform: ds3502
    id: fan_output
    ds3502_id: my_ds3502

# Fan component
fan:
  - platform: speed
    output: fan_output
    name: "DS3502 Fan"
    speed_count: 100
```



> Example YAML to use a non-optimistic template:
```
# Include the local components directory
external_components:
  - source: 
      type: local
      path: components

# I2C bus configuration
i2c:
  sda: GPIO6          # Replace with your GPIO configuration
  scl: GPIO7          # Replace with your GPIO configuration
  scan: true
  frequency: 100kHz   # Lower frequency for reliability

# Create the DS3502 base component
ds3502:
  id: my_ds3502
  address: 0x28

# Create the DS3502 output
output:
  - platform: ds3502
    id: ds3502_output
    ds3502_id: my_ds3502

# Create the DS3502 wiper sensor to update non-optimistic templates
sensor:
 - platform: ds3502
   id: ds3502_wiper
   ds3502_id: my_ds3502
   on_value:
    then:
      - component.update: fan_output

# Number component
number:
  - platform: template
    id: fan_output
    name: "Fan Output (0-100))"
    optimistic: false
    lambda: !lambda |-
      float read_state = id(ds3502_wiper).get_state();
      return ((read_state/127.0f)*100.0f);  //get_state() returns 0-127
    min_value: 0
    max_value: 100
    step: 1
    set_action:
      then:
        - output.set_level:
            id: ds3502_output
            level: !lambda 'return x/100;'
        - delay: 0.1s
        - component.update: ds3502_wiper
```
