#ifndef QT_TREE_EDITOR_PROPERTY_H
#define QT_TREE_EDITOR_PROPERTY_H

#include <QList>
#include <QStandardItem>
#include "qt_tree_editor/component/component.h"

template<typename ComponentType>
inline QList<QStandardItem*> createPair(const QString& name,
                                        const QString& datatype)
{
  QList<QStandardItem*> list;
  list << new ComponentType(name)
       << new ComponentType(datatype);

  list[0]->setEditable(false);
  list[1]->setEditable(false);

  return list;
}

template<typename T>
struct Property
{
  template<typename ComponentT>
  static void encode(const T& msg,
                     const QString& name,
                     QStandardItem* parent);

  static bool decode(const Component* item,
                     T& msg);
};

#endif // QT_TREE_EDITOR_PROPERTY_H
