#include "Timer.h"

void Timer::create(std::unique_ptr<ThreadData> &data)
{
    using namespace std::chrono;

    if (Settings::timeout <= 0) {
        return;
    }

    while (Settings::timeout > 0 && (!data || !data->isTerminated)) { //булева переменная, инициализированная в Utils.h
        static const int DATA_TIMEOUT = 50;

        std::this_thread::sleep_for(milliseconds{DATA_TIMEOUT});
        Settings::timeout -= DATA_TIMEOUT;
    }

    data->isTerminated = true;

    try {
        data->currentProcess.terminate(); //currentProcess - boost::process::child ThreadData::currentProcess, terminate - завершение
    } catch (std::exception &exception) {
        BOOST_LOG_TRIVIAL(error) << exception.what(); // посимвольный вывод кода ошибки
    }

    BOOST_LOG_TRIVIAL(info) << "Timeout";
}
