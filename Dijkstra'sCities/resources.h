//                   -RESOURCES.H



///////////////////////DEFINITIONS///////////////////////
#define MAX_CHARS 50 //Max characters per row in text file limited to 50
#define DIST_INF 99999 //"Infinite" value used to flag null paths, over twice the circumference of the earth
#define ROWS 100 //Number of rows capable of being read limited to 100 from text file
#define NOOFVERTICES 100 //Max number of vertices limited to 100 - for use in the adjacency matrix

//Boolean type definition
typedef enum { false, true } bool;

/////////////////////DATA_STRUCTURES/////////////////////

typedef struct List
{
    int size;
    struct node* head;
    struct node* tail;
} List;


typedef struct Graph_
{
    int noVeretices;
    struct List* linkedlist;
} graph;


typedef struct node
{
    int cityIndex;
    struct node* next;
    int weight;
    struct node* prev;

} node;


struct heapNode
{
    int  vertexNum;
    int distance;
};


struct heap
{
    int size;
    int *pPositions;
    int capacity;
    struct heapNode **array;
};

////////////////////GRAPH FUNCTIONS/////////////////////
int graphgetv(struct Graph_* graph1);
bool graphSetValues(struct Graph_ * pGraph, int distMatrix[100][100]);
void graphDestructor(struct Graph_* pGraph);
void graphDisplay(struct Graph_* pGraph, char placeNames[100][MAX_CHARS]);
struct  Graph_* graphConstructor(int noVertices);
void graphAddEdge(struct Graph_* graph, int sourceCityIndex, int destcityIndex, int weight);
int getlistSize(struct List* list);
bool checkListIsEmpty(struct List* list);
struct node* nodeConstructor(int cityIndex, int weight);
void nodeDestructor(struct node* node);
void nodeDisplay(struct node* pNode);

/////////////////////HEAP FUNCTIONS/////////////////////
struct heapNode* heapNodeConstructor(int v, int dist);
struct heap* heapConstructor(int capacity);
void heapSwapNodes(struct heapNode** pNode1, struct heapNode** pNode2);
int getHeapSize(struct heap* Heap);
int heapify(struct heap* Heap);
struct heapNode* getMinimumNode(struct heap* Heap);
void decreaseDist(struct heap* Heap, int cityIndex, int ndistance);
bool checkVertex(struct heap *Heap, int v);
