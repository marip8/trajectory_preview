#ifndef VECTOR_COMPONENT_H
#define VECTOR_COMPONENT_H

#include "qt_tree_editor/component/component.h"

template<typename T>
class VectorComponent : public Component
{
  using Component::Component;

public:
  typedef typename T::value_type Type;

  virtual QMenu* getContextMenu(const QModelIndex&) override;

private:

  void addChild(const bool);

  void removeAllChildren(const bool);
};

#endif // VECTOR_COMPONENT_H
