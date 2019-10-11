#include <QApplication>
#include <qt_tree_editor/tree_editor.hpp>
#include "qt_tree_editor_ros/geometry_msgs_properties.hpp"
#include "qt_tree_editor_ros/shape_msgs_properties.hpp"
#include "qt_tree_editor_ros/trajectory_msgs_properties.hpp"
#include <qt_tree_editor/tree_editor.hpp>
#include <gtest/gtest.h>

template <typename T>
struct Data
{
  static T create();
  static bool equal(const T& lhs, const T& rhs);
};

template <>
struct Data<ros::Time>
{
  static ros::Time create()
  {
    return ros::Time(1, 2);
  }

  static bool equal(const ros::Time& lhs, const ros::Time& rhs)
  {
    return lhs == rhs;
  }
};

template <>
struct Data<ros::Duration>
{
  static ros::Duration create()
  {
    return ros::Duration(2, 3);
  }

  static bool equal(const ros::Duration& lhs, const ros::Duration& rhs)
  {
    return lhs == rhs;
  }
};

template <>
struct Data<std_msgs::Header>
{
  static std_msgs::Header create()
  {
    std_msgs::Header h;
    h.seq = 10;
    h.stamp = Data<ros::Time>::create();
    h.frame_id = "test";
    return h;
  }

  static bool equal(const std_msgs::Header& lhs, const std_msgs::Header& rhs)
  {
    bool success = true;
    success &= lhs.seq == rhs.seq;
    success &= Data<ros::Time>::equal(lhs.stamp, rhs.stamp);
    success &= lhs.frame_id == rhs.frame_id;
    return success;
  }
};

template <>
struct Data<shape_msgs::SolidPrimitive>
{
  static shape_msgs::SolidPrimitive create()
  {
    shape_msgs::SolidPrimitive msg;
    msg.type = shape_msgs::SolidPrimitive::BOX;
    msg.dimensions.resize(3);
    msg.dimensions[shape_msgs::SolidPrimitive::BOX_X] = 1.0;
    msg.dimensions[shape_msgs::SolidPrimitive::BOX_Y] = 2.0;
    msg.dimensions[shape_msgs::SolidPrimitive::BOX_Z] = 3.0;
    return msg;
  }

  static bool equal(const shape_msgs::SolidPrimitive& lhs, const shape_msgs::SolidPrimitive& rhs)
  {
    bool success = true;
    success &= lhs.type == rhs.type;
    success &= lhs.dimensions == rhs.dimensions;
    return success;
  }
};

template <>
struct Data<geometry_msgs::Point>
{
  static geometry_msgs::Point create()
  {
    geometry_msgs::Point pt;
    pt.x = 1.0;
    pt.y = 2.0;
    pt.z = 3.0;
    return pt;
  }

  static bool equal(const geometry_msgs::Point& lhs, const geometry_msgs::Point& rhs)
  {
    bool success = true;
    success &= lhs.x == rhs.x;
    success &= lhs.y == rhs.y;
    success &= lhs.z == rhs.z;
    return success;
  }
};

template <>
struct Data<geometry_msgs::Quaternion>
{
  static geometry_msgs::Quaternion create()
  {
    geometry_msgs::Quaternion q;
    q.w = 1.0;
    q.x = 2.0;
    q.y = 3.0;
    q.z = 4.0;
    return q;
  }

  static bool equal(const geometry_msgs::Quaternion& lhs, const geometry_msgs::Quaternion& rhs)
  {
    bool success = true;
    success &= lhs.w == rhs.w;
    success &= lhs.x == rhs.x;
    success &= lhs.y == rhs.y;
    success &= lhs.z == rhs.z;
    return success;
  }
};

template <>
struct Data<geometry_msgs::Pose>
{
  static geometry_msgs::Pose create()
  {
    geometry_msgs::Pose p;
    p.position = Data<geometry_msgs::Point>::create();
    p.orientation = Data<geometry_msgs::Quaternion>::create();
    return p;
  }

  static bool equal(const geometry_msgs::Pose& lhs, const geometry_msgs::Pose& rhs)
  {
    bool success = true;
    success &= Data<geometry_msgs::Point>::equal(lhs.position, rhs.position);
    success &= Data<geometry_msgs::Quaternion>::equal(lhs.orientation, rhs.orientation);
    return success;
  }
};

template <>
struct Data<geometry_msgs::PoseArray>
{
  static geometry_msgs::PoseArray create()
  {
    geometry_msgs::PoseArray arr;
    arr.header = Data<std_msgs::Header>::create();
    const std::size_t n = 10;
    arr.poses.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      arr.poses.push_back(Data<geometry_msgs::Pose>::create());
    }
    return arr;
  }

  static bool equal(const geometry_msgs::PoseArray& lhs, const geometry_msgs::PoseArray& rhs)
  {
    bool success = true;
    success &= Data<std_msgs::Header>::equal(lhs.header, rhs.header);
    if (lhs.poses.size() == rhs.poses.size())
    {
      for (std::size_t i = 0; i < lhs.poses.size(); ++i)
      {
        success &= Data<geometry_msgs::Pose>::equal(lhs.poses[i], rhs.poses[i]);
      }
    }
    else
    {
      success &= false;
    }

    return success;
  }
};

template <>
struct Data<trajectory_msgs::JointTrajectoryPoint>
{
  static trajectory_msgs::JointTrajectoryPoint create()
  {
    trajectory_msgs::JointTrajectoryPoint pt;

    pt.positions.resize(6);
    std::iota(pt.positions.begin(), pt.positions.end(), 0);
    pt.velocities.resize(6);
    std::iota(pt.velocities.begin(), pt.velocities.end(), 0);
    pt.accelerations.resize(6);
    std::iota(pt.accelerations.begin(), pt.accelerations.end(), 0);
    pt.effort.resize(6);
    std::iota(pt.effort.begin(), pt.effort.end(), 0);

    pt.time_from_start = Data<ros::Duration>::create();

    return pt;
  }

  static bool equal(const trajectory_msgs::JointTrajectoryPoint& lhs, const trajectory_msgs::JointTrajectoryPoint& rhs)
  {
    bool success = true;
    success &= lhs.positions == rhs.positions;
    success &= lhs.velocities == rhs.velocities;
    success &= lhs.accelerations == rhs.accelerations;
    success &= lhs.effort == rhs.effort;
    success &= Data<ros::Duration>::equal(lhs.time_from_start, rhs.time_from_start);
    return success;
  }
};

template <>
struct Data<trajectory_msgs::JointTrajectory>
{
  static trajectory_msgs::JointTrajectory create()
  {
    trajectory_msgs::JointTrajectory msg;
    msg.header.stamp = Data<ros::Time>::create();
    msg.joint_names.insert(msg.joint_names.begin(), { "j1", "j2", "j3", "j4", "j5", "j6" });
    std::size_t n = 10;
    msg.points.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      msg.points.push_back(Data<trajectory_msgs::JointTrajectoryPoint>::create());
    }

    return msg;
  }

  static bool equal(const trajectory_msgs::JointTrajectory& lhs, const trajectory_msgs::JointTrajectory& rhs)
  {
    bool success = true;
    success &= Data<std_msgs::Header>::equal(lhs.header, rhs.header);
    success &= lhs.joint_names == rhs.joint_names;
    if (lhs.points.size() == rhs.points.size())
    {
      for (std::size_t i = 0; i < lhs.points.size(); ++i)
      {
        success &= Data<trajectory_msgs::JointTrajectoryPoint>::equal(lhs.points[i], rhs.points[i]);
      }
    }
    else
    {
      success &= false;
    }

    return success;
  }
};

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
                         geometry_msgs::Quaternion, geometry_msgs::Pose, geometry_msgs::PoseArray>
    Implementations;

TYPED_TEST_CASE(TreeEditorROSTest, Implementations);
TYPED_TEST_CASE(VectorTreeEditorROSTest, Implementations);

TYPED_TEST(TreeEditorROSTest, test)
{
  TypeParam msg = Data<TypeParam>::create();

  QWidget* widget = this->editor.createEditor(msg, "test");
  ASSERT_TRUE(widget != nullptr);

  TypeParam new_msg;
  ASSERT_TRUE(this->editor.get(new_msg));

  ASSERT_TRUE(Data<TypeParam>::equal(msg, new_msg));
}

TYPED_TEST(VectorTreeEditorROSTest, vtest)
{
  std::vector<TypeParam> v;
  const std::size_t n = 3;
  v.reserve(n);
  for (std::size_t i = 0; i < n; ++i)
  {
    v.push_back(Data<TypeParam>::create());
  }

  QWidget* widget = this->editor.createEditor(v, "vtest");
  ASSERT_TRUE(widget != nullptr);

  std::vector<TypeParam> new_v;
  ASSERT_TRUE(this->editor.get(new_v));

  ASSERT_EQ(v.size(), new_v.size());

  for (std::size_t i = 0; i < v.size(); ++i)
  {
    ASSERT_TRUE(Data<TypeParam>::equal(v[i], new_v[i]));
  }
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
