#include "qt_tree_editor/component/component_delegate.h"
#include <QComboBox>
#include <QFileDialog>
#include <ros/console.h>

ComponentDelegate::ComponentDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{

}

QWidget* ComponentDelegate::createEditor(QWidget* parent,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
  QVariant enum_map = index.data(EnumRole);
  if(enum_map.isValid())
  {
    QComboBox* box = new QComboBox(parent);

    box->addItems(enum_map.value<QMap<int, QString>>().values());
    box->setFrame(false);
    box->setMaximumHeight(30);
    return box;
  }

  QVariant file_dialog_data = index.data(FileDialogRole);
  if(file_dialog_data.isValid())
  {
    QFileDialog* file_dialog = new QFileDialog(parent);
    connect(file_dialog, &QFileDialog::fileSelected, this, &ComponentDelegate::onFileSelected);
    return file_dialog;
  }

  return QStyledItemDelegate::createEditor(parent, option, index);
}

void ComponentDelegate::setEditorData(QWidget* editor,
                                      const QModelIndex& index) const
{
  // Attempt to get an enumeration map from the data enum role
  QVariant enum_map = index.data(EnumRole);
  if(enum_map.isValid())
  {
    // Use the currently selected index (i.e. the data in the edit role) to set the text of the combo box
    QString enum_value = index.data(Qt::EditRole).toString();

    QComboBox* box = static_cast<QComboBox*>(editor);
    box->setCurrentText(enum_value);
  }

  QStyledItemDelegate::setEditorData(editor, index);
}

void ComponentDelegate::setModelData(QWidget* editor,
                                     QAbstractItemModel* model,
                                     const QModelIndex& index) const
{
  QVariant enum_map = index.data(EnumRole);
  if(enum_map.isValid())
  {
    QComboBox* box = static_cast<QComboBox*>(editor);
    if(!model->setData(index, box->currentText(), Qt::EditRole))
    {
      ROS_WARN_STREAM("Failed to set enumeration data");
    }
  }

  QVariant file_dialog_data = index.data(FileDialogRole);
  if(file_dialog_data.isValid())
  {
    if(!model->setData(index, file_path_, Qt::EditRole))
    {
      ROS_WARN_STREAM("Failed to set file dialog data");
    }
    return;
  }

  QStyledItemDelegate::setModelData(editor, model, index);
}

void ComponentDelegate::onFileSelected(const QString& file)
{
  file_path_ = file;
}
