#pragma once
#include "agentproc.h"
#include "lwnet.h"
#include "interpy.h"

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <list>
#include "SimpSim.h"
extern Actionary *actionary;
extern AgentTable agentTable;

struct plot_graph {
	std::vector<MetaObject*> nodes;
	std::multimap<int, int> edges;
	std::map<MetaObject*, bool> locations;
};

struct plot_point {
	MetaAction* action;
	std::map<int, MetaObject*> predicates;
	std::list<int> locations;
	std::list<int> characters;
};


plot_graph* getPlotGraph(const std::vector<plot_point>& plot_points, const std::vector<MetaAction*>& travel_actions){
	plot_graph *graph = new plot_graph();
	for (std::vector<plot_point>::const_iterator ai = plot_points.begin(); ai != plot_points.end(); ai++) {
		MetaAction* act = (*ai).action;
		bool travel = false;
		for (std::vector<MetaAction*>::const_iterator it = travel_actions.begin(); it != travel_actions.end(); it++) { //If we have a travel actions, then we should be able to locate it
			if ((*it) == act)
				travel = true;
		}
		if (travel) {//Once we have a travel action, we need to get the locations and make connections between them
			//One assumption that GameForge and Marijn make is that there is always two locations. We will make the same assumption for now, otherwise we would need more semantics.
			//These would be 

			//We could technically simulate this, as it's in the story. May do that later.
			if ((*ai).locations.size() > 1) {
				std::list<int>::const_iterator loc = (*ai).locations.begin();
				MetaObject *to = (*ai).predicates.find((*loc))->second;
				advance(loc, 1);
				MetaObject *from = (*ai).predicates.find((*loc))->second;
				//Next, we see if they exist, if they do, then we get the node number
				int to_n = -1;
				int from_n = -1;
				for (int i = 0; i < graph->nodes.size(); i++) {
					if (graph->nodes.at(i) == to) {
						to_n = i;
					}if (graph->nodes.at(i) == from) {
						from_n = i;
					}
				}
				if (to_n == -1) {
					to_n = graph->nodes.size();
					graph->nodes.push_back(to);
					graph->locations[to] = false;
				}if (from_n == -1) {
					from_n = graph->nodes.size();
					graph->nodes.push_back(from);
					graph->locations[from] = false;
				}
				//Here, I should do a check to make sure the nodes aren't already in there
				std::pair<std::multimap<int, int>::const_iterator, std::multimap<int, int>::const_iterator>range;
				bool found = false;
				range = graph->edges.equal_range(to_n);
				for (std::multimap<int, int>::const_iterator it = range.first; it != range.second; it++) {
					if ((*it).second == from_n)
						found = true;
				}
				if (!found)
					graph->edges.insert(std::make_pair(to_n, from_n));
				found = false;
				range = graph->edges.equal_range(from_n);
				for (std::multimap<int, int>::const_iterator it = range.first; it != range.second; it++) {
					if ((*it).second == to_n)
						found = true;
				}
				if (!found)
					graph->edges.insert(std::make_pair(from_n, to_n));
			}
		}
	}
	//Now we find all motif-only rooms by looking through the plot points
	for (std::vector<plot_point>::const_iterator ai = plot_points.begin(); ai != plot_points.end(); ai++) {
		int char_loc_pred_num = (*ai).characters.size() + (*ai).locations.size();
		if (char_loc_pred_num < (*ai).predicates.size()) {
			//We go through each location, and set their locations to true
			for (std::list<int>::const_iterator lai = (*ai).locations.begin(); lai != (*ai).locations.end(); lai++) {
				std::map<MetaObject*, bool>::iterator mo = graph->locations.find((*ai).predicates.find((*lai))->second);
				mo->second = true; //It is not a motif only
			}
		}
	}
	return graph;
}

//Decodes our plot points into a vector of plot points. Each string vector should be a line as 
//Action name, obj1,obj2... etc. Note that this should match what our action definitions in PAR are
//otherwise we have no connections
//In the paper, this is called a "Compiled Narrative"
std::vector<plot_point> readPlotPoints(const std::vector<std::string>& input_points) {
	std::vector<plot_point> result;
	std::size_t begin, end;
	for (std::vector<std::string>::const_iterator it = input_points.begin(); it != input_points.end(); it++) {
		plot_point point;
		//First, we get the action
		begin = (*it).find(',');
		std::string action_name = (*it).substr(0, begin);
		point.action = actionary->searchByNameAct(action_name);
		//Next, we get the objects
		end = -1;
		std::string line = (*it).substr(begin + 1);
		int num_objects = (int)std::count(line.begin(), line.end(), ',') + 1; //Add one for the last object
		for (int i = 0; i < num_objects; i++) { //This is where we start to get the objects.
			begin = end + 1;
			end = line.find(',', begin);
			std::string sub = line.substr(begin, end - begin);
			if (i == 0) {//Note that the first predicate is always the agent
				AgentProc* agent = NULL;
				if (agentTable.getAgent(sub) == NULL) {
					agent = new AgentProc(sub.c_str());
					agent->getObject()->setLocation(NULL);
				}
				else {
					agent = agentTable.getAgent(sub);
				}
				//if (agent->getObject()->getLocation() == NULL) {
				//	agent->getObject()->setLocation(World->getObject());
				//}
				point.predicates[i] = agent->getObject();
				point.characters.push_back(i);
			}
			else { //Otherwise, we determine if the parent of the object has an instance
				MetaObject *parent = actionary->searchByNameObj(sub.substr(0, sub.find("_")));
				if (parent != NULL) {
					//std::cout << sub << " has a parent of " << parent->getObjectName() << std::endl;
					//See if the object exists
					if (parent->isAgent()) {
						AgentProc* agent = NULL;
						if (agentTable.getAgent(sub) == NULL) {
							agent = new AgentProc(sub.c_str());
							agent->getObject()->setLocation(NULL);
						}
						else {
							agent = agentTable.getAgent(sub);
						}
						//if (agent->getObject()->getLocation() == NULL) {
						//	agent->getObject()->setLocation(World->getObject());
						//}
						point.predicates[i] = agent->getObject();
						point.characters.push_back(i);
					}
					else {
						MetaObject *obj = actionary->searchByNameObj(sub, true);//Start with types!
						if (obj == NULL || (!obj->isInstance() && !obj->isRoom())) {
							obj = new MetaObject(sub.c_str());
							obj->setParent(parent);
							//if it isn't a room
						}
						point.predicates[i] = obj;
						if (obj->isRoom())
							point.locations.push_back(i);
					
					}
				}
				else {
					//If it doesn't, we try to assign one through the affordances
					//We do i-1 cause we have no building
					//std::cout << "We did not find a parent, seraching affordances for "<< point.action->getNumAffordance(i-1) << std::endl;
					bool found = false;
					for (int j = 0; j < point.action->getNumAffordance(i-1) && !found; j++) {
						parent = point.action->searchAffordance(i-1, j);
						//std::cout << "Parent is " << parent->getObjectName() << std::endl;
						if (parent->isAgent()) {
							AgentProc* agent = NULL;
							if (agentTable.getAgent(sub) == NULL) {
								agent = new AgentProc(sub.c_str());
								agent->getObject()->setLocation(NULL);
							}
							else {
								agent = agentTable.getAgent(sub);
							}
							point.predicates[i] = agent->getObject();
							point.characters.push_back(i);
							found = true;
						}
						if (parent->isRoom()) {
							found = true;
						}
					}
					MetaObject *obj = actionary->searchByNameObj(sub, true);//Start with types!
					if (obj == NULL) {
						obj = new MetaObject(sub.c_str());
						obj->setParent(parent);
						//if it isn't a room
					}
					point.predicates[i] = obj;
					if (obj->isRoom())
						point.locations.push_back(i);
				}
			}
		}

		result.push_back(point);
	}
	return result;
}