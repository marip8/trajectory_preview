if(${CATKIN_ENABLE_TESTING})
  find_package(rostest REQUIRED)
  catkin_add_gtest(${PROJECT_NAME}_test utest.cpp)
  target_link_libraries(${PROJECT_NAME}_test
    qt_tree_editor::qt_tree_editor
    ${catkin_LIBRARIES}
  )
endif()
