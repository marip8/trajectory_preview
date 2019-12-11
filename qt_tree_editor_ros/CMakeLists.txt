cmake_minimum_required(VERSION 2.8.3)
project(qt_tree_editor_ros)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  rosconsole
  shape_msgs
  std_msgs
  trajectory_msgs
)

find_package(qt_tree_editor REQUIRED)

catkin_package(
  INCLUDE_DIRS
    include
  CATKIN_DEPENDS
    geometry_msgs
    rosconsole
    shape_msgs
    std_msgs
    trajectory_msgs
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

#############
## Install ##
#############

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

#############
## Testing ##
#############

if(${CATKIN_ENABLE_TESTING})
  find_package(rostest REQUIRED)
  catkin_add_gtest(${PROJECT_NAME}_test test/utest.cpp)
  target_link_libraries(${PROJECT_NAME}_test
    qt_tree_editor::qt_tree_editor
    ${catkin_LIBRARIES}
  )
endif()
