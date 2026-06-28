#include "mod_types.h"

#if LPF2_HAS_PORT_EXPANDER

#define DEFINE_VPED_METHOD(name, method, fun_obj_def) LPF2_DEFINE_METHOD(virtual_port_expander_device_##name, method, fun_obj_def)
#define GET_VPED_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(virtual_port_expander_device_##name)

#define SELF_TYPE mp_obj_lpf2_virtual_port_expander_device_t

extern "C" {

static mp_obj_t lpf2_virtual_port_expander_device_make_new(const mp_obj_type_t *type,
                                                            size_t n_args,
                                                            size_t n_kw,
                                                            const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    SELF_TYPE *o = (SELF_TYPE *)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;
    o->cpp_obj = new Lpf2::Virtual::PortExpanderDevice();
    o->owned = true;
    for (int i = 0; i < 4; ++i)
        o->port_refs[i] = MP_OBJ_NULL;

    return MP_OBJ_FROM_PTR(o);
}

DEFINE_VPED_METHOD(del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting Virtual PortExpanderDevice, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_VPED_METHOD(attach_port, (mp_obj_t self_in, mp_obj_t port_num_in, mp_obj_t port_in)
{
    auto self = GET_SELF();
    uint8_t num = (uint8_t)mp_obj_get_uint(port_num_in);
    if (num >= 4)
        mp_raise_ValueError(MP_ERROR_TEXT("port_num out of range (0-3)"));

    mp_obj_t native_port = lpf2_cast_to_native_base(port_in, &lpf2_virtual_port_type);
    if (native_port == MP_OBJ_NULL)
        native_port = lpf2_cast_to_native_base(port_in, &lpf2_port_type);
    if (native_port == MP_OBJ_NULL)
        mp_raise_TypeError(MP_ERROR_TEXT("expected lpf2.port"));

    mp_obj_lpf2_port_t *port_obj = (mp_obj_lpf2_port_t *)MP_OBJ_TO_PTR(native_port);
    self->cpp_obj->attachPort((Lpf2::PortExpander::PortNum)num, port_obj->cpp_obj);
    self->port_refs[num] = port_in;
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_3);

static const mp_rom_map_elem_t lpf2_virtual_port_expander_device_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),   MP_ROM_PTR(&GET_VPED_METHOD_OBJ(del))},
    {MP_ROM_QSTR(MP_QSTR_attachPort),MP_ROM_PTR(&GET_VPED_METHOD_OBJ(attach_port))},
};

static MP_DEFINE_CONST_DICT(lpf2_virtual_port_expander_device_locals_dict,
                             lpf2_virtual_port_expander_device_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_virtual_port_expander_device_type,
    MP_QSTR_virtual_port_expander_device,
    MP_TYPE_FLAG_NONE,
    make_new, (void *)lpf2_virtual_port_expander_device_make_new,
    locals_dict, &lpf2_virtual_port_expander_device_locals_dict
);

} // extern "C"

#endif // LPF2_HAS_PORT_EXPANDER
