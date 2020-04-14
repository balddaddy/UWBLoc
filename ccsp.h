/* ccsp.h    Signal Processing Functions
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
  *
  * WARNNING: by far, ONLY 1-D array functions have been tested
*/
#ifndef CCSP_AEROHUNT_H
#define CCSP_AEROHUNT_H

# include <complex>
# include <cstring>
# include <math.h>

namespace __CCSP__ {
  using namespace std;

  #define PI           3.14159265358979323846      // Pi

  void devideOddEven(double* X, int N) ;  // devide input array into even and odd ones
  void devideOddEven(complex<double>* X, int N) ;  // devide input array into even and odd ones

  int powerof2(double *X, double *Y, int N);          // if X is not power-of-2, append 0.0 at the end of X to the length of power-of-2
  int powerof2 (complex<double>* X, complex<double> *Y, int N);
  int* powerof2(double **X, double **Y, int M, int N);
  int* powerof2(complex<double>** X, complex<double> **Y, int M, int N);

  double freqrange(int timerange, int timesamplecount);   // get the frequency range of signal
  int freqresolution(int freqrange, int freqsamplecount); // get frequency resolution

  bool fft(complex<double> *X, int N); // faster fourier transform

  bool fft2d(complex<double> **X, int M, int N); // 2-dimensional FFT

  void fftshift(complex<double> *X, complex<double> *Y, int Ms); // move 0-frequency to center

  void fftshift(complex<double> **X, complex<double> **Y, int M, int N);   // 2-d fftshift

  bool ifft(complex<double> *X, int N); // Inverse Fast fourier transform

  bool ifft2d(complex<double> **X, int M, int N); // 2-dimensional IFFT

  void ifftshift(complex<double> *X, complex<double> *Y, int M); // move 0-time to center

  void ifftshift(complex<double> **X, complex<double> **Y, int M, int N);   // 2-d ifftshift

  void fir(double *coefs, double *X, double *Y, int M, int N); // FIR dignal filter
  void fir(complex<double> *coefs, complex<double> *X, complex<double> *Y, int M, int N); // FIR dignal filter

//  void fir2d(double *coefs, double **X, double **Y, int L, int M, int N); // 2-D FIR filter
//  void fir2d(complex<double>* coefs, complex<double>** X, complex<double> **Y, int L, int M, int N);
}

#endif // CCSP_AEROHUNT_H