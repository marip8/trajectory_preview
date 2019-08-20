#include <QApplication>
#include <qt_tree_editor/tree_editor.hpp>
#include "qt_tree_editor_ros/geometry_msgs_properties.hpp"
#include "qt_tree_editor_ros/shape_msgs_properties.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  // Create a message and fill it with some data
  geometry_msgs::PoseArray msg;
  for(int i = 0; i < 10; ++i)
  {
    msg.poses.push_back(geometry_msgs::Pose());
  }

//  shape_msgs::SolidPrimitive msg;
//  msg.type = shape_msgs::SolidPrimitive::BOX;
//  msg.dimensions.resize(3);
//  msg.dimensions[shape_msgs::SolidPrimitive::BOX_X] = 1.0;
//  msg.dimensions[shape_msgs::SolidPrimitive::BOX_Y] = 2.0;
//  msg.dimensions[shape_msgs::SolidPrimitive::BOX_Z] = 3.0;

  // Create a tree editor and show the widget
  qt_tree_editor::TreeEditor<decltype(msg)> tree_editor;
  QWidget* widget = tree_editor.createEditor(msg, "demo");
  widget->show();

  // Execute the application
  app.exec();

  bool success = tree_editor.get(msg);
  if(success)
  {
    ROS_INFO_STREAM("Message:\n" << msg);
    return 0;
  }
  else
  {
    ROS_ERROR_STREAM("Failure");
    return -1;
  }
}
