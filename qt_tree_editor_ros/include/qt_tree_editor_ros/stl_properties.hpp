#ifndef QT_TREE_EDITOR_ROS_STL_PROPERTIES_HPP
#define QT_TREE_EDITOR_ROS_STL_PROPERTIES_HPP

#include "qt_tree_editor/property/property.h"
#include "qt_tree_editor/component/impl/vector_component.hpp"
#include "qt_tree_editor/component/vector_child_component.h"
#include <ros/console.h>
#include <sstream>
#include <vector>

namespace qt_tree_editor
{
template <typename T>
struct Property<std::vector<T>>
{
  static void encode(const std::vector<T>& vec, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype("vector");
    using VectorT = std::vector<T>;
    parent->appendRow(createPair<VectorComponent<VectorT>>(name, datatype));

    Component* item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    if (!vec.empty())
    {
      for (std::size_t i = 0; i < vec.size(); ++i)
      {
        QString name = QString::fromStdString("[" + std::to_string(i) + "]");
        Property<T>::template encode<VectorChildComponent>(vec[i], name, item);
      }
    }
  }

  static bool decode(const Component* item, std::vector<T>& vec)
  {
    bool success = true;

    vec.clear();
    if (item->rowCount() > 0)
    {
      vec.resize(item->rowCount());
      for (std::size_t i = 0; i < vec.size(); ++i)
      {
        success &= Property<T>::decode(static_cast<Component*>(item->child(i)), vec[i]);
      }

      if (!success)
      {
        ROS_ERROR_STREAM("Failed to decode vector");
      }
    }

    return success;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_ROS_STL_PROPERTIES_HPP
