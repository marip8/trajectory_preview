cmake_minimum_required(VERSION 2.8.3)
project(qt_tree_editor_ros)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  qt_tree_editor
  rosconsole
  shape_msgs
  std_msgs
)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
)

catkin_package(
  INCLUDE_DIRS
    include
  CATKIN_DEPENDS
    geometry_msgs
    qt_tree_editor
    rosconsole
    shape_msgs
    std_msgs
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_demo
  src/demo/tree_editor_demo.cpp
)
target_link_libraries(${PROJECT_NAME}_demo
  ${catkin_LIBRARIES}
  Qt5::Core
  Qt5::Widgets
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}_demo
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
