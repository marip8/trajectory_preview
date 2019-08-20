#ifndef QT_TREE_EDITOR_TREE_EDITOR_HPP
#define QT_TREE_EDITOR_TREE_EDITOR_HPP

#include "qt_tree_editor/property/property_delegate.h"
#include "qt_tree_editor/property/property.h"
#include "qt_tree_editor/component/component_delegate.h"
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

namespace qt_tree_editor
{

template<typename T>
TreeEditor<T>::~TreeEditor()
{
  delete widget_;
  delete model_;
}

template<typename T>
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

template<typename T>
QWidget* TreeEditor<T>::createEditor(const T& msg,
                                     const QString& name)
{
  widget_ = new QWidget();
  setupUi(widget_);

  model_ = new QStandardItemModel();
  model_->setHorizontalHeaderLabels({"Name", "Type"});
  QStandardItem* item = model_->invisibleRootItem();
  Property<T>::template encode<Component>(msg, name, item);

  component_view_->setModel(model_);
  component_view_->setContextMenuPolicy(Qt::CustomContextMenu);
  component_view_->setItemDelegate(new ComponentDelegate());
  component_view_->expandAll();
  for (int i = 0; i < model_->columnCount(); ++i)
  {
    component_view_->resizeColumnToContents(i);
  }

  property_view_->setItemDelegate(new PropertyDelegate());

  QApplication::connect(component_view_, &QTreeView::clicked, std::bind(&TreeEditor::onTreeSelectionChanged, this, std::placeholders::_1));
  QApplication::connect(component_view_, &QTreeView::customContextMenuRequested, std::bind(&TreeEditor::onCustomContextMenu, this, std::placeholders::_1));

  return widget_;
}

template<typename T>
void TreeEditor<T>::onTreeSelectionChanged(const QModelIndex& index)
{

  const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
  QModelIndex sibling = index.sibling(index.row(), 0);
  if(sibling.isValid())
  {
    /* Note:
     * When working with the QStandardDataModel, using QModelIndex::internalPointer() to get the pointer
     * of an item will not work correctly. Use QStandardModel::itemFromIndex instead
     */
    Component* prop = static_cast<Component*>(model->itemFromIndex(sibling));
    if(prop)
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

template<typename T>
bool TreeEditor<T>::get(T& msg)
{
  return Property<T>::decode(static_cast<Component*>(model_->item(0, 0)), msg);
}

template<typename T>
void TreeEditor<T>::onCustomContextMenu(const QPoint& point)
{
  QModelIndex index = component_view_->indexAt(point);
  if(index.isValid())
  {
    const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
    Component* item = static_cast<Component*>(model->itemFromIndex(index));
    QMenu* menu = item->getContextMenu(index);
    if(menu)
    {
      menu->exec(component_view_->viewport()->mapToGlobal(point));
    }
    delete menu;
  }
}

} // namespace qt_tree_editor

#endif // QT_TREE_EDITOR_TREE_EDITOR_HPP
