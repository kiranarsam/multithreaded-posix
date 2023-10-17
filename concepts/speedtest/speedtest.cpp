/**
 * Download Speed:
 *      Download speed is the maximum amount of data your computer can receive from
 *  the internet in a single second.
 *
 * Upload Speed:
 *      Upload speed is the maximum amount of data your computer can send to
 *  the internet in a single second.
 *
 * Ping (Latency):
 *      Ping and Latency are often used interchangeably.
 *  However, when used in the context of speed test results, both ping and latency are a measurement of
 *  how quickly you can get a response after you’ve sent a request over a network.
 *  Latency and slow ping rates are often affected by packet size, packet loss, and jitter.
 *
 * Packet Loss:
 *      Packet loss occurs when lost packets of data don’t reach their destination after being
 *  sent across a network. This can often result in increased latency and slower network connection.
 *
 * Jitter:
 *      Jitter occurs when there is a variation in the time between when a signal is transmitted and
 *  when it is received over a network. This is often a result of network congestion.
 *
 * Mbps (megabits per second):
 *      Mbps stands for “megabits per second”. Mbps is a unit of measurement used to show
 *  the speed of a network or internet connection. Megabits per second refers to how many megabits can
 *  be downloaded or uploaded over a network. Internet service providers (ISPs) typically use Mbps
 *  when communicating the maximum download and upload speeds of their different internet plans.
 *
 */

#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

double performSpeedTest(const std::string &url)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        double speed = -1.0; // Speed in Mbps

        if (res == CURLE_OK)
        {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            if (response_code == 200)
            {
                // Parse the response to get speed details
                // In a real-world scenario, you may want to use a JSON library for better parsing
                // For simplicity, we assume a simple JSON response format here
                size_t pos = response.find("download");
                if (pos != std::string::npos)
                {
                    size_t start = response.find(":", pos) + 1;
                    size_t end = response.find(",", start);
                    std::string downloadSpeed = response.substr(start, end - start);
                    speed = std::stod(downloadSpeed) / (1024 * 1024); // Convert to Mbps
                }
            }
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return speed;
    }

    return -1.0;
}

int main()
{
    // Replace with the Speedtest server URL you want to use
    std::string speedtestUrl = "https://speedtest.net/api/js/servers";

    double downloadSpeed = performSpeedTest(speedtestUrl);

    if (downloadSpeed != -1.0)
    {
        std::cout << "Download Speed: " << downloadSpeed << " Mbps\n";
    }
    else
    {
        std::cerr << "Speed test failed\n";
    }

    return 0;
}
