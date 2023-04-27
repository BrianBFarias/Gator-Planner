#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include "Graph.cpp"
#include<fstream>
#include <string>
#include<sstream>

using namespace std;
using namespace sf;

class Button{
public:
    RectangleShape button;
    int posx, posy;
    int Cposx, Cposy;
    string title;
    int courseCredit;
    Font font;
    Font font2;
    string courseName;

    Button(Course course, int pos){
        font.loadFromFile("Drexs.ttf");
        font2.loadFromFile("arial.ttf");

        Color greyColor(210, 210, 210, 210);
        button.setSize(Vector2f(200.f, 60.f));
        button.setFillColor(greyColor);
        button.setPosition(80,pos+120);
        button.setOutlineColor(Color::Black);
        button.setOutlineThickness(5);
        courseCredit = course.credits;

        title = course.id;
        posx = 105;
        posy = pos+135;

        //implement course description later

    }
    void draw(RenderWindow &window){
        Text buttonText(title, font,30);
        buttonText.setFillColor(Color::Black);
        buttonText.setPosition(posx,posy);
        Text creditText("Credits: " + to_string(courseCredit), font2,20);
        creditText.setFillColor(Color::Black);
        creditText.setPosition(posx+200,posy);
        window.draw(button);
        window.draw(buttonText);
        window.draw(creditText);
    }
};

class Semester{
public:
    vector<Button> semesterOptions;

    Semester(vector<Course> availableCourses){
        int pos  = 20;

        for(auto & i:availableCourses){
            Button newButton(i, pos);
            semesterOptions.push_back(newButton);
            pos+=80;
        }
    }

    void draw(RenderWindow &window){
        for(auto & button: semesterOptions){
            button.draw(window);
        }
    }
};

class FullSchedule{
public:
    Font font;
    vector<vector<Course>> semesters;
    FullSchedule(vector<vector<Course>> semesters){
        this->semesters = std::move(semesters);
    };
    void draw(RenderWindow &window, int x){
        int moveHeader =0,moveClass,sem=1 ;
        Font font;
        font.loadFromFile("Drexs.ttf");
        Text column("Schedule for each Semester" , font, 40);
        column.setPosition(((x * 40+1000)/2)-300, 50);
        column.setFillColor(Color::Black);

        window.draw(column);
        for(auto & semester : semesters){
            Text column(to_string(sem), font, 20);
            column.setPosition(120+moveHeader,130);
            column.setFillColor(Color::Black);
            window.draw(column);
            moveClass=0;
            sem++;
            for(auto & cclass: semester){
                Text clas(cclass.id, font, 20);
                clas.setPosition(80+moveHeader, 160+moveClass);
                clas.setFillColor(Color::Black);
                moveClass+=60;
                window.draw(clas);

            }
            if(x<=4){
                moveHeader += (260);
            }
            else if(x>4 && x<=6){
                moveHeader += (220);
            }
            else if(x>6 && x<=8){
                moveHeader += (145);
            }
            else if(x>8 && x<=10){
                moveHeader += (125);
            }
            else{
                moveHeader += (110);
            }

        }
        }
    };

class OptimizedFullSchedule{
public:
    Font font;
    vector<vector<Course>> semesters;
    OptimizedFullSchedule(vector<vector<Course>> semesters){
        this->semesters = std::move(semesters);
    };
    void draw(RenderWindow &window, int x, int y){
        int moveHeader =0,moveClass,sem=1 ;
        Font font;
        font.loadFromFile("Drexs.ttf");
        Text column("Optimized Schedule for each Semester" , font, 40);
        column.setPosition(((x * 40+1000)/2)-350, 20+((y * 80 + 200)/2));
        column.setFillColor(Color::Black);

        window.draw(column);
        for(auto & semester : semesters){
            Text column(to_string(sem), font, 20);
            column.setPosition(120+moveHeader,((y * 80 + 200)/2)+130);
            column.setFillColor(Color::Black);
            window.draw(column);
            moveClass=0;
            sem++;
            for(auto & cclass: semester){
                Text clas(cclass.id, font, 20);
                clas.setPosition(80+moveHeader, ((y * 80 + 200)/2)+160+moveClass);
                clas.setFillColor(Color::Black);
                moveClass+=60;
                window.draw(clas);

            }
            if(x<=4){
                moveHeader += (260);
            }
            else if(x>4 && x<=6){
                moveHeader += (220);
            }
            else if(x>6 && x<=8){
                moveHeader += (145);
            }
            else if(x>8 && x<=10){
                moveHeader += (125);
            }
            else{
                moveHeader += (110);
            }

        }
    }
};

void readCSV(string filename, Graph& graph){
    // Open csv file
    std::ifstream inFile( filename);
    if(!inFile.is_open()){
        std::cout << "Error Opening file." << std::endl;
    }


    string line, id, name, credits, preReqList;

    unordered_set<std::string> preReqs;

    // Get the header
    getline(inFile, line);

    // Get the values
    while (std::getline(inFile, id, ',')) {
        std::getline(inFile, name, ',');
        std::getline(inFile,credits,',');
        std::getline(inFile,preReqList);

        if(preReqList.empty()){
            preReqList = "None";
        }
            // Remove the "" characters if needed
        else if(preReqList.at(0) == '\"'){
            preReqList = preReqList.substr(1, preReqList.size() - 2);
            // Convert to Stream
            std::stringstream preReqs_sstream(preReqList);
            std::string preReq;
            // std::cout << "printing all preReqs: " << std::endl;
            while (std::getline(preReqs_sstream, preReq, ','))
            {
                // examineString(preReq);
                // std::cout << " ";
                preReqs.insert(preReq);
            }
            // std::cout << std::endl;
        }
        else if (preReqList != "\r"){
            // std::cout << "printing all preReqs: " << std::endl;
            if (preReqList.at(preReqList.size() -1 ) == '\r'){preReqList = preReqList.substr(0, preReqList.size() - 1);}
            // examineString(preReqList);
            // std::cout << std::endl;
            preReqs.insert(preReqList);
        }

        graph.insertCourse(id, name, credits, preReqs);

        preReqs.clear();
    }

    inFile.close();
}

int findCourse(int y, vector<Course> availableCourses){
    int selectionpos =0;
    for(int ypos=100; ypos<availableCourses.size()*130; ypos+=80){
        if(y>ypos && y<(ypos+100)){
            return selectionpos;
        }
        selectionpos++;
    }
    return availableCourses.size();

}

void update(Semester &semester, int position){
    Color darkgreyColor(173, 216, 230, 200);
    semester.semesterOptions[position].button.setFillColor(darkgreyColor);
}

int main() {
    int sems, credits;
    cout << "How many Semesters do you want to take: ";
    cin >> sems;
    cout << "How many credits do you want to take for each semester: ";
    cin >> credits;

    RenderWindow window(sf::VideoMode(sems * 40 + 1000, 1100), "Scheduler");
    vector<Semester> semesters;
    int currentSem = 0, accumCredits = 0;
    bool newSemester = true, creditsReached = false, finalSem = false;

    Font font1;
    font1.loadFromFile("Drexs.ttf");
    Font font2;
    font2.loadFromFile("arial.ttf");

    Text GatorPlanner("Gator Planner", font1, 20);
    GatorPlanner.setPosition(10, 10);
    GatorPlanner.setFillColor(Color::Blue);


    Graph myGraph;
    readCSV("CourseData.csv", myGraph);
    Graph ogGraph = myGraph;
    vector<Course> availableCourses;
    vector<Course> selectedCourses;
    vector<vector<Course>> chosenPlan(sems);
    vector<vector<Course>> optimizedCourseSchedule;


    while (window.isOpen()) {
        sf::Event event;

        if (finalSem && creditsReached) {

        }

        if (newSemester && !finalSem) {
            availableCourses.clear();
            selectedCourses.clear();
            availableCourses = myGraph.getAvailableCourses(ogGraph);
            Semester addSem(availableCourses);
            semesters.push_back(addSem);
            newSemester = false;
            creditsReached = false;
            currentSem++;
            accumCredits = 0;
            if (currentSem == sems) {
                finalSem = true;
            }
        }


        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (selectedCourses.size() == availableCourses.size() && !finalSem && !selectedCourses.empty()) {
                newSemester = true;
            } else if (availableCourses.empty() || selectedCourses.size() == availableCourses.size()) {
                finalSem = true;
                creditsReached = true;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (x > 80 && x < 280) {
                    //send credits/grap
                    int selectedpos = findCourse(y, availableCourses);
                    //find button and associated value
                    if (selectedpos < availableCourses.size()) {
                        Course chosen = availableCourses[selectedpos];
                        if (!creditsReached) {
                            accumCredits += chosen.credits;
                            if (accumCredits < credits) {
                                update(semesters[currentSem - 1], selectedpos);
                                selectedCourses.push_back(chosen);
                                myGraph.removeCourse(chosen.id);
                                chosenPlan[currentSem - 1].push_back(chosen);
                                break;
                            } else if (accumCredits == credits) {
                                update(semesters[currentSem - 1], selectedpos);
                                selectedCourses.push_back(chosen);
                                myGraph.removeCourse(chosen.id);
                                chosenPlan[currentSem - 1].push_back(chosen);
                                newSemester = true;
                                creditsReached = true;
                                break;
                            } else if (selectedCourses.size() == availableCourses.size()) {
                                newSemester = true;
                                creditsReached = true;
                                finalSem = true;
                                break;
                            } else {
                                newSemester = true;
                                creditsReached = true;
                                break;
                            }

                        }
                    }
                    //if clicked return if credit count is reached
                }
            }

        }

        window.clear(Color::White);
        if (finalSem && (creditsReached)) {
            FullSchedule FinalSchedule(chosenPlan);
            FinalSchedule.draw(window, sems);

            Graph optimizedSchedule;

            for(auto & courseVector: chosenPlan){
                for(auto & course: courseVector){
                    //convert int back to string
                    stringstream s;
                    s<<course.credits;
                    string credits = s.str();
                    optimizedSchedule.insertCourse(course.id, course.name, credits, course.preReqs);
                }
            }

            optimizedCourseSchedule = optimizedSchedule.topSort(sems, credits);

            OptimizedFullSchedule optimalschedule(optimizedCourseSchedule);
            optimalschedule.draw(window, sems, credits);
        }

        else {
            Text header("Semester " + to_string(currentSem), font2, 50);
            header.setPosition(((sems * 40 + 800) / 2) - 70, 50);
            header.setFillColor(Color::Black);
            Text totCredit("Semester Credits: " + to_string(accumCredits) + " out of " + to_string(credits), font2, 25);
            totCredit.setFillColor(Color::Black);
            totCredit.setPosition(sems * 40 + 500, 1000);
            window.draw(header);
            window.draw(GatorPlanner);
            semesters[currentSem - 1].draw(window);
            window.draw(totCredit);
        }
        window.display();
    }
    Graph optimizedSchedule;
    for (auto &courseVector: chosenPlan) {
        for (auto &course: courseVector) {
            //convert int back to string
            stringstream s;
            s << course.credits;
            string credits = s.str();
            optimizedSchedule.insertCourse(course.id, course.name, credits, course.preReqs);
        }
    }
}

