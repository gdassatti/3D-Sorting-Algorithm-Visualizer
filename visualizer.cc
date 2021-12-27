//******************************************************************//
//                                                                  //
//  File name: visualizer.cc                                        //
//  Created by: Gavin Dassatti                                      //
//  Last Modified: 12/10/2021                                       //
//                                                                  //
//  This program, upon startup generates an array of random         //
//  elements in range [1 ... n]. Displays the array as 'stacks'     //  
//  of cubes in a 3D world where each of their heights along        //
//  the y-axis is directly correlated to the magnitude of the       //
//  element in the array at the given index. The user is able       //
//  around freely in the world and position their viewpoint         //  
//  in any orientation they choose. From here, the user can         //
//  display a drop down menu with a right click. This menu's        //  
//  options are related to different sorting algorithms. The        //
//  can choose whichever sort they would like to visualize and      //
//  watch the sorting algorithm manipulate the stacks of cubes      //
//  until they are all sorted by height.                            //
//                                                                  //
//******************************************************************//
#include <Angel.h>
#include <vector>
#include <random>
#include "cube.h"
#include "object.h"
#include "light.h"
#include <unistd.h>
#include <chrono>
using namespace std;

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

int size_of_array;  // size of the array we will be initializing and sorting
int grid;           // size of the grid -- based on size of array
int element;       


vector<int> initialArray;   // Array being modified in the sorting algorithms
vector<int> original;   // Keeps track of original array before sort
vector<cube *> cubes; // Holds the data for all of our cubes
vec4 *points;         // Holds the points for all of our cubes


// Shader variables
GLint model_view_loc;
GLint camera_view;
GLint projection;


// Functions for generating random
// numbers of different sets
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> color(0.15,0.15);
std::uniform_real_distribution<double> slcolor(0.4,1.0);
std::uniform_int_distribution<int> arraysize(10, 15);
std::uniform_int_distribution<int> arrayelement(1, 30);


// Perspective variables //
GLfloat fovy = 45.0;
GLfloat aspect = 1.0;
GLfloat zNear = 0.1;
GLfloat zFar = 300.0;
GLfloat radius = 6.5;


// Variables allowing the user
// to move around the world
GLfloat move_x = 0;
GLfloat move_y = 0;
GLfloat move_z = 0;
GLfloat view_angle = 0;
GLfloat rotate = 0;


// Window size
GLint win_h=0;
GLint win_w=0;

// Delay in between swaps
int TIME_DELAY = 50000;

// Variables distinguishing which sort to call
bool bubble_sort = false;
bool quicksort = false;
bool mergesort = false;
bool heapsort = false;
bool shellsort = false;
bool insertionsort = false;
bool selectionsort = false;


////////////////// RANDOMIZE ARRAY //////////////////
void randomizeArray(vector<int> &arr, int n){
    for(int i = 0; i < n; i++){
        arr[i] = arrayelement(mt);
        original[i] = arr[i];
    }
    glutPostRedisplay();
}
/////////////////////////////////////////////////////

///////////// RESET TO ORIGINAL ARRAY ///////////////
void resetfunc(vector<int>& arr, int n){
    for(int i = 0; i < n; i++){
        arr[i] = original[i];
    }
    glutPostRedisplay();
}
/////////////////////////////////////////////////////


//-----------------------------------------------------------------
// Setting drop down menu options
extern "C" void myMenu(int value){
    switch (value){
        case 1:
            bubble_sort = true;
            break;
        case 2:
            quicksort = true;
            break;
        case 3:
            mergesort = true;
            break;
        case 4:
            heapsort = true;
            break;
        case 5:
            shellsort = true;
            break;
        case 6:
            insertionsort = true;
            break;
        case 7:
            selectionsort = true;
            break;
        case 8:
            randomizeArray(initialArray, size_of_array);
            cout << "Array has been randomized" << endl;
            break;
        case 9:
            resetfunc(initialArray, size_of_array);
            cout << "Array has been reset to original" << endl;
            break;
        case 10:
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }

    glutPostRedisplay();

}
//-----------------------------------------------------------------


//----------------------------------------------
// Setting up the menu
void setupMenu(){
  glutCreateMenu(myMenu);
  glutAddMenuEntry("Bubble Sort", 1);
  glutAddMenuEntry("Quicksort", 2);
  glutAddMenuEntry("Mergesort", 3);
  glutAddMenuEntry("Heapsort", 4);
  glutAddMenuEntry("Shellsort", 5);
  glutAddMenuEntry("Insertion sort", 6);
  glutAddMenuEntry("Selection sort", 7);
  glutAddMenuEntry("Randomize Array", 8);
  glutAddMenuEntry("Reset Array", 9);
  glutAddMenuEntry("Quit", 10);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//-----------------------------------------------



//------------------------------------------------------------------------------
// Keyboard callback function
extern "C" void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 033:
        case 'q': case 'Q': // Exit
            exit(EXIT_SUCCESS);
            break;
        case '+':   // Increase the speed of algorithm
            TIME_DELAY -= 1000;
            break;
        case '-':   // Decrease the speed of algorithm
            TIME_DELAY += 1000;
            break;
        case 'a':   // rotate camera counter-clock-wise at current position
            view_angle -= 0.85;
            break;
        case 'd':   // rotate camera clock-wise at current position
            view_angle += 0.85;
            break;
        case 's':   // Moving backward along z-axis
            move_y -= 0.2;
            break;
        case 'w':   // Moving forward along z-axis
            move_y += 0.2;
            break;
        case 'z':   // rotating counter-clockwise around the array
            rotate -= 0.85;
            break;
        case 'x':  // rotating counter-clockwise around the array
            rotate += 0.85;
            break;
        case ' ': // Setting user input variable back to default
            TIME_DELAY = 500000;
            view_angle = 0;
            move_x = 0;
            move_y = 0;
            move_z = 0;
            rotate = 0;
            break;
    }

    glutPostRedisplay();

}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Special key callback function
extern "C" void special(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_UP:   // Move forwards
        move_z += 0.2;
        break;
    case GLUT_KEY_DOWN: // Move backwards
        move_z -= 0.2;
        break;
    case GLUT_KEY_LEFT: // Move to the left
        move_x += 0.2;
        break;
    case GLUT_KEY_RIGHT:// Move to the right
        move_x -= 0.2;
        break;
    }
    glutPostRedisplay();
}
//------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------
extern "C" void display(){

    // rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setting the camera orientation and allowing
    // movement from user input
    point4 eye(0.0 + move_x, 12 + move_y, -40.0 + move_z, 1.0);
    point4 at(0.0 + move_x, 12 + move_y, 200.0 + move_z, 1.0);
    vec4 up(0.0, 1.0, 0.0, 0.0);

    // Setting the camera view matrix, can rotate about the origin
    // or the fixed point the camera is currently at
    mat4 cv =  RotateY(view_angle)*LookAt(eye, at, up)*RotateY(rotate);
    glUniformMatrix4fv(camera_view, 1, GL_TRUE, cv);

    // Setting our perspective matrix
    mat4 p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(projection, 1, GL_TRUE, p);

    int length = ((size_of_array+2)/2);
    int count = 0;

    // Loop through the initial amout of grid cubes
    // place them in correct position and draw them
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < size_of_array + 2; j++){
            mat4 mv = Translate(j - length, -2.0, i + 0.0);
            cubes[count] -> x = j - length;
            cubes[count] -> y = -2.0;
            cubes[count] -> z = i - 0.0;
            cubes[count] -> set_mv(mv);
            cubes[count] -> draw();
            count++;
        }
    }

    // Loop through the rest of the cubes in the array which are the actual
    // array elements. Placing them on the grid and scaling along the y-axis
    // based on the value in the array's indices
    for(int i = 0; i < size_of_array; i++){
        mat4 mv = Translate(i - (size_of_array/2), -2.0 + initialArray[i] / 2.0, 1.0)*Scale(1, initialArray[i], 1);
        cubes[count] -> x = i - (size_of_array/2);
        cubes[count] -> y = -1.0;
        cubes[count] -> z = 1.0;
        cubes[count] -> set_mv(mv);
        cubes[count] -> draw();
        count++;
    }

    glutSwapBuffers();

}
//-------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------
// color_compare() function
// Quickly highlights the two stacks that are being
// compared and swapped, updates the screen, and
// switches the colors when they swap;
void color_compare(int i, int j){
    vec3 color_i = cubes[grid + i] -> getColor();
    vec3 color_j = cubes[grid + j] -> getColor();
    cubes[grid + i] -> color(0.8, 0.0, 0.0);
    cubes[grid + j] -> color(0.8, 0.0, 0.0);
    display();
    usleep(TIME_DELAY);
    cubes[grid + i] -> color(color_j);
    cubes[grid + j] -> color(color_i);
}
//--------------------------------------------------------



///////////////////////////////////////////// BUBBLE SORT /////////////////////////////////////////////
//  I used code from https://www.geeksforgeeks.org/bubble-sort/ to implement Swap() and bubblesort() //
///////////////////////////////////////////////////////////////////////////////////////////////////////

// Swaps two elements in the initial array
// and calls display() to update the screen
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
    display();
    usleep(TIME_DELAY);
}

// Simple bubble sort implementation
void bubblesort(vector<int> &arr){
    for (size_t i = 0; i < arr.size() - 1; i++){     
        for (size_t j = 0; j < arr.size()-i-1; j++){
            if (arr[j] > arr[j+1]){
                color_compare(j, j+1);
                swap(&arr[j], &arr[j+1]);
                
            }
        }
    }
}
///////////////////////////////////////////// BUBBLE SORT /////////////////////////////////////////////


////////////////////////////////////////////////// QUICKSORT //////////////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/quick-sort/ to implement quickSort() and partition()       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
array, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int partition(vector<int> &arr, int low, int high) 
{ 
    int pivot = arr[high]; // pivot
    color_compare(high, high); 
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far
  
    for (int j = low; j <= high - 1; j++) 
    { 
        // If current element is smaller than the pivot 
        if (arr[j] < pivot) 
        { 
            i++; // increment index of smaller element
            color_compare(i, j); 
            swap(&arr[i], &arr[j]); 
        } 
    }
    color_compare(i+1, high); 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
void quickSort(vector<int> &arr, int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
        at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
}
////////////////////////////////////////////////// QUICKSORT //////////////////////////////////////////////////



////////////////////////////////////////////////// MERGESORT //////////////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/merge-sort/ to implement merge() and mergeSort()           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Merges two subarrays of array[].
// First subarray is arr[begin..mid]
// Second subarray is arr[mid+1..end]
void merge(vector<int> &array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
  
    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
         *rightArray = new int[subArrayTwo];
  
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
  
    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array
  
    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            color_compare(indexOfSubArrayOne, indexOfSubArrayTwo*2);
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            display();
            usleep(TIME_DELAY);
        }
        else {
            color_compare(indexOfMergedArray, indexOfSubArrayTwo*2);
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            display();
            usleep(TIME_DELAY);
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        color_compare(indexOfMergedArray, indexOfSubArrayOne);
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
        display();
        usleep(TIME_DELAY);
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        color_compare(indexOfMergedArray, indexOfSubArrayTwo*2);
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
        display();
        usleep(TIME_DELAY);
    }
}
  
// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(vector<int> &array, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively
  
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}
////////////////////////////////////////////////// MERGESORT //////////////////////////////////////////////////



////////////////////////////////////////////////// HEAPSORT //////////////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/heap-sort/ to implement heapify() and heapSort()          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(vector<int> &arr, int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;
 
    // If largest is not root
    if (largest != i) {
        color_compare(i, largest);
        swap(&arr[i], &arr[largest]);
 
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}
 
// main function to do heap sort
void heapSort(vector<int> &arr, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
 
    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        color_compare(0, i);
        swap(&arr[0], &arr[i]);
 
        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}
////////////////////////////////////////////////// HEAPSORT //////////////////////////////////////////////////



////////////////////////////////////////////////// SHELLSORT //////////////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/shellsort/ to implement shellSort()                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* function to sort arr using shellSort */
void shellSort(vector<int> &arr, int n)
{
    // Start with a big gap, then reduce the gap
    for (int gap = n/2; gap > 0; gap /= 2)
    {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < n; i += 1)
        {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            int temp = arr[i];
 
            // shift earlier gap-sorted elements up until the correct
            // location for a[i] is found
            int j;           
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap){
                color_compare(j, j-gap);
                arr[j] = arr[j - gap];
                display();
                usleep(TIME_DELAY);
            }
            //  put temp (the original a[i]) in its correct location
            color_compare(j, i);
            arr[j] = temp;
            display();
            usleep(TIME_DELAY);
        }
    }
}
////////////////////////////////////////////////// SHELLSORT //////////////////////////////////////////////////



//////////////////////////////////////// INSERTION SORT /////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/insertion-sort/ to implement insertionSort() //
/////////////////////////////////////////////////////////////////////////////////////////////////
/* Function to sort an array using insertion sort*/
void insertionSort(vector<int> &arr, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        color_compare(i, i);
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            color_compare(j + 1, j);
            arr[j + 1] = arr[j];  
            j = j - 1;
            display();
            usleep(TIME_DELAY);
        }
        color_compare(j+1, i);
        arr[j + 1] = key;
        display();
        usleep(TIME_DELAY);
    }
}
//////////////////////////////////////// INSERTION SORT /////////////////////////////////////////



///////////////////////////////////////// SELECTION SORT /////////////////////////////////////////
// I used code from https://www.geeksforgeeks.org/selection-sort/ to implement selectionSort()  //
//////////////////////////////////////////////////////////////////////////////////////////////////
void selectionSort(vector<int> &arr, int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
        if (arr[j] < arr[min_idx])
            min_idx = j;
 
        // Swap the found minimum element with the first element
        color_compare(min_idx, i);
        swap(&arr[min_idx], &arr[i]);
    }
}
///////////////////////////////////////// SELECTION SORT /////////////////////////////////////////




/********************************************************************************************************************************************/
// Idle() callback Function
// Continously loops and calls a certain sorting algorithm on the array
// if the user clicks the corresponding option from the drop down menu
extern "C" void idle(){
    if(bubble_sort){
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        bubblesort(initialArray);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        bubble_sort = false;
        cout << "Bubble sort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(quicksort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        quickSort(initialArray, 0, len - 1);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        quicksort = false;
        cout << "Quicksort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(mergesort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        mergeSort(initialArray, 0, len - 1);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        mergesort = false;
        cout << "Mergesort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(heapsort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        heapSort(initialArray, len);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        heapsort = false;
        cout << "Heapsort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(shellsort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        shellSort(initialArray, len);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        shellsort = false;
        cout << "Shellsort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(insertionsort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        insertionSort(initialArray, len);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        insertionsort = false;
        cout << "Insertion sort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }else if(selectionsort){
        int len = initialArray.size();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        selectionSort(initialArray, len);
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        selectionsort = false;
        cout << "Selection sort complete - Array was sorted in: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s" << endl;
    }
}
/********************************************************************************************************************************************/



//----------------------------------------------------------------------------
// Reshape function to preserve our world
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  win_h = height;
  win_w = width;
  aspect = GLfloat(width)/height;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// myInit() generates an InitialArray upon startup of the program
// and inserts random integers into it to be displayed on the screen
void myInit(){

    int element;
    cout << "Generating random array..." << endl;
    //size_of_array = arraysize(mt);
    size_of_array = 45;
    grid = (size_of_array + 2) * 3;

    for(int i = 0; i < size_of_array; i++){
        element = arrayelement(mt);
        initialArray.push_back(element);
        original.push_back(element);
    }

}
//----------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------
void init(){

    GLint colorLoc;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Locations for positioning our camera
    model_view_loc = glGetUniformLocation(program, "model_view");
    camera_view = glGetUniformLocation(program, "camera_view");
    projection = glGetUniformLocation(program, "projection");

    // Now send the data to the GPU
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

    colorLoc = glGetUniformLocation(program, "vcolor"); 
    if (colorLoc==-1) {
        std::cerr << "Unable to find colorLoc parameter" << std::endl;
    }

    // Initializing all of the cubes I will be drawing to the screen
    // and inserting them into the cubes vector
    points = new vec4[cube::NumPoints*(size_of_array + grid)];
    for(int i = 0; i < size_of_array + grid; i++){
        cube *myCube = new cube(points, model_view_loc, 0, colorLoc);
        if(i < grid){
            myCube->color(color(mt), color(mt), color(mt));
        }else{
            myCube->color(slcolor(mt), slcolor(mt), slcolor(mt));
        }
        cubes.push_back(myCube);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(135.0/255.0, 206.0/255.0, 250.0/255.0, 1.0);   // Setting a sky blue background
    glBufferData(GL_ARRAY_BUFFER, (size_of_array+grid)*cube::NumPoints*sizeof(vec4), points, GL_STATIC_DRAW);  // Sending all of data to the GPU

}
//---------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------
// main function
int main(int argc, char **argv){

    myInit();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1500, 1000);
    glutCreateWindow("Sorting Algorithm Visualizer");

    glewInit();

    init();
    setupMenu();
    // GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutSpecialFunc(special);

    glutMainLoop();
    return(EXIT_SUCCESS);
}
//------------------------------------------------------------
