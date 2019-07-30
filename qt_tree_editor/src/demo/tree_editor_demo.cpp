#include <QApplication>
#include "qt_tree_editor/tree_editor.hpp"
#include "qt_tree_editor/property/impl/geometry_msgs_properties.hpp"
#include <console_bridge/console.h>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  // Create a message and fill it with some data
  geometry_msgs::PoseArray msg;
  for(int i = 0; i < 10; ++i)
  {
    msg.poses.push_back(geometry_msgs::Pose());
  }

  // Create a tree editor and show the widget
  TreeEditor<geometry_msgs::PoseArray> tree_editor;
  QWidget* widget = tree_editor.createEditor(msg, "pose_array");
  widget->show();

  // Execute the application
  app.exec();

  bool success = tree_editor.get(msg);
  if(success)
  {
    std::stringstream ss;
    ss << "Message:\n" << ros::message_traits::datatype(msg);
    CONSOLE_BRIDGE_logInform(ss.str().c_str());
    return -1;
  }
  else
  {
    CONSOLE_BRIDGE_logError("Failure");
    return 0;
  }
}
