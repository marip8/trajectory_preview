#ifndef QT_TREE_EDITOR_TREE_EDITOR_HPP
#define QT_TREE_EDITOR_TREE_EDITOR_HPP

#include "qt_tree_editor/component_delegate.h"
#include "qt_tree_editor/property.h"
#include "qt_tree_editor/tree_editor.h"
#include "ui_tree_editor.h"
#include <QWidget>
#include <QStandardItemModel>
#include <functional>

template<typename T>
TreeEditor<T>::~TreeEditor()
{
  delete widget_;
  delete model_;
}

template<typename T>
QWidget* TreeEditor<T>::createEditor(const T& msg,
                                     const QString& name)
{
  widget_ = new QWidget();
  ui_ = new Ui::TreeEditor();
  ui_->setupUi(widget_);

  model_ = new QStandardItemModel();
  model_->setHorizontalHeaderLabels({"Name", "Type"});
  QStandardItem* item = model_->invisibleRootItem();
  Property<T>::encode(msg, name, item);

  ui_->tree_view->setModel(model_);
  ui_->tree_view->expandAll();

  ui_->table_view->setItemDelegate(new ComponentDelegate());

  QApplication::connect(ui_->tree_view, &QTreeView::clicked, std::bind(&TreeEditor::onTreeSelectionChanged, this, std::placeholders::_1));

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
      ui_->table_view->setModel(prop->getPropertyModel());
    }
  }
}

template<typename T>
bool TreeEditor<T>::get(T& msg)
{
  return Property<T>::decode(static_cast<Component*>(model_->item(0, 0)), msg);
}

#endif // QT_TREE_EDITOR_TREE_EDITOR_HPP
