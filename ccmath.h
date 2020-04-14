/* ccmath.h    basic mathematic functions
  * Author: Charley Chang
  * Created: May 2018
  * This is free and you can do anything you want with this code
*/
 #ifndef CCMATH_H
#define CCMATH_H
# include <complex>

namespace __CCMATH__ {
  using namespace std;

  double abs2(complex<double> X);          // abs of a complex double number
  complex<double> multi(complex<double> X, complex<double> Y);   // mulitiply between two complex numbers
  int max(double *X, double &y, int N);              // find the maximum element in 1-D array
  int max(complex<double>* X, complex<double> &y, int N) ;
  int* max(double** X, double &y, int M, int N);     // find the maximum element in 2-D array
  int* max(complex<double>** X, complex<double> &y, int M, int N);

  int min(double* X, double &y, int N);              // find the minimum element in 1-D array
  int min(complex<double>* X, complex<double> &y, int N);
  int* min(double** X, double &y, int M, int N);      // find the minimum element in 2-D array
  int* min(complex<double>** X, complex<double> &y, int M, int N);

  void sort(double* X, int N, int Type = 0);                 // sort 1-D array
  void sort(complex<double>* X, int N, int Type = 0);
  void sort(double** X, int M, int N, int Type = 0);         // sort 2-D array
  void sort(complex<double>** X, int M, int N, int Type = 0);

  double mean(double* X, int N);   // mean value of an array
  complex<double> mean(complex<double>* X, int N);

  bool equal(const double &a, const double &b); // equal of complex type value
  bool equal(const complex<double> &a, const complex<double> &b);

  void reverse(double *X, int N);   // left-right flip of an array
  void reverse(complex<double> *X, int N);

  bool diff(double *X, double *Y, int M, int N, int Type = 0);    // difference between two N-distance elements of an array
  bool cross(double *X, double **Y, double **Z, int **Y_ID, int **Z_ID, double V, int M, int &N, int Type = 0);     // compare an array with a fix value, find out ID and values of two neighboring elements those acrossing the fix value

  int xcorr(double *X, double *Y, int M, int N, double **Z); // correlation between X & Y
  int xcorr(complex<double> *X, complex<double> *Y, int M, int N, complex<double> **Z);

  // Matrix Calculation
  int MatrixAdd(double **A, double **B, double **C, int row, int col); // C=A+B
  int MatrixAdd(complex<double> **A, complex<double> **B, complex<double> **C, int row, int col); // C=A+B
  int MatrixSub(double **A, double **B, double **C, int row, int col); // C=A-B
  int MatrixSub(complex<double> **A, complex<double> **B, complex<double> **C, int row, int col); // C=A-B

  int MatrixTranspose(double **A, double **B, int row, int col); // B=A'
  int MatrixTranspose(complex<double> **A, complex<double> **B, int row, int col); // B=A's
  int MatrixMultiply(double **A, double **B, double **C, int rowA, int colA, int rowB, int colB); // C=A*B
  int MatrixMultiply(complex<double> **A, complex<double> **B, complex<double> **C, int rowA, int colA, int rowB, int colB); // C=A*B
  int MatrixDotMultiply(double **A, double **B, double a, int rowA, int colA); // B=a*A
  int MatrixDotMultiply(complex<double> **A, complex<double> **B, complex<double> a, int rowA, int colA);
  int MatrixDet(double **A, double *b, int rowA, int colA); // b = det(A)
  int MatrixDet(complex<double> **A, complex<double> *b, int rowA, int colA);
  int MatrixCofactor(double **A, double **B, int rowA, int colA); // B = adjoint matrix of A
  int MatrixCofactor(complex<double> **A, complex<double> **B, int rowA, int colA);
  int MatrixInv(double **A, double **B, int rowA, int colA); // B = inv(A)
  int MatrixInv(complex<double> **A, complex<double> **B, int rowA, int colA);

  // statistic calculation
  bool lse(double** X, double** Y, double** A, int rowX, int colX, int rowY, int colY);  // least square estimation
  bool hist(double *X, double *Y, double *Z, int M, int N);  // Z = histogram of Y in axis X
  bool hist_edge(double *X, double &Y, int M, int N);  // find out hist-edge-value
}

#endif //CCMATH_H
