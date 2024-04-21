#include <thread>
#include <chrono>
#include <iostream>

#include <mutex>


using namespace std::chrono_literals;

class Data {
public:
    std::mutex mux;
    int number;
    double value;
};

void swap_lock(Data& a, Data&b) {
    std::lock(a.mux, b.mux);
    std::lock_guard<std::mutex> la(a.mux, std::adopt_lock);
    std::lock_guard<std::mutex> lb(b.mux, std::adopt_lock);

    int tmpn = a.number;
    a.number = b.number;
    b.number = tmpn;

    double tmpv = a.value;
    a.value = b.value;
    b.value = tmpv;
}

void swap_scoped(Data& a, Data&b) {
    std::scoped_lock lock(a.mux, b.mux);

    int tmpn = a.number;
    a.number = b.number;
    b.number = tmpn;

    double tmpv = a.value;
    a.value = b.value;
    b.value = tmpv;
}

void swap_unique(Data& a, Data&b) {
    std::unique_lock<std::mutex> la(a.mux, std::defer_lock);
    std::unique_lock<std::mutex> lb(b.mux, std::defer_lock);
    std::lock(la, lb);

    int tmpn = a.number;
    a.number = b.number;
    b.number = tmpn;

    double tmpv = a.value;
    a.value = b.value;
    b.value = tmpv;
}


int main() {

    return 0;
}
