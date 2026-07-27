"""LEGO Powered-Up colour indices (LWP ``ColorIDX``).

Used by :meth:`lpf2.port.setRgbColorIdx`, :class:`lpf2.devices.hub_led`
and the colour-sensor family.
"""

BLACK: int = 0
"""Black / off."""
PINK: int = 1
"""Pink."""
PURPLE: int = 2
"""Purple."""
BLUE: int = 3
"""Blue."""
LIGHTBLUE: int = 4
"""Light blue."""
CYAN: int = 5
"""Cyan."""
GREEN: int = 6
"""Green."""
YELLOW: int = 7
"""Yellow."""
ORANGE: int = 8
"""Orange."""
RED: int = 9
"""Red."""
WHITE: int = 10
"""White."""
NUM_COLORS: int = 11
"""Number of concrete colour entries (BLACK..WHITE)."""
NONE: int = 255
"""Sentinel: no colour / unknown."""
