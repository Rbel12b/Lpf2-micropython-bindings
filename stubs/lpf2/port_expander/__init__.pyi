"""LPF2 Port Expander support (extension device with 4 sub-ports)."""

from __future__ import annotations
from lpf2 import port as _port
from lpf2.port_expander import port_num as port_num
from lpf2.virtual import device as _virtual_device

class virtual_device(_virtual_device):
    """Emulated port-expander device (server side).

    Aggregates up to four attached :class:`lpf2.port` objects and
    forwards LPF2 traffic between them and the upstream port. Attach
    ports via :meth:`attachPort` before adding the virtual device to a
    :class:`lpf2.virtual.port`.
    """

    def __init__(self) -> None:
        """Construct an empty port-expander device (no ports attached)."""
        ...
    def attachPort(self, port_num: int, port: _port) -> None:
        """Bind ``port`` as sub-port ``port_num`` (0..3, see :mod:`~.port_num`)."""
        ...
