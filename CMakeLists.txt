cmake_minimum_required(VERSION 3.9)
SET(APP_NAME ProjectorFramework)

PROJECT(${APP_NAME})

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(GENERATOR_IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugWorker" CACHE TYPE INTERNAL FORCE)
    set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS "Debug;DebugWorker")
    set(CMAKE_C_FLAGS_DEBUGWORKER ${CMAKE_C_FLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_DEBUGWORKER ${CMAKE_CXX_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_DEBUGWORKER ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUGWORKER ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
    set(COMPILE_DEFINITIONS_DEBUGWORKER ${COMPILE_DEFINITIONS_DEBUG})
endif()

set(VISCOM_APP_NAME "${APP_NAME}" CACHE STRING "Name of the application to be build.")
set(VISCOM_INSTALL_BASE_PATH "D:/LabShare/cluster/apps/" CACHE PATH "Path to install the project to (should be the shared apps directory).")
set(VISCOM_CONFIG_NAME "single" CACHE STRING "Name/directory of the configuration files to be used.")
set(VISCOM_VIRTUAL_SCREEN_X 1920 CACHE INTEGER "Virtual screen size in x direction.")
set(VISCOM_VIRTUAL_SCREEN_Y 1080 CACHE INTEGER "Virtual screen size in y direction.")

add_subdirectory(extern/fwcore)

# Enable default importers
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "COL IMPORTER" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER     ON CACHE BOOL "FBX IMPORTER" FORCE)
set(ASSIMP_BUILD_MMD_IMPORTER     ON CACHE BOOL "MMD IMPORTER" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER     ON CACHE BOOL "OBJ IMPORTER" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER     ON CACHE BOOL "PLY IMPORTER" FORCE)
set(ASSIMP_BUILD_STL_IMPORTER     ON CACHE BOOL "STL IMPORTER" FORCE)

file(GLOB_RECURSE CFG_FILES ${PROJECT_SOURCE_DIR}/config/*.*)
file(GLOB_RECURSE DATA_FILES ${PROJECT_SOURCE_DIR}/data/*.*)
file(GLOB_RECURSE SHADER_FILES ${PROJECT_SOURCE_DIR}/resources/shader/*.*)
list(FILTER SHADER_FILES EXCLUDE REGEX ".*\.gen$")
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/app/*.h
    ${PROJECT_SOURCE_DIR}/src/app/*.cpp)
file(GLOB SRC_FILES_ROOT
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(APPEND SRC_FILES ${SRC_FILES_ROOT})
source_group("shader" FILES ${SHADER_FILES})

foreach(f ${SRC_FILES})
    file(RELATIVE_PATH SRCGR ${PROJECT_SOURCE_DIR} ${f})
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

add_executable(${APP_NAME} ${SRC_FILES} ${SHADER_FILES})
set_target_properties(${APP_NAME} PROPERTIES OUTPUT_NAME ${VISCOM_APP_NAME})
set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${APP_NAME} PRIVATE src)
target_link_libraries(${APP_NAME} VISCOMCore)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${APP_NAME} PRIVATE -stdlib=libc++)
    target_link_libraries(${APP_NAME} -stdlib=libc++ c++experimental c++abi)
    target_include_directories(${APP_NAME} SYSTEM BEFORE PRIVATE /usr/include/c++/v1)
endif()

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
endif()

set(VISCOM_CONFIG_BASE_DIR "../")
set(VISCOM_CONFIG_PROGRAM_PROPERTIES "../config/${VISCOM_CONFIG_NAME}/propertiesPrecompute.xml")
set(VISCOM_CONFIG_SGCT_CONFIG "../data/${VISCOM_CONFIG_NAME}/${VISCOM_CONFIG_NAME}.xml")
set(VISCOM_CONFIG_PROJECTOR_DATA "../data/${VISCOM_CONFIG_NAME}/ProjectorData.xml")
set(VISCOM_CONFIG_LOCAL "0")
configure_file("framework.cfg" "framework.cfg")
set(VISCOM_CONFIG_LOCAL "1 --slave")
configure_file("framework.cfg" "framework_local_worker.cfg")
set(VISCOM_CONFIG_BASE_DIR "./")
set(VISCOM_CONFIG_PROGRAM_PROPERTIES "../../calibration/config/${VISCOM_CONFIG_NAME}/propertiesPrecompute.xml")
set(VISCOM_CONFIG_SGCT_CONFIG "../../calibration/data/${VISCOM_CONFIG_NAME}/${VISCOM_CONFIG_NAME}.xml")
set(VISCOM_CONFIG_PROJECTOR_DATA "../../calibration/data/${VISCOM_CONFIG_NAME}/ProjectorData.xml")
set(VISCOM_CONFIG_LOCAL "-1")
configure_file("framework.cfg" "framework_install.cfg")

if(GENERATOR_IS_MULTI_CONFIG)
    foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        if(${CONFIG_TYPE} STREQUAL "DebugWorker")
            set(VISCOM_CMD_LINE "framework_local_worker.cfg")
        else()
            set(VISCOM_CMD_LINE "")
        endif()
        configure_file("RDSettings.cap" "${CONFIG_TYPE}Settings.cap")
    endforeach()
endif()

copy_core_lib_dlls(${APP_NAME})

install(TARGETS ${APP_NAME} RUNTIME DESTINATION ${VISCOM_INSTALL_BASE_PATH}/${VISCOM_APP_NAME})
install(DIRECTORY resources/ DESTINATION ${VISCOM_INSTALL_BASE_PATH}/${VISCOM_APP_NAME}/resources)
install(FILES ${CMAKE_BINARY_DIR}/framework_install.cfg DESTINATION ${VISCOM_INSTALL_BASE_PATH}/${VISCOM_APP_NAME} RENAME framework.cfg)
