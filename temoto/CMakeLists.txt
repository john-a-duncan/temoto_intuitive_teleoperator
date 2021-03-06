cmake_minimum_required(VERSION 2.8.3)
project(temoto)

set(CMAKE_CXX_FLAGS "-std=c++11")

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED
  actionlib
  actionlib_msgs
  genmsg
  geometry_msgs
  grasp_interface
  griffin_powermate
  keyboard_reader
  message_generation
  move_base_msgs
  move_base_to_manip
  moveit_core
  moveit_ros_planning_interface
  roscpp
  rospy
  std_msgs
  tf
  view_controller_msgs

  # For preplanned actions
  laser_stitcher
  pointcloud_processing_server
  pcl_ros
)

find_package (ur_script_interface REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(
# Boost REQUIRED COMPONENTS system
# )

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be thedd "message_generation" and every pack set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Command.msg
  Status.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  Goal.srv
  ChangeTf.srv
)

## Generate actions in the 'action' folder
add_action_files(DIRECTORY action/temoto
   FILES
   PreplannedSequence.action
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  actionlib_msgs
  geometry_msgs
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES beginner_tutorials
 CATKIN_DEPENDS actionlib_msgs roscpp rospy std_msgs geometry_msgs ur_script_interface
#  DEPENDS system_lib
#  CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${ur_script_interface_INCLUDE_DIRS}
  ${move_base_to_manip_INCLUDE_DIRS}
)

## Declare a C++ library
# Preplanned sequences:

add_library(close_hand src/preplanned_sequences/close_hand.cpp)
target_link_libraries(close_hand ${catkin_LIBRARIES})

add_library(enable_compliance src/preplanned_sequences/enable_compliance.cpp)
target_link_libraries(enable_compliance ${catkin_LIBRARIES} ${ur_script_interface_LIBRARIES})

add_library(go_to_laser_scan src/preplanned_sequences/go_to_laser_scan.cpp)
target_link_libraries(go_to_laser_scan ${catkin_LIBRARIES})

add_library(open_hand src/preplanned_sequences/open_hand.cpp)
target_link_libraries(open_hand ${catkin_LIBRARIES})

add_library(robot_please_approach src/preplanned_sequences/robot_please_approach.cpp)
target_link_libraries(robot_please_approach ${catkin_LIBRARIES} ${move_base_to_manip_LIBRARIES})
add_dependencies(robot_please_approach temoto_generate_messages_cpp move_base_to_manip)

add_library(robot_please_scan src/preplanned_sequences/robot_please_scan.cpp)
target_link_libraries(robot_please_scan ${catkin_LIBRARIES})

add_library(robot_push_button src/preplanned_sequences/robot_push_button.cpp)
target_link_libraries(robot_push_button ${catkin_LIBRARIES})


# Temoto common:

add_library(temoto_common src/temoto_common.cpp)
target_link_libraries(temoto_common ${catkin_LIBRARIES})
add_dependencies(temoto_common temoto_generate_messages_cpp)

## Declare a C++ executable

add_executable(move_robot src/move_robot.cpp)
target_link_libraries(move_robot ${catkin_LIBRARIES} temoto_common)
add_dependencies(move_robot temoto_generate_messages_cpp)

add_executable(start_teleop src/start_teleop.cpp)
target_link_libraries(start_teleop ${catkin_LIBRARIES} temoto_common)
add_dependencies(start_teleop temoto_generate_messages_cpp)

add_executable(rviz_visual src/rviz_visual.cpp)
target_link_libraries(rviz_visual ${catkin_LIBRARIES} temoto_common)
add_dependencies(rviz_visual temoto_generate_messages_cpp ${catkin_EXPORTED_TARGETS})

add_executable(interpret_utterance src/interpret_utterance.cpp)
target_link_libraries(interpret_utterance ${catkin_LIBRARIES} temoto_common)
add_dependencies(interpret_utterance temoto_generate_messages_cpp)

add_executable(human_frame_broadcaster src/human_frame_broadcaster.cpp)
target_link_libraries(human_frame_broadcaster ${catkin_LIBRARIES} temoto_common)
add_dependencies(human_frame_broadcaster temoto_generate_messages_cpp)

add_executable(navigate_robot src/navigate_robot.cpp)
target_link_libraries(navigate_robot ${catkin_LIBRARIES} temoto_common)
add_dependencies(navigate_robot temoto_generate_messages_cpp)

add_executable(preplanned_sequences src/preplanned_sequences.cpp)
add_dependencies(preplanned_sequences ${catkin_EXPORTED_TARGETS} )
target_link_libraries(preplanned_sequences ${catkin_LIBRARIES} close_hand open_hand enable_compliance go_to_laser_scan robot_please_approach robot_please_scan robot_push_button )

add_executable(kb_fake_voice_commander src/kb_fake_voice_commander.cpp)		
target_link_libraries(kb_fake_voice_commander ${catkin_LIBRARIES} temoto_common)		
add_dependencies(kb_fake_voice_commander temoto_generate_messages_cpp)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

catkin_install_python(PROGRAMS nodes/level_tf DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS beginner_tutorials beginner_tutorials_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
#  install(DIRECTORY include/${PROJECT_NAME}/
#    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#    FILES_MATCHING PATTERN "*.h"
#    PATTERN ".svn" EXCLUDE
#  )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_beginner_tutorials.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
