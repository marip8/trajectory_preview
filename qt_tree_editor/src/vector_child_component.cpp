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

  // Rename the parent's other children
  for (int i = 0; i < parent_item->rowCount(); ++i)
  {
    QStandardItem* child = parent_item->child(i, 0);
    std::string name = "[" + std::to_string(i) + "]";
    child->setText(QString::fromStdString(name));
  }
}

}  // namespace qt_tree_editor

#endif  // VECTOR_CHILD_COMPONENT_HPP
