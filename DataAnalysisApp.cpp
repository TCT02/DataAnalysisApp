// DataAnalysisApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <curl\curl.h>

CURLcode curl_global_init(long flags);

struct memory {
    char *memory;
    size_t size;
};

static size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp) {

    size_t realSize = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realSize + 1);

    if (ptr == NULL) {
        return 0;
    }

    mem->memory = ptr;
    memcpy(&mem->memory[mem->size], buffer, realSize);
    mem->size += realSize;
    mem->memory[mem->size] = 0;

    return realSize;
}

int main()
{
    //std::cout << "Hello World!\n";
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* handle;
    CURLcode res;
    struct memory chunk;

    chunk.memory = NULL;
    chunk.size = 0;

    struct curl_slist* headers = NULL;
    

    
    handle = curl_easy_init();
    //curl_global_init(handle);
    if (handle) {
        //Call NWS API for weather data based on San Diego's coord location 
        curl_easy_setopt(handle, CURLOPT_URL, "https://api.weather.gov/points/32.7157,-117.1611");
        //curl_easy_setopt(handle, CURLOPT_URL, "https://api.weather.gov/points/");

        headers = curl_slist_append(headers, "User-Agent: TCTWeather (terry78913@gmail.com)");
        headers = curl_slist_append(headers, "Accept: application/geo+json");

        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &chunk);

        res = curl_easy_perform(handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() returned %s/n", curl_easy_strerror(res));
        }
        else {
            char* dataPoint = NULL;
            printf("We got %d bytes to our callback in memory\n", (int)chunk.size);
            printf("Memory data: \n%s", chunk.memory);

            dataPoint = strstr(chunk.memory, "forecast");
            if (dataPoint) {
                printf("Found Domain at index %d\n", (dataPoint = chunk.memory));
            }
        }

        free(chunk.memory);
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
