#include <iostream>
#include <cmath>
#include <fstream>
#include <cassert>
#include <string>
#include <pthread.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <cerrno>
#include<conio.h>
#include<stdio.h>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <unordered_set>
#include <time.h>

int weight(int* vec, int len, std::unordered_set <int> exclude);
int** AllocateMatrix(int noRows,int noCols);
void DeallocateMatrix(int noRows,int** pMatrix);
void sum_rows_and_mod(int* row1, int* row2, int* result, int rowlength);
void multply_rows_and_mod(int* row1, int mult, int* result, int rowlength);

void update_weight_dist(int* weight_dist, int len, int* row, std::unordered_set<int> exclude);

int main(int argc, char* argv[])
{
    int cand_count;

    // read the number of codes to test and allocate memory
    std::ifstream readCandidates;
    readCandidates.open("C:/Users/user/Desktop/Dissertation/LinearCodes/Candidates_passed.txt");
    readCandidates >> cand_count;
    std::string candidates[cand_count];

    //read the candidate numbers into memory
    std::cout<<cand_count<<std::endl;
    for(int i=0; i<cand_count;i++){
        readCandidates >> candidates[i];

    }

    std::ifstream readFile;
    int min_w = 1000;
    int height;
    int width;
    std::string readfilename;
    int id = 10000001;

    int** A;
    int* rowsum1;
    int* rowsum2;
    int* rowsum3;
    int* rowsum4;
    int* rowsum5;
    int* rowsum6;
    int* rowsum7;
    int* row1;
    int* row2;
    int* row3;
    int* row4;
    int* row5;
    int* row6;
    int* row7;

    std::unordered_set <int> exclude {};
    std::vector <int> puncturelist {};
    int nnn = exclude.size();
    std::cout << "Size of the vector is :" << nnn <<std::endl;

    std::string n;


    //for(int cand_index=0;cand_index<cand_count+1;cand_index++){

    //loop over candidates
    for(int fileindex=0;fileindex < cand_count; fileindex++){

    exclude.clear();
    puncturelist.clear();

    n = candidates[fileindex];
    std::string first_two = n.substr(0, 2);

    //std::cout<<"C:/Users/user/Desktop/Dissertation/LinearCodes/"+n;
    readfilename = "C:/Users/user/Desktop/Dissertation/LinearCodes/"+n;
    readFile.open(readfilename);
    assert(readFile.is_open());
    std::cout<<readfilename<<std::endl;


    readFile >> height;
    readFile >> width;


    A = AllocateMatrix(height,width);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            readFile >> A[i][j];
            //std::cout<<A[i][j]<<",";
        }
        //std::mcout<<std::endl;
    }
    readFile.close();
    min_w = std::stoi(first_two);

    int* weight_dist = new int[width];
    int* weight_dist2 = new int[width];

    int punctcap = 85;
    int disthistory[punctcap];


    for(int puncturing = 0; puncturing < punctcap; puncturing ++){
        for(int i = 0;i<width;i++)
        {
            weight_dist[i] = 0;
        }
        for(int i = 0;i<width;i++)
        {
            weight_dist2[i] = 0;
        }

        row1 = A[0];
        row2 = A[1];
        row3 = A[2];
        row4 = A[3];
        if(height > 4){
        row5 = A[4];
        }
        if(height > 5){
        row6 = A[5];
        }
        if(height > 6){
        row7 = A[6];
        }


        rowsum1 = new int[width];
        rowsum2 = new int[width];
        rowsum3 = new int[width];
        rowsum4 = new int[width];
        rowsum5 = new int[width];
        rowsum6 = new int[width];
        rowsum7 = new int[width];

        int counter = 0;

        int new_min_distance = 100000;
        int temp = 1000000;

        int minwords = 0;

        //find minimum weight - don't really need to
//        for (int i=0;i<11;i++){
//        for (int j=0;j<11;j++){
//        for (int k=0;k<11;k++){
//        for (int l=0;l<11;l++){
//        for (int m=0;m<11;m++){
//        //for (int n=0;n<11;n++){
//        //for (int o=0;o<11;o++){
//
//                //std::cout<<i<<","<<j<<","<<k<<","<<l<<","<<m<<","<<n<<","<<o<<std::endl;
//                //scalar multiples
//                multply_rows_and_mod(row1,i,rowsum1,width);
//                multply_rows_and_mod(row2,j,rowsum2,width);
//                multply_rows_and_mod(row3,k,rowsum3,width);
//                multply_rows_and_mod(row4,l,rowsum4,width);
//                multply_rows_and_mod(row5,m,rowsum5,width);
//                //multply_rows_and_mod(row6,n,rowsum6,width);
//                //multply_rows_and_mod(row7,o,rowsum7,width);
//
//                //sum rows
//                sum_rows_and_mod(rowsum1,rowsum2,rowsum1,width);
//                sum_rows_and_mod(rowsum1,rowsum3,rowsum1,width);
//                sum_rows_and_mod(rowsum1,rowsum4,rowsum1,width);
//                sum_rows_and_mod(rowsum1,rowsum5,rowsum1,width);
//                //sum_rows_and_mod(rowsum1,rowsum6,rowsum1,width);
//                //sum_rows_and_mod(rowsum1,rowsum7,rowsum1,width);
//                temp = weight(rowsum1,width,exclude);
//                if (temp < new_min_distance && temp != 0)
//                {
//                    new_min_distance = temp;
//                }
//            }
//        }}}}
//
//        std::cout<<std::endl;
//        std::cout<<"Distance found:"<<new_min_distance<<std::endl;
//        min_w = new_min_distance;

        int M=1;
        int N=1;
        int O=1;
        if(height>4){M=11;}
        if(height>5){N=11;}
        if(height>6){O=11;}

        bool timekeep = 1;

        //find weight distribution of min weight codewords
        for (int i=0;i<11;i++){
        for (int j=0;j<11;j++){
        for (int k=0;k<11;k++){
        for (int l=0;l<11;l++){
        if(timekeep){
            auto start = std::chrono::steady_clock::now();

        for (int m=0;m<M;m++){
        for (int n=0;n<N;n++){
        for (int o=0;o<O;o++){

                //std::cout<<i<<","<<j<<","<<k<<","<<l<<","<<m<<","<<n<<","<<o<<std::endl;
                //scalar multiples
                multply_rows_and_mod(row1,i,rowsum1,width);
                multply_rows_and_mod(row2,j,rowsum2,width);
                multply_rows_and_mod(row3,k,rowsum3,width);
                multply_rows_and_mod(row4,l,rowsum4,width);
                if(height>4){multply_rows_and_mod(row5,m,rowsum5,width);}
                if(height>5){multply_rows_and_mod(row6,n,rowsum6,width);}
                if(height>6){multply_rows_and_mod(row7,o,rowsum7,width);}

                //sum rows
                sum_rows_and_mod(rowsum1,rowsum2,rowsum1,width);
                sum_rows_and_mod(rowsum1,rowsum3,rowsum1,width);
                sum_rows_and_mod(rowsum1,rowsum4,rowsum1,width);
                if(height>4){sum_rows_and_mod(rowsum1,rowsum5,rowsum1,width);}
                if(height>5){sum_rows_and_mod(rowsum1,rowsum6,rowsum1,width);}
                if(height>6){sum_rows_and_mod(rowsum1,rowsum7,rowsum1,width);}

                temp = weight(rowsum1,width,exclude);

                if (temp < new_min_distance && temp != 0)
                {
                    new_min_distance = temp;
                }
                //std::cout<<temp<<",";
                if (temp == min_w)
                {
                    minwords++;
                    update_weight_dist(weight_dist, width, rowsum1,exclude);
                }
            }
        }}
            auto endtime = std::chrono::steady_clock::now();
            auto diff = endtime - start;
            std::cout <<std::endl << std::chrono::duration <double> (diff).count() <<  std::endl;
            timekeep = 0;
        }
        else{
                        for (int m=0;m<M;m++){
        for (int n=0;n<N;n++){
        for (int o=0;o<O;o++){

                //std::cout<<i<<","<<j<<","<<k<<","<<l<<","<<m<<","<<n<<","<<o<<std::endl;
                //scalar multiples
                multply_rows_and_mod(row1,i,rowsum1,width);
                multply_rows_and_mod(row2,j,rowsum2,width);
                multply_rows_and_mod(row3,k,rowsum3,width);
                multply_rows_and_mod(row4,l,rowsum4,width);
                if(height>4){multply_rows_and_mod(row5,m,rowsum5,width);}
                if(height>5){multply_rows_and_mod(row6,n,rowsum6,width);}
                if(height>6){multply_rows_and_mod(row7,o,rowsum7,width);}

                //sum rows
                sum_rows_and_mod(rowsum1,rowsum2,rowsum1,width);
                sum_rows_and_mod(rowsum1,rowsum3,rowsum1,width);
                sum_rows_and_mod(rowsum1,rowsum4,rowsum1,width);
                if(height>4){sum_rows_and_mod(rowsum1,rowsum5,rowsum1,width);}
                if(height>5){sum_rows_and_mod(rowsum1,rowsum6,rowsum1,width);}
                if(height>6){sum_rows_and_mod(rowsum1,rowsum7,rowsum1,width);}

                temp = weight(rowsum1,width,exclude);

                if (temp < new_min_distance && temp != 0)
                {
                    new_min_distance = temp;
                }
                //std::cout<<temp<<",";
                if (temp == min_w)
                {
                    minwords++;
                    update_weight_dist(weight_dist, width, rowsum1,exclude);
                }
            }
        }}

        }
        }
        }
        }
        }

        std::cout<<std::endl;
        std::cout<<minwords<<" codewords of min distance"<<std::endl;;

        std::cout<<"N="<<width-exclude.size()<<", Minimum distance="<<min_w<<std::endl;


        for (auto elem : puncturelist)
        {
            weight_dist[elem] = -1;
            std::cout << elem << ",";
        }

        std::cout<<std::endl<<"weights:"<<std::endl;
        for(int i=0;i<width;i++)
        {
            std::cout<<weight_dist[i]<<",";
        }
        std::cout<<std::endl;

        int min_column = 10000000;
        int min_index = 0;
        for(int i=0;i<width;i++)
        {
            if (weight_dist[i]<min_column && !(exclude.count(i))){
                min_column = weight_dist[i];
                min_index = i;
            }
        }
        //std::cout<<std::endl;
        //std::cout<<"Min index:"<<min_index<<", Min value:"<<min_column<<", Puncturing ";

        disthistory[puncturing] = min_w;
        if (min_column != 0)
        {
            min_w -= 1;
        }

        for(int i=0; i < exclude.size(); i++)
        {
            //std::cout << exclude.at(i) << ' ';
        }
        //std::cout<<std::endl;
        exclude.insert(min_index);
        puncturelist.push_back(min_index);
    }



    std::string fileName = "C:/Users/user/Desktop/Dissertation/LinearCodes/"+n+"_output.txt";
    std::ofstream output_file;
    output_file.open(fileName.c_str());
    assert(output_file.is_open());
    //output_file << "puncturing"<<std::endl;

    for(int i=0; i < puncturelist.size(); i++)
    {
        output_file << puncturelist.at(i)<<",";
        //std::cout << exclude.at(i) << ' ';
    }

    output_file<<std::endl;
    //std::cout<<"distance history"<<std::endl;
    output_file<<"distance history"<<std::endl;
    for(int i=0;i<punctcap;i++){
        //std::cout<<disthistory[i]<<",";;
        output_file << disthistory[i]<<",";
    }

    output_file.close();

    std::cout<<"finish";





}

delete [] candidates;
DeallocateMatrix(height, A);
delete [] rowsum1;
delete [] rowsum2;
delete [] rowsum3;
delete [] rowsum4;
delete [] rowsum5;
}



int weight(int* vec, int len, std::unordered_set<int> exclude)
{
    int w = 0;
    for(int i=0; i<len;i++){
        if(exclude.count(i)) {
            ;
        } else {
            if (vec[i] != 0.0){
                w++;
            }
        }

    }
    return w;
}

void update_weight_dist(int* weight_dist, int len, int* row, std::unordered_set<int> exclude)
{
    for(int i=0; i<len; i++)
    {
        if(exclude.count(i)) {
            ;
        } else {
            if (row[i] != 0)
            {
                weight_dist[i] ++;
            }
        }
    }

}


void multply_rows_and_mod(int* row1, int mult, int* result, int rowlength){
    for(int i=0; i<rowlength; i++){
        result[i]=(row1[i]*mult)%11;
    }
}

void sum_rows_and_mod(int* row1, int* row2, int* result, int rowlength)
{
    for(int i=0; i<rowlength; i++){
        result[i]=(row1[i]+row2[i])%11;
    }
}

int** AllocateMatrix(int noRows,int noCols)
{
    int** pMatrix;
    pMatrix = new int*[noRows];
    for (int i=0; i<noRows; i++)
        {pMatrix[i] = new int[noCols];
    }
    return pMatrix;
}

void DeallocateMatrix(int noRows,int** pMatrix)
{
    for (int i=0; i<noRows; i++)
        {
            delete[] pMatrix[i];
    }
    delete[] pMatrix;
}



