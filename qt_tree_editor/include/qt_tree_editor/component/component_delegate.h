#ifndef QT_TREE_EDITOR_COMPONENT_DELEGATE_H
#define QT_TREE_EDITOR_COMPONENT_DELEGATE_H

#include "qt_tree_editor/user_roles.h"
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

class ComponentDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:

  ComponentDelegate(QObject* parent = nullptr);

  virtual QWidget* createEditor(QWidget* parent,
                                const QStyleOptionViewItem&,
                                const QModelIndex&) const override;

  virtual void setEditorData(QWidget* editor,
                             const QModelIndex& index) const override;

  virtual void setModelData(QWidget* editor,
                            QAbstractItemModel* model,
                            const QModelIndex& index) const override;
};

#endif // QT_TREE_EDITOR_COMPONENT_DELEGATE_H
