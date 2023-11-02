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


