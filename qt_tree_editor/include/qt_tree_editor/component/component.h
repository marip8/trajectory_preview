#ifndef QT_TREE_EDITOR_COMPONENT_H
#define QT_TREE_EDITOR_COMPONENT_H

#include <QStandardItem>

class QStandardItemModel;
class QMenu;

namespace qt_tree_editor
{
class Component : public QStandardItem
{
public:
  explicit Component();
  explicit Component(const QString& text);

  virtual ~Component();

  virtual int type() const override;

  QStandardItemModel* getPropertyModel() const;

  virtual QMenu* getContextMenu(const QModelIndex& index);

  void addProperty(const QString& name, const QVariant& value, const int role = Qt::EditRole);

protected:
  QStandardItemModel* properties_;
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_COMPONENT_H
