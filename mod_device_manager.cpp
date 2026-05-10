#include "mod_types.h"

#define DEFINE_DM_METHOD(name, method, fun_obj_def) LPF2_DEFINE_METHOD(device_manager_##name, method, fun_obj_def)
#define DEFINE_DM_METHOD_VAR_BETWEEN(name, method, min_args, max_args) LPF2_DEFINE_METHOD_VAR_BETWEEN(device_manager_##name, method, min_args, max_args)
#define GET_DM_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(device_manager_##name)

#define SELF_TYPE mp_obj_lpf2_device_manager_t

extern "C" {

static mp_obj_t lpf2_device_manager_make_new(const mp_obj_type_t *type,
                                              size_t n_args,
                                              size_t n_kw,
                                              const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 1, 1, false);

    mp_obj_t native_port = lpf2_cast_to_native_base(args[0], &lpf2_virtual_port_type);
    if (native_port == MP_OBJ_NULL)
        native_port = lpf2_cast_to_native_base(args[0], &lpf2_port_type);
    if (native_port == MP_OBJ_NULL)
        mp_raise_TypeError(MP_ERROR_TEXT("expected lpf2.port"));

    mp_obj_lpf2_port_t *port_obj = (mp_obj_lpf2_port_t *)MP_OBJ_TO_PTR(native_port);

    SELF_TYPE *o = (SELF_TYPE *)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;
    o->cpp_obj = new Lpf2::DeviceManager(*port_obj->cpp_obj);
    o->owned = true;
    o->port_ref = args[0];

    return MP_OBJ_FROM_PTR(o);
}

DEFINE_DM_METHOD(del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting DeviceManager, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_DM_METHOD(init, (mp_obj_t self_in)
{
    GET_SELF_CPP()->init();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_DM_METHOD(update, (mp_obj_t self_in)
{
    GET_SELF_CPP()->update();
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_DM_METHOD(get_device_type, (mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_SELF_CPP()->getDeviceType());
},
MP_DEFINE_CONST_FUN_OBJ_1);

static mp_obj_t make_device_obj(mp_obj_t manager_in, Lpf2::Device *dev)
{
    if (!dev)
        return mp_const_none;

    if (dev->hasCapability(Lpf2::Devices::EncoderMotor::CAP)) {
        mp_obj_lpf2_encoder_motor_t *o = m_new_obj(mp_obj_lpf2_encoder_motor_t);
        o->base.type = &lpf2_encoder_motor_type;
        o->cpp_obj = static_cast<Lpf2::Devices::EncoderMotor *>(dev);
        o->manager_ref = manager_in;
        return MP_OBJ_FROM_PTR(o);
    }
    if (dev->hasCapability(Lpf2::Devices::BasicMotor::CAP)) {
        mp_obj_lpf2_basic_motor_t *o = m_new_obj(mp_obj_lpf2_basic_motor_t);
        o->base.type = &lpf2_basic_motor_type;
        o->cpp_obj = static_cast<Lpf2::Devices::BasicMotor *>(dev);
        o->manager_ref = manager_in;
        return MP_OBJ_FROM_PTR(o);
    }
    if (dev->hasCapability(Lpf2::Devices::TechnicColorSensor::CAP)) {
        mp_obj_lpf2_color_sensor_t *o = m_new_obj(mp_obj_lpf2_color_sensor_t);
        o->base.type = &lpf2_color_sensor_type;
        o->cpp_obj = static_cast<Lpf2::Devices::TechnicColorSensor *>(dev);
        o->manager_ref = manager_in;
        return MP_OBJ_FROM_PTR(o);
    }
    if (dev->hasCapability(Lpf2::Devices::TechnicDistanceSensor::CAP)) {
        mp_obj_lpf2_distance_sensor_t *o = m_new_obj(mp_obj_lpf2_distance_sensor_t);
        o->base.type = &lpf2_distance_sensor_type;
        o->cpp_obj = static_cast<Lpf2::Devices::TechnicDistanceSensor *>(dev);
        o->manager_ref = manager_in;
        return MP_OBJ_FROM_PTR(o);
    }
    if (dev->hasCapability(Lpf2::Devices::PortExpander::CAP)) {
        mp_obj_lpf2_port_expander_dev_t *o = m_new_obj(mp_obj_lpf2_port_expander_dev_t);
        o->base.type = &lpf2_port_expander_type;
        o->cpp_obj = static_cast<Lpf2::Devices::PortExpander *>(dev);
        o->manager_ref = manager_in;
        return MP_OBJ_FROM_PTR(o);
    }

    return mp_const_none;
}

DEFINE_DM_METHOD(device, (mp_obj_t self_in)
{
    return make_device_obj(self_in, GET_SELF_CPP()->device());
},
MP_DEFINE_CONST_FUN_OBJ_1);

static const mp_rom_map_elem_t lpf2_device_manager_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),      MP_ROM_PTR(&GET_DM_METHOD_OBJ(del))},
    {MP_ROM_QSTR(MP_QSTR_init),         MP_ROM_PTR(&GET_DM_METHOD_OBJ(init))},
    {MP_ROM_QSTR(MP_QSTR_update),       MP_ROM_PTR(&GET_DM_METHOD_OBJ(update))},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType),MP_ROM_PTR(&GET_DM_METHOD_OBJ(get_device_type))},
    {MP_ROM_QSTR(MP_QSTR_device),       MP_ROM_PTR(&GET_DM_METHOD_OBJ(device))},
};

static MP_DEFINE_CONST_DICT(lpf2_device_manager_locals_dict, lpf2_device_manager_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_device_manager_type,
    MP_QSTR_device_manager,
    MP_TYPE_FLAG_NONE,
    make_new, (void *)lpf2_device_manager_make_new,
    locals_dict, &lpf2_device_manager_locals_dict
);

} // extern "C"
