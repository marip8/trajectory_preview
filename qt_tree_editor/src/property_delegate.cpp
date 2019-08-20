#include "qt_tree_editor/property/property_delegate.h"
#include <QComboBox>
#include <QFileDialog>
#include <QPainter>
#include <ros/console.h>

QWidget* PropertyDelegate::createEditor(QWidget* parent,
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
    connect(file_dialog, &QFileDialog::fileSelected, this, &PropertyDelegate::onFileSelected);
    return file_dialog;
  }

  return QStyledItemDelegate::createEditor(parent, option, index);
}

void PropertyDelegate::setEditorData(QWidget* editor,
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

void PropertyDelegate::setModelData(QWidget* editor,
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

void PropertyDelegate::onFileSelected(const QString& file)
{
  file_path_ = file;
}

void PropertyDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
  QStyledItemDelegate::paint(painter, option, index);

  painter->save();

  QPen pen;
  pen.setColor(Qt::gray);
  pen.setStyle(Qt::DotLine);
  painter->setPen(pen);

  painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
  painter->drawLine(option.rect.topRight(), option.rect.bottomRight());

  painter->restore();
}
