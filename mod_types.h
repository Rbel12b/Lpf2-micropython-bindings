#if defined(__has_include)
#  if __has_include("Lpf2/PortExpander/Device.hpp") && __has_include("Lpf2/PortExpander/VirtualDevice.hpp")
#    define LPF2_HAS_PORT_EXPANDER 1
#  endif
#endif
#ifndef LPF2_HAS_PORT_EXPANDER
#  define LPF2_HAS_PORT_EXPANDER 0
#endif

extern "C" {
#include "py/runtime.h"
#include "py/objtype.h"
}

#define GET_SELF() ((SELF_TYPE*)(MP_OBJ_TO_PTR(self_in)))
#define GET_SELF_CPP() (GET_SELF()->cpp_obj)

// Walk the instance-type tree to extract the native base of native_type.
// Handles multi-level Python subclassing (class C(B(A(native_type)))).
// Returns the native base object, or MP_OBJ_NULL if incompatible type.
static inline mp_obj_t lpf2_cast_to_native_base(mp_obj_t obj, const mp_obj_type_t *native_type)
{
    while (true) {
        const mp_obj_type_t *type = mp_obj_get_type(obj);
        for (const mp_obj_type_t *t = type; t != NULL;
             t = (const mp_obj_type_t *)MP_OBJ_TYPE_GET_SLOT_OR_NULL(t, parent)) {
            if (t == native_type) {
                return obj;
            }
        }
        if (!mp_obj_is_instance_type(type)) {
            return MP_OBJ_NULL;
        }
        obj = ((mp_obj_instance_t *)MP_OBJ_TO_PTR(obj))->subobj[0];
    }
}

#include "Lpf2/Port.hpp"
#include "Lpf2/Local/Port.hpp"
#include "Lpf2/LWPConst.hpp"
#include "Lpf2/Virtual/Port.hpp"
#include "Lpf2/Virtual/Device.hpp"
#include "Lpf2/Remote/Port.hpp"
#include "Lpf2/Hub.hpp"
#include "Lpf2/HubEmulation.hpp"
#include "Lpf2/DeviceManager.hpp"
#include "Lpf2/Devices/BasicMotor.hpp"
#include "Lpf2/Devices/EncoderMotor.hpp"
#include "Lpf2/Devices/ColorSensor.hpp"
#include "Lpf2/Devices/DistanceSensor.hpp"
#include "Lpf2/Devices/ColorDistanceSensor.hpp"
#include "Lpf2/Devices/HubLED.hpp"
#include "Lpf2/Devices/Accelerometer.hpp"
#include "Lpf2/Devices/Gyroscope.hpp"
#if LPF2_HAS_PORT_EXPANDER
#include "Lpf2/PortExpander/Device.hpp"
#include "Lpf2/PortExpander/VirtualDevice.hpp"
#endif

#define LPF2_DEFINE_METHOD(name, method, fun_obj_def) \
    static mp_obj_t lpf2_##name method \
    fun_obj_def(lpf2_##name##_obj, lpf2_##name);

#define LPF2_DEFINE_METHOD_VAR_BETWEEN(name, method, min_args, max_args) \
    static mp_obj_t lpf2_##name(size_t n_args, const mp_obj_t *args) method \
    MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lpf2_##name##_obj, min_args, max_args, lpf2_##name);

#define LPF2_GET_METHOD_OBJ(name) lpf2_##name##_obj

#define LPF2_REG(str, name) { MP_ROM_QSTR(MP_QSTR_##str), MP_ROM_INT((uint8_t)name) }
#define LPF2_DEFINE_CONST_DICT(name) MP_DEFINE_CONST_DICT(lpf2_##name##_globals, lpf2_##name##_globals_table)
#define LPF2_DEFINE_MOD(name) \
    const mp_obj_module_t lpf2_##name##_module = { \
        .base = { &mp_type_module }, \
        .globals = (mp_obj_dict_t*)&lpf2_##name##_globals, \
    }

#define LPF2_DEFINE_GLOB_TABLE(name, ...) \
    const mp_rom_map_elem_t lpf2_##name##_globals_table[] = { \
        { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_##name) }, \
        __VA_ARGS__ \
    }

#define LPF2_DEFINE_GLOB_TABLE_DICT(name, ...) \
    static LPF2_DEFINE_GLOB_TABLE(name, __VA_ARGS__); \
    static LPF2_DEFINE_CONST_DICT(name)

#define LPF2_DEFINE_MOD_WITH_GLOB(name, ...) \
    LPF2_DEFINE_GLOB_TABLE_DICT(name, __VA_ARGS__); \
    LPF2_DEFINE_MOD(name)

#define LPF2_DEFINE_MOD_WITH_GLOB_STATIC(name, ...) \
    LPF2_DEFINE_GLOB_TABLE_DICT(name, __VA_ARGS__); \
    static LPF2_DEFINE_MOD(name)

#define LPF2_DEFINE_MOD_WITH_GLOB_ATTR_USED(name, ...) \
    LPF2_DEFINE_GLOB_TABLE_DICT(name, __VA_ARGS__); \
    extern "C" __attribute__((used)) \
    LPF2_DEFINE_MOD(name)

#define LPF2_MOD_EXTERN(name) extern const mp_obj_module_t lpf2_##name##_module
#define LPF2_GET_MOD(name) lpf2_##name##_module

#define LPF2_GET_LPF2_MOD_REG(name) { MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_PTR(&lpf2_##name##_module) }
#define LPF2_GET_LPF2_TYPE_REG(name) { MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_PTR(&lpf2_##name##_type) }

#define LPF2_GET_PORT_METHOD_OBJ(name) LPF2_GET_METHOD_OBJ(port_##name)
#define LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(name) extern const mp_obj_fun_builtin_fixed_t LPF2_GET_PORT_METHOD_OBJ(name)
#define LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(name) extern const mp_obj_fun_builtin_var_t LPF2_GET_PORT_METHOD_OBJ(name)

extern "C" {

typedef struct _mp_obj_lpf2_port_t
{
    mp_obj_base_t base;
    Lpf2::Port *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
    bool is_trampoline = false;
    mp_obj_t parent_ref; // lifetime anchor for non-owning wrappers (e.g. port_expander_dev)
} mp_obj_lpf2_port_t;
extern const mp_obj_type_t lpf2_port_type;
extern const mp_obj_type_t lpf2_local_port_type;

typedef struct _mp_obj_lpf2_hub_t
{
    mp_obj_base_t base;
    Lpf2::Hub *cpp_obj = nullptr;
    bool owned = false;
    mp_obj_t port_cache; // dict PortNum(int) -> port wrapper; identity + GC anchor
} mp_obj_lpf2_hub_t;
extern const mp_obj_type_t lpf2_hub_type;

typedef struct _mp_obj_lpf2_virtual_port_t
{
    mp_obj_base_t base;
    Lpf2::Virtual::Port *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
    bool is_trampoline = false;
    mp_obj_t device_ref; // keeps attached virtual device alive for GC
} mp_obj_lpf2_virtual_port_t;
extern const mp_obj_type_t lpf2_virtual_port_type;

typedef struct _mp_obj_lpf2_mode_t
{
    mp_obj_base_t base;
    Lpf2::Mode *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
} mp_obj_lpf2_mode_t;
extern const mp_obj_type_t lpf2_mode_type;
typedef struct _mp_obj_lpf2_version_t
{
    mp_obj_base_t base;
    Lpf2::Version *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
} mp_obj_lpf2_version_t;
extern const mp_obj_type_t lpf2_version_type;

typedef struct _mp_obj_lpf2_device_descriptor_t
{
    mp_obj_base_t base;
    Lpf2::DeviceDescriptor *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
    mp_obj_t combo_list;
    mp_obj_t mode_list;
    mp_obj_t fw_version_ref;
    mp_obj_t hw_version_ref;
} mp_obj_lpf2_device_descriptor_t;
extern const mp_obj_type_t lpf2_device_descriptor_type;

typedef struct _mp_obj_lpf2_virtual_device_t
{
    mp_obj_base_t base;
    Lpf2::Virtual::Device *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
    mp_obj_t descriptor_ref; // keeps DeviceDescriptor alive (GenericDevice holds a reference)
    mp_obj_t write_data_cb;  // optional Python writeData callback
} mp_obj_lpf2_virtual_device_t;
extern const mp_obj_type_t lpf2_virtual_device_type;

typedef struct _mp_obj_lpf2_hub_emulation_t
{
    mp_obj_base_t base;
    Lpf2::HubEmulation *cpp_obj = nullptr;
    bool owned = false; // if true cpp_obj is owned by the mp obj
    mp_obj_t attached_ports; // Python list keeping attached port objects alive for GC
} mp_obj_lpf2_hub_emulation_t;
extern const mp_obj_type_t lpf2_hub_emulation_type;

typedef struct {
    mp_obj_base_t base;
    // Heap-allocated shared handle to the port-owned Device. The pointer
    // (and the shared_ptr itself) is created when the wrapper is minted
    // by Port::device(); freed in __del__. `gen` is the slot generation
    // observed at handoff — methods raise if it no longer matches.
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_basic_motor_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_encoder_motor_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_color_sensor_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_distance_sensor_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_color_distance_sensor_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_hub_led_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_accelerometer_t;
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_gyroscope_t;
#if LPF2_HAS_PORT_EXPANDER
typedef struct {
    mp_obj_base_t base;
    std::shared_ptr<Lpf2::DeviceSlot> *slot;
    uint32_t gen;
    mp_obj_t port_ref;
} mp_obj_lpf2_port_expander_dev_t;
#endif
extern const mp_obj_type_t lpf2_basic_motor_type;
extern const mp_obj_type_t lpf2_encoder_motor_type;
extern const mp_obj_type_t lpf2_color_sensor_type;
extern const mp_obj_type_t lpf2_distance_sensor_type;
extern const mp_obj_type_t lpf2_color_distance_sensor_type;
extern const mp_obj_type_t lpf2_hub_led_type;
extern const mp_obj_type_t lpf2_accelerometer_type;
extern const mp_obj_type_t lpf2_gyroscope_type;
#if LPF2_HAS_PORT_EXPANDER
extern const mp_obj_type_t lpf2_port_expander_type;
#endif

} // extern "C"

// Resolve the typed device pointer from a slot-based wrapper; raise
// RuntimeError if the slot was invalidated since this wrapper was created.
template <typename Cpp, typename Wrapper>
static inline Cpp *lpf2_device_get_cpp(mp_obj_t self_in)
{
    auto *self = (Wrapper *)MP_OBJ_TO_PTR(self_in);
    if (!self->slot || !*self->slot) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("device no longer available"));
    }
    Lpf2::DeviceSlot &slot = **self->slot;
    if (slot.gen != self->gen || slot.ptr == nullptr) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("device no longer available"));
    }
    return static_cast<Cpp *>(slot.ptr);
}

// Free the heap-allocated shared_ptr; safe to call multiple times.
template <typename Wrapper>
static inline void lpf2_device_slot_del(mp_obj_t self_in)
{
    auto *self = (Wrapper *)MP_OBJ_TO_PTR(self_in);
    if (self->slot) {
        delete self->slot;
        self->slot = nullptr;
    }
}

extern "C" {

#if LPF2_HAS_PORT_EXPANDER
typedef struct _mp_obj_lpf2_virtual_port_expander_device_t
{
    mp_obj_base_t base;
    Lpf2::Virtual::PortExpanderDevice *cpp_obj = nullptr;
    bool owned = false;
    mp_obj_t port_refs[4]; // indexed by PortExpander::PortNum (A=0..D=3)
} mp_obj_lpf2_virtual_port_expander_device_t;
extern const mp_obj_type_t lpf2_virtual_port_expander_device_type;
#endif

extern const mp_obj_fun_builtin_fixed_t lpf2_devices_register_default_obj;

LPF2_MOD_EXTERN(color);
LPF2_MOD_EXTERN(hub_type);
LPF2_MOD_EXTERN(hub_property);
LPF2_MOD_EXTERN(device_type);
LPF2_MOD_EXTERN(battery_type);
LPF2_MOD_EXTERN(alerts);
LPF2_MOD_EXTERN(button_state);
LPF2_MOD_EXTERN(port_num);
LPF2_MOD_EXTERN(motor_setting);
LPF2_MOD_EXTERN(battery);

LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(update);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(init);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(device);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(write_data);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(start_power);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(set_acc_time);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(set_dec_time);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(start_speed);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(start_speed_for_time);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(start_speed_for_degrees);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(goto_abs_pos);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(preset_encoder);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(set_rgb_color_idx);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(set_rgb_color);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(set_mode);
LPF2_DEFINE_PORT_METHOD_VAR_OBJ_EXTERN(set_mode_combo);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(is_device_connected);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_value);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_value_str);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_device_type);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_mode_count);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_view_count);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_mode);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_mode_combo_count);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_mode_combo);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_input_modes);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_output_modes);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_capabilities);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(get_info_str);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(speed_to_raw);
LPF2_DEFINE_PORT_METHOD_OBJ_EXTERN(raw_to_speed);

} // extern "C"