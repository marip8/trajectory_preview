#ifndef VECTOR_CHILD_COMPONENT_HPP
#define VECTOR_CHILD_COMPONENT_HPP

#include "qt_tree_editor/component/vector_child_component.h"
#include <QApplication>
#include <QMenu>
#include <functional>

namespace qt_tree_editor
{
QMenu* VectorChildComponent::getContextMenu(const QModelIndex& index)
{
  QMenu* menu = new QMenu();

  QAction* remove_one_action = menu->addAction("Remove");
  remove_one_action->setShortcut(QKeySequence::Delete);
  QApplication::connect(remove_one_action, &QAction::triggered,
                        std::bind(&VectorChildComponent::remove, this, std::placeholders::_1, index));

  return menu;
}

void VectorChildComponent::remove(const bool, const QModelIndex& index)
{
  QStandardItem* parent_item = parent();
  parent_item->removeRow(index.row());
}

}  // namespace qt_tree_editor

#endif  // VECTOR_CHILD_COMPONENT_HPP
