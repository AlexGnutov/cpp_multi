#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <map>

using namespace std::chrono_literals;

const int THREADS = 10;             ///< Число потоков.
const int LOOSER_THREAD_ID = 3;     ///< ID потока, который упадёт.
const double LOOSER_MOMENT = 55.0;  ///< Значение, при котором упадёт.
const int TIME_BASE = 25;           ///< База для сдвига потоков.
const double PROGRESS_INC = 0.5;    ///< Инкремент прогресса.
const double RACE_SCALE = 0.1;      ///< Коэффециент расхождения потоков во времени.
const int CONSOLE_MS = 100;         ///< Интервал обновления консоли.


//! Статусы состояния потока.
enum class Status {
    init,
    working,
    finished,
    error,
};

//! Преобразует статус в строку.
std::string status_to_string(Status s) {
    switch (s) {
        case Status::init: return "init";
        case Status::working: return "working";
        case Status::finished: return "finished";
        case Status::error: return "error   ";
    default: return "unknown";
    }
}

//! Объект с данными о состоянии потока.
struct ProcessData {
    int process_id;
    double progress = 0.0;
    Status status = Status::init;
    double elapsed = 0.0;
};


//! Класс сбора данных о процессе вычислений.
class StatusInfo {
public:

    // Обновляет статус по переданным данным процесса.
    void update_status(int process_id, Status status, const double& progress, const double& elapsed) {
        std::lock_guard<std::mutex> lf(mux);

        int y = process_id;

        if (process_data.count(process_id) != 1) {
            process_data[process_id] = ProcessData{process_id, progress, status};
            progress_bar[process_id] = 0;
            print_x_y(process_id, 0, std::to_string(process_id));
            print_x_y(process_id, 26, status_to_string(status));
        }

        auto data = process_data[process_id];

        if (data.status != status) {
            print_x_y(process_id, 26, status_to_string(status));
        }

        if (status != Status::error) {
            int done = progress / 10;
            if (done > progress_bar[process_id]) {
                print_x_y(process_id, 12 + done, "*");
                progress_bar[process_id] = done;
            }
        } else {
            print_x_y(process_id, 12, " ----------");
        }

        if (elapsed != 0) {
            print_x_y(process_id, 38, std::to_string(elapsed) + " ms");
        }

        process_data[process_id] = ProcessData{process_id, progress, status, elapsed};
    }

    void print_x_y(int y, int x, const std::string& status) {
        std::cout << "\033[" << y << ';' << x << 'H' << status << std::flush;
    }

private:
    std::mutex mux;
    std::map<int,int> progress_bar;
    std::map<int, ProcessData> process_data = {};
};


//! Класс для запуска потоков.
class Runner {
public:
    Runner(StatusInfo *sp): sp(sp){}

    void run(int num) {
        std::vector<std::thread> thrds = {};

        for (int i = 0; i < num; i++) {
            int ms = (i * RACE_SCALE +1) * TIME_BASE;
            thrds.emplace_back(std::thread([this, i, ms]() {
                this->loop_method(i, ms);
            }));
        }
        for (auto& t : thrds) {
            t.join();
        }
    }

    void loop_method(int process_id, int millisec) {
        double progress = 0;

        std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now();

        while (progress != 100.0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
            progress += PROGRESS_INC;
            sp->update_status(process_id, Status::working, progress, 0.0);

            if (progress > LOOSER_MOMENT && process_id == LOOSER_THREAD_ID) {
                sp->update_status(process_id, Status::error, 0.0, 0.0);
                return;
            }
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count();

        sp->update_status(process_id, Status::finished, 100.0, elapsed);
        return;
    }

private:
    StatusInfo *sp = nullptr;
};


int main() {

    StatusInfo sp;
    Runner r(&sp);

    system("clear");

    r.run(THREADS);

    std::cout << std::endl;
    return 0;
}
