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

### В чём смысл лабы?   
Нужно сделать приложение, в котором запросы на локацию делаются асинхронно, а потом для этой локации ищется погода, интересные места в пределах (какого-то установленного самим) радиуса и описания к этим местам. И делается всё асинхронно, потому что так быстрее.  
Сама получаемая инфа по локации, погоде, местам и описанию берется из API, чо каво - читать [тут](https://habr.com/ru/articles/464261/).  

Например, пользователь вводит в консоль `Kemerovo`, потом должен высветиться вот такой список (к примеру)   
```
1. Kemerovo [55.3550907, 86.0871213], Russia, null, place, city
2. Kemerovó-lakótelep [48.1183396, 19.8021625], Hungary, Salgótarján, place, suburb
3. Kemerovo Agglomeration [55.42353665, 86.63728357496711], Russia, null, boundary, economic
4. Kemerovo - Promyshlennaya [55.0845013, 85.9916531], Russia, null, highway, secondary
5. Kemerovo - Promyshlennaya [55.0807137, 85.9852439], Russia, null, highway, secondary
6. Kemerovo - Promyshlennaya [55.1148602, 86.0113947], Russia, null, highway, secondary
7. Kemerovo - Promyshlennaya [55.1955602, 86.0015435], Russia, null, highway, secondary
8. Kemerovo - Promyshlennaya [55.0567005, 85.9476312], Russia, null, highway, secondary
9. Kemerovo - Promyshlennaya [55.2259483, 86.030737], Russia, null, highway, primary
10. Kemerovo - Promyshlennaya [55.2215989, 86.0306752], Russia, null, highway, secondary
```
Откуда этот список появляется?   
Мы делаем Http-запрос: `https://graphhopper.com/api/1/geocode?q=Kemerovo&locale=en&limit=3&key=606d0fc3-813d-43fc-8c19-f9e6666f0677`  
*По дефолту показывается 5 объектов, но чтоб не захламлять ридми, поставила лимит=3, прост для примера    

#### !!! Attention !!! надо зарегаться на каждом сайте апишек и в лк получить свой API_KEY    

#### РЕКОМЕНДАЦИЯ  
Для того, чтоб быстро и в норм виде посмотреть запрос, рекомендую вот этот сайтик(или расширение):  `https://app.boomerangapi.com/workspace`   
На приведеный выше http-запрос вывод будет таким:  
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
Этот результат запроса показаыввется в json-формате.   
#### Как работать с json-ом?  
Читать [тут](https://habr.com/ru/articles/554274/)  

И так по аналогии с каждым из API. Читать документацию по апишке, смотреть, что выводит, и от этого парсить и т.д.  

#### Attention 2.0  
Если запросы не работают, то, возможно, из-за ограничений(?) на количество запросов. Использовать VPN тогда.  