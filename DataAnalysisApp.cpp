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

char* callNWSAPI(memory* chunk) {

    CURL* handle;
    CURLcode res;

    //Provides User Agent OR API key to identify your application
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: TCTWeather (terry78913@gmail.com)");
    headers = curl_slist_append(headers, "Accept: application/geo+json");

    //Fields for user input to fill in the future
    std::string fieldQuery = "forecast";
    std::string linkOfData = "";
    std::string apiURL = "";
    std::string apiKey = "";
    std::string fileAccept = "";
    char* dataPoint = NULL;

    handle = curl_easy_init();

    if (handle) {

        //Provides URL for API calls
        curl_easy_setopt(handle, CURLOPT_URL, "https://api.weather.gov/points/32.7157,-117.1611");

        //Sets header option to the content of headers
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

        //Sets the function that is used to handle incoming data 
        // (the function is tailored to however you need to handle new data)
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

        curl_easy_setopt(handle, CURLOPT_WRITEDATA, chunk);

        res = curl_easy_perform(handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() returned %s/n", curl_easy_strerror(res));
        }
        else {
            //Store datapoint
            printf("We got %d bytes to our callback in memory\n", (int)chunk->size);
            //printf("Memory data: \n%s", chunk->memory);

            dataPoint = strstr(chunk->memory, fieldQuery.c_str()); 
            //dataPoint = strstr(chunk->memory, "forecast");
            if (dataPoint) {
                printf("Found %s at index %d\n", fieldQuery.c_str(), (dataPoint = chunk->memory));
            }
        }

        
        //free(chunk->memory);
        curl_easy_cleanup(handle);
        return dataPoint;
    }
}

int main()
{
    //Setup curl for the entire program's lifetime
    curl_global_init(CURL_GLOBAL_ALL);

    char* dataPoint = NULL;
    struct memory chunk;
    chunk.memory = NULL;
    chunk.size = 0;

    struct memory* chunkPtr = &chunk;

    dataPoint = callNWSAPI(chunkPtr);
    printf("Data Printed in Main %s", dataPoint);

    //printf("Data Size: %s\n", chunk.memory);
    free(chunkPtr->memory);
    //free(dataPoint);
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
