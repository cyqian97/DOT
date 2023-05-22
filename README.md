## Fix errors 
### common_lib Compile errors
```sh
Errors     << common_lib:make /root/gagaga/logs/common_lib/build.make.000.log                                     
ln: failed to create symbolic link '/root/gagaga/src/common/libs/msgs/../include/common/msgs/autosense_msgs': No such file or directory
```
This is caused by the missing directories in ```src/common/libs```. Create ```srvs``` and ```msgs``` folders under ```src/common/libs``` will solve the problem. To avoid creating folders every git clone, just put a .gitkeep in both directories.

### PCL Compile Errors
Because of the current pcl 1.10 requirs c++14 and above, we need to change all
```
## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
#add_compile_options(-std=c++14)
```
to

```
## Compile as C++11, supported in ROS Kinetic and newer
#add_compile_options(-std=c++11)
add_compile_options(-std=c++14)
```

Problem also occurs when compiling /$CATKIN_WS/src/perception/libs/segmenters/src/don_segmenter.cpp, simply change the following code starting at line 105
```
for (; iter != clusters_indices.end(); ++iter) {
    PointICloudPtr cluster(new PointICloud);
    pcl::copyPointCloud<PointN, PointI>(*don_cloud_filtered, *iter,
                                        *cluster);

    cloud_clusters.push_back(cluster);
}
```
to
```
    for (const pcl::PointIndices & indices : clusters_indices) {
        PointICloudPtr cluster(new PointICloud);

        pcl::PointCloud<pcl::PointNormal> cloud_in = *don_cloud_filtered;
        pcl::PointCloud<pcl::PointXYZI> cloud_out = *cluster;

        // Allocate enough space and copy the basics
        cloud_out.points.resize (indices.indices.size ());
        cloud_out.header   = cloud_in.header;
        cloud_out.width    = std::uint32_t (indices.indices.size ());
        cloud_out.height   = 1;
        cloud_out.is_dense = cloud_in.is_dense;
        cloud_out.sensor_orientation_ = cloud_in.sensor_orientation_;
        cloud_out.sensor_origin_ = cloud_in.sensor_origin_;

        // Iterate over each point
        for (std::size_t i = 0; i < indices.indices.size (); ++i) {
            pcl::copyPoint (cloud_in.points[indices.indices[i]], cloud_out.points[i]);
        }

        cloud_clusters.push_back(cluster);
    }

```
following the tutorial https://github.com/AutoLidarPerception/segmenters_lib/issues/15.

For the kitti_ros package, more things need to be modified.
In the forked repository, the grammar of print and exception has been adapted to python3.
The package thread needed to be renamed to _thread.
'numpy' needs to be updated.
Some ```sys.path.append``` need to be added for python to find the packages.
The dot ```.``` needs to be added to as ```form .package import classname``` in the user-defined package 'utils' to avoid recursive import.
Function name of ```kitti.get_oxts_packets_and_poses``` needs to be changed to ```kitti.load_oxts_packets_and_poses``` in 'kitti_player.py'.