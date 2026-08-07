"""LEGO Powered-Up LPF2 protocol bindings.

Provides the core LPF2 primitives: ports (local, remote and virtual),
typed device wrappers, hub descriptors and a hub-emulation server.
Submodules expose the LPF2 enumerations (:mod:`~lpf2.color`,
:mod:`~lpf2.device_type`, :mod:`~lpf2.port_num` ...).
"""

from __future__ import annotations
from typing import Optional, Sequence, Union
from lpf2 import port_num as port_num
from lpf2 import hub_type as hub_type
from lpf2 import hub_property as hub_property
from lpf2 import device_type as device_type
from lpf2 import alerts as alerts
from lpf2 import battery_type as battery_type
from lpf2 import button_state as button_state
from lpf2 import color as color
from lpf2 import local as local
from lpf2 import virtual as virtual
from lpf2 import devices as devices
from lpf2 import port_expander as port_expander
from typing import Callable

class battery:
    """Battery voltage tracking + optional ADC divider reader.

    All methods are static. Defaults: max 9000 mV, min 6000 mV, current
    voltage equal to max. Percent = linear interpolation between min
    and max with a cutoff at min.
    """

    @staticmethod
    def setMaxVoltage(mv: int) -> None:
        """Set the full-charge voltage in mV."""
        ...
    @staticmethod
    def setMinVoltage(mv: int) -> None:
        """Set the cut-off (0%) voltage in mV."""
        ...
    @staticmethod
    def getMaxVoltage() -> int:
        """Get the full-charge voltage in mV."""
        ...
    @staticmethod
    def getMinVoltage() -> int:
        """Get the cut-off (0%) voltage in mV."""
        ...
    @staticmethod
    def setCurrentVoltage(mv: int) -> None:
        """Manually update the current battery voltage (mV).

        Use this if you have your own voltage source (not the built-in
        ADC divider).
        """
        ...
    @staticmethod
    def getCurrentVoltage() -> int:
        """Get the last-known current battery voltage in mV."""
        ...
    @staticmethod
    def getPercent() -> int:
        """Compute percentage from current voltage using the mapping
        installed via :meth:`setPercentFunc` (default: linear)."""
        ...
    @staticmethod
    def setPercentFunc(fn: Optional[Callable[[int, int, int], int]]) -> None:
        """Install a custom voltage->percent mapping ``fn(mV, vmin, vmax)``.

        Pass ``None`` to restore the default linear mapping.
        """
        ...
    @staticmethod
    def setupAdcDivider(
        adc_channel: int,
        adc_unit: int,
        r_top_ohms: float,
        r_bottom_ohms: float,
        vref_mv: int = 3300,
        samples: int = 8,
    ) -> bool:
        """Configure the ESP-IDF ADC + calibration for a resistive divider.

        ``adc_channel``/``adc_unit`` are ESP-IDF ``adc_channel_t`` /
        ``adc_unit_t`` values. ``r_top_ohms``/``r_bottom_ohms`` describe
        the divider (battery -> tap -> GND). Returns True on success.
        """
        ...
    @staticmethod
    def readBatteryVoltage() -> int:
        """Read the divider, average ``samples`` shots, store via
        :meth:`setCurrentVoltage`. Returns the reading in mV, or 0 if
        the ADC is not configured."""
        ...

class _motor_settings:
    """Motor tuning parameters (PID gains, deadbands, kinetic limits).

    One instance per motor type; consumed by the encoder-motor
    controller. Fields mirror the C++ ``Lpf2::MotorSettings`` layout.
    """
    id: int
    """Motor type identifier."""
    rated_max_speed: int
    """Rated max speed in device units."""
    max_voltage_mv: int
    """Voltage cap fed to the H-bridge."""
    speed_ksp: float
    """Speed-loop proportional gain."""
    speed_ksi: float
    """Speed-loop integral gain."""
    speed_int_clamp: float
    """Anti-windup clamp on the speed-loop integrator."""
    speed_deadband_pct: float
    """Deadband (%) below which speed error is treated as zero."""
    pos_kp: float
    """Position-loop proportional gain."""
    pos_ki: float
    """Position-loop integral gain."""
    pos_kd: float
    """Position-loop derivative gain."""
    pos_int_clamp: float
    """Anti-windup clamp on the position-loop integrator."""
    pos_deadband_deg: float
    """Deadband (deg) below which position error is treated as zero."""
    pos_decel_mdps2: float
    """Position-loop deceleration limit in mdeg/s^2."""
    breakaway_pct: float
    """Extra power (%) added on startup to overcome static friction."""
    kinetic_floor_pct: float
    """Minimum power (%) held while the motor is moving."""

class motor_setting:
    """Preset :class:`_motor_settings` instances for known LPF2 motors."""
    medium_linear: _motor_settings
    """Medium linear motor."""
    technic_large_linear: _motor_settings
    """Technic Large linear motor."""
    technic_xlarge_linear: _motor_settings
    """Technic XL linear motor."""
    technic_medium_angular: _motor_settings
    """Technic Medium Angular motor."""
    technic_large_angular: _motor_settings
    """Technic Large Angular motor."""
    technic_medium_angular_grey: _motor_settings
    """Technic Medium Angular motor (grey Control+ variant)."""
    technic_large_angular_grey: _motor_settings
    """Technic Large Angular motor (grey Control+ variant)."""

class mode:
    """Description of one LPF2 device mode (parsed from Port Mode Info).

    Combines the mode's name, value range (raw/PCT/SI), I/O mapping
    flags, dataset layout and the latest raw payload received on the
    mode.
    """
    name: str
    """Mode name (from the device)."""
    min: float
    """Raw value minimum."""
    max: float
    """Raw value maximum."""
    PCTmin: float
    """Percent-scale minimum."""
    PCTmax: float
    """Percent-scale maximum."""
    SImin: float
    """SI-scale minimum."""
    SImax: float
    """SI-scale maximum."""
    inMapping: int
    """Input-mapping bitmask (see LWP spec)."""
    outMapping: int
    """Output-mapping bitmask (see LWP spec)."""
    dataSets: int
    """Number of datasets per sample."""
    format: int
    """Sample element format (0=int8, 1=int16, 2=int32, 3=float)."""
    figures: int
    """Total display figures (integer + decimals)."""
    decimals: int
    """Decimals shown for display."""
    motorBias: int
    """Motor bias, per LWP."""
    flags: int
    """Capability/flags bitmask."""
    rawData: bytes
    """Latest raw payload for this mode."""

class version:
    """LEGO Wireless Protocol version tuple: Build/Major/Minor/Bugfix."""
    Build: int
    """Build number."""
    Major: int
    """Major version."""
    Minor: int
    """Minor version."""
    Bugfix: int
    """Bugfix version."""

class port:
    """Base LPF2 port.

    Base class for :class:`lpf2.local.port`, :class:`lpf2.virtual.port`
    and remote ports. Wraps the C++ ``Lpf2::Port``. Every port
    registers itself with the firmware's update registry at
    construction, so :meth:`update` is polled automatically each
    tick — callers just read state via :meth:`isDeviceConnected` /
    :meth:`device` / :meth:`getValue`.
    """

    def init(self) -> None:
        """Optional one-shot setup hook. Default: no-op."""
        ...
    def update(self) -> None:
        """Poll the port: run one tick of the underlying transport, then
        resolve the attached device via the device factory.

        The C firmware calls this every tick for every port, so user
        code does not need to call it. Calling it manually just runs
        an extra poll early. No-op while the port is disabled (see
        :meth:`disable`)."""
        ...
    def disable(self, disable: bool = True) -> None:
        """Enable/disable port polling.

        When disabled, :meth:`update` returns immediately without polling
        the transport or forwarding to the attached device. Use to pause
        a port temporarily (e.g. while another task drives the same
        hardware) without tearing down the device wrapper. Call
        ``disable(False)`` to resume.

        Subclasses may release/reacquire transport resources on the
        transition via the C++ ``_onDisable`` hook (e.g. deinit UART).
        """
        ...
    def isDisabled(self) -> bool:
        """True if the port is currently disabled."""
        ...
    def device(self) -> Union[
        devices.basic_motor,
        devices.encoder_motor,
        devices.color_sensor,
        devices.distance_sensor,
        devices.port_expander,
        devices.hub_led,
        devices.accelerometer,
        devices.gyroscope,
        None,
    ]:
        """Return the typed device wrapper for whatever is currently
        attached, or ``None`` if nothing is connected."""
        ...
    def writeData(self, mode: int, buf: bytes) -> int:
        """Send a raw payload to the device on the given mode. Returns
        an error code (0 = ok)."""
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
        """Run the motor at ``speed`` (-100..100). ``maxPower`` caps
        commanded power (0..100). ``useProfile`` selects the acc/dec
        profiles (low nibble = acc, next nibble = dec)."""
        ...
    def startSpeedForTime(self, time: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Run the motor at ``speed`` for ``time`` ms then apply
        ``endState`` (see :mod:`lpf2.button_state` / BrakingStyle)."""
        ...
    def startSpeedForDegrees(self, degrees: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move the motor by ``degrees`` (positive; use ``speed`` sign
        for direction) then apply ``endState``."""
        ...
    def gotoAbsPosition(self, absPos: int, speed: int = 100, maxPower: int = 100, endState: int = 0, useProfile: int = 0) -> None:
        """Move the motor to absolute encoder position ``absPos`` at
        ``speed`` (0..100) then apply ``endState``."""
        ...
    def presetEncoder(self, pos: int) -> None:
        """Reset the motor encoder to ``pos``. Also stops the motor."""
        ...
    def setRgbColorIdx(self, idx: int) -> None:
        """Set an RGB LED by :mod:`lpf2.color` index (uses mode 0)."""
        ...
    def setRgbColor(self, r: int, g: int, b: int) -> None:
        """Set an RGB LED by explicit RGB (0..255, uses mode 1)."""
        ...
    def setMode(self, mode: int, delta: float = 1.0) -> int:
        """Select the device's active input mode.

        ``delta`` is the minimum change (in raw units) that triggers a
        value-change callback. 0 = every update. Returns 0 on success.
        """
        ...
    def setModeCombo(self, idx: int, deltas: Sequence[float] = ()) -> int:
        """Activate one of the device's mode combinations.

        ``deltas`` are per-mode change thresholds in the order defined
        by the combo bitmask; empty = default of 1. Returns 0 on
        success.
        """
        ...
    def isDeviceConnected(self) -> bool:
        """True if a device has been detected on this port."""
        ...
    def getValue(self, mode: int, dataSet: int) -> float:
        """Parsed value of dataset ``dataSet`` of ``mode``. Format
        (int8/16/32/float) is taken from the mode descriptor."""
        ...
    def getValueStr(self, mode: int) -> str:
        """Human-readable value of ``mode`` formatted per the mode's
        figures/decimals."""
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
        """Capability bitmask (bit 0x04 = combinable, if the device has combos)."""
        ...
    def getInfoStr(self) -> str:
        """Diagnostic dump of port state (device type, modes, versions)."""
        ...
    def speedToRaw(self, speed: int) -> int:
        """Convert a signed -100..100 speed to the LPF2 raw byte (0..255)."""
        ...
    def rawToSpeed(self, raw: int) -> int:
        """Convert an LPF2 raw byte to a signed -100..100 speed."""
        ...

class device_descriptor:
    """Static descriptor for a device (modes + firmware/hardware versions).

    Used to build virtual devices without a live LPF2 handshake.
    """
    modes: list[mode]
    """List of mode descriptors, indexed by mode number."""
    fwVersion: version
    """Firmware version reported by the device."""
    hwVersion: version
    """Hardware version reported by the device."""

class hub:
    """BLE client for a remote LEGO PoweredUp / Control+ hub.

    Manages the NimBLE scan/connect state, mirrors the remote hub's
    ports as :class:`port` objects, and exposes hub properties.
    Registers itself with the firmware's update registry at
    construction, so :meth:`update` runs each tick automatically —
    scripts just wait on :meth:`isConnected` / :meth:`infoReady`.
    """

    def __init__(self) -> None: ...
    def init(self, addr_or_scan: Union[str, int, None] = None, scan_duration: int = 10) -> None:
        """Prepare BLE stack and set connect target.

        ``addr_or_scan`` may be:
        - a MAC-address string to target a specific hub,
        - an int scan duration (seconds), or
        - ``None`` to use ``scan_duration``.
        """
        ...
    def update(self) -> None:
        """Pump BLE state (scan/connect/message handling).

        Auto-called each tick from the firmware update registry, so
        user code does not need to call it. Calling it manually just
        runs an extra pump early."""
        ...
    def connectHub(self) -> bool:
        """Attempt to connect to the discovered hub. Returns True on success."""
        ...
    def isConnected(self) -> bool:
        """True while the BLE link is up."""
        ...
    def isConnecting(self) -> bool:
        """True while a connection attempt is in progress."""
        ...
    def isScanning(self) -> bool:
        """True while BLE is actively scanning."""
        ...
    def shutDownHub(self) -> None:
        """Send the LWP hub-shutdown action to the remote hub."""
        ...
    def infoReady(self) -> bool:
        """True once all initial info requests (hub props, port modes) have
        been answered or timed out."""
        ...
    def getName(self) -> str:
        """Advertised name of the connected hub."""
        ...
    def setName(self, name: str) -> None:
        """Change the hub's advertising name (persisted on the hub)."""
        ...
    def getBatteryType(self) -> int:
        """Battery type reported by the hub (see :mod:`lpf2.battery_type`)."""
        ...
    def getHubType(self) -> int:
        """Hub type reported over BLE (see :mod:`lpf2.hub_type`)."""
        ...
    def getHubAddress(self) -> str:
        """BLE MAC address of the hub."""
        ...
    def getAllInfoStr(self) -> str:
        """Formatted dump of hub properties + port descriptions."""
        ...
    def getHubPropStr(self, prop_id: int) -> str:
        """String rendering of one hub property (see :mod:`lpf2.hub_property`)."""
        ...
    def getPort(self, port_num: int) -> Optional[port]:
        """Return the remote :class:`port` at ``port_num`` (see
        :mod:`lpf2.port_num`), or ``None`` if not attached."""
        ...
    def setPortMode(self, port_num: int, mode: int, delta: int, notify: bool = True) -> int:
        """Configure input-format on one remote port. Returns 0 on success."""
        ...
    def setPortModeCombo(self, port_num: int, combo_idx: int, nibble_pairs: Sequence[int], deltas_per_mode: Sequence[int]) -> int:
        """Configure a combined-mode input-format on one remote port.

        ``nibble_pairs`` is a list of bytes with mode/dataset nibbles
        (high = mode, low = dataset); ``deltas_per_mode`` lists the
        thresholds in the same order.
        """
        ...

class hub_emulation:
    """BLE server that impersonates a LEGO hub so LEGO apps can connect.

    Advertises the LWP service, handles hub-property/port-info requests
    and forwards port I/O to attached :class:`port` objects. Configure
    it first (name, versions, battery), attach ports, then :meth:`start`.
    """

    def start(self) -> None:
        """Begin BLE advertising and start the message-handling task."""
        ...
    def stop(self) -> None:
        """Stop advertising / disconnect / delete the message task."""
        ...
    def setUseBuiltInDevices(self, use: bool) -> None:
        """If True, initialise the default built-in devices on start.
        Must be called before :meth:`start`."""
        ...
    def setBatteryLevel(self, level: int) -> None:
        """Reported battery percentage (0..100)."""
        ...
    def setBatteryType(self, type: int) -> None:
        """Reported battery type (see :mod:`lpf2.battery_type`)."""
        ...
    def setName(self, name: str) -> None:
        """Advertised hub name."""
        ...
    def setFirmwareVersion(self, v: version) -> None:
        """Firmware version reported to the app."""
        ...
    def setHardwareVersion(self, v: version) -> None:
        """Hardware version reported to the app."""
        ...
    def setButtonState(self, state: int) -> None:
        """Push a button-state update (see :mod:`lpf2.button_state`)."""
        ...
    def setAlert(self, alert: int, on: bool) -> None:
        """Assert or clear a hub alert (see :mod:`lpf2.alerts`)."""
        ...
    def getName(self) -> str:
        """Current advertised hub name."""
        ...
    def attachPort(self, port_num: int, p: port) -> None:
        """Attach a port object to a port number on the emulated hub.

        The port's lifetime must exceed the ``hub_emulation`` instance;
        this class does not take ownership.
        """
        ...
