#ifndef __DYNAMIC_VOXEL_FILTER_H
#define __DYNAMIC_VOXEL_FILTER_H

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Odometry.h>

#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/passthrough.h>
#include <pcl/search/kdtree.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/common/transforms.h>
#include <pcl/point_cloud.h>
#include <pcl/common/pca.h>

#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/LU"

#include <boost/thread.hpp>
#include <boost/multi_array.hpp>


typedef pcl::PointXYZINormal PointINormal;
typedef pcl::PointCloud<PointINormal> CloudINormal;
typedef pcl::PointCloud<PointINormal>::Ptr CloudINormalPtr;

// //typedef boost::multi_array<Eigen::Vector3f, 3> MultiArrayEVec3f;
//typedef boost::multi_array<State, 3> MultiArrayEVec3f;
//MultiArrayEVec3f pca3rd_voxel(boost::extents[VOXEL_NUM_X][VOXEL_NUM_Y][VOXEL_NUM_Z]);

class DynamicVoxelFilter
{
	public:
		DynamicVoxelFilter(void);

		CloudINormal pc_callback(const sensor_msgs::PointCloud2ConstPtr&);
		void odom_callback(const nav_msgs::OdometryConstPtr&);
		void execution(void);
		Eigen::Matrix3f eigen_estimation(CloudIVoxelPtr);
		CloudINormalPtr pc_addressing(CloudIVoxelPtr);
		void input_pt2voxel(CloudIVoxelPtr);
		void 3rd_main_component_estimation(void);
		void chronological_pca3rd_variance_calculation(void);
	
	private:
		bool pc_callback_flag = false;
		bool odom_callback_flag = false;
		bool tf_listen_flag = false;
		bool first_flag = false;

		const static float Occupied = 1.0, Unoccupied = 0.0, Unknown = 0.5;

		float Hz;
        float MAX_LENGTH, MAX_WIDTH, MAX_HEIGHT;
        float VOXEL_NUM_X, VOXEL_NUM_Y, VOXEL_NUM_Z;
        float voxel_size_x, voxel_size_y, voxel_size_z;
        // float RAY_NUM_PITCH, RAY_NUM_YAW;
        // float RAY_FOV_PITCH, RAY_FOV_YAW;
        // float ray_angle_pitch, ray_angle_yaw;

		ros::Subscriber pc_subscriber;
		ros::Subscriber odom_subscriber;
		ros::Publisher dynamic_pc_publisher;
		ros::Publisher static_pc_publisher;
		
		tf::TransformListener listener;
		tf::StampedTransform transform;

        sensor_msgs::PointCloud2 input_pc;
        CloudINormalPtr pcl_input_pc {new CloudINormal};

        Eigen::Vector3f zero_vector = Eigen::Vector3f::Zero();
		Eigen::Vector3f chronological_variance; // storage PCA 3rd vectors

		//std::vector<MultiArrayEVec3f> pca3rd_chronological_memories;

        struct Status{
            float occupation;
            float dynamic_probability;
            CloudINormalPtr pcl_pc;
            Eigen::Vector3f 3rd_main_component;
        };

        std::vector<std::vector<std::vector<Status> > > voxel_grid;

        /*
        struct RayCast{
            float pitch;
            float yaw;
            std::vector<Eigen::Vector3d> voxel_id;
        };
        
        std::vector<RayList> pre_cast_list;
        */
};

#endif// __DYNAMIC_VOXEL_FILTER_H
