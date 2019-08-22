cmake_minimum_required(VERSION 3.5.0)
project(qt_tree_editor VERSION 0.1.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
)

find_package(console_bridge REQUIRED)

###########
## Build ##
###########

list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_11 CXX_FEATURE_FOUND)

set(${PROJECT_NAME}_HEADERS
  include/${PROJECT_NAME}/property/property_delegate.h
  include/${PROJECT_NAME}/component/component_delegate.h
)

qt5_wrap_cpp(${PROJECT_NAME}_MOC_HEADERS
  ${${PROJECT_NAME}_HEADERS}
)

add_library(${PROJECT_NAME} SHARED
  src/component.cpp
  src/vector_child_component.cpp
  src/property_delegate.cpp
  src/component_delegate.cpp
  ${${PROJECT_NAME}_MOC_HEADERS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
  Qt5::Widgets
  Qt5::Core
  console_bridge
)
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -Wextra
  -Wsuggest-override
  -Wconversion
  -Wsign-conversion
)
if(CXX_FEATURE_FOUND EQUAL "-1")
  target_compile_options(${PROJECT_NAME} PUBLIC -std=c++11)
else()
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#############
## Install ##
#############

list(APPEND PACKAGE_LIBRARIES ${PROJECT_NAME})

# Install targets
install(TARGETS ${PACKAGE_LIBRARIES}
  EXPORT ${PROJECT_NAME}-targets
  DESTINATION lib
)

# Install target interface
install(EXPORT ${PROJECT_NAME}-targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
)

# Install files
install(FILES package.xml
  DESTINATION share/${PROJECT_NAME}
)

# Create cmake config files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

export(EXPORT ${PROJECT_NAME}-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake
)
