#include <iostream>
#include <chrono>
#include <omp.h>

#define THREAD_NUM 12

#define WARMUP_NUM 10
#define MEASURE_NUM 1000

#define M_ROWS 700
#define M_COLUMNS_V_ROWS 500

#define RAND_LEFT -1000
#define RAND_RIGHT 1000

double* multiply_matrix_by_vector(double** matrix, int rows, int columns, double* vector);

int randomInt(int left, int right)
{
    return rand() % (right - left + 1) + left;
}

void output_matrix(double** matrix, int rows, int columns)
{
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void output_vector(double* vector, int columns)
{
    for(int j = 0; j < columns; ++j)
    {
        std::cout << vector[j] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::srand(61771);
    double** matrix = malloc(sizeof(double*)*M_ROWS);//new double*[M_ROWS];
    double* vector = malloc(sizeof(double)*M_COLUMNS_V_ROWS);//new double[M_COLUMNS_V_ROWS];

    for(int i = 0; i < M_ROWS; ++i)
    {
        matrix[i] = new double[M_COLUMNS_V_ROWS];
        for(int j = 0; j < M_COLUMNS_V_ROWS; ++j)
        {
            matrix[i][j] = (double)randomInt(RAND_LEFT, RAND_RIGHT);
        }
    }

    for(int i = 0; i < M_COLUMNS_V_ROWS; ++i)
    {
        vector[i] = (double)randomInt(RAND_LEFT, RAND_RIGHT);
    }

    /*output_matrix(matrix, M_ROWS, M_COLUMNS_V_ROWS);
    output_vector(vector, M_COLUMNS_V_ROWS);*/

    //double* result = multiply_matrix_by_vector(matrix, M_ROWS, M_COLUMNS_V_ROWS, vector);
    for(int i = 0; i < WARMUP_NUM; ++i)
        multiply_matrix_by_vector(matrix, M_ROWS, M_COLUMNS_V_ROWS, vector);

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < MEASURE_NUM; ++i)
        multiply_matrix_by_vector(matrix, M_ROWS, M_COLUMNS_V_ROWS, vector);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "Time taken by function: "
         << duration.count()/1e6 << " seconds" << std::endl;

    //output_vector(result, M_ROWS);

    for(int i = 0; i < M_ROWS; ++i)
        delete matrix[i];

    delete [] matrix;
    delete [] vector;
    //delete result;
    return 0;
}

double* multiply_matrix_by_vector(double** matrix, int rows, int columns, double* vector)
{
    double* m_result = new double[M_ROWS];
    #pragma omp parallel for num_threads(THREAD_NUM)
    for(int i = 0; i < rows; ++i)
    {
        //#pragma omp parallel for num_threads(4)
        for(int j = 0; j < columns; ++j)
            m_result[i] += matrix[i][j] * vector[j];
    }

    return m_result;
}