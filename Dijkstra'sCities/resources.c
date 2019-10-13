//                      -RESOURCES.C-



#include <stdio.h>
#include <stdlib.h>
#include "resources.h"


/////////////////////////////////////////////////////////////////
// getgraphv - Returns the number of vertices for a specified graph
//
// Params:
// graph1 - pointer to chosen graph
// Returns the number of vertices
/////////////////////////////////////////////////////////////////
int graphgetv(struct Graph_* graph1)
{
    return graph1->noVeretices;
}

/////////////////////////////////////////////////////////////////
// graphSetValues - Builds adjacency graph based on the distance adjacency matrix
//
// Params:
// pGraph - 2d array to store the contents of the text file
// distMatrix   - The max number of rows in the text file
// Returns true when graph is populated.
/////////////////////////////////////////////////////////////////
bool graphSetValues(struct Graph_ * pGraph, int distMatrix[100][100])
{
    bool status = false; //Status initialised to false

    //Params Checked.
    if ((NULL == pGraph) || (NULL == distMatrix))
    {
        printf("Error: invalid parameter in graphSetValues()");
        return status;
    }

    int distMatrix_Row;
    int distMatrix_Col;

    //Iteration through adjacency matrix
    for (distMatrix_Row = 0; distMatrix_Row < 100; distMatrix_Row ++)
    {
        for(distMatrix_Col = 0; distMatrix_Col < 100; distMatrix_Col++)
        {
            if (distMatrix[distMatrix_Row][distMatrix_Col] != DIST_INF) //Any non infinite values are added to the graph
            {
                graphAddEdge(pGraph, distMatrix_Row, distMatrix_Col, distMatrix[distMatrix_Row][distMatrix_Col]);
            }
        }
    }
    if (graphgetv(pGraph) > 0)
    {
        status = true; //if graph is populated, function returns true
    }
    return status;
}

/////////////////////////////////////////////////////////////////
// graphDestructor - frees memory of a graph
//
// Params:
//pGraph - desired graph to free memory of
/////////////////////////////////////////////////////////////////
void graphDestructor(struct Graph_* pGraph)
{
    int i;
    int v = graphgetv(pGraph);
    for (i = 0; i < v; i++) //iteration through every linkedlist in the graph
    {
        //Every node is freed from each list array
        struct node* currentRecord;
        struct node* nextRecord = pGraph->linkedlist[i].head;
        while(nextRecord != NULL)
        {
            currentRecord = nextRecord;
            nextRecord = currentRecord->next;
            nodeDestructor(currentRecord);
        }
    }

    free(pGraph);
}
/////////////////////////////////////////////////////////////////
// graphDisplay - Displays the contents of a graph
//
// Params:
// pGraph - Graph to display
// placeNames to convert city indexes to their relevant city name.
/////////////////////////////////////////////////////////////////
void graphDisplay(struct Graph_* pGraph, char placeNames[100][MAX_CHARS])
{
    int d = 0;
    for(d = 0; d < NOOFVERTICES; d++) //d used to iterate through the placeNames array
    {
        //List containing all the adjacent nodes associated with that city is found
        struct List v = pGraph->linkedlist[d];
        node *n;
        if (pGraph->linkedlist[d].head != NULL) //parameterisable to the values in the graph which arent null
        {
            printf("[%d] %s -> ", d, placeNames[d]);
        }
        for (n = v.head; n != NULL; n = n->next)
        {
            //All nodes in the list are displayed
            printf("(%d %s, %d)", n->cityIndex, placeNames[n->cityIndex], n->weight);
        }
        if (pGraph->linkedlist[d].head != NULL)
        {
            printf("\n");
        }

    }
}


/////////////////////////////////////////////////////////////////
// graphConstructor - Builds the top level of the graph
//
// Params:
//noVertices - The size of the graph to make, the number of list arrays contained in it
//Returns the constructed graph
/////////////////////////////////////////////////////////////////

struct  Graph_* graphConstructor(int noVertices)
{
    //memory freed with malloc
    struct Graph_* graph = (struct Graph_*) malloc(sizeof(struct Graph_));
    graph->noVeretices = noVertices;
    //Lists are created for each vertex in the graph
    graph->linkedlist = (struct List*) malloc(noVertices * sizeof(struct List));
    int i;
    for (i = 0; i < noVertices; ++i)
        graph->linkedlist[i].head = NULL;

    return graph;
};

/////////////////////////////////////////////////////////////////
// graphAddEdge - Used in graphsetValues, adds an edge to the graph
//
// Params:
// graph - graph to add the edge to
//sourceCityIndex -
//destcityIndex   - - The desired values of the new edge
//weight          -
/////////////////////////////////////////////////////////////////

void graphAddEdge(struct Graph_* graph, int sourceCityIndex, int destcityIndex, int weight)
{
    struct node* nodeToAdd = nodeConstructor(destcityIndex, weight);
    nodeToAdd->next = graph->linkedlist[sourceCityIndex].head;
    graph->linkedlist[sourceCityIndex].head = nodeToAdd;
}
/////////////////////////////////////////////////////////////////
// getlistsize - Returns the size of a list in a graph
//
// Params:
// List - list to examine
// Returns the number of nodes in the list
/////////////////////////////////////////////////////////////////
int getlistSize(struct List* list)
{
    return list->size;
}

/////////////////////////////////////////////////////////////////
// checkListIsEmpty - does exactly what it says on the tin :)
//
// Params:
// List - list to examine
// Returns true if graph is not populated
/////////////////////////////////////////////////////////////////
bool checkListIsEmpty(struct List* list)
{
    if (getlistSize(list) == 0)
    {
        return true;
    }
    else if (getlistSize(list) != 0)
    {
        return false;
    }

}
/////////////////////////////////////////////////////////////////
// nodeConstructor - Builds nodes for the graph
//
// Params:
//cityIndex - the value of the city index for the given node
//weight - desired weight value for the specified node
// Returns the constructed node
/////////////////////////////////////////////////////////////////
struct node* nodeConstructor(int cityIndex, int weight)
{
    struct node* node;
    node = malloc( sizeof(struct node) ); //memory allocated
    node->cityIndex = cityIndex;
    node->weight = weight;
    node->next = NULL;
    node -> prev = NULL;
    return node;
};

/////////////////////////////////////////////////////////////////
// nodeDestructor - frees memory from a given node
/////////////////////////////////////////////////////////////////
void nodeDestructor(struct node* node)
{
    free(node);
}

/////////////////////////////////////////////////////////////////
// nodeDisplay - Outputs the weight and city index of a specified node
//
// Params:
//pNode - desired node to output contents
/////////////////////////////////////////////////////////////////
void nodeDisplay(struct node* pNode)
{
    int cityIndex = pNode ->cityIndex;
    int weight = pNode ->weight;
    printf("\n\n Checking Node /// Weight: %d /// City Index: %d \n\n", weight, cityIndex);
}

/////////////////////////////////////////////////////////////////
// heapNodeConstructor - creates a heap node (on the heap) and initialises it
//
// Params:
// vertexNum - graph to add the edge to
//distance   - The desired values of the new edge
//Returns the constructed heap node
/////////////////////////////////////////////////////////////////
struct heapNode* heapNodeConstructor(int vertexNum, int distance)
{
    struct heapNode* pHeapNode = (struct heapNode*) malloc(sizeof(struct heapNode));
    pHeapNode->vertexNum = vertexNum;
    pHeapNode->distance = distance;
    return pHeapNode;
}

/////////////////////////////////////////////////////////////////
// heapConstructor - creates a heap and initialises it
//
// Params:
// capacity - max number of nodes to be available to the heap
//Returns the constructed heap
/////////////////////////////////////////////////////////////////
struct heap* heapConstructor(int capacity)
{
    struct heap* Heap = (struct heap*) malloc(sizeof(struct heap));
    Heap->pPositions = (int *)malloc(capacity * sizeof(int));
    Heap->size = 0;
    Heap->capacity = capacity;
    Heap->array = (struct heapNode**) malloc(capacity * sizeof(struct heapNode*));
    return Heap;
}

/////////////////////////////////////////////////////////////////
// heapSwapNodes- needed for heapify function
//
// Params:
// pNode1, pNode2 - The 2 heapNodes to swap
/////////////////////////////////////////////////////////////////
void heapSwapNodes(struct heapNode** pNode1, struct heapNode** pNode2)
{
    struct heapNode* pHeapNew = *pNode1;
    *pNode1 = *pNode2;
    *pNode2 = pHeapNew;
}

/////////////////////////////////////////////////////////////////
// getHeapSize - getter function to return the current size of a heap
//
// Params:
// heap - The desired heap to check
//Returns its size
/////////////////////////////////////////////////////////////////

int getHeapSize(struct heap* Heap)
{
    return Heap->size;
}


/////////////////////////////////////////////////////////////////
// heapify - Creates the heap from its array elements
// see https://en.wikipedia.org/wiki/Heapsort
//
// Params:
// heap - The desired heap to check
/////////////////////////////////////////////////////////////////
int heapify(struct heap* Heap)
{
    int p = 0;

    while(1)
    {
        int c1 = 2*p+1;
        int c2 = 2*p+2;
        int c = -1;

        //Each node is checked as a parent node until its children have a value less than or equal to it
        if (c1 < Heap->size && Heap->array[p]->distance  > Heap->array[c1]->distance)
        {
            //left child node checked
            c = c1;
        }
        else if (c2 < Heap->size && Heap->array[p]->distance > Heap->array[c2]->distance)
        {
            // right child node checked
            c = c2;
        }
        else
        {
            break;
        }


        struct heapNode *parent = Heap->array[p];
        struct heapNode *child  = Heap->array[c];

        //Parent and child nodes placed in their correct location in the heap
        Heap->array[c] = parent;
        Heap->pPositions[parent->vertexNum] = c;

        Heap->array[p] = child;
        Heap->pPositions[child->vertexNum] = p;

        p = c;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////
// getMinimumNode - Extracts minimum node from heap
//
// Params:
// heap - The desired heap to check
// Returns the root node of desired heap
/////////////////////////////////////////////////////////////////
struct heapNode* getMinimumNode(struct heap* Heap)
{

    //Heap checked to not be empty
    if (getHeapSize(Heap) == 0)
    {
        printf("\n\nError: Heap is empty!");
    }

    //Root node retrieved from first position in heap array.
    struct Heap* root = Heap->array[0];

    //New root node updated and size adjusted
    Heap->array[0] = Heap->array[Heap->size - 1];
    --Heap->size;

    //Heapify needs to applied to new root node
    heapify(Heap);

    return root;

}
/////////////////////////////////////////////////////////////////
// decreaseDist - decreases the distance value for a given vertex
//
// Params:
// heap - The desired heap to update
// cityIndex - The required city to decrease the distance value of
// Returns the root node of desired heap
/////////////////////////////////////////////////////////////////
void decreaseDist(struct heap* Heap, int cityIndex, int ndistance)
{
    ///////////!!!!!!!POSSIBLE ERROR IN CODE??? DIJKSTRAS() OUTPUTTING SOME WRONG VALUES??!!!!!!!///////////


    //Index found and stored
    int i = Heap->pPositions[cityIndex];

    //Compares the new distance value to update with the value currently stored at the relative node.

    if(ndistance > Heap->array[i]->distance)
    {
        return;
        //Function ends if this value would not cause a decrease
    }
    else
    {
        //Value updated at index node.
        Heap->array[i]->distance = ndistance;
    }

    //The remainder of the tree is heapified.
    while (i && Heap->array[i]->distance < Heap->array[(i - 1) / 2]->distance)
    {

        Heap->pPositions[Heap->array[i]->vertexNum] = (i-1)/2;
        Heap->pPositions[Heap->array[(i-1)/2]->vertexNum] = i;
        heapSwapNodes(&Heap->array[i],  &Heap->array[(i - 1) / 2]);

        i = (i-1)/2;
    }
}

/////////////////////////////////////////////////////////////////
// checkVertex - Checks whether a vertex is contained in a heap
//
// Params:
// heap - The desired heap to update
// v - The vertex to search for
// Returns true if vertex is contained in heap, else false
/////////////////////////////////////////////////////////////////
bool checkVertex(struct heap *Heap, int v)
{
    //If the vertex number is included in the array, returns true
    if (Heap->pPositions[v] <= Heap->size)
    {
        printf("\nVertex found in heap\n");
        return true;
    }
    //If not, will return false
    else if (!(Heap->pPositions[v] <= Heap->size))
    {
        printf("\nWarning, Vertex not found in heap\n");
        return false;
    }
}

