#ifndef QT_TREE_EDITOR_COMPONENT_H
#define QT_TREE_EDITOR_COMPONENT_H

#include <QStandardItem>
#include <QStandardItemModel>

class Component : public QStandardItem
{
public:

  explicit Component();
  Component(const QString& text);

  virtual ~Component();

  virtual int type() const override;

  QStandardItemModel* getPropertyModel() const;

  void addProperty(const QString& name,
                   const QVariant& value,
                   const int role = Qt::EditRole);

private:

  QStandardItemModel* properties_;
};

#endif // QT_TREE_EDITOR_COMPONENT_H
