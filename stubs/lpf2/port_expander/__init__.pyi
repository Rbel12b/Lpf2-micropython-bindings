from __future__ import annotations
from lpf2 import port as _port
from lpf2.port_expander import port_num as port_num
from lpf2.virtual import device as _virtual_device

class virtual_device(_virtual_device):
    def __init__(self) -> None: ...
    def attachPort(self, port_num: int, port: _port) -> None: ...
