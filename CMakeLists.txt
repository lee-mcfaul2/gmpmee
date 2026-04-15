cmake_minimum_required(VERSION 3.14)
project(gmpmee VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Find GMP - prefer static
find_library(GMP_LIBRARY_STATIC NAMES libgmp.a
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

# Build gmpmee as a static library
add_library(gmpmee STATIC ${LIB_SOURCES})
target_include_directories(gmpmee PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    ${GMP_INCLUDE_DIR}
)
set_target_properties(gmpmee PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Create fat static library by merging gmpmee + GMP after build
set(FAT_LIB ${CMAKE_BINARY_DIR}/libgmpmee_fat.a)

add_custom_command(
    TARGET gmpmee POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Creating fat static library with GMP included..."
    COMMAND ${CMAKE_COMMAND}
        -DFAT_LIB=${FAT_LIB}
        -DTHIN_LIB=$<TARGET_FILE:gmpmee>
        -DGMP_LIB=${GMP_LIB}
        -DAR=${CMAKE_AR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/merge_libs.cmake
    COMMENT "Merging gmpmee + GMP into fat static library"
)

install(FILES ${FAT_LIB} DESTINATION lib RENAME libgmpmee.a)
install(FILES gmpmee.h DESTINATION include)

# Test executables link against both libs to verify
add_executable(simexp_test simexp_test.c)
target_link_libraries(simexp_test gmpmee ${GMP_LIB})

add_executable(simexp_time simexp_time.c)
target_link_libraries(simexp_time gmpmee ${GMP_LIB})

add_executable(fixexp_test fixexp_test.c)
target_link_libraries(fixexp_test gmpmee ${GMP_LIB})

add_executable(fixexp_time fixexp_time.c)
target_link_libraries(fixexp_time gmpmee ${GMP_LIB})

add_executable(millerrabin_test millerrabin_test.c)
target_link_libraries(millerrabin_test gmpmee ${GMP_LIB})
