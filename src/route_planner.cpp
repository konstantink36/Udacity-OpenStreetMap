#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
current_node->FindNeighbors();

for(RouteModel::Node *node : current_node->neighbors) {
    node->visited = true;
    node->parent = current_node;
    node->h_value = CalculateHValue(node);
    node->g_value = current_node->g_value + current_node->distance(*node);
    open_list.emplace_back(node);
}
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

 //Compare F value of 2 nodes:
bool Compare(RouteModel::Node *a, RouteModel::Node *b){
return a->g_value + a->h_value > b->g_value + b->h_value;
}

RouteModel::Node *RoutePlanner::NextNode() {
    //sort the open list according to F with Compare function 
    sort(open_list.begin(), open_list.end(), Compare);

    RouteModel:: Node * next_node = open_list.back();  //create pointer to node in the list with lowest F 
    open_list.pop_back();                               //remove that node from list
    return next_node;                                   //return the pointer
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
      while(current_node != start_node)
    {
        path_found.emplace_back(*current_node);
        distance += current_node->distance(*(current_node->parent));
        current_node = current_node->parent;
    }
  
  //add start node
  path_found.emplace_back(*current_node);
  //reverse path_found so that start node is first
    reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;
}


// The A* Search algorithm

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    current_node = start_node;
   current_node->visited = true;

    while(current_node != end_node) {
        AddNeighbors(current_node); 
        current_node = NextNode();
    }
     m_Model.path = ConstructFinalPath(current_node);
    
}