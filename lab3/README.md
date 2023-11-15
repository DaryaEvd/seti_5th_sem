## PLACES (ASYNC NETWORK)  

### Task  
Using asynchronous programming techniques (for example, CompletableFuture for Java) or reactive programming libraries (RxJava, for example), interact with several publicly available APIs and make an application with any interface based on these APIs. 

#### Requirements  
In this case, the API should be used like this:  
1. All calls must be made using an HTTP library with an asynchronous interface;  
2. All independent API calls must run simultaneously;  
3. API calls that are made based on data received from previous APIs should be formed as an asynchronous chain of calls;  
4. All results of work must be collected into a common object; poetic conclusion of the results of work during the work process is not allowed;  
5. Do not remove the lock waiting for intermediate results in a call chain; only blocking while waiting for the final result is allowed (in the case of a console application).  
6. In other words, the program logic should be formatted as two functions, the method of which returns CompletableFuture (or an analogue in your language) without blocking. The first function is carried out in step 2, the second - in step 2. 4 and 5 from the list below.  

#### Logic of program:  
1. In the input field, the user enters the name of something (for example, “Tsvetnoy Proezd”) and presses the search button;  
2. Location options are searched using method [1] and shown to the user in the form of a list;  
3. The user selects one location;  
4. Using method [2], the weather in a location is searched;  
5. Using method [3], interesting places in the location are searched, then for each found place, using method [4], descriptions are searched;  
6. Everything found is shown to the user.  

#### API METHODS:  
1. getting locations with coordinates and names:  
`https://docs.graphhopper.com/#operation/getGeocode`  
2. getting weather by coordinates   
`https://openweathermap.org/current`  
3. getting a list of interesting places by coordinates:  
`https://opentripmap.io/docs#/Objects%20list/getListOfPlacesByRadius`  
4. getting a description of a place by its id:  
` https://opentripmap.io/docs#/Object%20properties/getPlaceByXid`  

### What is the point of the lab?
You need to make an application in which requests for a location are made asynchronously, and then for this location the weather is searched, interesting places within a (some self-set) radius and descriptions of these places. And everything is done asynchronously, because it’s faster.
The information received on location, weather, places and descriptions is taken from the API, so read [here](https://habr.com/ru/articles/464261/).

For example, the user enters `Kemerovo` into the console, then the following list should be displayed (for example)   
```
1. Kemerovo [55.3550907, 86.0871213], Russia, place, city
2. Kemerovó-lakótelep [48.1183396, 19.8021625], Hungary, Salgótarján, place, suburb
3. Kešetovo [46.1539068, 15.0499646], Slovenia, Trbovlje, highway, living_street
4. Kemerovo - Promyshlennaya [55.1955602, 86.0015435], Russia, highway, secondary
5. Kemerovo - Promyshlennaya [55.2213858, 86.030791], Russia, highway, secondary
6. Kemerovo - Promyshlennaya [55.2259483, 86.0308483], Russia, highway, primary
7. Kemerovo - Promyshlennaya [55.2259483, 86.030737], Russia, highway, primary
8. Kemerovo - Promyshlennaya [55.2215989, 86.0306752], Russia, highway, secondary
9. Kemerovo Agglomeration [55.42353665, 86.63728357496711], Russia, boundary, economic
10. Kemerovo - Promyshlennaya [55.0845013, 85.9916531], Russia, highway, secondary
```
Where does this list come from?
We make an Http request: `https://graphhopper.com/api/1/geocode?q=Kemerovo&locale=en&limit=3&key=606d0fc3-813d-43fc-8c19-f9e6666f0677`  
By default, 5 objects are shown, but in order not to trash the readme, I set the limit = 3, just for example  

#### !!! Attention !!! register on each site of API and get your own API_KEY    

#### RECOMMENDATION 
In order to quickly and properly view the request, I recommend this website (or extension): `https://app.boomerangapi.com/workspace`  
The output from the above http request will be like this:
```
{
    "hits": [
        {
            "point": {
                "lat": 55.3550907,
                "lng": 86.0871213
            },
            "extent": [
                85.909499,
                55.2565763,
                86.2920401,
                55.5449212
            ],
            "name": "Kemerovo",
            "country": "Russia",
            "countrycode": "RU",
            "state": "Kuzbass",
            "osm_id": 1312868,
            "osm_type": "R",
            "osm_key": "place",
            "osm_value": "city"
        },
        {
            "point": {
                "lat": 48.1183396,
                "lng": 19.8021625
            },
            "name": "Kemerovó-lakótelep",
            "country": "Hungary",
            "countrycode": "HU",
            "city": "Salgótarján",
            "state": "Great Plain and North",
            "postcode": "3108",
            "osm_id": 1846777603,
            "osm_type": "N",
            "osm_key": "place",
            "osm_value": "suburb"
        },
        {
            "point": {
                "lat": 55.42353665,
                "lng": 86.63728357496711
            },
            "extent": [
                84.4478947,
                54.0101874,
                89.4026437,
                56.8351312
            ],
            "name": "Kemerovo Agglomeration",
            "country": "Russia",
            "countrycode": "RU",
            "osm_id": 14392586,
            "osm_type": "R",
            "osm_key": "boundary",
            "osm_value": "economic"
        }
    ],
    "locale": "en"
}
```
The result is in json-format.  
#### HOW TO WORK WITH JSON?   
Read [here](https://habr.com/ru/articles/554274/)  

And so on by analogy with each of the APIs. Read the documentation for the APIs, see what it outputs, and parse it from there, etc.  

#### Attention 2.0  
If queries don't work, it's probably due to limitations(?) on the number of queries. Use VPN.  
