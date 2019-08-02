#ifndef QT_TREE_EDITOR_PROPERTY_IMPL_SHAPE_MSGS_PROPERTIES_HPP
#define QT_TREE_EDITOR_PROPERTY_IMPL_SHAPE_MSGS_PROPERTIES_HPP

#include "qt_tree_editor/property/property.h"
#include "qt_tree_editor/user_roles.h"
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
    // Create a map representing the enumeration
    QMap<int, QString> map;
    map.insert(msg.BOX,"BOX");
    map.insert(msg.SPHERE, "SPHERE");
    map.insert(msg.CYLINDER,"CYLINDER");
    map.insert(msg.CONE, "CONE");

    // Add the map as data in the enumeration role
    sp_item->addProperty("type", QVariant::fromValue(map), EnumRole);

    /* Add the data in the edit role (from message if valid, or first element in enumeration otherwise)
     * This data will be overwritten by the component delegate when a different enum is selected from the combo box
     */
    QStandardItemModel* model = sp_item->getPropertyModel();
    auto it = map.find(msg.type);
    if(it != map.end())
    {
      model->setData(model->index(0, 1), it.value(), Qt::EditRole);
    }
    else
    {
      model->setData(model->index(0, 1), map.values().front(), Qt::EditRole);
    }

    QVariantList list;
    if(!msg.dimensions.empty())
    {
      for(const shape_msgs::SolidPrimitive::_dimensions_type::value_type& val : msg.dimensions)
      {
        list << val;
      }
    }
    else
    {
      list << 0.0 << 0.0 << 0.0;
    }
    sp_item->addProperty("dimensions", list, Qt::EditRole);
  }

  static bool decode(const Component* item,
                     shape_msgs::SolidPrimitive& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    if(model->rowCount() == 2)
    {
      // 1. Get the type data
      // 1a. Get the enumeration data
      QVariant enum_data = model->data(model->index(0, 1), EnumRole);

      // 1b. Get the current enumeration value
      QVariant enum_value = model->data(model->index(0, 1), Qt::EditRole);

      if(enum_data.isValid() && enum_value.isValid())
      {
        // Get the key associated with the current enumeration value
        QMap<int, QString> map = enum_data.value<QMap<int, QString>>();
        int key = map.key(enum_value.toString(), -1);
        if(key >= 0)
        {
          msg.type = static_cast<shape_msgs::SolidPrimitive::_type_type>(key);
        }
        else
        {
          ROS_ERROR_STREAM("Failed to interpret enumeration data");
        }

        // 2. Get the dimension "sub" properties
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
                ROS_ERROR_STREAM("Dimension element data is invalid");
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
