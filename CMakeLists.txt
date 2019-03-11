cmake_minimum_required(VERSION 3.3)

project(DisplaySettings)

find_package(WPEFramework)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

set(PLUGIN_NAME DisplaySettings)
set(MODULE_NAME ${NAMESPACE}${PLUGIN_NAME})

find_package(${NAMESPACE}Plugins REQUIRED)

add_library(${MODULE_NAME} SHARED
    Module.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_link_libraries(${MODULE_NAME} 
        PRIVATE 
            ${NAMESPACE}Plugins::${NAMESPACE}Plugins)


string(TOLOWER ${NAMESPACE} STORAGENAME)
install(TARGETS ${MODULE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${STORAGENAME}/plugins)

write_config(${PLUGIN_NAME})

find_package(IARMBus REQUIRED)
find_package(DS REQUIRED)
add_definitions(-DUSE_DISPLAY_SETTINGS)
target_include_directories(${DRM_PLUGIN_NAME} PRIVATE ${IARMBUS_INCLUDE_DIRS})
target_include_directories(${DRM_PLUGIN_NAME} PRIVATE ${DS_INCLUDE_DIRS})
target_link_libraries(${DRM_PLUGIN_NAME} ${IARMBUS_LIBRARIES} ${DS_LIBRARIES})

