#include "mod_types.h"

#define DEFINE_HUB_METHOD(name, method, fun_obj_def) LPF2_DEFINE_METHOD(hub_##name, method, fun_obj_def)
#define DEFINE_HUB_METHOD_VAR_BETWEEN(name, method, min_args, max_args) LPF2_DEFINE_METHOD_VAR_BETWEEN(hub_##name, method, min_args, max_args)
#define GET_HUB_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(hub_##name)

#define SELF_TYPE mp_obj_lpf2_hub_t

extern "C" {

static mp_obj_t lpf2_hub_make_new(const mp_obj_type_t *type,
                                  size_t n_args, size_t n_kw,
                                  const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    SELF_TYPE *o = (SELF_TYPE *)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;
    o->cpp_obj = new Lpf2::Hub();
    o->owned = true;
    o->port_cache = mp_obj_new_dict(0);
    lpf2_reg_add<Lpf2::Hub>(o->cpp_obj);
    return MP_OBJ_FROM_PTR(o);
}

DEFINE_HUB_METHOD_VAR_BETWEEN(init,
{
    auto self = (SELF_TYPE *)MP_OBJ_TO_PTR(args[0]);
    if (n_args == 1) {
        self->cpp_obj->init();
    } else if (n_args == 2) {
        if (mp_obj_is_str(args[1])) {
            size_t len;
            auto cstr = mp_obj_str_get_data(args[1], &len);
            self->cpp_obj->init(std::string(cstr, len));
        } else {
            self->cpp_obj->init((uint32_t)mp_obj_get_uint(args[1]));
        }
    } else {
        size_t len;
        auto cstr = mp_obj_str_get_data(args[1], &len);
        self->cpp_obj->init(std::string(cstr, len), (uint32_t)mp_obj_get_uint(args[2]));
    }
    return mp_const_none;
},
1, 3);

DEFINE_HUB_METHOD(update, (mp_obj_t self_in)
{
    GET_SELF_CPP()->update();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(connect_hub, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->connectHub());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(is_connected, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->isConnected());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(is_connecting, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->isConnecting());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(is_scanning, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->isScanning());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(shut_down_hub, (mp_obj_t self_in)
{
    GET_SELF_CPP()->shutDownHub();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(info_ready, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->infoReady());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(get_name, (mp_obj_t self_in)
{
    std::string s = GET_SELF_CPP()->getName();
    return mp_obj_new_str(s.c_str(), s.length());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(set_name, (mp_obj_t self_in, mp_obj_t name)
{
    size_t len;
    auto cstr = mp_obj_str_get_data(name, &len);
    GET_SELF_CPP()->setName(std::string(cstr, len));
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_HUB_METHOD(get_battery_type, (mp_obj_t self_in)
{
    return mp_obj_new_int_from_uint((uint8_t)GET_SELF_CPP()->getBatteryType());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(get_hub_type, (mp_obj_t self_in)
{
    return mp_obj_new_int_from_uint((uint16_t)GET_SELF_CPP()->getHubType());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(get_hub_address, (mp_obj_t self_in)
{
    std::string s = GET_SELF_CPP()->getHubAddress().toString();
    return mp_obj_new_str(s.c_str(), s.length());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(get_all_info_str, (mp_obj_t self_in)
{
    std::string s = GET_SELF_CPP()->getAllInfoStr();
    return mp_obj_new_str(s.c_str(), s.length());
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_HUB_METHOD(get_hub_prop_str, (mp_obj_t self_in, mp_obj_t prop_id)
{
    std::string s = GET_SELF_CPP()->getHubPropStr((Lpf2::HubPropertyType)mp_obj_get_uint(prop_id));
    return mp_obj_new_str(s.c_str(), s.length());
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_HUB_METHOD(get_port, (mp_obj_t self_in, mp_obj_t port_num_in)
{
    auto self = GET_SELF();
    mp_obj_t key = mp_obj_new_int_from_uint(mp_obj_get_uint(port_num_in));
    mp_map_elem_t *slot = mp_map_lookup(
        mp_obj_dict_get_map(self->port_cache), key, MP_MAP_LOOKUP);
    if (slot != NULL) {
        return slot->value;
    }
    Lpf2::Port *p = self->cpp_obj->getPort((Lpf2::PortNum)mp_obj_get_uint(port_num_in));
    if (!p) {
        return mp_const_none;
    }
    mp_obj_lpf2_port_t *o = (mp_obj_lpf2_port_t *)m_malloc_with_finaliser(sizeof(mp_obj_lpf2_port_t));
    o->base.type = &lpf2_port_type;
    o->cpp_obj = p;
    o->owned = false;
    o->is_trampoline = false;
    o->parent_ref = self_in;
    mp_obj_t wrapper = MP_OBJ_FROM_PTR(o);
    mp_obj_dict_store(self->port_cache, key, wrapper);
    return wrapper;
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_HUB_METHOD_VAR_BETWEEN(set_port_mode,
{
    auto self = (SELF_TYPE *)MP_OBJ_TO_PTR(args[0]);
    Lpf2::PortNum pn = (Lpf2::PortNum)mp_obj_get_uint(args[1]);
    uint8_t mode = (uint8_t)mp_obj_get_uint(args[2]);
    uint32_t delta = (uint32_t)mp_obj_get_uint(args[3]);
    bool notify = n_args > 4 ? mp_obj_is_true(args[4]) : true;
    return mp_obj_new_int(self->cpp_obj->setPortMode(pn, mode, delta, notify));
},
4, 5);

DEFINE_HUB_METHOD_VAR_BETWEEN(set_port_mode_combo,
{
    auto self = (SELF_TYPE *)MP_OBJ_TO_PTR(args[0]);
    Lpf2::PortNum pn = (Lpf2::PortNum)mp_obj_get_uint(args[1]);
    uint8_t comboIdx = (uint8_t)mp_obj_get_uint(args[2]);

    mp_obj_t *items;
    size_t len;
    mp_obj_get_array(args[3], &len, &items);
    std::vector<uint8_t> nibblePairs;
    nibblePairs.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        nibblePairs.push_back((uint8_t)mp_obj_get_uint(items[i]));
    }

    mp_obj_get_array(args[4], &len, &items);
    std::vector<uint32_t> deltasPerMode;
    deltasPerMode.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        deltasPerMode.push_back((uint32_t)mp_obj_get_uint(items[i]));
    }

    return mp_obj_new_int(self->cpp_obj->setPortModeCombo(pn, comboIdx, nibblePairs, deltasPerMode));
},
5, 5);

DEFINE_HUB_METHOD(del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting Hub, owner: %s", self->owned ? "true" : "false");
    if (self->cpp_obj) {
        lpf2_reg_remove<Lpf2::Hub>(self->cpp_obj);
    }
    if (self->owned && self->cpp_obj) {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

static const mp_rom_map_elem_t lpf2_hub_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(del))},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(init))},
    {MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(update))},
    {MP_ROM_QSTR(MP_QSTR_connectHub), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(connect_hub))},
    {MP_ROM_QSTR(MP_QSTR_isConnected), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(is_connected))},
    {MP_ROM_QSTR(MP_QSTR_isConnecting), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(is_connecting))},
    {MP_ROM_QSTR(MP_QSTR_isScanning), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(is_scanning))},
    {MP_ROM_QSTR(MP_QSTR_shutDownHub), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(shut_down_hub))},
    {MP_ROM_QSTR(MP_QSTR_infoReady), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(info_ready))},
    {MP_ROM_QSTR(MP_QSTR_getName), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_name))},
    {MP_ROM_QSTR(MP_QSTR_setName), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(set_name))},
    {MP_ROM_QSTR(MP_QSTR_getBatteryType), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_battery_type))},
    {MP_ROM_QSTR(MP_QSTR_getHubType), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_hub_type))},
    {MP_ROM_QSTR(MP_QSTR_getHubAddress), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_hub_address))},
    {MP_ROM_QSTR(MP_QSTR_getAllInfoStr), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_all_info_str))},
    {MP_ROM_QSTR(MP_QSTR_getHubPropStr), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_hub_prop_str))},
    {MP_ROM_QSTR(MP_QSTR_getPort), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(get_port))},
    {MP_ROM_QSTR(MP_QSTR_setPortMode), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(set_port_mode))},
    {MP_ROM_QSTR(MP_QSTR_setPortModeCombo), MP_ROM_PTR(&GET_HUB_METHOD_OBJ(set_port_mode_combo))},
};

static MP_DEFINE_CONST_DICT(lpf2_hub_locals_dict, lpf2_hub_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_hub_type,
    MP_QSTR_hub,
    MP_TYPE_FLAG_NONE,
    make_new, (void *)lpf2_hub_make_new,
    locals_dict, &lpf2_hub_locals_dict
);

// Remote::Port — non-constructible subclass of lpf2.port; minted by Hub.getPort()
extern const mp_obj_dict_t lpf2_port_locals_dict;

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_remote_port_type,
    MP_QSTR_remote_port,
    MP_TYPE_FLAG_NONE,
    parent, &lpf2_port_type,
    locals_dict, &lpf2_port_locals_dict
);

// lpf2.remote submodule containing the port type
LPF2_DEFINE_MOD_WITH_GLOB(remote,
    { MP_ROM_QSTR(MP_QSTR_port), MP_ROM_PTR(&lpf2_remote_port_type) }
);

// lpf2.hub_property enum module
LPF2_DEFINE_MOD_WITH_GLOB(hub_property,
    LPF2_REG(ADVERTISING_NAME,        Lpf2::HubPropertyType::ADVERTISING_NAME),
    LPF2_REG(BUTTON,                  Lpf2::HubPropertyType::BUTTON),
    LPF2_REG(FW_VERSION,              Lpf2::HubPropertyType::FW_VERSION),
    LPF2_REG(HW_VERSION,              Lpf2::HubPropertyType::HW_VERSION),
    LPF2_REG(RSSI,                    Lpf2::HubPropertyType::RSSI),
    LPF2_REG(BATTERY_VOLTAGE,         Lpf2::HubPropertyType::BATTERY_VOLTAGE),
    LPF2_REG(BATTERY_TYPE,            Lpf2::HubPropertyType::BATTERY_TYPE),
    LPF2_REG(MANUFACTURER_NAME,       Lpf2::HubPropertyType::MANUFACTURER_NAME),
    LPF2_REG(RADIO_FIRMWARE_VERSION,  Lpf2::HubPropertyType::RADIO_FIRMWARE_VERSION),
    LPF2_REG(LEGO_WIRELESS_PROTOCOL_VERSION, Lpf2::HubPropertyType::LEGO_WIRELESS_PROTOCOL_VERSION),
    LPF2_REG(SYSTEM_TYPE_ID,          Lpf2::HubPropertyType::SYSTEM_TYPE_ID),
    LPF2_REG(HW_NETWORK_ID,           Lpf2::HubPropertyType::HW_NETWORK_ID),
    LPF2_REG(PRIMARY_MAC_ADDRESS,     Lpf2::HubPropertyType::PRIMARY_MAC_ADDRESS),
    LPF2_REG(SECONDARY_MAC_ADDRESS,   Lpf2::HubPropertyType::SECONDARY_MAC_ADDRESS),
    LPF2_REG(HARDWARE_NETWORK_FAMILY, Lpf2::HubPropertyType::HARDWARE_NETWORK_FAMILY)
);

} // extern "C"
