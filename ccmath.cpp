/* ccmath.cpp    Basice mathematic functions
  * Author: Charley Chang
  * Created: May 2018
  * This is free and you can do anything you want with this code
*/

# include "ccmath.h"
# include <math.h>
# include <complex>
# include <memory.h>
#include <cfloat>

namespace __CCMATH__ {

  using namespace std;

  /// abs of a complex double number
  /// @ X: input complex douoble number
  /// return sqrt(X.real^2+X.imag^2)
  /// WARNING: recommend to use std function: abs();
  double abs2(complex<double> X)
  {
      return sqrt(X.imag()*X.imag()+X.real()*X.real());
  }

  /// multiply between 2 complex numbers
  /// @X: input number
  /// @Y: input number
  /// return complex number, mulitply result
  /// WARNING: recommend to use std function: * ;
  complex<double> multi(complex<double> X, complex<double> Y)
  {
      double real = X.real()*Y.real() - X.imag()*Y.imag();
      double imag = X.real()*Y.imag() + X.imag()*Y.real();
      return complex<double>(real,imag);
  }

  /// find the maximum element of 1-D array
  /// @X: 1-D array
  /// @y: maximum element to be output
  /// @N: array length
  /// return ID of the maximum element (ID starts from 0),
  int max(double *X, double &y, int N)
  {
      if (N < 2){
          y = X[0];
          return 0;
      }
      else if (N < 3){
          if (X[0] > X[1]){
              y = X[0];
              return 0;
          }
          else{
              y = X[1];
              return 1;
          }
      }
      else{
          int half_id = floor(N/2);   // half size of array
          double dMax_low, dMax_high; // maximum element of lower&higher half array
          int id_low = max(X, dMax_low, half_id);
          int id_high = max(X+half_id, dMax_high, N-half_id);
          if(dMax_low > dMax_high){
              y = dMax_low;
              return id_low;
          }
          else{
              y = dMax_high;
              return half_id+id_high;
          }
      }
  }

  int max(complex<double>* X, complex<double> &y, int N)
  {
      if (N < 2){
          y = X[0];
          return 0;
      }
      else if (N < 3){
          if (abs(X[0]) > abs(X[1])){
              y = X[0];
              return 0;
          }
          else{
              y = X[1];
              return 1;
          }
      }
      else{
          int half_id = floor(N/2);   // half_size of array
          complex<double> max_low, max_high;  // maximum element of the lower&higher half
          int id_low = max(X, max_low, half_id);
          int id_high = max(X+half_id, max_high, N-half_id);
          if(abs(max_low) > abs(max_high)){
              y = max_low;
              return id_low;
          }
          else{
              y = max_high;
              return half_id+id_high;
          }
      }
  }

  /// find the maximum element of 2-D array
  /// @X: 2-D array
  /// @y: maximum element
  /// @M: rows of array
  /// @N: columns of array
  /// return row and column ID of the maximum element
  /// WARMING: DON"T FORGET TO DESTROY THE RETURN HEAP
  int* max(double** X, double &y, int M, int N)
  {
      int *id = new int[M];       // maximum element ID of each row
      memset(id, 0, M*sizeof(int));
      double* Y = new double[M];  // maximum element of each row
      memset(Y, 0.0, M*sizeof(double));
      for (int i = 0; i < M; i++){
          id[i] = max(X[i], Y[i], N);
      }
      int* iid = new int[2];   // maximum element ID in rows and columns
      memset(iid, 0, 2*sizeof(int));
      iid[0] = max(Y, y, M);
      iid[1] =  id[iid[0]];
      delete[] id; delete[] Y;
      return iid;
  }

  int* max(complex<double>** X, complex<double> &y, int M, int N)
  {
      int *id = new int[M];
      complex<double> *Y = new complex<double>[M];
      for (int i = 0; i < M; i++){
          id[i] = max(X[i], Y[i], N);   // get maximum element of each row
      }
      int* iid = new int[2];
      iid[0] = max(Y, y, M);
      iid[1] =  id[iid[0]];
      delete[] id; delete[] Y;
      return iid;
  }

  /// find the minimum element of 1-D array
  /// @X: 1-D array
  /// @y: minimum element
  /// @N: array length
  /// return ID of the minimum element, start from 0
  int min(double* X, double &y, int N)
  {
      if (N < 2)                 // 1 element return 0
      {
          y = X[0];
          return 0;
      }
      else if (N < 3){           // 2 elements return ID of the maximum one, and put maximum value in X[0]
          if (X[0] < X[1])
          {
              y = X[0];
              return 0;
          }
          else{
              y = X[1];
              return 1;
          }
      }
      else{                     // > 2 element, min the lower and upper half
          int half_id = floor(N/2);
          double min_low, min_high;   // minimum element of the lower&higher half
          int id_low = min(X, min_low, half_id);
          int id_high = min(X+half_id, min_high, N-half_id);
          if(min_low < min_high){
              y = min_low;
              return id_low;
          }
          else{
              y = min_high;
              return half_id+id_high;
          }
      }
  }

  int min(complex<double>* X, complex<double> &y, int N)
  {
      if (N < 2){
          y = X[0];
          return 0;
      }
      else if (N < 3){
          if (abs(X[0]) < abs(X[1]))
          {
              y = X[0];
              return 0;
          }
          else{
              y = X[1];
              return 1;
          }
      }
      else{
          int half_id = floor(N/2);
          complex<double> min_low, min_high; // minimum element of the lower&higher half
          int id_low = min(X, min_low, half_id);
          int id_high = min(X+half_id, min_high, N-half_id);
          if(abs(min_low) < abs(min_high)){
              y = min_low;
              return id_low;
          }
          else{
              y = min_high;
              return half_id+id_high;
          }
      }
  }

  /// find the minimum element of 2-D array
  /// @X: 2-D array
  /// @M: rows of array
  /// @N: columns of array
  /// return row and column ID of the minimum element, X[0][0] is the minimum element
  /// WARMING: DON"T FORGET TO DESTROY THE RETURN HEAP
  int* min(double** X, double &y, int M, int N)
  {
      int *id = new int[M];
      double* Y = new double[M];
      for (int i = 0; i < M; i++){     // find minimum element in each row
          id[i] = min(X[i], Y[i], N);
      }
      int* iid = new int[2];
      iid[0] = min(Y, y, M);
      iid[1] =  id[iid[0]];
      delete[] id; delete[] Y;
      return iid;
  }

  int* min(complex<double>** X, complex<double> &y, int M, int N)
  {
      int *id = new int[M];
      complex<double>* Y = new complex<double>[M];
      for (int i = 0; i < M; i++){      // find minimum element in each row
          id[i] = min(X[i], Y[i], N);
      }
      int* iid = new int[2];
      iid[0] = min(Y, y, M);
      iid[1] =  id[iid[0]];
      delete[] id; delete[] Y;
      return iid;
  }

  /// sort 1-D array
  /// @ X: 1-D array
  /// @ N : array length;
  /// @ Type: 0 = increase; 1 = decrease
  /// the sorted array is stored back into X;
  void sort(double* X, int N, int Type)
  {
      switch (Type) {
        case 0:    // increase
        {
            if (N < 2) {    // if N < 2, do nothing
                return;
            }
            else{
                double *low = new double[N-1]; int nlow = 0;     // heap to store higher elements and length
                double *high = new double[N-1]; int nhigh = 0;   // heap to store lower elements and length
                for (int i = 0; i < N-1; i++) {   // earch all elements, higher than the last one store in higher heap, lower than last store in lower heap
                    if (X[i] > X[N-1]){
                        high[nhigh++] = X[i];
                    }
                    else{
                        low[nlow++] = X[i];
                    }
                }
                // sort the lower&higher parts, store them back into X X
                memcpy(X,low, nlow*sizeof(double));
                X[nlow] = X[N-1];
                memcpy(X+nlow+1, high, nhigh*sizeof(double));
                delete[] low;
                delete[] high;
                sort(X, nlow, Type);
                sort(X+nlow+1,nhigh,Type);
            }
            break;
        }
        case 1:
        {
            if (N < 2) {    // if N < 2, do nothing
                return;
            }
            else{
                double *low = new double[N-1]; int nlow = 0;     // heap to store higher elements and length
                double *high = new double[N-1]; int nhigh = 0;   // heap to store lower elements and length
                for (int i = 0; i < N-1; i++) {   // earch all elements, higher than the last one store in higher heap, lower than last store in lower heap
                    if (X[i] > X[N-1]){
                        high[nhigh++] = X[i];
                    }
                    else{
                        low[nlow++] = X[i];
                    }
                }
                // sort the lower&higher parts, store them back into X
                memcpy(X,high, nhigh*sizeof(double));
                X[nhigh] = X[N-1];
                memcpy(X+nhigh+1, low, nlow*sizeof(double));
                delete[] low;
                delete[] high;
                sort(X, nhigh, Type);
                sort(X+nhigh+1,nlow,Type);
            }
            break;
        }
      }
  }

  void sort(complex<double>* X, int N, int Type)
  {
      switch (Type) {
        case 0:    // increase
        {
            if (N < 2) {    // if N < 2, do nothing
                return;
            }
            else{
                complex<double> *low = new complex<double>[N-1]; int nlow = 0;    // heap to store higher elements and length
                complex<double> *high = new complex<double>[N-1]; int nhigh = 0;   // heap to store lower elements and length
                for (int i = 0; i < N-1; i++) {   // earch all elements, higher than the last one store in higher heap, lower than last store in lower heap
                    if (abs(X[i]) > abs(X[N-1])){
                        high[nhigh++] = X[i];
                    }
                    else{
                        low[nlow++] = X[i];
                    }
                }
                // sort the lower&higher parts, store them back into X
                memcpy(X,low, nlow*sizeof(complex<double>));
                X[nlow] = X[N-1];
                memcpy(X+nlow+1, high, nhigh*sizeof(complex<double>));
                delete[] low;
                delete[] high;
                sort(X, nlow, Type);
                sort(X+nlow+1,nhigh,Type);
            }
            break;
        }
        case 1:
        {
            if (N < 2) {    // if N < 2, do nothing
                return;
            }
            else{
                complex<double> *low = new complex<double>[N-1]; int nlow = 0;    // heap to store higher elements and length
                complex<double> *high = new complex<double>[N-1]; int nhigh = 0;   // heap to store lower elements and length
                for (int i = 0; i < N-1; i++) {   // earch all elements, higher than the last one store in higher heap, lower than last store in lower heap
                    if (abs(X[i]) > abs(X[N-1])){
                        high[nhigh++] = X[i];
                    }
                    else{
                        low[nlow++] = X[i];
                    }
                }
                // sort the lower&higher parts, store them back into X
                memcpy(X,high, nhigh*sizeof(complex<double>));
                X[nhigh] = X[N-1];
                memcpy(X+nhigh+1, low, nlow*sizeof(complex<double>));
                delete[] low;
                delete[] high;
                sort(X, nhigh, Type);
                sort(X+nhigh+1,nlow,Type);
            }
            break;
        }
      }
  }

  /// average value of 1-D array
  /// X: 1-D array
  /// N: length of array
  /// return average value
  double mean(double *X, int N)
  {
      double sum = 0;
      for (int i = 0; i < N; i++) // sum up
          sum += X[i];
      return sum/N;
  }

  complex<double> mean(complex<double> *X, int N)
  {
      complex<double> sum = complex<double>(0,0);
      for (int i = 0; i < N; i++) // sum up
          sum += X[i];
      return complex<double>(sum.real()/N,sum.imag()/N);
  }

  // double 类型的相等
  // a/b 比较的两个数
  // return true = 相等 false=不相等
  bool equal(const double &a, const double &b)
   {
       if (fabs(a - b) <  DBL_EPSILON)
           return true;
       else
           return false;
   }
   bool equal(const complex<double> &a, const complex<double> &b)
   {
       bool b1 = (a.real() == b.real());
       bool b2 = (a.imag() == b.imag());
       return b1&b2;
   }

  /// sort 2-D array
  /// @ X: 2-D array
  /// @ M: rows number
  /// @ N : colums number
  /// @ Type: 0 = increase; 1 = decrease
  void sort(double** X, int M, int N, int Type)
  {
        for (int i = 0; i < M; i++) {
            sort(X[i], N, Type);
        }
        for (int i = 0; i < N; i++) {
            double *Y = new double[M];
            for (int ii = 0; ii < M; ii++) {
                Y[ii] = X[ii][i];
            }
            sort(Y, M, Type);
            for (int ii = 0; ii < M; ii++) {
                X[ii][i] = Y[ii];
            }
            delete[] Y;
        }
  }

  void sort(complex<double>** X, int M, int N, int Type)
  {
        for (int i = 0; i < M; i++) {
            sort(X[i], N, Type);
        }
        for (int i = 0; i < N; i++) {
            complex<double> *Y = new complex<double>[M];
            for (int ii = 0; ii < M; ii++) {
                Y[ii] = X[ii][i];
            }
            sort(Y, M, Type);
            for (int ii = 0; ii < M; ii++) {
                X[ii][i] = Y[ii];
            }
            delete[] Y;
        }
  }

  /// reverse 1-D array
  /// @X  array
  /// @N  length of array
  void reverse(double *X, int N)
  {
      int M = 0;
      if (fmod(N,2) == 1) // if N is odd
      {
          M = (N - 1)/2;
      }
      else           // if N is even
      {
          M = N/2;
      }
      for (int i = 0; i < M; i++){  // reverse
          double Y = X[i];
          X[i] = X[N - i - 1];
          X[N-i-1] = Y;
      }
  }

  void reverse(complex<double> *X, int N)
  {
      int M = 0;
      if (fmod(N,2) == 1) // if N is odd
      {
          M = (N - 1)/2;
      }
      else           // if N is even
      {
          M = N/2;
      }
      for (int i = 0; i < M; i++){  // reverse
          complex<double> Y = X[i];
          X[i] = X[N - i - 1];
          X[N-i-1] = Y;
      }
  }


  // TYPE = 0: Y[i] = X[i] - X[i+N]
  // TYPE = 1: Y[i] = X[i+N] - X[i]
  // X: input array
  // Y: output array
  // M: length of X
  // N: distance of two minus elements of X
  // Type: difference order.
  // return: TRUE=success; FALSE = failed;
  // Warning: the memory of Y should be "new" before this function is called
  //          the size should be (M-N)*sizeof(double);
  bool diff(double *X, double *Y, int M, int N, int Type)
  {
      if (Y == NULL) // if Y is empty
          return false;

      if (N >= M) // if N-distance is bigger than length-of-X, return false
          return false;

      memset(Y, 0, (M-N)*sizeof(double));

      switch (Type) {
      case 0:
          for (int i = 0; i < M-N; i++)
          {
              Y[i] = X[i] - X[i+N];
          }
          break;
      case 1:
          for (int i = 0; i < M-N; i++)
          {
              Y[i] = X[i+N] - X[i];
          }
          break;
      default: // other return false;
          return false;
          break;
      }
      return true;
  }

  // TYPE = 0: rising cross, Y[i] = X[i](X[i] <= V), Z[i]=X[i+1](X[i+1] > V)
  // TYPE = 1: falling cross,Y[i] = X[i](X[i] > V), Z[i]=X[i+1](X[i+1] <= V)
  // X: input array
  // Y: one of two output arrays, X[i]
  // Z: one of two output arrays, X[i+1]
  // V: the value which X to be compared with
  // M: length of X
  // N: length of Y/Z
  // Type: 0=rising, 1=falling
  // return: TRUE=success; FALSE = failed;
  // Warning: the memory of Y/Z should be "new" before this function is called
  //          the size should be (M-N)*sizeof(double);
  bool cross(double *X, double **Y, double **Z, int **Y_ID, int **Z_ID, double V, int M, int &N, int Type)
  {
      if (*Y != NULL)
          delete[] *Y;
      if (*Z != NULL)
          delete[] *Z;
      if (*Y_ID != NULL)
          delete[] *Y_ID;
      if (*Z_ID != NULL)
          delete[] *Z_ID;
      N = 0;

      double *Y_tmp = new double[M];
      memset(Y_tmp, 0, M*sizeof(double));
      double *Z_tmp = new double[M];
      memset(Z_tmp, 0, M*sizeof(double));
      int *Y_ID_tmp = new int[M];
      memset(Y_ID_tmp, 0, M*sizeof(int));
      int *Z_ID_tmp = new int[M];

      for (int i = 0; i < M - 1; i++)
      {
          switch (Type) {
          case 0:
              if ((X[i] <= V)&&(X[i+1] > V))
              {
                  Y_tmp[N] = X[i];
                  Y_ID_tmp[N] = i;
                  Z_tmp[N] = X[i+1];
                  Z_ID_tmp[N] = i+1;
                  N++;
              }
              break;
          case 1:
              if ((X[i] > V)&&(X[i+1] <= V))
              {
                  Y_tmp[N] = X[i];
                  Y_ID_tmp[N] = i;
                  Z_tmp[N] = X[i+1];
                  Z_ID_tmp[N] = i+1;
                  N++;
              }
              break;
          default:
              return false;
              break;
          }
      }
      *Y = new double[N];
      *Z = new double[N];
      *Y_ID = new int[N];
      *Z_ID = new int[N];
      memcpy(*Y, Y_tmp, N*sizeof(double));
      memcpy(*Z, Z_tmp, N*sizeof(double));
      memcpy(*Y_ID, Y_ID_tmp, N*sizeof(int));
      memcpy(*Z_ID, Z_ID_tmp, N*sizeof(int));

      delete[] Y_tmp; delete[] Z_tmp;
      delete[] Y_ID_tmp; delete[] Z_ID_tmp;
      return true;
  }

  // Z = xcorr(X,Y)
  // X: input array
  // Y: input array
  // M: length of X
  // N: length of Y
  // Z: output
  // return: (length of z)=M+N-1
  int xcorr(double *X, double *Y, int M, int N, double **Z)
  {
      if (*Z != NULL)
          delete[] *Z;
      *Z = new double[M+N-1];

      for (int i = 0; i < M+N-1; i++)
      {
          *(*Z+i) = 0.0;
          int nTmp = i-N+1;
          if (nTmp < 0)
          {
              for (int j = N-i-1; j < N ; j++)
              {
                  if (j-N+i+1 > M)
                      continue;
                  *(*Z+i) += X[j-N+i+1]*Y[j];
              }
          }
          else
          {
              for (int j = i-N+1; j < M; j++)
              {
                  if (j+N-i-1 > N)
                      continue;
                  *(*Z+i) += X[j]*Y[j+N-i-1];
              }
          }
      }
      return M+N-1;
  }

  int xcorr(complex<double> *X, complex<double> *Y, int M, int N, complex<double> **Z)
  {
      if (*Z != NULL)
          delete[] *Z;
      *Z = new complex<double> [M+N-1];

      for (int i = 0; i < M+N-1; i++)
      {
          *(*Z+i) = 0;
          int nTmp = i-N+1;
          if (nTmp < 0)
          {
              for (int j = N-i-1; j < N ; j++)
              {
                  if (j-N+i+1 > M)
                      continue;
                  *(*Z+i) += X[j-N+i+1]*Y[j];
              }
          }
          else
          {
              for (int j = i-N+1; j < M; j++)
              {
                  if (j+N-i-1 > N)
                      continue;
                  *(*Z+i) += X[j]*Y[j+N-i-1];
              }
          }
      }

      return M+N-1;
  }

  // B = A', transpose of matrix
  // @A : input matrix
  // @B : output matrix
  // row: row number of A
  // col: column number of A
  int MatrixTranspose(double **A, double **B, int row, int col)
  {
      if(row < 1 || col < 1) // if row or column number smaller than 1
      {
          return 1;   // fail
      }
      else
      {
          for (int m = 0; m < col; m++)     // scan each column
          {
              for (int n = 0; n < row; n++) // and each row
              {
                  B[m][n] = A[n][m];  // transpose
              }
          }
          return 0;  // sucess
      }
  }

  int MatrixTranspose(complex<double> **A, complex<double> **B, int row, int col)
  {
      if(row < 1 || col < 1) // if row or column number smaller than 1
      {
          return 1;   // fail
      }
      else
      {
          for (int m = 0; m < col; m++)     // scan each column
          {
              for (int n = 0; n < row; n++) // and each row
              {
                  B[m][n] = A[n][m];  // transpose
              }
          }
          return 0;  // sucess
      }
  }

    // C = A+B
    // @ A: input 2D-matrix
    // @ B: input 2D-matrix
    // @ C: output 2D-matrix
    // @ row: row number
    // @ col: column number
    int MatrixAdd(double **A, double **B, double **C, int row, int col)
    {
        if (row < 1 || col < 1) // if row or column number less than 1
        {
            return 1;  // failed
        }
        else
        {
            for (int m = 0; m < row; m++) // scan through rows
            {
                for (int n = 0; n < col; n++) // scan through columns
                {
                    C[m][n] = A[m][n] + B[m][n]; // add
                }
            }
            return 0; // successful
        }
    }

    int MatrixAdd(complex<double> **A, complex<double> **B, complex<double> **C, int row, int col)
    {
        if (row < 1 || col < 1) // if row or column number less than 1
        {
            return 1;  // failed
        }
        else
        {
            for (int m = 0; m < row; m++) // scan through rows
            {
                for (int n = 0; n < col; n++) // scan through columns
                {
                    C[m][n] = A[m][n] + B[m][n]; // add
                }
            }
            return 0; // successful
        }
    }

    // C = A-B
    // @ A: input 2-D matrix
    // @ B: input 2-D matrix
    // @ C: output 2-D matrix
    // @ row: row number
    // @ col: column number
    int MatrixSub(double **A, double **B, double **C, int row, int col)
    {
        if (row < 1 || col < 1) // if row or column number less than 1
        {
            return 1;  // failed
        }
        else
        {
            for (int m = 0; m < row; m++) // scan through rows
            {
                for (int n = 0; n < col; n++) // scan through columns
                {
                    C[m][n] = A[m][n] - B[m][n]; // add
                }
            }
            return 0; // successful
        }
    }

    int MatrixSub(complex<double> **A, complex<double> **B, complex<double> **C, int row, int col)
    {
        if (row < 1 || col < 1) // if row or column number less than 1
        {
            return 1;  // failed
        }
        else
        {
            for (int m = 0; m < row; m++) // scan through rows
            {
                for (int n = 0; n < col; n++) // scan through columns
                {
                    C[m][n] = A[m][n] - B[m][n]; // add
                }
            }
            return 0; // successful
        }
    }

    // C = A*B
    // @ A: input 2-D matrix
    // @ B: input 2-D matrix
    // @ C: output 2-D matrix
    // @ rowA: row number of A
    // @ colA: column number of A
    // @ rowB: row number of B
    // @ colB: column number of B
    int MatrixMultiply(double **A, double **B, double **C, int rowA, int colA, int rowB, int colB)
    {
        if (rowA < 1 || colA < 1 || rowB < 1 || colB < 1) // if row or column number less than 1
        {
            return 1; // failed
        }
        else if (colA != rowB)  // if column number of A != row number of B
        {
            return 2; // failed
        }
        else
        {
            for (int m = 0; m < rowA; m++)   // scan rows of A
            {
                for (int n = 0; n < colB; n++) // scan columns of B
                {
                    C[m][n] = 0.0;
                    for (int k = 0; k < colA; k++)  // sum up the multiply elements
                    {
                        C[m][n] += A[m][k]*B[k][n];
                    }
                }
            }
            return 0; // successful
        }
    }

    int MatrixMultiply(complex<double> **A, complex<double> **B, complex<double> **C, int rowA, int colA, int rowB, int colB)
    {
        if (rowA < 1 || colA < 1 || rowB < 1 || colB < 1) // if row or column number less than 1
        {
            return 1; // failed
        }
        else if (colA != rowB)  // if column number of A != row number of B
        {
            return 2; // failed
        }
        else
        {
            for (int m = 0; m < rowA; m++)   // scan rows of A
            {
                for (int n = 0; n < colB; n++) // scan columns of B
                {
                    C[m][n] = complex<double>(0.0,0.0);
                    for (int k = 0; k < colA; k++)  // sum up the multiply elements
                    {
                        C[m][n] += A[m][k]*B[k][n];
                    }
                }
            }
            return 0; // successful
        }
    }

    // B = a*A
    // @A : input Matrix
    // @B : output Matrix
    // @a : input coefficient
    // @rowA: row number of A
    // @colA: column number of A
    int MatrixDotMultiply(double **A, double **B, double a, int rowA, int colA)
    {
        for (int m = 0; m < rowA; m++)
            for(int n = 0; n < colA; n++)
                B[m][n] = A[m][n]*a;
        return 0;
    }

    int MatrixDotMultiply(complex<double> **A, complex<double> **B, complex<double> a, int rowA, int colA)
    {
        for (int m = 0; m < rowA; m++)
            for(int n = 0; n < colA; n++)
                B[m][n] = A[m][n]*a;
        return 0;
    }

    // B = det(A)
    // @A : 2-D input matrix
    // @b : output result, 1 element
    // @rowA : row number of A
    // @colA : column number of A
    int MatrixDet(double **A, double *b, int rowA, int colA)
    {
        if (rowA < 1 || colA < 1)  // if row or column number less than 1, failed
        {
            return 1;
        }
        else if (rowA != colA) // if row number doesn't equal to column number, failed
        {
            return 2;
        }
        else
        {
            if (rowA == 1)     // one element matrix
            {
                *b = A[0][0];
                return 0;
            }
            else if(rowA == 2) // 2X2 matrix
            {
                *b = A[0][0]*A[1][1] - A[0][1]*A[1][0];
                return 0;
            }
            else if(rowA == 3) // 3X3 matrix
            {
                *b = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
                   - A[0][1] * (A[1][0] * A[2][2] - A[2][0] * A[1][2])
                   + A[0][2] * (A[1][0] * A[2][1] - A[2][0] * A[1][1]);
                return 0;
            }
            else // row and column number > 4
            {
                double det = 0.0;               // final result to be returned
                for (int m = 0; m < colA; m++)  // each element of the first row multiply with its adjoint matrix
                {
                    double dElement = A[0][m];   // each element of the first row
                    double dNewDet = 0.0;        // det of dElement's adjoint matrix
                    // creat a new sized matrix ( rowA - 1, colA - 1 ), adjoint matrix
                    double **dShrinkA = new double*[rowA-1];
                    for (int n = 1; n < rowA; n++)
                    {
                        dShrinkA[n-1] = new double[colA-1];
                        for (int k = 0; k < colA; k++)
                        {
                            if (k < m)
                            {
                                dShrinkA[n-1][k] = A[n][k];
                            }
                            else if (k > m)
                            {
                                dShrinkA[n-1][k-1] = A[n][k];
                            }
                        }
                    }
                    // calculate the det of dNewDet, and sum up the multiply result between dElement&dNewDet
                    if (MatrixDet(dShrinkA, &dNewDet, rowA-1, colA-1) == 0)
                    {
                          if ((m%2) == 0)
                             det += dElement * dNewDet;
                          else
                             det -= dElement * dNewDet;
                    }
                    delete[] dShrinkA;
                }
                *b = det;
                return 0;
            }
        }
    }

    int MatrixDet(complex<double> **A, complex<double> *b, int rowA, int colA)
    {
        if (rowA < 1 || colA < 1)  // if row or column number less than 1, failed
        {
            return 1;
        }
        else if (rowA != colA) // if row number doesn't equal to column number, failed
        {
            return 2;
        }
        else
        {
            if (rowA == 1)     // one element matrix
            {
                *b = A[0][0];
                return 0;
            }
            else if(rowA == 2) // 2X2 matrix
            {
                *b = A[0][0]*A[1][1] - A[0][1]*A[1][0];
                return 0;
            }
            else if(rowA == 3) // 3X3 matrix
            {
                *b = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
                   - A[0][1] * (A[1][0] * A[2][2] - A[2][0] * A[1][2])
                   + A[0][2] * (A[1][0] * A[2][1] - A[2][0] * A[1][1]);
                return 0;
            }
            else // row and column number > 4
            {
                complex<double> det = complex<double>(0.0,0.0);               // final result to be returned
                for (int m = 0; m < colA; m++)  // each element of the first row multiply with its adjoint matrix
                {
                    complex<double> dElement = A[0][m];   // each element of the first row
                    complex<double> dNewDet = complex<double>(0.0,0.0);        // det of dElement's adjoint matrix
                    // creat a new sized matrix ( rowA - 1, colA - 1 ), adjoint matrix
                    complex<double> **dShrinkA = new complex<double>*[rowA-1];
                    for (int n = 1; n < rowA; n++)
                    {
                        dShrinkA[n-1] = new complex<double>[colA-1];
                        for (int k = 0; k < colA; k++)
                        {
                            if (k < m)
                            {
                                dShrinkA[n-1][k] = A[n][k];
                            }
                            else if (k > m)
                            {
                                dShrinkA[n-1][k-1] = A[n][k];
                            }
                        }
                    }
                    if (MatrixDet(dShrinkA, &dNewDet, rowA-1, colA-1) == 0)
                    {
                          if ((m%2) == 0)
                             det += dElement * dNewDet;
                          else
                             det -= dElement * dNewDet;
                    }
                    delete[] dShrinkA;
                }
                *b = det;
                return 0;
            }
        }
    }

    // B = adjoint(A)
    // @ A: input matrix
    // @ B: output matrix
    // @ rowA: row number of A
    // @ colA: column number of A
    int MatrixCofactor(double **A, double **B, int rowA, int colA)
    {
        if (rowA != colA)   // fail
            return 1;
        if (rowA < 2)       // fail
            return 2;
        else if(rowA == 2)
        {
            B[0][0] = A[1][1];
            B[0][1] = -1.0 * A[1][0];
            B[1][0] = -1.0 * A[0][1];
            B[2][2] = A[0][0];
            return 0;
        }
        else if(rowA >= 3)     // B[i,j] = -1^{i+j}*det{adjoint[A]}
        {
            for (int m = 0; m < rowA; m++)
            {
                for (int n = 0; n < colA; n++)
                {
                    double **dNewA = new double*[rowA-1]; // for each element of A, generate its adjoint matrix
                    for (int l = 0; l < rowA-1; l++)
                    {
                        dNewA[l] = new double[colA-1];
                        for (int k = 0; k < colA-1; k++)
                        {
                            if ((l < m) && (k < n))
                            {
                                dNewA[l][k] = A[l][k];
                            }
                            else if ((l < m) && (k >= n))
                            {
                                dNewA[l][k] = A[l][k+1];
                            }
                            else if ((l >= m) && (k < n))
                            {
                                dNewA[l][k] = A[l+1][k];
                            }
                            else if ((l >= m) && (k >= n))
                            {
                                dNewA[l][k] = A[l+1][k+1];
                            }
                        }
                    }
                    double dDetNewA = 0.0;
                    if(MatrixDet(dNewA, &dDetNewA, rowA-1, colA-1) == 0)
                    {
                        B[n][m] = pow(-1.0,m+n)*dDetNewA; // B = -1^(m+n)Det[adjoint(A)]
                    }
                    delete[] dNewA;
                }
            }
            return 0;
        }
        return 0;
    }

    int MatrixCofactor(complex<double> **A, complex<double> **B, int rowA, int colA)
    {
        if (rowA != colA)   // fail
            return 1;
        if (rowA < 2)       // fail
            return 2;
        else if(rowA == 2)
        {
            B[0][0] = A[1][1];
            B[0][1] = -1.0 * A[1][0];
            B[1][0] = -1.0 * A[0][1];
            B[2][2] = A[0][0];
            return 0;
        }
        else if(rowA >= 3)     // B[i,j] = -1^{i+j}*det{adjoint[A]}
        {
            for (int m = 0; m < rowA; m++)
            {
                for (int n = 0; n < colA; n++)
                {
                    complex<double> **dNewA = new complex<double>*[rowA-1]; // for each element of A, generate its adjoint matrix
                    for (int l = 0; l < rowA-1; l++)
                    {
                        dNewA[l] = new complex<double>[colA-1];
                        for (int k = 0; k < colA-1; k++)
                        {
                            if ((l < m) && (k < n))
                            {
                                dNewA[l][k] = A[l][k];
                            }
                            else if ((l < m) && (k >= n))
                            {
                                dNewA[l][k] = A[l][k+1];
                            }
                            else if ((l >= m) && (k < n))
                            {
                                dNewA[l][k] = A[l+1][k];
                            }
                            else if ((l >= m) && (k >= n))
                            {
                                dNewA[l][k] = A[l+1][k+1];
                            }
                        }
                    }
                    complex<double> dDetNewA = complex<double>(0.0,0.0);
                    if(MatrixDet(dNewA, &dDetNewA, rowA-1, colA-1) == 0)
                    {
                        B[n][m] = pow(-1.0,m+n)*dDetNewA; // B = -1^(m+n)Det[adjoint(A)]
                    }
                    delete[] dNewA;
                }
            }
            return 0;
        }
        return 0;
    }


    // B = inv(A)
    // @A input matrix
    // @B output matrix
    // @rowA: row number of A
    // @colA: column number of A
    int MatrixInv(double **A, double **B, int rowA, int colA)
    {
        if(rowA < 1 || colA < 1) // fail
            return 1;
        else if (rowA != colA)   // if column and row numbers are different, fail
            return 2;
        else
        {
            if (rowA == 1)
            {
                if (A[0][0] == 0.0)
                    return 3;
                else
                    B[0][0] = 1/A[0][0];
                return 0;
            }
            else if (rowA == 2)
            {
                double det = 0.0;
                MatrixDet(A,&det,2,2);
                if (det == 0.0)
                    return 3;
                else
                {
                    B[0][0] = A[1][1] / det;
                    B[0][1] = -1.0 * A[0][1]/det;
                    B[1][0] = -1.0 * A[1][0]/det;
                    B[1][1] = A[0][0] / det;
                    return 0;
                }
            }
            else if (rowA == 3)
            {
                double det = 0.0;
                MatrixDet(A, &det, 3, 3);
                if (det == 0.0)
                    return 3;
                else
                {
                    B[0][0] = (A[1][1] * A[2][2] - A[2][1] * A[1][2]) / det;
                    B[0][1] = -1.0 * (A[0][1] * A[2][2] - A[2][1] * A[0][2]) / det;
                    B[0][2] = (A[0][1] * A[1][2] - A[1][1] * A[0][2]) / det;
                    B[1][0] = -1.0 * (A[1][0] * A[2][2] - A[2][0] * A[1][2]) / det;
                    B[1][1] = (A[0][0] * A[2][2] - A[2][0] * A[0][2]) / det;
                    B[1][2] = -1.0 * (A[0][0] * A[1][2] - A[1][0] * A[0][2]) / det;
                    B[2][0] = (A[1][0] * A[2][1] - A[2][0] * A[1][1]) / det;
                    B[2][1] = -1.0 * (A[0][0] * A[2][1] - A[2][0] * A[0][1]) / det;
                    B[2][2] = (A[0][0] * A[1][1] - A[1][0] * A[0][1]) / det;
                    return 0;
                }
            }
            else
            {
                double **dCoFactorMatrix = new double*[rowA];  // Adjoint Matrix of A
                for (int m = 0; m < rowA; m++)
                    dCoFactorMatrix[m] = new double[colA];
                double dDetA = 0.0;
                MatrixCofactor(A, dCoFactorMatrix, rowA, colA);
                MatrixDet(A, &dDetA, rowA, colA);
                MatrixDotMultiply(dCoFactorMatrix, B, 1/dDetA, rowA, colA);
                delete[] dCoFactorMatrix;
                return 0;
            }

        }
    }

    int MatrixInv(complex<double> **A, complex<double> **B, int rowA, int colA)
    {
        if(rowA < 1 || colA < 1) // fail
            return 1;
        else if (rowA != colA)   // if column and row numbers are different, fail
            return 2;
        else
        {
            if (rowA == 1)
            {
                if (A[0][0] == 0.0)
                    return 3;
                else
                    B[0][0] = complex<double>(1.0,0.0)/A[0][0];
                return 0;
            }
            else if (rowA == 2)
            {
                complex<double> det = 0.0;
                MatrixDet(A,&det,2,2);
                if (det == 0.0)
                    return 3;
                else
                {
                    B[0][0] = A[1][1] / det;
                    B[0][1] = -1.0 * A[0][1]/det;
                    B[1][0] = -1.0 * A[1][0]/det;
                    B[1][1] = A[0][0] / det;
                    return 0;
                }
            }
            else if (rowA == 3)
            {
                complex<double> det = 0.0;
                MatrixDet(A, &det, 3, 3);
                if (det == 0.0)
                    return 3;
                else
                {
                    B[0][0] = (A[1][1] * A[2][2] - A[2][1] * A[1][2]) / det;
                    B[0][1] = -1.0 * (A[0][1] * A[2][2] - A[2][1] * A[0][2]) / det;
                    B[0][2] = (A[0][1] * A[1][2] - A[1][1] * A[0][2]) / det;
                    B[1][0] = -1.0 * (A[1][0] * A[2][2] - A[2][0] * A[1][2]) / det;
                    B[1][1] = (A[0][0] * A[2][2] - A[2][0] * A[0][2]) / det;
                    B[1][2] = -1.0 * (A[0][0] * A[1][2] - A[1][0] * A[0][2]) / det;
                    B[2][0] = (A[1][0] * A[2][1] - A[2][0] * A[1][1]) / det;
                    B[2][1] = -1.0 * (A[0][0] * A[2][1] - A[2][0] * A[0][1]) / det;
                    B[2][2] = (A[0][0] * A[1][1] - A[1][0] * A[0][1]) / det;
                    return 0;
                }
            }
            else
            {
                complex<double> **dCoFactorMatrix = new complex<double>*[rowA];  // Adjoint Matrix of A
                for (int m = 0; m < rowA; m++)
                    dCoFactorMatrix[m] = new complex<double>[colA];
                complex<double> dDetA = complex<double>(0.0,0.0);
                MatrixCofactor(A, dCoFactorMatrix, rowA, colA);
                MatrixDet(A, &dDetA, rowA, colA);
                MatrixDotMultiply(dCoFactorMatrix, B, complex<double>(1.0,0.0)/dDetA, rowA, colA);
                delete[] dCoFactorMatrix;
                return 0;
            }
        }
    }

    // Least square estimation, the model is X*A = Y.
    // Here, X is 2D-Matrix; A is 1-D matrix, the coefficients to be estimated; Y is 1-D matrix.
    // @X: 2D matrix
    // @Y: 2D or 1D matrix
    // @M: number of rows of X, and length of Y
    // @N: number of columns of X
    // @A: output matrix
    bool lse(double** X, double** Y, double** A, int rowX, int colX, int rowY, int colY)
    {
        if (rowX != rowY)
            return false;
        double **tmp1, **tmp2, **tmp3, **tmp4;
        tmp1 = new double*[colX]; tmp2 = new double*[colX]; tmp3 = new double*[colX];
        tmp4 = new double*[colX];
        for (int i = 0; i < colX; i++){
            tmp1[i] = new double[rowX];   // X'
            tmp2[i] = new double[colX];   // X'*X
            tmp3[i] = new double[colY];   // X'*Y
            tmp4[i] = new double[colX];   // inv(X'*X)
        }
        MatrixTranspose(X, tmp1, rowX, colX); // tmp1 = X'
        MatrixMultiply(tmp1, X, tmp2, colX, rowX, rowX, colX); // tmp2 = X'*X
        MatrixMultiply(tmp1, Y, tmp3, colX, rowX, rowY, colY); // tmp3 = X'*Y
        MatrixInv(tmp2, tmp4, colX, colX);  // tmp4 = inv(X'*X);
        MatrixMultiply(tmp4, tmp3, A, colX, colX, colX, colY);
        delete[] tmp1; delete[] tmp2; delete[] tmp3; delete[] tmp4;
        return true;
    }

    /// Histogram   Z = histogram of Y in axis defined by X
    /// @X : axis-X, grids to fill Y
    /// @Y : values to fill in grids
    /// @Z : output result, accumulated number on each grid
    /// @M : length of X
    /// @N : length of Y
    bool hist(double *X, double *Y, double *Z, int M, int N)
    {
        if ((M < 2) || (N < 1))
            return false;
        memset(Z, 0.0, (M+1)*sizeof(double));
        sort(X,M);
        sort(Y,N);
        int nIdX = 0;     // current index of X grid
        for (int i = 0; i < N; i++) // scan through Y
        {
            if (Y[i] < X[0])  // if Y[i] fall into the most left area
            {
                Z[0]++;
            }
            else if (Y[i] >= X[M-1]){   // if Y[i] fall into the most right area
                Z[M] += N-i;           // sum all the left number into Z[M]
                return true;
            }
            else if ((Y[i] >= X[nIdX]) && (Y[i] < X[nIdX+1]))  // if Y[i] fall into {X[nIdx], X[nIdx+1]}
            {
                Z[nIdX+1]++;                                  // this element piles up into this area
            }
            else if (Y[i] >= X[nIdX+1])                        // if Y[i] fall into {X[nIdx+1], ...} area
            {
                nIdX++;                                       // let this element need to compare with next grid
                i--;
            }
        }
        return true;
    }

    /// find out the middle-value of the hist-grid of the maximum hist pile
    /// @X : hist grid array
    /// @Y : middle-value of the grid
    /// @M : length of X
    /// @N : maximum hist ID
    bool hist_edge(double *X, double &Y, int M, int N)
    {
        if (N > M)  // if the maximum ID surpass the range of X
            return false;

        if (N < 1)
        {
            Y = X[0];
        }
        else if (N == M)
        {
            Y = X[M-1];
        }
        else
        {
            Y = 0.5*(X[N - 1] + X[N]);
        }
        return true;
    }
}
