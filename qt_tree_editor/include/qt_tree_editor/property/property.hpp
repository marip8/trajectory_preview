#ifndef QT_TREE_EDITOR_PROPERTY_HPP
#define QT_TREE_EDITOR_PROPERTY_HPP

#include <QList>
#include <QStandardItem>
#include "qt_tree_editor/component/component.h"

#include <typeinfo>

namespace qt_tree_editor
{
template <typename ComponentType>
inline QList<QStandardItem*> createPair(const QString& name, const QString& datatype)
{
  QList<QStandardItem*> list;
  list << new ComponentType(name) << new ComponentType(datatype);

  list[0]->setEditable(false);
  list[1]->setEditable(false);

  return list;
}

template <typename T>
struct Property
{
  template <typename ComponentT = Component>
  static void encode(const T& msg, const QString& name, QStandardItem* parent)
  {
    QString datatype(typeid(T).name());
    parent->appendRow(createPair<ComponentT>(name, datatype));

    Component* item = static_cast<Component*>(parent->child(parent->rowCount() - 1));

    item->addProperty(datatype, QVariant::fromValue(msg));
  }

  static bool decode(const Component* item, T& msg)
  {
    const QStandardItemModel* model = item->getPropertyModel();
    if (model->rowCount() == 1)
    {
      msg = model->data(model->index(0, 1)).value<T>();
      return true;
    }
    return false;
  }
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_PROPERTY_HPP
