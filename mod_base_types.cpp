extern "C" {
#include "py/runtime.h"
#include "py/binary.h"
}

#define LOAD_STORE_VAL(val, type) \
    if (dest[0] == MP_OBJ_NULL) {\
        dest[0] = mp_obj_new_##type(val); \
    } else { \
        val = mp_obj_get_##type(dest[1]); \
        dest[0] = MP_OBJ_NULL; \
    }

#define LOAD_STORE_VAL_2(val, get, set) \
    if (dest[0] == MP_OBJ_NULL) {\
        dest[0] = mp_obj_new_##set(val); \
    } else { \
        val = mp_obj_get_##get(dest[1]); \
        dest[0] = MP_OBJ_NULL; \
    }

#define CASE_VAL_NAME(name, type) \
    case MP_QSTR_##name:{ \
        LOAD_STORE_VAL(self->name, type); \
        break; \
    }

#define CASE_VAL_NAME_2(name, get, set) \
    case MP_QSTR_##name:{ \
        LOAD_STORE_VAL_2(self->name, get, set); \
        break; \
    }

#define CASE_VAL_NAME_MEMBER(name, member, type) \
    case MP_QSTR_##name:{ \
        LOAD_STORE_VAL(self->member, type); \
        break; \
    }

#define CASE_VAL_NAME_MEMBER_2(name, member, get, set) \
    case MP_QSTR_##name:{ \
        LOAD_STORE_VAL_2(self->member, get, set); \
        break; \
    }

#include "mod_types.h"
extern "C" {
#define SELF_TYPE mp_obj_lpf2_mode_t

static mp_obj_t lpf2_mode_make_new(const mp_obj_type_t *type,
                                    size_t n_args,
                                    size_t n_kw,
                                    const mp_obj_t *args)
{

    SELF_TYPE *o = (SELF_TYPE*)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;

    o->cpp_obj = new Lpf2::Mode;
    o->owned = true;

    return MP_OBJ_FROM_PTR(o);
}

static void lpf2_mode_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    auto self = GET_SELF_CPP();

    switch (attr)
    {
    case MP_QSTR_name:
    {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_str(self->name.c_str(), self->name.length());
        } else {
            size_t len;
            auto s = mp_obj_str_get_data(dest[1], &len);
            self->name = std::string(s, len);
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    CASE_VAL_NAME(min, float);
    CASE_VAL_NAME(max, float);
    CASE_VAL_NAME(PCTmin, float);
    CASE_VAL_NAME(PCTmax, float);
    CASE_VAL_NAME(SImin, float);
    CASE_VAL_NAME(SImax, float);
    CASE_VAL_NAME_MEMBER_2(inMapping, in.val, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(outMapping, out.val, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(dataSets, data_sets, uint, int_from_uint);
    CASE_VAL_NAME_2(format, uint, int_from_uint);
    CASE_VAL_NAME_2(figures, uint, int_from_uint);
    CASE_VAL_NAME_2(decimals, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(motorBias, motor_bias, uint, int_from_uint);
    case MP_QSTR_flags:
    {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_int((*((uint64_t*)self->flags.bytes)));
        } else {
            (*((uint64_t*)self->flags.bytes)) = mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    case MP_QSTR_rawData:
    {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_bytes(self->rawData.data(), self->rawData.size());
        } else {
            mp_buffer_info_t buf;
            mp_get_buffer_raise(dest[1], &buf, MP_BUFFER_READ);
            self->rawData = std::vector<uint8_t>(
                (uint8_t*)buf.buf,
                (uint8_t*)buf.buf + buf.len
            );
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    
    default:
        dest[1] = MP_OBJ_SENTINEL;
        break;
    }
}

LPF2_DEFINE_METHOD(mode_del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting Mode, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

static const mp_rom_map_elem_t lpf2_mode_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&LPF2_GET_METHOD_OBJ(mode_del))},
};

static MP_DEFINE_CONST_DICT(lpf2_mode_locals_dict, lpf2_mode_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_mode_type, 
    MP_QSTR_mode,
    MP_TYPE_FLAG_NONE,
    make_new, (void*)lpf2_mode_make_new,
    attr, (void*)lpf2_mode_attr,
    locals_dict, &lpf2_mode_locals_dict
);

#undef SELF_TYPE
#define SELF_TYPE mp_obj_lpf2_version_t

static mp_obj_t lpf2_version_make_new(const mp_obj_type_t *type,
                                    size_t n_args,
                                    size_t n_kw,
                                    const mp_obj_t *args)
{

    SELF_TYPE *o = (SELF_TYPE*)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;

    o->cpp_obj = new Lpf2::Version;
    o->owned = true;

    return MP_OBJ_FROM_PTR(o);
}

static void lpf2_version_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    auto self = GET_SELF_CPP();

    switch (attr)
    {
    CASE_VAL_NAME_MEMBER_2(build, Build, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(major, Major, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(minor, Minor, uint, int_from_uint);
    CASE_VAL_NAME_MEMBER_2(bugfix, Bugfix, uint, int_from_uint);
    
    default:
        dest[1] = MP_OBJ_SENTINEL;
        break;
    }
}

LPF2_DEFINE_METHOD(version_del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting Version, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

static const mp_rom_map_elem_t lpf2_version_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&LPF2_GET_METHOD_OBJ(version_del))},
};

static MP_DEFINE_CONST_DICT(lpf2_version_locals_dict, lpf2_version_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_version_type, 
    MP_QSTR_version,
    MP_TYPE_FLAG_NONE,
    make_new, (void*)lpf2_version_make_new,
    attr, (void*)lpf2_version_attr,
    locals_dict, &lpf2_version_locals_dict
);

#undef SELF_TYPE
#define SELF_TYPE mp_obj_lpf2_device_descriptor_t

static mp_obj_t lpf2_device_descriptor_make_new(const mp_obj_type_t *type,
                                    size_t n_args,
                                    size_t n_kw,
                                    const mp_obj_t *args)
{

    SELF_TYPE *o = (SELF_TYPE*)m_malloc_with_finaliser(sizeof(SELF_TYPE));
    o->base.type = type;

    o->cpp_obj = new Lpf2::DeviceDescriptor;
    o->owned = true;

    return MP_OBJ_FROM_PTR(o);
}

static void lpf2_device_descriptor_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    auto self = GET_SELF_CPP();
    auto self_c = GET_SELF();

    switch (attr)
    {
    CASE_VAL_NAME_2(inModesMask, uint, int_from_uint);
    CASE_VAL_NAME_2(outModesMask, uint, int_from_uint);
    CASE_VAL_NAME_2(caps, uint, int_from_uint);
    case MP_QSTR_type:
    {
        if (dest[0] == MP_OBJ_NULL) {
            dest[0] = mp_obj_new_int_from_uint((uint8_t)self->type);
        } else {
            self->type = (Lpf2::DeviceType)mp_obj_get_uint(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    case MP_QSTR_combos:
    {
        if (dest[0] == MP_OBJ_NULL) {
            if (!self_c->combo_list)
            {
                mp_obj_list_t *list = (mp_obj_list_t*)mp_obj_new_list(self->combos.size(), NULL);

                for (size_t i = 0; i < self->combos.size(); ++i) {
                    list->items[i] = mp_obj_new_int(self->combos[i]);
                }
                self_c->combo_list = MP_OBJ_FROM_PTR(list);
            }
            dest[0] = self_c->combo_list;
        } else {
            mp_obj_t *items;
            size_t len;

            mp_obj_get_array(dest[1], &len, &items);

            std::vector<uint16_t> vec;
            vec.reserve(len);

            for (size_t i = 0; i < len; ++i) {
                vec.push_back((uint16_t)mp_obj_get_int(items[i]));
            }
            self->combos = vec;
            dest[0] = MP_OBJ_NULL;
            mp_obj_list_t *list = (mp_obj_list_t*)mp_obj_new_list(self->combos.size(), NULL);

            for (size_t i = 0; i < self->combos.size(); ++i) {
                list->items[i] = mp_obj_new_int(self->combos[i]);
            }
            self_c->combo_list = MP_OBJ_FROM_PTR(list);
        }
        break;
    }
    case MP_QSTR_fwVersion:
    {
        if (dest[0] == MP_OBJ_NULL) {
            mp_obj_lpf2_version_t *v = (mp_obj_lpf2_version_t*)m_malloc_with_finaliser(sizeof(mp_obj_lpf2_version_t));
            v->base.type = &lpf2_version_type;
            v->cpp_obj = new Lpf2::Version(self->fwVersion);
            v->owned = true;
            dest[0] = MP_OBJ_FROM_PTR(v);
        } else {
            mp_obj_t native_v = lpf2_cast_to_native_base(dest[1], &lpf2_version_type);
            if (native_v == MP_OBJ_NULL)
                mp_raise_TypeError(MP_ERROR_TEXT("expected version"));
            self->fwVersion = *((mp_obj_lpf2_version_t*)MP_OBJ_TO_PTR(native_v))->cpp_obj;
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    case MP_QSTR_hwVersion:
    {
        if (dest[0] == MP_OBJ_NULL) {
            mp_obj_lpf2_version_t *v = (mp_obj_lpf2_version_t*)m_malloc_with_finaliser(sizeof(mp_obj_lpf2_version_t));
            v->base.type = &lpf2_version_type;
            v->cpp_obj = new Lpf2::Version(self->hwVersion);
            v->owned = true;
            dest[0] = MP_OBJ_FROM_PTR(v);
        } else {
            mp_obj_t native_v = lpf2_cast_to_native_base(dest[1], &lpf2_version_type);
            if (native_v == MP_OBJ_NULL)
                mp_raise_TypeError(MP_ERROR_TEXT("expected version"));
            self->hwVersion = *((mp_obj_lpf2_version_t*)MP_OBJ_TO_PTR(native_v))->cpp_obj;
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    case MP_QSTR_modes:
    {
        if (dest[0] == MP_OBJ_NULL) {
            if (!self_c->mode_list) {
                mp_obj_list_t *list = (mp_obj_list_t*)mp_obj_new_list(self->modes.size(), NULL);
                for (size_t i = 0; i < self->modes.size(); ++i) {
                    mp_obj_lpf2_mode_t *m = (mp_obj_lpf2_mode_t*)m_malloc_with_finaliser(sizeof(mp_obj_lpf2_mode_t));
                    m->base.type = &lpf2_mode_type;
                    m->cpp_obj = new Lpf2::Mode(self->modes[i]);
                    m->owned = true;
                    list->items[i] = MP_OBJ_FROM_PTR(m);
                }
                self_c->mode_list = MP_OBJ_FROM_PTR(list);
            }
            dest[0] = self_c->mode_list;
        } else {
            mp_obj_t *items;
            size_t len;
            mp_obj_get_array(dest[1], &len, &items);

            std::vector<Lpf2::Mode> modes;
            modes.reserve(len);
            for (size_t i = 0; i < len; ++i) {
                mp_obj_t native_m = lpf2_cast_to_native_base(items[i], &lpf2_mode_type);
                if (native_m == MP_OBJ_NULL)
                    mp_raise_TypeError(MP_ERROR_TEXT("modes list must contain mode objects"));
                modes.push_back(*((mp_obj_lpf2_mode_t*)MP_OBJ_TO_PTR(native_m))->cpp_obj);
            }
            self->modes = modes;
            self_c->mode_list = MP_OBJ_NULL;
            dest[0] = MP_OBJ_NULL;
        }
        break;
    }
    default:
        dest[1] = MP_OBJ_SENTINEL;
        break;
    }
}

LPF2_DEFINE_METHOD(device_descriptor_del, (mp_obj_t self_in)
{
    auto self = GET_SELF();
    LPF2_LOG_V("Deleting DeviceDesc, owner: %s", self->owned ? "true" : "false");
    if (self->owned && self->cpp_obj)
    {
        delete self->cpp_obj;
        self->cpp_obj = nullptr;
    }
    return mp_const_none;
},
MP_DEFINE_CONST_FUN_OBJ_1);

static const mp_rom_map_elem_t lpf2_device_descriptor_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&LPF2_GET_METHOD_OBJ(device_descriptor_del))},
};

static MP_DEFINE_CONST_DICT(lpf2_device_descriptor_locals_dict, lpf2_device_descriptor_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    lpf2_device_descriptor_type, 
    MP_QSTR_device_descriptor,
    MP_TYPE_FLAG_NONE,
    make_new, (void*)lpf2_device_descriptor_make_new,
    attr, (void*)lpf2_device_descriptor_attr,
    locals_dict, &lpf2_device_descriptor_locals_dict
);
}