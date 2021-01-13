cmake_minimum_required(VERSION 3.5.0)
project(qt_tree_editor VERSION 0.1.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(console_bridge REQUIRED)
find_package(cmake_common_scripts REQUIRED)

###########
## Build ##
###########
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
  console_bridge
)
target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
target_cxx_version(${PROJECT_NAME} PUBLIC 14)

#############
## Install ##
#############

configure_package(NAMESPACE ${PROJECT_NAME} TARGETS ${PROJECT_NAME})

install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
)
