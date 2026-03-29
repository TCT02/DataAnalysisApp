// DataAnalysisApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <curl\curl.h>
#include <cstdlib>
#include <windows.h>
#include <nlohmann/json.hpp>
CURLcode curl_global_init(long flags);

// for convenience
using json = nlohmann::json;

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

void callNWSAPI(memory* chunk, std::string link) {

    CURL* handle;
    CURLcode res;

    //Provides User Agent OR API key to identify your application
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: TCTWeather (tct@gmail.com)");
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
        //curl_easy_setopt(handle, CURLOPT_URL, link);

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
            //printf("Found %s\n", (chunk->memory));
            printf("Found a JSON file\n");

          
        }

        
        //free(chunk->memory);
        curl_easy_cleanup(handle);
        //return dataPoint;
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
    std::string cityLink = "";
    std::string forecastLink = "";

    //Determined by user input

    //Prompt user for location and resolve that to a link from a dictionary of places
    //For now, it will be fixed on San Diego
    cityLink = "https://api.weather.gov/points/32.7157,-117.1611";

    struct memory* chunkPtr = &chunk;

    //Call API to obtain weather data for the first time
    callNWSAPI(chunkPtr, cityLink);
    //printf("Data Printed in Main %s", dataPoint);

    //Parse Json file for first API call
    json jsonFileBuffer = json::parse(chunkPtr->memory);

    printf("(In Main Function) Found %s\n", (chunkPtr->memory));

    forecastLink = jsonFileBuffer["properties"]["forecast"];
    std::cout << "Forecast from dataFile \n" << forecastLink << std::endl;

    std::cout << "-------------------------------------------------------------------- \n" << std::endl;
    //Call NWS a second time to pull the data specifically from the forecast link found above.
    callNWSAPI(chunkPtr, forecastLink);

    printf("Found Forecast from NWS %s\n", (chunkPtr->memory));

    //Parse Json File for forecast
    //json jsonFileBufferTwo = json::parse(chunkPtr->memory);
    //std::cout << "Forecast Periods \n" << jsonFileBufferTwo["properties"]["periods"] << std::endl;


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
