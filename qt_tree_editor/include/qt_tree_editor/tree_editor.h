#ifndef QT_TREE_EDITOR_TREE_EDITOR_H
#define QT_TREE_EDITOR_TREE_EDITOR_H

#include <QModelIndex>

class QFrame;
class QTreeView;
class QVBoxLayout;
class QWidget;
class QStandardItemModel;

namespace qt_tree_editor
{
template <typename T>
class TreeEditor
{
public:
  TreeEditor() = default;

  virtual ~TreeEditor();

  QWidget* createEditor(const T& msg, const QString& name);

  bool get(T& msg);

  inline const QTreeView* getComponentView() const
  {
    return component_view_;
  }

  inline const QTreeView* getPropertyView() const
  {
    return property_view_;
  }

private:
  void onTreeSelectionChanged(const QModelIndex& index);

  void setupUi(QWidget* parent);

  void onCustomContextMenu(const QPoint& point);

  QVBoxLayout* vertical_layout_;
  QTreeView* component_view_;
  QTreeView* property_view_;
  QFrame* line_;
  QWidget* widget_;

  QStandardItemModel* model_;
};

}  // namespace qt_tree_editor

#endif  // QT_TREE_EDITOR_TREE_EDITOR_H
