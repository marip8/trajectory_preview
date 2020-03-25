#ifndef QT_TREE_EDITOR_ROS_TEST_DATA_H
#define QT_TREE_EDITOR_ROS_TEST_DATA_H

#include <std_msgs/Header.h>
#include <shape_msgs/SolidPrimitive.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseArray.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <algorithm>

namespace qt_tree_editor_ros
{
namespace test
{
template <typename T>
struct Data
{
  static T create();
  static bool equal(const T& lhs, const T& rhs);
};

template <>
struct Data<ros::Time>
{
  using T = ros::Time;
  static T create()
  {
    return T(1, 2);
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
};

template <>
struct Data<ros::Duration>
{
  using T = ros::Duration;
  static T create()
  {
    return T(2, 3);
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
};

template <>
struct Data<std_msgs::Header>
{
  using T = std_msgs::Header;
  static T create()
  {
    T h;
    h.seq = 10;
    h.stamp = Data<T::_stamp_type>::create();
    h.frame_id = "test";
    return h;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.seq == rhs.seq;
    success &= Data<T::_stamp_type>::equal(lhs.stamp, rhs.stamp);
    success &= lhs.frame_id == rhs.frame_id;
    return success;
  }
};

template <>
struct Data<shape_msgs::SolidPrimitive>
{
  using T = shape_msgs::SolidPrimitive;
  static T create()
  {
    T msg;
    msg.type = T::BOX;
    msg.dimensions.resize(3);
    msg.dimensions[T::BOX_X] = 1.0;
    msg.dimensions[T::BOX_Y] = 2.0;
    msg.dimensions[T::BOX_Z] = 3.0;
    return msg;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.type == rhs.type;
    success &= lhs.dimensions == rhs.dimensions;
    return success;
  }
};

template <>
struct Data<geometry_msgs::Vector3>
{
  using T = geometry_msgs::Vector3;
  static T create()
  {
    T msg;
    msg.x = 1.0;
    msg.y = 2.0;
    msg.z = 3.0;
    return msg;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.x - rhs.x < std::numeric_limits<double>::epsilon();
    success &= lhs.y - rhs.y < std::numeric_limits<double>::epsilon();
    success &= lhs.z - rhs.z < std::numeric_limits<double>::epsilon();
    return success;
  }
};

template <>
struct Data<geometry_msgs::Point>
{
  using T = geometry_msgs::Point;
  static T create()
  {
    T pt;
    pt.x = 1.0;
    pt.y = 2.0;
    pt.z = 3.0;
    return pt;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.x - rhs.x < std::numeric_limits<double>::epsilon();
    success &= lhs.y - rhs.y < std::numeric_limits<double>::epsilon();
    success &= lhs.z - rhs.z < std::numeric_limits<double>::epsilon();
    return success;
  }
};

template <>
struct Data<geometry_msgs::Quaternion>
{
  using T = geometry_msgs::Quaternion;
  static T create()
  {
    T q;
    q.w = 1.0;
    q.x = 2.0;
    q.y = 3.0;
    q.z = 4.0;
    return q;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.w - rhs.w < std::numeric_limits<double>::epsilon();
    success &= lhs.x - rhs.x < std::numeric_limits<double>::epsilon();
    success &= lhs.y - rhs.y < std::numeric_limits<double>::epsilon();
    success &= lhs.z - rhs.z < std::numeric_limits<double>::epsilon();
    return success;
  }
};

template <>
struct Data<geometry_msgs::Pose>
{
  using T = geometry_msgs::Pose;
  static T create()
  {
    T p;
    p.position = Data<T::_position_type>::create();
    p.orientation = Data<T::_orientation_type>::create();
    return p;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= Data<T::_position_type>::equal(lhs.position, rhs.position);
    success &= Data<T::_orientation_type>::equal(lhs.orientation, rhs.orientation);
    return success;
  }
};

template <>
struct Data<geometry_msgs::PoseArray>
{
  using T = geometry_msgs::PoseArray;
  static T create()
  {
    T arr;
    arr.header = Data<T::_header_type>::create();
    const std::size_t n = 10;
    arr.poses.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      arr.poses.push_back(Data<T::_poses_type::value_type>::create());
    }
    return arr;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= Data<T::_header_type>::equal(lhs.header, rhs.header);
    if (lhs.poses.size() == rhs.poses.size())
    {
      for (std::size_t i = 0; i < lhs.poses.size(); ++i)
      {
        success &= Data<T::_poses_type::value_type>::equal(lhs.poses[i], rhs.poses[i]);
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
  using T = trajectory_msgs::JointTrajectoryPoint;
  static T create()
  {
    T pt;
    pt.positions.resize(6);
    std::iota(pt.positions.begin(), pt.positions.end(), 0);
    pt.velocities.resize(6);
    std::iota(pt.velocities.begin(), pt.velocities.end(), 0);
    pt.accelerations.resize(6);
    std::iota(pt.accelerations.begin(), pt.accelerations.end(), 0);
    pt.effort.resize(6);
    std::iota(pt.effort.begin(), pt.effort.end(), 0);

    pt.time_from_start = Data<T::_time_from_start_type>::create();

    return pt;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= lhs.positions == rhs.positions;
    success &= lhs.velocities == rhs.velocities;
    success &= lhs.accelerations == rhs.accelerations;
    success &= lhs.effort == rhs.effort;
    success &= Data<T::_time_from_start_type>::equal(lhs.time_from_start, rhs.time_from_start);
    return success;
  }
};

template <>
struct Data<trajectory_msgs::JointTrajectory>
{
  using T = trajectory_msgs::JointTrajectory;
  static T create()
  {
    T msg;
    msg.header = Data<T::_header_type>::create();
    msg.joint_names.insert(msg.joint_names.begin(), { "j1", "j2", "j3", "j4", "j5", "j6" });
    std::size_t n = 10;
    msg.points.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      msg.points.push_back(Data<T::_points_type::value_type>::create());
    }

    return msg;
  }

  static bool equal(const T& lhs, const T& rhs)
  {
    bool success = true;
    success &= Data<T::_header_type>::equal(lhs.header, rhs.header);
    success &= lhs.joint_names == rhs.joint_names;
    if (lhs.points.size() == rhs.points.size())
    {
      for (std::size_t i = 0; i < lhs.points.size(); ++i)
      {
        success &= Data<T::_points_type::value_type>::equal(lhs.points[i], rhs.points[i]);
      }
    }
    else
    {
      success &= false;
    }

    return success;
  }
};

} // namespace test
} // namespace qt_tree_editor_ros

#endif // QT_TREE_EDITOR_ROS_TEST_DATA_H
