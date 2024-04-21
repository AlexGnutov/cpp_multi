#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

//! Складывает два массива со сдвигом от начала - offset и на длине size.
void array_sum(int *arr_one, int *arr_two, int *sum_arr, int offset, int size) {
    int *st_one = arr_one + offset;
    int *st_two = arr_two + offset;
    int *st_sum = sum_arr + offset;
    for (int i = 0; i < size; ++i) {
        st_sum[i] = st_one[i] + st_two[i];
    }
}

double multicalc(int threads, int elements) {

    int thread_lot = elements / threads;
    int last_lot = elements % thread_lot;

    int * arr_one = new int[elements];
    int * arr_two = new int[elements];
    int * arr_sum = new int[elements];

    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> TV;
    for (size_t i = 0; i < threads - 1; ++i) {
        TV.emplace_back(array_sum, arr_one, arr_two, arr_sum, i * thread_lot, thread_lot);
    }
    if (last_lot > 0) {
        TV.emplace_back(array_sum, arr_one, arr_two, arr_sum, (threads-1) * thread_lot, last_lot);
    } else {
        TV.emplace_back(array_sum, arr_one, arr_two, arr_sum, (threads-1) * thread_lot, thread_lot);
    }

    for (auto & t : TV) {
        t.join();
    }

    auto stop = std::chrono::steady_clock::now();

    delete[] arr_one;
    delete[] arr_two;
    delete[] arr_sum;

    std::chrono::duration<double> seconds = stop-start;
    return seconds.count();
}


int main() {

    std::cout << "Количество аппаратных ядер: " << std::thread::hardware_concurrency() << std::endl;

    std::vector<int> tc{1,2,4,8,12};
    std::vector<int> el{1000,10000,100000,1000000};

    std::cout << std::setw(10) << " ";
    for (auto &e : el ) {
          std::cout << std::setw(11) << std::to_string(e);
    }
    std::cout << std::endl;

    for (auto &t : tc) {
        std::cout << std::setw(2) << t;
        std::cout << std::setw(10) << " потоков";
            for (auto &e : el ) {
                std::cout << std::setw(10) << std::fixed << multicalc(t,e) << "s";
            }
            std::cout << std::endl;
    }
    return 0;
}