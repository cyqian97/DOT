#include <nav_msgs/Odometry.h>
#include <tf2_eigen/tf2_eigen.h>

#include <Eigen/Core>

class PoseListener {
 public:
    Eigen::Matrix4d trans;
    tf2::Transform trans_gps_lidar;

    PoseListener();

    void callbackLidarEigne(const nav_msgs::Odometry odom) {
        geometry_msgs::Pose pose = odom.pose.pose;
        Eigen::Affine3d trans_affine;
        Eigen::fromMsg(pose, trans_affine);
        trans = trans_affine.matrix();
    }
};

PoseListener::PoseListener(){};