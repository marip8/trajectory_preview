#ifndef VECTOR_CHILD_COMPONENT_H
#define VECTOR_CHILD_COMPONENT_H

#include "qt_tree_editor/component/component.h"

class QMenu;

namespace qt_tree_editor
{

class VectorChildComponent : public Component
{
  using Component::Component;

  public:

  virtual QMenu* getContextMenu(const QModelIndex& index) override;

  private:

  void remove(const bool,
              const QModelIndex& index);
};

} // namespace qt_tree_editor

#endif // VECTOR_CHILD_COMPONENT_H
