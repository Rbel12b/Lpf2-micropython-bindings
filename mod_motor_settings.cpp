extern "C" {
#include "py/runtime.h"
#include "py/objtype.h"
}

#include "mod_types.h"

extern "C" {

typedef struct _mp_obj_lpf2_motor_settings_t {
    mp_obj_base_t base;
    Lpf2::Local::MotorSettings *cpp_obj;
} mp_obj_lpf2_motor_settings_t;

#define SELF_TYPE mp_obj_lpf2_motor_settings_t

#define MS_LOAD_STORE_FLOAT(field) \
    case MP_QSTR_##field: { \
        if (dest[0] == MP_OBJ_NULL) { \
            dest[0] = mp_obj_new_float(self->field); \
        } else { \
            self->field = mp_obj_get_float(dest[1]); \
            dest[0] = MP_OBJ_NULL; \
        } \
        break; \
    }

#define MS_LOAD_STORE_INT(field) \
    case MP_QSTR_##field: { \
        if (dest[0] == MP_OBJ_NULL) { \
            dest[0] = mp_obj_new_int(self->field); \
        } else { \
            self->field = mp_obj_get_int(dest[1]); \
            dest[0] = MP_OBJ_NULL; \
        } \
        break; \
    }

static void lpf2_motor_settings_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    auto self = GET_SELF_CPP();

    switch (attr)
    {
    case MP_QSTR_id: {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_int((int)self->id);
        } else {
            self->id = (Lpf2::DeviceType)mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    MS_LOAD_STORE_INT(rated_max_speed);
    MS_LOAD_STORE_INT(max_voltage_mv);
    MS_LOAD_STORE_FLOAT(speed_ksp);
    MS_LOAD_STORE_FLOAT(speed_ksi);
    MS_LOAD_STORE_FLOAT(speed_int_clamp);
    MS_LOAD_STORE_FLOAT(speed_deadband_pct);
    MS_LOAD_STORE_FLOAT(pos_kp);
    MS_LOAD_STORE_FLOAT(pos_ki);
    MS_LOAD_STORE_FLOAT(pos_kd);
    MS_LOAD_STORE_FLOAT(pos_int_clamp);
    MS_LOAD_STORE_FLOAT(pos_deadband_deg);
    case MP_QSTR_pos_decel_mdps2: {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_float((mp_float_t)self->pos_decel_mdps2);
        } else {
            self->pos_decel_mdps2 = (double)mp_obj_get_float(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    MS_LOAD_STORE_FLOAT(pos_handoff_deg);
    MS_LOAD_STORE_FLOAT(breakaway_pct);
    MS_LOAD_STORE_FLOAT(kinetic_floor_pct);

    default:
        dest[1] = MP_OBJ_SENTINEL;
        break;
    }
}

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_motor_settings_type,
    MP_QSTR_motor_settings,
    MP_TYPE_FLAG_NONE,
    attr, (void*)lpf2_motor_settings_attr
);

#define MS_WRAPPER(name, cpp_name) \
    const mp_obj_lpf2_motor_settings_t lpf2_motor_settings_##name##_obj = { \
        { &lpf2_motor_settings_type }, \
        &Lpf2::Local::cpp_name, \
    }

MS_WRAPPER(medium_linear,                MS_MEDIUM_LINEAR_MOTOR);
MS_WRAPPER(technic_large_linear,         MS_TECHNIC_LARGE_LINEAR_MOTOR);
MS_WRAPPER(technic_xlarge_linear,        MS_TECHNIC_XLARGE_LINEAR_MOTOR);
MS_WRAPPER(technic_medium_angular,       MS_TECHNIC_MEDIUM_ANGULAR_MOTOR);
MS_WRAPPER(technic_large_angular,        MS_TECHNIC_LARGE_ANGULAR_MOTOR);
MS_WRAPPER(technic_medium_angular_grey,  MS_TECHNIC_MEDIUM_ANGULAR_MOTOR_GREY);
MS_WRAPPER(technic_large_angular_grey,   MS_TECHNIC_LARGE_ANGULAR_MOTOR_GREY);

#define MS_ENTRY(name) \
    { MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_PTR(&lpf2_motor_settings_##name##_obj) }

LPF2_DEFINE_MOD_WITH_GLOB(motor_setting,
    MS_ENTRY(medium_linear),
    MS_ENTRY(technic_large_linear),
    MS_ENTRY(technic_xlarge_linear),
    MS_ENTRY(technic_medium_angular),
    MS_ENTRY(technic_large_angular),
    MS_ENTRY(technic_medium_angular_grey),
    MS_ENTRY(technic_large_angular_grey)
);

#undef SELF_TYPE

} // extern "C"
