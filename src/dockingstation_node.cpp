/*!
 * @file	dockingstation_node.cpp
 * @author	Hendrik Kolvenbach
 * @date	February, 2017
 */

#include "dockingstation/dockingstation.hpp"


int main(int argc, char** argv) {

  any_node::Nodewrap<dockingstation::Dockingstation> node(argc, argv, "dockingstation_node", 1);
  node.execute(90); // 90=priority of the thread calling the update(..) function (if any)
  // execute blocks until the node was requested to shut down (after reception of a signal (e.g. SIGINT) or after calling the any_node::Node::shutdown() function)
  return 0;
}
