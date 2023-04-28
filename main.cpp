#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include "Graph.cpp"
#include<fstream>
#include <string>
#include<sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

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
        column.setPosition(350, 620);
        column.setFillColor(Color::Black);

        window.draw(column);
        for(auto & semester : semesters){
            Text column(to_string(sem), font, 20);
            column.setPosition(120+moveHeader,700);
            column.setFillColor(Color::Black);
            window.draw(column);
            moveClass=0;
            sem++;
            for(auto & cclass: semester){
                Text clas(cclass.id, font, 20);
                clas.setPosition(80+moveHeader, 740 +moveClass);
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

class Textbox{
public:
    Textbox(){

    }

    Textbox(int size, Color color, bool sel){
        textbox.setCharacterSize(size);
        textbox.setFillColor(color);
        isSelected = sel;
        if(sel){
            textbox.setString("_");
        }
        else{
            textbox.setString("");
        }
    }

    void setFont(Font &font){
        textbox.setFont(font);
    }

    void setPosition(Vector2f pos){
        textbox.setPosition(pos);
    }

    void setLimit(bool ToF){
        haslimit = ToF;
    }

    void setLimit(bool ToF, int lim){
        haslimit = ToF;
        limit = lim-1;
    }

    void setSelected(bool sel){
        isSelected = sel;
        if(!sel){
            string t= text.str();
            string newT = "";

            for(int i =0; i <t.length()-1; i++){
                newT += t[i];
            }
            textbox.setString(newT);
        }
    }

    string getText(){
        return text.str();
    }

    void draw(RenderWindow &window){
        window.draw(textbox);
    }

    void typedOn(Event input){
        if(isSelected){
            int charTyped = input.text.unicode;
            if(charTyped<128){
                if(haslimit){
                    if(text.str().length()<=limit){
                        inputlogic(charTyped);
                    }
                    else if(text.str().length() > limit && DELETE_KEY){
                        deleteLastChar();
                    }
                    else{
                        inputlogic(charTyped);
                    }
                }
            }
        }
    }
private:
    Text textbox;
    ostringstream text;
    bool isSelected =false, haslimit = false;
    int limit;

    void inputlogic(int charTyped){
        if(charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY){
            text<< static_cast<char>(charTyped);
        }
        else if(charTyped == DELETE_KEY){
            if(text.str().length()>0){
                deleteLastChar();
            }
        }
        textbox.setString((text.str()+"_"));
    }
    void deleteLastChar(){
        string t= text.str();
        string newT = "";

        for(int i =0; i <t.length(); i++){
            newT += t[i];
        }
        text.str("");
        text <<newT;

        textbox.setString(text.str());
    }
};

void drawMenu(RenderWindow &window, Font &font1, Font &font2){
    sf::Image image;
    if (!(image.loadFromFile("Gator.jpg"))){
        std::cout << "Cannot load image";   //Load Image
    }
    Texture texture;
    texture.loadFromImage(image);
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.85,0.85);
    sprite.setPosition(-380,-20);

    RectangleShape box1(Vector2f{90,55});
    box1.setFillColor(Color{255,165,70});
    box1.setOutlineColor(Color::White);
    box1.setOutlineThickness(3);

    RectangleShape box2 = box1;

    box1.setPosition(200,520);
    box2.setPosition(1125,520);


    Text input1("Semesters", font2, 25);
    input1.setPosition(185,460);
    Text input2("Credits", font2, 25);
    input2.setPosition(1130,460);

    Text schedule("Schedule", font1, 80);
    schedule.setPosition(515,300);
    Text chomper("Chomper", font1, 80);
    chomper.setPosition(510,750);

    window.draw(sprite);
    window.draw(schedule);
    window.draw(chomper);
    window.draw(input1);
    window.draw(input2);
    window.draw(box1);
    window.draw(box2);

}

int main() {
    int sems=2, credits=12;

    RenderWindow window(sf::VideoMode(1400, 1200), "Scheduler");
    vector<Semester> semesters;
    int currentSem = 0, accumCredits = 0;
    bool newSemester = true, creditsReached = false, finalSem = false, inMenu = true, semsEntered = false, credEntered = false;

    Font font1;
    font1.loadFromFile("Drexs.ttf");
    Font font2;
    font2.loadFromFile("arial.ttf");

    Text GatorPlanner("Schedule Chomper", font1, 20);
    GatorPlanner.setPosition(10, 10);
    GatorPlanner.setFillColor(Color::Blue);


    Graph myGraph;
    readCSV("CourseData.csv", myGraph);
    Graph ogGraph = myGraph;
    vector<Course> availableCourses;
    vector<Course> selectedCourses;
    vector<vector<Course>> chosenPlan(sems);
    vector<vector<Course>> optimizedCourseSchedule;

    Textbox NumSemesters(45, Color::Black, false);
    Textbox NumCredits(45, Color::Black, false);
    NumSemesters.setLimit(true,2);
    NumCredits.setLimit(true,2);
    NumSemesters.setFont(font2);
    NumCredits.setFont(font2);
    NumSemesters.setPosition({220, 520});
    NumCredits.setPosition({1145, 520});


    while (window.isOpen()) {
        sf::Event Event;


        if(semsEntered && credEntered && inMenu){
            inMenu = false;
            std::string tempSems(NumSemesters.getText().begin(), NumSemesters.getText().end());
            sems = stoi(tempSems);
            std::string tempCreds(NumCredits.getText().begin(), NumCredits.getText().end());
            credits = stoi(tempCreds);
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

        while (window.pollEvent(Event)) {

            if (Event.type == sf::Event::Closed) {
                window.close();
            }
            if(Keyboard::isKeyPressed(Keyboard::Return) && inMenu){
                if(!semsEntered){
                    NumSemesters.setSelected(true);
                }
                else{
                    NumCredits.setSelected(true);
                }

                break;
            }
            else if(Keyboard::isKeyPressed(Keyboard::Escape) && inMenu){
                if(!semsEntered){
                    NumSemesters.setSelected(false);
                    semsEntered = true;
                }
                else if(!credEntered){
                    NumCredits.setSelected(false);
                    credEntered = true;
                }
                break;
            }

            if(inMenu && Event.type == Event::TextEntered){
                NumSemesters.typedOn(Event);
                break;
            }

            if (selectedCourses.size() == availableCourses.size() && !finalSem && !selectedCourses.empty()) {
                newSemester = true;
            }
            else if (availableCourses.empty() || selectedCourses.size() == availableCourses.size()) {
                finalSem = true;
                creditsReached = true;
            }

            if (Event.type == sf::Event::MouseButtonPressed) {
                int x = Event.mouseButton.x,y = Event.mouseButton.y;
                bool classAlrInserted = false;
                if (x > 80 && x < 280) {
                    //send credits/grap
                    int selectedpos = findCourse(y, availableCourses);
                    //find button and associated value
                    for(auto & m: selectedCourses){
                        if(m.id == availableCourses[selectedpos].id){
                            classAlrInserted = true;
                            break;
                        }
                    }
                    if (selectedpos < availableCourses.size() && !classAlrInserted) {
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

        if(inMenu){
            drawMenu(window, font1, font2);
            NumCredits.draw(window);
            NumSemesters.draw(window);
        }
        else if (finalSem && (creditsReached)) {
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
            totCredit.setPosition(830, 1200);
            window.draw(header);
            window.draw(GatorPlanner);
            semesters[currentSem - 1].draw(window);
            window.draw(totCredit);
        }
        window.display();
    }
}