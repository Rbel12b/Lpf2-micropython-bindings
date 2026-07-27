"""Hub / remote button-state codes (LWP ``ButtonState``).

PRESSED/RELEASED describe the hub power button; UP/DOWN/STOP are the
values reported by the PoweredUp Remote's +/-/red buttons.
"""

PRESSED: int = 0x01
"""Hub power button pressed."""
RELEASED: int = 0x00
"""Hub power button released."""
UP: int = 0x01
"""Remote button: ``+`` (up)."""
DOWN: int = 0xff
"""Remote button: ``-`` (down)."""
STOP: int = 0x7f
"""Remote button: red (stop)."""
