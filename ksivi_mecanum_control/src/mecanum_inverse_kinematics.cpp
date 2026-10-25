[0.007s] Invoking command in '/home/xoore4ik/X0re4ik/projects/ksivi/build/ksivi_mecanum_control': CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:/opt/ros/humble /usr/bin/cmake --build /home/xoore4ik/X0re4ik/projects/ksivi/build/ksivi_mecanum_control -- -j12 -l12
[0.039s] [35m[1mConsolidate compiler generated dependencies of target gtest[0m
[0.040s] [35m[1mConsolidate compiler generated dependencies of target ksivi_mecanum_control_core[0m
[0.041s] [35m[1mConsolidate compiler generated dependencies of target gtest_main[0m
[0.047s] [  9%] [32mBuilding CXX object CMakeFiles/ksivi_mecanum_control_core.dir/src/mecanum_controller_node.cpp.o[0m
[0.050s] [ 27%] Built target gtest
[0.051s] [ 45%] Built target gtest_main
[0.059s] [35m[1mConsolidate compiler generated dependencies of target test_inverse_kinematics[0m
[0.064s] In file included from [01m[K/home/xoore4ik/X0re4ik/projects/ksivi/ksivi_mecanum_control/src/mecanum_controller_node.cpp:1[m[K:
[0.065s] [01m[K/home/xoore4ik/X0re4ik/projects/ksivi/ksivi_mecanum_control/include/ksivi_mecanum_control/mecanum_controller_node.hpp:5:10:[m[K [01;31m[Kfatal error: [m[Krclcpp/rclcpp.hpp: No such file or directory
[0.065s]     5 | #include [01;31m[K"rclcpp/rclcpp.hpp"[m[K
[0.065s]       |          [01;31m[K^~~~~~~~~~~~~~~~~~~[m[K
[0.065s] compilation terminated.
[0.065s] gmake[2]: *** [CMakeFiles/ksivi_mecanum_control_core.dir/build.make:76: CMakeFiles/ksivi_mecanum_control_core.dir/src/mecanum_controller_node.cpp.o] Error 1
[0.065s] gmake[1]: *** [CMakeFiles/Makefile2:159: CMakeFiles/ksivi_mecanum_control_core.dir/all] Error 2
[0.065s] gmake[1]: *** Waiting for unfinished jobs....
[0.068s] [ 54%] [32mBuilding CXX object CMakeFiles/test_inverse_kinematics.dir/test/test_inverse_kinematics.cpp.o[0m
[0.118s] [01m[K/home/xoore4ik/X0re4ik/projects/ksivi/ksivi_mecanum_control/test/test_inverse_kinematics.cpp:3:10:[m[K [01;31m[Kfatal error: [m[Kksivi_mecanum_control/mecanum_inverse_kinematics.hpp: No such file or directory
[0.118s]     3 | #include [01;31m[K"ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"[m[K
[0.118s]       |          [01;31m[K^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[m[K
[0.118s] compilation terminated.
[0.120s] gmake[2]: *** [CMakeFiles/test_inverse_kinematics.dir/build.make:76: CMakeFiles/test_inverse_kinematics.dir/test/test_inverse_kinematics.cpp.o] Error 1
[0.121s] gmake[1]: *** [CMakeFiles/Makefile2:212: CMakeFiles/test_inverse_kinematics.dir/all] Error 2
[0.121s] gmake: *** [Makefile:146: all] Error 2
[0.123s] Invoked command in '/home/xoore4ik/X0re4ik/projects/ksivi/build/ksivi_mecanum_control' returned '2': CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:/opt/ros/humble /usr/bin/cmake --build /home/xoore4ik/X0re4ik/projects/ksivi/build/ksivi_mecanum_control -- -j12 -l12
