"""Hub alert types (see LWP 3.5.4)."""

LOW_BATTERY: int = 0x01
"""Battery voltage below the configured minimum."""
HIGH_CURRENT: int = 0x02
"""Motor / port current above safe limit."""
LOW_SIGNAL_STRENGTH: int = 0x03
"""BLE RSSI below acceptable threshold."""
OVER_POWER_CONITION: int = 0x04
"""Sustained over-power condition on one or more ports."""
