#include <iostream>

#include <future>
#include <thread>
#include <vector>

using namespace std;


const unsigned long MAX_CHUNK_SIZE = 25;

template<typename Iterator, typename T>
void p_foreach(Iterator first, Iterator last, void (*fn) (T &x))
{
    unsigned long const length = std::distance(first, last);
    unsigned long const max_chunk_size = 20000;

    // std::cout << std::this_thread::get_id() << std::endl;

    if (length <= max_chunk_size)
    {
        for (auto it = first; it != last; ++it) {
            fn(*it);
        };
    }
    else
    {
        Iterator mid_point = first;
        std::advance(mid_point, length / 2);
        auto fhr = std::async(p_foreach<Iterator, T>, first, mid_point, fn);
        p_foreach(mid_point, last, fn);
    }
}

void func(double &x) {
    x = x * 0.75;
}

int main()
{
    std::vector<double> vec(200000000, 10);

    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;

    p_foreach(vec.begin(), vec.end(), &func);

    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;

    return 0;
}
