#ifndef QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H
#define QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H

#include <QStyledItemDelegate>

class ComponentDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
  using QStyledItemDelegate::QStyledItemDelegate;

  virtual void paint(QPainter* painter,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) const override;
};

#endif // QT_TREE_EDITOR_COMPONENT_COMPONENT_DELEGATE_H
