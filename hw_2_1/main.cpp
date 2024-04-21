#include <thread>
#include <chrono>
#include <iostream>

#include <atomic>


using namespace std::chrono_literals;

const int MAX_QUEUE = 10;

std::atomic<int> queue{0};

void source(int max) {
    while (max > 0) {
        queue += 1;
        max -= 1;
        std::this_thread::sleep_for(1000ms);
    }
}

void worker() {
    std::this_thread::sleep_for(10ms);
    int served = 0;
    while (queue > 0) {
        queue -= 1;
        served += 1;
        std::cout << "Обслужил: " << served << " Клиентов в очереди: " << queue << " " << std::endl;
        std::this_thread::sleep_for(2000ms);
    }
    std::cout << "Операционист выполнил все задачи" << std::endl;
}

int main() {

    std::thread w_th(worker);
    std::thread s_th(source, MAX_QUEUE);

    w_th.join();
    s_th.join();

    return 0;
}
