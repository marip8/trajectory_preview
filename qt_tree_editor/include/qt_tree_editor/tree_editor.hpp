#ifndef QT_TREE_EDITOR_TREE_EDITOR_HPP
#define QT_TREE_EDITOR_TREE_EDITOR_HPP

#include "qt_tree_editor/property/property_delegate.h"
#include "qt_tree_editor/property/property.hpp"
#include "qt_tree_editor/component/component_delegate.h"
#include "qt_tree_editor/component/vector_child_component.h"
#include "qt_tree_editor/tree_editor.h"

#include <QStandardItemModel>
#include <functional>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>

#include <regex>

namespace qt_tree_editor
{
template <typename T>
TreeEditor<T>::~TreeEditor()
{
  delete widget_;
  delete model_;
}

template <typename T>
void TreeEditor<T>::setupUi(QWidget* parent)
{
  if (parent->objectName().isEmpty())
  {
    parent->setObjectName(QStringLiteral("TreeEditor"));
  }
  parent->resize(360, 570);

  vertical_layout_ = new QVBoxLayout(parent);
  vertical_layout_->setObjectName(QStringLiteral("verticalLayout"));

  component_view_ = new QTreeView(parent);
  component_view_->setObjectName(QStringLiteral("tree_view"));
  component_view_->setAlternatingRowColors(true);

  vertical_layout_->addWidget(component_view_);

  line_ = new QFrame(parent);
  line_->setObjectName(QStringLiteral("line"));
  line_->setFrameShape(QFrame::HLine);
  line_->setFrameShadow(QFrame::Sunken);

  vertical_layout_->addWidget(line_);

  property_view_ = new QTreeView(parent);
  property_view_->setObjectName(QStringLiteral("table_view"));
  property_view_->setAlternatingRowColors(true);

  vertical_layout_->addWidget(property_view_);

  parent->setWindowTitle(QApplication::translate("TreeEditor", "Form", 0));

  QMetaObject::connectSlotsByName(parent);
}

template <typename T>
QWidget* TreeEditor<T>::createEditor(const T& msg, const QString& name)
{
  widget_ = new QWidget();
  setupUi(widget_);

  model_ = new QStandardItemModel();
  model_->setHorizontalHeaderLabels({ "Name", "Type" });
  QStandardItem* item = model_->invisibleRootItem();
  Property<T>::template encode<Component>(msg, name, item);

  component_view_->setModel(model_);
  component_view_->setContextMenuPolicy(Qt::CustomContextMenu);
  component_view_->setItemDelegate(new ComponentDelegate());
  for (int i = 0; i < model_->columnCount(); ++i)
  {
    component_view_->resizeColumnToContents(i);
  }

  property_view_->setItemDelegate(new PropertyDelegate());

  QApplication::connect(
      component_view_->selectionModel(), &QItemSelectionModel::currentChanged,
      std::bind(&TreeEditor::onTreeSelectionChanged, this, std::placeholders::_1, std::placeholders::_2));

  QApplication::connect(component_view_, &QTreeView::customContextMenuRequested,
                        std::bind(&TreeEditor::onCustomContextMenu, this, std::placeholders::_1));

  QApplication::connect(
      model_, &QStandardItemModel::rowsRemoved,
      std::bind(&TreeEditor::onRowsRemoved, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  QApplication::connect(model_, &QStandardItemModel::rowsInserted,
                        std::bind(&TreeEditor::onRowsInserted, this, std::placeholders::_1, std::placeholders::_2,
                                  std::placeholders::_3));

  return widget_;
}

template <typename T>
void TreeEditor<T>::onTreeSelectionChanged(const QModelIndex& index, const QModelIndex&)
{
  const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
  QModelIndex sibling = index.sibling(index.row(), 0);
  if (sibling.isValid())
  {
    /* Note:
     * When working with the QStandardDataModel, using QModelIndex::internalPointer() to get the pointer
     * of an item will not work correctly. Use QStandardModel::itemFromIndex instead
     */
    Component* prop = static_cast<Component*>(model->itemFromIndex(sibling));
    if (prop)
    {
      property_view_->setModel(prop->getPropertyModel());
      property_view_->expandAll();
      for (int i = 0; i < model->columnCount(); ++i)
      {
        property_view_->resizeColumnToContents(i);
      }
    }
  }
}

template <typename T>
bool TreeEditor<T>::get(T& msg)
{
  return Property<T>::decode(static_cast<Component*>(model_->item(0, 0)), msg);
}

template <typename T>
void TreeEditor<T>::onCustomContextMenu(const QPoint& point)
{
  QModelIndex index = component_view_->indexAt(point);
  if (index.isValid())
  {
    const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
    Component* item = static_cast<Component*>(model->itemFromIndex(index));
    QMenu* menu = item->getContextMenu(index);
    if (menu)
    {
      menu->exec(component_view_->viewport()->mapToGlobal(point));
    }
    delete menu;
  }
}

template <typename T>
void TreeEditor<T>::updateVectorParentName(QStandardItem* parent_item)
{
  // Get the sibling item next to the input parent item. This item has the description of the vector
  QModelIndex sibling_index =
      parent_item->model()->sibling(parent_item->row(), parent_item->column() + 1, parent_item->index());
  QStandardItem* item = parent_item->model()->itemFromIndex(sibling_index);
  if (item)
  {
    // The vector description should be in the format <static/dynamic>_vector[<count>]
    // Replace the count
    const std::regex re("\\[(\\d*)\\]");
    const std::string current_name = item->data(Qt::DisplayRole).toString().toStdString();
    const std::string new_name =
        std::regex_replace(current_name, re, "[" + std::to_string(parent_item->rowCount()) + "]");
    item->setData(QString::fromStdString(new_name), Qt::DisplayRole);
  }
}

template <typename T>
void TreeEditor<T>::updateVectorElementNames(QStandardItem* parent_item)
{
  updateVectorParentName(parent_item);

  // Rename the parent's other children
  for (int i = 0; i < parent_item->rowCount(); ++i)
  {
    // We only want to rename vector child components, so dynamic cast to that type and check if it worked
    VectorChildComponent* child = dynamic_cast<VectorChildComponent*>(parent_item->child(i, 0));
    if (child)
    {
      std::string name = "[" + std::to_string(i) + "]";
      child->setText(QString::fromStdString(name));
    }
  }
}

template <typename T>
void TreeEditor<T>::onRowsRemoved(const QModelIndex& parent, int, int)

{
  QStandardItem* parent_item = model_->itemFromIndex(parent);
  if (parent_item)
  {
    updateVectorElementNames(parent_item);
  }
}

template <typename T>
void TreeEditor<T>::onRowsInserted(const QModelIndex& parent, int, int)
{
  QStandardItem* parent_item = model_->itemFromIndex(parent);
  if (parent_item)
  {
    updateVectorParentName(parent_item);
  }
}

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_TREE_EDITOR_HPP
