extern "C" {
#include "py/runtime.h"
#include "py/objtype.h"
}

#include "mod_types.h"
#include "Lpf2/Battery.hpp"

extern "C" {

// ---- voltage getters/setters ------------------------------------------------

static mp_obj_t lpf2_battery_set_max_voltage(mp_obj_t mv_in)
{
    Lpf2::Battery::setMaxVoltage((uint16_t)mp_obj_get_int(mv_in));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(lpf2_battery_set_max_voltage_obj, lpf2_battery_set_max_voltage);

static mp_obj_t lpf2_battery_set_min_voltage(mp_obj_t mv_in)
{
    Lpf2::Battery::setMinVoltage((uint16_t)mp_obj_get_int(mv_in));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(lpf2_battery_set_min_voltage_obj, lpf2_battery_set_min_voltage);

static mp_obj_t lpf2_battery_get_max_voltage(void)
{
    return mp_obj_new_int_from_uint(Lpf2::Battery::getMaxVoltage());
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_battery_get_max_voltage_obj, lpf2_battery_get_max_voltage);

static mp_obj_t lpf2_battery_get_min_voltage(void)
{
    return mp_obj_new_int_from_uint(Lpf2::Battery::getMinVoltage());
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_battery_get_min_voltage_obj, lpf2_battery_get_min_voltage);

static mp_obj_t lpf2_battery_set_current_voltage(mp_obj_t mv_in)
{
    Lpf2::Battery::setCurrentVoltage((uint16_t)mp_obj_get_int(mv_in));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(lpf2_battery_set_current_voltage_obj, lpf2_battery_set_current_voltage);

static mp_obj_t lpf2_battery_get_current_voltage(void)
{
    return mp_obj_new_int_from_uint(Lpf2::Battery::getCurrentVoltage());
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_battery_get_current_voltage_obj, lpf2_battery_get_current_voltage);

static mp_obj_t lpf2_battery_get_percent(void)
{
    return mp_obj_new_int_from_uint(Lpf2::Battery::getPercent());
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_battery_get_percent_obj, lpf2_battery_get_percent);

// ---- percent function trampoline -------------------------------------------

MP_REGISTER_ROOT_POINTER(mp_obj_t lpf2_battery_percent_cb);

static uint8_t lpf2_battery_percent_trampoline(uint16_t mV, uint16_t vmin, uint16_t vmax)
{
    mp_obj_t cb = MP_STATE_PORT(lpf2_battery_percent_cb);
    if (cb == MP_OBJ_NULL || cb == mp_const_none) {
        return 0;
    }
    mp_obj_t args[3] = {
        mp_obj_new_int_from_uint(mV),
        mp_obj_new_int_from_uint(vmin),
        mp_obj_new_int_from_uint(vmax),
    };
    mp_obj_t res = mp_call_function_n_kw(cb, 3, 0, args);
    int v = mp_obj_get_int(res);
    if (v < 0) v = 0;
    if (v > 100) v = 100;
    return (uint8_t)v;
}

static mp_obj_t lpf2_battery_set_percent_func(mp_obj_t fn_in)
{
    if (fn_in == mp_const_none) {
        MP_STATE_PORT(lpf2_battery_percent_cb) = mp_const_none;
        Lpf2::Battery::setPercentFunc(nullptr);
    } else {
        MP_STATE_PORT(lpf2_battery_percent_cb) = fn_in;
        Lpf2::Battery::setPercentFunc(lpf2_battery_percent_trampoline);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(lpf2_battery_set_percent_func_obj, lpf2_battery_set_percent_func);

// ---- ADC --------------------------------------------------------------------

static mp_obj_t lpf2_battery_setup_adc_divider(size_t n_args, const mp_obj_t *args)
{
    Lpf2::Battery::AdcConfig cfg{};
    cfg.adc_channel   = mp_obj_get_int(args[0]);
    cfg.adc_unit      = mp_obj_get_int(args[1]);
    cfg.r_top_ohms    = mp_obj_get_float(args[2]);
    cfg.r_bottom_ohms = mp_obj_get_float(args[3]);
    if (n_args >= 5) cfg.vref_mv = (uint16_t)mp_obj_get_int(args[4]);
    if (n_args >= 6) cfg.samples = (uint8_t)mp_obj_get_int(args[5]);
    return mp_obj_new_bool(Lpf2::Battery::setupAdcDivider(cfg));
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_battery_setup_adc_divider_obj, 4, 6, lpf2_battery_setup_adc_divider);

static mp_obj_t lpf2_battery_read_voltage(void)
{
    return mp_obj_new_int_from_uint(Lpf2::Battery::readBatteryVoltage());
}
MP_DEFINE_CONST_FUN_OBJ_0(lpf2_battery_read_voltage_obj, lpf2_battery_read_voltage);

// ---- module -----------------------------------------------------------------

LPF2_DEFINE_MOD_WITH_GLOB(battery,
    { MP_ROM_QSTR(MP_QSTR_setMaxVoltage),     MP_ROM_PTR(&lpf2_battery_set_max_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_setMinVoltage),     MP_ROM_PTR(&lpf2_battery_set_min_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_getMaxVoltage),     MP_ROM_PTR(&lpf2_battery_get_max_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_getMinVoltage),     MP_ROM_PTR(&lpf2_battery_get_min_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_setCurrentVoltage), MP_ROM_PTR(&lpf2_battery_set_current_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_getCurrentVoltage), MP_ROM_PTR(&lpf2_battery_get_current_voltage_obj) },
    { MP_ROM_QSTR(MP_QSTR_getPercent),        MP_ROM_PTR(&lpf2_battery_get_percent_obj) },
    { MP_ROM_QSTR(MP_QSTR_setPercentFunc),    MP_ROM_PTR(&lpf2_battery_set_percent_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_setupAdcDivider),   MP_ROM_PTR(&lpf2_battery_setup_adc_divider_obj) },
    { MP_ROM_QSTR(MP_QSTR_readBatteryVoltage),MP_ROM_PTR(&lpf2_battery_read_voltage_obj) }
);

} // extern "C"
