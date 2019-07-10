#ifndef QT_TREE_EDITOR_TREE_EDITOR_H
#define QT_TREE_EDITOR_TREE_EDITOR_H

#include <QModelIndex>

class QWidget;
class QStandardItemModel;

namespace Ui
{
class TreeEditor;
}

template<typename T>
class TreeEditor
{
public:

  TreeEditor() = default;

  virtual ~TreeEditor();

  QWidget* createEditor(const T& msg,
                        const QString& name);

  bool get(T& msg);

private:

  void onTreeSelectionChanged(const QModelIndex& index);

  Ui::TreeEditor* ui_;

  QWidget* widget_;

  QStandardItemModel* model_;
};

#endif // QT_TREE_EDITOR_TREE_EDITOR_H
