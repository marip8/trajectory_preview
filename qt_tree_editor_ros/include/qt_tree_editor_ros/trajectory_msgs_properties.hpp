#ifndef QT_TREE_EDITOR_ROS_TRAJECTORY_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_ROS_TRAJECTORY_MSGS_PROPERTIES_HPP

#include "qt_tree_editor_ros/stl_properties.hpp"
#include "qt_tree_editor_ros/std_msgs_properties.hpp"
#include <trajectory_msgs/JointTrajectory.h>

namespace
{
std::vector<QString> toQStringVector(const std::vector<std::string>& in)
{
  std::vector<QString> out;
  out.reserve(in.size());
  for (const std::string& str : in)
  {
    out.push_back(QString::fromStdString(str));
  }
  return out;
}

std::vector<std::string> fromQStringVector(const std::vector<QString>& in)
{
  std::vector<std::string> out;
  out.reserve(in.size());
  for (const QString& q : in)
  {
    out.push_back(q.toStdString());
  }
  return out;
}

} // namespace anonymous

namespace qt_tree_editor
{
template <>
struct Property<trajectory_msgs::JointTrajectoryPoint>
{
  template <typename ComponentT>
  static void encode(const trajectory_msgs::JointTrajectoryPoint& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<std::vector<double>>::encode(msg.positions, "positions", q_item);
    Property<std::vector<double>>::encode(msg.velocities, "velocities", q_item);
    Property<std::vector<double>>::encode(msg.accelerations, "accelerations", q_item);
    Property<std::vector<double>>::encode(msg.effort, "effort", q_item);
    Property<ros::Duration>::encode<Component>(msg.time_from_start, "time_from_start", q_item);
  }

  static bool decode(const Component* item, trajectory_msgs::JointTrajectoryPoint& msg)
  {
    bool success = false;
    if (item->rowCount() == 5)
    {
      success = true;
      success &= Property<std::vector<double>>::decode(static_cast<Component*>(item->child(0)), msg.positions);
      success &= Property<std::vector<double>>::decode(static_cast<Component*>(item->child(1)), msg.velocities);
      success &= Property<std::vector<double>>::decode(static_cast<Component*>(item->child(2)), msg.accelerations);
      success &= Property<std::vector<double>>::decode(static_cast<Component*>(item->child(3)), msg.effort);
      success &= Property<ros::Duration>::decode(static_cast<Component*>(item->child(4)), msg.time_from_start);
    }

    if (!success)
    {
      ROS_ERROR_STREAM("Failed to decode " << ros::message_traits::datatype(msg));
    }

    return success;
  }
};

template <>
struct Property<trajectory_msgs::JointTrajectory>
{
  template<typename ComponentT>
  static void encode(const trajectory_msgs::JointTrajectory& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<std_msgs::Header>::encode<Component>(msg.header, "header", q_item);
    Property<std::vector<QString>>::encode(toQStringVector(msg.joint_names), "joint_names", q_item);
    Property<std::vector<trajectory_msgs::JointTrajectoryPoint>>::encode(msg.points, "points", q_item);
  }

  static bool decode(const Component* item, trajectory_msgs::JointTrajectory& msg)
  {
    bool success = false;
    if (item->rowCount() == 3)
    {
      success = true;
      success &= Property<std_msgs::Header>::decode(static_cast<Component*>(item->child(0)), msg.header);

      std::vector<QString> v_qstring;
      success &= Property<std::vector<QString>>::decode(static_cast<Component*>(item->child(1)), v_qstring);
      msg.joint_names = fromQStringVector(v_qstring);

      success &= Property<std::vector<trajectory_msgs::JointTrajectoryPoint>>::decode(
          static_cast<Component*>(item->child(2)), msg.points);
    }

    if (!success)
    {
      ROS_ERROR_STREAM("Failed to decode " << ros::message_traits::datatype(msg));
    }

    return success;
  }
};

} // namespace qt_tree_editor

#endif // QT_TREE_EDITOR_ROS_TRAJECTORY_MSGS_PROPERTIES_HPP
