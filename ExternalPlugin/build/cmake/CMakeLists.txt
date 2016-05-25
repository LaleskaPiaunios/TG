cmake_minimum_required(VERSION 2.8.12)

project(externalplugin)

add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0)

find_package(Boost 1.53.0 REQUIRED system date_time chrono timer filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER  2.8.9)
  cmake_policy(SET CMP0020 NEW)
endif()

if(${CMAKE_MAJOR_VERSION} GREATER 2)
  cmake_policy(SET CMP0026 OLD)
  cmake_policy(SET CMP0042 NEW)
  cmake_policy(SET CMP0043 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER "3.1.2")
  cmake_policy(SET CMP0053 OLD)
endif()

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

if(WIN32)
  add_definitions(-DEXTERNALPLUGINDLL)
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB HDR_FILES ${SRC_DIR}/*.h)
file(GLOB HDR_FILES ${SRC_DIR}/qt/*.h)
file(GLOB HDR_FILES ${SRC_DIR}/qt/*.cpp)
file(GLOB UI_FILES  ${SRC_DIR}/qt/ui/*.ui)

find_package(terralib REQUIRED)

include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src
                    ${terralib_INCLUDE_DIRS}
                    ${terralib_DIR})


find_package(Qt5 5.1 QUIET COMPONENTS Core Gui Widgets PrintSupport)
if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  # uic'ing
  QT5_WRAP_UI(GEN_HDR_FILES ${UI_FILES})
  
  set(EXTERNALPLUGIN_FILES ${SRC_FILES} ${HDR_FILES} ${GEN_HDR_FILES})

  add_library(externalplugin SHARED ${EXTERNALPLUGIN_FILES})

  target_link_libraries(externalplugin terralib_mod_plugin
                                       terralib_mod_qt_apf
                                       terralib_mod_color
                                       terralib_mod_common
                                       terralib_mod_dataaccess
                                       terralib_mod_datatype
                                       terralib_mod_geometry
                                       terralib_mod_maptools
                                       terralib_mod_memory
                                       terralib_mod_qt_widgets
                                       terralib_mod_srs
                                       terralib_mod_symbology
                                       terralib_mod_raster
									   terralib_mod_vp_core)

  qt5_use_modules(externalplugin Widgets)

else()
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  include(${QT_USE_FILE})
  include_directories(${QT_INCLUDE_DIR})
  add_definitions(${QT_DEFINITIONS})

# uic'ing
  QT4_WRAP_UI(GEN_HDR_FILES ${UI_FILES})
  
# moc'ing
  set(HDRS_TO_MOC_FILES ${SRC_DIR}/qt/MyDialog.h)

  QT4_WRAP_CPP(GEN_SRC_FILES ${HDRS_TO_MOC_FILES})

  set(EXTERNALPLUGIN_FILES ${SRC_FILES} ${HDR_FILES} ${GEN_SRC_FILES} ${GEN_HDR_FILES})

  add_library(externalplugin SHARED ${EXTERNALPLUGIN_FILES})

  target_link_libraries(externalplugin terralib_mod_plugin
                                       terralib_mod_qt_apf
                                       ${QT_LIBRARIES})

endif()

set_target_properties(externalplugin
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

export(TARGETS externalplugin APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)

if(MSVC)
   add_custom_command(
        TARGET externalplugin
        POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/Debug ${terralib_DIR}/debug
        
        COMMENT "Copying plugin files..."
        VERBATIM
      )
 add_custom_command(
  TARGET externalplugin
  POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E  copy_directory ${CMAKE_BINARY_DIR}/Release ${terralib_DIR}/release
  
  COMMENT "Copying plugin files..."
  VERBATIM
   )
endif()
