/////////////////////////////////////////////////////////
//                                                     //
//                 Algorithms Assessment               //
//                                                     //
//                       Y3848937                      //
//                                                     //
/////////////////////////////////////////////////////////

//                      -MAIN.C-




///////////////////////LIBRARTIES////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strlen() function
#include <time.h> //used for timing

#include "resources.h"

///////////////////////DEFINITIONS///////////////////////
// for Dijkstra2
// Number of vertices in the graph
#define MAX_NO_VERTICES 100

///////////////////FUNCTION PTOTOTYPES///////////////////
//File and String Handling:
bool openFile(char fileArray[40][130], int fileMax);
bool getPlaceName(char* selectedRow, char nameType, char* extractedName);
bool getDistValue(char* selectedRow, int* extractedDistance);
bool extractFields(char* selectedRow, char* extractedLocation, char* extractedDestination, int* extractedDistance);
bool compareNames(char* place1, char* place2);

//Dijkstras/Primary Functions:
void dijkstra(graph* graph1, int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS]);
void dijkstra2(int adjacencyMatrix[MAX_NO_VERTICES][MAX_NO_VERTICES], int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS]);
void startTimedAlgorithm(graph* Graph1, char placeNames[100][MAX_CHARS], int distMatrix[MAX_NO_VERTICES][MAX_NO_VERTICES]);


//Utility Functions
int getCityIndex(char* cityName, char placeNames[100][MAX_CHARS]);
void printPathway(int* pathwayIndexes, int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS]);
int findMinDistance(int* distances, bool* shortestPathFlag);
bool initialiseDistMatrix(char fileArray[40][130], int* distances, int distMatrix[100][100], char *destinations[ROWS][MAX_CHARS], char *locations[ROWS][MAX_CHARS]);
int getStartingCityIndex(char placeNames[100][MAX_CHARS]);
int getDestinationCityIndex(char placeNames[100][MAX_CHARS]);

int main()
{
    char *locations[ROWS][MAX_CHARS]; //Array of location names
    char *destinations[ROWS][MAX_CHARS]; //Array of destination names
    int distances[ROWS]; //Array of distance values
    char fileArray[40][130];//Array to store inputted text file

    openFile(fileArray, 100);       //Allowance made for 100 possible pathways
    int  distMatrix[100][100];


    int  numPlaces = 0;
    char placeNames[100][MAX_CHARS];	// Allowance made for 100 possible place names.



    initialiseDistMatrix(fileArray, distances, distMatrix, destinations, locations);



    // ASSUMPTIONS: Locations might contain the same place more than once.
    //Destinations might contain the same place more than once. A place might be listed in the locations array but not in the destinations array (and vice versa).

    // For each row get the location name from the locations array. Search through the
    //placeNames array and add the location name (and inc num places) if it's not already there.
    //Output of this stage is the distMatrix x co-ordinate (where x is the placeName index where
    //the location name resides).


    char temp[MAX_CHARS];
    bool match_found = false;
    int noOfDuplicates = 0;
    int location_count = 0;
    int place_count = 0;
    int  xValue = 0;
    int  yValue = 0;

    // Fill the placenames array with a non duplicated set of names
    for (location_count = 0; location_count < 40; location_count++) // iterate through the rows(paths)
    {
        // placenames from locations
        match_found = false;
        for (place_count = 0; place_count < numPlaces; place_count++)
        {
            if (true == compareNames(locations[location_count], placeNames[place_count]))
            {
                match_found = true;
                noOfDuplicates++;
                break;
            }
        }
        xValue = place_count;

        if (false == match_found)
        {
            snprintf(temp, sizeof temp, "%s", locations[location_count]);
            snprintf(placeNames[numPlaces], sizeof placeNames[numPlaces], "%s", temp);
            numPlaces++;
        }

        // placenames from destinations
        match_found = false;
        for (place_count = 0; place_count < numPlaces; place_count++)
        {
            if (true == compareNames(destinations[location_count], placeNames[place_count]))
            {
                match_found = true;
                noOfDuplicates++;
                break;
            }
        }
        yValue = place_count;

        if (false == match_found)
        {
            snprintf(temp, sizeof temp, "%s", destinations[location_count]);
            snprintf(placeNames[numPlaces], sizeof placeNames[numPlaces], "%s", temp);
            numPlaces++;
        }
        printf("\n");

        // Set distMatrix[yValue][xValue] to the distance integer value for the current row.
        distMatrix[yValue][xValue] = distances[location_count];
        printf("Adding distMatrix[%d][%d] = %d\n", yValue, xValue, distances[location_count]);

        // Now set distMatrix[xValue][yValue} to the distance integer for the current row.
        distMatrix[xValue][yValue] = distances[location_count];
        printf("Adding distMatrix[%d][%d] = %d\n", xValue, yValue, distances[location_count]);



    } // end-for


    int b;
    for(b = 0; b < numPlaces; b++)
    {
        printf("Index %d = %s\n", b, placeNames[b] );
    }


    struct Graph_* Graph1;
    Graph1 = graphConstructor(NOOFVERTICES);
    graphSetValues(Graph1, distMatrix);


    if(checkListIsEmpty(Graph1) == false)
    {
        printf("\n\n[Test pass] List is not empty\n");
    }
    else
    {
        printf("[Test fail] List is reported as empty\n");
    }

    printf("See Above ^");
    printf("\n\nGraph1 Contents:\n\n");
    graphDisplay(Graph1, placeNames);

    char choice;

    while(1)
    {
        printf("\n\nBegin? Y/N");
        scanf("%c", &choice);
        if (choice == 'Y')
        {
            startTimedAlgorithm(Graph1,placeNames,distMatrix);
        }
        else if (choice == 'N')
        {
            break;
        }
        else
        {
            printf("Invalid input, please enter 'Y' or 'N'");
            getchar();
            choice == 'Y';
        }

    }

    graphDestructor(Graph1);
    return 0;
}
/////////////////////////////////////////////////////////////////
// openFile - Used to open the input text file and store its
//            contents line by line into a 2d array.
// Params:
// fileArray - 2d array to store the contents of the text file
// fileMax   - The max number of rows in the text file
// Returns true if successful
/////////////////////////////////////////////////////////////////

bool openFile(char fileArray[40][130], int fileMax)
{

    bool status = false; //Status variable to check whether function is successful

    //Params checked
    if ((NULL == fileMax) || (NULL == fileArray))
    {
        printf("Error: invalid parameter in openFile()");
        return status;
    }

    //File pointer to read contents set up and file is opened
    FILE* fpointer;
    fpointer = fopen("citypairs.txt","r");
    fileMax = 0;

    while(1)
    {
        //NOTE: Unsigned char variable used to fix bug where misc ascii characters at
        //the start of the file were being read.
        unsigned char linechar = (unsigned char)fgetc(fpointer); //linechar used as a temp location to store the contents of the file pointer
        int b = 0;
        fileArray[fileMax][b] = '\0'; //File array given initial null value again to avoid unwanted characters

        //Each row is read until the end of the file
        while(linechar!='\n' && !feof(fpointer))
        {
            //Temporary linechar value is stored in the array
            fileArray[fileMax][b++] = (char)linechar;
            linechar = (unsigned char)fgetc(fpointer);
        }
        if(feof(fpointer))
        {
            //When the end of the file is reached, the status is updated
            status = true;
            break;
        }
        fileArray[fileMax][b] = '\0';
        fileMax++;
    }
    fclose(fpointer);
    return status;
}

/////////////////////////////////////////////////////////////////
// getPlaceName - For a given row, retrieves a source or
//                destination place name.
// Params:
// selectedRow - pointer to row of text file array
// nameType - determines whether a location or destination is
//            to be extracted (loc = 1, dest = 2)
// extractedName - pointer to array to contain the extracted
//                 name
// Returns true if successful
/////////////////////////////////////////////////////////////////

bool getPlaceName(char* selectedRow, char nameType, char* extractedName)
{
    bool status = false;
    int charCount = 0;
    int typeCount;

    int nameBeginLocation = 0; //the starting location of the placename
    int nameEndLocation = 0;   //the ending location of the placename
    int storageIndex = 0;

    // Params checked.
    if ((NULL == selectedRow) || (NULL == extractedName) || ((nameType != 1) && (nameType != 2)))
    {
        printf("Error: invalid parameter in getPlaceName()");
        return status;
    }

    for (typeCount = 0; typeCount < nameType; typeCount++) //Loops down through the row depending on whether a location
        //or destination is to be retrieved.
    {
        //Search to find the starting location of the name
        while (charCount < strlen(selectedRow))
        {
            if ((toupper(selectedRow[charCount]) >= 'A') && (toupper(selectedRow[charCount]) <= 'Z'))
            {
                nameBeginLocation = charCount;
                break; //Break when found
            }
            charCount++;
        }

        //Search to find the ending location of the name
        while (charCount < strlen(selectedRow))
        {
            if (!((toupper(selectedRow[charCount]) >= 'A') && (toupper(selectedRow[charCount]) <= 'Z')))
            {
                nameEndLocation = charCount;
                status = true;
                break; // Break when found
            }
            charCount++;
        }

    }

    //Once the name parameters are found, the placename string is extracted.
    if (true == status)
    {
        for (charCount = nameBeginLocation; charCount < nameEndLocation; charCount++)
            extractedName[storageIndex++] = selectedRow[charCount];

        extractedName[storageIndex] = '\0'; //Null terminator added to the string
    }

    return status;
}

/////////////////////////////////////////////////////////////////
// getDistValue - For a given row, retrieves a distance value
//
// Params:
// selectedRow - pointer to row of text file array
// extractedDistance - pointer to int variable to contain the
//                     extracted value.
// Returns true if successful
/////////////////////////////////////////////////////////////////

bool getDistValue(char* selectedRow, int* extractedDistance)
{
    bool status = false;
    int charCount = 0;
    int lengthofRow = strlen(selectedRow);
    int valueBeginLocation = 0;
    int valueEndLocation = 0;
    char tempString[5]; // Temp storage of the value as a string, max distance assumed to be 5 digits or less

    // Param checking.
    if ((NULL == selectedRow) || (NULL == extractedDistance))
    {
        printf("Error: invalid parameter in getDistValue()");
        return status;
    }

    // Row is searched to find the beginning of the value.
    while (charCount < lengthofRow)
    {
        //Searching for a nonzero value to remove any zeros at the start of the int
        if (('1' <= selectedRow[charCount]) && (selectedRow[charCount] <= '9'))
        {
            valueBeginLocation = charCount;
            break; //Loop is broken once found
        }
        charCount++;
    }

    // Row is searched to find the end of the value, continuing from the starting location of the value
    while (charCount < lengthofRow)
    {
        if ((!(('0' <= selectedRow[charCount]) && (selectedRow[charCount] <= '9')))|| (charCount == lengthofRow - 1)) // we've also found the end of the value if we've reached the end of the string
        {
            valueEndLocation = charCount;
            status = true;
            break; //Loop is broken once found and status value updated.
        }
        charCount++;
    }

    // if value was found then return via extractedDistance parameter
    if (true == status)
    {
        for (charCount = 0; charCount <= valueEndLocation - valueBeginLocation; charCount++)
            tempString[charCount] = selectedRow[valueBeginLocation + charCount];

        tempString[charCount] = '\0'; //Null terminator added to the string

        *extractedDistance = atoi(tempString); //String casted to an integer and returned to the pointer, extractedDistance
    }

    return status;
}

/////////////////////////////////////////////////////////////////
// extractFields - Uses getDistValye() and getPlaceName() to extract all the fields of a given row.
//
// Params:
// extractedLocation - pointer to array to store location name
// extractedDestination - pointer to array to store destination name
// extractedDistance - pointer to int variable to store distance value
// Returns true if successful
/////////////////////////////////////////////////////////////////
bool extractFields(char* selectedRow, char* extractedLocation, char* extractedDestination, int* extractedDistance)
{
    bool status = false;

    // Params checked.
    if ((NULL == selectedRow) || (NULL == extractedLocation) || (NULL == extractedDestination))
    {
        printf("Error: invalid parameter in extractFields()\n");
        return;
    }

    //Status used to check whether the next fields are valid. If status is returned false, no more fields will be retrieved.

    // Source name is retrieved by passing a 1 as "nameType" to getPlaceName()
    status = getPlaceName(selectedRow, 1, extractedLocation);

    // Destination name is retrieved by passing a 2 as "nameType" to getPlaceName()
    if (true == status)
    {
        status = getPlaceName(selectedRow, 2, extractedDestination);
    }


    //Distance retrieved using getDistValue
    if (true == status)
    {
        status = getDistValue(selectedRow, extractedDistance);
    }

    return status;
}

/////////////////////////////////////////////////////////////////
// compareNames - Utility function to compare 2 string names
//
// Params:
// place1 - pointer to first place name
// place2 - pointer to second place name
// Returns true if match is found
/////////////////////////////////////////////////////////////////

bool compareNames(char* place1, char* place2)
{
    bool status = false;
    //Params Checked.
    if ((NULL == place1) || (NULL == place2))
    {
        printf("Error: invalid parameter in compareNames()\n");
        return;
    }

    //strcmp function used to compare the 2 names, returns true when match found
    if (strcmp(place1, place2) == 0)
    {
        status = true;
    }

    return status;
}

/////////////////////////////////////////////////////////////////
// getCityIndex - Utility function to return a city's index based on its position in the placeNames array
//
// Params:
// cityName - pointer to the place name to index
// placeNames - array of place names
// Returns the index as an integer of the requested city
/////////////////////////////////////////////////////////////////

int getCityIndex(char* cityName, char placeNames[100][MAX_CHARS])
{
    int cityIndex;
    int j;
    bool match = false;

    //Params Checked.
    if ((NULL == cityName) || (NULL == placeNames))
    {
        printf("Error: invalid parameter in getCityIndex()\n");
        return;
    }

    //Names array is looped through..
    for (j=0; j <= 100; j++)
    {
        match = compareNames(cityName, placeNames[j]);

        if (match == true)
        {
            break;
            //...loop broken when match found

        }
    }

    //if j exceeds the number of items in the placenames array...
    if (j == 101)
    {
        match = false;
        cityIndex = DIST_INF;
        printf("\nError: City name not found!\n");
        //...cityIndex given infinite value and error message displayed
    }

    //if match is found...
    else if (match = true)
    {
        cityIndex = j;
        printf("\nCity index found: %d\n", cityIndex);
        //cityIndex returns a value of j and outputted
    }

    return cityIndex;
}

/////////////////////////////////////////////////////////////////
// dijkstra - Runs dijkstras algorithm through a graph using a heap (MORE EFFICIENT)
//
// Params:
// graph* graph1 - pointer to a graph structure representing an adjacency graph
// startingCityIndex - index of the starting city
// destinationCityIndex - index of the desired destination city
// placeNames - array of placenames used when outputting the result of the algorithm
/////////////////////////////////////////////////////////////////

void dijkstra(graph* graph1, int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS])
{

    //Params Checked.
    if ((NULL == graph1) || (startingCityIndex == DIST_INF) || (destinationCityIndex == DIST_INF) || (NULL == placeNames))
    {
        printf("Error: invalid parameter in dijkstra()\n");
        return;
    }
    int* pathwayIndexes[30];
    int i = 0;
    int path_count =0;
    for (i=0; i<21; i++)
    {
        pathwayIndexes[i] = DIST_INF;
    }
    struct List* pList = graph1->linkedlist;
    struct node* pNode = pList->head;

    int noVertices = graphgetv(graph1);

    //Array set up to store distance values
    int* pDistances = malloc( noVertices * sizeof(int));

    //Heap is required to store the edge values
    struct heap* pHeap;
    pHeap = heapConstructor(noVertices);
    pHeap->size = noVertices;

    //Heap and distances array initialised
    int verticesCount;
    for (verticesCount = 0; verticesCount < noVertices; ++verticesCount)
    {
        if (verticesCount != startingCityIndex)
        {
            pDistances[verticesCount] = DIST_INF;
            //distances are all given infinite values to represent no path found...
        }
        else if (verticesCount == startingCityIndex)
        {
            pDistances[verticesCount] = 0;
            //...with the exception of the starting vertex which has a value of zero.
        }
        pHeap->array[verticesCount] = heapNodeConstructor(verticesCount, pDistances[verticesCount]);
        pHeap->pPositions[verticesCount] = verticesCount;
    }
    //Starting city is added to the heap as the root
    pHeap->array[startingCityIndex]->distance = 0;
    decreaseDist(pHeap, startingCityIndex, pDistances[startingCityIndex]);



    while (getHeapSize(pHeap) != 0)
    {
        //node with minimum distance is extracted and stored


        struct heapNode* heapNode1 = getMinimumNode(pHeap);
        int vertex_num = heapNode1->vertexNum;
        struct node* pNode = graph1->linkedlist[vertex_num].head;

        //Uncomment following function to check the extracted node in the heap
        //checkVertex(pNode, vertex_num);


        //nodes connected to the selected node have their distance values updated
        while(pNode != NULL)
        {

            int nodeDest = pNode->cityIndex;
            //Following line of code used to check the value of the node
            //nodeDisplay(pNode);
            if ((pNode->weight + pDistances[vertex_num] < pDistances[nodeDest]))
            {

                pDistances[nodeDest] = pDistances[vertex_num] + pNode->weight;
                //heap updated with new minimum values
                decreaseDist(pHeap, nodeDest, pDistances[nodeDest]);
                pathwayIndexes[nodeDest] = vertex_num;

            }
            //next node selected
            pNode = pNode->next;

        }


    }
    printf("\nMinimum distance between %s and %s using adjacency list is: %d\n", placeNames[startingCityIndex], placeNames[destinationCityIndex],pDistances[destinationCityIndex]);

    printPathway(pathwayIndexes,startingCityIndex,destinationCityIndex, placeNames);

    // free heap memory previously reserved using malloc
    free(pDistances);

}

/////////////////////////////////////////////////////////////////
// printPathway - Displays the pathway from a source to destination city
//
// Params:
// pathwayIndexed1 - pointer to an array containing the visited nodes of the heap.
// startingCityIndex - index of the starting city
// destinationCityIndex - index of the desired destination city
// placeNames - array of placenames used when outputting the result of the algorithm
/////////////////////////////////////////////////////////////////
void printPathway(int* pathwayIndexes, int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS])
{
    int x = destinationCityIndex;
    int len = 0;

    char temp[100][100];

    while (x != startingCityIndex)
    {
        // copy name of current city to the path names array
        strcpy(temp[len], placeNames[x]);
        x = pathwayIndexes[x];
        len++;
    }

    printf("%s -> ", placeNames[startingCityIndex]);
    int q;
    for (q = len-1; q > 0; q--)
    {
        printf("%s -> ", temp[q]);
    }
    printf("%s\n", placeNames[destinationCityIndex]);

}

/////////////////////////////////////////////////////////////////
// findMinDistance - Finds the shortest available edge available by searching through the vertices not included in the shortest path
//
// Params:
// distances - pointer to an array containing the visited nodes of the heap.
// shortestPathFlag - index of the starting city
// Returns the next node for dijhkstra2
/////////////////////////////////////////////////////////////////
int findMinDistance(int* distances, bool* shortestPathFlag)
{
    int min_index = DIST_INF;
    int min_node = DIST_INF;
    int vertex_Count;
    for (vertex_Count = 0; vertex_Count < MAX_NO_VERTICES; vertex_Count++)
    {
        if (shortestPathFlag[vertex_Count] == false && distances[vertex_Count] <= min_node)
        {
            min_node = distances[vertex_Count], min_index = vertex_Count;
        }
    }


    return min_index;
}

/////////////////////////////////////////////////////////////////
// dijkstra2 - Runs dijkstras algorithm through an adjacency matrix (LESS EFFICIENT)
//
// Params:
// adjacencyMatrix - 2d array used to map the distances between adjacent cities.
// startingCityIndex - index of the starting city
// destinationCityIndex - index of the desired destination city
// placeNames - array of placenames used when outputting the result of the algorithm
/////////////////////////////////////////////////////////////////
void dijkstra2(int adjacencyMatrix[MAX_NO_VERTICES][MAX_NO_VERTICES], int startingCityIndex, int destinationCityIndex, char placeNames[100][MAX_CHARS])
{
    //Params Checked.
    if ((NULL == adjacencyMatrix) || (startingCityIndex == DIST_INF) || (destinationCityIndex == DIST_INF) || (NULL == placeNames))
    {
        printf("Error: invalid parameter in dijkstra2()\n");
        return;
    }
    int distances[MAX_NO_VERTICES];
    bool shortestPathFlag[MAX_NO_VERTICES];
    int distances_count;
    int vertices_count;

    for (distances_count = 0; distances_count < MAX_NO_VERTICES; distances_count++)
    {
        if (distances_count != startingCityIndex)
        {
            distances[distances_count] = DIST_INF;
        }
        else if (distances_count == startingCityIndex)
        {
            distances[distances_count] = 0;
        }
        shortestPathFlag[distances_count] = false;
    }


    for (vertices_count = 0; vertices_count < MAX_NO_VERTICES - 1; vertices_count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = findMinDistance(distances, shortestPathFlag);

        // Mark the picked vertex as processed
        shortestPathFlag[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        int v;
        for (v = 0; v < MAX_NO_VERTICES; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!shortestPathFlag[v] && adjacencyMatrix[u][v] && distances[u] != DIST_INF
                    && distances[u] + adjacencyMatrix[u][v] < distances[v])
                distances[v] = distances[u] + adjacencyMatrix[u][v];
    }

    // print the constructed distance array
    printf("Minimum distance between %s and %s using adjacency matrix implementation is: %d\n", placeNames[startingCityIndex], placeNames[destinationCityIndex],distances[destinationCityIndex]);
}

/////////////////////////////////////////////////////////////////
// initialiseDistMatriz - Builds the distance matrix from the individual field arrays
//
// Params:
// fileArray - 2d array of text file to extract fields from
// distances - pointer to array of distances extracted from text file
// distMatrix - adjacency matrix to populate
// destinations - array containing destinations to extract
// locations - array containing locations to extract
// Returns true if distmatrix is populated
/////////////////////////////////////////////////////////////////
bool initialiseDistMatrix(char fileArray[40][130], int* distances, int distMatrix[100][100], char *destinations[ROWS][MAX_CHARS], char *locations[ROWS][MAX_CHARS])
{

    bool status = false;
    //Params Checked.
    if ((NULL == fileArray) || (NULL == distances) || (NULL == distMatrix) || (NULL == destinations) || (NULL == locations) )
    {
        printf("Error: invalid parameter in initialiseDistMatrix()\n");
        return;
    }
    // Iterate through every distMatrix element (10000 of them) and set to an initial value which signifies no path info
    // if i=j during this then set distMatrix[i][j] to 0.
    // TODO
    int row_count;
    int col_count;

    for (row_count = 0; row_count < 100; row_count++) // we want to iterate over the range 0 to 99; i.e. row_count < 100 (not 101)
        for (col_count = 0; col_count < 100; col_count++)
            distMatrix[row_count][col_count] = DIST_INF;
    // For each row, extract the fields and put into own array. Output to user as we go.
    printf("\n\n"); // just some spacing

    int j;
    for (j = 0; j < 40; j++)
    {
        extractFields(&fileArray[j][0], &locations[j][0], &destinations[j][0], &distances[j]);
        printf("Row: %d, Loc: %s, Dest: %s, Dist: %d\n", j, locations[j], destinations[j], distances[j]);
    }
    if (NULL != distMatrix)
    {
        status = true;
    }

    return status;
}

/////////////////////////////////////////////////////////////////
// getStartingCityIndex - Retrieves the starting city index from an inputted city name
//
// Params:
// placeNames - 2d array of deduped place names
// Returns the index of the inputted city
/////////////////////////////////////////////////////////////////
int getStartingCityIndex(char placeNames[100][MAX_CHARS])
{
    char* startCityName[50];
    int temp = DIST_INF;
    while (temp == DIST_INF)
    {
        printf("\n\nPlease enter the starting location name: ");
        gets(startCityName);
        temp = getCityIndex(startCityName, placeNames);
    }
    return temp;
}

/////////////////////////////////////////////////////////////////
// getDestinationCityIndex - Retrieves the destination city index from an inputted city name
//
// Params:
// placeNames - 2d array of deduped place names
// Returns the index of the inputted city
/////////////////////////////////////////////////////////////////
int getDestinationCityIndex(char placeNames[100][MAX_CHARS])
{
    char* destinationCityName[50];
    int temp = DIST_INF;
    while (temp == DIST_INF)
    {
        printf("\n\nPlease enter the Destination location name: ");
        gets(destinationCityName);
        temp = getCityIndex(destinationCityName, placeNames);
    }
    return temp;
}


/////////////////////////////////////////////////////////////////
// startTimedAlgorithm - Used to control the timing of both algorithms...used for testing
//
// Params:
// Graph1 - adajcency graph - to run dijkstra() function
// placeNames - list of placeNames - to run dijkstra and dijkstra2 functions
// distMatrix - adjacency matrix - to run dijkstra2() functions
// Returns the index of the inputted city
/////////////////////////////////////////////////////////////////
void startTimedAlgorithm(graph* Graph1, char placeNames[100][MAX_CHARS], int distMatrix[MAX_NO_VERTICES][MAX_NO_VERTICES])
{
    //Params Checked.
    if ((NULL == Graph1) || (NULL == placeNames) || (NULL == distMatrix))
    {
        printf("Error: invalid parameter in startTimedAlgorithm()\n");
        return;
    }




    getchar();
    int startCityIndex = getStartingCityIndex(placeNames);
    int destCityIndex = getDestinationCityIndex(placeNames);




    dijkstra(Graph1, startCityIndex,destCityIndex, placeNames);
    dijkstra2(&distMatrix[0][0], startCityIndex, destCityIndex, placeNames);

    //TIMING TEST CODE
 /*
    int j;
    double total_time = 0;
    double average_time = 0;
    int noOfSamples = 1000;
    for(j = 0; j < noOfSamples; j++)
    {
    clock_t t;
    t = clock();
    //DIJKSTRAS CODE

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    total_time += time_taken;

}
    average_time = total_time/noOfSamples;
    printf("\nTotal time to run function %d times: %f seconds\n", noOfSamples, total_time);
    printf("Average time to run function %d times: %f seconds\n", noOfSamples ,average_time);*/
}
