# Merge two static libraries into one fat static library
# Called as a cmake -P script with: FAT_LIB, THIN_LIB, GMP_LIB, AR

set(WORK_DIR "${FAT_LIB}_merge")
file(REMOVE_RECURSE "${WORK_DIR}")
file(MAKE_DIRECTORY "${WORK_DIR}/gmpmee")
file(MAKE_DIRECTORY "${WORK_DIR}/gmp")

# Extract both archives
execute_process(
    COMMAND ${AR} x ${THIN_LIB}
    WORKING_DIRECTORY "${WORK_DIR}/gmpmee"
    RESULT_VARIABLE result
)
if(result)
    message(FATAL_ERROR "Failed to extract gmpmee archive")
endif()

execute_process(
    COMMAND ${AR} x ${GMP_LIB}
    WORKING_DIRECTORY "${WORK_DIR}/gmp"
    RESULT_VARIABLE result
)
if(result)
    message(FATAL_ERROR "Failed to extract GMP archive")
endif()

# Collect all .o / .obj files
file(GLOB GMPMEE_OBJS "${WORK_DIR}/gmpmee/*.o" "${WORK_DIR}/gmpmee/*.obj")
file(GLOB GMP_OBJS "${WORK_DIR}/gmp/*.o" "${WORK_DIR}/gmp/*.obj")

# Remove the fat lib if it exists
file(REMOVE "${FAT_LIB}")

# Create combined archive
set(ALL_OBJS ${GMPMEE_OBJS} ${GMP_OBJS})
execute_process(
    COMMAND ${AR} rcs ${FAT_LIB} ${ALL_OBJS}
    RESULT_VARIABLE result
)
if(result)
    message(FATAL_ERROR "Failed to create fat archive")
endif()

# Clean up
file(REMOVE_RECURSE "${WORK_DIR}")

message(STATUS "Created fat static library: ${FAT_LIB}")
