set(EXEC_NAME "${PROJECT_NAME}_c")

#set(Header_Files "library.h")
#source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "cli.c")
source_group("Source Files" FILES "${Source_Files}")

add_executable(
        "${EXEC_NAME}"
        # "${Header_Files}"
        "${Source_Files}"
)

#target_include_directories(
#        "${EXEC_NAME}"
#        PRIVATE
#        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#        "$<INSTALL_INTERFACE:include>"
#)

target_link_libraries("${EXEC_NAME}" PRIVATE "redis_version")

# install rules
include(GNUInstallDirs)

install(TARGETS "${EXEC_NAME}"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${EXEC_NAME}"
        EXPORT "${EXEC_NAME}Targets")
install(TARGETS ${installable_libs}
        EXPORT "${EXEC_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
