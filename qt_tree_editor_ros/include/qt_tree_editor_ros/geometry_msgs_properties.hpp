#ifndef QT_TREE_EDITOR_ROS_GEOMETRY_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_ROS_GEOMETRY_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/vector_property.hpp"
#include "qt_tree_editor_ros/std_msgs_properties.hpp"
#include <geometry_msgs/PoseArray.h>

namespace qt_tree_editor
{
template <>
struct Property<geometry_msgs::Quaternion>
{
  template <typename ComponentT>
  static void encode(const geometry_msgs::Quaternion& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    q_item->addProperty("qw", QVariant(QVariant::Double, &msg.w));
    q_item->addProperty("qx", QVariant(QVariant::Double, &msg.x));
    q_item->addProperty("qy", QVariant(QVariant::Double, &msg.y));
    q_item->addProperty("qz", QVariant(QVariant::Double, &msg.z));
  }

  static bool decode(const Component* item, geometry_msgs::Quaternion& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if (model->rowCount() == 4)
    {
      bool w, x, y, z;
      msg.w = model->data(model->index(0, 1)).toDouble(&w);
      msg.x = model->data(model->index(1, 1)).toDouble(&x);
      msg.y = model->data(model->index(2, 1)).toDouble(&y);
      msg.z = model->data(model->index(3, 1)).toDouble(&z);

      return w && x && y && z;
    }

    ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));

    return false;
  }
};

template <>
struct Property<geometry_msgs::Point>
{
  template <typename ComponentT>
  static void encode(const geometry_msgs::Point& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    q_item->addProperty("x", QVariant(QVariant::Double, &msg.x));
    q_item->addProperty("y", QVariant(QVariant::Double, &msg.y));
    q_item->addProperty("z", QVariant(QVariant::Double, &msg.z));
  }

  static bool decode(const Component* item, geometry_msgs::Point& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if (model->rowCount() == 3)
    {
      bool x, y, z;
      msg.x = model->data(model->index(0, 1)).toDouble(&x);
      msg.y = model->data(model->index(1, 1)).toDouble(&y);
      msg.z = model->data(model->index(2, 1)).toDouble(&z);

      return x && y && z;
    }

    ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));

    return false;
  }
};

template <>
struct Property<geometry_msgs::Pose>
{
  template <typename ComponentT>
  static void encode(const geometry_msgs::Pose& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* pose_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<geometry_msgs::Point>::encode<Component>(msg.position, "position", pose_item);
    Property<geometry_msgs::Quaternion>::encode<Component>(msg.orientation, "orientation", pose_item);
  }

  static bool decode(const Component* item, geometry_msgs::Pose& msg)
  {
    if (item->rowCount() == 2)
    {
      bool point = Property<geometry_msgs::Point>::decode(static_cast<Component*>(item->child(0)), msg.position);
      bool quat = Property<geometry_msgs::Quaternion>::decode(static_cast<Component*>(item->child(1)), msg.orientation);

      return point && quat;
    }

    ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));

    return false;
  }
};

template <>
struct Property<geometry_msgs::PoseArray>
{
  template <typename ComponentT>
  static void encode(const geometry_msgs::PoseArray& msg, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* pose_array_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<std_msgs::Header>::encode<Component>(msg.header, "header", pose_array_item);
    VectorTreeProperty<geometry_msgs::Pose>::encode<Component>(msg.poses, "poses", pose_array_item);
  }

  static bool decode(const Component* item, geometry_msgs::PoseArray& msg)
  {
    bool success = false;
    if(item->rowCount() == 2)
    {
      bool header = Property<std_msgs::Header>::decode(static_cast<Component*>(item->child(0)), msg.header);
      bool poses = VectorTreeProperty<geometry_msgs::Pose>::decode(static_cast<Component*>(item->child(1)), msg.poses);
      success = poses && header;
    }

    if (!success)
    {
      ROS_ERROR_STREAM("Failed to decode " << ros::message_traits::datatype(msg));
    }

    return success;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_ROS_GEOMETRY_MSGS_PROPERTIES_HPP
