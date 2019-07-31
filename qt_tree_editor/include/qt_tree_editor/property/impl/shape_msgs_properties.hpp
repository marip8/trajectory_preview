#ifndef QT_TREE_EDITOR_PROPERTY_IMPL_SHAPE_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_PROPERTY_IMPL_SHAPE_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/property.h"
#include <shape_msgs/SolidPrimitive.h>
#include <ros/console.h>

template<>
struct Property<shape_msgs::SolidPrimitive>
{
  template<typename ComponentT>
  static void encode(const shape_msgs::SolidPrimitive& msg,
                     const QString& name,
                     QStandardItem* parent)
  {
    QString datatype(ros::message_traits::datatype(msg));
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* sp_item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    QMap<int, QString> map;
    map.insert(msg.BOX,"BOX");
    map.insert(msg.SPHERE, "SPHERE");
    map.insert(msg.CYLINDER,"CYLINDER");
    map.insert(msg.CONE, "CONE");

    sp_item->addProperty("type", map.keys().front(), Qt::EditRole);

    QStandardItemModel* model = sp_item->getPropertyModel();
    model->setData(model->index(0, 1), QVariant::fromValue(map), Qt::UserRole + 3);

    QVariantList list;
    if(msg.dimensions.empty())
    {
      list << 0.0 << 0.0 << 0.0;
    }
    else
    {
      for(const shape_msgs::SolidPrimitive::_dimensions_type::value_type& val : msg.dimensions)
      {
        list << val;
      }
    }
    sp_item->addProperty("dimensions", list, Qt::EditRole);
  }

  static bool decode(const Component* item,
                     shape_msgs::SolidPrimitive& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if(model->rowCount() == 2)
    {
      // Get the type data
      QVariant type_var = model->data(model->index(0, 1), Qt::EditRole);
      if(type_var.isValid())
      {
        msg.type = type_var.value<shape_msgs::SolidPrimitive::_type_type>();

        // Get the dimension "sub" properties
        const QStandardItem* dimensions = model->item(1);
        if(dimensions)
        {
          const int n = dimensions->rowCount();
          if(n > 0)
          {
            msg.dimensions.clear();
            msg.dimensions.reserve(n);

            for(int i = 0; i < n; ++i)
            {
              QStandardItem* dim = dimensions->child(i, 1);
              QVariant var = dim->data(Qt::EditRole);
              if(var.isValid())
              {
                msg.dimensions.push_back(var.value<shape_msgs::SolidPrimitive::_dimensions_type::value_type>());
              }
              else
              {
                ROS_ERROR_STREAM("Invalid data");
              }
            }
          }

          return true;
        }
        else
        {
          ROS_ERROR_STREAM("Dimension data is not valid");
        }
      }
      else
      {
        ROS_ERROR_STREAM("Type data is not valid");
      }
    }

    ROS_ERROR_STREAM("Failed to decode" << ros::message_traits::datatype(msg));
    return false;
  }
};

#endif // QT_TREE_EDITOR_PROPERTY_IMPL_SHAPE_MSGS_PROPERTIES_HPP
