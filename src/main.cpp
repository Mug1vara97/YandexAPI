#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string.h>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t real_size = size * nmemb;
    output->append(static_cast<char*>(contents), real_size);
    return real_size;
}

std::string curlRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_cleanup(curl);
            return response;
        } else {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return "";
}

// Функция для получения списка рейсов по заданной дате из одного пункта в другой
void getFlights(const std::string& date, const std::string& from, const std::string& to) {
    std::string url = "https://api.rasp.yandex.net/v3.0/search/?apikey=d946fec0-4ee8-4107-9212-07643c816efe"
                      "&format=json&date=" + date + "&from=" + from + "&to=" + to;

    std::string response = curlRequest(url);
    if (!response.empty()) {
        json data = json::parse(response);

        if (!data["segments"].is_null()) {
            for (const auto& segment : data["segments"]) {
                std::string carrierTitle = segment["thread"]["carrier"]["title"];
                std::cout << "Перевозчик: " << carrierTitle << std::endl;
                std::cout << "Отправление: " << segment["departure"] << std::endl;
                std::cout << "Прибытие: " << segment["arrival"] << std::endl;
                std::cout << std::endl;
            }
        } else {
            std::cout << "На указанную дату и в указанное место рейсов не найдено." << std::endl;
        }
    } else {
        std::cout << "Произошла ошибка при выполнении запроса API." << std::endl;
    }
}

// Функция для получения ближайших станций с использованием геокодера
void getNearestStations(const std::string& latitude, const std::string& longitude) { 
    std::string apiKey = "115dc605-fd1d-440c-a25c-f5317ad96828"; 
    std::string url = "https://catalog.api.2gis.com/3.0/items?q=" + latitude + "," + longitude + "&radius=1000&key=" + apiKey;

 
   std::string response = curlRequest(url);
   if (!response.empty()) {
    json data = json::parse(response);

    if (!data["result"].is_null()) {
        for (const auto& result : data["result"]) {
            std::string name = result["name"];
            std::cout << "Станция: " << name << std::endl;
            std::cout << "Адрес: " << result["address_name"] << std::endl;
            std::cout << std::endl;
        }
    } else {
        std::cout << "Рядом с указанным местоположением станций не найдено." << std::endl;
    }
} else {
    std::cout << "Произошла ошибка при выполнении запроса API." << std::endl;
}

}


// Функция для получения информации о перевозчике
void getCarrierInfo(const std::string& code) {
    std::string apiKey = "d946fec0-4ee8-4107-9212-07643c816efe";
    std::string url = "https://api.rasp.yandex.net/v3.0/carrier/?apikey=" + apiKey + "&code=" + code;

    std::string response = curlRequest(url);
    if (!response.empty()) {
        json data = json::parse(response);

        if (!data["title"].is_null()) {
            std::cout << "Перевозчик: " << data["title"] << std::endl;
            std::cout << "Страна: " << data["country_title"] << std::endl;
            std::cout << "Телефон: " << data["phones"][0] << std::endl;
            std::cout << "URL: " << data["url"] << std::endl;
        } else {
            std::cout << "Перевозчик не найден для указанного кода." << std::endl;
        }
    } else {
        std::cout << "Произошла ошибка при выполнении запроса API." << std::endl;
    }
}

int main() {
    std::string date, from, to;
    std::cout << "Введите дату (YYYY-MM-DD(2024-02-01)): ";
    std::cin >> date;
    std::cout << "Введите отправление(Казань - c43): ";
    std::cin >> from;
    std::cout << "Введите прибытие(Москва - c213): ";
    std::cin >> to;

    int choice;
    do {
        std::cout << std::endl;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Просмотр рейсов на указанную дату и места." << std::endl;
        std::cout << "2. Посмотреть ближайшие станции" << std::endl;
        std::cout << "3. Просмотрите информацию об перевозчике." << std::endl;
        std::cout << "Введите ваш выбор (1-3) или 0, чтобы выйти: ";
        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                getFlights(date, from, to);
                break;
            case 2: {
                std::string latitude, longitude;
                std::cout << "Введите широту: ";
                std::cin >> latitude;
                std::cout << "Введите долготу: ";
                std::cin >> longitude;
                getNearestStations(latitude, longitude);
            break;
        }
            case 3: {
                std::string code;
                std::cout << "Введите код перевозчика: ";
                std::cin >> code;
                getCarrierInfo(code);
                break;
            }
            case 0:
                std::cout << "Выход..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Пожалуйста, попробуйте еще раз." << std::endl;
                break;
        }
    } while (choice != 0);

    return 0;
}
