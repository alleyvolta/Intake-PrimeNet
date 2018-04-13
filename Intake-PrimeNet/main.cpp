// **************************************************************************
//  main.cpp
//  inputFormatter
//
//  Created by Alan Vitullo on 3/20/18.
//  Copyright © 2018 Alan Vitullo. All rights reserved.
// **************************************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "opencv2/opencv.hpp"

//
using namespace std;
using namespace cv;

// CELL_SIZE * BLOCK_SIZE must == GRID_SIZE /SCALING_FACTOR
//const int N = 16; //    N should be greater than 2, and certainly greater than 1.
const int GRID_SIZE = 256;
const int SCALING_FACTOR = 4;
const int CELL_SIZE = 8;
const int BLOCK_SIZE = 8;

enum INTENSITY:uchar {DARK=0, BRIGHT=255};

class grid; // forward: friend

// **************************************************************************
class block {
// **************************************************************************
    friend class grid;
public:
    block();
    block(int size, Point startingPoint);
    block(int size, Point startingPoint, INTENSITY intensity);
    ~block();
    //
    unsigned int getBlockID();
    int get_block_size();
    void set_block_size(int size);
    void setBlock_positions(Point startingPoint);
    vector<vector<Point> > getBlockPositions(block & B); //needs pointer imp
    void showBlockPositions();
    void setBlockIntensity(INTENSITY intensity);
    INTENSITY getBlockIntensity();
private:
    int block_size;
    static unsigned int ID;
    const unsigned int blockID;
    vector<vector<Point> > point_positions;
    INTENSITY block_intensity;
};
//  INITIALIZE IDENTITY BLOCK COUNTER
unsigned int block::ID=0;
//  RETURN BLOCK IDENTITY
unsigned int block::getBlockID(){
    return blockID;
};
block::block(): blockID(ID++){
    // STANDARD BLOCK W/ NO GIVEN INITIAL POSITION, SET TO (-) OUTSIDE THE ACTIVE GRID
    Point offGrid;
    offGrid.x = -GRID_SIZE;
    offGrid.y = -GRID_SIZE;
    setBlock_positions(offGrid);
    set_block_size(BLOCK_SIZE);
    setBlockIntensity(BRIGHT);
};
block::block(int size, Point startingPoint): blockID(ID++){
    // CUSTOM BLOCK W/ GIVEN INITIAL POSITION
    setBlock_positions(startingPoint);
    set_block_size(size);
    setBlockIntensity(BRIGHT);
};
block::block(int size, Point startingPoint, INTENSITY intensity): blockID(ID++){
    // CUSTOM BLOCK W/ GIVEN INITIAL POSITION
    setBlock_positions(startingPoint);
    set_block_size(size);
    setBlockIntensity(intensity);
};
block::~block(){
    //
};
void block::set_block_size(int size){
    block_size=size;
};
int block::get_block_size(){
    return block_size;
};
void block::setBlock_positions(Point startingPoint){
    vector<Point> row;
    int tempX, tempY;
    //
    for(int k=0; k<BLOCK_SIZE; k++){
        for (int l=0; l<BLOCK_SIZE; l++) {
            tempX=startingPoint.x +k;
            tempY=startingPoint.y +l;
            Point loc(tempX,tempY);
            row.push_back(loc);
        }
        point_positions.push_back(row);
        row.clear();
    }
};
vector<vector<Point> > block::getBlockPositions(block &B){
    return point_positions;
};
void block::showBlockPositions(){
    cout << endl;
    //
    int count =0;
    for(int k=0; k<BLOCK_SIZE; k++){
        for (int l=0; l<BLOCK_SIZE; l++) {
            cout << "X> " << point_positions[k][l].x <<"\t";
            cout << "Y> " << point_positions[k][l].y <<endl;
            cout << "Point> " << point_positions[k][l] << endl;
            count++;
            cout << "#" << count << endl;
        }
    }
};
void block::setBlockIntensity(INTENSITY intensity){
    block_intensity=intensity;
};
INTENSITY block::getBlockIntensity(){
    return block_intensity;
}

// **************************************************************************
class cell {
// **************************************************************************
    friend class grid;
public:
    cell();
    cell(Point startingPoint);
    cell(Point startingPoint, INTENSITY intensity);
    cell(int size, Point position);
    ~cell();
    unsigned int get_cellID();
    Point get_grid_location();
    int get_cell_size();
    void set_cell_size( int size);
    void set_grid_location(Point p);
    vector < vector<block> > get_cell_blocks();
    void form_cell_blocks(Point startingPoint, INTENSITY intensity);
    void form_cell_blocks(int block_size, Point startingPoint);
    void show_cell_();
private:
    int cell_size;
    int grid_index_ID;
    Point grid_location;
    static unsigned int ID;
    const unsigned int cellID;
    vector < vector<block> > cell_blocks;
};
//
unsigned int cell::ID=0;
//
unsigned int cell::get_cellID(){
    return ID;
};
cell::cell(): cellID(ID++){
    Point off_grid(-GRID_SIZE,-GRID_SIZE);
    set_grid_location(off_grid);
    set_cell_size(CELL_SIZE);
    form_cell_blocks(grid_location, BRIGHT);
};
cell::cell(Point startingPoint): cellID(ID++){
    set_grid_location(startingPoint);
    set_cell_size(CELL_SIZE);
    form_cell_blocks(startingPoint, BRIGHT);
};
cell::cell(Point startingPoint, INTENSITY intensity): cellID(ID++){
    set_grid_location(startingPoint);
    set_cell_size(CELL_SIZE);
    form_cell_blocks(startingPoint, intensity);
};
cell::cell(int size, Point startingPoint): cellID(ID++){
    set_grid_location(startingPoint);
    set_cell_size(cell_size);
    form_cell_blocks(grid_location, BRIGHT);
};
cell::~cell(){

};
void cell::set_cell_size(int size){
    cell_size=size;
}
int cell::get_cell_size(){
    return cell_size;
}
Point cell::get_grid_location(){
    return grid_location;
};
void cell::set_grid_location(Point p){
    grid_location=p;
}
vector < vector<block> > cell::get_cell_blocks(){
    return cell_blocks;
};
void cell::form_cell_blocks(Point startingPoint, INTENSITY intensity){
    vector<block> row;
    int tempX, tempY;
    //
    for(int k=0; k<cell_size; k++){
        for (int l=0; l<cell_size; l++) {
            tempX=startingPoint.x + (cell_size*k);
            tempY=startingPoint.y + (cell_size*l);
            Point loc(tempX,tempY);
            block bloc(BLOCK_SIZE, loc, intensity);
            row.push_back(bloc);
        }
        cell_blocks.push_back(row);
        row.clear();
    }
};
void cell::form_cell_blocks(int block_size, Point startingPoint){
    vector<block> row;
    int tempX, tempY;
    //
    for(int k=0; k<cell_size; k++){
        for (int l=0; l<cell_size; l++) {
            tempX=startingPoint.x + (cell_size*k);
            tempY=startingPoint.y + (cell_size*l);
            Point loc(tempX,tempY);
            block bloc(block_size, loc);
            row.push_back(bloc);
        }
        cell_blocks.push_back(row);
        row.clear();
    }
};

// **************************************************************************
class grid {
// **************************************************************************
public:
    grid();
    grid(int size);
    grid(vector<Point> cell_locations);
    grid(vector<Point> cell_locations, INTENSITY intensity);
    grid(int size, vector<Point> cell_locations);
    ~grid();
    int get_grid_size();
    void set_grid_size(int size);
    void place_cell(Point cell_location, INTENSITY intensity);
    void update_grid_image();
    Point get_grid_index(int index);
    Mat get_grid_image();
private:
    int grid_size;
    Mat grid_image;
    vector<Point> grid_index;
    vector<unsigned int> grid_index_ID;
    vector<cell>  grid_cells;
    void m_index_grid();
};
grid::grid(){
    Mat blank_image(GRID_SIZE, GRID_SIZE, CV_8UC1, Scalar(0));
    blank_image.copyTo(grid_image);
    set_grid_size(GRID_SIZE);
    m_index_grid();
};
grid::grid(vector<Point> cell_locations){
    Mat blank_image(GRID_SIZE+1, GRID_SIZE+1, CV_8UC1, Scalar(127));
    blank_image.copyTo(grid_image);
    set_grid_size(GRID_SIZE);
    m_index_grid();
    for (int i=0; i<cell_locations.size(); i++) {
        place_cell(cell_locations[i], BRIGHT);
    }
};
grid::grid(vector<Point> cell_locations, INTENSITY intensity){
    Mat blank_image(GRID_SIZE, GRID_SIZE, CV_8UC1, Scalar(0));
    blank_image.copyTo(grid_image);
    set_grid_size(GRID_SIZE);
    m_index_grid();
    for (int i=0; i<cell_locations.size(); i++) {
        place_cell(cell_locations[i], intensity);
    }
};
grid::grid(int size){
    Mat blank_image(size, size, CV_8UC1, Scalar(0));
    blank_image.copyTo(grid_image);
    set_grid_size(size);
    m_index_grid();
};
grid::grid(int size, vector<Point> cell_locations){
    Mat blank_image(size, size, CV_8UC1, Scalar(0));
    blank_image.copyTo(grid_image);
    set_grid_size(size);
    m_index_grid();
};
grid::~grid(){
    // DELETE VOLATILE MEMBERS
}
void grid::place_cell(Point cell_location, INTENSITY intensity){
    cell C(cell_location, intensity);
    for (int i=0; i<grid_index_ID.size(); i++) {
        if (cell_location==grid_index[i]) {
            C.grid_index_ID=grid_index_ID[i];
        }
    }
    grid_cells.push_back(C);
};
int grid::get_grid_size(){
    return grid_size;
}
void grid::set_grid_size(int size) {
    grid_size = size;
}
Point grid::get_grid_index(int index){
    for (int i=0; i<grid_index_ID.size(); i++) {
        if(grid_index_ID[i]==index){
            return grid_index[i];
        }
    }
    Point error(-GRID_SIZE,-GRID_SIZE);
    return error;
};
void grid::m_index_grid(){
    int ID=0;
    for (int i=0; i<SCALING_FACTOR; i++) {
        for (int j=0; j<SCALING_FACTOR; j++) {
            Point temp;

                temp.x = (j*(get_grid_size()/ SCALING_FACTOR)) ;
                temp.y = (i*(get_grid_size()/ SCALING_FACTOR)) ;

            grid_index.push_back(temp);
            grid_index_ID.push_back(ID);
            ID++;
        }
    }
};
void grid::update_grid_image(){
    Mat temp;
    vector<Point> p;
    vector<INTENSITY> b;
    //
    for (int i=0; i<grid_cells.size(); i++) {
        //
        for (int j=0; j<grid_cells[i].cell_blocks.size(); j++) {
            for (int k=0; k<grid_cells[i].cell_blocks[j].size(); k++) {
                //
                for (int l=0; l<grid_cells[i].cell_blocks[j][k].point_positions.size(); l++) {
                    for (int m=0; m<grid_cells[i].cell_blocks[j][k].point_positions[l].size(); m++) {
                        //
                        p.push_back(grid_cells[i].cell_blocks[j][k].point_positions[l][m]);
                        b.push_back(grid_cells[i].cell_blocks[j][k].block_intensity);
                    }
                }
            }
        }
    }
    for (int i=0; i<p.size(); i++) {
        //cout << "point> " << p[i] << endl;
        grid_image.at<uchar>(p[i]) = b[i];
    }

};
Mat grid::get_grid_image(){
    return grid_image;
};

#include <random>
#include <chrono>
#include <ctime>
#include <algorithm>

void comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    
    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) std::cout << " " << i;
        }
        std::cout << std::endl;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}

using namespace std::chrono;

// **************************************************************************
int main(int argc, const char * argv[]) {
// **************************************************************************
    //
    vector<string> input_files;
    string directory = "/Users/avitullo/Documents/PrimeNet/Labels/";
    string inputfile_log = "/Users/avitullo/Documents/PrimeNet/Labels/input_log.txt";
    ifstream inputStream;
    //
    inputStream.open(inputfile_log);
    if (!inputStream) {
        cout << endl << "-=ERROR: Input LOG file not found or unable to open.=-" << endl;
        return 9;
    }
    //
    while (!inputStream.eof()) {
        string temp;
        getline(inputStream, temp);
        input_files.push_back(temp);
    }
    inputStream.close();
    //
    for(int i=0; i<input_files.size(); i++) {
        //
        string training_file = input_files[i];
        string file_location = directory;
        file_location.append(training_file);
        char open_brackets = '(';
        string N_char;
        char closed_brackets = ')';
        string K_char;
        size_t obN = input_files[i].find(open_brackets);
        size_t cbN = input_files[i].find(closed_brackets);
        size_t obK = input_files[i].find(open_brackets, obN+1);
        size_t cbK = input_files[i].find(closed_brackets, obK-1);
        N_char = input_files[i].substr(obN+1, (cbN-obN)-1);
        K_char = input_files[i].substr(obK+1, (cbK-obK)-1);
        //int N = stoi(N_char);
        int K = stoi(K_char);
        vector < vector<int> > indicies;
    //
    inputStream.open(file_location);
    if (!inputStream) {
        cout << endl << "-=ERROR: Input TRAINING file not found or unable to open.=-" << endl;
        return 9;
    }
    else {
        int count =0;
        int i_num;
        vector<int> temp;
        while (inputStream >> i_num) {
            temp.push_back(i_num);
            count++;
            if (count==K) {
                count=0;
                indicies.push_back(temp);
                temp.clear();
            }
        }
    }
        inputStream.close();
        file_location = "";
        //
        string train_text_filename = "/Users/avitullo/Documents/PrimeNet/data/pncnnc18/train.txt";
        string val_text_filename = "/Users/avitullo/Documents/PrimeNet/data/pncnnc18/val.txt";
        ofstream outputstream_train;
        ofstream outputstream_val;
        outputstream_train.open(train_text_filename, std::ios_base::app);
        outputstream_val.open(val_text_filename, std::ios_base::app);
        if(!outputstream_train) {
            cout << endl << "Could not open training text file." << endl;
            return 9;
        }
        if(!outputstream_val) {
            cout << endl << "Could not open validation text file." << endl;
            return 9;
        }
        //
        int count = 0;
        grid plane1;
        vector<Mat> images;
        vector<Point> locations;
        vector<Point> empty_locations;
        vector<bool> location_state(16, false);
    
    for (int i=0; i<indicies.size(); i++) {
        for (int j=0; j<indicies[i].size(); j++) {
            Point temp;
            temp = plane1.get_grid_index(indicies[i][j]);
            location_state[indicies[i][j]] = true;
            locations.push_back(temp);
        }
        
        for (int k=0; k<location_state.size(); k++) {
            if (location_state[k]==false) {
                empty_locations.push_back(plane1.get_grid_index(k));
            }
        }
        
        grid plane2(locations, BRIGHT);
        grid plane3(empty_locations, BRIGHT);
        plane2.update_grid_image();
        plane3.update_grid_image();
        //
        vector<Mat> planes;
        planes.push_back(plane1.get_grid_image());
        planes.push_back(plane2.get_grid_image());
        planes.push_back(plane3.get_grid_image());
        Mat image(plane1.get_grid_image());
        merge(planes, image);
        images.push_back(image);
        fill(location_state.begin(), location_state.end(), false);
        locations.clear();
        empty_locations.clear();
        //
        string train_image_location = "/Users/avitullo/Documents/PrimeNet/Images/Train/";
        string val_image_location = "/Users/avitullo/Documents/PrimeNet/Images/Validation/";
        string image_filename = "img_";
        image_filename.append(to_string(count));
        image_filename.append("-");
        image_filename.append(N_char);
        image_filename.append("nCk");
        image_filename.append(K_char);
        image_filename.append(".jpg");
        
        count++;
        //
        if (K==3 || K==5 || K==7 || K==13) {
            outputstream_train << image_filename << " 0" << endl;
            train_image_location.append(image_filename);
            //imwrite(train_image_location, images[i]);
        }
        else if(K==1 || K==6 || K==10 || K==12 || K==14 || K==15 || K==16) {
            outputstream_train << image_filename << " 1" << endl;
            train_image_location.append(image_filename);
           imwrite(train_image_location, images[i]);
        }
        if (K==2 || K==11) {
            outputstream_val << image_filename << " 0" << endl;
            val_image_location.append(image_filename);
            imwrite(val_image_location, images[i]);
        }
        else if(K==8 || K==4) {
            outputstream_val << image_filename << " 1" << endl;
            val_image_location.append(image_filename);
            imwrite(val_image_location, images[i]);
        }
        
    }
    //
        outputstream_train.close();
        outputstream_val.close();
        images.clear();
    }
    
    //
    return 0;
}

// FWO -> April 7,2018 12:05PM PST
// ALV
    
