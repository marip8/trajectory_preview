#ifndef QT_TREE_EDITOR_PROPERTY_PROPERTY_DELEGATE_H
#define QT_TREE_EDITOR_PROPERTY_PROPERTY_DELEGATE_H

#include "qt_tree_editor/user_roles.h"
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

class PropertyDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
  using QStyledItemDelegate::QStyledItemDelegate;

  virtual QWidget* createEditor(QWidget* parent,
                                const QStyleOptionViewItem&,
                                const QModelIndex&) const override;

  virtual void setEditorData(QWidget* editor,
                             const QModelIndex& index) const override;

  virtual void setModelData(QWidget* editor,
                            QAbstractItemModel* model,
                            const QModelIndex& index) const override;

  virtual void paint(QPainter *painter,
                     const QStyleOptionViewItem &opt,
                     const QModelIndex &index) const override;

protected:

  void onFileSelected(const QString& file);

  QString file_path_;
};

#endif // QT_TREE_EDITOR_PROPERTY_PROPERTY_DELEGATE_H
