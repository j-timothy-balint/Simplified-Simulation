#ifndef __PAR_PLOT_POINTS
#define __PAR_PLOT_POINTS

#include <vector>
#include<map>

struct StrPlotGraph {
	std::vector<std::string> nodes;
	std::multimap<int, int> edges;
};




namespace SimpSim {

	__declspec(dllexport) StrPlotGraph* GetLocations(const std::vector<std::string>&,const std::string& action_location, const std::string& extern_path);
	__declspec(dllexport) std::string GetSimpSim(const std::vector<std::string>&,const std::string& action_location, const std::string& extern_path);
}

#endif // !__PAR_PLOT_POINTS
