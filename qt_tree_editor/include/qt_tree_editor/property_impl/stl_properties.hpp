#ifndef QT_TREE_EDITOR_PROPERTY_IMPL_STL_PROPERTIES_HPP
#define QT_TREE_EDITOR_PROPERTY_IMPL_STL_PROPERTIES_HPP

#include "qt_tree_editor/property.h"
#include <console_bridge/console.h>
#include <sstream>
#include <vector>

template<typename T>
struct Property<std::vector<T>>
{
  static void encode(const std::vector<T>& vec,
                     const QString& name,
                     QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype("vector");
    parent->appendRow(createPair(name, datatype));

    Component* item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    if(!vec.empty())
    {
      for(std::size_t i = 0; i < vec.size(); ++i)
      {
        QString name = QString::fromStdString("[" + std::to_string(i) + "]");
        Property<T>::encode(vec[i], name, item);
      }
    }
  }

  static bool decode(const Component* item,
                     std::vector<T>& vec)
  {
    vec.clear();
    vec.resize(item->rowCount() - 1);
    bool success = true;
    for(std::size_t i = 0; i < vec.size(); ++i)
    {
      success &= Property<T>::decode(static_cast<Component*>(item->child(i + 1)), vec[i]);
    }

    if(!success)
    {
      std::stringstream ss;
      ss << "Failed to decode vector";
      CONSOLE_BRIDGE_logError(ss.str().c_str());
    }

    return success;
  }
};

#endif // QT_TREE_EDITOR_PROPERTY_IMPL_STL_PROPERTIES_HPP
