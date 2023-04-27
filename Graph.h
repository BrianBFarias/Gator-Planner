#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Course{
    std::string id; // COP_3530 
    std::string name; // Data Structure and Alg..
    int credits;
    std::unordered_set<std::string> preReqs; // {cop_3503, cop_3502 ...}
    Course(){};
    Course(std::string id, std::string name, std::string credits, std::unordered_set<std::string>& preReqs);
    void addPreReq(std::string preReq);
    void print();
    // ~Course(){};
};

inline Course::Course(std::string id, std::string name, std::string credits, std::unordered_set<std::string>& preReqs){
    this->id = id;
    this->name = name;
    this->credits = stoi(credits);
    for (auto i : preReqs){
        this->preReqs.emplace(i);
    }
}

inline void Course::addPreReq(std::string preReq){
    preReqs.emplace(preReq);
}

inline void Course::print(){
    std::cout << "Course ID: " << id;
    std::cout << "| Name: " << name;
    std::cout << "| Credits: " << credits << std::endl;

}

class Graph
{
private:
    std::unordered_map<std::string, Course> adjList; // Maps: courseID --> Course Object
public:
    Graph();
    Graph(std::unordered_map<std::string, Course>&);
    void insertCourse(std::string id, std::string name, std::string credits, std::unordered_set<std::string>& preReq);
    void removeCourse(std::string id);
    std::vector<std::vector<Course>> topSort(int Semesters, int credits);
    std::vector<Course> getAvailableCourses(Graph& ogGraph);
    Course getCourse(string id);

};
