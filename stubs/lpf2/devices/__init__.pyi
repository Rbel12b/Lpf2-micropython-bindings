"""Typed wrappers for LPF2 devices attached to a :class:`lpf2.port`.

Each class corresponds to a C++ ``Lpf2::Devices::*Control`` interface
and is returned by :meth:`lpf2.port.device` once the port's factory has
identified the attached device.
"""

from __future__ import annotations
from lpf2 import port as _port

def registerDefault() -> None:
    """Register the built-in device factories with the global registry.

    Called automatically at module import; expose so custom builds can
    re-register after a soft reset.
    """
    ...

class basic_motor:
    """Simple DC motor with no encoder (train motor, PF-M motor)."""

    def startPower(self, pw: int) -> None:
        """Set motor power (-100..100). Negative = CCW, positive = CW.

        Special values: ``LPF2_POWER_FLOAT`` = float, ``LPF2_POWER_BRAKE`` = brake.
        """
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type (see :mod:`lpf2.device_type`)."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"DC Motor (dumb)"``)."""
        ...

class encoder_motor:
    """Motor with rotary encoder + position/speed control loops."""

    def startPower(self, pw: int) -> None:
        """Set motor power (-100..100). Same semantics as :meth:`basic_motor.startPower`."""
        ...
    def setAccTime(self, time: int, profile: int) -> None:
        """Set acceleration ramp time (ms) and profile index."""
        ...
    def setDecTime(self, time: int, profile: int) -> None:
        """Set deceleration ramp time (ms) and profile index."""
        ...
    def startSpeed(self, speed: int = 100, maxPower: int = 100, useProfile: int = 0) -> None:
        """Run continuously at ``speed`` (-100..100), power capped at ``maxPower``."""
        ...
    def startSpeedForTime(self, time: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Run at ``speed`` for ``time`` ms then apply ``endState`` (BrakingStyle)."""
        ...
    def startSpeedForDegrees(self, degrees: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Rotate by ``degrees`` (positive; direction from ``speed`` sign) then apply ``endState``."""
        ...
    def gotoAbsPosition(self, absPos: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move to absolute encoder position ``absPos`` at ``speed`` then apply ``endState``."""
        ...
    def presetEncoder(self, pos: int) -> None:
        """Reset the encoder counter to ``pos`` (also stops the motor)."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Motor with Encoder"``)."""
        ...

class color_sensor:
    """Technic Color Sensor.

    Modes 0/1/5/6 are polled through an internal combo so multiple
    values (colour, reflectivity, RGB, HSV) can be read without
    repeated mode switches.
    """
    MODE_COLOR: int = 0
    """Detected colour index (:mod:`lpf2.color`)."""
    MODE_REFLT: int = 1
    """Reflected-light intensity (0..100 PCT)."""
    MODE_AMBI: int = 2
    """Ambient-light intensity (0..100 PCT)."""
    MODE_LIGHT: int = 3
    """On-board LED output control (write-only)."""
    MODE_RGB: int = 5
    """Raw RGB channels + intensity."""
    MODE_HSV: int = 6
    """HSV reading (H 0..360, S 0..100, V 0..360)."""

    def getColorIdx(self) -> int:
        """Detected colour as :mod:`lpf2.color` index."""
        ...
    def getReflectivity(self) -> float:
        """Reflected-light percentage (0..100)."""
        ...
    def getRGB(self) -> tuple[int, int, int, int]:
        """``(r, g, b, intensity)`` raw channels."""
        ...
    def getHSV(self) -> tuple[int, int, int]:
        """``(h, s, v)`` — H 0..360, S 0..100, V 0..360."""
        ...
    def setLight(self, l1: int, l2: int, l3: int) -> None:
        """Drive the on-board light channels (each 0..100 PCT)."""
        ...
    def setMode(self, modeNum: int, delta: float = 1.0) -> None:
        """Switch active mode. ``delta`` = change threshold for callbacks
        (0 = every update)."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Technic Color Sensor"``)."""
        ...

class distance_sensor:
    """Technic Distance Sensor (time-of-flight)."""

    MODE_LIGHT: int = 5
    """On-board light output control (write-only)."""

    def setLight(self, l1: int, l2: int, l3: int, l4: int) -> None:
        """Drive the four on-board light segments (each 0..100 PCT)."""
        ...
    def getDistance(self) -> float:
        """Distance in centimetres."""
        ...
    def setMode(self, modeNum: int, delta: float = 1.0) -> None:
        """Switch active mode; ``delta`` = change threshold for callbacks."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Technic Distance Sensor"``)."""
        ...

class color_distance_sensor:
    """Boost Color & Distance Sensor (legacy, WeDo/Boost)."""

    MODE_COLOR: int = 0
    """Detected colour index."""
    MODE_DIST: int = 1
    """Measured distance (cm)."""
    MODE_REFLT: int = 3
    """Reflected-light percentage."""
    MODE_AMBI: int = 4
    """Ambient-light percentage."""
    MODE_LED: int = 5
    """On-board LED colour control (write-only)."""
    MODE_RGB: int = 6
    """Raw RGB channels."""
    MODE_IR: int = 7
    """LEGO PF IR transmitter (write-only)."""

    def getColorIdx(self) -> int:
        """Detected colour as :mod:`lpf2.color` index."""
        ...
    def getDistance(self) -> float:
        """Distance in centimetres."""
        ...
    def getReflectedLight(self) -> int:
        """Reflected-light percentage (0..100)."""
        ...
    def getAmbientLight(self) -> int:
        """Ambient-light percentage (0..100)."""
        ...
    def getRgb(self) -> tuple[int, int, int]:
        """Raw RGB channels."""
        ...
    def setIrTx(self, value: int) -> None:
        """Emit a value on the LEGO Power Functions RC IR channel."""
        ...
    def setLedColor(self, color: int) -> None:
        """Set the on-board LED colour (:mod:`lpf2.color` index)."""
        ...
    def setMode(self, modeNum: int, delta: float = 1.0) -> None:
        """Switch active mode; ``delta`` = change threshold for callbacks."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Color Distance Sensor"``)."""
        ...

class hub_led:
    """RGB LED attached to a hub's built-in LED port."""

    def setColorIdx(self, color: int) -> None:
        """Set colour by :mod:`lpf2.color` index."""
        ...
    def setColor(self, r: int, g: int, b: int) -> None:
        """Set colour by explicit RGB (each 0..255)."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Hub LED"``)."""
        ...

class accelerometer:
    """Built-in hub accelerometer (mode 0 = GRV, unit: mG)."""

    def getX(self) -> float:
        """X-axis acceleration in mG."""
        ...
    def getY(self) -> float:
        """Y-axis acceleration in mG."""
        ...
    def getZ(self) -> float:
        """Z-axis acceleration in mG."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Hub Accelerometer"``)."""
        ...

class gyroscope:
    """Built-in hub gyroscope (mode 0 = ROT, unit: dps)."""

    def getX(self) -> float:
        """X-axis angular velocity in dps."""
        ...
    def getY(self) -> float:
        """Y-axis angular velocity in dps."""
        ...
    def getZ(self) -> float:
        """Z-axis angular velocity in dps."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Hub Gyroscope"``)."""
        ...

class port_expander:
    """LPF2 Port Expander (extension module giving four sub-ports on one port)."""

    def getPort(self, port_num: int) -> _port:
        """Get the sub-port ``port_num`` (0..3, see
        :mod:`lpf2.port_expander.port_num`). Returns the sub-port as a
        :class:`lpf2.port`."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type."""
        ...
    def name(self) -> str:
        """Human-readable device name (``"Port Expander"``)."""
        ...
