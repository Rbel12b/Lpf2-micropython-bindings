#include "mod_types.h"

#define DEFINE_EMU_PORT_METHOD(name, method, fun_obj_def) \
    LPF2_DEFINE_METHOD(emulated_port_##name, method, fun_obj_def)
#define GET_EMU_PORT_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(emulated_port_##name)

#define SELF_TYPE mp_obj_lpf2_emulated_port_t

extern "C" {

static mp_obj_t lpf2_emulated_port_make_new(const mp_obj_type_t *type,
                                             size_t n_args,
                                             size_t n_kw,
                                             const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 1, 1, false);

    mp_obj_t native = lpf2_cast_to_native_base(args[0], &lpf2_local_port_type);
    if (native == MP_OBJ_NULL)
        mp_raise_TypeError(MP_ERROR_TEXT("expected lpf2.local.port"));

    auto *port_obj = (mp_obj_lpf2_port_t *)MP_OBJ_TO_PTR(native);
    if (port_obj->is_trampoline)
        mp_raise_TypeError(MP_ERROR_TEXT("emulated_port requires a hardware local port"));

    auto *local_port = static_cast<Lpf2::Local::Port *>(port_obj->cpp_obj);
    // Disable normal UART scanning so the UART is ours to use.
    local_port->disable(true);

    Lpf2::Local::Uart *uart = local_port->getIO().getUart();

    SELF_TYPE *o = (SELF_TYPE *)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;
    o->cpp_obj = new Lpf2::Local::EmulatedPort(*uart);
    o->owned = true;
    o->port_ref = args[0];
    o->device_ref = MP_OBJ_NULL;
    // Initialize UART + writer/parser before registering for auto-update.
    // Without this, hub_main_task could call update() with an uninitialized
    // writer (m_serial == nullptr) if the device value-change fires first.
    o->cpp_obj->init();
    lpf2_reg_add<Lpf2::Local::EmulatedPort>(o->cpp_obj);

    return MP_OBJ_FROM_PTR(o);
}

DEFINE_EMU_PORT_METHOD(del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    if (self->owned && self->cpp_obj) {
        lpf2_reg_remove<Lpf2::Local::EmulatedPort>(self->cpp_obj);
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_EMU_PORT_METHOD(init, (mp_obj_t self_in)
{
    GET_SELF_CPP()->init();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_EMU_PORT_METHOD(update, (mp_obj_t self_in)
{
    GET_SELF_CPP()->update();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_EMU_PORT_METHOD(is_host_connected, (mp_obj_t self_in)
{
    return mp_obj_new_bool(GET_SELF_CPP()->isHostConnected());
},
MP_DEFINE_CONST_FUN_OBJ_1);

static Lpf2::Virtual::Device *lpf2_emulated_port_resolve_device(mp_obj_t device_in)
{
    mp_obj_t native = lpf2_cast_to_native_base(device_in, &lpf2_virtual_device_type);
    if (native != MP_OBJ_NULL)
        return ((mp_obj_lpf2_virtual_device_t *)MP_OBJ_TO_PTR(native))->cpp_obj;
#if LPF2_HAS_PORT_EXPANDER
    native = lpf2_cast_to_native_base(device_in, &lpf2_virtual_port_expander_device_type);
    if (native != MP_OBJ_NULL)
        return ((mp_obj_lpf2_virtual_port_expander_device_t *)MP_OBJ_TO_PTR(native))->cpp_obj;
#endif
    return nullptr;
}

static mp_obj_t lpf2_emulated_port_attach_device(mp_obj_t self_in, mp_obj_t device_in)
{
    auto self = (SELF_TYPE *)MP_OBJ_TO_PTR(self_in);
    Lpf2::Virtual::Device *dev = lpf2_emulated_port_resolve_device(device_in);
    if (!dev)
        mp_raise_TypeError(MP_ERROR_TEXT("expected virtual device"));
    // Remove from auto-update registry: EmulatedPort::update() drives device->update() itself.
    lpf2_reg_remove<Lpf2::Virtual::Device>(dev);
    self->cpp_obj->attachDevice(*dev);
    self->device_ref = device_in;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_emulated_port_attach_device_obj, lpf2_emulated_port_attach_device);

static mp_obj_t lpf2_emulated_port_detach_device(mp_obj_t self_in)
{
    auto self = (SELF_TYPE *)MP_OBJ_TO_PTR(self_in);
    if (!self->cpp_obj)
        return mp_const_none;
    if (self->device_ref != MP_OBJ_NULL) {
        Lpf2::Virtual::Device *dev = lpf2_emulated_port_resolve_device(self->device_ref);
        if (dev)
            lpf2_reg_add<Lpf2::Virtual::Device>(dev);
        self->device_ref = MP_OBJ_NULL;
    }
    self->cpp_obj->detachDevice();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_emulated_port_detach_device_obj, lpf2_emulated_port_detach_device);

static const mp_rom_map_elem_t lpf2_emulated_port_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),         MP_ROM_PTR(&GET_EMU_PORT_METHOD_OBJ(del))},
    {MP_ROM_QSTR(MP_QSTR_init),            MP_ROM_PTR(&GET_EMU_PORT_METHOD_OBJ(init))},
    {MP_ROM_QSTR(MP_QSTR_update),          MP_ROM_PTR(&GET_EMU_PORT_METHOD_OBJ(update))},
    {MP_ROM_QSTR(MP_QSTR_isHostConnected), MP_ROM_PTR(&GET_EMU_PORT_METHOD_OBJ(is_host_connected))},
    {MP_ROM_QSTR(MP_QSTR_attachDevice),    MP_ROM_PTR(&lpf2_emulated_port_attach_device_obj)},
    {MP_ROM_QSTR(MP_QSTR_detachDevice),    MP_ROM_PTR(&lpf2_emulated_port_detach_device_obj)},
};

static MP_DEFINE_CONST_DICT(lpf2_emulated_port_locals_dict, lpf2_emulated_port_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_emulated_port_type,
    MP_QSTR_emulated_port,
    MP_TYPE_FLAG_NONE,
    make_new, (void *)lpf2_emulated_port_make_new,
    locals_dict, &lpf2_emulated_port_locals_dict
);

} // extern "C"
