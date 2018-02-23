//Shucheng (Alex) Liu
//10/28/2017

#include <iostream>
#include <fstream>

#include "CreateImageFile.h"

using namespace std;

int main(int argc, char * argv[]) {
    
    if(argc != 2){
        cout << "Error: number of input argument must be 1" << endl;
        exit(1);
    }
    
    int choice = 0;
    int width, height;

    cout << "Enter image width: ";
    cin >> width;
    cout << "Enter image height: ";
    cin >> height;

    Image pic(width, height);
    pic.readRawRGBFile(string(argv[1]));
    //pic.printArray();
    cout << "Choose an option" << endl << "1 for popularity, 2 for uniform partitioning" << endl;
    cin >> choice;
    cout << endl;
    if(choice == 1){
        cout << "producing image using Popularity Algorithm: outputPopularity.bmp" << endl;
        pic.setFilePath(string(argv[1]) + "Popularity.bmp");
        pic.popularity();
    }
    else{
        cout << "producing image using Uniform Partitioning Algorithm: outputUniform.bmp" << endl;
        pic.setFilePath(string(argv[1]) + "Uniform.bmp");
        pic.uniformPartitioning();
    }
    pic.createIndexedFile(choice);
    return 0;
}
