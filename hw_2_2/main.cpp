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
#include <condition_variable>

using namespace std::chrono_literals;

const int THREADS = 10;             ///< Число потоков.
const int LOOSER_THREAD_ID = 3;     ///< ID потока, который упадёт.
const double LOOSER_MOMENT = 55.0;  ///< Значение, при котором упадёт.
const int TIME_BASE = 25;           ///< База для сдвига потоков.
const double PROGRESS_INC = 0.5;    ///< Инкремент прогресса.
const double RACE_SCALE = 0.1;      ///< Коэффециент расхождения потоков во времени.
const int CONSOLE_MS = 100;         ///< Интервал обновления консоли.

std::condition_variable logged;     ///< Условие: все данные выведены в консоль.
std::mutex mux;                     ///< Мутекс завершения вывода.
bool is_logged;                     ///< Флаг: все данные выведены в консоль.

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
        case Status::error: return "error";
    default: return "unknown";
    }
}

//! Объект с данными о состоянии потока.
struct ProcessData {
    int process_id;
    double progress = 0.0;
    Status status = Status::init;
};


//! Класс сбора данных о процессе вычислений.
class StatusInfo {
public:

    // Обновляет статус по переданным данным процесса.
    void update_status(int process_id, Status status, const double& progress) {
        std::lock_guard<std::mutex> lf(mux);
        process_data[process_id] = ProcessData{process_id, progress, status};
    }

    // Выводит данные о процессах в консоль.
    void print_output() {
        std::lock_guard<std::mutex> lf(mux);
        system("clear");
        for (auto &pd : process_data) {
            std::cout << std::left << std::setw(5) << pd.second.process_id
                      << std::left << std::setw(15) << get_percent_string(pd.second.progress)
                      << std::left << std::setw(10) << status_to_string(pd.second.status)
                      << std::endl;
        }
    }

    // Формирует строку из звёздочек.
    std::string get_percent_string(const double& progress) {
        int num = progress * 0.1;
        return std::string(num, '*');
    }

    // Устанавливает флаг: все процессы завершены.
    void set_complete() {
        complete.store(true);
    }

    // Возвращает значение флага "все процессы завершены".
    bool is_complete() {
        return complete.load();
    }

private:

    std::mutex mux;
    std::map<int, ProcessData> process_data = {};
    std::atomic<bool> complete{false};
};


//! Класс для печати в консоль.
class ConsoleWriter {
public:
    ConsoleWriter(StatusInfo *si):si(si){}

    void run() {
        std::thread tr([this]() {
            while (!si->is_complete()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(CONSOLE_MS));
                this->si->print_output();
            }

            std::lock_guard<std::mutex> lock(mux);
            is_logged = true;
            logged.notify_one();

        });
        tr.detach();
    };

    StatusInfo *si;
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

        sp->set_complete();

        std::unique_lock<std::mutex> lock(mux);
        logged.wait(lock, [] {return is_logged; });
    }

    void loop_method(int process_id, int millisec) {
        double progress = 0;

        while (progress != 100.0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
            progress += PROGRESS_INC;
            sp->update_status(process_id, Status::working, progress);

            if (progress > LOOSER_MOMENT && process_id == LOOSER_THREAD_ID) {
                sp->update_status(process_id, Status::error, 0.0);
                return;
            }
        }

        sp->update_status(process_id, Status::finished, 100.0);
        return;
    }

private:
    StatusInfo *sp = nullptr;
};


int main() {

    StatusInfo sp;
    ConsoleWriter cw(&sp);
    Runner r(&sp);

    cw.run();
    r.run(THREADS);

    return 0;
}
