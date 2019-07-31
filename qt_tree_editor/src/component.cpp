#include "qt_tree_editor/component/component.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include <functional>

Component::Component()
  : QStandardItem()
  , properties_(new QStandardItemModel())
{
}

Component::Component(const QString& text)
  : QStandardItem(text)
  , properties_(new QStandardItemModel())
{

}

Component::~Component()
{
  delete properties_;
}

void Component::addProperty(const QString& name,
                            const QVariant& value,
                            const int role)
{
  QStandardItem* name_item = new QStandardItem(name);
  name_item->setEditable(false);

  if(value.canConvert<QList<QVariant>>())
  {
    // Append a "header" row for the list including the values of all
    properties_->appendRow({name_item, new QStandardItem(value.toString())});

    QStandardItem* header = properties_->item(properties_->rowCount() - 1);

    QVariantList list = value.toList();
    QList<QStandardItem*> items;
    for(int i = 0; i < list.size(); ++i)
    {
      std::string name = "[" + std::to_string(i) + "]";
      QStandardItem* tmp_name = new QStandardItem(QString::fromStdString(name));

      QStandardItem* item = new QStandardItem();
      item->setData(list[i], role);

      header->appendRow({tmp_name, item});
    }
  }
  else
  {
    QStandardItem* value_item = new QStandardItem();
    value_item->setData(value, role);
    properties_->appendRow({name_item, value_item});
  }
}

QStandardItemModel* Component::getPropertyModel() const
{
  return properties_;
}

int Component::type() const
{
  return UserType + 1;
}

QMenu* Component::getContextMenu(const QModelIndex&)
{
  return nullptr;
}
