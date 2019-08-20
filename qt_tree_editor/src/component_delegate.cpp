#include "qt_tree_editor/component/component_delegate.h"
#include <QPainter>

void ComponentDelegate::paint(QPainter* painter,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
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
