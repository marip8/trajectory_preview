#ifndef QT_TREE_EDITOR_PROPERTY_VECTOR_PROPERTY_HPP
#define QT_TREE_EDITOR_PROPERTY_VECTOR_PROPERTY_HPP

#include "qt_tree_editor/property/property.hpp"
#include "qt_tree_editor/component/impl/vector_component.hpp"
#include "qt_tree_editor/component/vector_child_component.h"
#include <vector>

namespace qt_tree_editor
{
/**
 * @brief Property struct specialization for encoding and decoding a vector of values into an tree hierarchy.
 */
template <typename T>
struct Property<std::vector<T>>
{
  /**
   * @ brief Specialization of the property encode function that allows the user to select whether or not the
   * vector tree is dynamically editable
   */
  template<typename ComponentT, bool editable = true>
  static void encode(const std::vector<T>& vec, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    if (editable)
    {
      using VectorT = std::vector<T>;
      std::string label = "dynamic_vector [" + std::to_string(vec.size()) + "]";
      parent->appendRow(createPair<VectorComponent<VectorT>>(name, QString::fromStdString(label)));
    }
    else
    {
      std::string label = "static_vector [" + std::to_string(vec.size()) + "]";
      parent->appendRow(createPair<ComponentT>(name, QString::fromStdString(label)));
    }

    Component* item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    if (!vec.empty())
    {
      for (std::size_t i = 0; i < vec.size(); ++i)
      {
        QString label = QString::fromStdString("[" + std::to_string(i) + "]");
        Property<T>::template encode<VectorChildComponent>(vec[i], label, item);
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
    }

    return success;
  }
};
template <typename T>
using VectorTreeProperty = Property<std::vector<T>>;

/**
 * @brief Property struct specialization for encoding and decoding a vector of values into a static
 * table of values. These values and their hierarchy are not editable by the user
 */
template <typename T>
struct VectorTableProperty : public Property<std::vector<T>>
{
  template <typename ComponentT = Component>
  static void encode(const std::vector<T>& vec, const QString& name, QStandardItem* parent)
  {
    // Add the top-level item
    QString datatype("table_vector");
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    // Add the child properties
    if (!vec.empty())
    {
      for (std::size_t i = 0; i < vec.size(); ++i)
      {
        QString name = QString::fromStdString("[" + std::to_string(i) + "]");
        item->addProperty(name, QVariant::fromValue(vec[i]));
      }
    }
  }

  static bool decode(const Component* item, std::vector<T>& vec)
  {
    const QStandardItemModel* model = item->getPropertyModel();

    bool success = true;

    vec.clear();
    if (model->rowCount() > 0)
    {
      vec.resize(model->rowCount());
      for (std::size_t i = 0; i < vec.size(); ++i)
      {
        vec[i] = model->data(model->index(i, 1)).value<T>();
      }
    }

    return success;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_PROPERTY_VECTOR_PROPERTY_HPP
