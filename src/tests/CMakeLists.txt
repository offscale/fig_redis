#######################
# Dependency download #
#######################

set(GREATEST_SHA256 "1292593d95c35eeccc89ffa1c91d6fe53b49f81cbf2c2b7758842b7f3186fcc2")
set(DOWNLOAD_DIR "${PROJECT_BINARY_DIR}/test_downloads")
file(MAKE_DIRECTORY "${DOWNLOAD_DIR}")
set(GREATEST_BASEFILENAME "greatest.h")
set(GREATEST_FILE "${PROJECT_BINARY_DIR}/downloads/${GREATEST_BASEFILENAME}")
set(GREATEST_URL "https://raw.githubusercontent.com/silentbicycle/greatest/11a6af1/greatest.h")
file(DOWNLOAD "${GREATEST_URL}" "${GREATEST_FILE}"
     EXPECTED_HASH "SHA256=${GREATEST_SHA256}")

####################
# Target formation #
####################

set(EXEC_NAME "test_${PROJECT_NAME}")

set(Header_Files "test_url_parsing.h")
source_group("${EXEC_NAME} Header Files" FILES "${Header_Files}")

set(Source_Files "test.c")
source_group("${EXEC_NAME} Source Files" FILES "${Source_Files}")

add_executable("${EXEC_NAME}" "${Header_Files}" "${Source_Files}")

target_link_libraries("${EXEC_NAME}" PRIVATE "redis_version")

target_include_directories(
        "${EXEC_NAME}"
        PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/downloads>"
        "$<INSTALL_INTERFACE:include>"
)

add_test(NAME "${EXEC_NAME}" COMMAND "${EXEC_NAME}")

#######################################
# Export / installation configuration #
#######################################

# setup the version numbering
#set_property(TARGET "${EXEC_NAME}" PROPERTY VERSION "1.0.0")
#set_property(TARGET "${EXEC_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${EXEC_NAME}" "${PROJECT_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
list(APPEND All_Header_Files "${Header_Files}")
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${EXEC_NAME}Targets")

install(FILES "${GREATEST_FILE}" "${Header_Files}" DESTINATION "include")
