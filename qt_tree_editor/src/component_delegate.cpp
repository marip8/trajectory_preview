#include "qt_tree_editor/component_delegate.h"
#include <QComboBox>
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
  if(!enum_map.isNull())
  {
    QComboBox* box = new QComboBox(parent);

    box->addItems(enum_map.value<QMap<int, QString>>().values());
    box->setFrame(false);
    box->setMaximumHeight(30);
    return box;
  }

  return QStyledItemDelegate::createEditor(parent, option, index);
}

void ComponentDelegate::setEditorData(QWidget* editor,
                                      const QModelIndex& index) const
{
  QVariant enum_map = index.data(EnumRole);
  if(!enum_map.isNull())
  {
    QMap<int, QString> map = enum_map.value<QMap<int, QString>>();
    QVariant data = index.data(Qt::EditRole);
    if(!data.isNull())
    {
      auto it = map.find(data.toInt());
      if(it != map.end())
      {
        QComboBox* box = static_cast<QComboBox*>(editor);
        box->setCurrentText(it.value());
      }
    }
  }

  QStyledItemDelegate::setEditorData(editor, index);
}

void ComponentDelegate::setModelData(QWidget* editor,
                                     QAbstractItemModel* model,
                                     const QModelIndex& index) const
{
  QVariant enum_map = index.data(EnumRole);
  if(!enum_map.isNull())
  {
    QMap<int, QString> map = enum_map.value<QMap<int, QString>>();

    QComboBox* box = static_cast<QComboBox*>(editor);
    int key = map.key(box->currentText(), -1);
    if(key >= 0)
    {
      if(!model->setData(index, key))
      {
        ROS_WARN_STREAM("Didn't set the data");
      }
    }
  }

  QStyledItemDelegate::setModelData(editor, model, index);
}
