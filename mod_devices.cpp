#include "mod_types.h"

#define DEVICE_DEL(name, wrapper_t)                                             \
    static mp_obj_t lpf2_##name##_del(mp_obj_t self_in)                         \
    {                                                                           \
        lpf2_device_slot_del<wrapper_t>(self_in);                               \
        return mp_const_none;                                                   \
    }                                                                           \
    static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_##name##_del_obj, lpf2_##name##_del)

extern "C" {

/* ------------------------------------------------------------------ */
/* basic_motor                                                          */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_basic_motor_t
#define GET_BM_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::BasicMotor, SELF_TYPE>(self_in)

DEVICE_DEL(basic_motor, SELF_TYPE);

static mp_obj_t lpf2_basic_motor_start_power(mp_obj_t self_in, mp_obj_t pw)
{
    GET_BM_CPP(self_in)->startPower((int8_t)mp_obj_get_int(pw));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_basic_motor_start_power_obj, lpf2_basic_motor_start_power);

static mp_obj_t lpf2_basic_motor_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_BM_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_basic_motor_get_device_type_obj, lpf2_basic_motor_get_device_type);

static mp_obj_t lpf2_basic_motor_name(mp_obj_t self_in)
{
    const char *n = GET_BM_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_basic_motor_name_obj, lpf2_basic_motor_name);

static const mp_rom_map_elem_t lpf2_basic_motor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_basic_motor_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_startPower),    MP_ROM_PTR(&lpf2_basic_motor_start_power_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_basic_motor_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_basic_motor_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_basic_motor_locals_dict, lpf2_basic_motor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_basic_motor_type,
    MP_QSTR_basic_motor,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_basic_motor_locals_dict
);

#undef SELF_TYPE
#undef GET_BM_CPP

/* ------------------------------------------------------------------ */
/* encoder_motor                                                        */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_encoder_motor_t
#define GET_EM_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::EncoderMotor, SELF_TYPE>(self_in)

DEVICE_DEL(encoder_motor, SELF_TYPE);

static mp_obj_t lpf2_encoder_motor_start_power(mp_obj_t self_in, mp_obj_t pw)
{
    GET_EM_CPP(self_in)->startPower((int8_t)mp_obj_get_int(pw));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_encoder_motor_start_power_obj, lpf2_encoder_motor_start_power);

static mp_obj_t lpf2_encoder_motor_set_acc_time(mp_obj_t self_in, mp_obj_t time, mp_obj_t profile)
{
    GET_EM_CPP(self_in)->setAccTime((uint16_t)mp_obj_get_uint(time),
                                    (Lpf2::AccelerationProfile)mp_obj_get_uint(profile));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(lpf2_encoder_motor_set_acc_time_obj, lpf2_encoder_motor_set_acc_time);

static mp_obj_t lpf2_encoder_motor_set_dec_time(mp_obj_t self_in, mp_obj_t time, mp_obj_t profile)
{
    GET_EM_CPP(self_in)->setDecTime((uint16_t)mp_obj_get_uint(time),
                                    (Lpf2::AccelerationProfile)mp_obj_get_uint(profile));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(lpf2_encoder_motor_set_dec_time_obj, lpf2_encoder_motor_set_dec_time);

static mp_obj_t lpf2_encoder_motor_start_speed(size_t n_args, const mp_obj_t *args)
{
    int8_t  speed      = n_args > 1 ? (int8_t)mp_obj_get_int(args[1]) : 100;
    uint8_t maxPower   = n_args > 2 ? (uint8_t)mp_obj_get_uint(args[2]) : 100;
    uint8_t useProfile = n_args > 3 ? (uint8_t)mp_obj_get_uint(args[3]) : 0;
    GET_EM_CPP(args[0])->startSpeed(speed, maxPower, useProfile);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_encoder_motor_start_speed_obj, 1, 4, lpf2_encoder_motor_start_speed);

static mp_obj_t lpf2_encoder_motor_start_speed_for_time(size_t n_args, const mp_obj_t *args)
{
    uint16_t time      = (uint16_t)mp_obj_get_uint(args[1]);
    int8_t   speed     = n_args > 2 ? (int8_t)mp_obj_get_int(args[2]) : 100;
    uint8_t  maxPower  = n_args > 3 ? (uint8_t)mp_obj_get_uint(args[3]) : 100;
    Lpf2::BrakingStyle endState = n_args > 4 ? (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]) : Lpf2::BrakingStyle::FLOAT;
    uint8_t  useProfile = n_args > 5 ? (uint8_t)mp_obj_get_uint(args[5]) : 0;
    GET_EM_CPP(args[0])->startSpeedForTime(time, speed, maxPower, endState, useProfile);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_encoder_motor_start_speed_for_time_obj, 2, 6, lpf2_encoder_motor_start_speed_for_time);

static mp_obj_t lpf2_encoder_motor_start_speed_for_degrees(size_t n_args, const mp_obj_t *args)
{
    uint32_t degrees   = (uint32_t)mp_obj_get_ll(args[1]);
    int8_t   speed     = n_args > 2 ? (int8_t)mp_obj_get_int(args[2]) : 100;
    uint8_t  maxPower  = n_args > 3 ? (uint8_t)mp_obj_get_uint(args[3]) : 100;
    Lpf2::BrakingStyle endState = n_args > 4 ? (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]) : Lpf2::BrakingStyle::FLOAT;
    uint8_t  useProfile = n_args > 5 ? (uint8_t)mp_obj_get_uint(args[5]) : 0;
    GET_EM_CPP(args[0])->startSpeedForDegrees(degrees, speed, maxPower, endState, useProfile);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_encoder_motor_start_speed_for_degrees_obj, 2, 6, lpf2_encoder_motor_start_speed_for_degrees);

static mp_obj_t lpf2_encoder_motor_goto_abs_pos(size_t n_args, const mp_obj_t *args)
{
    int32_t  absPos    = (int32_t) mp_obj_get_ll(args[1]);
    uint8_t  speed     = n_args > 2 ? (uint8_t)mp_obj_get_uint(args[2]) : 100;
    uint8_t  maxPower  = n_args > 3 ? (uint8_t)mp_obj_get_uint(args[3]) : 100;
    Lpf2::BrakingStyle endState = n_args > 4 ? (Lpf2::BrakingStyle)mp_obj_get_uint(args[4]) : Lpf2::BrakingStyle::FLOAT;
    uint8_t  useProfile = n_args > 5 ? (uint8_t)mp_obj_get_uint(args[5]) : 0;
    GET_EM_CPP(args[0])->gotoAbsPosition(absPos, speed, maxPower, endState, useProfile);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_encoder_motor_goto_abs_pos_obj, 2, 6, lpf2_encoder_motor_goto_abs_pos);

static mp_obj_t lpf2_encoder_motor_preset_encoder(mp_obj_t self_in, mp_obj_t pos)
{
    GET_EM_CPP(self_in)->presetEncoder((int32_t)mp_obj_get_ll(pos));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_encoder_motor_preset_encoder_obj, lpf2_encoder_motor_preset_encoder);

static mp_obj_t lpf2_encoder_motor_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_EM_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_encoder_motor_get_device_type_obj, lpf2_encoder_motor_get_device_type);

static mp_obj_t lpf2_encoder_motor_name(mp_obj_t self_in)
{
    const char *n = GET_EM_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_encoder_motor_name_obj, lpf2_encoder_motor_name);

static const mp_rom_map_elem_t lpf2_encoder_motor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),              MP_ROM_PTR(&lpf2_encoder_motor_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_startPower),           MP_ROM_PTR(&lpf2_encoder_motor_start_power_obj)},
    {MP_ROM_QSTR(MP_QSTR_setAccTime),           MP_ROM_PTR(&lpf2_encoder_motor_set_acc_time_obj)},
    {MP_ROM_QSTR(MP_QSTR_setDecTime),           MP_ROM_PTR(&lpf2_encoder_motor_set_dec_time_obj)},
    {MP_ROM_QSTR(MP_QSTR_startSpeed),           MP_ROM_PTR(&lpf2_encoder_motor_start_speed_obj)},
    {MP_ROM_QSTR(MP_QSTR_startSpeedForTime),    MP_ROM_PTR(&lpf2_encoder_motor_start_speed_for_time_obj)},
    {MP_ROM_QSTR(MP_QSTR_startSpeedForDegrees), MP_ROM_PTR(&lpf2_encoder_motor_start_speed_for_degrees_obj)},
    {MP_ROM_QSTR(MP_QSTR_gotoAbsPosition),      MP_ROM_PTR(&lpf2_encoder_motor_goto_abs_pos_obj)},
    {MP_ROM_QSTR(MP_QSTR_presetEncoder),        MP_ROM_PTR(&lpf2_encoder_motor_preset_encoder_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType),        MP_ROM_PTR(&lpf2_encoder_motor_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),                 MP_ROM_PTR(&lpf2_encoder_motor_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_encoder_motor_locals_dict, lpf2_encoder_motor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_encoder_motor_type,
    MP_QSTR_encoder_motor,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_encoder_motor_locals_dict
);

#undef SELF_TYPE
#undef GET_EM_CPP

/* ------------------------------------------------------------------ */
/* color_sensor                                                         */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_color_sensor_t
#define GET_CS_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::TechnicColorSensor, SELF_TYPE>(self_in)

DEVICE_DEL(color_sensor, SELF_TYPE);

static mp_obj_t lpf2_color_sensor_get_color_idx(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_CS_CPP(self_in)->getColorIdx());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_color_idx_obj, lpf2_color_sensor_get_color_idx);

static mp_obj_t lpf2_color_sensor_get_reflectivity(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_CS_CPP(self_in)->getReflectivity());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_reflectivity_obj, lpf2_color_sensor_get_reflectivity);

static mp_obj_t lpf2_color_sensor_get_ambient_light(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_CS_CPP(self_in)->getAmbientLight());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_ambient_light_obj, lpf2_color_sensor_get_ambient_light);

static mp_obj_t lpf2_color_sensor_get_rgb(mp_obj_t self_in)
{
    uint16_t r, g, b;
    GET_CS_CPP(self_in)->getRGB(r, g, b);
    mp_obj_t tup = mp_obj_new_tuple(3, nullptr);
    mp_obj_tuple_t *t = (mp_obj_tuple_t *)MP_OBJ_TO_PTR(tup);
    t->items[0] = mp_obj_new_int(r);
    t->items[1] = mp_obj_new_int(g);
    t->items[2] = mp_obj_new_int(b);
    return tup;
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_rgb_obj, lpf2_color_sensor_get_rgb);

static mp_obj_t lpf2_color_sensor_get_hsv(mp_obj_t self_in)
{
    uint16_t h, s, v;
    GET_CS_CPP(self_in)->getHSV(h, s, v);
    mp_obj_t tup = mp_obj_new_tuple(3, nullptr);
    mp_obj_tuple_t *t = (mp_obj_tuple_t *)MP_OBJ_TO_PTR(tup);
    t->items[0] = mp_obj_new_int(h);
    t->items[1] = mp_obj_new_int(s);
    t->items[2] = mp_obj_new_int(v);
    return tup;
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_hsv_obj, lpf2_color_sensor_get_hsv);

static mp_obj_t lpf2_color_sensor_set_light(size_t n_args, const mp_obj_t *args)
{
    GET_CS_CPP(args[0])->setLight(
        (uint8_t)mp_obj_get_uint(args[1]),
        (uint8_t)mp_obj_get_uint(args[2]),
        (uint8_t)mp_obj_get_uint(args[3]));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_color_sensor_set_light_obj, 4, 4, lpf2_color_sensor_set_light);

static mp_obj_t lpf2_color_sensor_set_mode(size_t n_args, const mp_obj_t *args)
{
    uint8_t modeNum = (uint8_t)mp_obj_get_uint(args[1]);
    float   delta   = n_args > 2 ? mp_obj_get_float(args[2]) : 1.0f;
    GET_CS_CPP(args[0])->setMode(modeNum, delta);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_color_sensor_set_mode_obj, 2, 3, lpf2_color_sensor_set_mode);

static mp_obj_t lpf2_color_sensor_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_CS_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_get_device_type_obj, lpf2_color_sensor_get_device_type);

static mp_obj_t lpf2_color_sensor_name(mp_obj_t self_in)
{
    const char *n = GET_CS_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_sensor_name_obj, lpf2_color_sensor_name);

static const mp_rom_map_elem_t lpf2_color_sensor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),         MP_ROM_PTR(&lpf2_color_sensor_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_getColorIdx),     MP_ROM_PTR(&lpf2_color_sensor_get_color_idx_obj)},
    {MP_ROM_QSTR(MP_QSTR_getReflectivity), MP_ROM_PTR(&lpf2_color_sensor_get_reflectivity_obj)},
    {MP_ROM_QSTR(MP_QSTR_getAmbientLight), MP_ROM_PTR(&lpf2_color_sensor_get_ambient_light_obj)},
    {MP_ROM_QSTR(MP_QSTR_getRGB),          MP_ROM_PTR(&lpf2_color_sensor_get_rgb_obj)},
    {MP_ROM_QSTR(MP_QSTR_getHSV),          MP_ROM_PTR(&lpf2_color_sensor_get_hsv_obj)},
    {MP_ROM_QSTR(MP_QSTR_setLight),        MP_ROM_PTR(&lpf2_color_sensor_set_light_obj)},
    {MP_ROM_QSTR(MP_QSTR_setMode),         MP_ROM_PTR(&lpf2_color_sensor_set_mode_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType),   MP_ROM_PTR(&lpf2_color_sensor_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),            MP_ROM_PTR(&lpf2_color_sensor_name_obj)},
    {MP_ROM_QSTR(MP_QSTR_MODE_COLOR),      MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_COLOR)},
    {MP_ROM_QSTR(MP_QSTR_MODE_REFLT),      MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_REFLT)},
    {MP_ROM_QSTR(MP_QSTR_MODE_AMBI),       MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_AMBI)},
    {MP_ROM_QSTR(MP_QSTR_MODE_LIGHT),      MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_LIGHT)},
    {MP_ROM_QSTR(MP_QSTR_MODE_RGB),        MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_RGB)},
    {MP_ROM_QSTR(MP_QSTR_MODE_HSV),        MP_ROM_INT(Lpf2::Devices::TechnicColorSensor::MODE_HSV)},
};
static MP_DEFINE_CONST_DICT(lpf2_color_sensor_locals_dict, lpf2_color_sensor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_color_sensor_type,
    MP_QSTR_color_sensor,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_color_sensor_locals_dict
);

#undef SELF_TYPE
#undef GET_CS_CPP

/* ------------------------------------------------------------------ */
/* distance_sensor                                                      */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_distance_sensor_t
#define GET_DS_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::TechnicDistanceSensor, SELF_TYPE>(self_in)

DEVICE_DEL(distance_sensor, SELF_TYPE);

static mp_obj_t lpf2_distance_sensor_set_light(size_t n_args, const mp_obj_t *args)
{
    GET_DS_CPP(args[0])->setLight(
        (uint8_t)mp_obj_get_uint(args[1]),
        (uint8_t)mp_obj_get_uint(args[2]),
        (uint8_t)mp_obj_get_uint(args[3]),
        (uint8_t)mp_obj_get_uint(args[4]));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_distance_sensor_set_light_obj, 5, 5, lpf2_distance_sensor_set_light);

static mp_obj_t lpf2_distance_sensor_get_distance(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_DS_CPP(self_in)->getDistance());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_distance_sensor_get_distance_obj, lpf2_distance_sensor_get_distance);

static mp_obj_t lpf2_distance_sensor_set_mode(size_t n_args, const mp_obj_t *args)
{
    uint8_t modeNum = (uint8_t)mp_obj_get_uint(args[1]);
    float   delta   = n_args > 2 ? mp_obj_get_float(args[2]) : 1.0f;
    GET_DS_CPP(args[0])->setMode(modeNum, delta);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_distance_sensor_set_mode_obj, 2, 3, lpf2_distance_sensor_set_mode);

static mp_obj_t lpf2_distance_sensor_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_DS_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_distance_sensor_get_device_type_obj, lpf2_distance_sensor_get_device_type);

static mp_obj_t lpf2_distance_sensor_name(mp_obj_t self_in)
{
    const char *n = GET_DS_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_distance_sensor_name_obj, lpf2_distance_sensor_name);

static const mp_rom_map_elem_t lpf2_distance_sensor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_distance_sensor_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_setLight),      MP_ROM_PTR(&lpf2_distance_sensor_set_light_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDistance),   MP_ROM_PTR(&lpf2_distance_sensor_get_distance_obj)},
    {MP_ROM_QSTR(MP_QSTR_setMode),       MP_ROM_PTR(&lpf2_distance_sensor_set_mode_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_distance_sensor_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_distance_sensor_name_obj)},
    {MP_ROM_QSTR(MP_QSTR_MODE_LIGHT),    MP_ROM_INT(Lpf2::Devices::TechnicDistanceSensor::MODE_LIGHT)},
};
static MP_DEFINE_CONST_DICT(lpf2_distance_sensor_locals_dict, lpf2_distance_sensor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_distance_sensor_type,
    MP_QSTR_distance_sensor,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_distance_sensor_locals_dict
);

#undef SELF_TYPE
#undef GET_DS_CPP

/* ------------------------------------------------------------------ */
/* color_distance_sensor                                                */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_color_distance_sensor_t
#define GET_CDS_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::ColorDistanceSensor, SELF_TYPE>(self_in)

DEVICE_DEL(color_distance_sensor, SELF_TYPE);

static mp_obj_t lpf2_color_distance_sensor_get_color_idx(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_CDS_CPP(self_in)->getColorIdx());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_color_idx_obj, lpf2_color_distance_sensor_get_color_idx);

static mp_obj_t lpf2_color_distance_sensor_get_distance(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_CDS_CPP(self_in)->getDistance());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_distance_obj, lpf2_color_distance_sensor_get_distance);

static mp_obj_t lpf2_color_distance_sensor_get_reflected_light(mp_obj_t self_in)
{
    return mp_obj_new_int(GET_CDS_CPP(self_in)->getReflectivity());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_reflected_light_obj, lpf2_color_distance_sensor_get_reflected_light);

static mp_obj_t lpf2_color_distance_sensor_get_ambient_light(mp_obj_t self_in)
{
    return mp_obj_new_int(GET_CDS_CPP(self_in)->getAmbientLight());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_ambient_light_obj, lpf2_color_distance_sensor_get_ambient_light);

static mp_obj_t lpf2_color_distance_sensor_get_rgb(mp_obj_t self_in)
{
    uint16_t r, g, b;
    GET_CDS_CPP(self_in)->getRGB(r, g, b);
    mp_obj_t tup = mp_obj_new_tuple(3, nullptr);
    mp_obj_tuple_t *t = (mp_obj_tuple_t *)MP_OBJ_TO_PTR(tup);
    t->items[0] = mp_obj_new_int(r);
    t->items[1] = mp_obj_new_int(g);
    t->items[2] = mp_obj_new_int(b);
    return tup;
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_rgb_obj, lpf2_color_distance_sensor_get_rgb);

static mp_obj_t lpf2_color_distance_sensor_set_ir_tx(mp_obj_t self_in, mp_obj_t value_in)
{
    GET_CDS_CPP(self_in)->setIrTx((uint16_t)mp_obj_get_uint(value_in));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_color_distance_sensor_set_ir_tx_obj, lpf2_color_distance_sensor_set_ir_tx);

static mp_obj_t lpf2_color_distance_sensor_set_led_color(mp_obj_t self_in, mp_obj_t color_in)
{
    GET_CDS_CPP(self_in)->setLedColor((Lpf2::ColorIDX)mp_obj_get_uint(color_in));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_color_distance_sensor_set_led_color_obj, lpf2_color_distance_sensor_set_led_color);

static mp_obj_t lpf2_color_distance_sensor_set_mode(size_t n_args, const mp_obj_t *args)
{
    uint8_t modeNum = (uint8_t)mp_obj_get_uint(args[1]);
    float   delta   = n_args > 2 ? mp_obj_get_float(args[2]) : 1.0f;
    GET_CDS_CPP(args[0])->setMode(modeNum, delta);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_color_distance_sensor_set_mode_obj, 2, 3, lpf2_color_distance_sensor_set_mode);

static mp_obj_t lpf2_color_distance_sensor_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_CDS_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_get_device_type_obj, lpf2_color_distance_sensor_get_device_type);

static mp_obj_t lpf2_color_distance_sensor_name(mp_obj_t self_in)
{
    const char *n = GET_CDS_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_color_distance_sensor_name_obj, lpf2_color_distance_sensor_name);

static const mp_rom_map_elem_t lpf2_color_distance_sensor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),          MP_ROM_PTR(&lpf2_color_distance_sensor_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_getColorIdx),      MP_ROM_PTR(&lpf2_color_distance_sensor_get_color_idx_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDistance),      MP_ROM_PTR(&lpf2_color_distance_sensor_get_distance_obj)},
    {MP_ROM_QSTR(MP_QSTR_getReflectedLight),MP_ROM_PTR(&lpf2_color_distance_sensor_get_reflected_light_obj)},
    {MP_ROM_QSTR(MP_QSTR_getAmbientLight),  MP_ROM_PTR(&lpf2_color_distance_sensor_get_ambient_light_obj)},
    {MP_ROM_QSTR(MP_QSTR_getRgb),           MP_ROM_PTR(&lpf2_color_distance_sensor_get_rgb_obj)},
    {MP_ROM_QSTR(MP_QSTR_setIrTx),          MP_ROM_PTR(&lpf2_color_distance_sensor_set_ir_tx_obj)},
    {MP_ROM_QSTR(MP_QSTR_setLedColor),      MP_ROM_PTR(&lpf2_color_distance_sensor_set_led_color_obj)},
    {MP_ROM_QSTR(MP_QSTR_setMode),          MP_ROM_PTR(&lpf2_color_distance_sensor_set_mode_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType),    MP_ROM_PTR(&lpf2_color_distance_sensor_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),             MP_ROM_PTR(&lpf2_color_distance_sensor_name_obj)},
    {MP_ROM_QSTR(MP_QSTR_MODE_COLOR),       MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_COLOR)},
    {MP_ROM_QSTR(MP_QSTR_MODE_DIST),        MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_DIST)},
    {MP_ROM_QSTR(MP_QSTR_MODE_REFLT),       MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_REFLT)},
    {MP_ROM_QSTR(MP_QSTR_MODE_AMBI),        MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_AMBI)},
    {MP_ROM_QSTR(MP_QSTR_MODE_LED),         MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_LED)},
    {MP_ROM_QSTR(MP_QSTR_MODE_RGB),         MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_RGB)},
    {MP_ROM_QSTR(MP_QSTR_MODE_IR),          MP_ROM_INT(Lpf2::Devices::ColorDistanceSensor::MODE_IR)},
};
static MP_DEFINE_CONST_DICT(lpf2_color_distance_sensor_locals_dict, lpf2_color_distance_sensor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_color_distance_sensor_type,
    MP_QSTR_color_distance_sensor,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_color_distance_sensor_locals_dict
);

#undef SELF_TYPE
#undef GET_CDS_CPP

/* ------------------------------------------------------------------ */
/* hub_led                                                              */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_hub_led_t
#define GET_HL_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::HubLED, SELF_TYPE>(self_in)

DEVICE_DEL(hub_led, SELF_TYPE);

static mp_obj_t lpf2_hub_led_set_color_idx(mp_obj_t self_in, mp_obj_t color_in)
{
    GET_HL_CPP(self_in)->setColorIdx((Lpf2::ColorIDX)mp_obj_get_uint(color_in));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_hub_led_set_color_idx_obj, lpf2_hub_led_set_color_idx);

static mp_obj_t lpf2_hub_led_set_color(size_t n_args, const mp_obj_t *args)
{
    GET_HL_CPP(args[0])->setColor(
        (uint8_t)mp_obj_get_uint(args[1]),
        (uint8_t)mp_obj_get_uint(args[2]),
        (uint8_t)mp_obj_get_uint(args[3]));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_hub_led_set_color_obj, 4, 4, lpf2_hub_led_set_color);

static mp_obj_t lpf2_hub_led_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_HL_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_hub_led_get_device_type_obj, lpf2_hub_led_get_device_type);

static mp_obj_t lpf2_hub_led_name(mp_obj_t self_in)
{
    const char *n = GET_HL_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_hub_led_name_obj, lpf2_hub_led_name);

static const mp_rom_map_elem_t lpf2_hub_led_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_hub_led_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_setColorIdx),   MP_ROM_PTR(&lpf2_hub_led_set_color_idx_obj)},
    {MP_ROM_QSTR(MP_QSTR_setColor),      MP_ROM_PTR(&lpf2_hub_led_set_color_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_hub_led_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_hub_led_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_hub_led_locals_dict, lpf2_hub_led_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_hub_led_type,
    MP_QSTR_hub_led,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_hub_led_locals_dict
);

#undef SELF_TYPE
#undef GET_HL_CPP

/* ------------------------------------------------------------------ */
/* accelerometer                                                        */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_accelerometer_t
#define GET_ACC_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::HubAccelerometer, SELF_TYPE>(self_in)

DEVICE_DEL(accelerometer, SELF_TYPE);

static mp_obj_t lpf2_accelerometer_get_x(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_ACC_CPP(self_in)->getX());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_accelerometer_get_x_obj, lpf2_accelerometer_get_x);

static mp_obj_t lpf2_accelerometer_get_y(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_ACC_CPP(self_in)->getY());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_accelerometer_get_y_obj, lpf2_accelerometer_get_y);

static mp_obj_t lpf2_accelerometer_get_z(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_ACC_CPP(self_in)->getZ());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_accelerometer_get_z_obj, lpf2_accelerometer_get_z);

static mp_obj_t lpf2_accelerometer_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_ACC_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_accelerometer_get_device_type_obj, lpf2_accelerometer_get_device_type);

static mp_obj_t lpf2_accelerometer_name(mp_obj_t self_in)
{
    const char *n = GET_ACC_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_accelerometer_name_obj, lpf2_accelerometer_name);

static const mp_rom_map_elem_t lpf2_accelerometer_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_accelerometer_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_getX),          MP_ROM_PTR(&lpf2_accelerometer_get_x_obj)},
    {MP_ROM_QSTR(MP_QSTR_getY),          MP_ROM_PTR(&lpf2_accelerometer_get_y_obj)},
    {MP_ROM_QSTR(MP_QSTR_getZ),          MP_ROM_PTR(&lpf2_accelerometer_get_z_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_accelerometer_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_accelerometer_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_accelerometer_locals_dict, lpf2_accelerometer_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_accelerometer_type,
    MP_QSTR_accelerometer,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_accelerometer_locals_dict
);

#undef SELF_TYPE
#undef GET_ACC_CPP

/* ------------------------------------------------------------------ */
/* gyroscope                                                            */
/* ------------------------------------------------------------------ */

#define SELF_TYPE mp_obj_lpf2_gyroscope_t
#define GET_GYR_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::HubGyroscope, SELF_TYPE>(self_in)

DEVICE_DEL(gyroscope, SELF_TYPE);

static mp_obj_t lpf2_gyroscope_get_x(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_GYR_CPP(self_in)->getX());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_gyroscope_get_x_obj, lpf2_gyroscope_get_x);

static mp_obj_t lpf2_gyroscope_get_y(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_GYR_CPP(self_in)->getY());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_gyroscope_get_y_obj, lpf2_gyroscope_get_y);

static mp_obj_t lpf2_gyroscope_get_z(mp_obj_t self_in)
{
    return mp_obj_new_float(GET_GYR_CPP(self_in)->getZ());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_gyroscope_get_z_obj, lpf2_gyroscope_get_z);

static mp_obj_t lpf2_gyroscope_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_GYR_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_gyroscope_get_device_type_obj, lpf2_gyroscope_get_device_type);

static mp_obj_t lpf2_gyroscope_name(mp_obj_t self_in)
{
    const char *n = GET_GYR_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_gyroscope_name_obj, lpf2_gyroscope_name);

static const mp_rom_map_elem_t lpf2_gyroscope_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_gyroscope_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_getX),          MP_ROM_PTR(&lpf2_gyroscope_get_x_obj)},
    {MP_ROM_QSTR(MP_QSTR_getY),          MP_ROM_PTR(&lpf2_gyroscope_get_y_obj)},
    {MP_ROM_QSTR(MP_QSTR_getZ),          MP_ROM_PTR(&lpf2_gyroscope_get_z_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_gyroscope_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_gyroscope_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_gyroscope_locals_dict, lpf2_gyroscope_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_gyroscope_type,
    MP_QSTR_gyroscope,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_gyroscope_locals_dict
);

#undef SELF_TYPE
#undef GET_GYR_CPP

/* ------------------------------------------------------------------ */
/* port_expander_dev (hardware)                                         */
/* ------------------------------------------------------------------ */

#if LPF2_HAS_PORT_EXPANDER
#define SELF_TYPE mp_obj_lpf2_port_expander_dev_t
#define GET_PE_CPP(self_in) lpf2_device_get_cpp<Lpf2::Devices::PortExpander, SELF_TYPE>(self_in)

DEVICE_DEL(port_expander_dev, SELF_TYPE);

static mp_obj_t lpf2_port_expander_dev_get_port(mp_obj_t self_in, mp_obj_t port_num_in)
{
    uint8_t num = (uint8_t)mp_obj_get_uint(port_num_in);
    Lpf2::Port *p = GET_PE_CPP(self_in)->getPort((Lpf2::PortNum)num);
    if (!p)
        mp_raise_ValueError(MP_ERROR_TEXT("invalid port number"));

    mp_obj_lpf2_port_t *o = m_new_obj(mp_obj_lpf2_port_t);
    o->base.type = &lpf2_port_type;
    o->cpp_obj = p;
    o->owned = false;
    o->is_trampoline = false;
    o->parent_ref = self_in;
    return MP_OBJ_FROM_PTR(o);
}
static MP_DEFINE_CONST_FUN_OBJ_2(lpf2_port_expander_dev_get_port_obj, lpf2_port_expander_dev_get_port);

static mp_obj_t lpf2_port_expander_dev_get_device_type(mp_obj_t self_in)
{
    return mp_obj_new_int((int)GET_PE_CPP(self_in)->getDeviceType());
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_port_expander_dev_get_device_type_obj, lpf2_port_expander_dev_get_device_type);

static mp_obj_t lpf2_port_expander_dev_name(mp_obj_t self_in)
{
    const char *n = GET_PE_CPP(self_in)->name();
    return mp_obj_new_str(n, strlen(n));
}
static MP_DEFINE_CONST_FUN_OBJ_1(lpf2_port_expander_dev_name_obj, lpf2_port_expander_dev_name);

static const mp_rom_map_elem_t lpf2_port_expander_dev_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__),       MP_ROM_PTR(&lpf2_port_expander_dev_del_obj)},
    {MP_ROM_QSTR(MP_QSTR_getPort),       MP_ROM_PTR(&lpf2_port_expander_dev_get_port_obj)},
    {MP_ROM_QSTR(MP_QSTR_getDeviceType), MP_ROM_PTR(&lpf2_port_expander_dev_get_device_type_obj)},
    {MP_ROM_QSTR(MP_QSTR_name),          MP_ROM_PTR(&lpf2_port_expander_dev_name_obj)},
};
static MP_DEFINE_CONST_DICT(lpf2_port_expander_dev_locals_dict, lpf2_port_expander_dev_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_port_expander_type,
    MP_QSTR_port_expander,
    MP_TYPE_FLAG_NONE,
    locals_dict, &lpf2_port_expander_dev_locals_dict
);

#undef SELF_TYPE
#undef GET_PE_CPP
#endif // LPF2_HAS_PORT_EXPANDER

/* ------------------------------------------------------------------ */
/* registerDefault                                                      */
/* ------------------------------------------------------------------ */

static mp_obj_t lpf2_devices_register_default(void)
{
    Lpf2::DeviceRegistry::registerDefault();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_devices_register_default_obj, lpf2_devices_register_default);

} // extern "C"
