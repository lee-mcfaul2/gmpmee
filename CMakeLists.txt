cmake_minimum_required(VERSION 3.14)
project(gmpmee VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Find GMP
find_library(GMP_LIBRARY NAMES gmp)
find_path(GMP_INCLUDE_DIR NAMES gmp.h)

if(NOT GMP_LIBRARY OR NOT GMP_INCLUDE_DIR)
    message(FATAL_ERROR "GNU MP library not found, see http://gmplib.org/")
endif()

message(STATUS "Found GMP: ${GMP_LIBRARY}")
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

# Shared library
add_library(gmpmee SHARED ${LIB_SOURCES})
target_include_directories(gmpmee PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    ${GMP_INCLUDE_DIR}
)
target_link_libraries(gmpmee PUBLIC ${GMP_LIBRARY})
set_target_properties(gmpmee PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER gmpmee.h
)

# Static library
add_library(gmpmee_static STATIC ${LIB_SOURCES})
target_include_directories(gmpmee_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    ${GMP_INCLUDE_DIR}
)
target_link_libraries(gmpmee_static PUBLIC ${GMP_LIBRARY})
set_target_properties(gmpmee_static PROPERTIES
    OUTPUT_NAME gmpmee
    POSITION_INDEPENDENT_CODE ON
)

# Test executables
add_executable(simexp_test simexp_test.c)
target_link_libraries(simexp_test gmpmee)

add_executable(simexp_time simexp_time.c)
target_link_libraries(simexp_time gmpmee)

add_executable(fixexp_test fixexp_test.c)
target_link_libraries(fixexp_test gmpmee)

add_executable(fixexp_time fixexp_time.c)
target_link_libraries(fixexp_time gmpmee)

add_executable(millerrabin_test millerrabin_test.c)
target_link_libraries(millerrabin_test gmpmee)

# Install rules
include(GNUInstallDirs)
install(TARGETS gmpmee gmpmee_static
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# CPack for packaging
set(CPACK_PACKAGE_NAME "gmpmee")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GMP Modular Exponentiation Extension")
set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
