#include "mod_types.h"

extern "C" {

/* --- lpf2.local --- */
LPF2_DEFINE_MOD_WITH_GLOB(local,
    { MP_ROM_QSTR(MP_QSTR_port), MP_ROM_PTR(&lpf2_local_port_type) }
);

/* --- lpf2.virtual --- */
LPF2_DEFINE_MOD_WITH_GLOB(virtual,
    { MP_ROM_QSTR(MP_QSTR_port),   MP_ROM_PTR(&lpf2_virtual_port_type) },
    { MP_ROM_QSTR(MP_QSTR_device), MP_ROM_PTR(&lpf2_virtual_device_type) }
);

/* --- lpf2.devices --- */
LPF2_DEFINE_MOD_WITH_GLOB(devices,
    LPF2_GET_LPF2_TYPE_REG(basic_motor),
    LPF2_GET_LPF2_TYPE_REG(encoder_motor),
    LPF2_GET_LPF2_TYPE_REG(color_sensor),
    LPF2_GET_LPF2_TYPE_REG(distance_sensor),
    LPF2_GET_LPF2_TYPE_REG(color_distance_sensor),
    LPF2_GET_LPF2_TYPE_REG(hub_led),
    LPF2_GET_LPF2_TYPE_REG(accelerometer),
    LPF2_GET_LPF2_TYPE_REG(gyroscope),
#if LPF2_HAS_PORT_EXPANDER
    LPF2_GET_LPF2_TYPE_REG(port_expander),
#endif
    { MP_ROM_QSTR(MP_QSTR_registerDefault), MP_ROM_PTR(&lpf2_devices_register_default_obj) }
);

#if LPF2_HAS_PORT_EXPANDER
/* --- lpf2.port_expander.port_num --- */
LPF2_DEFINE_MOD_WITH_GLOB_STATIC(port_expander_port_num,
    { MP_ROM_QSTR(MP_QSTR_A), MP_ROM_INT((uint8_t)Lpf2::PortExpander::PortNum::A) },
    { MP_ROM_QSTR(MP_QSTR_B), MP_ROM_INT((uint8_t)Lpf2::PortExpander::PortNum::B) },
    { MP_ROM_QSTR(MP_QSTR_C), MP_ROM_INT((uint8_t)Lpf2::PortExpander::PortNum::C) },
    { MP_ROM_QSTR(MP_QSTR_D), MP_ROM_INT((uint8_t)Lpf2::PortExpander::PortNum::D) }
);

/* --- lpf2.port_expander --- */
LPF2_DEFINE_MOD_WITH_GLOB(port_expander,
    LPF2_GET_LPF2_TYPE_REG(virtual_port_expander_device),
    { MP_ROM_QSTR(MP_QSTR_port_num), MP_ROM_PTR(&LPF2_GET_MOD(port_expander_port_num)) }
);
#endif

/* --- lpf2 --- */
LPF2_DEFINE_MOD_WITH_GLOB_ATTR_USED(lpf2,
    LPF2_GET_LPF2_MOD_REG(hub_type),
    LPF2_GET_LPF2_MOD_REG(device_type),
    LPF2_GET_LPF2_MOD_REG(alerts),
    LPF2_GET_LPF2_MOD_REG(battery_type),
    LPF2_GET_LPF2_MOD_REG(button_state),
    LPF2_GET_LPF2_MOD_REG(color),
    LPF2_GET_LPF2_MOD_REG(port_num),
    LPF2_GET_LPF2_MOD_REG(motor_setting),
    LPF2_GET_LPF2_MOD_REG(battery),
    LPF2_GET_LPF2_MOD_REG(local),
    LPF2_GET_LPF2_MOD_REG(virtual),
    LPF2_GET_LPF2_MOD_REG(devices),
#if LPF2_HAS_PORT_EXPANDER
    LPF2_GET_LPF2_MOD_REG(port_expander),
#endif
    LPF2_GET_LPF2_TYPE_REG(port),
    LPF2_GET_LPF2_TYPE_REG(mode),
    LPF2_GET_LPF2_TYPE_REG(version),
    LPF2_GET_LPF2_TYPE_REG(hub_emulation),
    LPF2_GET_LPF2_TYPE_REG(device_descriptor),
);

MP_REGISTER_MODULE(MP_QSTR_lpf2, LPF2_GET_MOD(lpf2));
} // extern "C"
