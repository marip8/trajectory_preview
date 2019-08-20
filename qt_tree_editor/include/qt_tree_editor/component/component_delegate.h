#ifndef QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H
#define QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H

#include <QStyledItemDelegate>

namespace qt_tree_editor
{

class ComponentDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  public:
  using QStyledItemDelegate::QStyledItemDelegate;

  virtual void paint(QPainter* painter,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) const override;
};

} // namespace qt_tree_editor

#endif // QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H
