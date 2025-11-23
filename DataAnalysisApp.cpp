// DataAnalysisApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <curl\curl.h>

CURLcode curl_global_init(long flags);

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp);

int main()
{
    std::cout << "Hello World!\n";
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* handle;
    CURLcode res;

    handle = curl_easy_init();
    //curl_global_init(handle);
    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, "https://example.com/");
        res = curl_easy_perform(handle);
        //curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() returned %s/n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(handle);
    }


    //Test coords for data 32.93066677912789,-117.14432584599716
    //test link for data https://api.weather.gov/points/32.93066677912789,-117.14432584599716

    curl_global_cleanup();
    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
