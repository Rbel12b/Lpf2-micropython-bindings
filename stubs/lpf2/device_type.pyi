"""LPF2 device-type identifiers (LWP ``IOTypeID``).

Values returned by :meth:`lpf2.port.getDeviceType` and matched against
by the device factories.
"""

UNKNOWNDEVICE: int = 0
"""No device / unrecognised type."""
SIMPLE_MEDIUM_LINEAR_MOTOR: int = 1
"""Simple medium linear motor (WeDo, Boost)."""
TRAIN_MOTOR: int = 2
"""Powered-Up train motor."""
LIGHT: int = 8
"""Powered-Up light."""
VOLTAGE_SENSOR: int = 20
"""Hub voltage sensor."""
CURRENT_SENSOR: int = 21
"""Hub current sensor."""
PIEZO_BUZZER: int = 22
"""Piezo buzzer."""
HUB_LED: int = 23
"""Hub built-in RGB LED."""
TILT_SENSOR: int = 34
"""WeDo tilt sensor."""
MOTION_SENSOR: int = 35
"""WeDo motion sensor."""
COLOR_DISTANCE_SENSOR: int = 37
"""Boost color & distance sensor."""
MEDIUM_LINEAR_MOTOR: int = 38
"""Boost medium linear motor."""
MOVE_HUB_MEDIUM_LINEAR_MOTOR: int = 39
"""Boost Move Hub built-in medium linear motor."""
MOVE_HUB_TILT_SENSOR: int = 40
"""Boost Move Hub built-in tilt sensor."""
DUPLO_TRAIN_BASE_MOTOR: int = 41
"""Duplo train base motor."""
DUPLO_TRAIN_BASE_SPEAKER: int = 42
"""Duplo train base speaker."""
DUPLO_TRAIN_BASE_COLOR_SENSOR: int = 43
"""Duplo train base colour sensor."""
DUPLO_TRAIN_BASE_SPEEDOMETER: int = 44
"""Duplo train base speedometer."""
TECHNIC_LARGE_LINEAR_MOTOR: int = 46
"""Technic Control+ large linear motor."""
TECHNIC_XLARGE_LINEAR_MOTOR: int = 47
"""Technic Control+ XL linear motor."""
TECHNIC_MEDIUM_ANGULAR_MOTOR: int = 48
"""Technic Control+ medium angular motor."""
TECHNIC_LARGE_ANGULAR_MOTOR: int = 49
"""Technic Control+ large angular motor."""
TECHNIC_MEDIUM_HUB_GEST_SENSOR: int = 54
"""Technic medium hub built-in gesture sensor."""
REMOTE_CONTROL_BUTTON: int = 55
"""Powered-Up remote control button pair."""
REMOTE_CONTROL_RSSI: int = 56
"""Powered-Up remote control RSSI sensor."""
TECHNIC_MEDIUM_HUB_ACCELEROMETER: int = 57
"""Technic medium hub built-in accelerometer."""
TECHNIC_MEDIUM_HUB_GYRO_SENSOR: int = 58
"""Technic medium hub built-in gyroscope."""
TECHNIC_MEDIUM_HUB_TILT_SENSOR: int = 59
"""Technic medium hub built-in tilt sensor."""
TECHNIC_MEDIUM_HUB_TEMPERATURE_SENSOR: int = 60
"""Technic medium hub built-in temperature sensor."""
TECHNIC_COLOR_SENSOR: int = 61
"""Technic colour sensor."""
TECHNIC_DISTANCE_SENSOR: int = 62
"""Technic distance sensor."""
TECHNIC_FORCE_SENSOR: int = 63
"""Technic force sensor."""
MARIO_HUB_GESTURE_SENSOR: int = 71
"""Mario hub gesture sensor."""
MARIO_HUB_BARCODE_SENSOR: int = 73
"""Mario hub barcode sensor."""
MARIO_HUB_PANT_SENSOR: int = 74
"""Mario hub pants sensor."""
TECHNIC_MEDIUM_ANGULAR_MOTOR_GREY: int = 75
"""Technic grey medium angular motor."""
TECHNIC_LARGE_ANGULAR_MOTOR_GREY: int = 76
"""Technic grey large angular motor."""
EV3_MEDIUM_MOTOR: int = 100
"""Mindstorms EV3 medium motor. No official LPF2 ID; assigned locally
for use with :meth:`lpf2.local.port.forceDeviceType` (drives raw PWM)."""
EV3_LARGE_MOTOR: int = 101
"""Mindstorms EV3 large motor. No official LPF2 ID; assigned locally
for use with :meth:`lpf2.local.port.forceDeviceType` (drives raw PWM)."""
