"""Per-hub-model port-number tables.

Each submodule maps a hub model's external ports (A/B/C/D/...) and
internal virtual ports (LED, ACCELEROMETER, VOLTAGE, ...) to the raw
LPF2 port numbers they advertise.
"""

from lpf2.port_num import controlplus as controlplus
from lpf2.port_num import duplotrain as duplotrain
from lpf2.port_num import movehub as movehub
from lpf2.port_num import poweredup as poweredup
from lpf2.port_num import poweredupremote as poweredupremote
from lpf2.port_num import mario as mario
