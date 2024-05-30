#include <iostream>

#include <future>
#include <thread>

using namespace std;


void find_min_idx(int start, int size, int *arr, std::promise<int> min_idx) {
    int min_v = arr[start];
    int idx = start;
    for (int i = start; i < size; ++i) {
        if (arr[i] < min_v) {
            min_v = arr[i];
            idx = i;
        }
    }
    min_idx.set_value(idx);
}

void selection_sort(int size, int *arr) {

    for (int i = 0; i < (size-1); ++i) {

        std::promise<int> p;
        std::future<int> fr_res = p.get_future();

        auto fr = std::async(find_min_idx, i, size, arr, std::move(p));
        int min_idx = fr_res.get();

        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}


int main()
{
    int arr[10] = {0, 3, 1, 5, 4, 1, 1, 23, 11, 7};

    selection_sort(10, arr);

    for (int i = 0; i < 10; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
