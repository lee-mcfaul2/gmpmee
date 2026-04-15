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
    ERROR_VARIABLE err_out
)
if(result)
    message(FATAL_ERROR "Failed to extract gmpmee archive: ${err_out}")
endif()

execute_process(
    COMMAND ${AR} x ${GMP_LIB}
    WORKING_DIRECTORY "${WORK_DIR}/gmp"
    RESULT_VARIABLE result
    ERROR_VARIABLE err_out
)
if(result)
    message(FATAL_ERROR "Failed to extract GMP archive: ${err_out}")
endif()

# Collect all object files
file(GLOB GMPMEE_OBJS "${WORK_DIR}/gmpmee/*")
file(GLOB GMP_OBJS "${WORK_DIR}/gmp/*")

message(STATUS "GMPMEE objects: ${GMPMEE_OBJS}")
message(STATUS "GMP objects: ${GMP_OBJS}")

set(ALL_OBJS ${GMPMEE_OBJS} ${GMP_OBJS})
list(LENGTH ALL_OBJS OBJ_COUNT)
message(STATUS "Total objects to merge: ${OBJ_COUNT}")

if(OBJ_COUNT EQUAL 0)
    message(FATAL_ERROR "No object files found to merge")
endif()

# Remove the fat lib if it exists
file(REMOVE "${FAT_LIB}")

# Use an MRI script to merge - works on all platforms and avoids
# command line length limits
set(MRI_SCRIPT "${WORK_DIR}/merge.mri")
file(WRITE "${MRI_SCRIPT}" "create ${FAT_LIB}\n")
file(APPEND "${MRI_SCRIPT}" "addlib ${THIN_LIB}\n")
file(APPEND "${MRI_SCRIPT}" "addlib ${GMP_LIB}\n")
file(APPEND "${MRI_SCRIPT}" "save\n")
file(APPEND "${MRI_SCRIPT}" "end\n")

execute_process(
    COMMAND ${AR} -M
    INPUT_FILE "${MRI_SCRIPT}"
    RESULT_VARIABLE result
    ERROR_VARIABLE err_out
)
if(result)
    message(STATUS "MRI merge failed (${err_out}), falling back to extract+repack")
    # Fallback: create archive in batches to avoid command line limits
    # First create with gmpmee objects
    execute_process(
        COMMAND ${AR} rcs ${FAT_LIB} ${GMPMEE_OBJS}
        RESULT_VARIABLE result
        ERROR_VARIABLE err_out
    )
    if(result)
        message(FATAL_ERROR "Failed to create archive with gmpmee objects: ${err_out}")
    endif()
    # Then append GMP objects
    execute_process(
        COMMAND ${AR} rs ${FAT_LIB} ${GMP_OBJS}
        RESULT_VARIABLE result
        ERROR_VARIABLE err_out
    )
    if(result)
        message(FATAL_ERROR "Failed to append GMP objects: ${err_out}")
    endif()
endif()

# Clean up
file(REMOVE_RECURSE "${WORK_DIR}")

message(STATUS "Created fat static library: ${FAT_LIB}")
