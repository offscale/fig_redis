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

if (TARGET "${DEPENDANT_LIBRARY}")
    list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
# install(FILES "${Header_Files}" DESTINATION "include")
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${EXEC_NAME}Targets")
