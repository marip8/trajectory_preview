#include <QApplication>
#include <qt_tree_editor/tree_editor.hpp>
#include "qt_tree_editor_ros/geometry_msgs_properties.hpp"
#include "qt_tree_editor_ros/shape_msgs_properties.hpp"
#include "qt_tree_editor_ros/trajectory_msgs_properties.hpp"
#include "qt_tree_editor_ros/test/data.h"
#include <gtest/gtest.h>

// Define a test fixture class template.
template <class T>
class TreeEditorROSTest : public testing::Test
{
public:
  using testing::Test::Test;

  qt_tree_editor::TreeEditor<T> editor;
};

// Define a test fixture class for the vector tree editor
template <class T>
class VectorTreeEditorROSTest : public testing::Test
{
public:
  using testing::Test::Test;

  qt_tree_editor::TreeEditor<std::vector<T>> editor;
};

typedef ::testing::Types<ros::Time, ros::Duration, std_msgs::Header, trajectory_msgs::JointTrajectoryPoint,
                         trajectory_msgs::JointTrajectory, shape_msgs::SolidPrimitive, geometry_msgs::Point,
                         geometry_msgs::Quaternion, geometry_msgs::Pose, geometry_msgs::PoseArray,
                         geometry_msgs::Vector3>
    Implementations;

TYPED_TEST_CASE(TreeEditorROSTest, Implementations);
TYPED_TEST_CASE(VectorTreeEditorROSTest, Implementations);

TYPED_TEST(TreeEditorROSTest, test)
{
  namespace qte_ros_test = qt_tree_editor_ros::test;

  TypeParam msg = qte_ros_test::Data<TypeParam>::create();

  QWidget* widget = this->editor.createEditor(msg, "test");
  ASSERT_TRUE(widget != nullptr);

  TypeParam new_msg;
  ASSERT_TRUE(this->editor.get(new_msg));

  ASSERT_TRUE(qte_ros_test::Data<TypeParam>::equal(msg, new_msg));
}

TYPED_TEST(VectorTreeEditorROSTest, vtest)
{
  namespace qte_ros_test = qt_tree_editor_ros::test;

  std::vector<TypeParam> v;
  const std::size_t n = 3;
  v.reserve(n);
  for (std::size_t i = 0; i < n; ++i)
  {
    v.push_back(qte_ros_test::Data<TypeParam>::create());
  }

  QWidget* widget = this->editor.createEditor(v, "vtest");
  ASSERT_TRUE(widget != nullptr);

  std::vector<TypeParam> new_v;
  ASSERT_TRUE(this->editor.get(new_v));

  ASSERT_EQ(v.size(), new_v.size());

  for (std::size_t i = 0; i < v.size(); ++i)
  {
    ASSERT_TRUE(qte_ros_test::Data<TypeParam>::equal(v[i], new_v[i]));
  }
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
