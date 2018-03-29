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

//
const int SCALING_FACTOR = 4;
const int GRID_SIZE = 256;
const int CELL_SIZE = 8;
const int BLOCK_SIZE = 4;

enum INTENSITY:uchar {DARK=0, BRIGHT=255};

class grid; // forward: friend

// **************************************************************************
class block {
// **************************************************************************
    friend class grid;
public:
    block();
    block(int size, Point startingPoint);
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
    cell(int size, Point position);
    ~cell();
    unsigned int get_cellID();
    Point get_grid_location();
    int get_cell_size();
    void set_cell_size( int size);
    void set_grid_location(Point p);
    vector < vector<block> > get_cell_blocks();
    void form_cell_blocks(Point startingPoint);
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
    form_cell_blocks(grid_location);
};
cell::cell(Point startingPoint): cellID(ID++){
    set_grid_location(startingPoint);
    set_cell_size(CELL_SIZE);
    form_cell_blocks(startingPoint);
};
cell::cell(int size, Point startingPoint): cellID(ID++){
    set_grid_location(startingPoint);
    set_cell_size(cell_size);
    form_cell_blocks(grid_location);
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
void cell::form_cell_blocks(Point startingPoint){
    vector<block> row;
    int tempX, tempY;
    //
    for(int k=0; k<cell_size; k++){
        for (int l=0; l<cell_size; l++) {
            tempX=startingPoint.x + (cell_size*k);
            tempY=startingPoint.y + (cell_size*l);
            Point loc(tempX,tempY);
            block bloc(BLOCK_SIZE, loc);
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
    grid(int size, vector<Point> cell_locations);
    ~grid();
    int get_grid_size();
    void set_grid_size(int size);
    void place_cell(Point cell_location);
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
    Mat blank_image(GRID_SIZE, GRID_SIZE, CV_8UC1, Scalar(0));
    blank_image.copyTo(grid_image);
    set_grid_size(GRID_SIZE);
    m_index_grid();
    for (int i=0; i<cell_locations.size(); i++) {
        place_cell(cell_locations[i]);
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
void grid::place_cell(Point cell_location){
    cell C(cell_location);
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
            if(j!=0)
                temp.x = (j*(get_grid_size()/ SCALING_FACTOR)) -1;
            else
                temp.x = 0;
            if(i!=0)
                temp.y = (i*(get_grid_size()/ SCALING_FACTOR)) -1;
            else
                temp.y = 0;
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
// **************************************************************************
int main(int argc, const char * argv[]) {
// **************************************************************************
    //
    ofstream outputStream;
    grid plane1;
    vector<int> indicies;
    vector<Point> locations;
    //
    outputStream.open("/Users/avitullo/Documents/imageLabeling-test.txt", ios::out | ios::app);
    if (!outputStream)   // Test for error.
    {
        std::cerr << "Error opening output file:\n";
        exit(1);
    }
    //
    for (int i=0; i<(16); i++) {
        if(i%2==0){
            indicies.push_back(i);
        }
    }
    for (int i=0; i<indicies.size(); i++) {
        locations.push_back(plane1.get_grid_index(indicies[i]));
    }
    grid plane2(locations);
    plane2.update_grid_image();
    //
    imwrite("/Users/avitullo/Documents/imageLabeling-test3.png", plane2.get_grid_image());
    //
    outputStream.close();
    //
    return 0;
}



    
