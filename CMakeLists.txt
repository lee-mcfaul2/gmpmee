cmake_minimum_required(VERSION 3.14)
project(gmpmee VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Find GMP - prefer static
find_library(GMP_LIBRARY_STATIC NAMES libgmp.a gmp.lib libgmp.lib
    HINTS
    ${CMAKE_PREFIX_PATH}/lib
    /usr/local/lib
    /opt/homebrew/lib
)
find_library(GMP_LIBRARY NAMES gmp
    HINTS
    ${CMAKE_PREFIX_PATH}/lib
    /usr/local/lib
    /opt/homebrew/lib
)
find_path(GMP_INCLUDE_DIR NAMES gmp.h
    HINTS
    ${CMAKE_PREFIX_PATH}/include
    /usr/local/include
    /opt/homebrew/include
)

# Use static GMP if available, otherwise fall back to shared
if(GMP_LIBRARY_STATIC)
    set(GMP_LIB ${GMP_LIBRARY_STATIC})
    message(STATUS "Found GMP (static): ${GMP_LIB}")
elseif(GMP_LIBRARY)
    set(GMP_LIB ${GMP_LIBRARY})
    message(STATUS "Found GMP (shared): ${GMP_LIB}")
else()
    message(FATAL_ERROR "GNU MP library not found, see http://gmplib.org/")
endif()

if(NOT GMP_INCLUDE_DIR)
    message(FATAL_ERROR "GNU MP headers not found, see http://gmplib.org/")
endif()
message(STATUS "GMP include: ${GMP_INCLUDE_DIR}")

# Check architecture for 64-bit
include(CheckTypeSize)
check_type_size("long" SIZEOF_LONG)
if(SIZEOF_LONG EQUAL 8)
    set(BITS64 TRUE)
endif()

# Option for REDC support
option(ENABLE_REDC "Use experimental redc-representation of moduli" OFF)

# Library sources
set(LIB_SOURCES
    spowm.c spowm_clear.c spowm_precomp.c spowm_block.c
    spowm_init.c spowm_table.c spowm_block_batch.c
    spowm_init_precomp.c spowm_naive.c
    array_alloc.c array_clear_dealloc.c array_urandomb.c
    array_alloc_init.c array_out_str.c
    fpowm.c fpowm_clear.c fpowm_init.c fpowm_precomp.c
    fpowm_init_precomp.c
    millerrabin_init.c millerrabin_next_cand.c millerrabin_clear.c
    millerrabin_trial.c millerrabin_once.c millerrabin_reps_rs.c
    millerrabin_rs.c millerrabin_next_rs.c
    millerrabin_safe_init.c millerrabin_safe_next_cand.c
    millerrabin_safe_clear.c millerrabin_safe_trial.c
    millerrabin_safe_reps_rs.c millerrabin_safe_rs.c
    millerrabin_safe_next_rs.c
    probab_prime_p_next.c probab_safe_prime_p.c
    probab_safe_prime_p_next.c
)

if(ENABLE_REDC)
    list(APPEND LIB_SOURCES
        backwards_mpn_redc_1.c redc_clear.c redc_init_set.c
        redc_encode.c redc_decode.c redc_mod.c
    )
    add_definitions(-DREDC)
endif()

# Static library with GMP baked in
add_library(gmpmee_objects OBJECT ${LIB_SOURCES})
target_include_directories(gmpmee_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GMP_INCLUDE_DIR}
)
set_target_properties(gmpmee_objects PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Create a fat static library that includes both gmpmee and GMP
# We do this by extracting GMP's .a and combining the object files
add_library(gmpmee_thin STATIC $<TARGET_OBJECTS:gmpmee_objects>)
set_target_properties(gmpmee_thin PROPERTIES
    OUTPUT_NAME gmpmee_thin
    POSITION_INDEPENDENT_CODE ON
)

# Custom target to create the fat static library
if(WIN32)
    # On Windows with MSVC, use lib.exe to merge
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/libgmpmee.lib
        COMMAND ${CMAKE_AR} /OUT:${CMAKE_BINARY_DIR}/libgmpmee.lib
            $<TARGET_FILE:gmpmee_thin> ${GMP_LIB}
        DEPENDS gmpmee_thin
        COMMENT "Creating fat static library with GMP included"
    )
    add_custom_target(gmpmee_fat ALL DEPENDS ${CMAKE_BINARY_DIR}/libgmpmee.lib)
    install(FILES ${CMAKE_BINARY_DIR}/libgmpmee.lib
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
else()
    # On Unix, use a MRI script to merge .a files
    file(WRITE ${CMAKE_BINARY_DIR}/merge.mri
"create ${CMAKE_BINARY_DIR}/libgmpmee.a
addlib $<TARGET_FILE:gmpmee_thin>
addlib ${GMP_LIB}
save
end
")
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/libgmpmee.a
        COMMAND ${CMAKE_AR} -M < ${CMAKE_BINARY_DIR}/merge.mri
        DEPENDS gmpmee_thin
        COMMENT "Creating fat static library with GMP included"
    )
    add_custom_target(gmpmee_fat ALL DEPENDS ${CMAKE_BINARY_DIR}/libgmpmee.a)
    install(FILES ${CMAKE_BINARY_DIR}/libgmpmee.a
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

install(FILES gmpmee.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Test executables - link statically against the fat lib
add_executable(simexp_test simexp_test.c)
target_include_directories(simexp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GMP_INCLUDE_DIR})
target_link_libraries(simexp_test gmpmee_thin ${GMP_LIB})

add_executable(simexp_time simexp_time.c)
target_include_directories(simexp_time PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GMP_INCLUDE_DIR})
target_link_libraries(simexp_time gmpmee_thin ${GMP_LIB})

add_executable(fixexp_test fixexp_test.c)
target_include_directories(fixexp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GMP_INCLUDE_DIR})
target_link_libraries(fixexp_test gmpmee_thin ${GMP_LIB})

add_executable(fixexp_time fixexp_time.c)
target_include_directories(fixexp_time PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GMP_INCLUDE_DIR})
target_link_libraries(fixexp_time gmpmee_thin ${GMP_LIB})

add_executable(millerrabin_test millerrabin_test.c)
target_include_directories(millerrabin_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GMP_INCLUDE_DIR})
target_link_libraries(millerrabin_test gmpmee_thin ${GMP_LIB})
