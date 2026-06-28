add_library(lpf2_mod INTERFACE)

set(LPF2_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lib/Lpf2)
set(LPF2_EXT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lib/Lpf2-ext)

# Lpf2-ext provides the PortExpander; it's optional.
if(EXISTS "${LPF2_EXT_DIR}/include/Lpf2/PortExpander/Device.hpp")
    set(LPF2_HAS_EXT TRUE)
else()
    set(LPF2_HAS_EXT FALSE)
    message(STATUS "Lpf2-ext not found at ${LPF2_EXT_DIR}; building without PortExpander support")
endif()

# Add source files to the library.
file(GLOB_RECURSE LPF2_SOURCES "${LPF2_DIR}/src/Lpf2/*.cpp")
if(LPF2_HAS_EXT)
    file(GLOB_RECURSE LPF2_EXT_SOURCES "${LPF2_EXT_DIR}/src/Lpf2/*.cpp")
    list(APPEND LPF2_SOURCES ${LPF2_EXT_SOURCES})
endif()
file(GLOB_RECURSE LPF2_MOD_SOURCES_CXX "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB_RECURSE LPF2_MOD_SOURCES_C "${CMAKE_CURRENT_LIST_DIR}/*.c")

target_sources(lpf2_mod INTERFACE
    ${LPF2_SOURCES}
    ${LPF2_MOD_SOURCES_CXX}
    ${LPF2_MOD_SOURCES_C}
)

# Add the current directory as an include directory.
target_include_directories(lpf2_mod INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LPF2_DIR}/include
    ${LPF2_DIR}/src
)
if(LPF2_HAS_EXT)
    target_include_directories(lpf2_mod INTERFACE
        ${LPF2_EXT_DIR}/include
        ${LPF2_EXT_DIR}/src
    )
    target_compile_definitions(lpf2_mod INTERFACE LPF2_HAS_PORT_EXPANDER=1)
endif()

target_compile_features(lpf2_mod INTERFACE cxx_std_20)

# Link the INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE lpf2_mod)
