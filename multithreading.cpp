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

int weight(int* vec, int len);
int** AllocateMatrix(int noRows,int noCols);
void DeallocateMatrix(int noRows,int** pMatrix);
void sum_rows_and_mod(int* row1, int* row2, int* result, int rowlength);
void multply_rows_and_mod(int* row1, int mult, int* result, int rowlength);
bool CheckCodewordSelection_1Row(int** A, int bestsofar, int &min_w,int width, int a);
bool CheckCodewordSelection_2Rows(int** A, int bestsofar, int &min_w,int width, int a, int b);
bool CheckCodewordSelection_3Rows(int** A, int bestsofar, int &min_w,int width, int a, int b, int c);
void *CheckCodewordSelection_4Rows(void *threadarg);

struct thread_data {
    int  thread_id;
    int** A;
    int bestsofar;
    int min_w;
    int width;
    int a;
    int b;
    int c;
    int d;
    int n;
    int height;
    //std::vector<int> Threads
};

int main(int argc, char* argv[])
{
    int cand_count = 368;
    //int ActiveThreadCount;
    //std::vector<int> ActiveThreads(10,0);


    // read the number of codes to test and allocate memory
    std::ifstream readCandidates;
    readCandidates.open("C:/Users/user/Desktop/Dissertation/LinearCodes/Candidates.txt");
    readCandidates >> cand_count;
    int candidates[cand_count];

    //read the candidate numbers into memory
    std::cout<<cand_count<<std::endl;
    for(int i=0; i<cand_count;i++){
        readCandidates >> candidates[i];
    }

    std::ifstream readFile;
    int bounds[6] = {90,87,81,81,80,78};
    bool crap = 0;
    int min_w = 1000;
    int height;
    int bestsofar = 62;
    int h_minus = 1;

    int width;
    int** A;
    std::string readfilename;

    auto t1 = std::chrono::high_resolution_clock::now();

    for(int cand_index=0;cand_index<cand_count+1;cand_index++){


        int n = candidates[cand_index];
        if(crap){
            std::cout<<" "<<min_w<<" ";
            remove(readfilename.c_str());
        }

        if (cand_index == cand_count){
            break;
        }

        readfilename = "C:/Users/user/Desktop/Dissertation/LinearCodes/code"+std::to_string(n)+".txt";
        readFile.open(readfilename);
        if (!readFile.is_open()){
            continue;
        }


        readFile >> height;
        readFile >> width;


        bestsofar = bounds[height-3];



        A = AllocateMatrix(height,width);
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                readFile >> A[i][j];

            }
        }

        readFile.close();

        //if(crap){std::cout<<"threw "<<cand_index-1<<" with weight "<<min_w<<std::endl;}
        crap = 0;
        min_w = 1000;
        crap = CheckCodewordSelection_1Row(A,bestsofar,min_w,width,height);
        if (crap){continue;}
        crap = CheckCodewordSelection_2Rows(A,bestsofar,min_w,width,height,height);
        if (crap){continue;}
        crap = CheckCodewordSelection_3Rows(A,bestsofar,min_w,width,height,height,height);
        if (crap){continue;}

        //send hard cases to another thread

        struct thread_data *my_data;
        my_data = new struct thread_data;
        my_data->thread_id = n;
        my_data->A=A;
        my_data->bestsofar=bestsofar;
        my_data->min_w=min_w;
        my_data->width=width;
        my_data->a=height-h_minus;
        my_data->b=height-h_minus;
        my_data->c=height-h_minus;
        my_data->d=height-h_minus;
        my_data->n=n;
        my_data->height=height;
        //std::vector<int> = ActiveThreads

        //std::cout<<"DATA:"<<my_data->height<<my_data->a<<my_data->b<<std::endl;

        //ActiveThreadCount = ActiveThreads.size();
        int rc;
        pthread_t thread;
        //int newthreadid = -1;
        //if (ActiveThreadCount < 10){

        //    for (int i=0,i<10;i++){
        //        for(int id=0;id<ActiveThreadCount;id++)
        //        {
        //            if (std::count(ActiveThreads.begin(), ActiveThreads.end(), i)  == 0)
        //            {
        //                newthreadid = i;
        //                break;
        //            }
        //        }
        //        if (newthreadid != -1){break;}
        //    }
        rc = pthread_create(&thread, NULL, CheckCodewordSelection_4Rows, (void *)my_data);
        //ActiveThreads.push_back()
        //ActiveThreadCount ++;
        if (rc)
        {
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }

    }

    DeallocateMatrix(height,A);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout<<"waiting to end"<<std::endl;
    Sleep(100000);


    auto duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
    std::cout << "time elapsed" << duration;

    return 0;
}



int weight(int* vec, int len)
{
    int w = 0;
    for(int i=0; i<len;i++){
        if (vec[i] != 0.0){
            w++;
        }
    }
    return w;
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

bool CheckCodewordSelection_1Row(int** A, int bestsofar, int &min_w,int width, int a)
{
    int* row1;
    int* rowsum1;
    bool crap = 0;

    rowsum1 = new int[width];

    for(int row_1_index = 0; row_1_index<a; row_1_index++){
        for (int k=1;k<11;k++){
            row1 = A[row_1_index];
            multply_rows_and_mod(row1,k,rowsum1,width);
            int w = weight(rowsum1,width);
            if (w < min_w && w != 0){
                min_w = w;
            }
            if(w<bestsofar && w != 0){
                crap = 1;
                break;
            }
        }
        if (crap){break;}
    }
    delete [] rowsum1;
    return crap;
}

bool CheckCodewordSelection_2Rows(int** A, int bestsofar, int &min_w,int width, int a, int b)
{
    int* row1;
    int* row2;
    int* rowsum1;
    int* rowsum2;
    bool crap = 0;

    rowsum1 = new int[width];
    rowsum2 = new int[width];

    for(int row_1_index = 0; row_1_index<a; row_1_index++){
       for(int row_2_index = 0; row_2_index<b; row_2_index++){
            for (int k=1;k<11;k++){
                for (int l=1;l<11;l++){
                    row1 = A[row_1_index];
                    row2 = A[row_2_index];
                    multply_rows_and_mod(row1,k,rowsum1,width);
                    multply_rows_and_mod(row2,l,rowsum2,width);
                    sum_rows_and_mod(rowsum1,rowsum2,rowsum1,width);
                    int w = weight(rowsum1,width);
                    if (w < min_w && w != 0){
                        min_w = w;
                    }
                    if(w<bestsofar && w != 0){
                        crap = 1;
                        break;
                    }
                }if (crap){break;}
            }if (crap){break;}
        }
        if (crap){break;}
    }
    delete [] rowsum1;
    delete [] rowsum2;

    return crap;
}

bool CheckCodewordSelection_3Rows(int** A, int bestsofar, int &min_w,int width, int a, int b, int c)
{
    int* row1;
    int* row2;
    int* row3;
    int* rowsum1;
    int* rowsum2;
    int* rowsum3;
    bool crap = 0;

    rowsum1 = new int[width];
    rowsum2 = new int[width];
    rowsum3 = new int[width];

    for(int row_1_index = 0; row_1_index<a; row_1_index++){
       for(int row_2_index = 0; row_2_index<b; row_2_index++){
            for(int row_3_index = 0; row_3_index<c; row_3_index++){
                for (int k=1;k<11;k++){
                    for (int l=1;l<11;l++){
                        for (int m=1;m<11;m++){
                            row1 = A[row_1_index];
                            row2 = A[row_2_index];
                            row3 = A[row_3_index];
                            multply_rows_and_mod(row1,k,rowsum1,width);
                            multply_rows_and_mod(row2,l,rowsum2,width);
                            multply_rows_and_mod(row3,m,rowsum3,width);
                            sum_rows_and_mod(rowsum1,rowsum2,rowsum1,width);
                            sum_rows_and_mod(rowsum1,rowsum3,rowsum1,width);
                            int w = weight(rowsum1,width);
                            if (w < min_w && w != 0){
                                min_w = w;
                            }
                            if(w<bestsofar && w != 0){
                                crap = 1;
                                break;
                            }
                        }if (crap){break;}
                    }if (crap){break;}
                }if (crap){break;}
            }if (crap){break;}
        }
        if (crap){break;}
    }
    delete [] rowsum1;
    delete [] rowsum2;
    delete [] rowsum3;

    return crap;
}


//overhead too big on this


void *CheckCodewordSelection_4Rows(void *threadarg) {
   struct thread_data *my_data;
   my_data = (struct thread_data *) threadarg;

   std::cout<<"starting thread "<<my_data->thread_id<<std::endl;

    int* row1;
    int* row2;
    int* row3;
    int* row4;
    int* rowsum1;
    int* rowsum2;
    int* rowsum3;
    int* rowsum4;
    bool crap = 0;

    //std::cout<<"a:"<<my_data->width<<std::endl;

    rowsum1 = new int[my_data->width];
    rowsum2 = new int[my_data->width];
    rowsum3 = new int[my_data->width];
    rowsum4 = new int[my_data->width];

    //std::cout<<my_data->thread_id<<" doing work"<<std::endl;
    for(int row_1_index = 0; row_1_index<my_data->a; row_1_index++){

       for(int row_2_index = 0; row_2_index<my_data->b; row_2_index++){
            for(int row_3_index = 0; row_3_index<my_data->c; row_3_index++){
                for(int row_4_index = 0; row_4_index<my_data->d; row_4_index++){
                    for (int k=1;k<11;k++){
                        for (int l=1;l<11;l++){
                            for (int m=1;m<11;m++){
                                for (int o=1;o<11;o++){
                                    row1 = my_data->A[row_1_index];
                                    row2 = my_data->A[row_2_index];
                                    row3 = my_data->A[row_3_index];
                                    row4 = my_data->A[row_4_index];
                                    multply_rows_and_mod(row1,k,rowsum1,my_data->width);
                                    multply_rows_and_mod(row2,l,rowsum2,my_data->width);
                                    multply_rows_and_mod(row3,m,rowsum3,my_data->width);
                                    multply_rows_and_mod(row4,o,rowsum4,my_data->width);
                                    sum_rows_and_mod(rowsum1,rowsum2,rowsum1,my_data->width);
                                    sum_rows_and_mod(rowsum1,rowsum3,rowsum1,my_data->width);
                                    sum_rows_and_mod(rowsum1,rowsum4,rowsum1,my_data->width);
                                    int w = weight(rowsum1,my_data->width);
                                    if (w < my_data->min_w && w != 0){
                                        my_data->min_w = w;
                                    }
                                    if(w<my_data->bestsofar && w != 0){
                                        crap = 1;
                                        break;
                                    }
                                }if (crap){break;}
                            }if (crap){break;}
                        }if (crap){break;}
                    }if (crap){break;}
                }if (crap){break;}
            }if (crap){break;}
        }

       std::cout<<my_data->thread_id<<" still open"<<std::endl;
        if (crap){break;}
    }
    delete [] rowsum1;
    delete [] rowsum2;
    delete [] rowsum3;
    delete [] rowsum4;

    std::string oldfilename = "C:/Users/user/Desktop/Dissertation/LinearCodes/code"+std::to_string(my_data->n)+".txt";

    //std::cout<<"old:"<<oldfilename<<std::endl<<"new:"<<newfilename<<std::endl;
    char* c1 = const_cast<char*>(oldfilename.c_str());
    if (!crap)
    {
        std::cout<<std::endl;
        std::cout<<my_data->n<<":"<<"["<<my_data->width<<","<<my_data->height<<","<<my_data->min_w<<"]"<<std::endl;



        if (my_data->min_w >= my_data->bestsofar){
            std::string newfilename =  "C:/Users/user/Desktop/Dissertation/CandidatePile/code"+std::to_string(my_data->n)+".txt";
            char* c2 = const_cast<char*>(newfilename.c_str());
            std::cout<<"saved "<<my_data->n<<std::endl;
            std::ofstream writeFile;
            writeFile.open("C:/Users/user/Desktop/Dissertation/CandidatePile/Candidates_passed.txt",std::ios_base::app);
            assert(writeFile.is_open());
            writeFile << my_data->n;
            writeFile << " ";
            writeFile.close();
            if(rename(c1,c2)!=0)
                std::cout<<"file renamed successfully.";
            else
                std::cout<<"error renaming file.";
//            std::ifstream in(c1, std::ios::in | std::ios::binary);
//            std::ofstream out(c2, std::ios::out | std::ios::binary);
//            out << in.rdbuf();
        }
    }
    if (crap)
    {
        std::cout<<std::endl<<my_data->thread_id<<" thrown, weight="<<my_data->min_w<<" vs "<<my_data->bestsofar<<std::endl;
    }
    std::remove(c1);


   std::cout<<"closing thread "<<my_data->thread_id<<std::endl;
   pthread_exit(NULL);
}


