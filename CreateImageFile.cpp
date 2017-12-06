//
// Created by lius214 on 10/16/2017.
//
#include "CreateImageFile.h"

using namespace std;

void selection_sort(vector<vector<int>> & sortVector, int n){
    int max;
    int temp;
    for (int i = 0; i < n-1; i++){
        max = i;
        for (int j = i+1; j < n; j++){
            if (sortVector.at(j).at(3) > sortVector.at(max).at(3))
                max = j;
        }
        for(int k = 0; k < 4; k++){
            temp = sortVector.at(max).at(k);
            sortVector.at(max).at(k) = sortVector.at(i).at(k);
            sortVector.at(i).at(k) = temp;
        }
    }
}

Image::Image(int x, int y){
    this->width = x;
    this->height = y;
}

Image::Image(char* fname, int x, int y){
    this->filename = fname;
    this->width = x;
    this->height = y;

}

void Image::setFilePath(char* fname){
    this->filename = fname;
}

void Image::popularity(){
    vector<vector<int>> frequency;
    bool found;

    for(int i = 0; i < height * width; i++){
        found = false;
        for(int j = 0; j < frequency.size(); j++){
            if(frequency.at(j).at(0) == RGBimage[i][0] && frequency.at(j).at(1) == RGBimage[i][1] && frequency.at(j).at(2) == RGBimage[i][2]) {
                frequency.at(j).at(3)++;
                found = true;
                break;
            }
        }
        if(!found){
            vector<int> color;
            color.push_back(RGBimage[i][0]); // R
            color.push_back(RGBimage[i][1]); // G
            color.push_back(RGBimage[i][2]); // B
            color.push_back(1); // Frequency
            frequency.push_back(color);
        }
    }

//    for(int i = 0; i < frequency.size(); i ++){
//        cout << frequency.at(i).at(0) << " " << frequency.at(i).at(1)<< " "  << frequency.at(i).at(2)<< " "  << frequency.at(i).at(3) << endl;
//    }

    // sorting
    selection_sort(frequency, frequency.size());

//    for(int i = 0; i < frequency.size(); i ++){
//        cout << frequency.at(i).at(0) << " " << frequency.at(i).at(1)<< " "  << frequency.at(i).at(2)<< " "  << frequency.at(i).at(3) << endl;
//    }

    for(int i = 0; i < 256; i++) {
        //record the index of the 256 most-frequent colors
        Popularity[i][0] = (unsigned char) frequency.at(i).at(2); //B
        Popularity[i][1] = (unsigned char) frequency.at(i).at(1); //G
        Popularity[i][2] = (unsigned char) frequency.at(i).at(0); //R
        Popularity[i][3] = (unsigned char) 1; //Alpha
    }

    double min;
    double dist;
    int pix;
    int padding = (4 - width%4) % 4; // number of zero paddings needed to get a multiple of 4 bytes in each row
    for (int i = height - 1; i >= 0; i--) {
        for(int k = 0; k < width; k ++){
            pix = 0;
            // start with the first index
            min = pow((double)(Popularity[0][0] - RGBimage[i*width + k][0]), 2.0) + pow(double(Popularity[0][1] - RGBimage[i*width + k][1]), 2.0) + pow((double)(Popularity[0][2] - RGBimage[i*width + k][2]), 2.0);
            for(int j = 0; j < 256; j++) {
                dist = pow((double)(Popularity[j][0] - RGBimage[i*width + k][0]), 2.0) + pow(double(Popularity[j][1] - RGBimage[i*width + k][1]), 2.0) + pow((double)(Popularity[j][2] - RGBimage[i*width + k][2]), 2.0);
                if(dist < min) {
                    min = dist;
                    pix = j; //assign the index number in 256 to corresponding color i in original color index
                }
            }
            // cout << pix << endl;
            indexPop.push_back((unsigned char) pix);
        }
        for(int j = 0; j < padding; j ++){
            indexPop.push_back((unsigned char) 0);
        }
    }
}

void Image::uniformPartitioning(){

    // color table setup
    // using the mid point of each range
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            for(int k = 0; k < 4; k ++){
                UniformPartition[32*i + 4*j + k][0] = ((unsigned char) (32 + 64 * k)); //B
                UniformPartition[32*i + 4*j + k][1] = ((unsigned char) (16 + 32 * j)); //G
                UniformPartition[32*i + 4*j + k][2] = ((unsigned char) (16 + 32 * i)); //R
                UniformPartition[32*i + 4*j + k][3] = ((unsigned char) 1); //Alpha
            }
        }
    }

    int padding = (4 - width%4) % 4; // number of zero paddings needed to get a multiple of 4 bytes in each row

    // calculating the pixel data
    unsigned int pix;
    for(int i = height - 1; i >= 0; i --){
        for(int k = 0; k < width; k ++){
            pix = 0;
            pix += (unsigned char) (RGBimage[i*width + k][0] / 32 * 32); // R
            pix += (unsigned char) (RGBimage[i*width + k][1] / 32 * 4); // G
            pix += (unsigned char) (RGBimage[i*width + k][2] / 64); // B
            // cout << pix << endl;
            indexUni.push_back((unsigned char) pix);
            }
        for(int j = 0; j < padding; j ++){
            indexUni.push_back((unsigned char) 0);
        }
    }
}

void Image::readRawRGBFile(char* fname){
    size_t result;
    FILE * infile;

    infile = fopen (fname , "rb");
    if (infile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek(infile , 0 , SEEK_END);
    lSize = ftell (infile);
    rewind (infile);

    RGBimage = new int*[lSize/3];
    for(int i = 0; i < lSize/3; i ++){
        RGBimage[i] = new int[3];
    }

    // allocate memory to contain the whole file:
    image = (unsigned char*) malloc (sizeof(unsigned char)*lSize);

    if (image == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the image
    result = fread (image,1,lSize,infile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    fclose(infile);
    for (int i = 0; i < lSize/3; i++) {
        for (int j = 0; j < 3; j++) {
            RGBimage[i][j] = (int) (*(image + 3*i + j));
        }
    }
}

void Image::printArray() {
    for(int i = 0; i < lSize/3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << RGBimage[i][j] << " ";
        }
        cout << endl;
    }
}

void Image::createIndexedFile(int option) {
    char b;
    char m;
    int fileSize;
    short zero;
    int offsetToPixelData;
    int headerSize;
    int imageWidth;
    int imageHeight;
    short one;
    short bitsPerPixel;
    int compressionType;
    int compression;
    int preferredXResolution;
    int preferredYResolution;
    int numColorMapEntries;
    int numSignificantColors;
    unsigned char z=0;

    /* creating the output file*/
    ofstream outFile(filename, ofstream::binary);

    /* headers for beginning of the .bmp file*/
    b = 'B';
    outFile.write(reinterpret_cast<char *>(&b), 1);
    m = 'M';
    outFile.write(reinterpret_cast<char *>(&m), 1);
    fileSize = height * width;
    outFile.write(reinterpret_cast<char *>(&fileSize), 4);
    zero = 0;
    outFile.write(reinterpret_cast<char *>(&zero), 2);
    outFile.write(reinterpret_cast<char *>(&zero), 2);
    offsetToPixelData = 1078;
    outFile.write(reinterpret_cast<char *>(&offsetToPixelData), 4);
    headerSize = 40;
    outFile.write(reinterpret_cast<char *>(&headerSize), 4);
    imageWidth = width;
    outFile.write(reinterpret_cast<char *>(&imageWidth), 4);
    imageHeight = height;
    outFile.write(reinterpret_cast<char *>(&imageHeight), 4);
    one = 1;
    outFile.write(reinterpret_cast<char *>(&one), 2);
    bitsPerPixel = 8;
    outFile.write(reinterpret_cast<char *>(&bitsPerPixel), 2);
    compressionType = 0;
    outFile.write(reinterpret_cast<char *>(&compressionType), 4);
    compression = 0;
    outFile.write(reinterpret_cast<char *>(&compression), 4);
    preferredXResolution = 0;
    outFile.write(reinterpret_cast<char *>(&preferredXResolution), 4);
    preferredYResolution = 0;
    outFile.write(reinterpret_cast<char *>(&preferredYResolution), 4);
    numColorMapEntries = 256;
    outFile.write(reinterpret_cast<char *>(&numColorMapEntries), 4);
    numSignificantColors = 256;
    outFile.write(reinterpret_cast<char *>(&numSignificantColors), 4);


    /* write the color table output to the file*/
    if(option == 1){
        for(int i = 0; i < 256; i ++){
            outFile.write(reinterpret_cast<char *>(&Popularity[i][0]), 1);
            outFile.write(reinterpret_cast<char *>(&Popularity[i][1]), 1);
            outFile.write(reinterpret_cast<char *>(&Popularity[i][2]), 1);
            outFile.write(reinterpret_cast<char *>(&Popularity[i][3]), 1);

//             cout << (int)Popularity[i][0] << " " << (int)Popularity[i][1] << " " << (int)Popularity[i][2] << " " << (int)Popularity[i][3] << endl;
        }
    }
    else{
        for(int i = 0; i < 256; i ++){
            outFile.write(reinterpret_cast<char *>(&UniformPartition[i][0]), 1);
            outFile.write(reinterpret_cast<char *>(&UniformPartition[i][1]), 1);
            outFile.write(reinterpret_cast<char *>(&UniformPartition[i][2]), 1);
            outFile.write(reinterpret_cast<char *>(&UniformPartition[i][3]), 1);

            // cout << (int)UniformPartition[i][0] << " " << (int)UniformPartition[i][1] << " " << (int)UniformPartition[i][2] << " " << (int)UniformPartition[i][3] << endl;
        }
    }

    /* write the color data*/
    if(option == 1){
        for(int i = 0; i < indexPop.size() ; i ++) {
            outFile.write(reinterpret_cast<char *>(&(indexPop.at(i))), 1);
            // cout << i << endl;
            // cout << (int) indexPop.at(i) << endl;
        }
    }
    else{
        for(int i = 0; i < indexUni.size() ; i ++){
            outFile.write(reinterpret_cast<char *>(&(indexUni.at(i))), 1);
            // cout << i << endl;
            // cout << (int) indexUni.at(i) << endl;
        }
    }

    outFile.close();
}