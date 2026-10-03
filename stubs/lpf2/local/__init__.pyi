"""Ports driven directly by local hub UART (built-in hub sockets).

:class:`port` = :class:`lpf2.port` on local LPF2 UART transport.
Instances live in C++, exposed via board module (e.g. ``hub.ports.A``).
"""

from lpf2 import port as _port
from lpf2 import mode
from lpf2.virtual import device as _virtual_device
from typing import Optional, Sequence

class port(_port):
    """Local (on-hub) LPF2 port.

    Same public API as :class:`lpf2.port`; explicit subclass so
    isinstance checks distinguish local from virtual / remote ports.
    """
    def forceDeviceType(self, type: int) -> None:
        """Disable the port and force it to report device type ``type``
        (see :mod:`lpf2.device_type`).

        Used to drive devices that do not go through the LPF2 handshake
        (e.g. EV3 motors via :data:`lpf2.device_type.EV3_MEDIUM_MOTOR` /
        :data:`~lpf2.device_type.EV3_LARGE_MOTOR`). The port stops its
        UART/analog polling, but ``setPower`` / ``startPower`` still
        drive the PWM output pins directly.

        Call :meth:`enable` to resume normal detection.
        """
        ...
    def writeData(self, mode: int, buf: bytes) -> int:
        """Send raw payload to device on given mode. Returns error code (0 = ok)."""
        ...
    def startPower(self, pw: int) -> None:
        """Set motor power. ``pw`` in [-100..100]: negative = CCW, positive = CW."""
        ...
    def setAccTime(self, time: int, profile: int) -> None:
        """Set acceleration time (ms) and profile for the motor."""
        ...
    def setDecTime(self, time: int, profile: int) -> None:
        """Set deceleration time (ms) and profile for the motor."""
        ...
    def startSpeed(self, speed: int = 100, maxPower: int = 100, useProfile: int = 0) -> None:
        """Run motor at ``speed`` (-100..100). ``maxPower`` caps commanded
        power (0..100). ``useProfile`` selects acc/dec profiles (low nibble
        = acc, next nibble = dec)."""
        ...
    def startSpeedForTime(self, time: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Run motor at ``speed`` for ``time`` ms then apply ``endState``
        (BrakingStyle: 0 = float, 126 = hold, 127 = brake)."""
        ...
    def startSpeedForDegrees(self, degrees: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move motor by ``degrees`` (positive; direction from ``speed`` sign)
        then apply ``endState``."""
        ...
    def gotoAbsPosition(self, absPos: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move motor to absolute encoder position ``absPos`` at ``speed``
        (0..100) then apply ``endState``."""
        ...
    def presetEncoder(self, pos: int) -> None:
        """Reset motor encoder to ``pos``. Also stops the motor."""
        ...
    def setRgbColorIdx(self, idx: int) -> None:
        """Set RGB LED by :mod:`lpf2.color` index (uses mode 0)."""
        ...
    def setRgbColor(self, r: int, g: int, b: int) -> None:
        """Set RGB LED by explicit RGB (0..255, uses mode 1)."""
        ...
    def setMode(self, mode: int, delta: float = 1.0) -> int:
        """Select active input mode. ``delta`` = minimum raw-unit change
        that triggers value-change callback (0 = every update). Returns 0
        on success."""
        ...
    def setModeCombo(self, idx: int, deltas: Sequence[float] = ()) -> int:
        """Activate one of the device's mode combinations. ``deltas`` = per-mode
        thresholds in bitmask order; empty = default of 1. Returns 0 on success."""
        ...
    def isDeviceConnected(self) -> bool:
        """True if a device has been detected on this port."""
        ...
    def getValue(self, mode: int, dataSet: int) -> float:
        """Parsed value of dataset ``dataSet`` of ``mode``. Format
        (int8/16/32/float) taken from mode descriptor."""
        ...
    def getValueStr(self, mode: int) -> str:
        """Human-readable value of ``mode`` formatted per mode's figures/decimals."""
        ...
    def getDeviceType(self) -> int:
        """Reported LPF2 device type (see :mod:`lpf2.device_type`)."""
        ...
    def getModeCount(self) -> int:
        """Total number of modes exposed by the device."""
        ...
    def getViewCount(self) -> int:
        """Number of modes the device would show in the LEGO app."""
        ...
    def getMode(self, num: int) -> Optional[mode]:
        """:class:`mode` descriptor for mode ``num`` or ``None``."""
        ...
    def getModeComboCount(self) -> int:
        """Number of active mode-combo entries."""
        ...
    def getModeCombo(self, combo: int) -> int:
        """Mode-combo bitmask at index ``combo``. Bit N = mode N."""
        ...
    def getInputModes(self) -> int:
        """Bitmask of modes that produce values."""
        ...
    def getOutputModes(self) -> int:
        """Bitmask of modes that accept values."""
        ...
    def getCapabilities(self) -> int:
        """Capability bitmask (bit 0x04 = combinable if device has combos)."""
        ...
    def getInfoStr(self) -> str:
        """Diagnostic dump of port state (device type, modes, versions)."""
        ...
    def speedToRaw(self, speed: int) -> int:
        """Convert signed -100..100 speed to LPF2 raw byte (0..255)."""
        ...
    def rawToSpeed(self, raw: int) -> int:
        """Convert LPF2 raw byte to signed -100..100 speed."""
        ...


class emulated_port:
    """LPF2 slave port: emulates a device on a physical hub UART socket.

    Takes ownership of a hardware :class:`port` (calls ``disable`` on it)
    and drives its UART as an LPF2 slave. The connected host sees whatever
    :class:`~lpf2.virtual.device` is attached — typically a
    :class:`lpf2.port_expander.virtual_port_expander_device` to expose
    multiple sub-ports.

    The firmware update loop pumps the emulated port automatically; no
    explicit ``update()`` call is required in normal user code.
    """

    def __init__(self, port: port) -> None:
        """Bind to ``port``.

        ``port`` must be a hardware :class:`port` (e.g. ``hub.ports.F``).
        Its UART scanning is disabled immediately; call ``port.enable()``
        to restore normal operation after the emulated port is deleted.
        """
        ...

    def init(self) -> None:
        """Initialise the UART and begin the host-detection handshake.

        Call once after :meth:`attachDevice`. The port holds TX low
        (break condition) at **115200 baud** and waits indefinitely for
        the host to send ``CMD_SPEED``, then enumerates the attached device.
        In slow-enumeration mode (see :meth:`setSlowEnumeration`) the port
        uses 2400 baud and skips the wait.
        """
        ...

    def update(self) -> None:
        """Pump the protocol state machine manually.

        Not needed when the emulated port is registered with the firmware
        update loop (the default). Call only if you need fine-grained
        control over update timing.
        """
        ...

    def isHostConnected(self) -> bool:
        """True once the host has completed the info handshake and is
        sending data-phase messages."""
        ...

    def setSlowEnumeration(self, slow: bool) -> None:
        """Select slow (EV3 / 2400 baud) or fast (LPF2 / 115200 baud) enumeration.

        Must be called before :meth:`init`.  Default is ``False`` (115200,
        waits indefinitely for ``CMD_SPEED``).  Pass ``True`` to skip the
        LPF2 handshake and go directly to 2400 baud EV3 mode.
        """
        ...

    def isSlowEnumeration(self) -> bool:
        """True if slow-enumeration (2400 baud / EV3) mode is active."""
        ...

    def attachDevice(self, device: _virtual_device) -> None:
        """Attach ``device`` as the emulated device on this port.

        Accepts :class:`lpf2.virtual.device` or
        :class:`lpf2.port_expander.virtual_port_expander_device`.
        The device is removed from the firmware's auto-update registry
        while attached; :meth:`update` drives it instead.
        """
        ...

    def detachDevice(self) -> None:
        """Detach the current device and return it to the auto-update registry."""
        ...
