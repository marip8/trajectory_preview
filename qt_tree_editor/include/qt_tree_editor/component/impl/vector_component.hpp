#include "qt_tree_editor/component/vector_component.h"
#include "qt_tree_editor/component/vector_child_component.h"
#include "qt_tree_editor/property/property.hpp"
#include <QMenu>
#include <QApplication>
#include <functional>

namespace qt_tree_editor
{
template <typename T>
QMenu* VectorComponent<T>::getContextMenu(const QModelIndex&)
{
  QMenu* menu = new QMenu();

  QAction* add_action = menu->addAction("Add");
  add_action->setShortcut(QKeySequence::New);
  QApplication::connect(add_action, &QAction::triggered,
                        std::bind(&VectorComponent<T>::addChild, this, std::placeholders::_1));

  QAction* remove_all_action = menu->addAction("Remove All Children");
  remove_all_action->setShortcut(QKeySequence::Delete);
  remove_all_action->setStatusTip("Remove all children");
  QApplication::connect(remove_all_action, &QAction::triggered,
                        std::bind(&VectorComponent<T>::removeAllChildren, this, std::placeholders::_1));

  return menu;
}

template <typename T>
void VectorComponent<T>::removeAllChildren(const bool)
{
  removeRows(0, rowCount());
}

template <typename T>
void VectorComponent<T>::addChild(const bool)
{
  Type msg;
  const std::string name = "[" + std::to_string(rowCount()) + "]";
  Property<Type>::template encode<VectorChildComponent>(msg, QString::fromStdString(name), this);
}

}  // namespace qt_tree_editor
