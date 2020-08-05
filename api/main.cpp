#include <iostream>
#include <string>
#include <cpr/cpr.h> //curl request to get the data from desired API
#include <nlohmann/json.hpp>//to define json object

using namespace std;
using namespace cpr;
using json = nlohmann::json;


 void get_location(string location_api, string address, double* lat, double* lng) {
    /// <summary>
    /// find the latitude and longitude of an address with the opencage API
    /// address should separate with comma and without any space such as
    /// 17,goodyear,irvine
    /// </summary>
    /// <param name="location_api"><open cage api key/param>
    /// <param name="address">string of the address</param>
    /// <param name="lat">will return latitude</param>
    /// <param name="lng">will return longitude</param>

    string base_url = "https://api.opencagedata.com/geocode/v1/json?q=";
    string final_url = base_url + address + "&key=" + location_api;
    Response r = Get(Url{ final_url });
    
    json j = json::parse(r.text);
    *lat = j["results"][1]["geometry"]["lat"];//return latitude
    *lng = j["results"][1]["geometry"]["lng"];//return longitude

}

 json get_forecasting(string solcast_api, string location_api, string address, int hours) {
     /// <summary>
     ///  by getting lat and lng it will return a json file of the forecasted weather data for 48 to 168 hours
     /// </summary>
     /// <param name="solcast_api">Solcast API key</param>
     /// <param name="location_api">opencage API key</param>
     /// <param name="address">string address</param>
     /// <param name="hours">number of selected hours for forecastin between 48 to 168 hours</param>
     /// <returns>forecasted data with json format</returns>

     double lat, lng;
     get_location(location_api, address, &lat, &lng);

     string base_url = "https://api.solcast.com.au/world_radiation/forecasts?format=json&latitude=";
     string final_url = base_url + to_string(lat) + "&longitude" + to_string(lng) + "&hours=" + to_string(hours) + "&api_key=" + solcast_api;
     Response r = Get(Url{ final_url });
     json j = json::parse(r.text);

     return j;//return json file of forecasted data for 7 days
}

json get_recorded(string solcast_api, string location_api, string address, int hours) {
    /// <summary>
    /// by getting lat and lng it will return a json file of the recorded weather data 48 to 168 hours
    /// </summary>
    /// <param name="solcast_api">Solcast API key</param>
    /// <param name="location_api">opencage API key</param>
    /// <param name="address">string address</param>
    /// <param name="hours">number of selected hours for forecastin between 48 to 168 hours</param>
    /// <returns>recorded data with json format</returns>

    double lat, lng;
    get_location(location_api, address, &lat, &lng);
    string base_url = "https://api.solcast.com.au/world_radiation/estimated_actuals?format=json&latitude=";
    string final_url = base_url + to_string(lat) + "&longitude" + to_string(lng) +  "&hours=" + to_string(hours) + "&api_key=" + solcast_api;
    Response r = Get(Url{ final_url });
    json j = json::parse(r.text);

    return j;//return json file of recorded data
}

int main() {

    string solcast_api = "Cy_svKl2MNHobcsBWczeH-6MQ6cp2N6N";//solcast API key
    string location_api = "23e6edd3ccc7437b90c589fd7c9c6213"; //opencage API key

    int hours = 168; // it can be from 48 to 168 hours 
    
    string address;
    //address = "17,goodyear,irvine";
    cin >> address;//without space and separate with comma

    json forecast = get_forecasting(solcast_api, location_api, address, hours);  //forecasted data 

    json recorded = get_recorded(solcast_api, location_api, address, hours);  //recorded data


    //just to look inside of the json file
    for (const auto& item : recorded.items())
    {
        cout << item.key() << "\n\n" << "%%%%%%%%%%%" << "\n\n";
        for (const auto& val : item.value().items())
        {
            cout << val << "\n" << "___________________" << "\n\n" << "  " << val.key() << "::::::: " << val.value() << "\n";
        }
    }

}