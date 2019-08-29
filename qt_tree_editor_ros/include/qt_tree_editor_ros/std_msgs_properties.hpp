#ifndef QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/property.h"
#include <std_msgs/Header.h>
#include <ros/console.h>
#include "qt_tree_editor/user_roles.h"

namespace qt_tree_editor
{
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
  }

  static bool decode(const Component* item, std_msgs::Header& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if (model->rowCount() == 2)
    {
      bool seq;
      msg.seq = model->data(model->index(0, 1)).toDouble(&seq);
      msg.frame_id = model->data(model->index(1, 1)).toString().toStdString();

      return seq;
    }

    ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));

    return false;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_ROS_STD_MSGS_PROPERTIES_HPP
