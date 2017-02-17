cmake_minimum_required(VERSION 2.8.3)
project(dockingstation)

add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  any_node
  roscpp
  rospy
  std_msgs
)


catkin_package(
 INCLUDE_DIRS include
 LIBRARIES dockingstation adc
 CATKIN_DEPENDS any_node roscpp rospy std_msgs
 DEPENDS wiringPi
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(dockingstation
  src/dockingstation.cpp
)

add_library(adc
  src/adc.cpp
)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
add_dependencies(dockingstation ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(dockingstation
  ${catkin_LIBRARIES}
  wiringPi
)

target_link_libraries(adc
  ${catkin_LIBRARIES}
  wiringPi
)

## Declare a C++ executable
 add_executable(dockingstation_node src/dockingstation_node.cpp)

## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(dockingstation_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
 target_link_libraries(dockingstation_node 
 dockingstation 
 adc 
 ${catkin_LIBRARIES} 
 wiringPi
 )
