"""In-firmware emulated LPF2 ports and devices.

Pair a :class:`port` with a :class:`device` (typically a subclass) to
present a fake device to the rest of the stack — used by
:class:`lpf2.hub_emulation` to expose Python-implemented devices to
LEGO apps, and by the port expander to synthesise sub-ports.
"""

from __future__ import annotations
from typing import Sequence
from lpf2 import port as _port
from lpf2 import device_descriptor

class port(_port):
    """LPF2 port whose transport is a Python-side :class:`device`.

    ``attachDevice`` binds a :class:`device` instance; from then on
    calls to the port's motor/sensor methods are forwarded to the
    device's overrides.
    """

    def attachDevice(self, device: device) -> None:
        """Bind ``device`` to this port. Replaces any previously attached device."""
        ...
    def detachDevice(self) -> None:
        """Detach the current device. The port then reports disconnected."""
        ...

class device:
    """Emulated LPF2 device backed by a :class:`~lpf2.device_descriptor`.

    Subclass to implement custom behaviour: override
    :meth:`startPower`, :meth:`setMode`, :meth:`writeData` etc.
    The default overrides on this class call the C++ default logging
    implementation, so it is safe to call ``super().startPower(pw)``
    from a subclass — recursion into Python is prevented on the C++
    side.

    Registers itself with the firmware's update registry at
    construction, so any C++ ``update()`` behaviour runs each tick
    automatically. Python code does not need to poll the device.
    """

    def setModeData(self, mode: int, buf: bytes) -> None:
        """Store ``buf`` as the latest raw payload for ``mode`` and fire
        the value-change callback."""
        ...
    def setWriteDataCallback(self, cb: object) -> None:
        """Install a callback ``cb(mode, data, userData)`` that intercepts
        writes made via :meth:`writeData` before the default logging
        implementation runs."""
        ...
    def startPower(self, pw: int) -> None:
        """Motor power (-100..100). Default: log the call."""
        ...
    def setAccTime(self, time: int, profile: int) -> None:
        """Set acceleration time/profile. Default: log the call."""
        ...
    def setDecTime(self, time: int, profile: int) -> None:
        """Set deceleration time/profile. Default: log the call."""
        ...
    def startSpeed(self, speed: int = 100, maxPower: int = 100, useProfile: int = 0) -> None:
        """Run at ``speed`` for as long as commanded. Default: log the call."""
        ...
    def startSpeedForTime(self, time: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Run at ``speed`` for ``time`` ms. Default: log the call."""
        ...
    def startSpeedForDegrees(self, degrees: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Rotate by ``degrees``. Default: log the call."""
        ...
    def gotoAbsPosition(self, absPos: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move to absolute encoder position. Default: log the call."""
        ...
    def presetEncoder(self, pos: int) -> None:
        """Reset the encoder to ``pos``. Default: log the call."""
        ...
    def writeData(self, mode: int, buf: bytes) -> int:
        """Handle a raw write on ``mode``. Default: dispatch to the
        write callback installed via :meth:`setWriteDataCallback`, else
        log the payload. Return 0 on success."""
        ...
    def setMode(self, mode: int, delta: float = 1.0) -> int:
        """Select the active input mode. Default: log the call. Returns 0."""
        ...
    def setModeCombo(self, idx: int, deltas: Sequence[float] = ()) -> int:
        """Select a mode combination. Default: log the call. Returns 0."""
        ...
    def __init__(self, descriptor: device_descriptor) -> None:
        """Build a device from a :class:`lpf2.device_descriptor`.

        The descriptor defines the modes, versions and I/O masks the
        emulated device advertises.
        """
        ...
