#include <iostream>
#include <vector>
#include <fstream>
#include <set>

/*====================================================================================================================*/

int independenceNumber = 0;

std::set <std::vector <int> > result;

/*====================================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyListFromFile();

void startBronKerbosch(std::vector <std::vector <int> >& adjacencyList);

void bronKerboschAlgorithm(std::vector <int>& maxCliques, std::vector <int>& possibleCliques, std::vector <int>& exceptedCliques, std::vector <std::vector <int> >& adjacencyList);

void writeToOutputFile(std::set <std::vector <int> >& result, const int& independenceNumber);

/*====================================================================================================================*/

int main()
{
    std::vector <std::vector <int> > adjacencyList = inputAdjacencyListFromFile();

    startBronKerbosch(adjacencyList);

    writeToOutputFile(result, independenceNumber);
}

/*====================================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyListFromFile()
{
    std::ifstream reader("../test1.txt"); //test1.txt - test4.txt

    if (reader.is_open())
    {
        int numOfVertexes;

        reader >> numOfVertexes;

        std::vector <std::vector <int> > adjacencyList(numOfVertexes, std::vector <int>());

        for (int i = 0; i < numOfVertexes; ++i)
            for (int j = 0; j < numOfVertexes; ++j)
                if (!reader.eof())
                {
                    int temp;
                    reader >> temp;
                    if (temp > 0)
                        adjacencyList[i].push_back(j);
                }
                else
                    break;

        reader.close();

        std::ofstream cleaner("../Result.txt");
        cleaner << "List of maximum cliques:\n";
        cleaner.close();

        return adjacencyList;
    }
    else
    {
        reader.close();

        std::cout << "File Alert!";

        std::exit(0);
    }
}

void startBronKerbosch(std::vector <std::vector <int> >& adjacencyList)
{
    static std::vector <int> maxCliques = {}, possibleCliques = {}, exceptedCliques = {};

    for (int i = 0; i < adjacencyList.size(); ++i)
        possibleCliques.push_back(i);

    bronKerboschAlgorithm(maxCliques, possibleCliques, exceptedCliques, adjacencyList);
}

void bronKerboschAlgorithm(std::vector <int>& maxCliques, std::vector <int>& possibleCliques, std::vector <int>& exceptedCliques, std::vector <std::vector <int> >& adjacencyList)
{
    if (possibleCliques.empty() && exceptedCliques.empty())
    {
        result.insert(maxCliques);
        if (independenceNumber < maxCliques.size())
            independenceNumber = maxCliques.size();
        return;
    }

    std::vector <int> cycleVector = possibleCliques;

    for (std::size_t node = 0; node < cycleVector.size(); ++node)
    {
        std::vector <int> intersection = {}, intersectionException = {};

        if (!adjacencyList[node].empty())
            for (const auto& nodeB : adjacencyList[node])
            {
                for (const auto& nodeP : possibleCliques)
                    if (nodeB == nodeP)
                        intersection.push_back(nodeB);

                if (!exceptedCliques.empty())
                    for (const auto& nodeX : exceptedCliques)
                        if (nodeB == nodeX)
                            intersectionException.push_back(nodeB);
            }

        maxCliques.push_back(cycleVector[node]);

        bronKerboschAlgorithm(maxCliques, intersection, intersectionException, adjacencyList);

        possibleCliques.erase(remove(possibleCliques.begin(), possibleCliques.end(), node), possibleCliques.end());

        exceptedCliques.push_back(cycleVector[node]);
    }
}

void writeToOutputFile(std::set <std::vector <int> >& result, const int& independenceNumber)
{
    std::cout << "Independence number of graph is " << independenceNumber << std::endl;

    std::ofstream writer("../output.txt", std::ios::app);

    if (writer.is_open())
    {
        for (const auto& vector : result)
        {
            for (const auto& elem : vector)
                writer << elem << " ";
            writer << std::endl;
        }

        writer.close();
    }
    else
    {
        writer.close();

        std::cout << "File Alert!\n";

        std::exit(0);
    }
}