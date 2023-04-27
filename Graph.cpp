#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(std::unordered_map<std::string, Course> &adjList)
{
    this->adjList = adjList;
}

void Graph::insertCourse(std::string id, std::string name, std::string credits, std::unordered_set<std::string> &preReqs)
{
    adjList[id] = Course(id, name, credits, preReqs);
}

void Graph::removeCourse(std::string id)
{
    //remove as preReq for any classes
    for(auto & course : adjList){
        if(!course.second.preReqs.empty()){
            for(auto & preReq: course.second.preReqs){
                if(preReq == id){
                    course.second.preReqs.erase(id);
                    break;
                }
            }
        }
    }
    adjList.erase(id);
}

// sorts through a graph of slected classes and returns a vector of vector mock optimized plan
std::vector<std::vector<Course>>  Graph::topSort(int semester, int credits) {

    std::unordered_map<std::string, Course> tempAdjList = this->adjList;
    int currentSemester = 1;
    vector<Course> temp;
    vector<Course> optimizedSemester;
    vector<std::vector<Course>> results;

    while(currentSemester <= semester){

        int currentCredits = 0;
        for (auto course : tempAdjList) {
            if (course.second.preReqs.size() == 0) {
                temp.push_back(adjList.at(course.first));
            }
        }

        sort(temp.begin(), temp.end(), [](const auto& lhs, const auto&rhs) {
            return lhs.credits > rhs.credits;
        });

        for (auto iter : temp) {
            if (currentCredits + iter.credits <= credits) {
                optimizedSemester.push_back(iter);
                currentCredits += iter.credits;
            }
        }
        for (auto i : optimizedSemester) {
            for (auto &course: tempAdjList) {
                if (!course.second.preReqs.empty()) {
                    for (auto &preReq: course.second.preReqs) {
                        if (preReq == i.id) {
                            course.second.preReqs.erase(i.id);
                            break;
                        }
                    }
                }
            }
            tempAdjList.erase(i.id);
        }

        results.push_back(optimizedSemester);
        optimizedSemester.clear();
        temp.clear();
        currentSemester++;
    }
    return results;
}

std::vector<Course> Graph::getAvailableCourses(Graph& ogGraph){

    std::vector<Course> result;
    for (auto iter : adjList) {
        if (iter.second.preReqs.empty()) {
            result.push_back(ogGraph.getCourse(iter.first));
        }
    }
    return result;
}

Course Graph::getCourse(string id){
    /*Find the Course in the graph, and return it*/
    return adjList.at(id);
}
