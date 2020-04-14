/* ccsp.cpp    Charley Chang's Signal Processing Functions
  * Author: Charley Chang
  * Created: May 2018
  * Function:  fft -- an implementation of the Cooley-Turkey recursive FFT algorithm
               2dfft --  two dimensional fft
               ifft -- inverse fft
               2difft -- two dimensional IFFT
               fftshift -- move zero frequency to the center of the array
               ifftshift -- move zero time to the center of the array
               fir  -- finite inpulse response dignal filter
  * This is free and you can do anything you want with this code
*/

# include "ccsp.h"
# include "ccmath.h"
# include <memory.h>

namespace __CCSP__ {

  using namespace std;
  using namespace __CCMATH__;

  /// devide input array into two parts, copy even elements into lower-half
  /// and odd elements into upper-half. The length of array must be a power-of-2
  /// @ X: input/output 1-dimensional array
  /// @ N: length of input array
  void devideOddEven(double* X, int N)
  {
     double* Y = new double[N/2]; // temp storage heap
     for (int i = 0; i < N/2; i++) {
         X[i] = X[2*i];          // copy even elements to lower-half
         Y[i] = X[2*i + 1];      // copy odd  elements to temporay heap
     }
     memcpy(X+N/2, Y, N/2*sizeof(double));
     delete[] Y;
     return;
  }

  void devideOddEven(complex<double>* X, int N)
  {
      complex<double>* Y = new complex<double>[N/2]; // temp storage heap
      for (int i = 0; i < N/2; i++) {
        X[i] = X[2*i];          // copy even elements to lower-half
        Y[i] = X[2*i + 1];      // copy odd  elements to temporay heap
      }
      memcpy(X+N/2, Y, N/2*sizeof(complex<double>));
      delete[] Y;
      return;
  }

  /// Append 0.0 to the end of X, until its length is power-of-2
  /// @ X : array, the appended array will also store into it
  /// @ Y : the output result
  /// @ N : length of X
  /// return the length after appending
  int powerof2 (double *X, double *Y, int N)
  {
      double M = log10(N)/log10(2);
      if ( M-floor(M) == 0)
          return N;
      else {  // if N > 1 and not power-of-2, append zero until length is power-of-2
        int L = pow(2,ceil(M));    // the final length
        memset(Y, 0.0 , L*sizeof(double));
        memcpy(Y, X, N*sizeof(double));
        return L;
      }
  }

  int powerof2 (complex<double>* X, complex<double> *Y, int N)
  {
      double M = log10(N)/log10(2);
      if ( M-floor(M) == 0)
          return N;
      else {  // if N > 1 and not power-of-2, append zero
        int L = pow(2,ceil(M));
        memset(Y,0,L*sizeof(complex<double>));
        memcpy(Y, X, N*sizeof(complex<double>));
        return L;
      }
  }

  /// @X :2-D array
  /// @M : rows number
  /// @N : column number
  /// @ bDelete : delete X before renew it or not, true = delete
  /// return: two elements, new rows and new columns
  /// WARNNING: DON't FORGET TO RELEASE the returning memory
  int* powerof2(double **X, double **Y, int M, int N)
  {
      int* L = new int[2];            // the size of array after appendication
      L[0] = M; L[1] = N;
      double K = log10(M)/log10(2);
      if ( K-floor(K) > 0)
          L[0] = pow(2,ceil(K));
      K = log10(N)/log10(2);
      if ( K-floor(K) > 0)
          L[1] = pow(2,ceil(K));

      if (L[0] > M || L[1] > N)
      {
          for(int i = 0; i < M; i++){
              memset(Y[i], 0, L[1]*sizeof(double));
              memcpy(Y[i], X[i], N*sizeof(double));
          }
          for(int i = M; i < L[0]; i++){
              memset(Y[i], 0, L[1]*sizeof(double));
          }
      }
      return L;
  }

  int* powerof2(complex<double>** X, complex<double> **Y, int M, int N)
  {
      int* L = new int[2];
      L[0] = M; L[1] = N;
      double K = log10(M)/log10(2);
      if ( K-floor(K) > 0)
          L[0] = pow(2,ceil(K));
      K = log10(N)/log10(2);
      if ( K-floor(K) > 0)
          L[1] = pow(2,ceil(K));

      if (L[0] > M || L[1] > N)
      {
          for(int i = 0; i < M; i++){
              memset(Y[i], 0, L[1]*sizeof(complex<double>));
              memcpy(Y[i], X[i], N*sizeof(complex<double>));
          }
          for(int i = M; i < L[0]; i++){
              memset(Y[i], 0, L[1]*sizeof(complex<double>));
          }
      }
      return L;
  }

  /// get the frequency range of signal
  /// @ timerange : lasting time of the signal
  /// @ timesamplecount: the count of the samples of the signal in all time-range
  /// return frequency range after FFT (> 0 side)
  double freqrange(int timerange, int timesamplecount)
  {
      return timesamplecount/timerange;
  }

  /// get frequency resolution
  /// @ freqrange : frequency range of the signal (both > 0 & < 0 sides)
  /// @ freqsamplecount: count of samples of the signal in all frequency-range
  /// return frequency resolution
  int freqresolution(int freqrange, int freqsamplecount)
  {
      return freqrange/freqsamplecount;
  }


  /// 1-D FFT of Cooley-Tukey algorithm
  /// @ X:  complex signal with double-precision,
  ///       the first half elements are FFT result, upper-half is a reflection of lower-half
  /// @ N:  length, which must be power-of-2, if not (power-of-2-N) zeros will be appended.
  /// return true = success; false = failed
  /// WARNNING: N must be power-of-2, if not use powerof2() to fix it
  bool fft (complex<double> *X, int N)
  {
      double M = log10(N)/log10(2.0);
      if (M-floor(M) > 0)    // if N is not power-of-2, return
          return false;
      if (N < 2)
          return false;
      else {      // if N > 1 and N is power-of-2, calculate fft
        devideOddEven(X, N);
        fft(X, N/2);
        fft(X+N/2, N/2);
        for (int i = 0; i < N/2; i++) {
          complex<double> E = X[i];
          complex<double> O = X[i+N/2];
          complex<double> W = complex<double>(cos(2*PI*i/N), sin(-2*PI*i/N));
          X[i] = E + W * O;
          X[i+N/2] = E - W * O;
        }
        return true;
      }
  }

  /// 2-D FFT
  /// @ X : 2-D complex signal array
  /// @ M : rows of array
  /// @ N : columns of array
  /// return true = success; false = failed
  /// WARNNING: M, N MUST be power-of-2, if NOT use powerof2() to fix it
  bool fft2d(complex<double> **X, int M, int N)
  {
      // if M != power-of-2, fail
      double L = log10(M)/log10(2);
      if(L-floor(L) > 0)
          return false;
      // if N != power-of-2, fail
      L = log10(N)/log10(2);
      if(L-floor(L) > 0)
          return false;
      // FFT
      for (int i = 0; i < M; i++) { // calculate FFT on each row
         fft(X[i], N);
      }
      for (int i = 0; i < N; i++) { // calculate FFT on each column
         complex<double>* Y = new complex<double> [M];   // temporay heap to store this column elements
         for (int ii = 0; ii < M; ii++) {
            Y[ii] = X[ii][i];
         }
         fft(Y,M);
         for (int ii = 0; ii < M; ii++) {
            X[ii][i] = Y[ii];
         }
         delete[] Y;
      }
      return true;
  }

  /// move 0-frequency to center of the 1-D array
  /// @ X: 1-D array
  /// @ Y: 1-D array, output result
  /// @ N: array length
  void fftshift(complex<double> *X, complex<double> *Y, int N)
  {
      int M;
      if (fmod(N,2) == 1)  // N is odd
      {
          M = (N+1)/2;
      }
      else{  // N is even
          M = N/2;
      }
      memcpy(Y, X+M, (N-M)*sizeof(complex<double>));
      memcpy(Y+N-M, X, M*sizeof(complex<double>));
  }

  /// move 0-frequency to center of the 2-D array
  /// @ M : rows of array
  /// @ N : columns of array
  void fftshift(complex<double> **X, complex<double> **Y, int M, int N)
  {
      for (int i = 0; i < M; i++) { // calculate FFT on each row
         fftshift(X[i], Y[i], N);
      }
      complex<double>* Z = new complex<double> [M];   // temporay heap to store this column elements
      complex<double>* Q = new complex<double> [M];
      for (int i = 0; i < N; i++) { // calculate FFT on each column
         for (int ii = 0; ii < M; ii++) {
            Z[ii] = Y[ii][i];
         }
         fftshift(Z, Q, M);
         for (int ii = 0; ii < M; ii++) {
            Y[ii][i] = Q[ii];
         }
      }
      delete[] Z; delete[] Q;
  }

  /// IFFT of 1-D array
  /// @X  1-D complex signal array, and also the calculated IFFT result
  /// @N  length of array
  /// return false == failed; true = successful
  /// WARNNING: N MUST be power-of-2, if not use powerof2() to fix
  bool ifft(complex<double> *X, int N)
  {
      double M = log10(N)/log10(2.0);
      if (M-floor(M) > 0)    // if N is not power-of-2, return
          return false;
      reverse(X+1, N-1);
      fft(X, N);
      for(int i = 0; i < N; i++){
          X[i] = X[i]/double(N);
      }
      return true;
  }


  /// IFFT of 2-D array
  /// @ X : input and output array
  /// @ M : rows of array
  /// @ N : columns of array
  /// return true = successful; false == failed
  /// WARNNING: M, N MUST be power-of-2, if NOT use powerof2() to fix it
  bool ifft2d(complex<double> **X, int M, int N)
  {
      // if M != power-of-2, fail
      double L = log10(M)/log10(2);
      if(L-floor(L) > 0)
          return false;
      // if N != power-of-2, fail
      L = log10(N)/log10(2);
      if(L-floor(L) > 0)
          return false;
      // FFT
      for (int i = 0; i < M; i++) { // calculate FFT on each row
         ifft(X[i], N);
      }
      for (int i = 0; i < N; i++) { // calculate FFT on each column
         complex<double>* Y = new complex<double> [M];   // temporay heap to store this column elements
         for (int ii = 0; ii < M; ii++) {
            Y[ii] = X[ii][i];
         }
         ifft(Y,M);
         for (int ii = 0; ii < M; ii++) {
            X[ii][i] = Y[ii];
         }
         delete[] Y;
      }
      return true;
  }

  /// move 0-time to center of the 1-D array
  /// @ X: 1-D array
  /// @ Y: output
  /// @ N: array length
  void ifftshift(complex<double> *X, complex<double> *Y, int N)
  {
      fftshift(X, Y, N);
  }

  /// move 0-frequency to center of the 2-D array
  /// @ M : rows of array
  /// @ N : columns of array
  void ifftshift(complex<double>** X, complex<double> **Y, int M, int N)
  {
      fftshift(X, Y, M, N);
  }

  /// FIR filter
  /// @ coefs: coefficents of FIR filter
  /// @ X: input and output signal array
  /// @ Y: filter output, length of Y = N
  /// @ M: length of filter coefficents
  /// @ N: length of signal
  void fir(double *coefs, double *X, double *Y, int M, int N)
  {
      memset(Y,0.0, N*sizeof(double));
	  double *Buffer = new double[M+N];  // heap to store temporay signal
      memset(Buffer,0.0, (M+N)*sizeof(double));
      memcpy(Buffer+M-1, X, N*sizeof(double));

	  for(int i = 0; i < N; i++){
          for (int ii = 0; ii < M; ii++){
              Y[i] += coefs[ii] * Buffer[i+M-1-ii];  // filter algorithm
          }
      }
	  delete[] Buffer;
  }

  void fir(complex<double> *coefs, complex<double> *X, complex<double> *Y, int M, int N)
  {
      memset(Y,0.0, N*sizeof(complex<double>));
      complex<double>* Buffer = new complex<double>[M+N];          // heap to store input
      memset(Buffer,0.0, (M+N)*sizeof(complex<double>));
      memcpy(Buffer+M-1, X, N*sizeof(complex<double>));

      for(int i = 0; i < N; i++){
          for (int ii = 0; ii < M; ii++){
              Y[i] += coefs[ii] * Buffer[i+M-1-ii];  // filter algorithm
          }
      }
      delete[] Buffer;
  }

  /// FIR filter for 2-dimensional array
  /// @ coefs : coefficents of FIR filter
  /// @ X: 2-D array
  /// @ Y: filter output, size of Y = (M,N)
  /// @ L: filter length
  /// @ M: rows of signal
  /// @ N: columns of signal
//  void fir2d(double *coefs, double **X, double **Y, int L, int M, int N)
//  {
//      for (int i = 0; i < M; i++) { // calculate FIR on each row
//         fir(coefs, X[i], Y[i], L, N);
//      }
//      for (int i = 0; i < N; i++) { // calculate FFT on each column
//         double *Z = new double [M];   // temporay heap to store this column elements
//         double *Buffer = new double[M];  // temporary heap to store filter output
//         for (int ii = 0; ii < M; ii++) {
//            Z[ii] = Y[ii][i];
//         }
//         fir(coefs, Z, Buffer, L, M);  // filter on column elements
//         for (int ii = 0; ii < M; ii++) {
//            Y[ii][i] = Buffer[ii];
//         }
//         delete[] Z; delete[] Buffer;
//      }
//  }

//  void fir2d(complex<double> *coefs, complex<double> **X, complex<double> **Y, int L, int M, int N)
//  {
//      for (int i = 0; i < M; i++) { // calculate FIR on each row
//         fir(coefs, X[i], Y[i], L, N);
//      }
//      for (int i = 0; i < N; i++) { // calculate FFT on each column
//         complex<double> *Z = new complex<double> [M];   // temporay heap to store this column elements
//         complex<double> *Buffer = new complex<double>[M];  // temporary heap to store filter output
//         for (int ii = 0; ii < M; ii++) {
//            Z[ii] = Y[ii][i];
//         }
//         fir(coefs, Z, Buffer, L, M);  // filter on column elements
//         for (int ii = 0; ii < M; ii++) {
//            Y[ii][i] = Buffer[ii];
//         }
//         delete[] Z; delete[] Buffer;
//      }
//  }

}
