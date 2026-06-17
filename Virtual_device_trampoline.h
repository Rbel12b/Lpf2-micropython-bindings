#pragma once

extern "C" {
#include "py/runtime.h"
}

#include "Lpf2/Virtual/Device.hpp"

class VirtualDeviceTrampoline : public Lpf2::Virtual::GenericDevice
{
public:
    mp_obj_t self;

    VirtualDeviceTrampoline(mp_obj_t self_in, const Lpf2::DeviceDescriptor &desc)
        : Lpf2::Virtual::GenericDevice(desc), self(self_in) {}

    void startPower(int8_t pw) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_startPower);
        mp_call_function_1(method, mp_obj_new_int(pw));
    }

    void setAccTime(uint16_t accTime, Lpf2::AccelerationProfile accProfile = 1) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_setAccTime);
        mp_obj_t args[2] = {
            mp_obj_new_int(accTime),
            mp_obj_new_int(accProfile)};
        mp_call_function_n_kw(method, 2, 0, args);
    }

    void setDecTime(uint16_t decTime, Lpf2::AccelerationProfile decProfile = 1) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_setDecTime);
        mp_obj_t args[2] = {
            mp_obj_new_int(decTime),
            mp_obj_new_int(decProfile)};
        mp_call_function_n_kw(method, 2, 0, args);
    }

    void startSpeed(int8_t speed, uint8_t maxPower, uint8_t useProfile = 0) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_startSpeed);
        mp_obj_t args[3] = {
            mp_obj_new_int(speed),
            mp_obj_new_int(maxPower),
            mp_obj_new_int(useProfile)};
        mp_call_function_n_kw(method, 3, 0, args);
    }

    void startSpeedForTime(uint16_t time, int8_t speed, uint8_t maxPower,
                           Lpf2::BrakingStyle endState, uint8_t useProfile = 0) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_startSpeedForTime);
        mp_obj_t args[5] = {
            mp_obj_new_int(time),
            mp_obj_new_int(speed),
            mp_obj_new_int(maxPower),
            mp_obj_new_int((uint8_t)endState),
            mp_obj_new_int(useProfile)};
        mp_call_function_n_kw(method, 5, 0, args);
    }

    void startSpeedForDegrees(uint32_t degrees, int8_t speed, uint8_t maxPower,
                              Lpf2::BrakingStyle endState, uint8_t useProfile = 0) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_startSpeedForDegrees);
        mp_obj_t args[5] = {
            mp_obj_new_int_from_uint(degrees),
            mp_obj_new_int(speed),
            mp_obj_new_int(maxPower),
            mp_obj_new_int((uint8_t)endState),
            mp_obj_new_int(useProfile)};
        mp_call_function_n_kw(method, 5, 0, args);
    }

    void gotoAbsPosition(int32_t absPos, uint8_t speed, uint8_t maxPower,
                         Lpf2::BrakingStyle endState, uint8_t useProfile = 0) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_gotoAbsPosition);
        mp_obj_t args[5] = {
            mp_obj_new_int(absPos),
            mp_obj_new_int(speed),
            mp_obj_new_int(maxPower),
            mp_obj_new_int((uint8_t)endState),
            mp_obj_new_int(useProfile)};
        mp_call_function_n_kw(method, 5, 0, args);
    }

    void presetEncoder(int32_t pos) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_presetEncoder);
        mp_call_function_1(method, mp_obj_new_int(pos));
    }

    int writeData(uint8_t mode, const std::vector<uint8_t> &data) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_writeData);
        mp_obj_t args[2] = {
            mp_obj_new_int(mode),
            mp_obj_new_bytes(data.data(), data.size())};
        mp_obj_t ret = mp_call_function_n_kw(method, 2, 0, args);
        return mp_obj_get_int(ret);
    }

    int setMode(uint8_t mode, float delta = 1.0f) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_setMode);
        mp_obj_t ret = mp_call_function_2(method, mp_obj_new_int(mode), mp_obj_new_float(delta));
        return mp_obj_get_int(ret);
    }

    int setModeCombo(uint8_t idx, const std::vector<float>& deltas = {}) override
    {
        mp_obj_t method = mp_load_attr(self, MP_QSTR_setModeCombo);
        mp_obj_t tup = mp_obj_new_tuple(deltas.size(), nullptr);
        mp_obj_tuple_t *t = (mp_obj_tuple_t*)MP_OBJ_TO_PTR(tup);
        for (size_t i = 0; i < deltas.size(); ++i) {
            t->items[i] = mp_obj_new_float(deltas[i]);
        }
        mp_obj_t ret = mp_call_function_2(method, mp_obj_new_int(idx), tup);
        return mp_obj_get_int(ret);
    }
};
