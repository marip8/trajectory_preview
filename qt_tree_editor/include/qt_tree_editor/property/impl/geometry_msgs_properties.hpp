#ifndef QT_TREE_EDITOR_GEOMETRY_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_GEOMETRY_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/impl/stl_properties.hpp"
#include "qt_tree_editor/property/impl/std_msgs_properties.hpp"
#include <geometry_msgs/PoseArray.h>

template<>
struct Property<geometry_msgs::Quaternion>
{
  static void encode(const geometry_msgs::Quaternion& msg,
                     const QString& name,
                     QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    q_item->addProperty("qw", QVariant(QVariant::Double, &msg.w));
    q_item->addProperty("qx", QVariant(QVariant::Double, &msg.x));
    q_item->addProperty("qy", QVariant(QVariant::Double, &msg.y));
    q_item->addProperty("qz", QVariant(QVariant::Double, &msg.z));
  }

  static bool decode(const Component* item,
                     geometry_msgs::Quaternion& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if(model->rowCount() == 4)
    {
      bool w, x, y, z;
      msg.w = model->data(model->index(0, 1)).toDouble(&w);
      msg.x = model->data(model->index(1, 1)).toDouble(&x);
      msg.y = model->data(model->index(2, 1)).toDouble(&y);
      msg.z = model->data(model->index(3, 1)).toDouble(&z);

      return w && x && y && z;
    }

    std::stringstream ss;
    ss << "Failed to decode" << ros::message_traits::datatype(msg);
    CONSOLE_BRIDGE_logError(ss.str().c_str());

    return false;
  }
};

template<>
struct Property<geometry_msgs::Point>
{
  static void encode(const geometry_msgs::Point& msg,
                     const QString& name,
                     QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair(name, datatype));

    Component* q_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    q_item->addProperty("x", QVariant(QVariant::Double, &msg.x));
    q_item->addProperty("y", QVariant(QVariant::Double, &msg.y));
    q_item->addProperty("z", QVariant(QVariant::Double, &msg.z));
  }

  static bool decode(const Component* item,
                     geometry_msgs::Point& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if(model->rowCount() == 3)
    {
      bool x, y, z;
      msg.x = model->data(model->index(0, 1)).toDouble(&x);
      msg.y = model->data(model->index(1, 1)).toDouble(&y);
      msg.z = model->data(model->index(2, 1)).toDouble(&z);

      return x && y && z;
    }

    std::stringstream ss;
    ss << "Failed to decode" << ros::message_traits::datatype(msg);
    CONSOLE_BRIDGE_logError(ss.str().c_str());

    return false;
  }
};

template<>
struct Property<geometry_msgs::Pose>
{
  static void encode(const geometry_msgs::Pose& msg,
                     const QString& name,
                     QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair(name, datatype));

    Component* pose_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<geometry_msgs::Point>::encode(msg.position, "position", pose_item);
    Property<geometry_msgs::Quaternion>::encode(msg.orientation, "orientation", pose_item);
  }

  static bool decode(const Component* item,
                     geometry_msgs::Pose& msg)
  {
    if(item->rowCount() == 2)
    {
      bool point = Property<geometry_msgs::Point>::decode(static_cast<Component*>(item->child(0)), msg.position);
      bool quat = Property<geometry_msgs::Quaternion>::decode(static_cast<Component*>(item->child(1)), msg.orientation);

      return point && quat;
    }

    std::stringstream ss;
    ss << "Failed to decode" << ros::message_traits::datatype(msg);
    CONSOLE_BRIDGE_logError(ss.str().c_str());

    return false;
  }
};

template<>
struct Property<geometry_msgs::PoseArray>
{
  static void encode(const geometry_msgs::PoseArray& msg,
                     const QString& name,
                     QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair(name, datatype));

    Component* pose_array_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    Property<std_msgs::Header>::encode(msg.header, "header", pose_array_item);
    Property<std::vector<geometry_msgs::Pose>>::encode(msg.poses, "poses", pose_array_item);
  }

  static bool decode(const Component* item,
                     geometry_msgs::PoseArray& msg)
  {
    bool header = Property<std_msgs::Header>::decode(static_cast<Component*>(item->child(0)), msg.header);
    bool poses = Property<std::vector<geometry_msgs::Pose>>::decode(static_cast<Component*>(item->child(1)), msg.poses);
    bool success = poses && header;

    if(!success)
    {
      std::stringstream ss;
      ss << "Failed to decode" << ros::message_traits::datatype(msg);
      CONSOLE_BRIDGE_logError(ss.str().c_str());
    }

    return success;
  }
};

#endif // QT_TREE_EDITOR_GEOMETRY_MSGS_PROPERTIES_HPP
