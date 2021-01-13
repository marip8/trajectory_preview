#ifndef QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/property.hpp"
#include <std_msgs/Header.h>
#include <ros/console.h>
#include "qt_tree_editor/user_roles.h"

namespace qt_tree_editor
{
template <>
struct Property<ros::Time>
{
  template <typename ComponentT>
  static void encode(const ros::Time& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype("ros::Time");
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    q_item->addProperty("sec", QVariant(QVariant::Int, &msg.sec));
    q_item->addProperty("nsec", QVariant(QVariant::Int, &msg.nsec));
  }

  static bool decode(const Component* item, ros::Time& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if (model->rowCount() == 2)
    {
      bool sec, nsec;
      msg.sec = model->data(model->index(0, 1)).toInt(&sec);
      msg.nsec = model->data(model->index(1, 1)).toInt(&nsec);

      return (sec && nsec);
    }

    ROS_ERROR_STREAM("Failed to decode ros::Time");

    return false;
  }
};

template <>
struct Property<ros::Duration>
{
  template <typename ComponentT>
  static void encode(const ros::Duration& msg, const QString& name, QStandardItem* parent)
  {
    // Convert to time and use its implementation
    Property<ros::Time>::encode<Component>(ros::Time(msg.sec, msg.nsec), name, parent);
  }

  static bool decode(const Component* item, ros::Duration& msg)
  {
    ros::Time time_msg;
    bool success = Property<ros::Time>::decode(item, time_msg);
    if (success)
    {
      msg.sec = time_msg.sec;
      msg.nsec = time_msg.nsec;
    }
    else
    {
      ROS_ERROR_STREAM("Failed to decode ros::Duration");
    }

    return success;
  }
};

template <>
struct Property<std_msgs::Header>
{
  template <typename ComponentT>
  static void encode(const std_msgs::Header& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* header_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    header_item->addProperty("seq", QVariant(QVariant::UInt, &msg.seq));
    header_item->addProperty("frame_id", QVariant::fromValue(QString::fromStdString(msg.frame_id)));
    Property<ros::Time>::encode<Component>(msg.stamp, "stamp", header_item);
  }

  static bool decode(const Component* item, std_msgs::Header& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    bool success = false;
    if (model->rowCount() == 2)
    {
      success = true;
      bool seq;
      msg.seq = model->data(model->index(0, 1)).toDouble(&seq);
      msg.frame_id = model->data(model->index(1, 1)).toString().toStdString();

      success &= seq;
    }

    success &= Property<ros::Time>::decode(static_cast<Component*>(item->child(0)), msg.stamp);

    if (!success)
    {
      ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));
    }

    return success;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP
