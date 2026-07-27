"""Hub-property identifiers (LWP 3.5.2).

Passed to :meth:`lpf2.hub.getHubPropStr` and used by
:class:`lpf2.hub_emulation` to publish hub metadata.
"""

from __future__ import annotations

ADVERTISING_NAME: int = 0x01
"""Advertised hub name (UTF-8, up to 14 bytes)."""
BUTTON: int = 0x02
"""Power-button state."""
FW_VERSION: int = 0x03
"""Firmware version."""
HW_VERSION: int = 0x04
"""Hardware version."""
RSSI: int = 0x05
"""BLE RSSI (dBm, signed)."""
BATTERY_VOLTAGE: int = 0x06
"""Battery level percentage (0..100)."""
BATTERY_TYPE: int = 0x07
"""Battery type (see :mod:`lpf2.battery_type`)."""
MANUFACTURER_NAME: int = 0x08
"""Manufacturer name string."""
RADIO_FIRMWARE_VERSION: int = 0x09
"""Radio firmware version string."""
LEGO_WIRELESS_PROTOCOL_VERSION: int = 0x0A
"""LWP version reported by the hub."""
SYSTEM_TYPE_ID: int = 0x0B
"""System/hub type id byte."""
HW_NETWORK_ID: int = 0x0C
"""Hardware network id."""
PRIMARY_MAC_ADDRESS: int = 0x0D
"""Primary BLE MAC."""
SECONDARY_MAC_ADDRESS: int = 0x0E
"""Secondary BLE MAC (for hubs with two radios)."""
HARDWARE_NETWORK_FAMILY: int = 0x0F
"""Hardware network family byte."""
