cmake_minimum_required(VERSION 2.8.3)
project(qt_tree_editor)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  shape_msgs
  std_msgs
  rosconsole
)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${PROJECT_NAME}
  CATKIN_DEPENDS
    geometry_msgs
    shape_msgs
    std_msgs
    rosconsole
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${PROJECT_NAME}_UIS
  ui/tree_editor.ui
)

qt5_wrap_ui(${PROJECT_NAME}_MOC_UI
  ${${PROJECT_NAME}_UIS}
)

set(${PROJECT_NAME}_HEADERS
  include/${PROJECT_NAME}/component/component_delegate.h
)

qt5_wrap_cpp(${PROJECT_NAME}_MOC_HEADERS
  ${${PROJECT_NAME}_HEADERS}
)

add_library(${PROJECT_NAME}
  src/component.cpp
  src/vector_child_component.cpp
  src/component_delegate.cpp
  ${${PROJECT_NAME}_MOC_HEADERS}
  ${${PROJECT_NAME}_MOC_UI}
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
)

add_executable(${PROJECT_NAME}_demo
  src/demo/tree_editor_demo.cpp
)
target_link_libraries(${PROJECT_NAME}_demo
  ${catkin_LIBRARIES}
  ${PROJECT_NAME}
  Qt5::Core
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_demo
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(FILES
  ui/tree_editor.ui
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#############
## Testing ##
#############
