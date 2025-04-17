#include <iostream>
#include <thread>


    void travail(int id) {
        std::cout << "\nThread " << id << " travaille...\n";
    }

    int main() {
        std::thread t1(travail, 1);
        std::thread t2(travail, 2);

        t1.join();
        t2.join();




    return 0;
}
