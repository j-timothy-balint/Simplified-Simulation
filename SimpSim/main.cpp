#include "SimpSim.h"

#include "PlotPoints.h"
#include <fstream>
#include <sstream>

Actionary *actionary;  // global pointer to the actionary
AgentTable agentTable;
parTime *partime;	// global pointer to PAR time class (So we can manipulate time)
char* actionLocation = strdup("IEEE TOG/Castle/Castle-Actions/"); // Relative path to par (or action) folder
//char* actionLocation = strdup("../../../../Heist-Actions/");
int PAR::dbg = 1; /*< Used to see all the debug information in the code */
FILE* PAR::file_name = stdout; //fopen("D:/par.log", "w");/*! <The output stream of all debug information */
extern ActionTable actionTable; //Holds the mapping of actions to performance code

int doSpeak(iPAR *ipar) {
	std::string subject = ipar->getAgent()->getObjectName();
	//std::cout << "Running action " << ipar->par->getActionName() << " by " << subject << "." << std::endl;
	ipar->setFinished(true);
	return 1;
}
int doNothing(iPAR *ipar) { //This is a no-op function
	return 1;
}


/*Sets up the world based on the plot points*/
AgentProc* setupWorld(const std::vector<plot_point>& plot_points, plot_graph* graph) {
	AgentProc*  world = new AgentProc("Character_World");
	for (std::vector<plot_point>::const_iterator it = plot_points.begin(); it != plot_points.end(); it++) {
		for (std::list<int>::const_iterator a_it = (*it).characters.begin(); a_it != (*it).characters.end(); a_it++) {
			MetaObject* obj = (*it).predicates.find((*a_it))->second;
			std::map<MetaObject*, bool>::const_iterator motif_only = graph->locations.find(obj);
			if (obj->getLocation() == NULL && (*motif_only).second) { //Otherwise we don't need to track it
				obj->setLocation(world->getObject());
			}
		}
		for (std::list<int>::const_iterator l_it = (*it).locations.begin(); l_it != (*it).locations.end(); l_it++) {
			MetaObject* obj = (*it).predicates.find((*l_it))->second;
			if (obj->getLocation() == NULL) {
				obj->setLocation(world->getObject());
			}
		}
	}
	return world;
}



int getAgentPosition(const std::vector<MetaObject*>& objects) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects.at(i)->isAgent())
			return i;
	}
	return -1;
}

//This is the function that actually runs SimpSim to generate the file that we need
void runSimulation(const std::vector<plot_point>& plot_points, AgentProc* world) {
	int error = 0;
	iPAR *world_action = new iPAR(actionary->searchByNameAct("temporial")); //21 is our temporal action for heist, 14 is our temporial action for castle
	world_action->setAgent(world->getObject());
	world_action->setFinished(false);
	world->addAction(world_action);
	//Sets up our world state
	for (int i = 0; i < 20; i++)
		LWNetList::advance(&error);
	//Now we can start to run the narrative
	//Our of our actions should be finished action, so we should be good
	for (auto &point : plot_points) { //auto is depreciated and I shouldn't get into this habit
		iPAR *new_par = new iPAR(point.action);
		int agent_position = point.characters.front(); //First character is always our agent in par
		MetaObject *agent = point.predicates.find(agent_position)->second;
		//std::cout << "Agent is " << point.objects[agent_position]->getObjectName() << " for action " << point.action->getActionName() << std::endl;
		new_par->setAgent(agent);
		int obj_counter = 0;
		for (int i = 0; i < point.predicates.size(); i++) {
			if (i != agent_position) {
				new_par->setObject(point.predicates.find(i)->second, obj_counter);
				obj_counter++;
			}
		}
		new_par->setFinished(false);
		//if (new_par->par->getActionName() != "poisons") {

		//Now that the action is created, we add it to the system, and then run it
		AgentProc *p = agentTable.getAgent(agent->getObjectName());
		p->addAction(new_par);
		int count = 0;
		while (!new_par->getFinished() && count < 100) { //While we have an action
			LWNetList::advance(&error);
			count++;
		}
	}

	//Now we finish the narrative
	world_action->setFinished(true);
	for (int i = 0; i < 20; i++)
		LWNetList::advance(&error);

	//If we want to get the sets, then we may want to read in the file it makes and return the string
}

void setUpActionTable(const std::string& extern_start_path) {
	// Make sure that the Actionary was created
	if (actionary == NULL) {
		actionary = new Actionary();

		actionary->init();
		actionary->loadExternPythonFunctionFile(extern_start_path+"external_functions.py");
	}
	else {
		std::cout << "We still have an actionary" << std::endl;
	}
	int counter = 0;
	MetaAction *act = actionary->getAllActions(counter);
	while (act != NULL) {
		actionTable.addFunctions(act->getActionName().c_str(), &doSpeak); // Link the Speak action with the code to perform it.
		counter++;
		act = actionary->getAllActions(counter);
	}
	
	/*actionTable.addFunctions("talkmultiple", &doSpeak);
	actionTable.addFunctions("swaps", &doSpeak);
	actionTable.addFunctions("poisons", &doSpeak);
	actionTable.addFunctions("placeon", &doSpeak);
	actionTable.addFunctions("pickup", &doSpeak);
	actionTable.addFunctions("move", &doSpeak);
	actionTable.addFunctions("marry", &doSpeak);
	actionTable.addFunctions("hides", &doSpeak);
	actionTable.addFunctions("give", &doSpeak);
	actionTable.addFunctions("fallinlove", &doSpeak);
	actionTable.addFunctions("drink", &doSpeak);
	actionTable.addFunctions("dies", &doSpeak);*/
	actionTable.addFunctions("temporial", &doNothing);

	/*actionTable.addFunctions("arrest", &doSpeak);
	actionTable.addFunctions("buy", &doSpeak);
	actionTable.addFunctions("buydress", &doSpeak);
	actionTable.addFunctions("buydrink", &doSpeak);
	actionTable.addFunctions("cheat", &doSpeak);
	actionTable.addFunctions("escort", &doSpeak);
	actionTable.addFunctions("getaway", &doSpeak);
	actionTable.addFunctions("hatch", &doSpeak);
	actionTable.addFunctions("hide", &doSpeak);
	actionTable.addFunctions("holdup", &doSpeak);
	actionTable.addFunctions("lay", &doSpeak);
	actionTable.addFunctions("collect", &doSpeak);
	actionTable.addFunctions("open", &doSpeak);
	actionTable.addFunctions("pickpocket", &doSpeak);
	actionTable.addFunctions("pickup", &doSpeak);
	actionTable.addFunctions("ride", &doSpeak);
	actionTable.addFunctions("sell", &doSpeak);
	actionTable.addFunctions("take", &doSpeak);
	actionTable.addFunctions("withdraw", &doSpeak);
	actionTable.addFunctions("pawn", &doSpeak);*/
}

__declspec(dllexport) StrPlotGraph* SimpSim::GetLocations(const std::vector<std::string>& lines,const std::string& action_location, const std::string& extern_path) {
	actionLocation = strdup(action_location.c_str());
	partime = new parTime();			// setup the timing info for the simulation
	partime->setTimeOffset(0);	// hours, minutes, seconds from midnight
	partime->setTimeRate(1);			// how fast should time change
	int error = 0;
	setUpActionTable(extern_path);		// Builds the action table
	MetaObject* room = actionary->searchByNameObj("room");
	if (room != NULL) //Sets our room to be a room
		room->setRoom();
	std::vector<plot_point> points = readPlotPoints(lines);
	std::vector<MetaAction*> travel;
	travel.push_back(actionary->searchByNameAct("move"));
	plot_graph* graph = getPlotGraph(points, travel);
	StrPlotGraph *result = new StrPlotGraph();
	//Copy the results into our plot graph
	for (std::vector<MetaObject*>::const_iterator it = graph->nodes.begin(); it != graph->nodes.end(); it++) {
		result->nodes.push_back((*it)->getObjectName());
	}
	for (std::multimap<int, int>::const_iterator it = graph->edges.begin(); it != graph->edges.end(); it++) {
		result->edges.insert(std::make_pair((*it).first, (*it).second));
	}
	delete graph;
	return result;
}
__declspec(dllexport) std::string SimpSim::GetSimpSim(const std::vector<std::string>& lines,const std::string& action_location,const std::string& extern_path) {
	actionLocation = strdup(action_location.c_str());
	partime = new parTime();			// setup the timing info for the simulation
	partime->setTimeOffset(0);	// hours, minutes, seconds from midnight
	partime->setTimeRate(1);			// how fast should time change
	int error = 0;
	setUpActionTable(extern_path);		// Builds the action table
							//Create an environment agent that will manage all the other agents
	MetaObject* room = actionary->searchByNameObj("room");
	if (room != NULL) //Sets our room to be a room
		room->setRoom();
	std::vector<plot_point> points = readPlotPoints(lines);
	std::vector<MetaAction*> travel;
	travel.push_back(actionary->searchByNameAct("move"));
	plot_graph* graph = getPlotGraph(points, travel);
	AgentProc*  world = setupWorld(points, graph);
	runSimulation(points, world);
	std::ifstream myfile;
	std::string result;

	//Next, we simply read in the file, and return that
	myfile.open("narrative_table.txt");
	if (myfile.good()) {
		myfile.seekg(0, myfile.end);
		int size = myfile.tellg();
		myfile.seekg(0, myfile.beg);
		char *buffer = new char[size];
		myfile.read(buffer, size);
		result = std::string(buffer);
		delete buffer;
		myfile.close();
		//delete pbuf;
	}
	else {
		result = "";
	}
	return result;
}


int main(void){
	/*partime = new parTime();			// setup the timing info for the simulation
	partime->setTimeOffset(0);	// hours, minutes, seconds from midnight
	partime->setTimeRate(1);			// how fast should time change
	int error = 0;
	setUpActionTable();		// Builds the action table
	//Create an environment agent that will manage all the other agents
	
	MetaObject* room = actionary->searchByNameObj("room");
	if (room != NULL) //Sets our room to be a room
		room->setRoom();*/
	std::vector<std::string> lines;
	std::ifstream myfile;
	std::string line;
	myfile.open("IEEE TOG/Castle/castle_plan.pddl");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			lines.push_back(line);
		}
		myfile.close();
	}

	StrPlotGraph* graph = SimpSim::GetLocations(lines,"IEEE TOG/Castle/Castle-Actions/", "Extern-Python-Files/");
	std::string result = SimpSim::GetSimpSim(lines, "IEEE TOG/Castle/Castle-Actions/","Extern-Python-Files/");
	std::cout << result;
	/*std::vector<plot_point> points = readPlotPoints(lines);
	std::vector<MetaAction*> travel;
	travel.push_back(actionary->searchByNameAct("move"));
	plot_graph* graph = getPlotGraph(points, travel);
	AgentProc*  world = setupWorld(points, graph);
	runSimulation(points, world);*/
	system("PAUSE");

}


