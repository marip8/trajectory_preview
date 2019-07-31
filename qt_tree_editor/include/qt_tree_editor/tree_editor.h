#ifndef QT_TREE_EDITOR_TREE_EDITOR_H
#define QT_TREE_EDITOR_TREE_EDITOR_H

#include <QModelIndex>

class QFrame;
class QTreeView;
class QVBoxLayout;
class QWidget;
class QStandardItemModel;

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

  void setupUi(QWidget* parent);

  QVBoxLayout* vertical_layout_;
  QTreeView* component_view_;
  QTreeView* property_view_;
  QFrame* line_;
  QWidget* widget_;

  QStandardItemModel* model_;
};

#endif // QT_TREE_EDITOR_TREE_EDITOR_H
