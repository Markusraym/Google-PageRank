#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <regex>
using namespace std;
#define null 0.0
#define key first //define 1st and 2nd as key and value to make code easier to read
#define value second

class AdjacencyList {
private:
	map<std::string, float> updatedRanks;
	map<string, vector<string>> graph;
	map<std::string, float> oldRanks;
	map<std::string, float> indexMap;
	map<string, vector<pair<string, float>>> graph_2;
	//unordered_map<int, string> updatedRanks;
	//map<string, vector<pair<string, float>>>& getGraph()
	//Think about what member variables you need to initialize
public:
	//Think about what helper functions you will need in the algorithm
	AdjacencyList Ranks(int power_iterations, AdjacencyList& list, vector<string>& toStrings); //meat and potatoes of this code
	map<std::string, float>& getUpdatedRanks() //getter function to return the updated ranks
	{
		return updatedRanks;
	}

	map<string, vector<string>>& getGraph() //getter function to return the graph itself
	{
		return graph;
	}
	map<std::string, float>& getOldRanks() //getter function to return the old ranks
	{
		return oldRanks;
	}

	void printRanks(AdjacencyList& list) //print the vertex and it's rank to two decimal places
	{
		for (const auto& element : list.getOldRanks())
		{
			std::cout << element.key << " ";
			std::cout << std::fixed << std::setprecision(2) << element.value << std::endl;
		}
	}

	float getdValue(float d) //Returns the 1/d value for the "matrix" M
	{
		return (1.0 / d);
	}

	map<std::string, float>& getIndexMap() //returns the map with every key having its own index
	{
		return indexMap;
	}

	void exchange(AdjacencyList& list) //swap everything over to get it ready for next iteration
	{
		std::swap(list.getOldRanks(), list.getUpdatedRanks()); //this may cause an error
	}

	string mapIndex;
	float newRank;
};


int main()
{
	int lineCount, power_iterations;
	int currentID = 0;
	std::string from, to;
	std::cin >> lineCount;
	std::cin >> power_iterations;
	AdjacencyList list;
	power_iterations = power_iterations - 1;
	int d = 0; //for the 1/d values
	vector<string> toStrings;

	for (int i = 0; i < lineCount; i++)
	{
		std::cin >> from;
		if (from == "exit") //Program stops if user types in "exit" 
		{
			break;
		}
		std::cin >> to;
		if (to == "exit") //Program stops if user types in "exit"
		{
			break;
		}
		//cout << from << '\n;
		//coit << to << '\n;

		list.getGraph()[from].push_back(to);
		toStrings.push_back(to);

		// Add new URLs to the map with unique IDs //TO DO: INCORPORATE THE INDEX MAP AND THIS CODE FOR EASE OF LIFE!!!!!!!
		if (list.getIndexMap().find(from) == list.getIndexMap().end()) {
			list.getIndexMap()[from] = currentID;
			currentID++;
		}
		if (list.getIndexMap().find(to) == list.getIndexMap().end()) {
			list.getIndexMap()[to] = currentID;
			currentID++;
		}
	}
	//fixes a bug where if a URL is only on the to side but not from, it will never be inserted into the graph
	for (const auto& str : toStrings) {
		// Check if the string is in the graph
		auto element = list.getGraph().find(str);
		if (element == list.getGraph().end()) {
			// If not, insert it with an empty vector as its value
			list.getGraph()[str] = std::vector<std::string>();
		}
	}
	//Create an integer to store the original rank value
	double originalRank = 1.0 / list.getGraph().size();
	for (auto& element : list.getGraph()) {
		list.getOldRanks()[element.key] = originalRank; //Assign all oldRanks vertex's with 1 / graph.size
		list.getUpdatedRanks()[element.key] = null; //Assign all updatedRanks vertex's with a null of 0
	}

	list.Ranks(power_iterations, list, toStrings);
	list.printRanks(list);
}

AdjacencyList AdjacencyList::Ranks(int power_iterations, AdjacencyList& list, vector<string>& toStrings)
{
	//map<string, vector<pair<string, float>>>& getGraph()
	//map<string, vector<string>> finalgraph;

	for (int i = 0; i < power_iterations; i++)
	{
		for (auto& element : list.getGraph())
		{
			newRank = 0;
			mapIndex = element.key; //copy the vertex into a string, this is what rank we will be calculating
			string domain; // to hold the portion after the "."

			// Find the position of the dot character
			int dotPos = mapIndex.find(".");
			if (dotPos != std::string::npos) {
				// Extract the portion after the dot
				domain = mapIndex.substr(dotPos + 1);
			}
			//TO DO: DO SOMETHING WITH THE DOMAIN VARIABLE TO AVOID BUGS WITH MATCHING URLS WITH DIFFERENT DOMAINS!!!!

			// look through the graph 
			for (auto& elementSquared : list.getGraph())
			{
				// look through the element's vector contains the vertex that mapIndex points to. if it doesn't equal end(), compute M * r
				if (std::find(elementSquared.value.begin(), elementSquared.value.end(), mapIndex) != elementSquared.value.end())
				{
					//Calculate M * r -> i.e rank(1) * 1/d + rank(5) * 1/d
					newRank = newRank + (list.getOldRanks()[elementSquared.key] * list.getdValue(list.getGraph()[elementSquared.key].size()));
				}
			}
			list.getUpdatedRanks()[mapIndex] = newRank; //assign the rank to the updated ranks map
		}
		list.exchange(list); //this may cause an error
	}
	return list;
}


/*
	A node's rank, r, is determined by the sum of the edges pointing towards that node. Each node's rank is determined by it's incoming edges.
	A node's outgoing edges, or what it's pointing to, determines the rank of the nodes its pointing towards. If a node, n, is pointing to 3 other nodes, those
	3 node's ranks will be n/3.
	outdegree of a node: Is the rank that the node is contributing to the nodes its pointing towards
	*/

	/*
	TA HELP:
	- Very confused on steps 2 and 3
	- Not sure where to go from having the values in my map
	- Confused on outdegree
	- What exactly are we calculating in step 2's diagram? Why is it 1/d?
	  Shouldn't it be rank/outdegree that dictates how much rank a node is contributing towards what it's pointing towards?
	- In step 3, where is time coming from?
	- I'd like some help figuring out how to make these calculations for rank on paper, and some guidance on how I'd implement it in code.
	  Do I use the same format from the slides, map <string, vector<pair<String, int>>> graph?
	*/

	//calculate M, set up r(0), then do matrix multiplication 

	//Once we represent webpages in an adjacecny list graph, we will run the pagerank algorithm to determine importance
	//Create a graph object
	//Created_Graph.PageRank(power_iterations);

/*
		// Add new URLs to the map with unique IDs
		if (list.getMap().find(from) == list.getMap().end()) {
			list.getMap()[from] = currentID;
			currentID++;
		}
		if (list.getMap().find(to) == list.getMap().end()) {
			list.getMap()[to] = currentID;
			currentID++;
		} */


		/*
		//Update the weight of every 1/d in the pair to be 1/vector.size()
		for (auto& pair : list.getGraph()[to])
		{
			pair.second = 1.0 / list.getGraph()[to].size();
		} */



		//DO POWER ITERATIONS HERE 
		/*
		for (int i = 0; i < power_iterations; i++)
		{
			for (int i = 0; i < list.getGraph().size(); i++)
			{
				/*
				So I first look at the first item in my Ranks map, I read what the first URL is, and then I want to access that URL in the adjacency graph
				by searching for that URL key. I then iterate through the vector of that key, extracting the outdegree of every URL in the vector
				*/
				//} 
			//} 