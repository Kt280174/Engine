#include <any>
#include <iostream>

#include "Wrapper.h"
#include "Engine.h"
class Subject {
public:
    int x;
    int y;
    Subject(int x, int y) : x(x), y(y) {}
    int f3(int x1, int y1) {
        if(x1 > y1){
            std::cout<< x1 <<" > "<<y1<<std::endl;
        }
        std::cout << x << ' ' << y << std::endl;
        return x + y;
    }
    void f4(std::string x1, std::string y1) {
        std::cout << x1 << " x1 = " <<" y1 = " << y1 << std::endl;
    }
};

int main() {
    Subject subj(1, 2);
    Wrapper wrapper(&subj, &Subject::f3, {{"arg1", 4}, {"arg2", 5}});
    Engine engine;
    engine.registerCommand(&wrapper, "command1");
    engine.execute("command1", {{"arg1", 4}, {"arg2", 5}});

    return 0;
}