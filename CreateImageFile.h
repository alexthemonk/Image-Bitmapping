//
// Created by lius214 on 10/16/2017.
//

#ifndef CSC361LAB1_CREATEIMAGEFILE_H
#define CSC361LAB1_CREATEIMAGEFILE_H

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cmath>

using namespace std;

class Image{
private:
    char* filename;
    int height;
    int width;
    long lSize;
    int ** RGBimage;
    unsigned char * image;
    unsigned char Popularity[256][4];; // color table
    unsigned char UniformPartition[256][4]; // color table
    vector<unsigned char> indexPop; // output bmp file data
    vector<unsigned char> indexUni; // output bmp file data
public:
    Image(int x, int y);
    Image(char* fname, int x, int y);
    void setFilePath(char* fname);
    void createIndexedFile(int option);
    void readRawRGBFile(char* fname);
    void popularity();
    void uniformPartitioning();
    void printArray();
};

#endif //CSC361LAB1_CREATEIMAGEFILE_H
