extern "C" {
#include "py/runtime.h"
}

#include "mod_types.h"
#include "Virtual_device_trampoline.h"

#define DEFINE_VDEV_METHOD(name, method, fun_obj_def) LPF2_DEFINE_METHOD(virtual_device_##name, method, fun_obj_def)
#define DEFINE_VDEV_METHOD_VAR_BETWEEN(name, method, min_args, max_args) LPF2_DEFINE_METHOD_VAR_BETWEEN(virtual_device_##name, method, min_args, max_args)
#define GET_VDEV_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(virtual_device_##name)

#define SELF_TYPE mp_obj_lpf2_virtual_device_t
#define GET_VDEV_GENERIC(self_in) (static_cast<Lpf2::Virtual::GenericDevice*>(((SELF_TYPE*)MP_OBJ_TO_PTR(self_in))->cpp_obj))

extern "C" {

static mp_obj_t lpf2_virtual_device_make_new(const mp_obj_type_t *type,
                                              size_t n_args,
                                              size_t n_kw,
                                              const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 1, 1, false);

    if (!mp_obj_is_type(args[0], &lpf2_device_descriptor_type))
    {
        mp_raise_TypeError(MP_ERROR_TEXT("expected device_descriptor"));
    }

    mp_obj_lpf2_device_descriptor_t *desc = (mp_obj_lpf2_device_descriptor_t *)MP_OBJ_TO_PTR(args[0]);

    SELF_TYPE *o = (SELF_TYPE *)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;
    o->cpp_obj = new VirtualDeviceTrampoline(MP_OBJ_FROM_PTR(o), *desc->cpp_obj);
    o->owned = true;
    o->descriptor_ref = args[0];
    o->write_data_cb = MP_OBJ_NULL;

    return MP_OBJ_FROM_PTR(o);
}

DEFINE_VDEV_METHOD(del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting Virtual Device, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

DEFINE_VDEV_METHOD(set_mode_data, (mp_obj_t self_in, mp_obj_t mode_in, mp_obj_t buf_in)
{
    mp_buffer_info_t buf;
    mp_get_buffer_raise(buf_in, &buf, MP_BUFFER_READ);

    std::vector<uint8_t> data(
        (uint8_t *)buf.buf,
        (uint8_t *)buf.buf + buf.len);

    GET_VDEV_GENERIC(self_in)->setModeData(
        (uint8_t)mp_obj_get_uint(mode_in), data);

    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_3);

DEFINE_VDEV_METHOD(set_write_data_callback, (mp_obj_t self_in, mp_obj_t cb_in)
{
    auto self = GET_SELF();
    self->write_data_cb = cb_in;

    mp_obj_t cb = cb_in;
    GET_VDEV_GENERIC(self_in)->setWriteDataCallback(
        [cb](uint8_t mode, const std::vector<uint8_t> &data, void *) -> int {
            mp_obj_t call_args[2] = {
                mp_obj_new_int(mode),
                mp_obj_new_bytes(data.data(), data.size())};
            mp_obj_t ret = mp_call_function_n_kw(cb, 2, 0, call_args);
            return mp_obj_get_int(ret);
        });

    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_2);

/* Default motor method implementations — call GenericDevice non-virtually to avoid
   recursion when the trampoline dispatches to Python, which then calls these defaults. */

DEFINE_VDEV_METHOD(start_power, (mp_obj_t self_in, mp_obj_t pw)
{
    GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::startPower(
        (int8_t)mp_obj_get_int(pw));
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_VDEV_METHOD(set_acc_time, (mp_obj_t self_in, mp_obj_t time, mp_obj_t profile)
{
    GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::setAccTime(
        (uint16_t)mp_obj_get_uint(time),
        (Lpf2::AccelerationProfile)mp_obj_get_uint(profile));
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_3);

DEFINE_VDEV_METHOD(set_dec_time, (mp_obj_t self_in, mp_obj_t time, mp_obj_t profile)
{
    GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::setDecTime(
        (uint16_t)mp_obj_get_uint(time),
        (Lpf2::AccelerationProfile)mp_obj_get_uint(profile));
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_3);

DEFINE_VDEV_METHOD_VAR_BETWEEN(start_speed,
{
    int8_t speed       = (int8_t)  mp_obj_get_int(args[1]);
    uint8_t maxPower   = (uint8_t) mp_obj_get_uint(args[2]);
    uint8_t useProfile = 0;
    if (n_args > 3) {
        useProfile = (uint8_t)mp_obj_get_uint(args[3]);
    }
    GET_VDEV_GENERIC(args[0])->Lpf2::Virtual::GenericDevice::startSpeed(speed, maxPower, useProfile);
    return mp_const_none;
},
3, 4);

DEFINE_VDEV_METHOD_VAR_BETWEEN(start_speed_for_time,
{
    uint16_t time      = (uint16_t) mp_obj_get_uint(args[1]);
    int8_t speed       = (int8_t)   mp_obj_get_int(args[2]);
    uint8_t maxPower   = (uint8_t)  mp_obj_get_uint(args[3]);
    Lpf2::BrakingStyle endState = (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]);
    uint8_t useProfile = 0;
    if (n_args > 5) {
        useProfile = (uint8_t)mp_obj_get_uint(args[5]);
    }
    GET_VDEV_GENERIC(args[0])->Lpf2::Virtual::GenericDevice::startSpeedForTime(
        time, speed, maxPower, endState, useProfile);
    return mp_const_none;
},
5, 6);

DEFINE_VDEV_METHOD_VAR_BETWEEN(start_speed_for_degrees,
{
    uint32_t degrees   = (uint32_t) mp_obj_get_ll(args[1]);
    int8_t speed       = (int8_t)   mp_obj_get_int(args[2]);
    uint8_t maxPower   = (uint8_t)  mp_obj_get_uint(args[3]);
    Lpf2::BrakingStyle endState = (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]);
    uint8_t useProfile = 0;
    if (n_args > 5) {
        useProfile = (uint8_t)mp_obj_get_uint(args[5]);
    }
    GET_VDEV_GENERIC(args[0])->Lpf2::Virtual::GenericDevice::startSpeedForDegrees(
        degrees, speed, maxPower, endState, useProfile);
    return mp_const_none;
},
5, 6);

DEFINE_VDEV_METHOD_VAR_BETWEEN(goto_abs_position,
{
    int32_t absPos     = (int32_t)  mp_obj_get_ll(args[1]);
    uint8_t speed      = (uint8_t)  mp_obj_get_uint(args[2]);
    uint8_t maxPower   = (uint8_t)  mp_obj_get_uint(args[3]);
    Lpf2::BrakingStyle endState = (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]);
    uint8_t useProfile = 0;
    if (n_args > 5) {
        useProfile = (uint8_t)mp_obj_get_uint(args[5]);
    }
    GET_VDEV_GENERIC(args[0])->Lpf2::Virtual::GenericDevice::gotoAbsPosition(
        absPos, speed, maxPower, endState, useProfile);
    return mp_const_none;
},
5, 6);

DEFINE_VDEV_METHOD(preset_encoder, (mp_obj_t self_in, mp_obj_t pos)
{
    GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::presetEncoder(
        (int32_t)mp_obj_get_ll(pos));
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_VDEV_METHOD(write_data, (mp_obj_t self_in, mp_obj_t mode_in, mp_obj_t buf_in)
{
    mp_buffer_info_t buf;
    mp_get_buffer_raise(buf_in, &buf, MP_BUFFER_READ);

    std::vector<uint8_t> data(
        (uint8_t *)buf.buf,
        (uint8_t *)buf.buf + buf.len);

    int ret = GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::writeData(
        (uint8_t)mp_obj_get_uint(mode_in), data);
    return mp_obj_new_int(ret);
},
MP_DEFINE_CONST_FUN_OBJ_3);

DEFINE_VDEV_METHOD(set_mode, (mp_obj_t self_in, mp_obj_t mode)
{
    int ret = GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::setMode(
        (uint8_t)mp_obj_get_uint(mode));
    return mp_obj_new_int(ret);
},
MP_DEFINE_CONST_FUN_OBJ_2);

DEFINE_VDEV_METHOD(set_mode_combo, (mp_obj_t self_in, mp_obj_t idx)
{
    int ret = GET_VDEV_GENERIC(self_in)->Lpf2::Virtual::GenericDevice::setModeCombo(
        (uint8_t)mp_obj_get_uint(idx));
    return mp_obj_new_int(ret);
},
MP_DEFINE_CONST_FUN_OBJ_2);

static const mp_rom_map_elem_t lpf2_virtual_device_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),              MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(del))},
    {MP_ROM_QSTR(MP_QSTR_setModeData),          MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_mode_data))},
    {MP_ROM_QSTR(MP_QSTR_setWriteDataCallback), MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_write_data_callback))},
    /* Default motor implementations — Python subclasses may override these */
    {MP_ROM_QSTR(MP_QSTR_startPower),           MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(start_power))},
    {MP_ROM_QSTR(MP_QSTR_setAccTime),           MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_acc_time))},
    {MP_ROM_QSTR(MP_QSTR_setDecTime),           MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_dec_time))},
    {MP_ROM_QSTR(MP_QSTR_startSpeed),           MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(start_speed))},
    {MP_ROM_QSTR(MP_QSTR_startSpeedForTime),    MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(start_speed_for_time))},
    {MP_ROM_QSTR(MP_QSTR_startSpeedForDegrees), MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(start_speed_for_degrees))},
    {MP_ROM_QSTR(MP_QSTR_gotoAbsPosition),      MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(goto_abs_position))},
    {MP_ROM_QSTR(MP_QSTR_presetEncoder),        MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(preset_encoder))},
    {MP_ROM_QSTR(MP_QSTR_writeData),            MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(write_data))},
    {MP_ROM_QSTR(MP_QSTR_setMode),              MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_mode))},
    {MP_ROM_QSTR(MP_QSTR_setModeCombo),         MP_ROM_PTR(&GET_VDEV_METHOD_OBJ(set_mode_combo))},
};

static MP_DEFINE_CONST_DICT(lpf2_virtual_device_locals_dict, lpf2_virtual_device_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_virtual_device_type,
    MP_QSTR_virtual_device,
    MP_TYPE_FLAG_NONE,
    make_new, (void *)lpf2_virtual_device_make_new,
    locals_dict, &lpf2_virtual_device_locals_dict
);

} // extern "C"
