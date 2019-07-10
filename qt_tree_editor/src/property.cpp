#include "qt_tree_editor/component.h"

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
  QStandardItem* value_item = new QStandardItem();
  value_item->setData(value, role);
  properties_->appendRow({name_item, value_item});
}

QStandardItemModel* Component::getPropertyModel() const
{
  return properties_;
}

int Component::type() const
{
  return UserType + 1;
}
