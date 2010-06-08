# include <cstdlib>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <complex>

using namespace std;

# include "subpak.H"

//****************************************************************************80

double angle_shift ( double alpha, double beta )

//****************************************************************************80
//
//  Purpose:
//
//    ANGLE_SHIFT shifts angle ALPHA to lie between BETA and BETA+2PI.
//
//  Discussion:
//
//    The input angle ALPHA is shifted by multiples of 2 * PI to lie
//    between BETA and BETA+2*PI.
//
//    The resulting angle GAMMA has all the same trigonometric function
//    values as ALPHA.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double ALPHA, the angle to be shifted.
//
//    Input, double BETA, defines the lower endpoint of
//    the angle range.
//
//    Output, double ANGLE_SHIFT, the shifted angle.
//
{
  double gamma;
  double pi = 3.141592653589793;

  if ( alpha < beta )
  {
    gamma = beta - fmod (  beta - alpha, 2.0 * pi ) + 2.0 * pi;
  }
  else
  {
    gamma = beta + fmod ( alpha - beta, 2.0 * pi );
  }

  return gamma;
}
//****************************************************************************80

double angle_shift_deg ( double alpha, double beta )

//****************************************************************************80
//
//  Purpose:
//
//    ANGLE_SHIFT_DEG shifts angle ALPHA to lie between BETA and BETA+360.
//
//  Discussion:
//
//    The input angle ALPHA is shifted by multiples of 360 to lie
//    between BETA and BETA+360.
//
//    The resulting angle GAMMA has all the same trigonometric function
//    values as ALPHA.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double ALPHA, the angle to be shifted.
//
//    Input, double BETA, defines the lower endpoint of
//    the angle range.
//
//    Output, double ANGLE_SHIFT, the shifted angle.
//
{
  double gamma;

  if ( alpha < beta )
  {
    gamma = beta - fmod ( beta - alpha, 360.0 ) + 360.0;
  }
  else
  {
    gamma = beta + fmod ( alpha - beta, 360.0 );
  }

  return gamma;
}
//****************************************************************************80

double *angle_to_rgb ( double angle )

//****************************************************************************80
//
//  Purpose:
//
//    ANGLE_TO_RGB returns a color on the perimeter of the color hexagon.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double ANGLE, the angle in the color hexagon.  The sextants are
//    defined by the following points:
//        0 degrees, 1, 0, 0, red;
//       60 degrees, 1, 1, 0, yellow;
//      120 degrees, 0, 1, 0, green;
//      180 degrees, 0, 1, 1, cyan;
//      240 degrees, 0, 0, 1, blue;
//      300 degrees, 1, 0, 1, magenta.
//
//    Output, double ANGLE_TO_RGB[3], the RGB specifications for the 
//    color that lies at the given angle, on the perimeter of the 
//    color hexagon.  One value will be 1, and one value will be 0.
//
{
# define DEGREES_TO_RADIANS ( 3.141592653589793 / 180.0 )

  double angle2;
  double *rgb;

  rgb = new double[3];

  angle = r8_modp ( angle, 360.0 );

  if ( angle <= 60.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * angle / 4.0;
    rgb[0] = 1.0;
    rgb[1] = tan ( angle2 );
    rgb[2] = 0.0;
  }
  else if ( angle <= 120.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * angle / 4.0;
    rgb[0] = cos ( angle2 ) / sin ( angle2 );
    rgb[1] = 1.0;
    rgb[2] = 0.0;
  }
  else if ( angle <= 180.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * ( angle - 120.0 ) / 4.0;
    rgb[0] = 0.0;
    rgb[1] = 1.0;
    rgb[2] = tan ( angle2 );
  }
  else if ( angle <= 240.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * ( angle - 120.0 ) / 4.0;
    rgb[0] = 0.0;
    rgb[1] = cos ( angle2 ) / sin ( angle2 );
    rgb[2] = 1.0;
  }
  else if ( angle <= 300.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * ( angle - 240.0 ) / 4.0;
    rgb[0] = tan ( angle2 );
    rgb[1] = 0.0;
    rgb[2] = 1.0;
  }
  else if ( angle <= 360.0 )
  {
    angle2 = DEGREES_TO_RADIANS * 3.0 * ( angle - 240.0 ) / 4.0;
    rgb[0] = 1.0;
    rgb[1] = 0.0;
    rgb[2] = cos ( angle2 ) / sin ( angle2 );
  }

  return rgb;
# undef DEGREES_TO_RADIANS
}
//****************************************************************************80

void axis_limits ( double xmin, double xmax, int ndivs, double *pxmin, 
  double *pxmax, double *pxdiv, int *nticks )

//****************************************************************************80
//
//  Purpose:
//
//    AXIS_LIMITS returns "nice" axis limits for a plot.
//
//  Discussion:
//
//    The routine is given information about the range of a variable, and
//    the number of divisions desired.  It returns suggestions for
//    labeling a plotting axis for the variable, including the
//    starting and ending points, the length of a single division,
//    and a suggested tick marking for the axis.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double XMIN, XMAX, the lower and upper values that must be
//    included on the axis.  XMIN must be less than XMAX.
//
//    Input, int NDIVS, the number of divisions desired along
//    the axis.
//
//    Output, double *PXMIN, *PXMAX, the recommended lower and upper axis
//    bounds.  It will be the case that PXMIN <= XMIN < XMAX <= PXMAX.
//
//    Output, double *PXDIV, the recommended size of a single division.
//
//    Output, int *NTICKS, a suggested number of ticks to use,
//    if subdividing each of the NDIVS divisions of the axis.
//
{
# define NSTEPS 5

  double best;
  double good;
  int i;
  int ihi;
  int ilo;
  int intlog;
  int iticks[NSTEPS] = { 5, 4, 4, 5, 5 };
  int ival;
  int j;
  double pxmax2;
  double pxmin2;
  double pxdiv2;
  double reldif;
  double steps[NSTEPS] = { 1.0,  2.0,  4.0,  5.0, 10.0 };
  double temp;

  if ( xmin == xmax )
  {
    xmin = xmin - 0.5;
    xmax = xmax + 0.5;
  }
  else if ( xmax < xmin )
  {
    temp = xmin;
    xmin = xmax;
    xmax = temp;
  }

  if ( ndivs <= 0 )
  {
    ndivs = 5;
  }
//
//  Set RELDIF, the size of the X interval divided by the largest X.
//
  if ( xmax != xmin )
  {
    reldif = ( xmax - xmin ) / r8_max ( r8_abs ( xmax ), r8_abs ( xmin ) );
  }
  else
  {
    reldif = 0.0;
  }
//
//  If RELDIF tells us that XMIN and XMAX are extremely close,
//  do some simple things.
//
  if ( reldif < 0.00001 )
  {
    if ( xmax == 0.0 )
    {
      *pxdiv = 1.0;
    }
    else
    {
      intlog = ( int ) ( r8_log_10 ( xmax ) );

      if ( intlog < 0 )
      {
        intlog = intlog - 1;
      }

      *pxdiv = pow ( 10.0, intlog );

      if ( 1.0 < *pxdiv )
      {
        *pxdiv = 1.0;
      }

    }

    *nticks = 5;
    *pxmin = xmax - ( double ) ( ndivs / 2 ) * (*pxdiv);
    *pxmax = xmax + ( double ) ( ndivs - ( ndivs / 2 ) ) * (*pxdiv);
  }
//
//  But now handle the more general case, when XMIN and XMAX
//  are relatively far apart.
//
  else
  {
    best = -999.0;
//
//  On second loop, increase INTLOG by 1.
//
    for ( j = 1; j <= 2; j++ )
    {
//
//  Compute INTLOG, roughly the logarithm base 10 of the range
//  divided by the number of divisions.
//
      intlog = ( int ) ( r8_log_10 ( ( xmax - xmin ) / ( double ) ( ndivs ) ) ) 
        + ( j - 1 );

      if ( xmax - xmin  < ( double ) ( ndivs ) )
      {
        intlog = intlog - 1;
      }
//
//  Now consider taking 1, 2, 4, 5 or 10 steps of size 10**INTLOG:
//
      for ( i = 1; i <= NSTEPS; i++ )
      {
//
//  Compute the size of each step.
//
        pxdiv2 = steps[i-1] * pow ( 10.0, intlog );
//
//  Make sure NDIVS steps can reach from XMIN to XMAX, at least.
//
        if ( xmax <= xmin + ndivs * pxdiv2 )
        {
//
//  Now decide where to start the axis.
//  Start the axis at PXMIN2, to the left of XMIN, and
//  representing a whole number of steps of size PXDIV2.
//
          if ( 0.0 <= xmin )
          {
            ival = ( int ) ( xmin / pxdiv2 );
          }
          else
          {
            ival = ( int ) ( xmin / pxdiv2 ) - 1;
          }

          pxmin2 = ival * pxdiv2;
//
//  PXMAX2 is, of course, NDIVS steps above PXMIN2.
//
          pxmax2 = pxmin2 + ndivs * pxdiv2;
//
//  Only consider going on if PXMAX2 is at least XMAX.
//
          if ( xmax <= pxmax2 )
          {
//
//  Now judge this grid by the relative amount of wasted axis length.
//
            good = ( xmax - xmin ) / ( pxmax2 - pxmin2 );

            if ( best < good )
            {
              best = good;
              *pxmax = pxmax2;
              *pxmin = pxmin2;
              *pxdiv = pxdiv2;
              *nticks = iticks[i-1];
            }
          }
        }
      }
    }
  }
//
//  If necessary, adjust the locations of PXMIN and PXMAX so that the
//  interval is more symmetric in containing XMIN through XMAX.
//
  for ( ; ; )
  {
    ilo = ( int ) ( ( xmin - *pxmin ) / (*pxdiv) );
    ihi = ( int ) ( ( *pxmax - xmax ) / (*pxdiv) );

    if ( ihi < ilo + 2 )
    {
      break;
    }

    *pxmin = *pxmin - (*pxdiv);
    *pxmax = *pxmax - (*pxdiv);

  }

  return;
# undef NSTEPS
}
//****************************************************************************80

int bar_check ( int digit[12] )

//****************************************************************************80
//
//  Purpose:
//
//    BAR_CHECK computes the check digit for a barcode.
//
//  Formula:
//
//    CHECK = SUM ( I = 1, 11, by 2's ) DIGIT(I)
//       + 3 * SUM ( I = 2, 10, by 2's ) DIGIT(I)
//
//    CHECK = MOD ( 10 - MOD ( CHECK, 10 ), 10 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIGIT[12], entries 1 through 11 of DIGIT contain
//    the digits of the bar code.  Each entry must be between 0 and 9.
//    The 12th digit should be the check digit.
//
//    Output, int BAR_CHECK, the correct check digit.  If the bar code
//    is correct, then DIGIT(12) should equal BAR_CHECK.
//
{
  int check;

  check =  
          ( digit[0] + digit[2] + digit[4] + digit[6] + digit[8] + digit[10] )
    + 3 * ( digit[1] + digit[3] + digit[5] + digit[7] + digit[9] );

  check =  ( 10 - ( check % 10 ) ) % 10;

  return check;
}
//****************************************************************************80

char *bar_code ( int digit[] )

//****************************************************************************80
//
//  Purpose:
//
//    BAR_CODE constructs the 113 character barcode from 11 digits.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int DIGIT(12).
//    On input, the first 11 entries of DIGIT contain a code to be
//    turned into a barcode.
//    On output, the 12-th entry of DIGIT is a check digit.
//
//    Output, char BAR_CODE[114], the 113 character bar code corresponding 
//    to the digit information.
//
{
  char *bar;
  int check;
  char *codel;
  char *coder;
  int i;

  bar = new char[114];
//
//  9 character quiet zone.
//
  strcpy ( bar, "000000000" );
//
//  3 character guard pattern.
//
  strcpy ( bar+9, "101" );
//
//  7 character product category.
//
  codel = bar_digit_code_left ( digit[0] );
  strcpy ( bar+12, codel );
  delete [] codel;
//
//  35 characters contain the 5 digit manufacturer code.
//
  for ( i = 0; i < 5; i++ )
  {
    codel = bar_digit_code_left ( digit[i+1] );
    strcpy ( bar+19+i*7, codel );
    delete [] codel;
  }
//
//  Center guard pattern.
//
  strcpy ( bar+54, "01010" );
//
//  35 characters contain the 5 digit product code.
//
  for ( i = 0; i < 5; i++ )
  {
    coder = bar_digit_code_right ( digit[i+6] );
    strcpy ( bar+59+i*7, coder );
    delete [] coder;
  }
//
//  Compute the check digit.
//
  check = bar_check ( digit );
  digit[11] = check;

  coder = bar_digit_code_right ( check );
  strcpy ( bar+94, coder );
//
//  Guard pattern.
//
  strcpy ( bar+101, "101" );
//
//  Quiet zone.
//
  strcpy ( bar+104, "000000000" );
  bar[113] = '\0';

  return bar;
}
//****************************************************************************80

char *bar_digit_code_left ( int digit )

//****************************************************************************80
//
//  Purpose:
//
//    BAR_DIGIT_CODE_LEFT returns the 7 character left bar code for a digit.
//
//  Example:
//
//    DIGIT = 3
//    CODEL = '0111101'
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIGIT, the digit, between 0 and 9.
//
//    Output, char BAR_CODE_DIGIT_LEFT[8], the 7 character left code for the digit.
//
{
  char *codel;
  int i;

  codel = new char[8];

  if ( digit == 0 )
  {
    strcpy ( codel, "0001101" );
  }
  else if ( digit == 1 )
  {
    strcpy ( codel, "0011001" );
  }
  else if ( digit == 2 )
  {
    strcpy ( codel, "0010011" );
  }
  else if ( digit == 3 )
  {
    strcpy ( codel, "0111101" );
  }
  else if ( digit == 4 )
  {
    strcpy ( codel, "0100011" );
  }
  else if ( digit == 5 )
  {
    strcpy ( codel, "0110001" );
  }
  else if ( digit == 6 )
  {
    strcpy ( codel, "0101111" );
  }
  else if ( digit == 7 )
  {
    strcpy ( codel, "0111011" );
  }
  else if ( digit == 8 )
  {
    strcpy ( codel, "0110111" );
  }
  else if ( digit == 9 )
  {
    strcpy ( codel, "0001011" );
  }
  else
  {
    strcpy ( codel, "???????" );
  }

  return codel;
}
//****************************************************************************80

char *bar_digit_code_right ( int digit )

//****************************************************************************80
//
//  Purpose:
//
//    BAR_DIGIT_CODE_RIGHT returns the 7 character right bar code for a digit.
//
//  Example:
//
//    DIGIT = 3
//    CODER = "1000010"
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIGIT, the digit, between 0 and 9.
//
//    Output, char BAR_DIGIT_CODE_RIGHT[8], the 7 character right code.
//
{
  char *coder;
  int i;

  coder = new char[8];

  if ( digit == 0 )
  {
    strcpy ( coder, "1110010" );
  }
  else if ( digit == 1 )
  {
    strcpy ( coder, "1100110" );
  }
  else if ( digit == 2 )
  {
    strcpy ( coder, "1101100" );
  }
  else if ( digit == 3 )
  {
    strcpy ( coder, "1000010" );
  }
  else if ( digit == 4 )
  {
    strcpy ( coder, "1011100" );
  }
  else if ( digit == 5 )
  {
    strcpy ( coder, "1001110" );
  }
  else if ( digit == 6 )
  {
    strcpy ( coder, "1010000" );
  }
  else if ( digit == 7 )
  {
    strcpy ( coder, "1000100" );
  }
  else if ( digit == 8 )
  {
    strcpy ( coder, "1001000" );
  }
  else if ( digit == 9 )
  {
    strcpy ( coder, "1110100" );
  }
  else
  {
    strcpy ( coder, "???????" );
  }

  return coder;
}
//****************************************************************************80

double bmi_english ( double w_lb, double h_ft, double h_in )

//****************************************************************************80
//
//  Purpose:
//
//    BMI_ENGLISH computes the body mass index given English measurements.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double W_LB, the body weight in pounds.
//
//    Input, double H_FT, H_IN, the body height in feet and inches
//
//    Output, double BMI_ENGLISH, the body mass index.
//
{
  double h_m;
  double value;
  double w_kg;

  w_kg = pounds_to_kilograms ( w_lb );

  h_m = feet_to_meters ( h_ft + ( h_in / 12.0 ) );

  value = bmi_metric ( w_kg, h_m );

  return value;
}
//****************************************************************************80

double bmi_metric ( double w_kg, double h_m )

//****************************************************************************80
//
//  Purpose:
//
//    BMI_METRIC computes the body mass index given metric measurements.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double W_KG, the body weight in kilograms.
//
//    Input, double H_M, the body height in meters.
//
//    Output, double BMI_METRIC, the body mass index.
//
{
  double value;

  value = w_kg / ( h_m * h_m );

  return value;
}
//****************************************************************************80

float c4_abs ( complex <float> x )

//****************************************************************************80
//
//  Purpose:
//
//    C4_ABS returns the absolute value of a C4.
//
//  Discussion:
//
//    A C4 is a single precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <float> X, the value whose norm is desired.
//
//    Output, float C4_ABS, the magnitude of X.
//
{
  float value;

  value = sqrt ( pow ( real ( x ), 2 ) + pow ( imag ( x ), 2 ) );

  return value;
}
//****************************************************************************80

float c4_argument ( complex <float> x )

//****************************************************************************80
//
//  Purpose:
//
//    C4_ARGUMENT returns the argument of a C4.
//
//  Discussion:
//
//    A C4 is a single precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <float> X, the value whose argument is desired.
//
//    Output, float C4_ARGUMENT, the argument of X.
//
{
  float value;

  if ( imag ( x ) == 0.0 && real ( x ) == 0.0 )
  {
    value = 0.0;
  }
  else
  {
    value = atan2 ( imag ( x ), real ( x ) );
  }

  return value;
}
//****************************************************************************80

float c4_magnitude ( complex <float> x )

//****************************************************************************80
//
//  Purpose:
//
//    C4_MAGNITUDE returns the magnitude of a C4.
//
//  Discussion:
//
//    A C4 is a single precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <float> X, the value whose norm is desired.
//
//    Output, float C4_MAGNITUDE, the magnitude of X.
//
{
  float value;

  value = sqrt ( pow ( real ( x ), 2 ) + pow ( imag ( x ), 2 ) );

  return value;
}
//****************************************************************************80

void c4mat_print_some ( int m, int n, complex <float> a[], int ilo, int jlo, 
  int ihi, int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C4MAT_PRINT_SOME prints some of a C4MAT.
//
//  Discussion:
//
//    A C4MAT is a matrix of single precision complex values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the matrix.
//
//    Input, complex <float> A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, the first row and
//    column, and the last row and column to be printed.
//
//    Input, char *TITLE, a title to be printed.
//
{
  complex <float> c;
  int i;
  int i2hi;
  int i2lo;
  int inc;
  int incx = 4;
  int j;
  int j2;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }
//
//  Print the columns of the matrix, in strips of INCX.
//
  for ( j2lo = jlo; j2lo <= jhi; j2lo = j2lo + incx )
  {
    j2hi = j2lo + incx - 1;
    j2hi = i4_min ( j2hi, n );
    j2hi = i4_min ( j2hi, jhi );

    inc = j2hi + 1 - j2lo;

    cout << "\n";
    cout << "  Col: ";
    for ( j = j2lo; j <= j2hi; j++ )
    {
      j2 = j + 1 - j2lo;
      cout << "          " << setw(10) << j;
    }
    cout << "\n";
    cout << "  Row\n";
    cout << "  ---\n";
//
//  Determine the range of the rows in this strip.
//
    i2lo = i4_max ( ilo, 1 );
    i2hi = i4_min ( ihi, m );

    for ( i = i2lo; i <= i2hi; i++ )
    {
//
//  Print out (up to) INCX entries in row I, that lie in the current strip.
//
      for ( j2 = 1; j2 <= inc; j2++ )
      {
        j = j2lo - 1 + j2;
        c = a[i-1+(j-1)*m];
        cout << "  " << setw(8) << real ( c )
             << "  " << setw(8) << imag ( c );
      }
      cout << "\n";
    }
  }
  cout << "\n";

  return;
}
//****************************************************************************80

void c4vec_print_some ( int n, complex <float> a[], int i_lo, int i_hi, 
  char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C4VEC_PRINT_SOME prints some of a C4VEC.
//
//  Discussion:
//
//    A C4VEC is a vector of single precision complex values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, complex <float> A[N], the vector to be printed.
//
//    Input, int I_LO, I_HI, the first and last entries to print.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = i4_max ( 0, i_lo ); i <= i4_min ( i_hi, n - 1 ); i++ )
  {
    cout << "  " << setw(6) << i+1
         << "  " << real ( a[i] )
         << "  " << imag ( a[i] ) << "\n";
  }

  return;
}
//****************************************************************************80

double c8_argument ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_ARGUMENT returns the argument of a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the value whose argument is desired.
//
//    Output, double C8_ARGUMENT, the argument of X.
//
{
  double value;

  if ( imag ( x ) == 0.0 && real ( x ) == 0.0 )
  {
    value = 0.0;
  }
  else
  {
    value = atan2 ( imag ( x ), real ( x ) );
  }

  return value;
}
//****************************************************************************80

complex <double> c8_cube_root ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_CUBE_ROOT returns the principal cube root of a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the number whose cube root is desired.
//
//    Output, complex <double> C8_CUBE_ROOT, the cube root of X.
//
{
  double argument;
  double magnitude;
  complex <double> value;

  argument = c8_argument ( x );
  magnitude = c8_magnitude ( x );

  if ( magnitude == 0.0 )
  {
    value = complex <double> ( 0.0, 0.0 );
  }
  else
  {
    value = pow ( magnitude, 1.0 / 3.0 ) 
      * complex <double> ( cos ( argument / 3.0 ), sin ( argument / 3.0 ) );
  }

  return value;
}
//****************************************************************************80

complex <double> c8_i ( )

//****************************************************************************80
//
//  Purpose:
//
//    C8_I returns the value of the imaginary unit, i as a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, complex <double> C8_I, the value of complex i.
//
{
  complex <double> value;

  value = complex <double> ( 0.0, 1.0 );

  return value;
}
//****************************************************************************80

bool c8_le_l1 ( complex <double> x, complex <double> y )

//****************************************************************************80
//
//  Purpose:
//
//    C8_LE_L1 := X <= Y for C8 values, and the L1 norm.
//
//  Discussion:
//
//    A C8 is a complex double precision value.
//
//    The L1 norm can be defined here as:
//
//      C8_NORM_L1(X) = abs ( real (X) ) + abs ( imag (X) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, Y, the values to be compared.
//
//    Output, bool C8_LE_L1, is TRUE if X <= Y.
//
{
  bool value;

  if ( r8_abs ( real ( x ) ) + r8_abs ( imag ( x ) ) <= 
       r8_abs ( real ( y ) ) + r8_abs ( imag ( y ) ) )
  {
    value = true;
  }
  else
  {
    value = false;
  }
  return value;
}
//****************************************************************************80

bool c8_le_l2 ( complex <double> x, complex <double> y )

//****************************************************************************80
//
//  Purpose:
//
//    C8_LE_L2 := X <= Y for C8 values, and the L2 norm.
//
//  Discussion:
//
//    A C8 is a complex double precision value.
//
//    The L2 norm can be defined here as:
//
//      C8_NORM_L2(X) = sqrt ( ( real (X) )^2 + ( imag (X) )^2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, Y, the values to be compared.
//
//    Output, bool C8_LE_L2, is TRUE if X <= Y.
//
{
  bool value;

  if ( pow ( real ( x ), 2 ) + pow ( imag ( x ), 2 ) <= 
       pow ( real ( y ), 2 ) + pow ( imag ( y ), 2 ) )
  {
    value = true;
  }
  else
  {
    value = false;
  }
  return value;
}
//****************************************************************************80

bool c8_le_li ( complex <double> x, complex <double> y )

//****************************************************************************80
//
//  Purpose:
//
//    C8_LE_LI := X <= Y for C8 values, and the L Infinity norm.
//
//  Discussion:
//
//    A C8 is a complex double precision value.
//
//    The L Infinity norm can be defined here as:
//
//      C8_NORM_LI(X) = max ( abs ( real (X) ), abs ( imag (X) ) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, Y, the values to be compared.
//
//    Output, bool C8_LE_LI, is TRUE if X <= Y.
//
{
  bool value;

  if ( r8_max ( r8_abs ( real ( x ) ), r8_abs ( imag ( x ) ) ) <= 
       r8_max ( r8_abs ( real ( y ) ), r8_abs ( imag ( y ) ) ) )
  {
    value = true;
  }
  else
  {
    value = false;
  }
  return value;
}
//****************************************************************************80

double c8_magnitude ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_MAGNITUDE returns the magnitude of a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the value whose norm is desired.
//
//    Output, double C8_MAGNITUDE, the magnitude of X.
//
{
  double magnitude;

  magnitude = sqrt ( pow ( real ( x ), 2 ) + pow ( imag ( x ), 2 ) );

  return magnitude;
}
//****************************************************************************80

double c8_norm_l1 ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_NORM_L1 evaluates the L1 norm of a C8.
//
//  Discussion:
//
//    A C8 is a complex <double> value.
//
//    Numbers of equal norm lie along diamonds centered at (0,0).
//
//    The L1 norm can be defined here as:
//
//      C8_NORM_L1(X) = abs ( real (X) ) + abs ( imag (X) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the value whose norm is desired.
//
//    Output, double C8_NORM_L1, the norm of X.
//
{
  double value;

  value = r8_abs ( real ( x ) ) + abs ( imag ( x ) );

  return value;
}
//****************************************************************************80

double c8_norm_l2 ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_NORM_L2 evaluates the L2 norm of a C8.
//
//  Discussion:
//
//    A C8 is a complex <double> value.
//
//    Numbers of equal norm lie on circles centered at (0,0).
//
//    The L2 norm can be defined here as:
//
//      C8_NORM_L2(X) = sqrt ( ( real (X) )**2 + ( imag ( X ) )**2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the value whose norm is desired.
//
//    Output, double C8_NORM_L2, the 2-norm of X.
//
{
  double value;

  value = sqrt ( pow ( real ( x ), 2 )
               + pow ( imag ( x ), 2 ) );

  return value;
}
//****************************************************************************80

double c8_norm_li ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_NORM_LI evaluates the L-infinity norm of a C8.
//
//  Discussion:
//
//    A C8 is a complex <double> value.
//
//    Numbers of equal norm lie along squares whose centers are at (0,0).
//
//    The L-infinity norm can be defined here as:
//
//      C8_NORM_LI(X) = max ( abs ( real (X) ), abs ( imag (X) ) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the value whose norm is desired.
//
//    Output, double C8_NORM_LI, the infinity norm of X.
//
{
  double value;

  value = r8_max ( r8_abs ( real ( x ) ), abs ( imag ( x ) ) );

  return value;
}
//****************************************************************************80

complex <double> c8_normal_01 ( int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    C8_NORMAL_01 returns a unit pseudonormal C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, complex <double> C8_NORMAL_01, a unit pseudornormal value.
//
{
  double pi = 3.141592653589793;
  double v1;
  double v2;
  double x_c;
  double x_r;
  complex <double> value;

  v1 = r8_uniform_01 ( seed );
  v2 = r8_uniform_01 ( seed );

  x_r = sqrt ( -2.0 * log ( v1 ) ) * cos ( 2.0 * pi * v2 );
  x_c = sqrt ( -2.0 * log ( v1 ) ) * sin ( 2.0 * pi * v2 );

  value = complex <double> ( x_r, x_c );

  return value;
}
//****************************************************************************80

complex <double> c8_one ( )

//****************************************************************************80
//
//  Purpose:
//
//    C8_ONE returns the value of complex 1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    05 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, complex <double> C8_ONE, the value of complex 1.
//
{
  complex <double> value;

  value = complex <double> ( 1.0, 0.0);

  return value;
}
//****************************************************************************80

void c8_print ( complex <double> a, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C8_PRINT prints a C8, with an optional title.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> A, the value to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//
{
  if ( 0 < s_len_trim ( title ) )
  {
    cout << title
         << "  ( " << setw(14) << real ( a )
         << ", "   << setw(14) << imag ( a ) << " )\n";
  }
  else
  {
    cout << "( " << setw(14) << real ( a )
         << ", " << setw(14) << imag ( a ) << " )\n";
  }

  return;
}
//****************************************************************************80

complex <double> c8_sqrt ( complex <double> x )

//****************************************************************************80
//
//  Purpose:
//
//    C8_SQRT returns the principal square root of a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, complex <double> X, the number whose square root is desired.
//
//    Output, complex <double> C8_SQRT, the square root of X.
//
{
  double argument;
  double magnitude;
  complex <double> value;

  argument = c8_argument ( x );
  magnitude = c8_magnitude ( x );

  if ( magnitude == 0.0 )
  {
    value = complex <double> ( 0.0, 0.0 );
  }
  else
  {
    value = sqrt ( magnitude ) 
      * complex <double> ( cos ( argument / 2.0 ), sin ( argument / 2.0 ) );
  }

  return value;
}
//****************************************************************************80

void c8_swap ( complex <double> *x, complex <double> *y )

//****************************************************************************80
//
//  Purpose:
//
//    C8_SWAP swaps two C8's.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, complex <double> *X, *Y.  On output, the values of X and
//    Y have been interchanged.
//
{
  complex <double> z;

   z = *x; 
  *x = *y;
  *y =  z;

  return;
}
//****************************************************************************80

complex <double> c8_uniform_01 ( int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    C8_UNIFORM_01 returns a unit pseudorandom C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//    The angle should be uniformly distributed between 0 and 2 * PI,
//    the square root of the radius uniformly distributed between 0 and 1.
//
//    This results in a uniform distribution of values in the unit circle.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, complex <double> C8_UNIFORM_01, a pseudorandom complex value.
//
{
  double r;
  int k;
  double pi = 3.141592653589793;
  double theta;
  complex <double> value;

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }

  r = sqrt ( ( double ) ( *seed ) * 4.656612875E-10 );

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }

  theta = 2.0 * pi * ( ( double ) ( *seed ) * 4.656612875E-10 );

  value = r * complex <double> ( cos ( theta ), sin ( theta ) );

  return value;
}
//****************************************************************************80

complex <double> c8_zero ( )

//****************************************************************************80
//
//  Purpose:
//
//    C8_ZERO returns the value of 0 as a C8.
//
//  Discussion:
//
//    A C8 is a double precision complex value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, complex <double> C8_ZERO, the value of complex 0.
//
{
  complex <double> value;

  value = complex <double> ( 0.0, 0.0 );

  return value;
}
//****************************************************************************80

void c8mat_copy ( int m, int n, complex <double> a1[], complex <double> a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_COPY copies one C8MAT to another.
//
//  Discussion:
//
//    An C8MAT is a doubly dimensioned array of complex double precision values, 
//    which may be stored as a vector in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, complex <double> A1[M*N], the matrix to be copied.
//
//    Output, complex <double> A2[M*N], the copy of A1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return;
}
//****************************************************************************80

complex <double> *c8mat_copy_new ( int m, int n, complex <double> a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_COPY_NEW copies one C8MAT to a "new" C8MAT.
//
//  Discussion:
//
//    An C8MAT is a doubly dimensioned array of complex double precision values, 
//    which may be stored as a vector in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, complex <double> A1[M*N], the matrix to be copied.
//
//    Output, complex <double> C8MAT_COPY_NEW[M*N], the copy of A1.
//
{
  complex <double> *a2;
  int i;
  int j;

  a2 = new complex <double>[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return a2;
}
//****************************************************************************80

complex <double> *c8mat_identity ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_IDENTITY sets a C8MAT to the identity.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Output, complex <double> C8MAT_IDENTITY[N*N], the matrix.
//
{
  complex <double> *a;
  int i;
  int j;

  a = new complex <double> [n*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      if ( i == j )
      {
        a[i+j*n] = complex <double> ( 1.0, 0.0 );
      }
      else
      {
        a[i+j*n] = complex <double> ( 0.0, 0.0 );
      }
    }
  }
  return a;
}
//****************************************************************************80

complex <double> *c8mat_indicator ( int m, int n )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_INDICATOR returns the C8MAT indicator matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Output, complex <double> C8MAT_IDENTITY[M*N], the matrix.
//
{
  complex <double> *a;
  int i;
  int j;

  a = new complex <double> [m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = complex <double> ( i, j );
    }
  }
  return a;
}
//****************************************************************************80

void c8mat_nint ( int m, int n, complex <double> a[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_NINT rounds the entries of a C8MAT.
//
//  Discussion:
//
//    A C8MAT is a matrix of complex <double> values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//
//    Input/output, complex <double> A[M*N], the matrix to be NINT'ed.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = complex <double> ( 
        r8_nint ( real ( a[i+j*m] ) ), 
        r8_nint ( imag ( a[i+j*m] ) ) );
    }
  }
  return;
}
//****************************************************************************80

void c8mat_print ( int m, int n, complex <double> a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_PRINT prints a C8MAT.
//
//  Discussion:
//
//    A C8MAT is a matrix of double precision complex values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the matrix.
//
//    Input, complex <double> A[M*N], the matrix.
//
//    Input, char *TITLE, a title to be printed.
//
{
  c8mat_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//****************************************************************************80

void c8mat_print_some ( int m, int n, complex <double> a[], int ilo, int jlo, 
  int ihi, int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_PRINT_SOME prints some of a C8MAT.
//
//  Discussion:
//
//    A C8MAT is a matrix of double precision complex values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the matrix.
//
//    Input, complex <double> A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, the first row and
//    column, and the last row and column to be printed.
//
//    Input, char *TITLE, a title to be printed.
//
{
  complex <double> c;
  int i;
  int i2hi;
  int i2lo;
  int inc;
  int incx = 4;
  int j;
  int j2;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }
//
//  Print the columns of the matrix, in strips of INCX.
//
  for ( j2lo = jlo; j2lo <= jhi; j2lo = j2lo + incx )
  {
    j2hi = j2lo + incx - 1;
    j2hi = i4_min ( j2hi, n );
    j2hi = i4_min ( j2hi, jhi );

    inc = j2hi + 1 - j2lo;

    cout << "\n";
    cout << "  Col: ";
    for ( j = j2lo; j <= j2hi; j++ )
    {
      j2 = j + 1 - j2lo;
      cout << "          " << setw(10) << j;
    }
    cout << "\n";
    cout << "  Row\n";
    cout << "  ---\n";
//
//  Determine the range of the rows in this strip.
//
    i2lo = i4_max ( ilo, 1 );
    i2hi = i4_min ( ihi, m );

    for ( i = i2lo; i <= i2hi; i++ )
    {
//
//  Print out (up to) INCX entries in row I, that lie in the current strip.
//
      for ( j2 = 1; j2 <= inc; j2++ )
      {
        j = j2lo - 1 + j2;
        c = a[i-1+(j-1)*m];
        cout << "  " << setw(8) << real ( c )
             << "  " << setw(8) << imag ( c );
      }
      cout << "\n";
    }
  }
  cout << "\n";

  return;
}
//****************************************************************************80

complex <double> *c8mat_uniform_01 ( int m, int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    C8MAT_UNIFORM_01 returns a unit pseudorandom C8MAT.
//
//  Discussion:
//
//    A C8MAT is a matrix of double precision complex values.
//
//    The angles should be uniformly distributed between 0 and 2 * PI,
//    the square roots of the radius uniformly distributed between 0 and 1.
//
//    This results in a uniform distribution of values in the unit circle.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 March 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the matrix.
//
//    Input/output, int *SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, complex <double> C[M*N], the pseudorandom complex matrix.
//
{
  complex <double> *c;
  int i;
  int j;
  double r;
  int k;
  double pi = 3.141592653589793;
  double theta;

  c = new complex <double> [m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      k = *seed / 127773;

      *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

      if ( *seed < 0 )
      {
        *seed = *seed + 2147483647;
      }

      r = sqrt ( ( double ) ( *seed ) * 4.656612875E-10 );

      k = *seed / 127773;

      *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

      if ( *seed < 0 )
      {
        *seed = *seed + 2147483647;
      }

      theta = 2.0 * pi * ( ( double ) ( *seed ) * 4.656612875E-10 );

      c[i+j*m] = r * complex <double> ( cos ( theta ), sin ( theta ) );
    }
  }
  return c;
}
//****************************************************************************80

void c8vec_copy ( int n, complex <double> a1[], complex <double> a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_COPY copies a C8VEC.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, complex <double> A1[N], the vector to be copied.
//
//    Output, complex <double> A2[N], the copy of A1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return;
}
//****************************************************************************80

complex <double> *c8vec_copy_new ( int n, complex <double> a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_COPY_NEW copies a C8VEC to a "new" C8VEC.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, complex <double> A1[N], the vector to be copied.
//
//    Output, complex <double> C8VEC_COPY_NEW[N], the copy of A1.
//
{
  complex <double> *a2;
  int i;

  a2 = new complex <double>[n];

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return a2;
}
//****************************************************************************80

complex <double> *c8vec_indicator ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_INDICATOR sets a C8VEC to the indicator vector.
//
//  Discussion:
//
//    A C8VEC is a vector of double precision complex values.
//
//    X(1:N) = ( 1-1i, 2-2i, 3-3i, 4-4i, ... )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Output, complex <double> C8VEC_INDICATOR[N], the array to be initialized.
//
{
  complex <double> *a;
  int i;

  a = new complex <double> [n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = complex <double> ( i+1, -i-1 );
  }

  return a;
}
//****************************************************************************80

double c8vec_norm_l2 ( int n, complex <double> a[] )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_NORM_L2 returns the L2 norm of a C8VEC.
//
//  Discussion:
//
//    The vector L2 norm is defined as:
//
//      value = sqrt ( sum ( 1 <= I <= N ) conjg ( A(I) ) * A(I) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, complex <double> A[N], the vector whose L2 norm is desired.
//
//    Output, double C8VEC_NORM_L2, the L2 norm of A.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value 
          + real ( a[i] ) * real ( a[i] ) 
          + imag ( a[i] ) * imag ( a[i] );
  }
  value = sqrt ( value );

  return value;
}
//****************************************************************************80

void c8vec_print ( int n, complex <double> a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_PRINT prints a C8VEC, with an optional title.
//
//  Discussion:
//
//    A C8VEC is a vector of double precision complex values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, complex <double> A[N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i < n; i++ )
  {
    cout << "  " << setw(6) << i
         << "  " << real ( a[i] )
         << "  " << imag ( a[i] ) << "\n";
  }

  return;
}
//****************************************************************************80

complex <double> *c8vec_uniform_01 ( int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_UNIFORM_01 returns a unit C8VEC.
//
//  Discussion:
//
//    A C8VEC is a vector of double precision complex values.
//
//    The angles should be uniformly distributed between 0 and 2 * PI,
//    the square roots of the radius uniformly distributed between 0 and 1.
//
//    This results in a uniform distribution of values in the unit circle.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 March 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of values to compute.
//
//    Input/output, int *SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, complex <double> C8VEC_UNIFORM_01[N], the pseudorandom complex vector.
//
{
  complex <double> *c;
  int i;
  double r;
  int k;
  double pi = 3.141592653589793;
  double theta;

  c = new complex <double> [n];

  for ( i = 0; i < n; i++ )
  {
    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + 2147483647;
    }

    r = sqrt ( ( double ) ( *seed ) * 4.656612875E-10 );

    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + 2147483647;
    }

    theta = 2.0 * pi * ( ( double ) ( *seed ) * 4.656612875E-10 );

    c[i] = r * complex <double> ( cos ( theta ), sin ( theta ) );
  }

  return c;
}
//****************************************************************************80

complex <double> *c8vec_unity ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    C8VEC_UNITY returns the N roots of unity in a C8VEC.
//
//  Discussion:
//
//    A C8VEC is a vector of double precision complex values.
//
//    X(1:N) = exp ( 2 * PI * (0:N-1) / N )
//
//    X(1:N)**N = ( (1,0), (1,0), ..., (1,0) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Output, complex <double> C8VEC_UNITY[N], the N roots of unity.
//
{
  complex <double> *a;
  int i;
  double pi = 3.141592653589793;
  double theta;

  a = new complex <double> [n];

  for ( i = 0; i < n; i++ )
  {
    theta = pi * ( double ) ( 2 * i ) / ( double ) ( n );
    a[i] = complex <double> ( cos ( theta ), sin ( theta ) );
  }

  return a;
}
//****************************************************************************80

char ch_cap ( char c )

//****************************************************************************80
//
//  Purpose:
//
//    CH_CAP capitalizes a single character.
//
//  Discussion:
//
//    This routine should be equivalent to the library "toupper" function.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 July 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char C, the character to capitalize.
//
//    Output, char CH_CAP, the capitalized character.
//
{
  if ( 97 <= c && c <= 122 ) 
  {
    c = c - 32;
  }   

  return c;
}
//****************************************************************************80

bool ch_eqi ( char ch1, char ch2 )

//****************************************************************************80
//
//  Purpose:
//
//    CH_EQI is true if two characters are equal, disregarding case.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 June 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char CH1, CH2, the characters to compare.
//
//    Output, bool CH_EQI, is true if the two characters are equal,
//    disregarding case.
//
{
  bool value;

  if ( 97 <= ch1 && ch1 <= 122 ) 
  {
    ch1 = ch1 - 32;
  } 
  if ( 97 <= ch2 && ch2 <= 122 ) 
  {
    ch2 = ch2 - 32;
  }     

  value = ( ch1 == ch2 );

  return value;
}
//****************************************************************************80

bool ch_is_digit ( char c )

//****************************************************************************80
//
//  Purpose:
//
//    CH_IS_DIGIT returns TRUE if a character is a decimal digit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 December 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char C, the character to be analyzed.
//
//    Output, bool CH_IS_DIGIT, is TRUE if C is a digit.
//
{
  bool value;

  if ( '0' <= c && c <= '9' )
  {
    value = true;
  }
  else
  {
    value = false;
  }
  return value;
}
//****************************************************************************80

int ch_to_digit ( char ch )

//****************************************************************************80
//
//  Purpose:
//
//    CH_TO_DIGIT returns the integer value of a base 10 digit.
//
//  Example:
//
//     CH  DIGIT
//    ---  -----
//    '0'    0
//    '1'    1
//    ...  ...
//    '9'    9
//    ' '    0
//    'X'   -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 June 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char CH, the decimal digit, '0' through '9' or blank are legal.
//
//    Output, int CH_TO_DIGIT, the corresponding integer value.  If the 
//    character was 'illegal', then DIGIT is -1.
//
{
  int digit;

  if ( '0' <= ch && ch <= '9' )
  {
    digit = ch - '0';
  }
  else if ( ch == ' ' )
  {
    digit = 0;
  }
  else
  {
    digit = -1;
  }

  return digit;
}
//****************************************************************************80

double degrees_to_radians ( double degrees )

//****************************************************************************80
//
//  Purpose:
//
//    DEGREES_TO_RADIANS converts an angle measure from degrees to radians.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double DEGREES, the angle measure in degrees.
//
//    Output, double DEGREES_TO_RADIANS, the angle measure in radians.
//
{
  double value;

  value = ( degrees / 180.0 ) * 3.141592653589793;

  return value;
}
//****************************************************************************80

double e_constant ( )

//****************************************************************************80
//
//  Purpose:
//
//    E_CONSTANT returns the value of E.
//
//  Discussion:
//
//    "E" was named in honor of Euler, but is known as Napier's constant.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double E_CONSTANT, the base of the natural 
//    logarithm system.
//
{
  double value = 2.718281828459045;

  return value;
}
//****************************************************************************80

double euler_constant ( )

//****************************************************************************80
//
//  Purpose:
//
//    EULER_CONSTANT returns the value of the Euler-Mascheroni constant.
//
//  Discussion:
//
//    The Euler-Mascheroni constant is often denoted by a lower-case
//    Gamma.  Gamma is defined as
//
//      Gamma = limit ( M -> Infinity ) ( Sum ( 1 <= N <= M ) 1 / N ) - Log ( M )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double EULER_CONSTANT, the value of the 
//    Euler-Mascheroni constant.
//
{
  double value = 0.5772156649015328;

  return value;
}
//****************************************************************************80

void fac_div ( int prime_num, int npower1[], int npower2[], int npower3[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_DIV divides two quantities represented as prime factors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER1[PRIME_NUM], the powers of primes
//    in the representation of the first quantity.
//
//    Input, int NPOWER2[PRIME_NUM], the powers of primes
//    in the representation of the second quantity.
//
//    Output, int NPOWER3[PRIME_NUM], the powers of primes
//    in the representation of the quotient.
//
{
  int i;

  for ( i = 0; i < prime_num; i++ )
  {
    npower3[i] = npower1[i] - npower2[i];
  }

  return;
}
//****************************************************************************80

void fac_gcd ( int prime_num, int npower1[], int npower2[], int npower3[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_GCD finds the GCD of two products of prime factors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER1[PRIME_NUM], the powers of primes
//    in the representation of the first quantity.  All the powers
//    must be nonnegative.
//
//    Input, int NPOWER2[PRIME_NUM], the powers of primes
//    in the representation of the second quantity.  All the powers
//    must be nonnegative.
//
//    Output, int NPOWER3[PRIME_NUM], the powers of primes
//    in the representation of the GCD.
//
{
  int i;

  for ( i = 0; i < prime_num; i++ )
  {
    if ( npower1[i] < 0 )
    {
      cout << "\n";
      cout << "FAC_GCD - Fatal error!\n";
      cout << "  One of the powers is negative.\n";
      exit ( 1 );
    }

    if ( npower2[i] < 0 )
    {
      cout << "\n";
      cout << "FAC_GCD - Fatal error!\n";
      cout << "  One of the powers is negative.\n";
      exit ( 1 );
    }

    npower3[i] = i4_min ( npower1[i], npower2[i] );
  }

  return;
}
//****************************************************************************80

void fac_lcm ( int prime_num, int npower1[], int npower2[], int npower3[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_LCM finds the LCM of two products of prime factors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER1[PRIME_NUM], the powers of primes
//    in the representation of the first quantity.
//
//    Input, int NPOWER2[PRIME_NUM], the powers of primes
//    in the representation of the second quantity.
//
//    Output, int NPOWER3[PRIME_NUM], the powers of primes
//    in the representation of the LCM.
//
{
  int i;

  for ( i = 0; i < prime_num; i++ )
  {
    if ( npower1[i] < 0 )
    {
      cout << "\n";
      cout << "FAC_LCM - Fatal error!\n";
      cout << "  One of the powers is negative.\n";
      exit ( 1 );
    }

    if ( npower2[i] < 0 )
    {
      cout << "\n";
      cout << "FAC_LCM - Fatal error!\n";
      cout << "  One of the powers is negative.\n";
      exit ( 1 );
    }

    npower3[i] = i4_max ( npower1[i], npower2[i] );
  }

  return;
}
//****************************************************************************80

void fac_mul ( int prime_num, int npower1[], int npower2[], int npower3[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_MUL multiplies two quantities represented as prime factors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER1[PRIME_NUM], the powers of primes
//    in the representation of the first quantity.
//
//    Input, int NPOWER2[PRIME_NUM], the powers of primes
//    in the representation of the second quantity.
//
//    Output, int NPOWER3[PRIME_NUM], the powers of primes
//    in the representation of the product.
//
{
  int i;

  for ( i = 0; i < prime_num; i++ )
  {
    npower3[i] = npower1[i] + npower2[i];
  }
  return;
}
//****************************************************************************80

void fac_print ( int prime_num, int npower[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_PRINT prints a product of prime factors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER[PRIME_NUM], the powers of primes
//    in the representation of the quantity.
//
{
  int i;

  cout << "\n";
  cout << "   Prime     Power\n";
  cout << "\n";

  for ( i = 0; i < prime_num; i++ )
  {
    if ( npower[i] != 0 )
    {
      cout << "  " << setw(8) << prime ( i+1 )
           << "  " << setw(8) << npower[i] << "\n";
    }
  }

  return;
}
//****************************************************************************80

int fac_to_i4 ( int prime_num, int npower[] )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_TO_I4 converts a product of prime factors into an I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER[PRIME_NUM], the powers of primes
//    in the representation of the quantity.  If any of these powers
//    are negative, then INTVAL will be set to 0.
//
//    Output, int FAC_TO_I4, the integer represented by the product of the
//    prime factors.
//
{
  int factor;
  int i;
  int intval;
  int j;

  intval = 1;

  for ( i = 0; i < prime_num; i++ )
  {
    if ( npower[i] < 0 )
    {
      cout << "\n";
      cout << "FAC_TO_I4 - Fatal error!\n";
      cout << "  One of the powers is negative.\n";
      exit ( 1 );
    }
    factor = prime ( i+1 );

    for ( j = 1; j <= npower[i]; j++ )
    {
      intval = intval * factor;
    }
  }

  return intval;
}
//****************************************************************************80

void fac_to_rat ( int prime_num, int npower[], int *top, int *bot )

//****************************************************************************80
//
//  Purpose:
//
//    FAC_TO_RAT converts a prime factorization into a rational value.
//
//  Example:
//
//    Start with the prime factorization representation:
//
//      40/9 = 2**3 * 3**(-2) * 5
//
//    Input:
//
//      NPOWER = ( 3, -2, 1 )
//
//    Output:
//
//      TOP = 40 ( = 2**3 * 5**1 = PRIME(1)**3                 * PRIME(3)**1 )
//      BOT = 9  ( = 3**2        =               PRIME(2)**2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PRIME_NUM, the index of the highest prime number
//    used in the representations.
//
//    Input, int NPOWER[PRIME_NUM].  NPOWER(I) is the power of
//    the I-th prime in the prime factorization.  NPOWER(I) may
//    be positive or negative.
//
//    Output, int TOP, BOT, the top and bottom of a rational value.
//
{
  int i;

  *top = 1;
  *bot = 1;

  for ( i = 0; i < prime_num; i++ )
  {
    if ( 0 < npower[i] )
    {
      *top = *top * i4_power ( prime ( i + 1 ), npower[i] );
    }
    else if ( npower[i] < 0 )
    {
      *bot = *bot * i4_power ( prime ( i + 1 ), -npower[i] );
    }
  }

  return;
}
//****************************************************************************80

double feet_to_meters ( double ft )

//****************************************************************************80
//
//  Purpose:
//
//    FEET_TO_METERS converts a measurement in feet to meters.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double FT, the length in feet.
//
//    Output, double FEET_TO_METERS, the corresponding length in meters.
//
{
  double value;

  value = 0.0254 * 12.0 * ft;

  return value;
}
//****************************************************************************80

double gauss_sum ( int ndim, int n, double amplitude[], double center[], 
  double width[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    GAUSS_SUM evaluates a function that is the sum of Gaussians.
//
//  Discussion:
//
//    Gauss_Sum(X) = Sum ( 1 <= J <= Ngauss ) Amplitude(I) * exp ( -Arg )
//
//    where
//
//      Arg = sum ( 1 <= I <= NDIM ) ( ( ( X(I) - Center(I,J) ) / Width(J) )^2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NDIM, the spatial dimension.
//
//    Input, int N, the number of component Gaussian functions.
//
//    Input, double AMPLITUDE[N], CENTER[NDIM*N], WIDTH[N],
//    the amplitude, center and width for the component Gaussian functions.
//
//    Input, double X[NDIM], the point at which the function
//    is to be evaluated.
//
//    Output, double GAUSS_SUM, the value of the function.
//
{
  double arg;
  int i;
  int j;
  double value;

  value = 0.0;

  for ( j = 0; j < n; j++ )
  {
    arg = 0.0;
    for ( i = 0; i < ndim; i++ )
    {
      arg = arg + pow ( ( x[i] - center[i+j*ndim] ) / width[j], 2 );
    }
    value = value + amplitude[j] * exp ( -arg );
  }

  return value;
}
//****************************************************************************80

unsigned long get_seed ( )

//****************************************************************************80
//
//  Purpose:
//
//    GET_SEED returns a random seed for the random number generator.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, unsigned long GET_SEED, a random seed value.
//
{
# define UNSIGNED_LONG_MAX 4294967295UL

  time_t clock;
  int i;
  int hours;
  int minutes;
  int seconds;
  struct tm *lt;
  static unsigned long seed = 0;
  time_t tloc;
//
//  If the internal seed is 0, generate a value based on the time.
//
  if ( seed == 0 )
  {
    clock = time ( &tloc );
    lt = localtime ( &clock );
//
//  Extract HOURS.
//
    hours = lt->tm_hour;
//
//  In case of 24 hour clocks, shift so that HOURS is between 1 and 12.
//
    if ( 12 < hours )
    {
      hours = hours - 12;
    }
//
//  Move HOURS to 0, 1, ..., 11
//
    hours = hours - 1;

    minutes = lt->tm_min;

    seconds = lt->tm_sec;

    seed = seconds + 60 * ( minutes + 60 * hours );
//
//  We want values in [1,43200], not [0,43199].
//
    seed = seed + 1;
//
//  Remap SEED from [1,43200] to [1,UNSIGNED_LONG_MAX].
//
    seed = ( unsigned long ) 
      ( ( ( double ) seed )
      * ( ( double ) UNSIGNED_LONG_MAX ) / ( 60.0 * 60.0 * 12.0 ) );
  }
//
//  Never use a seed of 0.
//
  if ( seed == 0 )
  {
    seed = 1;
  }

  return seed;

# undef UNSIGNED_LONG_MAX
}
//****************************************************************************80

double *grid1 ( int dim_num, int nstep, double x1[], double x2[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID1 finds grid points between X1 and X2 in N dimensions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the dimension of the points X1 and X2.
//
//    Input, int NSTEP, the number of points to be generated.
//    NSTEP must be at least 2.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], the first and last
//    points, between which the equally spaced points are
//    to be computed.
//
//    Output, double X[DIM_NUM*NSTEP], the set of equally spaced
//    points.  Each column of X represents one point, with X[*,1] = X1
//    and X[*,NSTEP] = X2.
//
{
  int i;
  int j;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID1 - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    return NULL;
  }

  if ( nstep < 2 )
  {
    cout << "\n";
    cout << "GRID1 - Fatal error!\n";
    cout << "  NSTEP < 2.\n";
    cout << "  NSTEP = " << nstep << "\n";
    return NULL;
  }

  x = new double[dim_num*nstep];

  for ( j = 1; j <= nstep; j++ )
  {
    for ( i = 0; i < dim_num; i++ )
    {
      x[i+(j-1)*dim_num] =
        ( ( double ) ( nstep - j     ) * x1[i]   
        + ( double ) (         j - 1 ) * x2[i] ) 
        / ( double ) ( nstep     - 1 );
    }
  }
  return x;
}
//****************************************************************************80

double *grid1n ( int j, int dim_num, int nstep, double x1[], double x2[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID1N finds the I-th grid point between X1 and X2 in N dimensions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 April 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J, the number of the desired point.
//    Normally J would be between 1 and NSTEP, but that is
//    not necessary.  Note that J = 1 returns X1 and J = NSTEP
//    returns X2.
//
//    Input, int DIM_NUM, the dimension of the points X, X1 and X2.
//
//    Input, int NSTEP, this is the number of equally
//    spaced points that are between X1 and X2.  NSTEP must
//    be at least 2, because X1 and X2 are always included
//    in the set of points.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], the first and last
//    points, between which the equally spaced points lie.
//
//    Output, double GRID1N[DIM_NUM], the J-th grid point between X1
//    and X2.
//
{
  int i;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID1N - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    return NULL;
  }

  if ( nstep < 2 )
  {
    cout << "\n";
    cout << "GRID1N - Fatal error!\n";
    cout << "  NSTEP < 2.\n";
    cout << "  NSTEP = " << nstep << "\n";
    return NULL;
  }

  x = new double[dim_num];

  for ( i = 0; i < dim_num; i++ )
  {
    x[i] = ( ( double ) ( nstep - j     ) * x1[i]
           + ( double ) (         j - 1 ) * x2[i] )
           / ( double ) ( nstep     - 1 );
  }

  return x;
}
//****************************************************************************80

double *grid2 ( int j1, int j2, int dim_num, int nstep, double x1[], 
  double x2[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID2 computes grid points between X1 and X2 in N dimensions.
//
//  Discussion:
//
//    GRID2 computes grid points between X1 and X2 in N dimensions.
//
//    However, X1 need not be the first point computed, nor X2 the last.
//    The user must specify the steps on which X1 and X2 are passed
//    through.  These steps may even be outside the range of 1 through NSTEP.
//
//    We assume that a set of equally spaced points have
//    been drawn on the line through X1 and X2, and that
//    they have been numbered, with X1 labeled J1 and X2
//    labeled J2.  J1 or J2 may be between 1 and NSTEP,
//    in which case X1 or X2 will actually be returned in the
//    X array, but there is no requirement that J1 or J2
//    satisfy this condition.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J1, J2.  J1 specifies the step on which
//    X1 would be computed, and similarly for J2.  
//    J1 and J2 must be distinct.
//
//    Input, int DIM_NUM, the dimension of the points X1 and X2.
//
//    Input, int NSTEP, this is the number of equally
//    spaced points that are to be generated.
//    NSTEP should be at least 1.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], the points that define
//    the line along which the equally spaced points are generated, and
//    which may or may not be included in the set of computed points.
//
//    Output, double GRID2[DIM_NUM*NSTEP], the set of equally spaced
//    points.  Each column of X represents one point.
//    If 1 <= J1 <= NSTEP, then X(*,J1) = X1, and similarly for J2.
//
{
  int i;
  int j;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID2 - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    return NULL;
  }

  if ( j1 == j2 )
  {
    cout << "\n";
    cout << "GRID2 - Fatal error!\n";
    cout << "  J1 = J2, leading to zero denominator.\n";
    cout << "  J1 = " << j1 << "\n";
    cout << "  J2 = " << j2 << "\n";
    return NULL;
  }

  x = new double[nstep*dim_num];

  for ( j = 1; j <= nstep; j++ )
  {
    for ( i = 0; i < dim_num; i++ )
    {
      x[i+(j-1)*dim_num] = ( ( double ) ( j2 - j      ) * x1[i]   
                           + ( double ) (      j - j1 ) * x2[i] ) 
                           / ( double ) ( j2     - j1 );
    }
  }

  return x;
}
//****************************************************************************80

double *grid2n ( int j, int j1, int j2, int dim_num, double x1[], double x2[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID2N computes one grid point between X1 and X2 in N dimensions.
//
//  Discussion:
//
//    However, X1 need not be the first point computed, nor X2 the last.
//    The user must specify the steps on which X1 and X2 are passed through.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J, the J coordinate of the desired point.
//    Note that if J = J1, X will be returned as X1, and if
//    J = J2, X will be returned as X2.
//
//    Input, int J1, J2.  J1 specifies the step on which
//    X1 would be computed, and similarly for J2.  That is,
//    we assume that a set of equally spaced points have
//    been drawn on the line through X1 and X2, and that
//    they have been numbered, with X1 labeled J1 and X2
//    labeled J2.  J1 and J2 must be distinct.
//
//    Input, int DIM_NUM, the dimension of the points X1 and X2.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], the points that define
//    the line along which the equally spaced points are
//    generated, and which may or may not be included in the
//    set of computed points.
//
//    Output, double GRID_2N[DIM_NUM].  X(I) is the J-th point from the
//    set of equally spaced points.
//
{
  int i;
  double *x;

  x = new double[dim_num];

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID2N - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    exit ( 1 );
  }

  if ( j1 == j2 )
  {
    cout << "\n";
    cout << "GRID2N - Fatal error!\n";
    cout << "  J1 = J2, leading to zero denominator.\n";
    cout << "  J1 = " << j1 << "\n";
    cout << "  J2 = " << j2 << "\n";
    exit ( 1 );
  }

  for ( i = 0; i < dim_num; i++ )
  {
    x[i] = ( ( double ) ( j2 - j      ) * x1[i]   
           + ( double ) (      j - j1 ) * x2[i] )
           / ( double ) ( j2     - j1 );
  }

  return x;
}
//****************************************************************************80

double *grid3 ( int dim_num, int nstep1, int nstep2, double x1[], double x2[],
  double x3[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID3 computes a grid on the parallelogram set by X1, X2 and X3 in N space.
//
//  Discussion:
//
//    The line between X1 and X2 will have NSTEP1 points generated along 
//    it, and the line between X1 and X3 will have NSTEP2 points generated
//    along it.
//
//    Fixing the second and third indices of X represents one point, with
//    the following special values:
//
//      X(*,1,1)      = X1
//      X(*,NSTEP1,1) = X2
//      X(*,1,NSTEP2) = X3.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the dimension of the points X1, X2 and X3.
//
//    Input, int NSTEP1, NSTEP2.  These are the number of
//    equally spaced points to generate in the first and second
//    directions.  NSTEP1 and NSTEP2 must be at least 2, because X1, X2 and
//    X3 are always included in the set of points.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], X3[DIM_NUM], the points
//    which define three corners of the parallelogram on
//    which the grid will be generated.
//
//    Output, double GRID3[DIM_NUM*NSTEP1*NSTEP2], the set of equally
//    spaced points.  
//
{
  int i;
  int j;
  int k;
  double psi1;
  double psi2;
  double psi3;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID3 - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    exit ( 1 );
  }

  if ( nstep1 < 2 )
  {
    cout << "\n";
    cout << "GRID3 - Fatal error!\n";
    cout << "  NSTEP1 < 2.\n";
    cout << "  NSTEP1 = " << nstep1 << "\n";
    return NULL;
  }

  if ( nstep2 < 2 )
  {
    cout << "\n";
    cout << "GRID3 - Fatal error!\n";
    cout << "  NSTEP2 < 2.\n";
    cout << "  NSTEP2 = " << nstep2 << "\n";
    return NULL;
  }

  x = new double[dim_num*nstep1*nstep2];

  for ( j = 1; j <= nstep1; j++ )
  {
    psi2 = ( double ) ( j      - 1 ) 
         / ( double ) ( nstep1 - 1 );

    for ( k = 1; k <= nstep2; k++ )
    {
      psi3 = ( double ) (      k - 1 ) 
           / ( double ) ( nstep2 - 1 );

      psi1 = 1.0 - psi2 - psi3;

      for ( i = 0; i < dim_num; i++ )
      {
        x[i+(j-1)*dim_num+(k-1)*dim_num*nstep1] = 
            psi1 * x1[i]
          + psi2 * x2[i]
          + psi3 * x3[i];
      }
    }
  }

  return x;
}
//****************************************************************************80

double *grid3n ( int j, int k, int dim_num, int nstep1, int nstep2, 
  double x1[], double x2[], double x3[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID3N computes a parallelogram grid on 3 points in N dimensions.
//
//  Discussion:
//
//    The line between X1 and X2 will have NSTEP1
//    points generated along it, and the line between X1 and
//    X3 will have NSTEP2 points generated along it.
//
//    The following special values are:
//
//      J       K         X
//
//      1       1         X1
//      NSTEP1  1         X2
//      1       NSTEP2    X3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 April 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J, K, the parallelogram coordinates
//    of the point.  J measures steps from X1 to X2, and
//    K measures steps from X1 to X3.  Normally, J would
//    be between 1 and NSTEP1, K between 1 and NSTEP2,
//    but this is not necessary.
//
//    Input, int DIM_NUM, the dimension of the points X1, X2 and X3.
//
//    Input, int NSTEP1, NSTEP2.  These are the number of
//    equally spaced points to generate in the first and second
//    directions.  NSTEP1 and NSTEP2 must be at least 2, because X1, X2 and
//    X3 are always included in the set of points.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], X3[DIM_NUM], the points
//    which define three corners of the parallelogram on
//    which the grid will be generated.
//
//    Output, double GRID3N[DIM_NUM], the point with coordinates (J,K)
//    from the the set of equally spaced points.  
//
{
  int i;
  double psi1;
  double psi2;
  double psi3;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID3N - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    exit ( 1 );
  }

  if ( nstep1 < 2 )
  {
    cout << "\n";
    cout << "GRID3N - Fatal error!\n";
    cout << "  NSTEP1 < 2.\n";
    cout << "  NSTEP1 = " << nstep1 << "\n";
    return NULL;
  }

  if ( nstep2 < 2 )
  {
    cout << "\n";
    cout << "GRID3N - Fatal error!\n";
    cout << "  NSTEP2 < 2.\n";
    cout << "  NSTEP2 = " << nstep2 << "\n";
    return NULL;
  }

  x = new double[dim_num];

  psi2 = ( double ) ( j - 1  ) / ( double ) ( nstep1 - 1 );

  psi3 = ( double ) ( k - 1  ) / ( double ) ( nstep2 - 1 );

  psi1 = 1.0 - psi2 - psi3;

  for ( i = 0; i < dim_num; i++ )
  {
    x[i] = psi1 * x1[i] + psi2 * x2[i] + psi3 * x3[i];
  }

  return x;
}
//****************************************************************************80

double *grid4 ( int j1, int j2, int k1, int k2, int dim_num, int nstep1, 
  int nstep2, double x1[], double x2[], double x3[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID4 computes a grid on the parallelogram set by X1, X2 and X3 in N space.
//
//  Discussion:
//
//    Unlike GRID3, GRID4 does not necessarily place X1 at the
//    "origin" of the parallelogram, with X2 and X3 set at the
//    extreme J and K coordinates.  Instead, the user is free
//    to specify the J and K coordinates of the points, although
//    they are required to lie on a subparallelogram of the
//    larger one.
//
//    The line through X1 and X2 will have NSTEP1
//    points generated along it, and the line through X1 and
//    X3 will have NSTEP2 points generated along it.
//
//    If we imagine that the
//    main parallelogram is drawn first, with coordinate
//    ranges 1 <= J <= NSTEP1 and 1 <= K <= NSTEP2, then
//    these indices determine the (J,K) coordinates of the
//    three points, namely:
//
//      X1 : (J1,K1)
//      X2 : (J2,K1)
//      X3 : (J1,K2)
//
//    Of course, we actually start with the points X1, X2,
//    and X3, and they define a parallelogram and a (J,K)
//    coordinate system over the plane containing them.  We
//    then are free to consider the parallelogram defined
//    by the three points (1,1), (NSTEP1,1) and (1,NSTEP2),
//    which may or may not contain any of the points X1, X2
//    and X3.
//
//    Assuming that the indices J1, J2, K1 and K2 are "within
//    bounds", the following special values will be computed:
//
//      X(*,J1,K1) = X1
//      X(*,J2,K1) = X2
//      X(*,J1,K2) = X3.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J1, J2, K1, K2, the indices.  
//
//    Input, int DIM_NUM, the dimension of the points X1, X2 and X3.
//
//    Input, int NSTEP1, NSTEP2.  These are the number of
//    equally spaced points to generate in the first and second
//    directions.  NSTEP1 and NSTEP2 should be at least 1.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], X3[DIM_NUM], the points
//    which define three corners of the parallelogram on
//    which the grid will be generated.
//
//    Output, double X[DIM_NUM*NSTEP1*NSTEP2], the set of equally
//    spaced points.  Fixing the second and third indices
//    of X represents one point.
//
{
  int i;
  int j;
  int k;
  double psi1;
  double psi2;
  double psi3;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID4 - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    exit ( 1 );
  }

  if ( nstep1 < 2 )
  {
    cout << "\n";
    cout << "GRID4 - Fatal error!\n";
    cout << "  NSTEP1 < 2.\n";
    cout << "  NSTEP1 = " << nstep1 << "\n";
    return NULL;
  }

  if ( nstep2 < 2 )
  {
    cout << "\n";
    cout << "GRID4 - Fatal error!\n";
    cout << "  NSTEP2 < 2.\n";
    cout << "  NSTEP2 = " << nstep2 << "\n";
    return NULL;
  }

  if ( j1 == j2 )
  {
    cout << "\n";
    cout << "GRID4 - Fatal error!\n";
    cout << "  J1 = J2, leading to zero denominator.\n";
    cout << "  J1 = " << j1 << "\n";
    cout << "  J2 = " << j2 << "\n";
    exit ( 1 );
  }

  if ( k1 == k2 )
  {
    cout << "\n";
    cout << "GRID4 - Fatal error!\n";
    cout << "  K1 = K2, leading to zero denominator.\n";
    cout << "  K1 = " << k1 << "\n";
    cout << "  K2 = " << k2 << "\n";
    exit ( 1 );
  }

  x = new double[dim_num*nstep1*nstep2];

  for ( j = 1; j <= nstep1; j++ )
  {
    psi2 = ( double ) (  j - j1 ) 
         / ( double ) ( j2 - j1 );

    for ( k = 1; k <= nstep2; k++ )
    {
      psi3 = ( double ) (  k - k1 ) 
           / ( double ) ( k2 - k1 );

      psi1 = 1.0 - psi2 - psi3;

      for ( i = 0; i < dim_num; i++ )
      {
        x[i+(j-1)*dim_num+(k-1)*dim_num*nstep1] =
            psi1 * x1[i]
          + psi2 * x2[i]
          + psi3 * x3[i];
      }
    }
  }

  return x;
}
//****************************************************************************80

double *grid4n ( int j, int j1, int j2, int k, int k1, int k2, int dim_num, 
  int nstep1, int nstep2, double x1[], double x2[], double x3[] )

//****************************************************************************80
//
//  Purpose:
//
//    GRID4N computes a single point on a parallelogram grid in N space.
//
//  Discussion:
//
//    The computation is identical to that of GRID4, except that
//    only one point at a time is computed.
//
//    The line through X1 and X2 will have NSTEP1
//    points generated along it, and the line through X1 and
//    X3 will have NSTEP2 points generated along it.
//
//    The following special values will be computed:
//
//      J  K  X
//
//      J1 K1 X1
//      J2 K2 X2
//      J1 K2 X3
//
//    If we imagine that the main parallelogram is drawn first, with 
//    coordinate ranges 1 <= J <= NSTEP1 and 1 <= K <= NSTEP2, then
//    the indices J and K determine the (J,K) coordinates of the
//    three points X1, X2, and X3, namely:
//
//      X1 : (J1,K1)
//      X2 : (J2,K1)
//      X3 : (J1,K2)
//
//    Of course, we actually start with the points X1, X2,
//    and X3, and they define a parallelogram and an (J,K)
//    coordinate system over the plane containing them.  We
//    then are free to consider the parallelogram defined
//    by the three points (1,1), (NSTEP1,1) and (1,NSTEP2),
//    which may or may not contain any of the points X1, X2
//    and X3.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int J, the J coordinate of the point X.
//
//    Input, int J1, J2.  See discussion.
//
//    Input, int K, the K coordinate of the point X.
//
//    Input, int K1, K2.  See discussion.
//
//    Input, int DIM_NUM, the dimension of the points X, X1, X2 and X3.
//
//    Input, int NSTEP1, NSTEP2.  These are the number of
//    equally spaced points generated in the first and second
//    directions.
//    NSTEP1 and NSTEP2 should be at least 1.
//
//    Input, double X1[DIM_NUM], X2[DIM_NUM], X3[DIM_NUM], the points
//    which define three corners of the parallelogram on
//    which the grid will be generated.
//
//    Output, double GRID4N[DIM_NUM], the point whose parallelogram
//    coordinates are (J,K).
//
{
  int i;
  double psi1;
  double psi2;
  double psi3;
  double *x;

  if ( dim_num < 1 )
  {
    cout << "\n";
    cout << "GRID4N - Fatal error!\n";
    cout << "  DIM_NUM < 1.\n";
    cout << "  DIM_NUM = " << dim_num << "\n";
    exit ( 1 );
  }

  if ( nstep1 < 2 )
  {
    cout << "\n";
    cout << "GRID4N - Fatal error!\n";
    cout << "  NSTEP1 < 2.\n";
    cout << "  NSTEP1 = " << nstep1 << "\n";
    return NULL;
  }

  if ( nstep2 < 2 )
  {
    cout << "\n";
    cout << "GRID4N - Fatal error!\n";
    cout << "  NSTEP2 < 2.\n";
    cout << "  NSTEP2 = " << nstep2 << "\n";
    return NULL;
  }

  if ( j1 == j2 )
  {
    cout << "\n";
    cout << "GRID4N - Fatal error!\n";
    cout << "  J1 = J2, leading to zero denominator.\n";
    cout << "  J1 = " << j1 << "\n";
    cout << "  J2 = " << j2 << "\n";
    exit ( 1 );
  }

  if ( k1 == k2 )
  {
    cout << "\n";
    cout << "GRID4N - Fatal error!\n";
    cout << "  K1 = K2, leading to zero denominator.\n";
    cout << "  K1 = " << k1 << "\n";
    cout << "  K2 = " << k2 << "\n";
    exit ( 1 );
  }

  psi2 = double ( j  - j1 ) / double ( j2 - j1 );

  psi3 = double ( k  - k1 ) / double ( k2 - k1 );

  psi1 = 1.0 - psi2 - psi3;

  x = new double[dim_num];

  for ( i = 0; i < dim_num; i++ )
  {
    x[i] = psi1 * x1[i] + psi2 * x2[i] + psi3 * x3[i];
  }

  return x;
}
//****************************************************************************80

short int i2_reverse_bytes ( short int x )

//****************************************************************************80
//
//  Purpose:
//
//    I2_REVERSE_BYTES reverses the two bytes in an I2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, short int X, a value whose bytes are to be reversed.
//
//    Output, short int I2_REVERSE_BYTES, a value with
//    bytes in reverse order.
//
{
  char c;
  union 
  {
    short int yshortint;
    char ychar[2];
  } y;

  y.yshortint = x;
  
  c = y.ychar[0];
  y.ychar[0] = y.ychar[1];
  y.ychar[1] = c;

  return ( y.yshortint );
}
//****************************************************************************80

int i4_bit_hi1 ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_BIT_HI1 returns the position of the high 1 bit base 2 in an I4.
//
//  Example:
//
//       N    Binary    Hi 1
//    ----    --------  ----
//       0           0     0
//       1           1     1
//       2          10     2
//       3          11     2 
//       4         100     3
//       5         101     3
//       6         110     3
//       7         111     3
//       8        1000     4
//       9        1001     4
//      10        1010     4
//      11        1011     4
//      12        1100     4
//      13        1101     4
//      14        1110     4
//      15        1111     4
//      16       10000     5
//      17       10001     5
//    1023  1111111111    10
//    1024 10000000000    11
//    1025 10000000001    11
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the integer to be measured.
//    N should be nonnegative.  If N is nonpositive, I4_BIT_HI1
//    will always be 0.
//
//    Output, int I4_BIT_HI1, the location of the high order bit.
//
{
  int bit;

  bit = 0;

  while ( 0 < n )
  {
    bit = bit + 1;
    n = n / 2;
  }

  return bit;
}
//****************************************************************************80

int i4_bit_lo0 ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_BIT_LO0 returns the position of the low 0 bit base 2 in an I4.
//
//  Example:
//
//       N    Binary    Lo 0
//    ----    --------  ----
//       0           0     1
//       1           1     2
//       2          10     1
//       3          11     3 
//       4         100     1
//       5         101     2
//       6         110     1
//       7         111     4
//       8        1000     1
//       9        1001     2
//      10        1010     1
//      11        1011     3
//      12        1100     1
//      13        1101     2
//      14        1110     1
//      15        1111     5
//      16       10000     1
//      17       10001     2
//    1023  1111111111     1
//    1024 10000000000     1
//    1025 10000000001     1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the integer to be measured.
//    N should be nonnegative.
//
//    Output, int I4_BIT_LO0, the position of the low 1 bit.
//
{
  int bit;
  int n2;

  bit = 0;

  while ( true )
  {
    bit = bit + 1;
    n2 = n / 2;

    if ( n == 2 * n2 )
    {
      break;
    }

    n = n2;

  }

  return bit;
}
//****************************************************************************80

int i4_bit_lo1 ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_BIT_LO1 returns the position of the low 1 bit base 2 in an I4.
//
//  Example:
//
//       N    Binary    Lo 1
//    ----    --------  ----
//       0           0     0
//       1           1     1
//       2          10     2
//       3          11     1
//       4         100     3
//       5         101     1
//       6         110     2
//       7         111     1
//       8        1000     4
//       9        1001     1
//      10        1010     2
//      11        1011     1
//      12        1100     3
//      13        1101     1
//      14        1110     2
//      15        1111     1
//      16       10000     5
//      17       10001     1
//    1023  1111111111     1
//    1024 10000000000    11
//    1025 10000000001     1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the integer to be measured.
//    N should be nonnegative.
//
//    Output, int I4_BIT_LO1, the position of the low 1 bit.
//
{

  int bit;
  int i;
  int i2;

  bit = 0;
  i = n;

  for ( ; ; )
  {
    bit = bit + 1;
    i2 = i / 2;

    if ( i != 2 * i2 )
    {
      break;
    }

    i = i2;
  }

  return bit;
}
//****************************************************************************80

int i4_bit_reverse ( int i, int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_BIT_REVERSE reverses the bits in an I4.
//
//  Discussion:
//
//    An I4 is an int value.
//
//  Example:
//
//       I      N  2^N     I4_BIT_REVERSE ( I, N )
//    ----    --------  -----------------------
//       0      0    1     0
//       1      0    1     1
//
//       0      3    8     0
//       1      3    8     4
//       2      3    8     2
//       3      3    8     6
//       4      3    8     1
//       5      3    8     5
//       6      3    8     3
//       7      3    8     7
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 March 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer to be bit reversed.
//    I should be nonnegative.  Normally I < 2^N.
//
//    Input, int N, indicates the number of bits to
//    be reverse (N+1) or the base with respect to which the integer is to 
//    be reversed (2^N).  N should be nonnegative.
//
//    Output, int I4_BIT_REVERSE, the bit reversed value.
//
{
  int b;
  int j;
  int value;

  if ( i < 0 )
  {
    value = -1;
  }
  else if ( n < 0 )
  {
    value = -1;
  }
  else
  {
    b = i4_power ( 2, n );
    j = ( i % b );

    value = 0;

    for ( ; ; )
    {
      if ( b == 1 )
      {
        value = value + j;
        j = 0;
        break;
      }
      else
      {
        if ( ( j % 2 ) == 1 )
        {
          value = value + b / 2;
          j = j - 1;
        }

        j = j / 2 ;      
        b = b / 2;
      }
    }
  }

  return value;
}
//****************************************************************************80

int i4_characteristic ( int q )

//****************************************************************************80
//
//  Purpose:
//
//    I4_CHARACTERISTIC gives the characteristic for an I4.
//
//  Discussion:
//
//    For any positive integer Q, the characteristic is:
//
//    Q, if Q is a prime;
//    P, if Q = P**N for some prime P and some integer N;
//    0, otherwise, that is, if Q is negative, 0, 1, or the product
//       of more than one distinct prime.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 March 2004
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Harald Niederreiter,
//    Algorithm 738:
//    Programs to Generate Niederreiter's Low-Discrepancy Sequences,
//    ACM Transactions on Mathematical Software,
//    Volume 20, Number 4, pages 494-495, 1994.
//
//  Parameters:
//
//    Input, int Q, the value to be tested.
//
//    Output, int I4_CHARACTERISTIC, the characteristic of Q.
//
{
  int i;
  int i_maximum;

  if ( q <= 1 )
  {
    return 0;
  }
//
//  If Q is not prime, then there is at least one prime factor
//  of Q no greater than SQRT(Q)+1.
//
//  A faster code would only consider prime values of I,
//  but that entails storing a table of primes and limiting the
//  size of Q.  Simplicity and flexibility for now//
//
  i_maximum = ( int ) ( sqrt ( ( double ) ( q ) ) ) + 1;

  for ( i = 2; i <= i_maximum; i++ )
  {
    if ( ( q % i ) == 0 )
    {
      while ( ( q % i ) == 0 )
      {
        q = q / i;
      }

      if ( q == 1 )
      {
        return i;
      }
      else
      {
        return 0;
      }
    }
  }
//
//  If no factor was found, then Q is prime.
//
  return q;
}
//****************************************************************************80

int i4_choose ( int n, int k )

//****************************************************************************80
//
//  Purpose:
//
//    I4_CHOOSE computes the binomial coefficient C(N,K).
//
//  Discussion:
//
//    The value is calculated in such a way as to avoid overflow and
//    roundoff.  The calculation is done in integer arithmetic.
//
//    The formula used is:
//
//      C(N,K) = N// / ( K// * (N-K)// )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 November 2007
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    ML Wolfson, HV Wright,
//    Algorithm 160:
//    Combinatorial of M Things Taken N at a Time,
//    Communications of the ACM,
//    Volume 6, Number 4, April 1963, page 161.
//
//  Parameters:
//
//    Input, int N, K, the values of N and K.
//
//    Output, int I4_CHOOSE, the number of combinations of N
//    things taken K at a time.
//
{
  int i;
  int mn;
  int mx;
  int value;

  mn = i4_min ( k, n - k );

  if ( mn < 0 )
  {
    value = 0;
  }
  else if ( mn == 0 )
  {
    value = 1;
  }
  else
  {
    mx = i4_max ( k, n - k );
    value = mx + 1;

    for ( i = 2; i <= mn; i++ )
    {
      value = ( value * ( mx + i ) ) / i;
    }
  }

  return value;
}
//****************************************************************************80

int i4_div_rounded ( int a, int b )

//****************************************************************************80
//
//  Purpose:
//
//    I4_DIV_ROUNDED computes the rounded result of I4 division.
//
//  Discussion:
//
//    This routine computes C = A / B, where A, B and C are integers
//    and C is the closest integer value to the exact real result.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int A, B, the number to be divided,
//    and the divisor.
//
//    Output, int I4_DIV_ROUNDED, the rounded result
//    of the division.
//
{
  int a_abs;
  int b_abs;
  int c;
  int c_abs;
  int c_s;
  int i4_huge = 2147483647;

  if ( a == 0 )
  {
    c_abs = i4_huge;
    c_s = i4_sign ( b );
  }
  else
  {
    a_abs = abs ( a );
    b_abs = abs ( b );
    c_s = i4_sign ( a ) * i4_sign ( b );

    c_abs = a_abs / b_abs;

    if ( ( 2 * c_abs + 1 ) * b_abs < 2 * a_abs )
    {
      c_abs = c_abs + 1;
    }
  }

  c = c_s * c_abs;

  return c;
}
//****************************************************************************80

int i4_divp ( int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_DIVP returns the smallest multiple of J greater than or equal to an I4.
//
//  Example:
//
//    I  J  I4_DIVP(I,J)
//
//    0  4    0
//    1  4    1
//    2  4    1
//    3  4    1
//    4  4    1
//    5  4    2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number to be analyzed.
//
//    Input, int J, the number, multiples of which will
//    be compared against I.  J may not be zero.
//
//    Output, int I4_DIVP, the smallest multiple of J that
//    is greater than or equal to I.
//
{
  int value;

  if ( j == 0 )
  {
    cout << "\n";
    cout << "I4_DIVP - Fatal error!\n";
    cout << "  The input value of J was zero.\n";
    exit ( 1 );
  }

  value = 1 + ( i - 1 ) / j;

  return value;
}
//****************************************************************************80

bool i4_even ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_EVEN returns TRUE if an I4 is even.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer to be tested.
//
//    Output, bool I4_EVEN, is TRUE if I is even.
//
{
  bool value;

  value = ( ( i % 2 ) == 0 );

  return value;
}
//****************************************************************************80

int i4_factorial ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_FACTORIAL computes the factorial of N.
//
//  Discussion:
//
//    factorial ( N ) = product ( 1 <= I <= N ) I
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the factorial function.
//    If N is less than 1, the function value is returned as 1.
//    0 <= N <= 13 is required.
//
//    Output, int I4_FACTORIAL, the factorial of N.
//
{
  int i;
  int value;

  value = 1;

  if ( 13 < n ) 
  {
    value = - 1;
    cout << "I4_FACTORIAL - Fatal error!\n";
    cout << "  I4_FACTORIAL(N) cannot be computed as an integer\n";
    cout << "  for 13 < N.\n";
    cout << "  Input value N = " << n << "\n";
    exit ( 1 );
  }

  for ( i = 1; i <= n; i++ )
  {
    value = value * i;
  }

  return value;
}
//****************************************************************************80

int i4_gcd ( int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_GCD finds the greatest common divisor of two I4's.
//
//  Discussion:
//
//    Note that only the absolute values of I and J are
//    considered, so that the result is always nonnegative.
//
//    If I or J is 0, I4_GCD is returned as max ( 1, abs ( I ), abs ( J ) ).
//
//    If I and J have no common factor, I4_GCD is returned as 1.
//
//    Otherwise, using the Euclidean algorithm, I4_GCD is the
//    greatest common divisor of I and J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, two numbers whose GCD is desired.
//
//    Output, int I4_GCD, the greatest common divisor of I and J.
//
{
  int p;
  int q;
  int r;
//
//  Return immediately if either I or J is zero.
//
  if ( i == 0 )
  {
    q = i4_max ( 1, abs ( j ) );
    return q;
  }
  else if ( j == 0 )
  {
    q = i4_max ( 1, abs ( i ) );
    return q;
  }
//
//  Set IP to the larger of I and J, IQ to the smaller.
//  This way, we can alter IP and IQ as we go.
//
  p = i4_max ( abs ( i ), abs ( j ) );
  q = i4_min ( abs ( i ), abs ( j ) );
//
//  Carry out the Euclidean algorithm.
//
  for ( ; ; )
  {
    r = p % q;

    if ( r == 0 )
    {
      break;
    }
    p = q;
    q = r;
  }

  return q;
}
//****************************************************************************80

int i4_gcdb ( int i, int j, int k )

//****************************************************************************80
//
//  Purpose:
//  
//    I4_GCDB finds the greatest common divisor of the form K**N of two I4's.
//
//  Discussion:
//
//    Note that if J is negative, I4_GCDB will also be negative.
//    This is because it is likely that the caller is forming
//    the fraction I/J, and so any minus sign should be
//    factored out of J.
//
//    If I and J are both zero, I4_GCDB is returned as 1.
//
//    If I is zero and J is not, I4_GCDB is returned as J,
//    and vice versa.
//
//    If I and J are nonzero, and have no common divisor of the
//    form K**N, I4_GCDB is returned as 1.
//
//    Otherwise, I4_GCDB is returned as the largest common divisor
//    of the form K**N shared by I and J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, two numbers whose greatest common divisor K**N
//    is desired.
//
//    Input, int K, the possible divisor of I and J.
//
//    Output, int I4_GCDB, the greatest common divisor of
//    the form K**N shared by I and J.
//
{
  int value;

  value = 1;
//
//  If both I and J are zero, I4_GCDB is 1.
//
  if ( i == 0 && j == 0 )
  {
    value = 1;
    return value;
  }
//
//  If just one of I and J is zero, I4_GCDB is the other one.
//
  if ( i == 0 )
  {
    value = j;
    return value;
  }
  else if ( j == 0 )
  {
    value = i;
    return value;
  }
//
//  Divide out K as long as you can.
//
  if ( 0 < j )
  {
    value = 1;
  }
  else
  {
    value = -1;
  }

  for ( ; ; )
  {
    if ( ( i % k ) != 0 || ( j % k ) != 0 )
    {
      break;
    }

    value = value * k;
    i = i / k;
    j = j / k;

  }

  return value;
}
//****************************************************************************80

int i4_huge ( )

//****************************************************************************80
//
//  Purpose:
//
//    I4_HUGE returns a "huge" I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, int I4_HUGE, a "huge" I4.
//
{
  return 2147483647;
}
//****************************************************************************80

double i4_huge_normalizer ( )

//****************************************************************************80
//
//  Purpose:
//
//    I4_HUGE_NORMALIZER returns the "normalizer" for I4_HUGE.
//
//  Discussion:
//
//    The value returned is 1 / ( I4_HUGE + 1 ).
//
//    For any I4, it should be the case that
//
//     -1 < I4 * I4_HUGE_NORMALIZER < 1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double I4_HUGE_NORMALIZER, the "normalizer"
//    for I4_HUGE.
//
{
  double value = 4.656612873077392578125E-10;

  return value;
}
//****************************************************************************80

bool i4_is_power_of_2 ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_IS_POWER_OF_2 reports whether an I4 is a power of 2.
//
//  Discussion:
//
//    The powers of 2 are 1, 2, 4, 8, 16, and so on.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the integer to be tested.
//
//    Output, bool I4_IS_POWER_OF_2, is TRUE if N is a power of 2.
//
{
  if ( n <= 0 )
  {
    return false;
  }

  while ( n != 1 )
  {
    if ( ( n % 2 ) == 1 )
    {
      return false;
    }
    n = n / 2;
  }

  return true;
}
//****************************************************************************80

bool i4_is_prime ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_IS_PRIME reports whether an I4 is prime.
//
//  Discussion:
//
//    A simple, unoptimized sieve of Erasthosthenes is used to
//    check whether N can be divided by any integer between 2
//    and SQRT(N).
//
//    Note that negative numbers, 0 and 1 are not considered prime.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the integer to be tested.
//
//    Output, bool I4_IS_PRIME, is TRUE if N is prime, and FALSE
//    otherwise.  
//
{
  int i;
  int nhi;

  if ( n <= 0 )
  {
    return false;
  }

  if ( n == 1 )
  {
    return false;
  }

  if ( n <= 3 )
  {
    return true;
  }

  nhi = ( int ) ( sqrt ( ( double ) ( n ) ) );

  for ( i = 2; i <= nhi; i++ )
  {
    if ( ( n % i ) == 0 )
    {
      return false;
    }
  }

  return true;
}
//****************************************************************************80

int i4_lcm ( int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_LCM computes the least common multiple of two I4's.
//
//  Discussion:
//
//    The least common multiple may be defined as
//
//      LCM(I,J) = ABS( I * J ) / GCF(I,J)
//
//    where GCF(I,J) is the greatest common factor of I and J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, the integers whose LCM is desired.
//
//    Output, int I4_LCM, the least common multiple of I and J.
//    I4_LCM is never negative.  I4_LCM is 0 if either I or J is zero.
//
{
  int value;

  value = abs ( i * ( j / i4_gcd ( i, j ) ) );

  return value;
}
//****************************************************************************80

int i4_log_10 ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_LOG_10 returns the integer part of the logarithm base 10 of an I4.
//
//  Example:
//
//        I  I4_LOG_10
//    -----  --------
//        0    0
//        1    0
//        2    0
//        9    0
//       10    1
//       11    1
//       99    1
//      100    2
//      101    2
//      999    2
//     1000    3
//     1001    3
//     9999    3
//    10000    4
//
//  Discussion:
//
//    I4_LOG_10 ( I ) + 1 is the number of decimal digits in I.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number whose logarithm base 10 is desired.
//
//    Output, int I4_LOG_10, the integer part of the logarithm base 10 of
//    the absolute value of X.
//
{
  int i_abs;
  int ten_pow;
  int value;

  if ( i == 0 )
  {
    value = 0;
  }
  else
  {
    value = 0;
    ten_pow = 10;

    i_abs = abs ( i );

    while ( ten_pow <= i_abs )
    {
      value = value + 1;
      ten_pow = ten_pow * 10;
    }

  }

  return value;
}
//****************************************************************************80

int i4_log_2 ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_LOG_2 returns the integer part of the logarithm base 2 of an I4.
//
//  Example:
//
//        I  I4_LOG_10
//    -----  --------
//        0    0
//        1    0
//        2    1
//        3    1
//        4    2
//        5    2
//        7    2
//        8    3
//        9    3
//     1000    9
//     1024   10
//
//  Discussion:
//
//    I4_LOG_2 ( I ) + 1 is the number of binary digits in I.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number whose logarithm base 2 is desired.
//
//    Output, int I4_LOG_2, the integer part of the logarithm base 2 of
//    the absolute value of X.
//
{
  int i_abs;
  int two_pow;
  int value;

  if ( i == 0 )
  {
    value = 0;
  }
  else
  {
    value = 0;
    two_pow = 2;

    i_abs = abs ( i );

    while ( two_pow <= i_abs )
    {
      value = value + 1;
      two_pow = two_pow * 2;
    }
  }

  return value;
}
//****************************************************************************80

int i4_log_i4 ( int i4, int j4 )

//****************************************************************************80
//
//  Purpose:
//
//    I4_LOG_I4 returns the logarithm of an I4 to an I4 base.
//
//  Discussion:
//
//    Only the integer part of the logarithm is returned.
//
//    If 
//
//      K4 = I4_LOG_J4 ( I4, J4 ),
//
//    then we ordinarily have
//
//      J4^(K4-1) < I4 <= J4^K4.
//
//    The base J4 should be positive, and at least 2.  If J4 is negative,
//    a computation is made using the absolute value of J4.  If J4 is
//    -1, 0, or 1, the logarithm is returned as 0.
//
//    The number I4 should be positive and at least 2.  If I4 is negative,
//    a computation is made using the absolute value of I4.  If I4 is
//    -1, 0, or 1, then the logarithm is returned as 0.
//
//    An I4 is an integer ( kind = 4 ) value.
//
//  Example:
//
//    I4  J4  K4
//
//     0   3   0
//     1   3   0
//     2   3   0
//     3   3   1
//     4   3   1
//     8   3   1
//     9   3   2
//    10   3   2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I4, the number whose logarithm is desired.
//
//    Input, int J4, the base of the logarithms.
//
//    Output, int I4_LOG_I4, the integer part of the logarithm
//    base abs(J4) of abs(I4).
//
{
  int i4_abs;
  int j4_abs;
  int value;

  value = 0;

  i4_abs = abs ( i4 );

  if ( 2 <= i4_abs )
  {
    j4_abs = abs ( j4 );

    if ( 2 <= j4_abs )
    {
      while ( j4_abs <= i4_abs )
      {
        i4_abs = i4_abs / j4_abs;
        value = value + 1;
      }
    }
  }
  return value;
}
//****************************************************************************80

int i4_log_r8 ( int x, double b )

//****************************************************************************80
//
//  Purpose:
//
//    I4_LOG_R8 returns the integer part of the logarithm base B of an I4.
//
//  Discussion:
//
//    The base B is normally positive, but in any case only the absolute value
//    of B is considered.
//
//    The number X is normally positive, but in any case only the absolute
//    value of X is considered.
//
//  Example:
//
//    If B is greater than 1, and X is positive:
//
//    if 1/B**2 <  X <= 1/B   I4_LOG_R8(X) = -1,
//    if 1/B    <  X <= 1     I4_LOG_R8(X) = 0,
//    if 1      <= X <  B,    I4_LOG_R8(X) = 0,
//    if B      <= X <  B**2  I4_LOG_R8(X) = 1,
//    if B**2   <= X <  B**3  I4_LOG_R8(X) = 2.
//
//    For positive I4_LOG_R8(X), it should be true that
//
//      ABS(B)**I4_LOG_R8(X) <= ABS(X) < ABS(B)**(I4_LOG_R8(X)+1).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X, the number whose logarithm base B is desired.
//    If X is 0, then I4_LOG_B is returned as -HUGE().
//
//    Input, double B, the absolute value of the base of the
//    logarithms.  B must not be -1, 0, or 1.
//
//    Output, int I4_LOG_R8, the integer part of the logarithm
//    base abs(B) of abs(X).
//
{
  double b_abs;
  int i4_huge = 2147483647;
  double x_abs;
  int value;
  int value_sign;

  if ( x == 0 )
  {
    value = - i4_huge;
    return value;
  }

  b_abs = r8_abs ( b );
  value = 0;

  if ( b_abs == 1.0 )
  {
    return value;
  }

  if ( b == 0.0 )
  {
    return value;
  }

  x_abs = r8_abs ( ( double ) ( x ) );

  if ( b_abs < 1.0 )
  {
    value_sign = -1;
    b_abs = 1.0 / b_abs;
  }
  else
  {
    value_sign = +1;
  }

  if ( 1.0 <= x_abs && x_abs < b_abs )
  {
    value = value_sign * value;
    return value;
  }

  while ( b_abs < x_abs )
  {
    x_abs = x_abs / b_abs;
    value = value + 1;
  }

  while ( x_abs * b_abs <= 1.0 )
  {
    x_abs = x_abs * b_abs;
    value = value - 1;
  }
//
//  If the absolute value of the base was less than 1, we inverted
//  earlier.  Now negate the logarithm to account for that.
//
  value = value_sign * value;

  return value;
}
//****************************************************************************80

void i4_mant ( double x, int *s, int *j, int *k, int *l )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MANT computes the "mantissa" of an R4.
//
//  Discussion:
//
//    I4_MANT computes the "mantissa" or "fraction part" of a real
//    number X, which it stores as a pair of integers, (J/K).
//
//    It also computes the sign, and the integer part of the logarithm
//    (base 2) of X.
//
//    On return:
//
//      X = S * (J/K) * 2**L
//
//    where 
//
//      S is +1 or -1,
//      K is a power of 2, 
//      1 <= (J/K) < 2, 
//      L is an integer.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 June 2000
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the real number to be decomposed.
//
//    Output, int *S, the "sign" of the number.
//    S will be -1 if X is less than 0, and +1 if X is greater
//    than or equal to zero.
//
//    Output, int J, the top part of the mantissa fraction.
//
//    Output, int K, the bottom part of the mantissa
//    fraction.  K is a power of 2.
//
//    Output, int L, the integer part of the logarithm (base 2) of X.
//
{
//
//  1: Handle the special case of 0.
//
  if ( x == 0.0 )
  {
    *s = 1;
    *j = 0;
    *k = 1;
    *l = 0;
    return;
  }
//
//  2: Determine the sign IS.
//
  if ( 0.0 < x )
  {
    *s = 1;
  }
  else
  {
    *s = -1;
    x = -x;
  }
//
//  3: Force X to lie between 1 and 2, and compute the logarithm L.
//
  *l = 0;

  while ( 2.0 <= x )
  {
    x = x / 2.0;
    *l = *l + 1;
  }

  while ( x < 1.0 )
  {
    x = x * 2.0;
    *l = *l - 1;
  }
//
//  4: Now strip out the mantissa as J/K.
//
  *j = 0;
  *k = 1;

  for ( ; ; )
  {
    *j = 2 * (*j);

    if ( 1.0 <= x )
    {
      *j = *j + 1;
      x = x - 1.0;
    }

    if ( x == 0.0 )
    {
      break;
    }

    *k = 2 * (*k);
    x = x * 2.0;

  }

  return;
}
//****************************************************************************80

int i4_max ( int i1, int i2 )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MAX returns the maximum of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 October 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, are two integers to be compared.
//
//    Output, int I4_MAX, the larger of I1 and I2.
//
{
  int value;

  if ( i2 < i1 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
//****************************************************************************80

int i4_min ( int i1, int i2 )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MIN returns the minimum of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 October 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, two integers to be compared.
//
//    Output, int I4_MIN, the smaller of I1 and I2.
//
{
  int value;

  if ( i1 < i2 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
//****************************************************************************80

void i4_moddiv ( int n, int d, int *m, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MODDIV breaks an I4 into a multiple of a divisor and remainder.
//
//  Discussion:
//
//    N = M * D + R
//
//    0 <= || R || < || D ||
//
//    R has the sign of N.
//
//  Example:
//
//    N         D       M      R
//
//   107       50      2      7
//   107      -50     -2      7
//  -107       50     -2     -7
//  -107      -50      2     -7
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number to be decomposed.
//
//    Input, int D, the divisor.  D may not be zero.
//
//    Output, int *M, the number of times N
//    is evenly divided by D.
//
//    Output, int *R, a remainder.
//
{
  if ( d == 0 )
  {
    cout << "\n";
    cout << "I4_MODDIV - Fatal error!\n";
    cout << "  Input divisor D = 0\n";
    exit ( 1 );
  }

  *m = n / d;
  *r = n - d * (*m);

  return;
}
//****************************************************************************80

int i4_modp ( int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MODP returns the nonnegative remainder of I4 division.
//
//  Discussion:
//
//    If 
//      NREM = I4_MODP ( I, J ) 
//      NMULT = ( I - NREM ) / J
//    then
//      I = J * NMULT + NREM
//    where NREM is always nonnegative.
//
//    The MOD function computes a result with the same sign as the
//    quantity being divided.  Thus, suppose you had an angle A,
//    and you wanted to ensure that it was between 0 and 360.
//    Then mod(A,360) would do, if A was positive, but if A
//    was negative, your result would be between -360 and 0.
//
//    On the other hand, I4_MODP(A,360) is between 0 and 360, always.
//
//        I         J     MOD  I4_MODP   I4_MODP Factorization
// 
//      107        50       7       7    107 =  2 *  50 + 7
//      107       -50       7       7    107 = -2 * -50 + 7
//     -107        50      -7      43   -107 = -3 *  50 + 43
//     -107       -50      -7      43   -107 =  3 * -50 + 43
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number to be divided.
//
//    Input, int J, the number that divides I.
//
//    Output, int I4_MODP, the nonnegative remainder when I is 
//    divided by J.
//
{
  int value;

  if ( j == 0 )
  {
    cout << "\n";
    cout << "I4_MODP - Fatal error!\n";
    cout << "  I4_MODP ( I, J ) called with J = " << j << "\n";
    exit ( 1 );
  }

  value = i % j;

  if ( value < 0 )
  {
    value = value + abs ( j );
  }

  return value;
}
//****************************************************************************80

int i4_mop ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_MOP returns the I-th power of -1 as an I4 value.
//
//  Discussion:
//
//    An I4 is an int value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 November 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the power of -1.
//
//    Output, int I4_MOP, the I-th power of -1.
//
{
  int value;

  if ( ( i % 2 ) == 0 )
  {
    value = 1;
  }
  else
  {
    value = -1;
  }

  return value;
}
//****************************************************************************80

bool i4_odd ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_ODD returns TRUE if an I4 is odd.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer to be tested.
//
//    Output, bool I4_ODD, is TRUE if I is odd.
//
{
  bool value;

  value = ( ( i % 2 ) != 0 );

  return value;
}
//****************************************************************************80

int i4_power ( int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_POWER returns the value of I^J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, the base and the power.  J should be nonnegative.
//
//    Output, int I4_POWER, the value of I^J.
//
{
  int k;
  int value;

  if ( j < 0 )
  {
    if ( i == 1 )
    {
      value = 1;
    }
    else if ( i == 0 )
    {
      cout << "\n";
      cout << "I4_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J negative.\n";
      exit ( 1 );
    }
    else
    {
      value = 0;
    }
  }
  else if ( j == 0 )
  {
    if ( i == 0 )
    {
      cout << "\n";
      cout << "I4_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J = 0.\n";
      exit ( 1 );
    }
    else
    {
      value = 1;
    }
  }
  else if ( j == 1 )
  {
    value = i;
  }
  else
  {
    value = 1;
    for ( k = 1; k <= j; k++ )
    {
      value = value * i;
    }
  }
  return value;
}
//****************************************************************************80

int i4_reverse_bytes ( int x )

//****************************************************************************80
//
//  Purpose:
//
//    I4_REVERSE_BYTES reverses the bytes in an I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X, a value whose bytes are to be reversed.
//
//    Output, int I4_REVERSE_BYTES, a value whose bytes are
//    in reverse order from those in X.
//
{
  char c;
  union
  {
    int yint;
    char ychar[4];
  } y;

  y.yint = x;
  
  c = y.ychar[0];
  y.ychar[0] = y.ychar[3];
  y.ychar[3] = c;

  c = y.ychar[1];
  y.ychar[1] = y.ychar[2];
  y.ychar[2] = c;

  return ( y.yint );
}
//****************************************************************************80

int i4_sign ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_SIGN returns the sign of an I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer whose sign is desired.
//
//    Output, int I4_SIGN, the sign of I.
{
  int value;

  if ( i < 0 ) 
  {
    value = -1;
  }
  else
  {
    value = 1;
  }
  return value;
}
//****************************************************************************80

void i4_swap ( int *i, int *j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_SWAP switches two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 January 2002
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *I, *J.  On output, the values of I and
//    J have been interchanged.
//
{
  int k;

  k = *i;
  *i = *j;
  *j = k;
 
  return;
}
//****************************************************************************80

void i4_swap3 ( int *i, int *j, int *k )

//****************************************************************************80
//
//  Purpose:
//
//    I4_SWAP3 swaps three I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *I, *J, *K.  On output, the values of I, J, and K
//    have been interchanged.
//
{
  int l;

   l = *i;
  *i = *j;
  *j = *k;
  *k =  l;

  return;
}
//****************************************************************************80

double i4_to_angle ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_TO_ANGLE maps I4's to points on a circle.
//
//  Discussion:
//
//    The angles are intended to be used to select colors on a color
//    hexagon whose 6 vertices are red, yellow, green, cyan, blue,
//    magenta.
//
//  Example:
//
//     I   X      ANGLE
//
//     0   0/3      0
//     1   1/3    120
//     2   2/3    240
//
//     3   1/6     60
//     4   3/6    180
//     5   5/6    300
//
//     6   1/12    30
//     7   3/12    90
//     8   5/12   150
//     9   7/12   210
//    10   9/12   270
//    11  11/12   330
//
//    12   1/24    15
//    13   3/24    45
//    14   5/24    75
//    etc
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 January 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the index of the desired color.
//
//    Output, double I4_TO_ANGLE, an angle, measured in degrees, between 0 and 360.
//
{
  double angle;
  int i1;
  int i2;
  int i3;
  int i4;

  if ( 0 <= abs ( i ) && abs ( i ) <= 2 )
  {
    angle = 120.0 * ( double ) ( abs ( i ) );
  }
  else
  {
    i1 = i4_log_2 ( abs ( i ) / 3 );
    i2 = abs ( i ) + 1 - 3 * i4_power ( 2, i1 );
    i3 = 2 * ( i2 - 1 ) + 1;
    i4 = 3 * i4_power ( 2, ( i1 + 1 ) );

    angle = 360.0 * ( double ) ( i3 ) / ( double ) ( i4 );
  }

  return angle;
}
//****************************************************************************80

int *i4_to_digits_decimal ( int i, int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4_TO_DIGITS_DECIMAL determines the last N decimal digits of an I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer to be analyzed.
//
//    Input, int N, the number of digits to determine.
//
//    Output, int I4_TO_DIGITS_DECIMAL[N], the last N decimal digits of I.
//    DIGIT[I-1] is the "coefficient" of 10**(I-1).
//
{
  int *digit;
  int j;

  digit = new int[n];

  i = abs ( i );

  for ( j = 1; j <= n; j++ )
  {
    digit[j-1] = i % 10;
    i = ( i - digit[j-1] ) / 10;
  }

  return digit;
}
//****************************************************************************80

int *i4_to_fac ( int i, int prime_num )

//****************************************************************************80
//
//  Purpose:
//
//    I4_TO_FAC converts an I4 into a product of prime factors.
//
//  Discussion:
//
//    This routine will fail if the input integer is not positive,
//    or if PRIME_NUM is too small to account for the factors of the integer.
//
//    The formula is:
//
//      I = Product ( 1 <= J <= PRIME_NUM ) PRIME(J)**NPOWER(J).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the integer to be factored.
//
//    Input, int PRIME_NUM, the number of prime factors for
//    which storage has been allocated.
//
//    Output, int I4_TO_FAC[PRIME_NUM], the powers of the primes.
//
{
  int j;
  int *npower;
  int p;

  if ( i <= 0 )
  {
    cout << "\n";
    cout << "I4_TO_FAC - Fatal error!\n";
    cout << "  Input integer I is not positive.\n";
    exit ( 1 );
  }

  npower = new int[prime_num];
//
//  Try dividing the remainder by each prime.
//
  for ( j = 1; j <= prime_num; j++ )
  {
    npower[j-1] = 0;

    p = prime ( j );

    while ( ( i % p ) == 0 )
    {
      npower[j-1] = npower[j-1] + 1;
      i = i / p;
    }

  }
  return npower;
}
//****************************************************************************80

void i4_to_halton ( int dim_num, int step, int seed[], int leap[], int base[], 
  double r[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4_TO_HALTON computes one element of a leaped Halton subsequence.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 July 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    John Halton,
//    On the efficiency of certain quasi-random sequences of points
//    in evaluating multi-dimensional integrals,
//    Numerische Mathematik,
//    Volume 2, 1960, pages 84-90.
//
//    John Halton, GB Smith,
//    Algorithm 247: Radical-Inverse Quasi-Random Point Sequence,
//    Communications of the ACM,
//    Volume 7, 1964, pages 701-702.
//
//    Ladislav Kocis, William Whiten,
//    Computational Investigations of Low-Discrepancy Sequences,
//    ACM Transactions on Mathematical Software,
//    Volume 23, Number 2, 1997, pages 266-294.
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//    1 <= DIM_NUM is required.
//
//    Input, int STEP, the index of the subsequence element.
//    0 <= STEP is required.
//
//    Input, int SEED[DIM_NUM], the Halton sequence index corresponding 
//    to STEP = 0.
//    0 <= SEED(1:DIM_NUM) is required.
//
//    Input, int LEAP[DIM_NUM], the successive jumps in the Halton sequence.
//    1 <= LEAP(1:DIM_NUM) is required.
//
//    Input, int BASE[DIM_NUM], the Halton bases.
//    1 < BASE(1:DIM_NUM) is required.
//
//    Output, double R[DIM_NUM], the STEP-th element of the leaped 
//    Halton subsequence.
//
{
  double base_inv;
  int digit;
  int i;
  int seed2;
//
//  Check the input.
//
//if ( !halham_dim_num_check ( dim_num ) )
//{
//  exit ( 1 );
//}
//
//if ( !halham_step_check ( step ) )
//{
//  exit ( 1 );
//}
//if ( !halham_seed_check ( dim_num, seed ) )
//{
//  exit ( 1 );
//}
//
//if ( !halham_leap_check ( dim_num, leap ) )
//{
//  exit ( 1 );
//}
//
//if ( !halton_base_check ( dim_num, base ) )
//{
//  exit ( 1 );
//}
//
//  Calculate the data.
//
  for ( i = 0; i < dim_num; i++ )
  {
    seed2 = seed[i] + step * leap[i];

    r[i] = 0.0;

    base_inv = 1.0 / ( ( double ) base[i] );

    while ( seed2 != 0 )
    {
      digit = seed2 % base[i];
      r[i] = r[i] + ( ( double ) digit ) * base_inv;
      base_inv = base_inv / ( ( double ) base[i] );
      seed2 = seed2 / base[i];
    }
  }

  return;
}
//****************************************************************************80

char i4_to_isbn ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_TO_ISBN converts an I4 to an ISBN digit.
//
//  Discussion:
//
//    Only the integers 0 through 10 can be input.  The representation
//    of 10 is 'X'.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Book Industry Study Group,
//    The Evolution in Product Identification:
//    Sunrise 2005 and the ISBN-13,
//    http://www.bisg.org/docs/The_Evolution_in_Product_ID.pdf
//
//  Parameters:
//
//    Input, int I, an integer between 0 and 10.
//
//    Output, char I4_TO_ISBN, the ISBN character code of the integer.
//    If I is illegal, then I4_TO_ISBN is set to '?'.
//
{
       if ( i == 0 )
  {
    return '0';
  }
  else if ( i == 1 )
  {
    return '1';
  }
  else if ( i == 2 )
  {
    return '2';
  }
  else if ( i == 3 )
  {
    return '3';
  }
  else if ( i == 4 )
  {
    return '4';
  }
  else if ( i == 5 )
  {
    return '5';
  }
  else if ( i == 6 )
  {
    return '6';
  }
  else if ( i == 7 )
  {
    return '7';
  }
  else if ( i == 8 )
  {
    return '8';
  }
  else if ( i == 9 )
  {
    return '9';
  }
  else if ( i == 10 )
  {
    return 'X';
  }
  else
  {
    return '?';
  }
}
//****************************************************************************80

int i4_uniform ( int a, int b, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4_UNIFORM returns a scaled pseudorandom I4.
//
//  Discussion:
//
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley Interscience, page 95, 1998.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Peter Lewis, Allen Goodman, James Miller
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input, int A, B, the limits of the interval.
//
//    Input/output, int *SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, int I4_UNIFORM, a number between A and B.
//
{
  int k;
  float r;
  int value;

  if ( *seed == 0 )
  {
    cerr << "\n";
    cerr << "I4_UNIFORM - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }

  r = ( float ) ( *seed ) * 4.656612875E-10;
//
//  Scale R to lie between A-0.5 and B+0.5.
//
  r = ( 1.0 - r ) * ( ( float ) ( i4_min ( a, b ) ) - 0.5 ) 
    +         r   * ( ( float ) ( i4_max ( a, b ) ) + 0.5 );
//
//  Use rounding to convert R to an integer between A and B.
//
  value = r4_nint ( r );

  value = i4_max ( value, i4_min ( a, b ) );
  value = i4_min ( value, i4_max ( a, b ) );

  return value;
}
//****************************************************************************80

void i4_unswap3 ( int *i, int *j, int *k )

//****************************************************************************80
//
//  Purpose:
//
//    I4_UNSWAP3 unswaps three I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *I, *J, *K.  On output, the values of I, J, and K
//    have been interchanged.
//
{
  int l;

   l = *k;
  *k = *j;
  *j = *i;
  *i =  l;

  return;
}
//****************************************************************************80

int i4_walsh_1d ( double x, int digit )

//****************************************************************************80
//
//  Purpose:
//
//    I4_WALSH_1D evaluates the Walsh function of a real scalar argument.
//
//  Discussion:
//
//    Consider the binary representation of X, and number the digits 
//    in descending order, from leading to lowest, with the units digit
//    being numbered 0.
//
//    The Walsh function W(J)(X) is equal to the J-th binary digit of X.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the argument of the Walsh function.
//
//    Input, int DIGIT, the index of the Walsh function.
//
//    Output, int I4_WALSH_1D, the value of the Walsh function.
//
{
  int n;
  int value;
//
//  Hide the effect of the sign of X.
//
  x = r8_abs ( x );
//
//  If DIGIT is positive, divide by 2 DIGIT times.
//  If DIGIT is negative, multiply by 2 (-DIGIT) times.
//
  if ( 0 <= digit ) 
  {
    x = x / ( double ) i4_power ( 2, digit );
  }
  else
  {
    x = x * ( double ) i4_power ( 2, -digit );
  }
//
//  Make it an integer.
//  Because it's positive, and we're using INT, we don't change the
//  units digit.
//
  n = ( int ) x ;
//
//  Is the units digit odd or even?
//
  if ( ( n % 2 ) == 0 )
  {
    value = 0;
  }
  else
  {
    value = 1;
  }

  return value;
}
//****************************************************************************80

int i4_width ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    I4_WIDTH returns the "width" of an I4.
//
//  Example:
//
//        I  I4_WIDTH
//    -----  -------
//    -1234    5
//     -123    4
//      -12    3
//       -1    2
//        0    1
//        1    1
//       12    2
//      123    3
//     1234    4
//    12345    5
//
//  Discussion:
//
//    The width of an integer is the number of characters necessary to print it.
//
//    The width of an integer can be useful when setting the appropriate output
//    format for a vector or array of values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the number whose width is desired.
//
//    Output, int I4_WIDTH, the number of characters necessary to represent
//    the integer in base 10, including a negative sign if necessary.
//
{
  int width;

  if ( 0 <= i )
  {
    width = i4_log_10 ( i ) + 1;
  }
  else
  {
    width = i4_log_10 ( i ) + 2;
  }

  return width;
}
//****************************************************************************80

int i4_wrap ( int ival, int ilo, int ihi )

//****************************************************************************80
//
//  Purpose:
//
//    I4_WRAP forces an I4 to lie between given limits by wrapping.
//
//  Example:
//
//    ILO = 4, IHI = 8
//
//    I   Value
//
//    -2     8
//    -1     4
//     0     5
//     1     6
//     2     7
//     3     8
//     4     4
//     5     5
//     6     6
//     7     7
//     8     8
//     9     4
//    10     5
//    11     6
//    12     7
//    13     8
//    14     4
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int IVAL, an integer value.
//
//    Input, int ILO, IHI, the desired bounds for the integer value.
//
//    Output, int I4_WRAP, a "wrapped" version of IVAL.
//
{
  int jhi;
  int jlo;
  int value;
  int wide;

  jlo = i4_min ( ilo, ihi );
  jhi = i4_max ( ilo, ihi );

  wide = jhi + 1 - jlo;

  if ( wide == 1 )
  {
    value = jlo;
  }
  else
  {
    value = jlo + i4_modp ( ival - jlo, wide );
  }

  return value;
}
//****************************************************************************80

unsigned int i4_xor ( unsigned int i, unsigned int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4_XOR calculates the exclusive OR of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 February 2005
//
//  Author:
//
//   John Burkardt
//
//  Parameters:
//
//    Input, unsigned int I, J, two values whose exclusive OR is needed.
//
//    Output, unsigned int I4_XOR, the exclusive OR of I and J.
//
{
  unsigned int i2;
  unsigned int j2;
  unsigned int k;
  unsigned int l;

  k = 0;
  l = 1;

  while ( i != 0 || j != 0 )
  {
    i2 = i / 2;
    j2 = j / 2;

    if ( 
      ( ( i == 2 * i2 ) && ( j != 2 * j2 ) ) ||
      ( ( i != 2 * i2 ) && ( j == 2 * j2 ) ) )
    {
      k = k + l;
    }

    i = i2;
    j = j2;
    l = 2 * l;
  }

  return k;
}
//****************************************************************************80

void i43mat_flip_cols ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I43MAT_FLIP_COLS swaps the columns of an I43MAT.
//
//  Discussion:
//
//    An I43MAT is a matrix, each of whose entries is an I43, a triple of integers.
//
//    An I43MAT can be stored as a 3 x M x N array, where M counts the "columns"
//    and N counts the "rows".
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 June 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[3*M*N], the matrix whose columns are to be flipped.
//
{
  int b;
  int i;
  int j;
  int k;
  
  for ( k = 0; k < ( n / 2 ); k++ )
  {
    for ( j = 0; j < m; j++ )
    {
      for ( i = 0; i < 3; i++ )
      {
        b                    = a[i+j*3+     k *m*3];
        a[i+j*3+     k *m*3] = a[i+j*3+(n-1-k)*m*3];
        a[i+j*3+(n-1-k)*m*3] = b;
      }
    }
  }

  return;
}
//****************************************************************************80

void i43mat_flip_rows ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I43MAT_FLIP_ROWS swaps the rows of an I43MAT.
//
//  Discussion:
//
//    An I43MAT is a matrix, each of whose entries is an I43, a triple of integers.
//
//    An I43MAT can be stored as a 3 x M x N array, where M counts the "columns"
//    and N counts the "rows".
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 June 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[3*M*N], the matrix whose rows are to be flipped.
//
{
  int b;
  int i;
  int j;
  int k;
  
  for ( k = 0; k < n; k++ )
  {
    for ( j = 0; j < ( m / 2 ); j++ )
    {
      for ( i = 0; i < 3; i++ )
      {
        b                    = a[i+     j *3+k*m*3];
        a[i+     j *3+k*m*3] = a[i+(m-1-j)*3+k*m*3];
        a[i+(m-1-j)*3+k*m*3] = b;
      }
    }
  }

  return;
}
//****************************************************************************80

int i4col_compare ( int m, int n, int a[], int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_COMPARE compares columns I and J of an I4COL.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, I = 2, J = 4
//
//      A = (
//        1  2  3  4
//        5  6  7  8
//        9 10 11 12 )
//
//    Output:
//
//      I4COL_COMPARE = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A[M*N], an array of N columns of vectors of length M.
//
//    Input, int I, J, the columns to be compared.
//    I and J must be between 1 and N.
//
//    Output, int I4COL_COMPARE, the results of the comparison:
//    -1, column I < column J,
//     0, column I = column J,
//    +1, column J < column I.
//
{
  int k;
//
//  Check.
//
  if ( i < 1 )
  {
    cout << "\n";
    cout << "I4COL_COMPARE - Fatal error!\n";
    cout << "  Column index I = " << i << " is less than 1.\n";
    exit ( 1 );
  }

  if ( n < i )
  {
    cout << "\n";
    cout << "I4COL_COMPARE - Fatal error!\n";
    cout << "  N = " << n << " is less than column index I = " << i << ".\n";
    exit ( 1 );
  }

  if ( j < 1 )
  {
    cout << "\n";
    cout << "I4COL_COMPARE - Fatal error!\n";
    cout << "  Column index J = " << j << " is less than 1.\n";
    exit ( 1 );
  }

  if ( n < j )
  {
    cout << "\n";
    cout << "I4COL_COMPARE - Fatal error!\n";
    cout << "  N = " << n << " is less than column index J = " << j << ".\n";
    exit ( 1 );
  }

  if ( i == j )
  {
    return 0;
  }

  k = 1;

  while ( k <= m )
  {
    if ( a[k-1+(i-1)*m] < a[k-1+(j-1)*m] )
    {
      return (-1);
    }
    else if ( a[k-1+(j-1)*m] < a[k-1+(i-1)*m] )
    {
      return 1;
    }
    k = k + 1;
  }

  return 0;
}
//****************************************************************************80

int i4col_find ( int m, int n, int a[], int ivec[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_FIND seeks a table column equal to an I4COL.
//
//  Example:
//
//    M = 3, N = 4,
//
//    A = (
//      1  2  3  4
//      5  6  7  8
//      9 10 11 12 )
//
//    IVEC = ( 3, 7, 11 )
//
//    ICOL = 3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in
//    the table.  M is also the length of IVEC.
//
//    Input, int A[M*N], an array of N columns of vectors
//    of length M.
//
//    Input, int IVEC[M], a vector to be matched with the data
//    in the array.
//
//    Output, int I4COL_FIND, the index of the first column of the table
//    which exactly matches every entry of IVEC, or -1 if no match
//    could be found.
//
{
  int i;
  int col;
  int j;

  if ( m <= 0 )
  {
    col = -1;
    return col;
  }

  for ( j = 1; j <= n; j++ )
  {
    i = 1;

    while ( ivec[i-1] == a[i-1+(j-1)*m] )
    {
      if ( i == m )
      {
        col = j;
        return col;
      }

      i = i + 1;
    }
  }

  col = -1;
  return col;
}
//****************************************************************************80

void i4col_find_item ( int m, int n, int table[], int item, 
  int *row, int *col )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_FIND_ITEM searches an I4COL for a given value.
//
//  Discussion:
//
//    The two dimensional information in TABLE is stored as a one dimensional
//    array, by columns.
//
//    The values ROW and COL will be one-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns
//    in the table.
//
//    Input, int TABLE[M*N], the table to search.
//
//    Input, int ITEM, the value to search for.
//
//    Output, int *ROW, *COL, the row and column indices
//    of the first occurrence of the value ITEM.  The search
//    is conducted by rows.  If the item is not found, then
//    ROW = COL = -1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( table[i+j*m] == item )
      {
        *row = i+1;
        *col = j+1;
        return;
      }
    }
  }

  *row = -1;
  *col = -1;

  return;
}
//****************************************************************************80

void i4col_find_pair_wrap ( int m, int n, int a[], int item1, int item2, 
  int *row, int *col )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_FIND_PAIR_WRAP wrap searches an I4COL for a pair of items.
//
//  Discussion:
//
//    The items (ITEM1, ITEM2) must occur consecutively.
//    However, wrapping is allowed, that is, if ITEM1 occurs
//    in the last row, and ITEM2 "follows" it in the first row
//    of the same column, a match is declared. 
//
//    If the pair of items is not found, then ROW = COL = -1.  
//
//    The values ROW and COL will be one-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns
//    in the table.
//
//    Input, int A[M*N], the table to search.
//
//    Input, int ITEM1, ITEM2, the values to search for.
//
//    Output, int *ROW, *COL, the row and column indices
//    of the first occurrence of the value ITEM1 followed immediately
//    by ITEM2.
//
{
  int i;
  int i2;
  int j;

  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= m; i++ )
    {
      if ( a[i-1+(j-1)*m] == item1 )
      {
        i2 = i + 1;

        if ( m < i2 )
        {
          i2 = 1;
        }

        if ( a[i2-1+(j-1)*m] == item2 )
        {
          *row = i;
          *col = j;
          return;
        }
      }
    }
  }

  *row = -1;
  *col = -1;

  return;
}
//****************************************************************************80

int *i4col_first_index ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_FIRST_INDEX indexes the first occurrence of values in an I4COL.
//
//  Discussion:
//
//    An I4COL is an M by N array of I4 values.
//    It is regarded as an array of N columns of length M.
//
//    For element A(1:M,J) of the matrix, FIRST_INDEX(J) is the index in A of
//    the first column whose entries are equal to A(1:M,J).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//    The length of an "element" of A, and the number of "elements".
//
//    Input, int A[M*N], the array.
//
//    Output, int I4COL_FIRST_INDEX[N], the first occurrence index.
//
{
  int diff;
  int *first_index;
  int i;
  int j1;
  int j2;

  first_index = new int[n];

  for ( j1 = 0; j1 < n; j1++ )
  {
    first_index[j1] = -1;
  }
  for ( j1 = 0; j1 < n; j1++ )
  {
    if ( first_index[j1] == -1 )
    {
      first_index[j1] = j1;

      for ( j2 = j1 + 1; j2 < n; j2++ )
      {
        diff = 0;
        for ( i = 0; i < m; i++ )
        {
          diff = diff + abs ( a[i+j1*m] - a[i+j2*m] );
        }
        if ( diff == 0 )
        {
          first_index[j2] = j1;
        }
      }
    }
  }
  return first_index;
}
//****************************************************************************80

void i4col_sort_a ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORT_A ascending sorts the columns of an I4COL.
//
//  Discussion:
//
//    In lexicographic order, the statement "X < Y", applied to two
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, X is less than Y if, at the first index where they
//    differ, the X value is less than the Y value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A.
//
//    Input/output, int A[M*N].
//    On input, the array of N columns of M vectors;
//    On output, the columns of A have been sorted in ascending
//    lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      i4col_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = i4col_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }

  }

  return;
}
//****************************************************************************80

void i4col_sort_d ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORT_D descending sorts the columns of an I4COL.
//
//  Discussion:
//
//    In lexicographic order, the statement "X < Y", applied to two
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, X is less than Y if, at the first index where they
//    differ, the X value is less than the Y value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A.
//
//    Input/output, int A[M*N].
//    On input, the array of N columns of M vectors;
//    On output, the columns of A have been sorted in descending
//    lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      i4col_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = -i4col_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }

  }

  return;
}
//****************************************************************************80

void i4col_sort2_a ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORT2_A ascending sorts the elements of each column of an I4COL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A, and the length
//    of a vector of data.
//
//    Input/output, int A[M*N].
//    On input, the array of N columns of M vectors.
//    On output, the elements of each column of A have been sorted in ascending
//    order.
//
{
  int col;
  int i;
  int indx;
  int isgn;
  int j;
  int row;
  int temp;

  if ( m <= 1 )
  {
    return;
  }

  if ( n <= 0 )
  {
    return;
  }
//
//  Initialize.
//
  for ( col = 0; col < n; col++ )
  {
    i = 0;
    indx = 0;
    isgn = 0;
    j = 0;
//
//  Call the external heap sorter.
//
    for ( ; ; )
    {
      sort_heap_external ( m, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
      if ( 0 < indx )
      {
        temp       = a[i-1+col*m];
        a[i-1+col*m] = a[j-1+col*m];
        a[j-1+col*m] = temp;
      }
//
//  Compare the I and J objects.
//
      else if ( indx < 0 )
      {
        if ( a[j-1+col*m] < a[i-1+col*m] )
        {
          isgn = +1;
        }
        else
        {
          isgn = -1;
        }
      }
      else if ( indx == 0 )
      {
        break;
      }
    }
  }

  return;
}
//****************************************************************************80

void i4col_sort2_d ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORT2_D descending sorts the elements of each column of an I4COL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A, and the length
//    of a vector of data.
//
//    Input/output, int A[M*N].
//    On input, the array of N columns of M vectors.
//    On output, the elements of each column of A have been sorted in descending
//    order.
//
{
  int col;
  int i;
  int indx;
  int isgn;
  int j;
  int row;
  int temp;

  if ( m <= 1 )
  {
    return;
  }

  if ( n <= 0 )
  {
    return;
  }
//
//  Initialize.
//
  for ( col = 0; col < n; col++ )
  {
    i = 0;
    indx = 0;
    isgn = 0;
    j = 0;
//
//  Call the external heap sorter.
//
    for ( ; ; )
    {
      sort_heap_external ( m, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
      if ( 0 < indx )
      {
        temp       = a[i-1+col*m];
        a[i-1+col*m] = a[j-1+col*m];
        a[j-1+col*m] = temp;
      }
//
//  Compare the I and J objects.
//
      else if ( indx < 0 )
      {
        if ( a[i-1+col*m] < a[j-1+col*m] )
        {
          isgn = +1;
        }
        else
        {
          isgn = -1;
        }
      }
      else if ( indx == 0 )
      {
        break;
      }
    }
  }

  return;
}
//****************************************************************************80

int i4col_sorted_singleton_count ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORTED_SINGLETON_COUNT counts singletons in an I4COL.
//
//  Discussion:
//
//    The columns of the array may be ascending or descending sorted.
//
//    A "singleton" is an item that occurs exactly once.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A[M*N], a sorted array, containing
//    N columns of data.
//
//    Output, int I4COL_SORTED_SINGLETON_COUNT, the number of singletons.
//
{
  bool differ_from_next;
  bool differ_from_previous;
  int i;
  int j;
  int singleton_num;

  singleton_num = 0;

  if ( n <= 0 )
  {
    return singleton_num;
  }

  differ_from_next = true;

  for ( j = 0; j < n; j++ )
  {
    differ_from_previous = differ_from_next;

    if ( j < n )
    {
      differ_from_next = false;
      for ( i = 0; i < m; i++ )
      {
        if ( a[i+j*m] != a[i+(j+1)*m] )
        {
          differ_from_next = true;
          break;
        }
      }
    }
    else
    {
      differ_from_next = true;
    }

    if ( differ_from_previous && differ_from_next )
    {
      singleton_num = singleton_num + 1;
    }
  }

  return singleton_num;
}
//****************************************************************************80

void i4col_sorted_unique ( int m, int n, int a[], int *unique_num )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORTED_UNIQUE keeps unique elements in a sorted I4COL.
//
//  Discussion:
//
//    The array can be sorted into ascending or descending order.
//    The important point is that identical elements must be stored
//    in adjacent positions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A, and the length of
//    a vector of data.
//
//    Input, int N, the number of columns of A.
//
//    Input/output, int A[M*N].
//    On input, the sorted array of N columns of M-vectors.
//    On output, a sorted array of columns of M-vectors.
//
//    Output, int *UNIQUE_NUM, the number of unique columns of A.
//
{
  int i;
  int j1;
  int j2;
  bool same;

  if ( n <= 0 )
  {
    *unique_num = 0;
    return;
  }

  j1 = 1;

  for ( j2 = 2; j2 <= n; j2++ )
  {
    same = true;
    for ( i = 1; i <= m; i++ )
    {
      if ( a[i-1+(j1-1)*m] != a[i-1+(j2-1)] )
      {
        same = false;
        break;
      }
    }
    if ( !same )
    {
      j1 = j1 + 1;
      for ( i = 1; i <= m; i++ )
      {
        a[i-1+(j1-1)*m] = a[i-1+(j2-1)*m];
      }
    }
  }

 *unique_num = j1;

  return;
}
//****************************************************************************80

int i4col_sorted_unique_count ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SORTED_UNIQUE_COUNT counts unique elements in an I4COL.
//
//  Discussion:
//
//    The columns of the array may be ascending or descending sorted.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 February 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A[M*N], a sorted array, containing
//    N columns of data.
//
//    Output, int I4COL_SORTED_UNIQUE_COUNT, the number of unique columns.
//
{
  int i;
  int j1;
  int j2;
  int unique_num;

  if ( n <= 0 )
  {
    unique_num = 0;
    return unique_num;
  }

  unique_num = 1;
  j1 = 0;

  for ( j2 = 1; j2 < n; j2++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( a[i+j1*m] != a[i+j2*m] )
      {
        unique_num = unique_num + 1;
        j1 = j2;
        break;
      }
    }
  }

  return unique_num;
}
//****************************************************************************80

void i4col_swap ( int m, int n, int a[], int icol1, int icol2 )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_SWAP swaps two columns of an I4COL.
//
//  Discussion:
//
//    The two dimensional information is stored as a one dimensional
//    array, by columns.
//
//    The row indices are 1 based, NOT 0 based//  However, a preprocessor
//    variable, called OFFSET, can be reset from 1 to 0 if you wish to
//    use 0-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 April 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[M*N], an array of data.
//
//    Input, int ICOL1, ICOL2, the two columns to swap.
//    These indices should be between 1 and N.
//
{
# define OFFSET 1

  int i;
  int t;
//
//  Check.
//
  if ( icol1 - OFFSET < 0 || n-1 < icol1 - OFFSET )
  {
    cout << "\n";
    cout << "I4COL_SWAP - Fatal error!\n";
    cout << "  ICOL1 is out of range.\n";
    exit ( 1 );
  }

  if ( icol2 - OFFSET < 0 || n-1 < icol2 - OFFSET )
  {
    cout << "\n";
    cout << "I4COL_SWAP - Fatal error!\n";
    cout << "  ICOL2 is out of range.\n";
    exit ( 1 );
  }

  if ( icol1 == icol2 )
  {
    return;
  }
  for ( i = 0; i < m; i++ )
  {
    t                     = a[i+(icol1-OFFSET)*m];
    a[i+(icol1-OFFSET)*m] = a[i+(icol2-OFFSET)*m];
    a[i+(icol2-OFFSET)*m] = t;
  }

  return;
# undef OFFSET
}
//****************************************************************************80

int *i4col_unique_index ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4COL_UNIQUE_INDEX indexes the unique occurrence of values in an I4COL.
//
//  Discussion:
//
//    An I4COL is an M by N array of I4 values.
//    It is regarded as an array of N columns of length M.
//
//    For element A(1:M,J) of the matrix, UNIQUE_INDEX(J) is the uniqueness index
//    of A(1:M,J).  That is, if A_UNIQUE contains the unique elements of A, 
//    gathered in order, then 
//
//      A_UNIQUE ( 1:M, UNIQUE_INDEX(J) ) = A(1:M,J)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//    The length of an "element" of A, and the number of "elements".
//
//    Input, int A[M*N], the array.
//
//    Output, int I4COL_UNIQUE_INDEX[N], the unique index.
//
{
  int diff;
  int i;
  int j1;
  int j2;
  int *unique_index;
  int unique_num;

  unique_index = new int[n];

  for ( j1 = 0; j1 < n; j1++ )
  {
    unique_index[j1] = -1;
  }
  unique_num = 0;

  for ( j1 = 0; j1 < n; j1++ )
  {
    if ( unique_index[j1] == -1 )
    {
      unique_index[j1] = unique_num;

      for ( j2 = j1 + 1; j2 < n; j2++ )
      {
        diff = 0;
        for ( i = 0; i < m; i++ )
        {
          diff = diff + abs ( a[i+j1*m] - a[i+j2*m] );
        }
        if ( diff == 0 )
        {
          unique_index[j2] = unique_num;
        }
      }
      unique_num = unique_num + 1;
    }
  }
  return unique_index;
}
//****************************************************************************80

void i4i4_sort_a ( int i1, int i2, int *j1, int *j2 )

//****************************************************************************80
//
//  Purpose:
//
//    I4I4_SORT_A ascending sorts a pair of I4's.
//
//  Discussion:
//
//    The program allows the reasonable call:
//
//      i4i4_sort_a ( i1, i2, &i1, &i2 );
//
//    and this will return the reasonable result.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, the values to sort.
//
//    Output, int J1, J2, the sorted values.
//
{
  int k1;
  int k2;
//
//  Copy arguments, so that the user can make "reasonable" calls like:
//
//    i4i4_sort_a ( i1, i2, &i1, &i2 );
//
  k1 = i1;
  k2 = i2;

  *j1 = i4_min ( k1, k2 );
  *j2 = i4_max ( k1, k2 );

  return;
}
//****************************************************************************80

void i4i4i4_sort_a ( int i1, int i2, int i3, int *j1, int *j2, int *j3 )

//****************************************************************************80
//
//  Purpose:
//
//    I4I4I4_SORT_A ascending sorts a triple of I4's.
//
//  Discussion:
//
//    The program allows the reasonable call:
//
//      i4i4i4_sort_a ( i1, i2, i3, &i1, &i2, &i3 );
//
//    and this will return the reasonable result.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, I3, the values to sort.
//
//    Output, int *J1, *J2, *J3, the sorted values.
//
{
  int k1;
  int k2;
  int k3;
//
//  Copy arguments, so that the user can make "reasonable" calls like:
//
//    i4i4i4_sort_a ( i1, i2, i3, &i1, &i2, &i3 );
//
  k1 = i1;
  k2 = i2;
  k3 = i3;

  *j1 = i4_min ( i4_min ( k1, k2 ), i4_min ( k2, k3 ) );
  *j2 = i4_min ( i4_max ( k1, k2 ), 
        i4_min ( i4_max ( k2, k3 ), i4_max ( k3, k1 ) ) );
  *j3 = i4_max ( i4_max ( k1, k2 ), i4_max ( k2, k3 ) );

  return;
}
//****************************************************************************80

double i4int_to_r8int ( int imin, int imax, int i, double rmin, double rmax )

//****************************************************************************80
//
//  Purpose:
//
//    I4INT_TO_R8INT maps an I4 interval to an R8 interval.
//
//  Discussion:
//
//    The formula is
//
//      R := RMIN + ( RMAX - RMIN ) * ( I - IMIN ) / ( IMAX - IMIN )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int IMIN, IMAX, the range.
//
//    Input, int I, the integer to be converted.
//
//    Input, double RMIN, RMAX, the range.
//
//    Output, double R, the corresponding value in [RMIN,RMAX].
//
{
  double r;

  if ( imax == imin )
  {
    r = 0.5 * ( rmin + rmax );
  }
  else
  {
    r = ( ( double ) ( imax - i        ) * rmin   
        + ( double ) (        i - imin ) * rmax ) 
        / ( double ) ( imax     - imin );
  }

  return r;
}
//****************************************************************************80

void i4mat_copy ( int m, int n, int a1[], int a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_COPY copies one I4MAT to another.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A1[M*N], the matrix to be copied.
//
//    Output, int A2[M*N], the copy of A1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return;
}
//****************************************************************************80

int *i4mat_copy_new ( int m, int n, int a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_COPY_NEW copies an I4MAT to a "new" I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A1[M*N], the matrix to be copied.
//
//    Output, int I4MAT_COPY_NEW[M*N], the copy of A1.
//
{
  int *a2;
  int i;
  int j;

  a2 = new int[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return a2;
}
//****************************************************************************80

void i4mat_elim ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_ELIM carries out exact Gauss elimination on an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input/output, int A[M*N].  On input, the M by N matrix to
//    be Gauss eliminated.  On output, the Gauss-eliminated matrix.
//
{
  int amax;
  int i;
  int *icol;
  int ifact;
  int imax;
  int imult;
  int *irow;
  int iswap;
  int j;
  int jcol;
  int jmult;

  icol = new int[n];
  irow = new int[m];
//
//  Initialize the swap parity counter.
//
  iswap = 1;
//
//  For each column JCOL...
//
  for ( jcol = 0; jcol < i4_min ( m, n ); jcol++ )
  {
//
//  Find the maximum element in rows JCOL through M.
//
    amax = abs ( a[jcol+jcol*m] );
    imax = jcol;

    for ( i = jcol+1; i < m; i++ )
    {
      if ( amax < abs ( a[i+jcol*m] ) )
      {
        amax = abs ( a[i+jcol*m] );
        imax = i;
      }
    }
//
//  If the maximum entry is nonzero, then...
//
    if ( amax != 0 )
    {
//
//  If the maximum entry does not occur in row JCOL, then swap rows.
//
      if ( imax != jcol )
      {
        iswap = -iswap;
        i4vec_swap ( n, a+jcol*m, a+imax*m );
      }
//
//  Eliminate all nonzero entries in column JCOL, below the diagonal entry.
//
      for ( i = jcol+1; i < m; i++ )
      {
        if ( a[i+jcol*m] != 0 )
        {
          jmult = a[i+jcol*m];
          imult = a[jcol+jcol*m];
          ifact = i4_gcd ( imult, jmult );
          imult = imult / ifact;
          jmult = jmult / ifact;

          for ( j = jcol; j < n; j++ )
          {
            a[i+j*m] = jmult * a[jcol+j*m] - imult * a[i+j*m];
          }
        }
      }
//
//  Remove any row or column factors.
//
      i4mat_red ( m, n, a, irow, icol );
    }
  }

  delete [] icol;
  delete [] irow;

  return;
}
//****************************************************************************80

void i4mat_flip_cols ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_FLIP_COLS swaps the columns of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    To "flip" the columns of an I4MAT is to start with something like
//
//      11 12 13 14 15
//      21 22 23 24 25
//      31 32 33 34 35
//      41 42 43 44 45
//      51 52 53 54 55
//
//    and return
//
//      15 14 13 12 11
//      25 24 23 22 21
//      35 34 33 32 31
//      45 44 43 42 41
//      55 54 53 52 51
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 June 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[M*N], the matrix whose columns are to be flipped.
//
{
  int b;
  int i;
  int j;
  
  for ( i = 0; i < m; i++ )
  {
    for ( j = 0; j < ( n / 2 ); j++ )
    {
      b              = a[i+     j *m];
      a[i+     j *m] = a[i+(n-1-j)*m];
      a[i+(n-1-j)*m] = b;
    }
  }
  return;
}
//****************************************************************************80

void i4mat_flip_rows ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_FLIP_ROWS swaps the rows of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    To "flip" the rows of an I4MAT is to start with something like
//
//      11 12 13 14 15
//      21 22 23 24 25
//      31 32 33 34 35
//      41 42 43 44 45
//      51 52 53 54 55
//
//    and return
//
//      51 52 53 54 55
//      41 42 43 44 45
//      31 32 33 34 35
//      21 22 23 24 25
//      11 12 13 14 15
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 June 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[M*N], the matrix whose rows are to be flipped.
//
{
  int b;
  int i;
  int j;
  
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < ( m / 2 ); i++ )
    {
      b            = a[    i+j*m];
      a[    i+j*m] = a[m-1-i+j*m];
      a[m-1-i+j*m] = b;
    }
  }
  return;
}
//****************************************************************************80

int *i4mat_indicator ( int m, int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_INDICATOR sets up an "indicator" I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    The value of each entry suggests its location, as in:
//
//      11  12  13  14
//      21  22  23  24
//      31  32  33  34
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 May 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Output, int I4MAT_INDICATOR[M*N], the table.
//
{
  int *a;
  int fac;
  int i;
  int j;

  a = new int[m*n];
  fac = i4_power ( 10, i4_log_10 ( n ) + 1 );

  for ( i = 1; i <= m; i++ )
  {
    for ( j = 1; j <= n; j++ )
    {
      a[i-1+(j-1)*m] = fac * i + j;
    }
  }
  return a;
}
//****************************************************************************80

int *i4mat_l1_inverse ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_L1_INVERSE inverts a unit lower triangular I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    A unit lower triangular matrix is a matrix with only 1's on the main
//    diagonal, and only 0's above the main diagonal.
//
//    The inverse of an integer unit lower triangular matrix is also
//    an integer unit lower triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, int A[N*N], the unit lower triangular matrix.
//
//    Output, int I4MAT_L1_INVERSE[N*N], the inverse matrix.
//
{
  int *b;
  int i;
  int j;
  int k;

  b = new int[n*n];

  for ( i = 1; i <= n; i++ )
  {
    for ( j = 1; j < i; j++ )
    {
      b[i-1+(j-1)*n] = 0;
      for ( k = 1; k < i; k++ )
      {
        b[i-1+(j-1)*n] = b[i-1+(j-1)*n] + a[(i-1)+(k-1)*n] * b[(k-1)+(j-1)*n];
      }
    }
    b[i-1+(i-1)*n] = 1;
    for ( j = i+1; j <= n; j++ )
    {
      b[i-1+(j-1)*n] = 0;
    }
  }

  return b;
}
//****************************************************************************80

void i4mat_max_index ( int m, int n, int a[], int *i_max, int *j_max )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_MAX_INDEX returns the location of the maximum of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, int A[M*N], the M by N matrix.
//
//    Output, int *I_MAX, *J_MAX, the indices of the maximum entry of A.
//
{
  int i;
  int j;

  *i_max = -1;
  *j_max = -1;

  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= m; i++ )
    {
      if ( i == 1 && j == 1 )
      {
        *i_max = i;
        *j_max = j;
      }
      else if ( a[*i_max-1+(*j_max-1)*m] < a[i-1+(j-1)*m] )
      {
        *i_max = i;
        *j_max = j;
      }
    }
  }
  return;
}
//****************************************************************************80

void i4mat_min_index ( int m, int n, int a[], int *i_min, int *j_min )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_MIN_INDEX returns the location of the minimum of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, int A[M*N], the M by N matrix.
//
//    Output, int *I_MIN, *J_MIN, the indices of the minimum entry of A.
//
{
  int i;
  int j;

  *i_min = -1;
  *j_min = -1;

  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= m; i++ )
    {
      if ( i == 1 && j == 1 )
      {
        *i_min = i;
        *j_min = j;
      }
      else if ( a[i-1+(j-1)*m] < a[*i_min-1+(*j_min-1)*m] )
      {
        *i_min = i;
        *j_min = j;
      }
    }
  }
  return;
}
//****************************************************************************80

int *i4mat_mm ( int n1, int n2, int n3, int a[], int b[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_MM multiplies two I4MAT's.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, N3, the order of the matrices.
//
//    Input, int A[N1*N2], double B[N2*N#], the matrices to multiply.
//
//    Output, int I4MAT_MM[N1*N3], the product matrix C = A * B.
//
{
  int *c;
  int i;
  int j;
  int k;

  c = new int[n1*n3];

  for ( i = 0; i < n1; i ++ )
  {
    for ( j = 0; j < n3; j++ )
    {
      c[i+j*n1] = 0;
      for ( k = 0; k < n2; k++ )
      {
        c[i+j*n1] = c[i+j*n1] + a[i+k*n1] * b[k+j*n2];
      }
    }
  }
  return c;
}
//****************************************************************************80

void i4mat_perm_uniform ( int n, int a[], int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_PERM_UNIFORM selects a random permutation of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    The matrix is assumed to be square.  A single permutation is
//    applied to both rows and columns.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of rows and columns in the array.
//
//    Input/output, int A[N*N], the N by N array to be permuted.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
{
  int i;
  int j;
  int k1;
  int k2;
  int temp;
//
//  Permute the rows and columns together.
//
  for ( k1 = 0; k1 < n; k1++ )
  {
    k2 = i4_uniform ( k1, n-1, seed );

    for ( j = 0; j < n; j++ )
    {
      temp      = a[k1+j*n];
      a[k1+j*n] = a[k2+j*n];
      a[k2+j*n] = temp;
    }
    for ( i = 0; i < n; i++ )
    {
      temp      = a[i+k1*n];
      a[i+k1*n] = a[i+k2*n];
      a[i+k2*n] = temp;
    }
  }

  return;
}
//****************************************************************************80

void i4mat_perm2_uniform ( int m, int n, int a[], int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_PERM2_UNIFORM selects a random permutation of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    The matrix may be rectangular.  Separate permutations are
//    applied to the rows and columns.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[M*N], the M by N array to be permuted.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
{
  int i;
  int j;
  int k1;
  int k2;
  int temp;
//
//  Permute the rows.
//
  for ( k1 = 0; k1 < m; k1++ )
  {
    k2 = i4_uniform ( k1, m-1, seed );

    for ( j = 0; j < n; j++ )
    {
      temp      = a[k1+j*m];
      a[k1+j*m] = a[k2+j*m];
      a[k2+j*m] = temp;
    }
  }
//
//  Permute the columns.
//
  for ( k1 = 0; k1 < n; k1++ )
  {
    k2 = i4_uniform ( k1, n-1, seed );

    for ( i = 0; i < m; i++ )
    {
      temp      = a[i+k1*m];
      a[i+k1*m] = a[i+k2*m];
      a[i+k2*m] = temp;
    }
  }
  return;
}
//****************************************************************************80

void i4mat_print ( int m, int n, int a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_PRINT prints an I4MAT, with an optional title.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, int A[M*N], the M by N matrix.
//
//    Input, char *TITLE, a title to be printed.
//
{
  int i;
  int j;
  int jhi;
  int jlo;

  i4mat_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//****************************************************************************80

void i4mat_print_some ( int m, int n, int a[], int ilo, int jlo, int ihi, 
  int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_PRINT_SOME prints some of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Input, int A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, designate the first row and
//    column, and the last row and column to be printed.
//
//    Input, char *TITLE, a title for the matrix.
//
{
# define INCX 10

  int i;
  int i2hi;
  int i2lo;
  int j;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }
//
//  Print the columns of the matrix, in strips of INCX.
//
  for ( j2lo = jlo; j2lo <= jhi; j2lo = j2lo + INCX )
  {
    j2hi = j2lo + INCX - 1;
    j2hi = i4_min ( j2hi, n );
    j2hi = i4_min ( j2hi, jhi );

    cout << "\n";
//
//  For each column J in the current range...
//
//  Write the header.
//
    cout << "  Col:";
    for ( j = j2lo; j <= j2hi; j++ )
    {
      cout << "  " << setw(6) << j;
    }
    cout << "\n";
    cout << "  Row\n";
    cout << "\n";
//
//  Determine the range of the rows in this strip.
//
    i2lo = i4_max ( ilo, 1 );
    i2hi = i4_min ( ihi, m );

    for ( i = i2lo; i <= i2hi; i++ )
    {
//
//  Print out (up to INCX) entries in row I, that lie in the current strip.
//
      cout << setw(5) << i;
      for ( j = j2lo; j <= j2hi; j++ )
      {
        cout << "  " << setw(6) << a[i-1+(j-1)*m];
      }
      cout << "\n";
    }
  }
  cout << "\n";

  return;
# undef INCX
}
//****************************************************************************80

void i4mat_red ( int m, int n, int a[], int row[], int col[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_RED divides out common factors in a row or column of an I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in the matrix.
//
//    Input, int N, the number of columns in the matrix.
//
//    Input/output, int A[M*N], on input, the M by N matrix to be reduced.
//    On output, A has been reduced.  The greatest common factor in any
//    row or column is 1.
//
//    Output, int ROW[M], the row factors that were divided out.
//
//    Output, int COL[N], the column factors that were divided out.
//
{
  int factor;
  int i;
  int j;
  int *temp;

  if ( m <= 0 )
  {
    cout << "\n";
    cout << "I4MAT_RED - Warning!\n";
    cout << "  M must be greater than 0.\n";
    cout << "  Input M = " << m << "\n";
    exit ( 1 );
  }

  if ( n <= 0 )
  {
    cout << "\n";
    cout << "I4MAT_RED - Warning!\n";
    cout << "  N must be greater than 0.\n";
    cout << "  Input N = " << n << "\n";
    exit ( 1 );
  }
//
//  Remove factors common to a column.
//
  for ( j = 0; j < n; j++ )
  {
    col[j] = i4vec_red ( m, a+j*m );
  }
//
//  Remove factors common to a row.
//
  temp = new int[n];

  for ( i = 0; i < m; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      temp[j] = a[i+j*m];
    }
    row[i] = i4vec_red ( n, temp );
    for ( j = 0; j < n; j++ )
    {
      a[i+j*m] = temp[j];
    }
  }
  delete [] temp;

  return;
}
//****************************************************************************80

void i4mat_transpose_print ( int m, int n, int a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_TRANSPOSE_PRINT prints an I4MAT, transposed.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 January 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, int A[M*N], the M by N matrix.
//
//    Input, char *TITLE, a title to be printed.
//
{
  int i;
  int j;
  int jhi;
  int jlo;

  i4mat_transpose_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//****************************************************************************80

void i4mat_transpose_print_some ( int m, int n, int a[], int ilo, int jlo, 
  int ihi, int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_TRANSPOSE_PRINT_SOME prints some of an I4MAT, transposed.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 June 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Input, int A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, designate the first row and
//    column, and the last row and column to be printed.
//
//    Input, char *TITLE, a title for the matrix.
//
{
# define INCX 10

  int i;
  int i2hi;
  int i2lo;
  int j;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }
//
//  Print the columns of the matrix, in strips of INCX.
//
  for ( i2lo = ilo; i2lo <= ihi; i2lo = i2lo + INCX )
  {
    i2hi = i2lo + INCX - 1;
    i2hi = i4_min ( i2hi, m );
    i2hi = i4_min ( i2hi, ihi );

    cout << "\n";
//
//  For each row I in the current range...
//
//  Write the header.
//
    cout << "  Row: ";
    for ( i = i2lo; i <= i2hi; i++ )
    {
      cout << setw(6) << i << "  ";
    }
    cout << "\n";
    cout << "  Col\n";
    cout << "\n";
//
//  Determine the range of the rows in this strip.
//
    j2lo = i4_max ( jlo, 1 );
    j2hi = i4_min ( jhi, n );

    for ( j = j2lo; j <= j2hi; j++ )
    {
//
//  Print out (up to INCX) entries in column J, that lie in the current strip.
//
      cout << setw(5) << j << "  ";
      for ( i = i2lo; i <= i2hi; i++ )
      {
        cout << setw(6) << a[i-1+(j-1)*m] << "  ";
      }
      cout << "\n";
    }

  }

  cout << "\n";

  return;
# undef INCX
}
//****************************************************************************80

int *i4mat_u1_inverse ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_U1_INVERSE inverts a unit upper triangular I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    A unit upper triangular matrix is a matrix with only 1's on the main
//    diagonal, and only 0's below the main diagonal.
//
//    The inverse of an integer unit upper triangular matrix is also
//    an integer unit upper triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, int A[N*N], the unit upper triangular matrix.
//
//    Output, int I4MAT_U1_INVERSE[N*N], the inverse matrix.
//
{
  int *b;
  int i;
  int j;
  int k;

  b = new int[n*n];

  for ( j = n-1; 0 <= j; j-- )
  {
    for ( i = j+1; i < n; i++ )
    {
      b[i+j*n] = 0;
    }
    b[j+j*n] = 1;

    for ( i = j-1; 0 <= i; i-- )
    {
      b[i+j*n] = 0;
      for ( k = i+1; k <= j; k++ )
      {
        b[i+j*n] = b[i+j*n] - a[i+k*n] * b[k+j*n];
      }
    }
  }

  return b;
}
//****************************************************************************80

int *i4mat_uniform ( int m, int n, int a, int b, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4MAT_UNIFORM returns a scaled pseudorandom I4MAT.
//
//  Discussion:
//
//    An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      unif = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley Interscience, page 95, 1998.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Peter Lewis, Allen Goodman, James Miller
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A, B, the limits of the pseudorandom values.
//
//    Input/output, int *SEED, the "seed" value.  Normally, this
//    value should not be 0.  On output, SEED has 
//    been updated.
//
//    Output, int I4MAT_UNIFORM[M*N], a matrix of pseudorandom values.
//
{
  int i;
  int j;
  int k;
  float r;
  int value;
  int *x;

  if ( *seed == 0 )
  {
    cerr << "\n";
    cerr << "I4MAT_UNIFORM - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  x = new int[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      k = *seed / 127773;

      *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

      if ( *seed < 0 )
      {
        *seed = *seed + 2147483647;
      }

      r = ( float ) ( *seed ) * 4.656612875E-10;
//
//  Scale R to lie between A-0.5 and B+0.5.
//
      r = ( 1.0 - r ) * ( ( float ) ( i4_min ( a, b ) ) - 0.5 ) 
        +         r   * ( ( float ) ( i4_max ( a, b ) ) + 0.5 );
//
//  Use rounding to convert R to an integer between A and B.
//
      value = r4_nint ( r );

      value = i4_max ( value, i4_min ( a, b ) );
      value = i4_min ( value, i4_max ( a, b ) );

      x[i+j*m] = value;
    }
  }

  return x;
}
//****************************************************************************80

int i4row_compare ( int m, int n, int a[], int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_COMPARE compares two rows of a integer array.
//
//  Discussion:
//
//    The two dimensional information is stored in a one dimensional array,
//    by columns.  The entry A(I,J) is stored in A[I+J*M].
//
//    The input arguments I and J are row indices.  They DO NOT use the
//    C convention of starting at 0, but rather start at 1.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, I = 2, J = 3
//
//      A = (
//        1  2  3  4
//        5  6  7  8
//        9 10 11 12 )
//
//    Output:
//
//      I4ROW_COMPARE = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 October 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int  A[M*N], the array of data.
//
//    Input, int I, J, the rows to be compared.
//    I and J must be between 1 and M.
//
//    Output, int I4ROW_COMPARE, the results of the comparison:
//    -1, row I < row J,
//     0, row I = row J,
//    +1, row J < row I.
//
{
  int k;
//
//  Check that I and J are legal.
//
  if ( i < 1 )
  {
    cout << "\n";
    cout << "I4ROW_COMPARE - Fatal error!\n";
    cout << "  Row index I is less than 1.\n";
    cout << "  I = " << i << "\n";
    exit ( 1 );
  }
  else if ( m < i )
  {
    cout << "\n";
    cout << "I4ROW_COMPARE - Fatal error!\n";
    cout << "  Row index I is out of bounds.\n";
    cout << "  I = " << i << "\n";
    cout << "  Maximum legal value is M = " << m << "\n";
    exit ( 1 );
  }

  if ( j < 1 )
  {
    cout << "\n";
    cout << "I4ROW_COMPARE - Fatal error!\n";
    cout << "  Row index J is less than 1.\n";
    cout << "  J = " << j << "\n";
    exit ( 1 );
  }
  else if ( m < j )
  {
    cout << "\n";
    cout << "I4ROW_COMPARE - Fatal error!\n";
    cout << "  Row index J is out of bounds.\n";
    cout << "  J = " << j << "\n";
    cout << "  Maximum legal value is M = " << m << "\n";
    exit ( 1 );
  }

  if ( i == j )
  {
    return 0;
  }

  for ( k = 0; k < n; k++ )
  {
    if ( a[(i-1)+k*m] < a[(j-1)+k*m] )
    {
      return -1;
    }
    else if ( a[(j-1)+k*m] < a[(i-1)+k*m] )
    {
      return +1;
    }
  }

  return 0;
}
//****************************************************************************80

void i4row_find_item ( int m, int n, int a[], int item, int *row, int *col )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_FIND_ITEM searches the rows of an I4ROW for a given value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A[M*N], the table to search.
//
//    Input, int ITEM, the value to search for.
//
//    Output, int ROW, COL, the row and column indices
//    of the first occurrence of the value ITEM.  The search
//    is conducted by rows.  If the item is not found, then
//    ROW = COL = -1.
//
{
  int i;
  int j;

  *row = -1;
  *col = -1;

  for ( i = 0; i < m; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      if ( a[i+j*m] == item )
      {
        *row = i + 1;
        *col = j + 1;
        return;
      }
    }
  }
  return;
}
//****************************************************************************80

int *i4row_max ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_MAX returns the maximums of an I4ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MAX =
//      3
//      7
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, int A[M*N], the array to be examined.
//
//    Output, int IMAX[M]; IMAX[I] is the column of A in which
//    the maximum for row I occurs.
//
//    Output, int I4ROW_MAX[M], the maximums of the rows.
//
{
  int i;
  int j;
  int *amax;

  amax = new int[m];

  for ( i = 0; i < m; i++ )
  {
    amax[i] = a[i+0*m];

    for ( j = 1; j < n; j++ )
    {
      if ( amax[i] < a[i+j*m] )
      {
        amax[i] = a[i+j*m];
      }
    }
  }
  return amax;
}
//****************************************************************************80

double *i4row_mean ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_MEAN returns the means of an I4ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MEAN =
//      2
//      5
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, int A[M*N], the array to be examined.
//
//    Output, double I4ROW_MEAN[M], the means, or averages, of the rows.
//
{
  int i;
  int j;
  double *mean;

  mean = new double[m];

  for ( i = 0; i < m; i++ )
  {
    mean[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      mean[i] = mean[i] + ( double ) a[i+j*m];
    }
    mean[i] = mean[i] / ( double ) ( n );
  }

  return mean;
}
//****************************************************************************80

int *i4row_min ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_MIN returns the minimums of an I4ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MIN =
//      1
//      2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, int A[M*N], the array to be examined.
//
//    Output, int I4ROW_MIN[M], the minimums of the rows.
//
{
  int i;
  int j;
  int *amin;

  amin = new int[m];

  for ( i = 0; i < m; i++ )
  {
    amin[i] = a[i+0*m];
    for ( j = 1; j < n; j++ )
    {
      if ( a[i+j*m] < amin[i] )
      {
        amin[i] = a[i+j*m];
      }
    }
  }

  return amin;
}
//****************************************************************************80

void i4row_sort_a ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_SORT_A ascending sorts the rows of an I4ROW.
//
//  Discussion:
//
//    In lexicographic order, the statement "X < Y", applied to two
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, X is less than Y if, at the first index where they
//    differ, the X value is less than the Y value.
//
//  Example:
//
//    Input:
//
//      M = 5, N = 3
//
//      A =
//        3  2  1
//        2  4  3
//        3  1  8
//        2  4  2
//        1  9  9
//
//    Output:
//
//      A =
//        1  9  9
//        2  4  2
//        2  4  3
//        3  1  8
//        3  2  1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A.
//
//    Input/output, int A[M*N].
//    On input, the array of M rows of N-vectors.
//    On output, the rows of A have been sorted in ascending
//    lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( m, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      i4row_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = i4row_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }

  }

  return;
}
//****************************************************************************80

void i4row_sort_d ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_SORT_D descending sorts the rows of an I4ROW.
//
//  Discussion:
//
//    In lexicographic order, the statement "X < Y", applied to two real
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, the first time they differ, X is smaller.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows and columns of A.
//
//    Input/output, int A[M*N].
//    On input, the array of M rows of N-vectors.
//    On output, the rows of A have been sorted in descending
//    lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( m, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      i4row_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = -i4row_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }
  return;
}
//****************************************************************************80

void i4row_sort2_d ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_SORT2_D descending sorts the elements of each row of an I4ROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of A.
//
//    Input, int N, the number of columns of A, and the length
//    of a vector of data.
//
//    Input/output, int A[M*N].
//    On input, the array of M rows of N-vectors.
//    On output, the elements of each row of A have been sorted in descending
//    order.
//
{
  int i;
  int indx;
  int isgn;
  int j;
  int row;
  int temp;

  if ( m <= 1 )
  {
    return;
  }

  if ( n <= 0 )
  {
    return;
  }
//
//  Initialize.
//
  for ( row = 0; row < m; row++ )
  {
    i = 0;
    indx = 0;
    isgn = 0;
    j = 0;
//
//  Call the external heap sorter.
//
    for ( ; ; )
    {
      sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
      if ( 0 < indx )
      {
        temp           = a[row+(i-1)*m];
        a[row+(i-1)*m] = a[row+(j-1)*m];
        a[row+(j-1)*m] = temp;
      }
//
//  Compare the I and J objects.
//
      else if ( indx < 0 )
      {
        if ( a[row+(i-1)*m] < a[row+(j-1)*m] )
        {
          isgn = +1;
        }
        else
        {
          isgn = -1;
        }
      }
      else if ( indx == 0 )
      {
        break;
      }
    }
  }

  return;
}
//****************************************************************************80

int *i4row_sum ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_SUM returns the sums of the rows of an I4ROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, int A[M*N], the M by N array.
//
//    Output, int I4ROW_SUM[M], the sum of the entries of 
//    each row.
//
{
  int i;
  int j;
  int *rowsum;

  rowsum = new int[m];

  for ( i = 0; i < m; i++ )
  {
    rowsum[i] = 0;
    for ( j = 0; j < n; j++ )
    {
      rowsum[i] = rowsum[i] + a[i+j*m];
    }
  }
  return rowsum;
}
//****************************************************************************80

void i4row_swap ( int m, int n, int a[], int irow1, int irow2 )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_SWAP swaps two rows of an I4ROW.
//
//  Discussion:
//
//    The two dimensional information is stored as a one dimensional
//    array, by columns.
//
//    The row indices are 1 based, NOT 0 based//  However, a preprocessor
//    variable, called OFFSET, can be reset from 1 to 0 if you wish to
//    use 0-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int A[M*N], an array of data.
//
//    Input, int IROW1, IROW2, the two rows to swap.
//    These indices should be between 1 and M.
//
{
# define OFFSET 1

  int j;
  int t;
//
//  Check.
//
  if ( irow1 < 0+OFFSET || m-1+OFFSET < irow1 )
  {
    cout << "\n";
    cout << "I4ROW_SWAP - Fatal error!\n";
    cout << "  IROW1 is out of range.\n";
    exit ( 1 );
  }

  if ( irow2 < 0+OFFSET || m-1+OFFSET < irow2 )
  {
    cout << "\n";
    cout << "I4ROW_SWAP - Fatal error!\n";
    cout << "  IROW2 is out of range.\n";
    exit ( 1 );
  }

  if ( irow1 == irow2 )
  {
    return;
  }
  for ( j = 0; j < n; j++ )
  {
    t                   = a[irow1-OFFSET+j*m];
    a[irow1-OFFSET+j*m] = a[irow2-OFFSET+j*m];
    a[irow2-OFFSET+j*m] = t;
  }

  return;
# undef OFFSET
}
//****************************************************************************80

double *i4row_variance ( int m, int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4ROW_VARIANCE returns the variances of an I4ROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, int A[M*N], the array whose variances are desired.
//
//    Output, double I4ROW_VARIANCE[M], the variances of the rows.
//
{
  int i;
  int j;
  double mean;
  double *variance;

  variance = new double[m];

  for ( i = 0; i < m; i++ )
  {
    mean = 0.0;
    for ( j = 0; j < n; j++ )
    {
      mean = mean + ( double ) a[i+j*m];
    }
    mean = mean / ( double ) ( n );

    variance[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      variance[i] = variance[i] + pow ( ( ( double ) a[i+j*m] - mean ), 2 );
    }

    if ( 1 < n )
    {
      variance[i] = variance[i] / ( double ) ( n - 1 );
    }
    else
    {
      variance[i] = 0.0;
    }
  }

  return variance;
}
//****************************************************************************80

int i4vec_amax ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMAX returns the largest magnitude in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be searched.
//
//    Output, int I4VEC_AMAX, the value of the entry of largest magnitude.
//
{
  int amax;
  int i;

  amax = a[0];

  for ( i = 1; i < n; i++ )
  {
    if ( abs ( amax ) < abs ( a[i] ) )
    {
      amax = a[i];
    }
  }
  return amax;
}
//****************************************************************************80

int i4vec_amax_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMAX_INDEX returns the index of the maximum absolute value in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_AMAX_INDEX, the index of the entry of largest magnitude.
//
{
  int amax;
  int amax_index;
  int i;

  if ( n <= 0 )
  {
    amax_index = -1;
  }
  else
  {
    amax_index = 1;
    amax = abs ( a[0] );

    for ( i = 2; i <= n; i++ )
    {
      if ( amax < abs ( a[i-1] ) )
      {
        amax_index = i;
        amax = abs ( a[i-1] );
      }
    }
  }
  return amax_index;
}
//****************************************************************************80

int i4vec_amin ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMIN returns the smallest magnitude in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be checked.
//
//    Output, int I4VEC_AMIN, the value of the smallest magnitude.
//
{
  int amin;
  int i;

  amin = a[0];
 
  for ( i = 1; i < n; i++ )
  {
    if ( abs ( a[i] ) < abs ( amin ) )
    {
      amin = a[i];
    }
  }
  return amin;
}
//****************************************************************************80

int i4vec_amin_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMIN_INDEX returns the index of the minimum absolute value in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, int I4VEC_AMIN_INDEX, the index of the entry of smallest magnitude.
//
{
  int amin;
  int amin_index;
  int i;

  if ( n <= 0 )
  {
    amin_index = -1;
  }
  else
  {
    amin_index = 1;
    amin = abs ( a[0] );

    for ( i = 2; i <= n; i++ )
    {
      if ( abs ( a[i-1] ) < amin )
      {
        amin_index = i;
        amin = abs ( a[i-1] );
      }
    }
  }
  return amin_index;
}
//****************************************************************************80

int i4vec_aminz ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMINZ returns the smallest nonzero magnitude in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be checked.
//
//    Output, int I4VEC_AMINZ, the value of the smallest nonzero magnitude.
//    If all entries are zero, I4VEC_AMINZ is 0.
//
{
  int aminz;
  int i;

  aminz = 0;
 
  for ( i = 0; i < n; i++ )
  {
    if ( a[i] != 0 )
    {
      if ( aminz == 0 || abs ( a[i] ) < abs ( aminz ) )
     {
        aminz = a[i];
      }
    }
  }
 
  return aminz;
}
//****************************************************************************80

int i4vec_aminz_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AMINZ_INDEX returns the smallest nonzero magnitude in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries to be checked.
//
//    Input, int A[N], the vector to be checked.
//
//    Output, int I4VEC_AMINZ_INDEX, the entry of the smallest nonzero magnitude.
//    If all entries are zero, AMINZ_INDEX is 0.
//
{
  int aminz;
  int i;
  int aminz_index;

  aminz = 0;
  aminz_index = -1;

  for ( i = 1; i <= n; i++ )
  {
    if ( a[i-1] != 0 )
    {
      if ( aminz_index == 0 || abs ( a[i-1] ) < aminz )
      {
        aminz = abs ( a[i-1] );
        aminz_index = i;
      }
    }
  }

  return aminz_index;
}
//****************************************************************************80

int *i4vec_ascend_sub ( int n, int a[], int *length )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ASCEND_SUB computes the longest ascending subsequence of vector.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The subsequence is required to be strictly increasing.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the length of the vector.
//
//    Input, int A[N], the vector to be examined.
//
//    Output, int *LENGTH, the length of the longest increasing subsequence.
//
//    Output, int I4VEC_ASCEND_SUB[LENGTH], a longest increasing 
//    subsequence of A.
//
{
  int i;
  int j;
  int k;
  int *sub;
  int *top;
  int *top_prev;

  if ( n <= 0 )
  {
    *length = 0;
    return NULL;
  }

  top = new int[n];
  for ( i = 0; i < n; i++ )
  {
    top[i] = 0;
  }

  top_prev = new int[n];
  for ( i = 0; i < n; i++ )
  {
    top_prev[i] = 0;
  }

  *length = 0;

  for ( i = 0; i < n; i++ )
  {
    k = -1;
    for ( j = 0; j < *length; j++ )
    {
      if ( a[i] <= a[top[j]] )
      {
        k = j;
        break;
      }
    }
    if ( k == -1 )
    {
      k = *length;
      *length = *length + 1;
    }

    top[k] = i;

    if ( 0 < k )
    {
      top_prev[i] = top[k-1];
    }
    else
    {
      top_prev[i] = -1;
    }
  }
//
//  Extract the subsequence.
//
  sub = new int[*length];

  j = top[*length-1];
  sub[*length-1] = a[j];

  for ( i = *length-2; 0 <= i; i-- )
  {
    j = top_prev[j];
    sub[i] = a[j];
  }

  delete [] top;
  delete [] top_prev;

  return sub;
}
//****************************************************************************80

bool i4vec_ascends ( int n, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ASCENDS determines if an I4VEC is (weakly) ascending.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    X = ( -8, 1, 2, 3, 7, 7, 9 )
//
//    I4VEC_ASCENDS = TRUE
//
//    The sequence is not required to be strictly ascending.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the array.
//
//    Input, int X[N], the array to be examined.
//
//    Output, bool I4VEC_ASCENDS, is TRUE if the entries of X ascend.
//
{
  int i;

  for ( i = 1; i <= n-1; i++ )
  {
    if ( x[i] < x[i-1] )
    {
      return false;
    }
  }

  return true;
}
//****************************************************************************80

void i4vec_axpy ( int n, int ia, int ix[], int incx, int iy[], int incy )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_AXPY adds IA times the vector IX to the vector IY.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of IX and IY.
//
//    Input, int IA, the scalar value by which each entry
//    of IX is multiplied before being added to IY.
//
//    Input, int IX[*], the vector, a multiple of which is to be 
//    added to IY.
//
//    Input, int INCX, the increment between successive entries of IX.
//
//    Input/output, int IY[*].
//
//    On output, each entry of IY has been increased by
//    IA times the corresponding entry of IX.
//
//    Input, int INCY, the increment between successive entries of IY.
//
{
  int i;
  int indx;
  int indy;

  indx = 0;
  indy = 0;
 
  for ( i = 0; i < n; i++ )
  {
    iy[indy] = iy[indy] + ia * ix[indx];
 
    indx = indx + incx;
    indy = indy + incy;
  }
 
  return;
}
//****************************************************************************80

void i4vec_bracket ( int n, int x[], int xval, int *left, int *right )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_BRACKET searches a sorted I4VEC for successive brackets of a value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    If the values in the vector are thought of as defining intervals
//    on the number line, then this routine searches for the interval
//    containing the given value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input, int X[N], an array that has been sorted into ascending order.
//
//    Input, int XVAL, a value to be bracketed.
//
//    Output, int *LEFT, *RIGHT, the results of the search.
//    In the most common case, 0 <= LEFT < LEFT + 1 = RIGHT <= N-1,
//    and X[LEFT] <= XVAL <= X[RIGHT].
//
//    Special cases:
//      Value is less than all data values:
//        LEFT = -1, RIGHT = 0, and XVAL < X[RIGHT].
//      Value is greater than all data values:
//        LEFT = N-1, RIGHT = -1, and X[LEFT] < XVAL.
//      Value is equal to a data value:
//        LEFT = RIGHT, and X[LEFT] = X[RIGHT] = XVAL.
//      Algorithm failure:
//        LEFT = RIGHT = -1.
//
{
  int high;
  int low;
  int mid;
//
//  XVAL < X[0].
//
  if ( xval < x[0] )
  {
    *left = -1;
    *right = 0;
  }
//
//  X[N] < XVAL.
//
  else if ( x[n-1] < xval )
  {
    *left = n-1;
    *right = -1;
  }
//
//  N = 1
//
  else if ( n == 1 )
  {
    *left = 0;
    *right = 0;
  }
//
//  X[0] <= XVAL <= X[N-1].
//
  else
  {
    low = 0;
    high = n - 2;

    for ( ; ; )
    {
      mid = ( low + high ) / 2;

      if ( high < low )
      {
        cout << "\n";
        cout << "I4VEC_BRACKET - Fatal error!\n";
        cout << "  Algorithm or data failure.\n";
        *left = -1;
        *right = -1;
        return;
      }

      if ( x[mid] == xval )
      {
        *left = mid;
        *right = mid;
        return;
      }
      else if ( x[mid+1] == xval )
      {
        *left = mid + 1;
        *right = mid + 1;
        return;
      }
      else if ( x[mid] < xval && xval < x[mid+1] )
      {
        *left = mid;
        *right = mid + 1;
        return;
      }
      else if ( x[mid+1] < xval )
      {
        low = mid + 1;
      }
      else if ( xval < x[mid] )
      {
        high = mid - 1;
      }
    }
  }
  return;
}
//****************************************************************************80

int i4vec_compare ( int n, int a[], int b[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_COMPARE compares two I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The lexicographic ordering is used.
//
//  Example:
//
//    Input:
//
//      A = ( 2, 6, 2 )
//      B = ( 2, 8, 12 )
//
//    Output:
//
//      ISGN = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, int A[N], B[N], the vectors to be compared.
//
//    Output, int I4VEC_COMPARE, the results of the comparison:
//    -1, A is lexicographically less than B,
//     0, A is equal to B,
//    +1, A is lexicographically greater than B.
//
{
  int isgn;
  int k;

  isgn = 0;

  for ( k = 0; k < n; k++ )
  {
    if ( a[k] < b[k] )
    {
      isgn = - 1;
      return isgn;
    }
    else if ( b[k] < a[k] )
    {
      isgn = + 1;
      return isgn;
    }
  }

  return isgn;
}
//****************************************************************************80

void i4vec_copy ( int n, int a1[], int a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_COPY copies an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, int A1[N], the vector to be copied.
//
//    Output, int A2[N], the copy of A1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return;
}
//****************************************************************************80

int *i4vec_copy_new ( int n, int a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_COPY_NEW copies an I4VEC to a "new" I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, int A1[N], the vector to be copied.
//
//    Output, int I4VEC_COPY_NEW[N], the copy of A1.
//
{
  int *a2;
  int i;

  a2 = new int[n];

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return a2;
}
//****************************************************************************80

int *i4vec_cum ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_CUM computes the cumulutive sum of the entries of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      A = (/ 1, 2, 3, 4 /)
//
//    Output:
//
//      A_CUM = (/ 0, 1, 3, 6, 10 /)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be summed.
//
//    Output, int I4VEC_CUM[N+1], the cumulative sum of the entries of A.
//
{
  int *a_cum;
  int i;

  a_cum = new int[n+1];

  a_cum[0] = 0;

  for ( i = 1; i <= n; i++ )
  {
    a_cum[i] = a_cum[i-1] + a[i-1];
  }

  return a_cum;
}
//****************************************************************************80

bool i4vec_descends ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_DESCENDS determines if an I4VEC is (weakly) descending.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    X = ( 9, 7, 7, 3, 2, 1, -8 )
//
//    I4VEC_DESCENDS = TRUE
//
//    The sequence is not required to be strictly descending.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the array.
//
//    Input, int A[N], the array to be examined.
//
//    Output, bool I4VEC_DESCENDS, is TRUE if the entries of X descend.
//
{
  int i;

  for ( i = 1; i <= n-1; i++ )
  {
    if ( a[i-1] < a[i] )
    {
      return false;
    }
  }

  return true;
}
//****************************************************************************80

void i4vec_direct_product ( int factor_index, int factor_order, 
  int factor_value[], int factor_num, int point_num, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_DIRECT_PRODUCT creates a direct product of I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    To explain what is going on here, suppose we had to construct
//    a multidimensional quadrature rule as the product of K rules
//    for 1D quadrature.
//
//    The product rule will be represented as a list of points and weights.
//
//    The J-th item in the product rule will be associated with
//      item J1 of 1D rule 1,
//      item J2 of 1D rule 2, 
//      ..., 
//      item JK of 1D rule K.
//
//    In particular, 
//      X(J) = ( X(1,J1), X(2,J2), ..., X(K,JK))
//    and
//      W(J) = W(1,J1) * W(2,J2) * ... * W(K,JK)
//
//    So we can construct the quadrature rule if we can properly
//    distribute the information in the 1D quadrature rules.
//
//    This routine carries out that task for the points X.
//
//    Another way to do this would be to compute, one by one, the
//    set of all possible indices (J1,J2,...,JK), and then index
//    the appropriate information.  An advantage of the method shown
//    here is that you can process the K-th set of information and
//    then discard it.
//
//  Example:
//
//    Rule 1: 
//      Order = 4
//      X(1:4) = ( 1, 2, 3, 4 )
//
//    Rule 2:
//      Order = 3
//      X(1:3) = ( 10, 20, 30 )
//
//    Rule 3:
//      Order = 2
//      X(1:2) = ( 100, 200 )
//
//    Product Rule:
//      Order = 24
//      X(1:24) = 
//        ( 1, 10, 100 )
//        ( 2, 10, 100 )
//        ( 3, 10, 100 )
//        ( 4, 10, 100 )
//        ( 1, 20, 100 )
//        ( 2, 20, 100 )
//        ( 3, 20, 100 )
//        ( 4, 20, 100 )
//        ( 1, 30, 100 )
//        ( 2, 30, 100 )
//        ( 3, 30, 100 )
//        ( 4, 30, 100 )
//        ( 1, 10, 200 )
//        ( 2, 10, 200 )
//        ( 3, 10, 200 )
//        ( 4, 10, 200 )
//        ( 1, 20, 200 )
//        ( 2, 20, 200 )
//        ( 3, 20, 200 )
//        ( 4, 20, 200 )
//        ( 1, 30, 200 )
//        ( 2, 30, 200 )
//        ( 3, 30, 200 )
//        ( 4, 30, 200 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int FACTOR_INDEX, the index of the factor being processed.
//    The first factor processed must be factor 0.
//
//    Input, int FACTOR_ORDER, the order of the factor.
//
//    Input, int FACTOR_VALUE[FACTOR_ORDER], the factor values for
//    factor FACTOR_INDEX.
//
//    Input, int FACTOR_NUM, the number of factors.
//
//    Input, int POINT_NUM, the number of elements in the direct product.
//
//    Input/output, int X[FACTOR_NUM*POINT_NUM], the elements of the
//    direct product, which are built up gradually.  Before the first call,
//    X might be set to 0.  After each factor has been input, X should
//    have the correct value.
//
//  Local Parameters:
//
//    Local, integer START, the first location of a block of values to set.
//
//    Local, integer CONTIG, the number of consecutive values to set.
//
//    Local, integer SKIP, the distance from the current value of START
//    to the next location of a block of values to set.
//
//    Local, integer REP, the number of blocks of values to set.
//
{
  static int contig = 0;
  int i;
  int j;
  int k;
  static int rep = 0;
  static int skip = 0;
  int start;

  if ( factor_index == 0 )
  {
    contig = 1;
    skip = 1;
    rep = point_num;
  }

  rep = rep / factor_order;
  skip = skip * factor_order;

  for ( j = 0; j < factor_order; j++ )
  {
    start = 0 + j * contig;

    for ( k = 1; k <= rep; k++ )
    {
      for ( i = start; i < start + contig; i++ )
      {
        x[factor_index+i*factor_num] = factor_value[j];
      }
      start = start + skip;
    }
  }

  contig = contig * factor_order;

  return;
}
//****************************************************************************80

void i4vec_direct_product2 ( int factor_index, int factor_order, 
  int factor_value[], int factor_num, int point_num, int w[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_DIRECT_PRODUCT2 creates a direct product of I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    To explain what is going on here, suppose we had to construct
//    a multidimensional quadrature rule as the product of K rules
//    for 1D quadrature.
//
//    The product rule will be represented as a list of points and weights.
//
//    The J-th item in the product rule will be associated with
//      item J1 of 1D rule 1,
//      item J2 of 1D rule 2, 
//      ..., 
//      item JK of 1D rule K.
//
//    In particular, 
//      X(J) = ( X(1,J1), X(2,J2), ..., X(K,JK))
//    and
//      W(J) = W(1,J1) * W(2,J2) * ... * W(K,JK)
//
//    So we can construct the quadrature rule if we can properly
//    distribute the information in the 1D quadrature rules.
//
//    This routine carries out that task for the weights W.
//
//    Another way to do this would be to compute, one by one, the
//    set of all possible indices (J1,J2,...,JK), and then index
//    the appropriate information.  An advantage of the method shown
//    here is that you can process the K-th set of information and
//    then discard it.
//
//  Example:
//
//    Rule 1: 
//      Order = 4
//      W(1:4) = ( 2, 3, 5, 7 )
//
//    Rule 2:
//      Order = 3
//      W(1:3) = ( 11, 13, 17 )
//
//    Rule 3:
//      Order = 2
//      W(1:2) = ( 19, 23 )
//
//    Product Rule:
//      Order = 24
//      W(1:24) =
//        ( 2 * 11 * 19 )
//        ( 3 * 11 * 19 )
//        ( 4 * 11 * 19 )
//        ( 7 * 11 * 19 )
//        ( 2 * 13 * 19 )
//        ( 3 * 13 * 19 )
//        ( 5 * 13 * 19 )
//        ( 7 * 13 * 19 )
//        ( 2 * 17 * 19 )
//        ( 3 * 17 * 19 )
//        ( 5 * 17 * 19 )
//        ( 7 * 17 * 19 )
//        ( 2 * 11 * 23 )
//        ( 3 * 11 * 23 )
//        ( 5 * 11 * 23 )
//        ( 7 * 11 * 23 )
//        ( 2 * 13 * 23 )
//        ( 3 * 13 * 23 )
//        ( 5 * 13 * 23 )
//        ( 7 * 13 * 23 )
//        ( 2 * 17 * 23 )
//        ( 3 * 17 * 23 )
//        ( 5 * 17 * 23 )
//        ( 7 * 17 * 23 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int FACTOR_INDEX, the index of the factor being processed.
//    The first factor processed must be factor 0.
//
//    Input, int FACTOR_ORDER, the order of the factor.
//
//    Input, int FACTOR_VALUE[FACTOR_ORDER], the factor values for
//    factor FACTOR_INDEX.
//
//    Input, int FACTOR_NUM, the number of factors.
//
//    Input, int POINT_NUM, the number of elements in the direct product.
//
//    Input/output, int W[POINT_NUM], the elements of the
//    direct product, which are built up gradually.  Before the first call,
//    W must be set to 1.
//
//  Local Parameters:
//
//    Local, integer START, the first location of a block of values to set.
//
//    Local, integer CONTIG, the number of consecutive values to set.
//
//    Local, integer SKIP, the distance from the current value of START
//    to the next location of a block of values to set.
//
//    Local, integer REP, the number of blocks of values to set.
//
{
  static int contig = 0;
  int i;
  int j;
  int k;
  static int rep = 0;
  static int skip = 0;
  int start;

  if ( factor_index == 0 )
  {
    contig = 1;
    skip = 1;
    rep = point_num;
  }

  rep = rep / factor_order;
  skip = skip * factor_order;

  for ( j = 0; j < factor_order; j++ )
  {
    start = 0 + j * contig;

    for ( k = 1; k <= rep; k++ )
    {
      for ( i = start; i < start + contig; i++ )
      {
        w[i] = w[i] * factor_value[j];
      }
      start = start + skip;
    }
  }

  contig = contig * factor_order;

  return;
}
//****************************************************************************80

int *i4vec_first_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_FIRST_INDEX indexes the first occurrence of values in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    For element A(I) of the vector, FIRST_INDEX(I) is the index in A of
//    the first occurrence of the value A(I).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_FIRST_INDEX[N], the first occurrence index.
//
{
  int *first_index;
  int i;
  int j;

  first_index = new int[n];

  for ( i = 0; i < n; i++ )
  {
    first_index[i] = -1;
  }
  for ( i = 0; i < n; i++ )
  {
    if ( first_index[i] == -1 )
    {
      first_index[i] = i;
      for ( j = i + 1; j < n; j++ )
      {
        if ( a[i] == a[j] )
        {
          first_index[j] = i;
        }
      }
    }
  }
  return first_index;
}
//****************************************************************************80

int i4vec_frac ( int n, int a[], int k )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_FRAC searches for the K-th smallest entry in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Hoare's algorithm is used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, int A[N].
//    On input, A is the array to search.
//    On output, the elements of A have been somewhat rearranged.
//
//    Input, int K, the fractile to be sought.  If K = 1, the minimum
//    entry is sought.  If K = N, the maximum is sought.  Other values
//    of K search for the entry which is K-th in size.  K must be at
//    least 1, and no greater than N.
//
//    Output, double I4VEC_FRAC, the value of the K-th fractile of A.
//
{
  int frac;
  int i;
  int iryt;
  int j;
  int left;
  int temp;
  int w;
  int x;

  if ( n <= 0 )
  {
    cout << "\n";
    cout << "I4VEC_FRAC - Fatal error!\n";
    cout << "  Illegal nonpositive value of N = " << n << "\n";
    exit ( 1 );
  }

  if ( k <= 0 )
  {
    cout << "\n";
    cout << "I4VEC_FRAC - Fatal error!\n";
    cout << "  Illegal nonpositive value of K = " << k << "\n";
    exit ( 1 );
  }

  if ( n < k )
  {
    cout << "\n";
    cout << "I4VEC_FRAC - Fatal error!\n";
    cout << "  Illegal N < K, K = " << k << "\n";
    exit ( 1 );
  }

  left = 1;
  iryt = n;

  for ( ; ; )
  {
    if ( iryt <= left )
    {
      frac = a[k-1];
      break;
    }

    x = a[k-1];
    i = left;
    j = iryt;

    for ( ; ; )
    {
      if ( j < i )
      {
        if ( j < k )
        {
          left = i;
        }
        if ( k < i )
        {
          iryt = j;
        }
        break;
      }
//
//  Find I so that X <= A(I).
//
      while ( a[i-1] < x )
      {
        i = i + 1;
      }
//
//  Find J so that A(J) <= X.
//
      while ( x < a[j-1] )
      {
        j = j - 1;
      }

      if ( i <= j )
      {
        temp   = a[i-1];
        a[i-1] = a[j-1];
        a[j-1] = temp;
        i = i + 1;
        j = j - 1;
      }
    }
  }

  return frac;
}
//****************************************************************************80

int i4vec_gcd ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_GCD finds the greatest common divisor of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the vector.
//
//    Input, int A[N], the vector to be checked.
//
//    Output, int I4VEC_GCD, the greatest common divisor of all entries of A.
//
{
  int gcd;
  int i;

  gcd = i4vec_amax ( n, a );

  for ( i = 0; i < n; i++ )
  {
    gcd = i4_gcd ( gcd, a[i] );
  }

  return gcd;
}
//****************************************************************************80

void i4vec_heap_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HEAP_A reorders an I4VEC into a ascending heap.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    An ascending heap is an array A with the property that, for every index J,
//    A[J] <= A[2*J+1] and A[J] <= A[2*J+2], (as long as the indices
//    2*J+1 and 2*J+2 are legal).
//
//  Diagram:
//
//                  A(0)
//                /      \
//            A(1)         A(2)
//          /     \        /  \
//      A(3)       A(4)  A(5) A(6)
//      /  \       /   \
//    A(7) A(8)  A(9) A(10)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the size of the input array.
//
//    Input/output, int A[N].
//    On input, an unsorted array.
//    On output, the array has been reordered into a heap.
//
{
  int i;
  int ifree;
  int key;
  int m;
//
//  Only nodes (N/2)-1 down to 0 can be "parent" nodes.
//
  for ( i = (n/2)-1; 0 <= i; i-- )
  { 
//
//  Copy the value out of the parent node.
//  Position IFREE is now "open".
//
    key = a[i];
    ifree = i;

    for ( ; ; )
    {
//
//  Positions 2*IFREE + 1 and 2*IFREE + 2 are the descendants of position
//  IFREE.  (One or both may not exist because they equal or exceed N.)
//
      m = 2 * ifree + 1;
//
//  Does the first position exist?
//
      if ( n <= m )
      {
        break;
      }
      else
      {
//
//  Does the second position exist?
//
        if ( m + 1 < n )
        {
//
//  If both positions exist, take the larger of the two values,
//  and update M if necessary.
//
          if ( a[m+1] < a[m] )
          {
            m = m + 1;
          }
        }
//
//  If the large descendant is larger than KEY, move it up,
//  and update IFREE, the location of the free position, and
//  consider the descendants of THIS position.
//
        if ( a[m] <= key )
        {
          break;
        }

        a[ifree] = a[m];
        ifree = m;
      }
    }
//
//  When you have stopped shifting items up, return the item you
//  pulled out back to the heap.
//
    a[ifree] = key;
  }

  return;
}
//****************************************************************************80

void i4vec_heap_d ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HEAP_D reorders an I4VEC into a descending heap.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    A heap is an array A with the property that, for every index J,
//    A[J] >= A[2*J+1] and A[J] >= A[2*J+2], (as long as the indices
//    2*J+1 and 2*J+2 are legal).
//
//  Diagram:
//
//                  A(0)
//                /      \
//            A(1)         A(2)
//          /     \        /  \
//      A(3)       A(4)  A(5) A(6)
//      /  \       /   \
//    A(7) A(8)  A(9) A(10)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the size of the input array.
//
//    Input/output, int A[N].
//    On input, an unsorted array.
//    On output, the array has been reordered into a heap.
//
{
  int i;
  int ifree;
  int key;
  int m;
//
//  Only nodes (N/2)-1 down to 0 can be "parent" nodes.
//
  for ( i = (n/2)-1; 0 <= i; i-- )
  { 
//
//  Copy the value out of the parent node.
//  Position IFREE is now "open".
//
    key = a[i];
    ifree = i;

    for ( ; ; )
    {
//
//  Positions 2*IFREE + 1 and 2*IFREE + 2 are the descendants of position
//  IFREE.  (One or both may not exist because they equal or exceed N.)
//
      m = 2 * ifree + 1;
//
//  Does the first position exist?
//
      if ( n <= m )
      {
        break;
      }
      else
      {
//
//  Does the second position exist?
//
        if ( m + 1 < n )
        {
//
//  If both positions exist, take the larger of the two values,
//  and update M if necessary.
//
          if ( a[m] < a[m+1] )
          {
            m = m + 1;
          }
        }
//
//  If the large descendant is larger than KEY, move it up,
//  and update IFREE, the location of the free position, and
//  consider the descendants of THIS position.
//
        if ( key < a[m] )
        {
          a[ifree] = a[m];
          ifree = m;
        }
        else
        {
          break;
        }
      }
    }
//
//  When you have stopped shifting items up, return the item you
//  pulled out back to the heap.
//
    a[ifree] = key;
  }

  return;
}
//****************************************************************************80

int i4vec_heap_d_extract ( int *n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HEAP_D_EXTRACT extracts the maximum value from a descending heap.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    In other words, the routine finds the maximum value in the
//    heap, returns that value to the user, deletes that value from
//    the heap, and restores the heap to its proper form.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, page 150.
//
//  Parameters:
//
//    Input/output, int *N, the number of items in the heap.
//
//    Input/output, int A[N], the heap.
//
//    Output, int VALUE, the item of maximum value, which has been
//    removed from the heap.
//
{
  int value;

  if ( *n < 1 )
  {
    cout << "\n";
    cout << "I4VEC_HEAP_D_EXTRACT - Fatal error!\n";
    cout << "  The heap is empty.\n";
    exit ( 1 );
  }
//
//  Get the maximum value.
//
  value = a[0];

  if ( *n == 1 )
  {
    *n = 0;
    return value;
  }
//
//  Shift the last value down.
//
  a[0] = a[*n-1];
//
//  Restore the heap structure.
//
  *n = *n - 1;
  i4vec_sort_heap_d ( *n, a );

  return value;
}
//****************************************************************************80

void i4vec_heap_d_insert ( int *n, int a[], int value )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HEAP_D_INSERT inserts a new value into a descending heap.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, page 150.
//
//  Parameters:
//
//    Input/output, int *N, the number of items in the heap.
//
//    Input/output, int A[N], the heap.
//
//    Input, int VALUE, the value to be inserted.
//
{
  int i;
  int parent;

  *n = *n + 1;
  i = *n;

  while ( 1 < i )
  {
    parent = i / 2;

    if ( value <= a[parent-1] )
    {
      break;
    }
    a[i-1] = a[parent-1];
    i = parent;
  }

  a[i-1] = value;

  return;
}
//****************************************************************************80

int i4vec_heap_d_max ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HEAP_D_MAX returns the maximum value in a descending heap of integers.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    This is one of three functions needed to model a priority queue.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Thomas Cormen, Charles Leiserson, Ronald Rivest,
//    Introduction to Algorithms,
//    MIT Press, page 150.
//
//  Parameters:
//
//    Input, int N, the number of items in the heap.
//
//    Input, int A[N], the heap.
//
//    Output, int I4VEC_HEAP_D_MAX, the maximum value in the heap.
//
{
  int value;

  value = a[0];

  return value;
}
//****************************************************************************80

int *i4vec_histogram ( int n, int a[], int histo_num )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_HISTOGRAM computes a histogram of the elements of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    It is assumed that the entries in the vector A are nonnegative.
//    Only values between 0 and HISTO_NUM will be histogrammed.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the array to examine.
//
//    Input, int HISTO_NUM, the maximum value for which a
//    histogram entry will be computed.
//
//    Output, int I4VEC_HISTOGRAM[HISTO_NUM+1], contains the number of
//    entries of A with the values of 0 through HISTO_NUM.
//
{
  int *histo_gram;
  int i;

  histo_gram = new int[histo_num+1];

  for ( i = 0; i <= histo_num; i++ )
  {
    histo_gram[i] = 0;
  }

  for ( i = 0; i < n; i++ )
  {
    if ( 0 <= a[i] && a[i] <= histo_num )
    {
      histo_gram[a[i]] = histo_gram[a[i]] + 1;
    }
  }

  return histo_gram;
}
//****************************************************************************80

int i4vec_index ( int n, int a[], int aval )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX returns the location of the first occurrence of a given value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be searched.
//
//    Input, int AVAL, the value to be indexed.
//
//    Output, int I4VEC_INDEX, the first location in A which has the
//    value AVAL, or -1 if no such index exists.
//
{
  int i;
  int index;

  for ( i = 1; i <= n; i++ )
  {
    if ( a[i-1] == aval )
    {
      index = i;
      return index;
    }
  }

  index = -1;

  return index;
}
//****************************************************************************80

void i4vec_index_delete_all ( int n, int x[], int indx[], int xval, 
  int *n2, int x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_DELETE_ALL deletes all occurrences of a value from an indexed sorted list.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Note that the value of N is adjusted because of the deletions//
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, int X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, int XVAL, the value to be sought.
//
//    Output, int *N2, the size of the current list.
//
//    Output, int X2[N2], the list.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int equal;
  int equal1;
  int equal2;
  int get;
  int i;
  int less;
  int more;
  int put;

  if ( n < 1 )
  {
    *n2 = 0;
    return;
  }

  i4vec_copy ( n, indx, indx2 );
  i4vec_copy ( n, x, x2 );
  *n2 = n;

  i4vec_index_search ( *n2, x2, indx2, xval, &less, &equal, &more );

  if ( equal == 0 )
  {
    return;
  }

  equal1 = equal;

  for ( ; ; )
  {
    if ( equal1 <= 1 )
    {
      break;
    }

    if ( x2[indx2[equal1-2]-1] != xval )
    {
      break;
    }
    equal1 = equal1 - 1;
  }

  equal2 = equal;

  for ( ; ; )
  {
    if ( *n2 <= equal2 )
    {
      break;
    }

    if ( x2[indx2[equal2]-1] != xval )
    {
      break;
    }
    equal2 = equal2 + 1;
  }
//
//  Discard certain X values.
//
  put = 0;

  for ( get = 1; get <= *n2; get++ )
  {
    if ( x2[get-1] != xval )
    {
      put = put + 1;
      x2[put-1] = x2[get-1];
    }
  }
//
//  Adjust the INDX values.
//
  for ( equal = equal1; equal <= equal2; equal++ )
  {
    for ( i = 1; i <= *n2; i++ )
    {
      if ( indx2[equal-1] < indx2[i-1] )
      {
        indx2[i-1] = indx2[i-1] - 1;
      }
    }
  }
//
//  Discard certain INDX values.
//
  for ( i = 0; i <= *n2 - equal2 - 1; i++ )
  {
    indx2[equal1+i-1] = indx2[equal2+i];
  }
  for ( i = *n2 + equal1 - equal2; i <= *n2; i++ )
  {
    indx2[i-1] = 0;
  }
//
//  Adjust N.
//
  *n2 = put;

  return;
}
//****************************************************************************80

void i4vec_index_delete_dupes ( int n, int x[], int indx[], 
  int *n2, int x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_DELETE_DUPES deletes duplicates from an indexed sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The output quantities N2, X2, and INDX2 are computed from the
//    input quantities by sorting, and eliminating duplicates.
//
//    The output arrays should be dimensioned of size N, unless the user
//    knows in advance what the value of N2 will be.
//
//    The output arrays may be identified with the input arrays.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the input list.
//
//    Input, int X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Output, int *N2, the number of unique entries in X.
//
//    Output, int X2[N2], a copy of the list which has
//    been sorted, and made unique.
//
//    Output, int INDX2[N2], the sort index of the new list.
//
{
  int i;
  int n3;
  int *x3;

  i = 0;
  n3 = 0;
  x3 = new int[n];

  for ( ; ; )
  {
    i = i + 1;

    if ( n < i )
    {
      break;
    }

    if ( 1 < i )
    {
      if ( x[indx[i-1]-1] == x3[n3-1] )
      {
        continue;
      }
    }
    n3 = n3 + 1;
    x3[n3-1] = x[indx[i-1]-1];
  }
//
//  Set the output data.
//
  *n2 = n3;
  i4vec_copy ( n3, x3, x2 );
  for ( i = 0; i < n3; i++ )
  {
    indx2[i] = i + 1;
  }

  delete [] x3;

  return;
}
//****************************************************************************80

void i4vec_index_delete_one ( int n, int x[], int indx[], int xval, 
  int *n2, int x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_DELETE_ONE deletes one copy of an I4 from an indexed sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    If the value occurs in the list more than once, only one copy is deleted.
//
//    Note that the value of N is adjusted because of the deletions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 October 2000
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, int X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, int XVAL, the value to be sought.
//
//    Output, int *N2, the size of the current list.
//
//    Output, int X2[N2], the list.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int equal;
  int i;
  int j;
  int less;
  int more;

  if ( n < 1 )
  {
    *n2 = 0;
    return;
  }

  *n2 = n;
  i4vec_copy ( *n2, indx, indx2 );
  i4vec_copy ( *n2, x, x2 );

  i4vec_index_search ( *n2, x2, indx2, xval, &less, &equal, &more );

  if ( equal != 0 )
  {
    j = indx2[equal-1];
    for ( i = j; i <= *n2-1; i++ )
    {
      x2[i-1] = x[i];
    }
    for ( i = equal; i <= *n2-1; i++ )
    {
      indx2[i-1] = indx2[i];
    }
    for ( i = 1; i <= *n2 - 1; i++ )
    {
      if ( j < indx2[i-1] )
      {
        indx2[i-1] = indx2[i-1] - 1;
      }
    }
    *n2 = *n2 - 1;
  }

  return;
}
//****************************************************************************80

void i4vec_index_insert ( int *n, int x[], int indx[], int xval )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_INSERT inserts an I4 into an indexed sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *N, the size of the current list.
//
//    Input, int X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, int XVAL, the value to be sought.
//
{
  int equal;
  int i;
  int less;
  int more;

  if ( *n <= 0 )
  {
    *n = 1;
    x[0] = xval;
    indx[0] = 1;
    return;
  }

  i4vec_index_search ( *n, x, indx, xval, &less, &equal, &more );

  x[*n] = xval;
  for ( i = *n; more <= i; i-- )
  {
    indx[i] = indx[i-1];
  }
  indx[more-1] = *n + 1;
  *n = *n + 1;

  return;
}
//****************************************************************************80

void i4vec_index_insert_unique ( int *n, int x[], int indx[], int xval )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_INSERT_UNIQUE inserts a unique I4 in an indexed sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *N, the size of the current list.
//    If the input value XVAL does not already occur in X, then N is increased.
//
//    Input/output, int X[N], the list.
//    If the input value XVAL does not already occur in X, then it is added
//    to X.
//
//    Input/output, int INDX[N], the sort index of the list.
//    If the input value XVAL does not already occur in X, then INDX is updated.
//
//    Input, int XVAL, the value which will be inserted into the X
//    vector if it is not there already.
//
{
  int equal;
  int i;
  int less;
  int more;

  if ( *n <= 0 )
  {
    *n = 1;
    x[0] = xval;
    indx[0] = 1;
    return;
  }
//
//  Does XVAL already occur in X?
//
  i4vec_index_search ( *n, x, indx, xval, &less, &equal, &more );

  if ( equal == 0 )
  {
    x[*n] = xval;
    for ( i = *n; more <= i; i-- )
    {
      indx[i] = indx[i-1];
    }
    indx[more-1] = *n + 1;
    *n = *n + 1;
  }

  return;
}
//****************************************************************************80

void i4vec_index_order ( int n, int x[], int indx[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_ORDER sorts an I4VEC using an index vector.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The index vector itself is not modified.  Therefore, the pair
//    (X,INDX) no longer represents an index sorted vector.  If this
//    relationship is to be preserved, then simply set INDX(1:N)=(1:N).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input/output, int X[N], the list.  On output, the list
//    has been sorted.
//
//    Input, int INDX[N], the sort index of the list.
//
{
  int i;
  int *y;

  y = new int[n];

  for ( i = 0; i < n; i++ )
  {
    y[i] = x[indx[i]-1];
  }
  for ( i = 0; i < n; i++ )
  {
    x[i] = y[i];
  }
  delete [] y;

  return;
}
//****************************************************************************80

void i4vec_index_search ( int n, int x[], int indx[], int xval, int *less, 
  int *equal, int *more )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_SEARCH searches for an I4 in an indexed sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, int X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, int XVAL, the value to be sought.
//
//    Output, int *LESS, *EQUAL, *MORE, the indexes in INDX of the
//    entries of X that are just less than, equal to, and just greater
//    than XVAL.  If XVAL does not occur in X, then EQUAL is zero.
//    If XVAL is the minimum entry of X, then LESS is 0.  If XVAL
//    is the greatest entry of X, then MORE is N+1.
//
{
  int hi;
  int lo;
  int mid;
  int xhi;
  int xlo;
  int xmid;

  if ( n <= 0 )
  {
    *less = 0;
    *equal = 0;
    *more = 0;
    return;
  }

  lo = 1;
  hi = n;
  xlo = x[indx[lo-1]-1];
  xhi = x[indx[hi-1]-1];

  if ( xval < xlo )
  {
    *less = 0;
    *equal = 0;
    *more = 1;
    return;
  }
  else if ( xval == xlo )
  {
    *less = 0;
    *equal = 1;
    *more = 2;
    return;
  }

  if ( xhi < xval )
  {
    *less = n;
    *equal = 0;
    *more = n + 1;
    return;
  }
  else if ( xval == xhi )
  {
    *less = n - 1;
    *equal = n;
    *more = n + 1;
    return;
  }

  for ( ; ; )
  {
    if ( lo + 1 == hi )
    {
      *less = lo;
      *equal = 0;
      *more = hi;
      return;
    }

    mid = ( lo + hi ) / 2;
    xmid = x[indx[mid-1]-1];

    if ( xval == xmid )
    {
      *equal = mid;
      *less = mid - 1;
      *more = mid + 1;
      return;
    }
    else if ( xval < xmid )
    {
      hi = mid;
    }
    else if ( xmid < xval )
    {
      lo = mid;
    }
  }
  return;
}
//****************************************************************************80

void i4vec_index_sort_unique ( int n, int x[], int *n2, int x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDEX_SORT_UNIQUE creates a sort index for an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, int X[N], the list.  
//
//    Output, int *N2, the number of unique elements in X.
//
//    Output, int X2[N2], a list of the unique elements of X.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int i;

  *n2 = 0;

  for ( i = 0; i < n; i++ )
  {
    i4vec_index_insert_unique ( n2, x2, indx2, x[i] );
  }

  for ( i = *n2; i < n; i++ )
  {
    x2[i] = -1;
  }
  for ( i = *n2; i < n; i++ )
  {
    indx2[i] = -1;
  }

  return;
}
//****************************************************************************80

int *i4vec_indicator ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDICATOR sets an I4VEC to the indicator vector.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 February 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Output, int I4VEC_INDICATOR(N), the initialized array.
//
{
  int *a;
  int i;

  a = new int[n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = i + 1;
  }
  return a;
}
//****************************************************************************80

void i4vec_insert ( int n, int a[], int pos, int value )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INSERT inserts a value into an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the array on input.
//
//    Input/output, int A[N+1], the array.  On input, A is assumed
//    to contain N entries.  On output, A actually contains N+1 entries.
//
//    Input, int POS, the position to be assigned the new entry.
//    1 <= POS <= N+1.
//
//    Input, int VALUE, the value to be inserted at the given position.
//
{
  int i;

  if ( pos < 1 || n+1 < pos )
  {
    cout << "\n";
    cout << "I4VEC_INSERT - Fatal error!\n";
    cout << "  Illegal insertion position = " << pos << "\n";
    exit ( 1 );
  }
  else
  {
    for ( i = n+1; pos+1 <= i; i-- )
    {
      a[i-1] = a[i-2];
    }
    a[pos-1] = value;
  }

  return;
}
//****************************************************************************80

int i4vec_max ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MAX returns the value of the maximum element in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array to be checked.
//
//    Output, int I4VEC_MAX, the value of the maximum element.  This
//    is set to 0 if N <= 0.
//
{
  int i;
  int value;

  if ( n <= 0 )
  {
    return 0;
  }

  value = a[0];

  for ( i = 1; i < n; i++ )
  {
    if ( value < a[i] )
    {
      value = a[i];
    }
  }

  return value;  
}
//****************************************************************************80

int i4vec_max_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MAX_INDEX returns the index of the maximum value in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_MAX_INDEX, the index of the largest entry.
//
{
  int i;
  int max_index;

  if ( n <= 0 )
  {
    max_index = -1;
  }
  else
  {
    max_index = 0;

    for ( i = 1; i < n; i++ )
    {
      if ( a[max_index] < a[i] )
      {
        max_index = i;
      }
    }
  }

  return max_index;
}
//****************************************************************************80

int i4vec_max_index_last ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MAX_INDEX_LAST: index of the last maximum value in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_MAX_INDEX_LAST, the index of the last largest entry.
//
{
  int i;
  int max_index_last;

  if ( n <= 0 )
  {
    max_index_last = -1;
  }
  else
  {
    max_index_last = 0;

    for ( i = n-1; 0 <= i; i-- )
    {
      if ( a[max_index_last] < a[i] )
      {
        max_index_last = i;
      }
    }
  }

  return max_index_last;
}
//****************************************************************************80

double i4vec_mean ( int n, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MEAN returns the mean of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int X[N], the vector whose mean is desired.
//
//    Output, double I4VEC_MEAN, the mean, or average, of the vector entries.
//
{
  int i;
  double mean;

  mean = 0.0;
  for ( i = 0; i < n; i++ )
  {
    mean = mean + ( double ) x[i];
  }

  mean = mean / ( double ) n;

  return mean;
}
//****************************************************************************80

int i4vec_median ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MEDIAN returns the median of an unsorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Hoare's algorithm is used.  The values of the vector are
//    rearranged by this routine.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, int A[N], the array to search.  On output,
//    the order of the elements of A has been somewhat changed.
//
//    Output, int I4VEC_MEDIAN, the value of the median of A.
//
{
  int k;
  int median;

  k = ( n + 1 ) / 2;

  median = i4vec_frac ( n, a, k );

  return median;
}
//****************************************************************************80

int *i4vec_merge_a ( int na, int a[], int nb, int b[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MERGE_A merges two ascending sorted I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The elements of A and B should be sorted in ascending order.
//
//    The elements in the output array C will also be in ascending order,
//    and unique.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NA, the dimension of A.
//
//    Input, int A[NA], the first sorted array.
//
//    Input, int NB, the dimension of B.
//
//    Input, int B[NB], the second sorted array.
//
//    Output, int C[NA+NB], the merged unique sorted array.
//
{
  int *c;
  int j;
  int ja;
  int jb;
  int nc;
  int order;

  ja = 0;
  jb = 0;
  nc = 0;

  order = i4vec_order_type ( na, a );

  if ( order < 0 || 2 < order )
  {
    cout << "\n";
    cout << "I4VEC_MERGE_A - Fatal error!\n";
    cout << "  The input array A is not ascending sorted.\n";
    exit ( 1 );
  }

  order = i4vec_order_type ( nb, b );

  if ( order < 0 || 2 < order )
  {
    cout << "\n";
    cout << "I4VEC_MERGE_A - Fatal error!\n";
    cout << "  The input array B is not ascending sorted.\n";
    exit ( 1 );
  }

  c = new int[na+nb];

  for ( ; ; )
  {
//
//  If we've used up all the entries of A, stick the rest of B on the end.
//
    if ( na <= ja )
    {
      for ( j = 1; j <= nb - jb; j++ )
      {
        if ( nc == 0 || c[nc] < b[jb] )
        {
          c[nc] = b[jb];
          nc = nc + 1;
        }
        jb = jb + 1;
      }
      break;
    }
//
//  If we've used up all the entries of B, stick the rest of A on the end.
//
    else if ( nb <= jb )
    {
     for ( j = 1; j <= na - ja; j++ )
     {
        if ( nc == 0 || c[nc] < a[ja] )
        {
          c[nc] = a[ja];
          nc = nc + 1;
        }
        ja = ja + 1;
      }
      break;
    }
//
//  Otherwise, if the next entry of A is smaller, that's our candidate.
//
    else if ( a[ja] <= b[jb] )
    {
      if ( nc == 0 || c[nc] < a[ja] )
      {
        c[nc] = a[ja];
        nc = nc + 1;
      }
      ja = ja + 1;
    }
//
//  ...or if the next entry of B is the smaller, consider that.
//
    else
    {
      if ( nc == 0 || c[nc] < b[jb] )
      {
        c[nc] = b[jb];
        nc = nc + 1;
      }
      jb = jb + 1;
    }
  }
  return c;
}
//****************************************************************************80

int i4vec_min ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MIN returns the value of the minimum element in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array to be checked.
//
//    Output, int I4VEC_MIN, the value of the minimum element.  This
//    is set to 0 if N <= 0.
//
{
  int i;
  int value;

  if ( n <= 0 )
  {
    return 0;
  }

  value = a[0];

  for ( i = 1; i < n; i++ )
  {
    if ( a[i] < value )
    {
      value = a[i];
    }
  }
  return value; 
}
//****************************************************************************80

int i4vec_min_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_MIN_INDEX returns the index of the minimum value in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_MIN_INDEX, the index of the smallest entry.
//
{
  int i;
  int min_index;

  if ( n <= 0 )
  {
    min_index = -1;
  }
  else
  {
    min_index = 0;

    for ( i = 1; i < n; i++ )
    {
      if ( a[i] < a[min_index] )
      {
        min_index = i;
      }
    }
  }

  return min_index;
}
//****************************************************************************80

int i4vec_nonzero_count ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_NONZERO_COUNT counts the nonzero entries in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the input array.
//
//    Input, int A[N], an array.
//
//    Output, int NONZERO_COUNT, the number of nonzero entries.
//
{
  int i;
  int nonzero_count;

  nonzero_count = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] != 0 )
    {
      nonzero_count = nonzero_count + 1;
    }
  }

  return nonzero_count;
}
//****************************************************************************80

void i4vec_nonzero_first ( int n, int x[], int *nz, int indx[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_NONZERO_FIRST left-shifts all nonzeros in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The routine preserves the ordering of the nonzero entries.  It counts
//    the nonzeros, and returns an index vector.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int X[N], the vector to be shifted.
//
//    Output, int *NZ, the number of nonzero entries in the vector.
//
//    Output, int INDX[N], contains the original location of each entry.
//
{
  int j;
  int k;

  *nz = 0;

  for ( j = 1; j <= n; j++ )
  {
    indx[j-1] = j;
  }

  j = 0;

  while ( j < n )
  {
    j = j + 1;

    if ( x[j-1] != 0 )
    {
      *nz = *nz + 1;

      if ( *nz != j )
      {
        x[*nz-1] = x[j-1];
        x[j-1] = 0;

        k = indx[*nz-1];
        indx[*nz-1] = j;
        indx[j-1] = k;
      }
    }
  }

  return;
}
//****************************************************************************80

int i4vec_order_type ( int n, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ORDER_TYPE: is an I4VEC is (non)strictly ascending/descending?
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the array.
//
//    Input, int X[N], the array to be checked.
//
//    Output, int I4VEC_ORDER_TYPE, order indicator:
//    -1, no discernable order;
//    0, all entries are equal;
//    1, ascending order;
//    2, strictly ascending order;
//    3, descending order;
//    4, strictly descending order.
//
{
  int i;
  int order;
// 
//  Search for the first value not equal to X(0). 
//
  i = 0;

  for ( ; ; ) 
  {
    i = i + 1;
    if ( n-1 < i ) 
    {
      order = 0;
      return order;
    }

    if ( x[0] < x[i] ) 
    {
      if ( i == 1 ) 
      {
        order = 2;
        break;
      }
      else
      {
        order = 1;
        break;
      }
    }
    else if ( x[i] < x[0] ) 
    {
      if ( i == 1 ) 
      {
        order = 4;
        break;
      }
      else
      {
        order = 3;
        break;
      }
    }
  }
// 
//  Now we have a "direction".  Examine subsequent entries. 
//
  for ( ; ; ) 
  {
    i = i + 1;
    if ( n - 1 < i ) 
    {
      break;
    }

    if ( order == 1 )
    {
      if ( x[i] < x[i-1] )
      {
        order = -1;
        break;
      }
    }
    else if ( order == 2 ) 
    {
      if ( x[i] < x[i-1] )
      {
        order = -1;
        break;
      }
      else if ( x[i] == x[i-1] ) 
      {
        order = 1;
      }
    }
    else if ( order == 3 ) 
    {
      if ( x[i-1] < x[i] ) 
      {
        order = -1;
        break;
      }
    }
    else if ( order == 4 ) 
    {
      if ( x[i-1] < x[i] ) 
      {
        order = -1;
        break;
      }
      else if ( x[i] == x[i-1] ) 
      {
        order = 3;
      }
    }
  }
  return order;
}
//****************************************************************************80

bool i4vec_pairwise_prime ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PAIRWISE_PRIME checks whether an I4VEC is pairwise prime.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Two positive integers I and J are pairwise prime if they have no common
//    factor greater than 1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of values to check.
//
//    Input, int A[N], the vector of integers.
//
//    Output, bool I4VEC_PAIRWISE_PRIME, is TRUE if the vector of integers
//    is pairwise prime.
//
{
  int i;
  int j;
  int pairwise_prime;

  pairwise_prime = false;

  for ( i = 0; i < n; i++ )
  {
    for ( j = i+1; j < n; j++ )
    {
      if ( i4_gcd ( a[i], a[j] ) != 1 )
      {
        return pairwise_prime;
      }
    }
  }

  pairwise_prime = true;

  return pairwise_prime;
}
//****************************************************************************80

void i4vec_part ( int n, int nval, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PART partitions an int NVAL into N nearly equal parts.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      N = 5, NVAL = 17
//
//    Output:
//
//      X = ( 4, 4, 3, 3, 3 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int NVAL, the integer to be partitioned.
//    NVAL may be positive, zero, or negative.
//
//    Output, int X[N], the partition of NVAL.  The entries of
//    X add up to NVAL.  The entries of X are either all equal, or
//    differ by at most 1.  The entries of X all have the same sign
//    as NVAL, and the "largest" entries occur first.
//
{
  int i;
  int j;

  for ( i = 0; i < n; i++ )
  {
    x[i] = 0;
  }

  if ( 0 < nval )
  {
    j = 0;
    for ( i = 0; i < nval; i++ )
    {
      x[j] = x[j] + 1;
      j = j + 1;
      if ( n <= j )
      {
        j = 0;
      }
    }

  }
  else if ( nval < 0 )
  {
    j = 0;
    for ( i = nval; i < 0; i++ )
    {
      x[j] = x[j] - 1;
      j = j + 1;
      if ( n <= j )
      {
        j = 0;
      }
    }

  }

  return;
}
//****************************************************************************80

void i4vec_part_quick_a ( int n, int a[], int *l, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PART_QUICK_A reorders an I4VEC as part of a quick sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    I4VEC_PART_QUICK_A reorders the entries of A.  Using A[0] as a
//    key, all entries of A that are less than or equal to A[0] will
//    precede A[0] which precedes all entries that are greater than A[0].
//
//  Example:
//
//    Input:
//
//      N = 8
//
//      A = ( 6, 7, 3, 1, 6, 8, 2, 9 )
//
//    Output:
//
//      L = 3, R = 6
//
//      A = ( 3, 1, 2, 6, 6, 8, 9, 7 )
//            -------        -------
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, int A[N].  On input, the array to be checked.
//    On output, A has been reordered as described above.
//
//    Output, int L, R, the indices of A that define the three segments.
//    Let KEY = the input value of A[0].  Then
//    I <= L             A(I) < KEY;
//     L < I < R         A(I) = KEY;
//             R <= I    A(I) > KEY.
//
{
  int i;
  int key;
  int m;
  int temp;

  if ( n < 1 )
  {
    cout << "\n";
    cout << "I4VEC_PART_QUICK_A - Fatal error!\n";
    cout << "  N < 1.\n";
    exit ( 1 );
  }
  else if ( n == 1 )
  {
    *l = 0;
    *r = 2;
    return;
  }

  key = a[0];
  m = 1;
//
//  The elements of unknown size have indices between L+1 and R-1.
//
  *l = 1;
  *r = n + 1;

  for ( i = 2; i <= n; i++ )
  {

    if ( key < a[*l] )
    {
      *r = *r - 1;
      temp = a[*r-1];
      a[*r-1] = a[*l];
      a[*l] = temp;
    }
    else if ( a[*l] == key )
    {
      m = m + 1;
      temp = a[m-1];
      a[m-1] = a[*l];
      a[*l] = temp;
      *l = *l + 1;
    }
    else if ( a[*l] < key )
    {
      *l = *l + 1;
    }

  }
//
//  Now shift small elements to the left, and KEY elements to center.
//
  for ( i = 1; i <= *l -m; i++ )
  {
    a[i-1] = a[i+m-1];
  }

  *l = *l - m;

  for ( i = *l+1; i <= *l+m; i++ )
  {
    a[i-1] = key;
  }

  return;
}
//****************************************************************************80

void i4vec_permute ( int n, int p[], int base, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PERMUTE permutes an I4VEC in place.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    This routine permutes an array of integer "objects", but the same
//    logic can be used to permute an array of objects of any arithmetic
//    type, or an array of objects of any complexity.  The only temporary
//    storage required is enough to store a single object.  The number
//    of data movements made is N + the number of cycles of order 2 or more,
//    which is never more than N + N/2.
//
//  Example:
//
//    Input:
//
//      N = 5
//      P = (   1,   3,   4,   0,   2 )
//      A = (   1,   2,   3,   4,   5 )
//
//    Output:
//
//      A    = (   2,   4,   5,   1,   3 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int P[N], the permutation.  P(I) = J means
//    that the I-th element of the output array should be the J-th
//    element of the input array.  
//
//    Input, int BASE, is 0 for a 0-based permutation and 1 for 
//    a 1-based permutation.
//
//    Input/output, int A[N], the array to be permuted.
//
{
  int a_temp;
  int i;
  int ierror;
  int iget;
  int iput;
  int istart;

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "I4VEC_PERMUTE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }
//
//  In order for the sign negation trick to work, we need to assume that the
//  entries of P are strictly positive.  Presumably, the lowest number is BASE.
//  So temporarily add 1-BASE to each entry to force positivity.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] + 1 - base;
  }
//
//  Search for the next element of the permutation that has not been used.
//
  for ( istart = 1; istart <= n; istart++ )
  {
    if ( p[istart-1] < 0 )
    {
      continue;
    }
    else if ( p[istart-1] == istart )
    {
      p[istart-1] = - p[istart-1];
      continue;
    }
    else
    {
      a_temp = a[istart-1];
      iget = istart;
//
//  Copy the new value into the vacated entry.
//
      for ( ; ; )
      {
        iput = iget;
        iget = p[iget-1];

        p[iput-1] = - p[iput-1];

        if ( iget < 1 || n < iget )
        {
          cout << "\n";
          cout << "I4VEC_PERMUTE - Fatal error!\n";
          cout << "  Entry IPUT = " << iput << " of the permutation has\n";
          cout << "  an illegal value IGET = " << iget << ".\n";
          exit ( 1 );
        }

        if ( iget == istart )
        {
          a[iput-1] = a_temp;
          break;
        }
        a[iput-1] = a[iget-1];
      }
    }
  }
//
//  Restore the signs of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = - p[i];
  }
//
//  Restore the base of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] - 1 + base;
  }

  return;
}
//****************************************************************************80

void i4vec_permute_uniform ( int n, int a[], int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PERMUTE_UNIFORM randomly permutes an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input/output, int A[N], the array to be permuted.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
{
  int base = 0;
  int *p;

  p = perm_uniform ( n, base, seed );

  i4vec_permute ( n, p, base, a );

  delete [] p;

  return;
}
//****************************************************************************80

void i4vec_print ( int n, int a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PRINT prints an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, int A[N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i < n; i++ ) 
  {
    cout << setw(6) << i << "  " 
         << setw(8) << a[i]  << "\n";
  }

  return;
}
//****************************************************************************80

void i4vec_print_some ( int n, int a[], int i_lo, int i_hi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PRINT_SOME prints "some" of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4 values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the vector.
//
//    Input, int A[N], the vector to be printed.
//
//    Input, int I_LO, I_HI, the first and last indices to print.
//    The routine expects 1 <= I_LO <= I_HI <= N.
//
//    Input, char *TITLE, an optional title.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = i4_max ( 1, i_lo ); i <= i4_min ( n, i_hi ); i++ )
  {
    cout << "  " << setw(8)  << i      << "  "
         << "  " << setw(12) << a[i-1] << "\n";
  }

  return;
}
//****************************************************************************80

int i4vec_product ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_PRODUCT multiplies the entries of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      A = ( 1, 2, 3, 4 )
//
//    Output:
//
//      I4VEC_PRODUCT = 24
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector
//
//    Output, int I4VEC_PRODUCT, the product of the entries of A.
//
{
  int i;
  int product;

  product = 1;
  for ( i = 0; i < n; i++ )
  {
    product = product * a[i];
  }

  return product;
}
//****************************************************************************80

int i4vec_red ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_RED divides out common factors in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int A[*], the vector to be reduced.
//    On output, the entries have no common factor
//    greater than 1.
//
//    Output, int I4VEC_RED, the common factor that was divided out.
//
{
  int factor;
  int i;
//
//  Find the smallest nonzero value.
//
  factor = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] != 0 )
    {
      if ( factor == 0 )
      {
        factor = abs ( a[i] );
      }
      else if ( abs ( a[i] ) < factor )
      {
        factor = abs ( a[i] );
      }
    }
  }

  if ( factor == 0 )
  {
    return factor;
  }
//
//  Find the greatest common factor of the entire vector.
//
  for ( i = 0; i < n; i++ )
  {
    factor = i4_gcd ( a[i], factor );
  }

  if ( factor == 1 )
  {
    return factor;
  }
//
//  Divide out the common factor.
//
  for ( i = 0; i < n; i++ )
  {
    a[i] = a[i] / factor;
  }

  return factor;
}
//****************************************************************************80

void i4vec_reverse ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_REVERSE reverses the elements of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      N = 5,
//      A = ( 11, 12, 13, 14, 15 ).
//
//    Output:
//
//      A = ( 15, 14, 13, 12, 11 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, int A(N), the array to be reversed.
//
{
  int i;
  int j;

  for ( i = 0; i < n / 2; i++ )
  {
    j        = a[i];
    a[i]     = a[n-1-i];
    a[n-1-i] = j;
  }

  return;
}
//****************************************************************************80

void i4vec_rotate ( int n, int m, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ROTATE rotates an I4VEC in place.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      N = 5, M = 2
//      X    = ( 1, 2, 3, 4, 5 )
//
//    Output:
//
//      X    = ( 4, 5, 1, 2, 3 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int M, the number of positions to the right that
//    each element should be moved.  Elements that shift pass position
//    N "wrap around" to the beginning of the array.
//
//    Input/output, int X[N], the array to be rotated.
//
{
  int iget;
  int iput;
  int istart;
  int mcopy;
  int nset;
  int temp;
//
//  Force M to be positive, between 0 and N-1.
//
  mcopy = i4_modp ( m, n );

  if ( mcopy == 0 )
  {
    return;
  }

  istart = 0;
  nset = 0;

  for ( ; ; )
  {
    if ( n <= istart )
    {
      return;
    }

    temp = x[istart];
    iget = istart;
//
//  Copy the new value into the vacated entry.
//
    for ( ; ; )
    {
      iput = iget;

      iget = iget - mcopy;

      if ( iget < 0 )
      {
        iget = iget + n;
      }

      if ( iget == istart )
      {
        break;
      }

      x[iput] = x[iget];
      nset = nset + 1;

    }

    x[iput] = temp;
    nset = nset + 1;

    if ( n <= nset )
    {
      break;
    }
      
    istart = istart + 1;

  }

  return;
}
//****************************************************************************80

int i4vec_run_count ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_RUN_COUNT counts runs of equal values in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    A run is a sequence of equal values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be examined.
//
//    Output, int I4VEC_RUN_COUNT, the number of runs.
//
{
  int i;
  int run_count;
  int test;

  run_count = 0;

  if ( n < 1 )
  {
    return run_count;
  }

  test = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( i == 0 || a[i] != test )
    {
      run_count = run_count + 1;
      test = a[i];
    }
  }

  return run_count;
}
//****************************************************************************80

int i4vec_search_binary_a ( int n, int a[], int b )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SEARCH_BINARY_A searches an ascending sorted I4VEC for a value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Binary search is used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.9,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 26.
//
//  Parameters:
//
//    Input, int N, the number of elements in the vector.
//
//    Input, int A[N], the array to be searched.  A must
//    be sorted in ascending order.
//
//    Input, int B, the value to be searched for.
//
//    Output, int I4VEC_SEARCH_BINARY_A, the result of the search.
//    -1, B does not occur in A.
//    I, A[I] = B.
//
{
  int high;
  int index;
  int low;
  int mid;
//
//  Check.
//
  if ( n <= 0 )
  {
    cout << "\n";
    cout << "I4VEC_SEARCH_BINARY_A - Fatal error!\n";
    cout << "  The array dimension N is less than 1.\n";
    exit ( 1 );
  }

  index = -1;

  low = 1;
  high = n;

  while ( low <= high )
  {
    mid = ( low + high ) / 2;

    if ( a[mid-1] == b )
    {
      index = mid;
      break;
    }
    else if ( a[mid-1] < b )
    {
      low = mid + 1;
    }
    else if ( b < a[mid-1] )
    {
      high = mid - 1;
    }
  }
  return index;
}
//****************************************************************************80

int i4vec_search_binary_d ( int n, int a[], int b )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SEARCH_BINARY_D searches a descending sorted I4VEC for a value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Binary search is used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.9,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 26.
//
//  Parameters:
//
//    Input, int N, the number of elements in the vector.
//
//    Input, int A[N], the array to be searched.  A must
//    be sorted in descending order.
//
//    Input, int B, the value to be searched for.
//
//    Output, int I4VEC_SEARCH_BINARY_D, the result of the search.
//    -1, B does not occur in A.
//    I, A[I] = B.
//
{
  int high;
  int index;
  int low;
  int mid;
//
//  Check.
//
  if ( n <= 0 )
  {
    cout << "\n";
    cout << "I4VEC_SEARCH_BINARY_D - Fatal error!\n";
    cout << "  The array dimension N is less than 1.\n";
    exit ( 1 );
  }

  index = -1;

  low = 1;
  high = n;

  while ( low <= high )
  {
    mid = ( low + high ) / 2;

    if ( a[mid-1] == b )
    {
      index = mid;
      break;
    }
    else if ( b < a[mid-1] )
    {
      low = mid + 1;
    }
    else if ( a[mid-1] < b )
    {
      high = mid - 1;
    }
  }
  return index;
}
//****************************************************************************80

void i4vec_sort_bubble_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_BUBBLE_A ascending sorts an I4VEC using bubble sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input/output, int A[N].
//    On input, an unsorted array of ints.
//    On output, A has been sorted.
//
{
  int i;
  int j;
  int temp;

  for ( i = 0; i < n-1; i++ )
  {
    for ( j = i+1; j < n; j++ )
    {
      if ( a[j] < a[i] )
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
  return;
}
//****************************************************************************80

void i4vec_sort_bubble_d ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_BUBBLE_D descending sorts an I4VEC using bubble sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input/output, int A[N].
//    On input, an unsorted array of ints.
//    On output, A has been sorted.
//
{
  int i;
  int j;
  int temp;

  for ( i = 0; i < n-1; i++ )
  {
    for ( j = i+1; j < n; j++ )
    {
      if ( a[i] < a[j] )
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
  return;
}
//****************************************************************************80

void i4vec_sort_heap_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_HEAP_A ascending sorts an I4VEC using heap sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, int A[N].
//    On input, the array to be sorted;
//    On output, the array has been sorted.
//
{
  int n1;
  int temp;

  if ( n <= 1 )
  {
    return;
  }
//
//  1: Put A into descending heap form.
//
  i4vec_heap_d ( n, a );
//
//  2: Sort A.
//
//  The largest object in the heap is in A[0].
//  Move it to position A[N-1].
//
  temp = a[0];
  a[0] = a[n-1];
  a[n-1] = temp;
//
//  Consider the diminished heap of size N1.
//
  for ( n1 = n-1; 2 <= n1; n1-- )
  {
//
//  Restore the heap structure of the initial N1 entries of A.
//
    i4vec_heap_d ( n1, a );
//
//  Take the largest object from A[0] and move it to A[N1-1].
//
    temp = a[0];
    a[0] = a[n1-1];
    a[n1-1] = temp;
  }
  return;
}
//****************************************************************************80

void i4vec_sort_heap_d ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_HEAP_D descending sorts an I4VEC using heap sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, int A[N].
//    On input, the array to be sorted;
//    On output, the array has been sorted.
//
{
  int n1;
  int temp;

  if ( n <= 1 )
  {
    return;
  }
//
//  1: Put A into ascending heap form.
//
  i4vec_heap_a ( n, a );
//
//  2: Sort A.
//
//  The smallest object in the heap is in A[0].
//  Move it to position A[N-1].
//
  temp = a[0];
  a[0] = a[n-1];
  a[n-1] = temp;
//
//  Consider the diminished heap of size N1.
//
  for ( n1 = n-1; 2 <= n1; n1-- )
  {
//
//  Restore the heap structure of the initial N1 entries of A.
//
    i4vec_heap_a ( n1, a );
//
//  Take the smallest object from A[0] and move it to A[N1-1].
//
    temp = a[0];
    a[0] = a[n1-1];
    a[n1-1] = temp;
  }

  return;
}
//****************************************************************************80

int *i4vec_sort_heap_index_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      i4vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], an array to be index-sorted.
//
//    Output, int I4VEC_SORT_HEAP_INDEX_A[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  int aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {

    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j-1]] < a[indx[j]] )
        {
          j = j + 1;
        }
      }

      if ( aval < a[indx[j-1]] )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

int *i4vec_sort_heap_index_d ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_HEAP_INDEX_D does an indexed heap descending sort of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      i4vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, int A[N], an array to be index-sorted.
//
//    Output, int I4VEC_SORT_HEAP_INDEX_D[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  int aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j]] < a[indx[j-1]] )
        {
          j = j + 1;
        }
      }

      if ( a[indx[j-1]] < aval )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }

    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

void i4vec_sort_insert_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_INSERT_A uses an ascending insertion sort on an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.1,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 11.
//
//  Parameters:
//
//    Input, int N, the number of items in the vector.
//    N must be positive.
//
//    Input/output, int A[N].
//    On input, A contains data to be sorted.
//    On output, the entries of A have been sorted in ascending order.
//
{
  int i;
  int j;
  int x;

  for ( i = 1; i < n; i++ )
  {
    x = a[i];

    j = i;

    while ( 1 <= j && x < a[j-1] )
    {
      a[j] = a[j-1];
      j = j - 1;
    }

    a[j] = x;
  }

  return;
}
//****************************************************************************80

void i4vec_sort_insert_d ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_INSERT_D uses a descending insertion sort on an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.1,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 11.
//
//  Parameters:
//
//    Input, int N, the number of items in the vector.
//    N must be positive.
//
//    Input/output, int A[N].
//
//    On input, A contains data to be sorted.
//    On output, the entries of A have been sorted in ascending order.
//
{
  int i;
  int j;
  int x;

  for ( i = 1; i < n; i++ )
  {
    x = a[i];
    j = i;

    while ( 1 <= j && a[j-1] < x )
    {
      a[j] = a[j-1];
      j = j - 1;
    }
    a[j] = x;
  }

  return;
}
//****************************************************************************80

void i4vec_sort_quick_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_QUICK_A ascending sorts an I4VEC using quick sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      N = 7
//
//      A = ( 6, 7, 3, 2, 9, 1, 8 )
//
//    Output:
//
//      A = ( 1, 2, 3, 6, 7, 8, 9 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, int A[N].  On input, the array to be sorted.
//    On output, A has been reordered into ascending order.
//
{
# define LEVEL_MAX 30

  int base;
  int l_segment;
  int level;
  int n_segment;
  int rsave[LEVEL_MAX];
  int r_segment;

  if ( n < 1 )
  {
    cerr << "\n";
    cerr << "I4VEC_SORT_QUICK_A - Fatal error!\n";
    cerr << "  N < 1.\n";
    exit ( 1 );
  }
  else if ( n == 1 )
  {
    return;
  }

  level = 1;
  rsave[0] = n + 1;
  base = 1;
  n_segment = n;

  while ( 0 < n_segment )
  {
//
//  Partition the segment.
//
    i4vec_part_quick_a ( n_segment, a+base-1, &l_segment, &r_segment );
//
//  If the left segment has more than one element, we need to partition it.
//
    if ( 1 < l_segment )
    {
      if ( LEVEL_MAX < level )
      {
        cerr << "\n";
        cerr << "I4VEC_SORT_QUICK_A - Fatal error!\n";
        cerr << "  Exceeding recursion maximum of " << LEVEL_MAX << "\n";
        exit ( 1 );
      }

      level = level + 1;
      n_segment = l_segment;
      rsave[level-1] = r_segment + base - 1;
    }
//
//  The left segment and the middle segment are sorted.
//  Must the right segment be partitioned?
//
    else if ( r_segment < n_segment )
    {
      n_segment = n_segment + 1 - r_segment;
      base = base + r_segment - 1;
    }
//
//  Otherwise, we back up a level if there is an earlier one.
//
    else
    {
      for ( ; ; )
      {
        if ( 1 < level )
        {
          base = rsave[level-1];
          n_segment = rsave[level-2] - rsave[level-1];
          level = level - 1;
          if ( 0 < n_segment )
          {
            break;
          }
        }
        else
        {
          n_segment = 0;
          break;
        }
      }
    }
  }

  return;
# undef LEVEL_MAX
}
//****************************************************************************80

void i4vec_sort_shell_a ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORT_SHELL_A ascending sorts an I4VEC using Shell's sort.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, int A[N].
//    On input, an array to be sorted.
//    On output, the sorted array.
//
{
  int asave;
  int base;
  int i;
  int ifree;
  int inc;
  int ipow;
  int j;
  int k;
  int maxpow;

  if ( n <= 1 )
  {
    return;
  }
//
//  Determine the smallest MAXPOW so that
//    N <= ( 3**MAXPOW - 1 ) / 2
//
  maxpow = 1;
  base = 3;

  while ( base < 2 * n + 1 )
  {
    maxpow = maxpow + 1;
    base = base * 3;
  }

  if ( 1 < maxpow )
  {
    maxpow = maxpow - 1;
    base = base / 3;
  }
//
//  Now sort groups of size ( 3**IPOW - 1 ) / 2.
//
  for ( ipow = maxpow; 1 <= ipow; ipow = ipow - 1 )
  {
    inc = ( base - 1 ) / 2;
    base = base / 3;
//
//  Sort the values with indices equal to K mod INC.
//
    for ( k = 1; k <= inc; k++ )
    {
//
//  Insertion sort of the items with index
//  INC+K, 2*INC+K, 3*INC+K, ...
//
      for ( i = inc + k; i <= n; i = i + inc )
      {
        asave = a[i-1];
        ifree = i;
        j = i - inc;

        for ( ; ; )
        {
          if ( j < 1 )
          {
            break;
          }

          if ( a[j-1] <= asave )
          {
            break;
          }
          ifree = j;
          a[j+inc-1] = a[j-1];
          j = j - inc;
        }
        a[ifree-1] = asave;
      }
    }
  }
  return;
}
//****************************************************************************80

void i4vec_sorted_undex ( int x_num, int x_val[], int x_unique_num, int undx[], 
  int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORTED_UNDEX returns unique sorted indexes for a sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    Assuming X is already sorted, we examine the entries of X in order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(I) = X(UNDX(I)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I    X    XU  Undx  Xdnu
//    ----+----+----+-----+-----+
//      0 | 11 |  11    0     0
//      1 | 11 |  22    4     0
//      2 | 11 |  33    7     0
//      3 | 11 |  55    8     0
//      4 | 22 |              1
//      5 | 22 |              1
//      6 | 22 |              1
//      7 | 33 |              2
//      8 | 55 |              3
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_NUM, the number of data values.
//
//    Input, int X_VAL[X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  int i;
  int j;
//
//  Walk through the sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = i;

  xdnu[i] = j;

  for ( i = 1; i < x_num; i++ )
  {
    if ( x_val[i] != x_val[undx[j]] )
    {
      j = j + 1;
      undx[j] = i;
    }
    xdnu[i] = j;
  }

  return;
}
//****************************************************************************80

int i4vec_sorted_unique ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORTED_UNIQUE finds the unique elements in a sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements in A.
//
//    Input/output, int A[N].  On input, the sorted
//    integer array.  On output, the unique elements in A.
//
//    Output, int I4VEC_SORTED_UNIQUE, the number of unique elements in A.
//
{
  int i;
  int unique_num;

  unique_num = 0;

  if ( n <= 0 )
  {
    return unique_num;
  }

  unique_num = 1;

  for ( i = 1; i < n; i++ )
  {
    if ( a[i] != a[unique_num-1] ) 
    {
      unique_num = unique_num + 1;
      a[unique_num-1] = a[i];
    }
  }

  return unique_num;
}
//****************************************************************************80

int i4vec_sorted_unique_count ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORTED_UNIQUE_COUNT counts unique elements in a sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Because the array is sorted, this algorithm is O(N).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the sorted array to examine.
//
//    Output, int I4VEC_SORTED_UNIQUE_COUNT, the number of unique elements of A.
//
{
  int i;
  int unique_num;

  unique_num = 0;

  if ( n < 1 )
  {
    return unique_num;
  }

  unique_num = 1;

  for ( i = 1; i < n; i++ )
  {
    if ( a[i-1] != a[i] )
    {
      unique_num = unique_num + 1;
    }
  }

  return unique_num;
}
//****************************************************************************80

void i4vec_sorted_unique_hist ( int n, int a[], int maxuniq, int *unique_num, 
  int auniq[], int acount[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SORTED_UNIQUE_HIST histograms the unique elements of a sorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the array to examine, which must have been
//    sorted.
//
//    Input, int MAXUNIQ, the maximum number of unique elements
//    that can be handled.  If there are more than MAXUNIQ unique
//    elements in A, the excess will be ignored.
//
//    Output, int *UNIQUE_NUM, the number of unique elements of A.
//
//    Output, int AUNIQ[UNIQUE_NUM], the unique elements of A.
//
//    Output, int ACOUNT[UNIQUE_NUM], the number of times each element
//    of AUNIQ occurs in A.
//
{
  int i;
  int index;
//
//  Start taking statistics.
//
  index = -1;

  for ( i = 0; i < n; i++ )
  {
    if ( i == 0 )
    {
      index = 0;
      auniq[index] = a[0];
      acount[index] = 1;
    }
    else if ( a[i] == auniq[index] )
    {
      acount[index] = acount[index] + 1;
    }
    else if ( index + 1 < maxuniq ) 
    {
      index = index + 1;
      auniq[index] = a[i];
      acount[index] = 1;
    }
  }

  *unique_num = index + 1;

  return;
}
//****************************************************************************80

int i4vec_split ( int n, int a[], int split )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SPLIT "splits" an unsorted I4VEC based on a splitting value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    If the vector is already sorted, it is simpler to do a binary search
//    on the data than to call this routine.
//
//    The vector is not assumed to be sorted before input, and is not
//    sorted during processing.  If sorting is not needed, then it is
//    more efficient to use this routine.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, int A[N], the array to split.  On output,
//    all the entries of A that are less than or equal to SPLIT
//    are in A(1:I4VEC_SPLIT).
//
//    Input, int SPLIT, the value used to split the vector.
//    It is not necessary that any value of A actually equal SPLIT.
//
//    Output, int I4VEC_SPLIT, indicates the position of the last
//    entry of the split vector that is less than or equal to SPLIT.
//
{
  int i;
  int i1;
  int i2;
  int i3;
  int j1;
  int j2;
  int j3;
  int temp;
//
//  Partition the vector into A1, A2, A3, where
//    A1 = A(I1:J1) holds values <= SPLIT,
//    A2 = A(I2:J2) holds untested values,
//    A3 = A(I3:J3) holds values > SPLIT.
//
  i1 = 1;
  j1 = 0;

  i2 = 1;
  j2 = n;

  i3 = n+1;
  j3 = n;
//
//  Pick the next item from A2, and move it into A1 or A3.
//  Adjust indices appropriately.
//
  for ( i = 0; i < n; i++ )
  {
    if ( a[i2-1] <= split )
    {
      i2 = i2 + 1;
      j1 = j1 + 1;
    }
    else
    {
      temp    = a[i2-1];
      a[i2-1] = a[i3-2];
      a[i3-2] = temp;
      i3 = i3 - 1;
      j2 = j2 - 1;
    }
  }

  return j1;
}
//****************************************************************************80

int i4vec_sum ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SUM sums the entries of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      A = ( 1, 2, 3, 4 )
//
//    Output:
//
//      I4VEC_SUM = 10
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector to be summed.
//
//    Output, int I4VEC_SUM, the sum of the entries of A.
//
{
  int i;
  int sum;

  sum = 0;
  for ( i = 0; i < n; i++ )
  {
    sum = sum + a[i];
  }

  return sum;
}
//****************************************************************************80

void i4vec_swap ( int n, int a1[], int a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_SWAP swaps two I4VEC's.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the arrays.
//
//    Input/output, int A1[N], A2[N], the
//    two arrays whose entries are to be swapped.
//
{
  int i;
  int j;

  for ( i = 0; i < n; i++ )
  {
    j     = a1[i];
    a1[i] = a2[i];
    a2[i] = j;
  }
 
  return;
}
//****************************************************************************80

void i4vec_transpose_print ( int n, int a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_TRANSPOSE_PRINT prints an I4VEC "transposed".
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }
//    TITLE = "My vector:  "
//
//    My vector:      1    2    3    4    5
//                    6    7    8    9   10
//                   11
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, int A[N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank or NULL.
//
{
  int i;
  int ihi;
  int ilo;
  int title_len;

  if ( 0 < s_len_trim ( title ) )
  {
    title_len = strlen ( title );

    for ( ilo = 1; ilo <= n; ilo = ilo + 5 )
    {
      ihi = i4_min ( ilo + 5 - 1, n );
      if ( ilo == 1 )
      {
        cout << title;
      }
      else
      {
        for ( i = 1; i <= title_len; i++ )
        {
          cout << " ";
        }
      }
      for ( i = ilo; i <= ihi; i++ )
      {
        cout << setw(12) << a[i-1];
      }
      cout << "\n";
    }
  }
  else
  {
    for ( ilo = 1; ilo <= n; ilo = ilo + 5 )
    {
      ihi = i4_min ( ilo + 5 - 1, n );
      for ( i = ilo; i <= ihi; i++ )
      {
        cout << setw(12) << a[i-1];
      }
      cout << "\n";
    }
  }

  return;
}
//****************************************************************************80

void i4vec_undex ( int x_num, int x_val[], int x_unique_num, int undx[], 
  int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_UNDEX returns unique sorted indexes for an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of X, which creates arrays INDX and XDNI.  (If all the entries
//    of X are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of X in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(*) = X(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I    X  Indx  Xdni      XU  Undx  Xdnu
//    ----+----+-----+-----+-------+-----+-----+
//      0 | 11     0     0 |    11     0     0
//      1 | 22     2     4 |    22     1     1
//      2 | 11     5     1 |    33     3     0
//      3 | 33     8     7 |    55     4     2
//      4 | 55     1     8 |                 3
//      5 | 11     6     2 |                 0
//      6 | 22     7     5 |                 1
//      7 | 22     3     6 |                 1
//      8 | 11     4     3 |                 0
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_NUM, the number of data values.
//
//    Input, int X_VAL[X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  int i;
  int *indx;
  int j;
//
//  Implicitly sort the array.
//
  indx = i4vec_sort_heap_index_a ( x_num, x_val );
//
//  Walk through the implicitly sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = indx[i];

  xdnu[indx[i]] = j;

  for ( i = 1; i < x_num; i++ )
  {
    if ( x_val[indx[i]] != x_val[undx[j]] )
    {
      j = j + 1;
      undx[j] = indx[i];
    }
    xdnu[indx[i]] = j;
  }
  delete [] indx;

  return;
}
//****************************************************************************80

int *i4vec_uniform ( int n, int a, int b, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_UNIFORM returns a scaled pseudorandom I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The pseudorandom numbers should be uniformly distributed
//    between A and B.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley Interscience, page 95, 1998.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Peter Lewis, Allen Goodman, James Miller
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input, integer N, the dimension of the vector.
//
//    Input, int A, B, the limits of the interval.
//
//    Input/output, int *SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, int IVEC_UNIFORM[N], a vector of random values between A and B.
//
{
  int i;
  int k;
  float r;
  int value;
  int *x;
  
  if ( *seed == 0 )
  {
    cerr << "\n";
    cerr << "I4VEC_UNIFORM - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  x = new int[n];

  for ( i = 0; i < n; i++ )
  {
    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + 2147483647;
    }

    r = ( float ) ( *seed ) * 4.656612875E-10;
//
//  Scale R to lie between A-0.5 and B+0.5.
//
    r = ( 1.0 - r ) * ( ( float ) ( i4_min ( a, b ) ) - 0.5 ) 
      +         r   * ( ( float ) ( i4_max ( a, b ) ) + 0.5 );
//
//  Use rounding to convert R to an integer between A and B.
//
    value = r4_nint ( r );

    value = i4_max ( value, i4_min ( a, b ) );
    value = i4_min ( value, i4_max ( a, b ) );

    x[i] = value;
  }

  return x;
}
//****************************************************************************80

int i4vec_unique_count ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_UNIQUE_COUNT counts the unique elements in an unsorted I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    Because the array is unsorted, this algorithm is O(N^2).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the array to examine, which does NOT have to
//    be sorted.
//
//    Output, int I4VEC_UNIQUE_COUNT, the number of unique elements of A.
//
{
  int i;
  int j;
  int unique_num;

  unique_num = 0;

  for ( i = 0; i < n; i++ )
  {
    unique_num = unique_num + 1;

    for ( j = 0; j < i; j++ )
    {
      if ( a[i] == a[j] )
      {
        unique_num = unique_num - 1;
        break;
      }
    }
  }

  return unique_num;
}
//****************************************************************************80

int *i4vec_unique_index ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_UNIQUE_INDEX indexes the unique occurrence of values in an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    For element A(I) of the vector, UNIQUE_INDEX(I) is the uniqueness index
//    of A(I).  That is, if A_UNIQUE contains the unique elements of A, 
//    gathered in order, then 
//
//      A_UNIQUE ( UNIQUE_INDEX(I) ) = A(I)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, int A[N], the array.
//
//    Output, int I4VEC_UNIQUE_INDEX[N], the unique index.
//
{
  int i;
  int j;
  int *unique_index;
  int unique_num;

  unique_index = new int[n];

  for ( i = 0; i < n; i++ )
  {
    unique_index[i] = -1;
  }
  unique_num = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( unique_index[i] == -1 )
    {
      unique_index[i] = unique_num;
      for ( j = i + 1; j < n; j++ )
      {
        if ( a[i] == a[j] )
        {
          unique_index[j] = unique_num;
        }
      }
      unique_num = unique_num + 1;
    }
  }
  return unique_index;
}
//****************************************************************************80

int *i4vec_value_index ( int n, int a[], int value, int max_index, 
  int *n_index )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_VALUE_INDEX indexes I4VEC entries equal to a given value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Example:
//
//    Input:
//
//      N = 10
//      A = (  2, 3, 1, 3, 2, 4, 2, 3, 5, 3 )
//      X_VALUE = 3
//
//    Output:
//
//      N_INDEX = 4
//      VALUE_INDEX = ( 2, 4, 8, 10 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int A[N], the array to be indexed.
//
//    Input, int VALUE, a value to be searched for.
//
//    Input, int MAX_INDEX, the maximum number of indices to find.
//
//    Output, int *N_INDEX, the number of entries equal to VALUE.
//
//    Output, int I4VEC_VALUE_INDEX[MAX_INDEX], the indices of entries
//    equal to VALUE.
//
{
  int i;
  int *value_index;

  value_index = new int[max_index];

  *n_index = 0;

  for ( i = 1; i <= n; i++ )
  {
    if ( a[i-1] == value )
    {
      if ( max_index <= *n_index )
      {
        break;
      }
      value_index[*n_index] = i;
      *n_index = *n_index + 1;
    }
  }

  return value_index;
}
//****************************************************************************80

int i4vec_value_num ( int n, int a[], int value)

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_VALUE_NUM counts I4VEC entries equal to a given value.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int A[N], the array to be indexed.
//
//    Input, int VALUE, a value to be searched for.
//
//    Input, int I4VEC_VALUE_NUM, the number of times the value occurs.
//
{
  int i;
  int value_num;

  value_num = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] == value )
    {
      value_num = value_num + 1;
    }
  }

  return value_num;
}
//****************************************************************************80

double i4vec_variance ( int n, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_VARIANCE returns the variance of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int X[N], the vector whose variance is desired.
//
//    Output, double I4VEC_VARIANCE, the variance of the vector entries.
//
{
  int i;
  double mean;
  double variance;

  mean = i4vec_mean ( n, x );

  variance = 0.0;
  for ( i = 0; i < n; i++ )
  {
    variance = variance + ( ( double ) x[i] - mean ) * ( ( double ) x[i] - mean );
  }

  if ( 1 < n )
  {
    variance = variance / ( double ) ( n - 1 );
  }
  else
  {
    variance = 0.0;
  }

  return variance;
}
//****************************************************************************80

int i4vec_width ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_WIDTH returns the "width" of an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//    The width of an integer vector is simply the maximum of the widths of
//    its entries.
//
//    The width of a single integer is the number of characters 
//    necessary to print it.
//
//    The width of an integer vector can be useful when the vector is 
//    to be printed.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, int A[N], the vector.
//
//    Output, int I4VEC_WIDTH, the width of the vector.
//
{
  int i;
  int width;

  width = -1;

  for ( i = 0; i < n; i++ )
  {
    width = i4_max ( width, i4_width ( a[i] ) );
  }
  return width;
}
//****************************************************************************80

void i4vec_zero ( int n, int a[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ZERO zeroes an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, int A[N], a vector of zeroes.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0;
  }
  return;
}
//****************************************************************************80

int *i4vec_zero_new ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_ZERO_NEW creates and zeroes an I4VEC.
//
//  Discussion:
//
//    An I4VEC is a vector of I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, int I4VEC_ZERO_NEW[N], a vector of zeroes.
//
{
  int *a;
  int i;

  a = new int[n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0;
  }
  return a;
}
//****************************************************************************80

int i4vec2_compare ( int n, int a1[], int a2[], int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC2_COMPARE compares pairs of integers stored in two vectors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of data items.
//
//    Input, int A1[N], A2[N], contain the two components of each item.
//
//    Input, int I, J, the items to be compared.  These values will be
//    1-based indices for the arrays A1 and A2.
//
//    Output, int I4VEC2_COMPARE, the results of the comparison:
//    -1, item I < item J,
//     0, item I = item J,
//    +1, item J < item I.
//
{
  int isgn;

  isgn = 0;

  if ( a1[i-1] < a1[j-1] )
  {
    isgn = -1;
  }
  else if ( a1[i-1] == a1[j-1] )
  {
    if ( a2[i-1] < a2[j-1] )
    {
      isgn = -1;
    }
    else if ( a2[i-1] < a2[j-1] )
    {
      isgn = 0;
    }
    else if ( a2[j-1] < a2[i-1] )
    {
      isgn = +1;
    }
  }
  else if ( a1[j-1] < a1[i-1] )
  {
    isgn = +1;
  }

  return isgn;
}
//****************************************************************************80

void i4vec2_print ( int n, int a1[], int a2[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC2_PRINT prints an I4VEC2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the vectors.
//
//    Input, int A1[N], int A2[N], the vectors to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i <= n-1; i++ ) 
  {
    cout << setw(6) << i + 1 << "  " 
         << setw(8) << a1[i]    << "  " 
         << setw(8) << a2[i]    << "\n";
  }

  return;
}
//****************************************************************************80

void i4vec2_sort_a ( int n, int a1[], int a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC2_SORT_A ascending sorts an I4VEC2.
//
//  Discussion:
//
//    Each item to be sorted is a pair of integers (I,J), with the I
//    and J values stored in separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items of data.
//
//    Input/output, int A1[N], A2[N], the data to be sorted..
//
{
  int i;
  int indx;
  int isgn;
  int j;
  int temp;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      temp    = a1[i-1];
      a1[i-1] = a1[j-1];
      a1[j-1] = temp;

      temp    = a2[i-1];
      a2[i-1] = a2[j-1];
      a2[j-1] = temp;
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = i4vec2_compare ( n, a1, a2, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }
  return;
}
//****************************************************************************80

void i4vec2_sort_d ( int n, int a1[], int a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC2_SORT_D descending sorts an I4VEC2.
//
//  Discussion:
//
//    Each item to be sorted is a pair of integers (I,J), with the I
//    and J values stored in separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items of data.
//
//    Input/output, int A1[N], A2[N], the data to be sorted..
//
{
  int i;
  int indx;
  int isgn;
  int j;
  int temp;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      temp    = a1[i-1];
      a1[i-1] = a1[j-1];
      a1[j-1] = temp;

      temp    = a2[i-1];
      a2[i-1] = a2[j-1];
      a2[j-1] = temp;
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = -i4vec2_compare ( n, a1, a2, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

void i4vec2_sorted_unique ( int n, int a1[], int a2[], int *unique_num )

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC2_SORTED_UNIQUE keeps the unique elements in an I4VEC2.
//
//  Discussion:
//
//    Item I is stored as the pair A1(I), A2(I).
//
//    The items must have been sorted, or at least it must be the
//    case that equal items are stored in adjacent vector locations.
//
//    If the items were not sorted, then this routine will only
//    replace a string of equal values by a single representative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 July 2000
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items.
//
//    Input/output, int A1[N], A2[N].
//    On input, the array of N items.
//    On output, an array of UNIQUE_NUM unique items.
//
//    Output, int *UNIQUE_NUM, the number of unique items.
//
{
  int itest;

  *unique_num = 0;

  if ( n <= 0 )
  {
    return;
  }

  *unique_num = 1;

  for ( itest = 1; itest < n; itest++ )
  {
    if ( a1[itest] != a1[*unique_num-1] || 
         a2[itest] != a2[*unique_num-1] )
    {
      a1[*unique_num] = a1[itest];
      a2[*unique_num] = a2[itest];
      *unique_num = *unique_num + 1;
    }
  }

  return;
}
//****************************************************************************80

long long int i8_huge ( )

//****************************************************************************80
//
//  Purpose:
//
//    I8_HUGE returns a "huge" I8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, long long int I8_HUGE, a "huge" I8.
//
{
  long long int value;

  value = 9223372036854775807LL;

  return value;
}
//****************************************************************************80

long long int i8_max ( long long int i1, long long int i2 )

//****************************************************************************80
//
//  Purpose:
//
//    I8_MAX returns the maximum of two I8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, long long int I1, I2, two integers to be compared.
//
//    Output, long long int I8_MAX, the larger of I1 and I2.
//
{
  long long int value;

  if ( i2 < i1 ) 
  {
    value = i1;
  }
  else 
  {
    value = i2;
  }
  return value;
}
//****************************************************************************80

long long int i8_min ( long long int i1, long long int i2 )

//****************************************************************************80
//
//  Purpose:
//
//    I8_MIN returns the smaller of two I8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, long long int I1, I2, two integers to be compared.
//
//    Output, long long int I8_MIN, the smaller of I1 and I2.
//
{
  long long int value;

  if ( i1 < i2 ) 
  {
    value = i1;
  }
  else 
  {
    value = i2;
  }
  return value;
}
//****************************************************************************80

long long int i8_power ( long long int i, long long int j )

//****************************************************************************80
//
//  Purpose:
//
//    I8_POWER returns the value of I^J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, long long int I, J, the base and the power.  
//    J should be nonnegative.
//
//    Output, long long int I8_POWER, the value of I^J.
//
{
  long long int k;
  long long int value;

  if ( j < 0 )
  {
    if ( i == 1 )
    {
      value = 1;
    }
    else if ( i == 0 )
    {
      cout << "\n";
      cout << "I8_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J negative.\n";
      exit ( 1 );
    }
    else
    {
      value = 0;
    }
  }
  else if ( j == 0 )
  {
    if ( i == 0 )
    {
      cout << "\n";
      cout << "I8_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J = 0.\n";
      exit ( 1 );
    }
    else
    {
      value = 1;
    }
  }
  else if ( j == 1 )
  {
    value = i;
  }
  else
  {
    value = 1;
    for ( k = 1; k <= j; k++ )
    {
      value = value * i;
    }
  }
  return value;
}
//****************************************************************************80

long long int i8_reverse_bytes ( long long int x )

//****************************************************************************80
//
//  Purpose:
//
//    I8_REVERSE_BYTES reverses the bytes in an I8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, long long int X, a value whose bytes are to be reversed.
//
//    Output, long long int I8_REVERSE_BYTES, a value whose bytes are
//    in reverse order from those in X.
//
{
  char c;
  union
  {
    long long int ylongint;
    char ychar[8];
  } y;

  y.ylongint = x;
  
  c = y.ychar[0];
  y.ychar[0] = y.ychar[7];
  y.ychar[7] = c;

  c = y.ychar[1];
  y.ychar[1] = y.ychar[6];
  y.ychar[6] = c;

  c = y.ychar[2];
  y.ychar[2] = y.ychar[5];
  y.ychar[5] = c;

  c = y.ychar[3];
  y.ychar[3] = y.ychar[4];
  y.ychar[4] = c;

  return ( y.ylongint );
}
//****************************************************************************80

unsigned long long int i8_xor ( unsigned long long int i, 
  unsigned long long int j )

//****************************************************************************80
//
//  Purpose:
//
//    I8_XOR calculates the exclusive OR of two I8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//   John Burkardt
//
//  Parameters:
//
//    Input, unsigned long long int I, J, two values whose exclusive OR 
//    is needed.
//
//    Output, unsigned long long int I8_XOR, the exclusive OR of I and J.
//
{
  unsigned long long int i2;
  unsigned long long int j2;
  unsigned long long int k;
  unsigned long long int l;

  k = 0;
  l = 1;

  while ( i != 0 || j != 0 )
  {
    i2 = i / 2;
    j2 = j / 2;

    if ( 
      ( ( i == 2 * i2 ) && ( j != 2 * j2 ) ) ||
      ( ( i != 2 * i2 ) && ( j == 2 * j2 ) ) )
    {
      k = k + l;
    }

    i = i2;
    j = j2;
    l = 2 * l;
  }

  return k;
}
//****************************************************************************80

void ij_next ( int *i, int *j, int n )

//****************************************************************************80
//
//  Purpose:
//
//    IJ_NEXT returns the next matrix index.
//
//  Discussion:
//
//    For N = 3, the sequence of indices returned is:
//
//      (1,1), (1,2), (1,3), (2,1), (2,2), (2,3), (3,1), (3,2), (3,3), (0,0).
//
//    Note that once the value (N,N) is returned, the next value returned
//    will be (0,0).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *I, *J.  On input, the current pair of indices.
//    On output, the next pair of indices.  If either index is illegal on
//    input, the output value of (I,J) will be (1,1).
//
//    Input, int N, the maximum value for I and J.
//
{
  if ( n < 1 )
  {
    *i = 0;
    *j = 0;
    return;
  }

  if ( *i < 1 || n < *i || *j < 1 || n < *j )
  {
    *i = 1;
    *j = 1;
    return;
  }

  if ( *j < n )
  {
    *j = *j + 1;
  }
  else if ( *i < n )
  {
    *i = *i + 1;
    *j = 1;
  }
  else
  {
    *i = 0;
    *j = 0;
  }

  return;
}
//****************************************************************************80

void ij_next_gt ( int *i, int *j, int n )

//****************************************************************************80
//
//  Purpose:
//
//    IJ_NEXT_GT returns the next matrix index, with the constraint that I < J.
//
//  Discussion:
//
//    For N = 5, the sequence of indices returned is:
//
//      (1,2), (1,3), (1,4), (1,5), (2,3), (2,4), (2,5), (3,4), (3,5), (4,5).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *I, *J.  On input, the current pair of indices.
//    On output, the next pair of indices.  If either index is illegal on
//    input, the output value of (I,J) will be (1,2).
//
//    Input, int N, the maximum value for I and J.
//    A value of N less than 2 is nonsense.
//
{
  if ( n < 2 )
  {
    *i = 0;
    *j = 0;
    return;
  }

  if ( *i < 1 || n < *i || *j < 1 || n < *j || *j <= *i )
  {
    *i = 1;
    *j = 2;
    return;
  }

  if ( *j < n )
  {
    *j = *j + 1;
  }
  else if ( *i < n - 1 )
  {
    *i = *i + 1;
    *j = *i + 1;
  }
  else
  {
    *i = 0;
    *j = 0;
  }

  return;
}
//****************************************************************************80

void index_box2_next_2d ( int n1, int n2, int ic, int jc, int *i, int *j, 
  int *more )

//****************************************************************************80
//
//  Purpose:
//
//    INDEX_BOX2_NEXT_2D produces indices on the surface of a box in 2D.
//
//  Discussion:
//
//    The box has center at (IC,JC), and has half-widths N1 and N2.
//    The indices are exactly those which are between (IC-N1,JC-N2) and
//    (IC+N1,JC+N2) with the property that at least one of I and J
//    is an "extreme" value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, the half-widths of the box, that is, the
//    maximum distance allowed between (IC,JC) and (I,J).
//
//    Input, int IC, JC, the central cell of the box.
//
//    Input/output, int *I, *J.  On input, the previous index set.
//    On output, the next index set.  On the first call, MORE should
//    be set to FALSE, and the input values of I and J are ignored.
//
//    Input/output, bool *MORE.
//    On the first call for a given box, the user should set MORE to FALSE.
//    On return, the routine sets MORE to TRUE.
//    When there are no more indices, the routine sets MORE to FALSE.
//
{
  if ( !(*more) )
  {
    *more = true;
    *i = ic - n1;
    *j = jc - n2;
    return;
  }

  if ( *i == ic + n1 && 
       *j == jc + n2 )
  {
    *more = false;
    return;
  }
//
//  Increment J.
//
  *j = *j + 1;
//
//  Check J.
//
  if ( jc + n2 < *j )
  {
    *j = jc - n2;
    *i = *i + 1;
  }
  else if ( *j < jc + n2 && ( *i == ic - n1 || *i == ic + n1 ) )
  {
    return;
  }
  else
  {
    *j = jc + n2;
    return;
  }

  return;
}
//****************************************************************************80

void index_box2_next_3d ( int n1, int n2, int n3, int ic, int jc, int kc, 
  int *i, int *j, int *k, bool *more )

//****************************************************************************80
//
//  Purpose:
//
//    INDEX_BOX2_NEXT_3D produces indices on the surface of a box in 3D.
//
//  Discussion:
//
//    The box has a central cell of (IC,JC,KC), with a half widths of
//    (N1,N2,N3).  The indices are exactly those between (IC-N1,JC-N2,KC-N3) and
//    (IC+N1,JC+N2,KC+N3) with the property that at least one of I, J, and K
//    is an "extreme" value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, N3, the "half widths" of the box, that is, the
//    maximum distances from the central cell allowed for I, J and K.
//
//    Input, int IC, JC, KC, the central cell of the box.
//
//    Input/output, int *I, *J, *K.  On input, the previous index set.
//    On output, the next index set.  On the first call, MORE should
//    be set to FALSE, and the input values of I, J, and K are ignored.
//
//    Input/output, bool *MORE.
//    On the first call for a given box, the user should set MORE to FALSE.
//    On return, the routine sets MORE to TRUE.
//    When there are no more indices, the routine sets MORE to FALSE.
//
{
  if ( !(*more) )
  {
    *more = true;
    *i = ic - n1;
    *j = jc - n2;
    *k = kc - n3;
    return;
  }

  if ( *i == ic + n1 && 
       *j == jc + n2 && 
       *k == kc + n3 )
  {
    *more = false;
    return;
  }
//
//  Increment K.
//
  *k = *k + 1;
//
//  Check K.
//
  if ( kc + n3 < *k )
  {
    *k = kc - n3;
    *j = *j + 1;
  }
  else if ( *k < kc + n3 &&
    ( *i == ic - n1 || 
      *i == ic + n1 || 
      *j == jc - n2 || 
      *j == jc + n2 ) )
  {
    return;
  }
  else
  {
    *k = kc + n3;
    return;
  }
//
//  Check J.
//
  if ( jc + n2 < *j )
  {
    *j = jc - n2;
    *i = *i + 1;
  }
  else if ( *j < jc + n2 &&
    ( *i == ic - n1 || 
      *i == ic + n1 || 
      *k == kc - n3 || 
      *k == kc + n3 ) )
  {
    return;
  }
  else
  {
    *j = jc + n2;
    return;
  }

  return;
}
//****************************************************************************80

int isbn_check ( char *isbn )

//****************************************************************************80
//
//  Purpose:
//
//    ISBN_CHECK checks an ISBN code.
//
//  Discussion:
//
//    ISBN stands for International Standard Book Number.  A unique ISBN
//    is assigned to each new book.  The ISBN includes 10 digits.  There is
//    an initial digit, then a dash, then a set of digits which are a
//    code for the publisher, another digit, and then the check digit:
//
//      initial-publisher-book-check
//
//    The number of digits used for the publisher and book codes can vary,
//    but the check digit is always one digit, and the total number of
//    digits is always 10.
//
//    The check digit is interesting, because it is a way of trying to
//    make sure that an ISBN has not been incorrectly copied.  Specifically,
//    if the ISBN is correct, then its ten digits will satisfy
//
//       10 * A + 9 * B + 8 * C + 7 * D + 6 * E
//      + 5 * F * 4 * G * 3 * H + 2 * I +     J  = 0 mod 11.
//
//    Here, we've taken 'A' to represent the first digit and 'J' the
//    last (which is the check digit).  In order for the code to work,
//    the value of J must be allowed to be anything from 0 to 10.  In
//    cases where J works out to be 10, the special digit 'X' is used.
//    An 'X' digit can only occur in this last check-digit position
//    on an ISBN.
//
//  Example:
//
//    0-8493-9640-9
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Book Industry Study Group,
//    The Evolution in Product Identification:
//    Sunrise 2005 and the ISBN-13,
//    http://www.bisg.org/docs/The_Evolution_in_Product_ID.pdf
//
//  Parameters:
//
//    Input, char *ISBN, an ISBN code.
//
//    Output, int ISBN_CHECK, the value of the ISBN check sum.
//    If CHECK is zero, the ISBN code is legitimate.
//    If CHECK is -1, then the ISBN code is not legitimate because it does
//    not contain exactly 10 digits.  If CHECK is between 1 and 10, then
//    the ISBN code has the right number of digits, but at least one of
//    the digits is incorrect.
//
{
  char c;
  int check;
  int digit[10];
  int i;
  int lenc;
  int num_digit;
//
//  Determine how many digits have been supplied.
//
  lenc = s_len_trim ( isbn );

  i = 0;
  num_digit = 0;

  for ( ; ; )
  {
    i = i + 1;

    if ( lenc < i )
    {
      break;
    }

    c = isbn[i-1];

    if ( ch_is_digit ( c ) )
    {
      digit[num_digit] = isbn_to_i4 ( c );
      num_digit = num_digit + 1;
    }
    else if ( ( num_digit == 9 && isbn[i-1] == 'X' ) ||
              ( num_digit == 9 && isbn[i-1] == 'x' ) )
    {
      digit[num_digit] = isbn_to_i4 ( c );
      num_digit = num_digit + 1;
    }

    if ( 10 <= num_digit )
    {
      break;
    }
  }
//
//  If we didn't get exactly 10 digits, return with an error.
//
  if ( num_digit != 10 )
  {
    check = -1;
    return check;
  }
//
//  Compute the checksum.
//
  check = 0;
  for ( i = 0; i < 10; i++ )
  {
    check = check + ( 11 - i ) * digit[i];
  }

  check = ( check % 11 );

  return check;
}
//****************************************************************************80

void isbn_fill ( char *isbn )

//****************************************************************************80
//
//  Purpose:
//
//    ISBN_FILL fills in a missing digit in an ISBN code.
//
//  Example:
//
//    Input:
//
//      0-8493-9?40-9
//
//    Output:
//
//      0-8493-9640-9
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Book Industry Study Group,
//    The Evolution in Product Identification:
//    Sunrise 2005 and the ISBN-13,
//    http://www.bisg.org/docs/The_Evolution_in_Product_ID.pdf
//
//  Parameters:
//
//    Input/output, character *ISBN, a partial ISBN code.  On input,
//    a single digit has been replaced by the character '?', signifying
//    that that digit is missing.  The routine replaces the question
//    mark by the correct digit.
//
{
  char c;
  int check;
  int digit[10];
  int digit_pos;
  int i;
  int isbn_pos;
  int j;
  int k;
  int lenc;
  int num_digit;

  lenc = s_len_trim ( isbn );

  i = 0;
  isbn_pos = -1;
  digit_pos = -1;
  num_digit = 0;

  for ( ; ; )
  {
    i = i + 1;

    if ( lenc < i )
    {
      break;
    }

    c = isbn[i-1];

    if ( ch_is_digit ( c ) )
    {
      num_digit = num_digit + 1;
      digit[num_digit-1] = isbn_to_i4 ( c );
    }
    else if ( ( num_digit == 9 && isbn[i-1] == 'X' ) || 
              ( num_digit == 9 && isbn[i-1] == 'x' ) )
    {
      num_digit = num_digit + 1;
      digit[num_digit-1] = isbn_to_i4 ( c );
    }
    else if ( c == '?' )
    {
      if ( isbn_pos == -1 )
      {
        num_digit = num_digit + 1;
        digit[num_digit-1] = 0;
        digit_pos = num_digit;
        isbn_pos = i;
      }
      else
      {
        cout << "\n";
        cout << "ISBN_FILL - Fatal error!\n";
        cout << "  Only one question mark is allowed.\n";
        exit ( 1 );
      }
    }

    if ( 10 <= num_digit )
    {
      break;
    }
  }

  if ( num_digit != 10 )
  {
    cout << "\n";
    cout << "ISBN_FILL - Fatal error!\n";
    cout << "  The input ISBN code did not have 10 digits.\n";
    exit ( 1 );
  }

  if ( isbn_pos == -1 )
  {
    cout << "\n";
    cout << "ISBN_FILL - Fatal error!\n";
    cout << "  A question mark is required.\n";
    exit ( 1 );
  }

  check = 0;
  for ( i = 1; i <= 10; i++ )
  {
    check = check + ( 11 - i ) * digit[i-1];
  }

  check = ( check % 11 );

  if ( check == 0 )
  {
    k = 0;
  }
//
//  Need to solve the modular equation:
//
//    A * X = B mod C
//
//  Below is a stupid way.  One day I will come back and fix this up.
//
  else
  {
    for ( i = 1; i <= 10; i++ )
    {
      j = ( 11 - digit_pos ) * i + check;
      if ( ( j % 11 ) == 0 )
      {
        k = i;
      }
    }
  }

  isbn[isbn_pos-1] = i4_to_isbn ( k );

  return;
}
//****************************************************************************80

int isbn_to_i4 ( char c )

//****************************************************************************80
//
//  Purpose:
//
//    ISBN_TO_I4 converts an ISBN character into an I4.
//
//  Discussion:
//
//    The characters '0' through '9' stand for themselves, but
//    the character 'X' or 'x' stands for 10.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Book Industry Study Group,
//    The Evolution in Product Identification:
//    Sunrise 2005 and the ISBN-13,
//    http://www.bisg.org/docs/The_Evolution_in_Product_ID.pdf
//
//  Parameters:
//
//    Input, char C, the ISBN character code to be converted.
//
//    Output, int ISBN_TO_I4, the numeric value of the character
//    code, between 0 and 10.  This value is returned as -1 if C is
//    not a valid character code.
//
{
  int value;

  if ( '0' <= c && c <= '9' )
  {
    value = c - '0';
  }
  else if ( c == 'X' || c == 'x' )
  {
    value = 10;
  }
  else
  {
    value = -1;
  }

  return value;
}
//****************************************************************************80

int iset2_compare ( int x1, int y1, int x2, int y2 )

//****************************************************************************80
//
//  Purpose:
//
//    ISET2_COMPARE compares two I2 sets.
//
//  Discussion:
//
//    The I2 set (X1,Y1) < (X2,Y2) if
//
//      min ( X1, Y1 ) < min ( X2, Y2 ) or
//      min ( X1, Y1 ) = min ( X2, Y2 ) and max ( X1, Y1 ) < max ( X2, Y2 )
//
//    The I2 set (X1,Y1) = (X2,Y2) if
//
//      min ( X1, Y1 ) = min ( X2, Y2 ) and max ( X1, Y1 ) = max ( X2, Y2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X1, Y1, the first I2 set.
//
//    Input, int X2, Y2, the second I2 set.
//
//    Output, int ISET2_COMPARE: 
//    -1, (X1,Y1) < (X2,Y2).
//     0, (X1,Y1) = (X2,Y2).
//    +1, (X1,Y1) > (X2,Y2).
//
{
  int a1;
  int a2;
  int b1;
  int b2;
  int value;

  a1 = i4_min ( x1, y1 );
  b1 = i4_max ( x1, y1 );

  a2 = i4_min ( x2, y2 );
  b2 = i4_max ( x2, y2 );

  if ( a1 < a2 )
  {
    value = -1;
  }
  else if ( a2 < a1 )
  {
    value = +1;
  }
  else if ( b1 < b2 )
  {
    value = -1;
  }
  else if ( b2 < b1 )
  {
    value = +1;
  }
  else
  {
    value = 0;
  }
  return value;
}
//****************************************************************************80

int lcm_12n ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    LCM_12N computes the least common multiple of the integers 1 through N.
//
//  Example:
//
//    N    LCM_12N
//
//    1          1
//    2          2
//    3          3
//    4         12
//    5         60
//    6         60
//    7        420
//    8        840
//    9       2520
//   10       2520
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the value of N.
//
//    Output, int LCM_12N, the least common multiple of the integers 1 to N.
//
{
  int i;
  int imult;
  int j;
  int value;

  value = 1;

  for ( i = 2; i <= n; i++ )
  {
    imult = i;
    for ( j = 1; j < i; j++ )
    {
      if ( ( imult % ( i - j ) ) == 0 )
      {
        imult = imult / ( i - j );
      }
    }
    value = value * imult;
  }

  return value;
}
//****************************************************************************80

void lvec_print ( int n, bool a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    LVEC_PRINT prints a logical vector.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 April 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, bool A[N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( s_len_trim ( title ) != 0 )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i < n; i++ ) 
  {
    cout << setw(6) << i << "  " 
         << setw(1) << a[i]  << "\n";
  }

  return;
}
//****************************************************************************80

int pause_input ( )

//****************************************************************************80
//
//  Purpose:
//
//    PAUSE_INPUT waits until an input character is entered.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
// 
//    13 January 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, int PAUSE_INPUT, the character read from STDIN.
//
{
  cout << "Press RETURN to continue.\n";
  return ( getc ( stdin ) );
}
//****************************************************************************80

void pause_seconds ( int seconds )

//****************************************************************************80
//
//  Purpose:
//
//    PAUSE_SECONDS waits a specified number of seconds.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
// 
//    13 January 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int SECONDS, the number of seconds to pause.
//
{
  time_t t1;
  time_t t2;

  t1 = time ( NULL );
  t2 = t1;
  while ( t2 - t1 < seconds )
  {
    t2 = time ( NULL );
  }

  return;
}
//****************************************************************************80

bool perm_check ( int n, int p[], int base )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_CHECK checks that a vector represents a permutation.
//
//  Discussion:
//
//    The routine verifies that each of the integers from BASE to
//    to BASE+N-1 occurs among the N entries of the permutation.
//
//    Set the input quantity BASE to 0, if P is a 0-based permutation,
//    or to 1 if P is a 1-based permutation.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries.
//
//    Input, int P[N], the array to check.
//
//    Input, int BASE, the index base.
//
//    Output, bool PERM_CHECK, is TRUE if the permutation is OK.
//
{
  bool found;
  int i;
  int seek;

  for ( seek = base; seek < base + n; seek++ )
  {
    found = false;

    for ( i = 0; i < n; i++ )
    {
      if ( p[i] == seek )
      {
        found = true;
        break;
      }
    }

    if ( !found )
    {
      return false;
    }

  }

  return true;
}
//****************************************************************************80

void perm_cycle ( int n, int p[], int *isgn, int *ncycle, int iopt )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_CYCLE analyzes a permutation.
//
//  Discussion:
//
//    The routine will count cycles, find the sign of a permutation,
//    and tag a permutation.
//
//  Example:
//
//    Input:
//
//      N = 9
//      IOPT = 1
//      P = 2, 3, 9, 6, 7, 8, 5, 4, 1
//
//    Output:
//
//      NCYCLE = 3
//      ISGN = +1
//      P = -2, 3, 9, -6, -7, 8, 5, 4, 1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 May 2003
//
//  Author:
//
//    FORTRAN77 original version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of objects being permuted.
//
//    Input/output, int P[N].  On input, P describes a
//    permutation, in the sense that entry I is to be moved to P[I].
//    If IOPT = 0, then P will not be changed by this routine.
//    If IOPT = 1, then on output, P will be "tagged".  That is,
//    one element of every cycle in P will be negated.  In this way,
//    a user can traverse a cycle by starting at any entry I1 of P
//    which is negative, moving to I2 = ABS(P[I1]), then to
//    P[I2], and so on, until returning to I1.
//
//    Output, int *ISGN, the "sign" of the permutation, which is
//    +1 if the permutation is even, -1 if odd.  Every permutation
//    may be produced by a certain number of pairwise switches.
//    If the number of switches is even, the permutation itself is
//    called even.
//
//    Output, int *NCYCLE, the number of cycles in the permutation.
//
//    Input, int IOPT, requests tagging.
//    0, the permutation will not be tagged.
//    1, the permutation will be tagged.
//
{
  int base = 1;
  bool error;
  int i;
  int i1;
  int i2;
  int is;

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "PERM_CYCLE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }

  is = 1;
  *ncycle = n;

  for ( i = 1; i <= n; i++ )
  {
    i1 = p[i-1];

    while ( i < i1 )
    {
      *ncycle = *ncycle - 1;
      i2 = p[i1-1];
      p[i1-1] = -i2;
      i1 = i2;
    }

    if ( iopt != 0 )
    {
      is = - i4_sign ( p[i-1] );
    }
    p[i-1] = abs ( p[i-1] ) * i4_sign ( is );
  }

  *isgn = 1 - 2 * ( ( n - *ncycle ) % 2 );

  return;
}
//****************************************************************************80

int *perm_free ( int npart, int ipart[], int nfree )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_FREE reports the number of unused items in a partial permutation.
//
//  Discussion:
//
//    It is assumed that the N objects being permuted are the integers
//    from 1 to N, and that IPART contains a "partial" permutation, that
//    is, the NPART entries of IPART represent the beginning of a
//    permutation of all N items.
//
//    The routine returns in IFREE the items that have not been used yet.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPART, the number of entries in IPART.  NPART may be 0.
//
//    Input, int IPART(NPART), the partial permutation, which should
//    contain, at most once, some of the integers between 1 and
//    NPART+NFREE.
//
//    Input, int NFREE, the number of integers that have not been
//    used in IPART.  This is simply N - NPART.  NFREE may be zero.
//
//    Output, int PERM_FREE[NFREE], the integers between 1 and NPART+NFREE
//    that were not used in IPART.
//
{
  int i;
  int *ifree;
  int j;
  int k;
  int match;
  int n;

  n = npart + nfree;

  if ( npart < 0 )
  {
    cout << "\n";
    cout << "PERM_FREE - Fatal error!\n";
    cout << "  NPART < 0.\n";
    cout << "  NPART = " << npart << "\n";
    exit ( 1 );
  }
  else if ( npart == 0 )
  {
    ifree = i4vec_indicator ( n );
    return ifree;
  }
  else if ( nfree < 0 )
  {
    cout << "\n";
    cout << "PERM_FREE - Fatal error!\n";
    cout << "  NFREE < 0.\n";
    cout << "  NFREE =  << nfree << \n";
    exit ( 1 );
  }
  else if ( nfree == 0 )
  {
    return NULL;
  }
  else
  {
    ifree = new int[nfree];

    k = 0;

    for ( i = 1; i <= n; i++ )
    {
      match = 0;

      for ( j = 1; j <= npart; j++ )
      {
        if ( ipart[j-1] == i )
        {
          match = j;
          break;
        }
      }

      if ( match == 0 )
      {
        k = k + 1;
        if ( nfree < k )
        {
          cout << "\n";
          cout << "PERM_FREE - Fatal error!\n";
          cout << "  The partial permutation is illegal.\n";
          cout << "  It should contain, at most once, some of\n";
          cout << "  the integers between 1 and N = " << n << "\n";
          cout << "  The number of integers that have not\n";
          cout << "  been used is at least K = " << k << "\n";
          cout << "  This should be exactly NFREE = " << nfree << "\n";
          i4vec_print ( npart, ipart, "  The partial permutation:" );
          exit ( 1 );
        }
        ifree[k-1] = i;
      }
    }
  }

  return ifree;
}
//****************************************************************************80

void perm_inverse ( int n, int p[] )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_INVERSE inverts a permutation "in place".
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects being permuted.
//
//    Input/output, int P[N], the permutation, in standard index form.
//    On output, P describes the inverse permutation
//
{
  int base = 1;
  int i;
  int i0;
  int i1;
  int i2;
  int is;

  if ( n <= 0 )
  {
    cout << "\n";
    cout << "PERM_INVERSE - Fatal error!\n";
    cout << "  Input value of N = " << n << "\n";
    exit ( 1 );
  }

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "PERM_INVERSE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }

  is = 1;

  for ( i = 1; i <= n; i++ )
  {
    i1 = p[i-1];

    while ( i < i1 )
    {
      i2 = p[i1-1];
      p[i1-1] = -i2;
      i1 = i2;
    }

    is = - i4_sign ( p[i-1] );
    p[i-1] = i4_sign ( is ) * abs ( p[i-1] );
  }

  for ( i = 1; i <= n; i++ )
  {
    i1 = - p[i-1];

    if ( 0 <= i1 )
    {
      i0 = i;

      for ( ; ; )
      {
        i2 = p[i1-1];
        p[i1-1] = i0;

        if ( i2 < 0 )
        {
          break;
        }
        i0 = i1;
        i1 = i2;
      }
    }
  }
  return;
}
//****************************************************************************80

void perm_print ( int n, int p[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_PRINT prints a permutation.
//
//  Example:
//
//    Input:
//
//      P = 7 2 4 1 5 3 6
//
//    Printed output:
//
//      "This is the permutation:"
//
//      1 2 3 4 5 6 7
//      7 2 4 1 5 3 6
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects permuted.
//
//    Input, int P[N], the permutation, in standard index form.
//
//    Input, char *TITLE, an optional title.
//    If no title is supplied, then only the permutation is printed.
//
{
  int i;
  int ihi;
  int ilo;
  int inc = 20;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";

    for ( ilo = 0; ilo < n; ilo = ilo + inc )
    {
      ihi = ilo + inc;
      if ( n < ihi ) 
      {
        ihi = n;
      }
      cout << "\n";
      for ( i = ilo; i < ihi; i++ )
      {
        cout << setw(4) << i+1;
      }
      cout << "\n";
      for ( i = ilo; i < ihi; i++ )
      {
        cout << setw(4) << p[i];
      }
      cout << "\n";
    }
  }
  else
  {
    for ( ilo = 0; ilo < n; ilo = ilo + inc )
    {
      ihi = ilo + inc;
      if ( n < ihi ) 
      {
        ihi = n;
      }
      for ( i = ilo; i < ihi; i++ )
      {
        cout << setw(4) << p[i];
      }
      cout << "\n";
    }
  }

  return;
}
//****************************************************************************80

int *perm_uniform ( int n, int base, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    PERM_UNIFORM selects a random permutation of N objects.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of objects to be permuted.
//
//    Input, int BASE, is 0 for a 0-based permutation and 1 for 
//    a 1-based permutation.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, int PERM_UNIFORM[N], a permutation of (BASE, BASE+1, ..., BASE+N-1).
//
{
  int i;
  int j;
  int k;
  int *p;

  p = new int[n];
 
  for ( i = 0; i < n; i++ )
  {
    p[i] = i + base;
  }

  for ( i = 0; i < n; i++ )
  {
    j = i4_uniform ( i, n - 1, seed );
    k    = p[i];
    p[i] = p[j];
    p[j] = k;
  }
 
  return p;
}
//****************************************************************************80

double pounds_to_kilograms ( double lb )

//****************************************************************************80
//
//  Purpose:
//
//    POUNDS_TO_KILOGRAMS converts a measurement in pounds to kilograms.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double LB, the weight in pounds.
//
//    Output, double POUNDS_TO_KILOGRAMS, the corresponding weight in kilograms.
//
{
  double value;

  value = 0.4535924 * lb;

  return value;
}
//****************************************************************************80

int prime ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    PRIME returns any of the first PRIME_MAX prime numbers.
//
//  Discussion:
//
//    PRIME_MAX is 1600, and the largest prime stored is 13499.
//
//    Thanks to Bart Vandewoestyne for pointing out a typo, 18 February 2005.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 February 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Daniel Zwillinger,
//    CRC Standard Mathematical Tables and Formulae,
//    30th Edition,
//    CRC Press, 1996, pages 95-98.
//
//  Parameters:
//
//    Input, int N, the index of the desired prime number.
//    In general, is should be true that 0 <= N <= PRIME_MAX.
//    N = -1 returns PRIME_MAX, the index of the largest prime available.
//    N = 0 is legal, returning PRIME = 1.
//
//    Output, int PRIME, the N-th prime.  If N is out of range, PRIME
//    is returned as -1.
//
{
# define PRIME_MAX 1600

  int npvec[PRIME_MAX] = {
        2,    3,    5,    7,   11,   13,   17,   19,   23,   29,
       31,   37,   41,   43,   47,   53,   59,   61,   67,   71,
       73,   79,   83,   89,   97,  101,  103,  107,  109,  113,
      127,  131,  137,  139,  149,  151,  157,  163,  167,  173,
      179,  181,  191,  193,  197,  199,  211,  223,  227,  229,
      233,  239,  241,  251,  257,  263,  269,  271,  277,  281,
      283,  293,  307,  311,  313,  317,  331,  337,  347,  349,
      353,  359,  367,  373,  379,  383,  389,  397,  401,  409,
      419,  421,  431,  433,  439,  443,  449,  457,  461,  463,
      467,  479,  487,  491,  499,  503,  509,  521,  523,  541,
      547,  557,  563,  569,  571,  577,  587,  593,  599,  601,
      607,  613,  617,  619,  631,  641,  643,  647,  653,  659,
      661,  673,  677,  683,  691,  701,  709,  719,  727,  733,
      739,  743,  751,  757,  761,  769,  773,  787,  797,  809,
      811,  821,  823,  827,  829,  839,  853,  857,  859,  863,
      877,  881,  883,  887,  907,  911,  919,  929,  937,  941,
      947,  953,  967,  971,  977,  983,  991,  997, 1009, 1013,
     1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
     1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
     1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
     1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 
     1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 
     1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 
     1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 
     1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 
     1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 
     1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 
     1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 
     1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
     1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
     1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053,
     2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
     2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213,
     2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
     2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 
     2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 
     2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 
     2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 
     2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 
     2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
     2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 
     2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 
     2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 
     3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 
     3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 
     3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 
     3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 
     3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 
     3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 
     3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571,
     3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643,
     3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727,
     3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821,
     3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907,
     3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
     4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057,
     4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139,
     4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231,
     4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297,
     4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
     4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 
     4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 
     4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 
     4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 
     4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 
     4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 
     4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 
     5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 
     5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 
     5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279,
     5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387,
     5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443,
     5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521,
     5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639,
     5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693,
     5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,
     5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857,
     5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939,
     5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053,
     6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,
     6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221,
     6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301,
     6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 
     6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 
     6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 
     6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 
     6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 
     6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 
     6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 
     6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
     7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 
     7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 
     7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 
     7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 
     7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 
     7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 
     7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 
     7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723, 
     7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 
     7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919,
     7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017,
     8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111,
     8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219,
     8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291,
     8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387,
     8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501,
     8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597,
     8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677,
     8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741, 
     8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831,
     8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929,
     8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007, 9011,
     9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109,
     9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199,
     9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283,
     9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377,
     9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439,
     9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533,
     9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631,
     9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733,
     9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811,
     9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887,
     9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973,10007,
    10009,10037,10039,10061,10067,10069,10079,10091,10093,10099,
    10103,10111,10133,10139,10141,10151,10159,10163,10169,10177,
    10181,10193,10211,10223,10243,10247,10253,10259,10267,10271,
    10273,10289,10301,10303,10313,10321,10331,10333,10337,10343,
    10357,10369,10391,10399,10427,10429,10433,10453,10457,10459,
    10463,10477,10487,10499,10501,10513,10529,10531,10559,10567,
    10589,10597,10601,10607,10613,10627,10631,10639,10651,10657,
    10663,10667,10687,10691,10709,10711,10723,10729,10733,10739,
    10753,10771,10781,10789,10799,10831,10837,10847,10853,10859,
    10861,10867,10883,10889,10891,10903,10909,10937,10939,10949,
    10957,10973,10979,10987,10993,11003,11027,11047,11057,11059,
    11069,11071,11083,11087,11093,11113,11117,11119,11131,11149,
    11159,11161,11171,11173,11177,11197,11213,11239,11243,11251,
    11257,11261,11273,11279,11287,11299,11311,11317,11321,11329,
    11351,11353,11369,11383,11393,11399,11411,11423,11437,11443,
    11447,11467,11471,11483,11489,11491,11497,11503,11519,11527,
    11549,11551,11579,11587,11593,11597,11617,11621,11633,11657,
    11677,11681,11689,11699,11701,11717,11719,11731,11743,11777,
    11779,11783,11789,11801,11807,11813,11821,11827,11831,11833,
    11839,11863,11867,11887,11897,11903,11909,11923,11927,11933,
    11939,11941,11953,11959,11969,11971,11981,11987,12007,12011,
    12037,12041,12043,12049,12071,12073,12097,12101,12107,12109,
    12113,12119,12143,12149,12157,12161,12163,12197,12203,12211,
    12227,12239,12241,12251,12253,12263,12269,12277,12281,12289,
    12301,12323,12329,12343,12347,12373,12377,12379,12391,12401,
    12409,12413,12421,12433,12437,12451,12457,12473,12479,12487,
    12491,12497,12503,12511,12517,12527,12539,12541,12547,12553,
    12569,12577,12583,12589,12601,12611,12613,12619,12637,12641, 
    12647,12653,12659,12671,12689,12697,12703,12713,12721,12739, 
    12743,12757,12763,12781,12791,12799,12809,12821,12823,12829, 
    12841,12853,12889,12893,12899,12907,12911,12917,12919,12923, 
    12941,12953,12959,12967,12973,12979,12983,13001,13003,13007, 
    13009,13033,13037,13043,13049,13063,13093,13099,13103,13109, 
    13121,13127,13147,13151,13159,13163,13171,13177,13183,13187, 
    13217,13219,13229,13241,13249,13259,13267,13291,13297,13309, 
    13313,13327,13331,13337,13339,13367,13381,13397,13399,13411, 
    13417,13421,13441,13451,13457,13463,13469,13477,13487,13499 };

  if ( n == -1 )
  {
    return PRIME_MAX;
  }
  else if ( n == 0 )
  {
    return 1;
  }
  else if ( n <= PRIME_MAX )
  {
    return npvec[n-1];
  }
  else
  {
    cout << "\n";
    cout << "PRIME - Fatal error!\n";
    cout << "  Unexpected input value of n = " << n << "\n";
    exit ( 1 );
  }

  return 0;
# undef PRIME_MAX
}
//****************************************************************************80

int prime_ge ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    PRIME_GE returns the smallest prime greater than or equal to N.
//
//  Example:
//
//    N     PRIME_GE
//
//    -10    2
//      1    2
//      2    2
//      3    3
//      4    5
//      5    5
//      6    7
//      7    7
//      8   11
//      9   11
//     10   11
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number to be bounded.
//
//    Output, int PRIME_GE, the smallest prime number that is greater
//    than or equal to N.  However, if N is larger than the
//    largest prime stored, then PRIME_GE is returned as -1.
//
{
  int i_hi;
  int i_lo;
  int i_mid;
  int p;
  int p_hi;
  int p_lo;
  int p_mid;

  if ( n <= 2 )
  {
    p = 2;
  }
  else
  {
    i_lo = 1;
    p_lo = prime(i_lo);
    i_hi = prime(-1);
    p_hi = prime(i_hi);

    if ( p_hi < n )
    {
      p = - p_hi;
    }
    else
    {
      for ( ; ; )
      {
        if ( i_lo + 1 == i_hi )
        {
          p = p_hi;
          break;
        }

        i_mid = ( i_lo + i_hi ) / 2;
        p_mid = prime(i_mid);

        if ( p_mid < n )
        {
          i_lo = i_mid;
          p_lo = p_mid;
        }
        else if ( n <= p_mid )
        {
          i_hi = i_mid;
          p_hi = p_mid;
        }
      }
    }
  }

  return p;
}
//****************************************************************************80

float r4_abs ( float x )

//****************************************************************************80
//
//  Purpose:
//
//    R4_ABS returns the absolute value of an R4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 December 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, the quantity whose absolute value is desired.
//
//    Output, float R4_ABS, the absolute value of X.
//
{
  float value;

  if ( 0.0 <= x )
  {
    value = x;
  } 
  else
  {
    value = -x;
  }
  return value;
}
//****************************************************************************80

float r4_atan ( float y, float x )

//****************************************************************************80
//
//  Purpose:
//
//    R4_ATAN computes the inverse tangent of the ratio Y / X.
//
//  Discussion:
//
//    R4_ATAN returns an angle whose tangent is ( Y / X ), a job which
//    the built in functions ATAN and ATAN2 already do.
//
//    However:
//
//    * R4_ATAN always returns a positive angle, between 0 and 2 PI,
//      while ATAN and ATAN2 return angles in the interval [-PI/2,+PI/2]
//      and [-PI,+PI] respectively;
//
//    * R4_ATAN accounts for the signs of X and Y, (as does ATAN2).  The ATAN
//     function by contrast always returns an angle in the first or fourth
//     quadrants.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float Y, X, two quantities which represent the tangent of
//    an angle.  If Y is not zero, then the tangent is (Y/X).
//
//    Output, float R4_ATAN, an angle between 0 and 2 * PI, whose tangent is
//    (Y/X), and which lies in the appropriate quadrant so that the signs
//    of its cosine and sine match those of X and Y.
//
{
  float abs_x;
  float abs_y;
  float pi = 3.141592653589793;
  float theta;
  float theta_0;
//
//  Special cases:
//
  if ( x == 0.0 )
  {
    if ( 0.0 < y )
    {
      theta = pi / 2.0;
    }
    else if ( y < 0.0 )
    {
      theta = 3.0 * pi / 2.0;
    }
    else if ( y == 0.0 )
    {
      theta = 0.0;
    }
  }
  else if ( y == 0.0 )
  {
    if ( 0.0 < x )
    {
      theta = 0.0;
    }
    else if ( x < 0.0 )
    {
      theta = pi;
    }
  }
//
//  We assume that ATAN2 is correct when both arguments are positive.
//
  else
  {
    abs_y = r4_abs ( y );
    abs_x = r4_abs ( x );

    theta_0 = atan2 ( abs_y, abs_x );

    if ( 0.0 < x && 0.0 < y )
    {
      theta = theta_0;
    }
    else if ( x < 0.0 && 0.0 < y )
    {
      theta = pi - theta_0;
    }
    else if ( x < 0.0 && y < 0.0 )
    {
      theta = pi + theta_0;
    }
    else if ( 0.0 < x && y < 0.0 )
    {
      theta = 2.0 * pi - theta_0;
    }
  }
  return theta;
}
//****************************************************************************80

float r4_epsilon ( )

//****************************************************************************80
//
//  Purpose:
//
//    R4_EPSILON returns the R4 roundoff unit.
//
//  Discussion:
//
//    The roundoff unit is a number R which is a power of 2 with the 
//    property that, to the precision of the computer's arithmetic,
//      1 < 1 + R
//    but 
//      1 = ( 1 + R / 2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 July 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, float R4_EPSILON, the R4 round-off unit.
//
{
  float value;

  value = 1.0;

  while ( 1.0 < ( float ) ( 1.0 + value )  )
  {
    value = value / 2.0;
  }

  value = 2.0 * value;

  return value;
}
//****************************************************************************80

float r4_exp ( float x )

//****************************************************************************80
//
//  Purpose:
//
//    R4_EXP computes the exponential function, avoiding overflow and underflow.
//
//  Discussion:
//
//    My experience with the G95 compiler has included many unpleasant
//    floating point exceptions when very small arguments are given to
//    the exponential function.
//
//    This routine is designed to avoid such problems.
//
//    Ideally, the rule would be:
//
//                    X <= log ( TINY ) => R4_EXP ( X ) = 0
//    log ( HUGE ) <= X                 => R4_EXP ( X ) = HUGE
//
//    However, the G95 math library seems to produce infinity for
//    EXP ( LOG ( HUGE ( X ) ), rather than HUGE ( X ), so we've
//    included a fudge factor.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, the argument of the exponential function.
//
//    Output, float R4_EXP, the value of exp ( X ).
//
{
  float log_max = 88.71397;
  float log_min = -87.34528;
  float value;

  if ( x <= log_min )
  {
    value = 0.0;
  }
  else if ( x < log_max )
  {
    value = exp ( x );
  }
  else
  {
    value = r4_huge ( );
  }

  return value;
}
//****************************************************************************80

float r4_huge ( )

//****************************************************************************80
//
//  Purpose:
//
//    R4_HUGE returns a "huge" R4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, float R4_HUGE, a "huge" R4 value.
//
{
  float value;

  value = 0.3402823466385E+39;

  return value;
}
//****************************************************************************80

float r4_max ( float x, float y )

//****************************************************************************80
//
//  Purpose:
//
//    R4_MAX returns the maximum of two R4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, Y, the quantities to compare.
//
//    Output, float R4_MAX, the maximum of X and Y.
//
{
  float value;

  if ( y < x )
  {
    value = x;
  } 
  else
  {
    value = y;
  }
  return value;
}
//****************************************************************************80

float r4_min ( float x, float y )

//****************************************************************************80
//
//  Purpose:
//
//    R4_MIN returns the minimum of two R4's..
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, Y, the quantities to compare.
//
//    Output, float R_MIN, the minimum of X and Y.
//
{
  float value;

  if ( y < x )
  {
    value = y;
  } 
  else
  {
    value = x;
  }
  return value;
}
//****************************************************************************80

int r4_nint ( float x )

//****************************************************************************80
//
//  Purpose:
//
//    R4_NINT returns the nearest integer to an R4.
//
//  Example:
//
//        X         R4_NINT
//
//      1.3         1
//      1.4         1
//      1.5         1 or 2
//      1.6         2
//      0.0         0
//     -0.7        -1
//     -1.1        -1
//     -1.6        -2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, the value.
//
//    Output, int R4_NINT, the nearest integer to X.
//
{
  int value;

  if ( x < 0.0 )
  {
    value = - ( int ) ( r4_abs ( x ) + 0.5 );
  }
  else
  {
    value =   ( int ) ( r4_abs ( x ) + 0.5 );
  }

  return value;
}
//****************************************************************************80

float r4_reverse_bytes ( float x )

//****************************************************************************80
//
//  Purpose:
//
//    R4_REVERSE_BYTES reverses the four bytes in an R4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, float X, a value whose bytes are to be reversed.
//
//    Output, R4_REVERSE_BYTES, a value with bytes in reverse order;
//
{
  char c;
  union
  {
    float yfloat;
    char ychar[4];
  } y;

  y.yfloat = x;
  
  c = y.ychar[0];
  y.ychar[0] = y.ychar[3];
  y.ychar[3] = c;

  c = y.ychar[1];
  y.ychar[1] = y.ychar[2];
  y.ychar[2] = c;

  return ( y.yfloat );
}
//****************************************************************************80

void r4_swap ( float *x, float *y )

//****************************************************************************80
//
//  Purpose:
//
//    R4_SWAP switches two R4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, float *X, *Y.  On output, the values of X and
//    Y have been interchanged.
//
{
  float z;

  z = *x;
  *x = *y;
  *y = z;
 
  return;
}
//****************************************************************************80

float r4_tiny ( )

//****************************************************************************80
//
//  Purpose:
//
//    R4_TINY returns a "tiny" R4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 March 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, float R4_TINY, a "tiny" R4 value.
//
{
  float value;

  value = 0.1175494350822E-37;

  return value;
}
//****************************************************************************80

float r4_uniform_01 ( int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R4_UNIFORM_01 returns a unit pseudorandom R4.
//
//  Discussion:
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      r4_uniform_01 = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//    If the initial seed is 12345, then the first three computations are
//
//      Input     Output      R4_UNIFORM_01
//      SEED      SEED
//
//         12345   207482415  0.096616
//     207482415  1790989824  0.833995
//    1790989824  2035175616  0.947702
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 November 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation
//    edited by Jerry Banks,
//    Wiley Interscience, page 95, 1998.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Peter Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input/output, int *SEED, the "seed" value.  Normally, this
//    value should not be 0.  On output, SEED has been updated.
//
//    Output, float R4_UNIFORM_01, a new pseudorandom variate, strictly between
//    0 and 1.
//
{
  int k;
  float value;

  if ( *seed == 0 )
  {
    cerr << "\n";
    cerr << "R4_UNIFORM_01 - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }
//
//  Although SEED can be represented exactly as a 32 bit integer,
//  it generally cannot be represented exactly as a 32 bit real number//
//
  value = ( float ) ( *seed ) * 4.656612875E-10;

  return value;
}
//****************************************************************************80

double r8_abs ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ABS returns the absolute value of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the quantity whose absolute value is desired.
//
//    Output, double R8_ABS, the absolute value of X.
//
{
  double value;

  if ( 0.0 <= x )
  {
    value = x;
  } 
  else
  {
    value = - x;
  }
  return value;
}
//****************************************************************************80

double r8_atan ( double y, double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ATAN computes the inverse tangent of the ratio Y / X.
//
//  Discussion:
//
//    R8_ATAN returns an angle whose tangent is ( Y / X ), a job which
//    the built in functions ATAN and ATAN2 already do.
//
//    However:
//
//    * R8_ATAN always returns a positive angle, between 0 and 2 PI,
//      while ATAN and ATAN2 return angles in the interval [-PI/2,+PI/2]
//      and [-PI,+PI] respectively;
//
//    * R8_ATAN accounts for the signs of X and Y, (as does ATAN2).  The ATAN
//     function by contrast always returns an angle in the first or fourth
//     quadrants.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double Y, X, two quantities which represent the tangent of
//    an angle.  If Y is not zero, then the tangent is (Y/X).
//
//    Output, double R8_ATAN, an angle between 0 and 2 * PI, whose tangent is
//    (Y/X), and which lies in the appropriate quadrant so that the signs
//    of its cosine and sine match those of X and Y.
//
{
  double abs_x;
  double abs_y;
  double pi = 3.141592653589793;
  double theta;
  double theta_0;
//
//  Special cases:
//
  if ( x == 0.0 )
  {
    if ( 0.0 < y )
    {
      theta = pi / 2.0;
    }
    else if ( y < 0.0 )
    {
      theta = 3.0 * pi / 2.0;
    }
    else if ( y == 0.0 )
    {
      theta = 0.0;
    }
  }
  else if ( y == 0.0 )
  {
    if ( 0.0 < x )
    {
      theta = 0.0;
    }
    else if ( x < 0.0 )
    {
      theta = pi;
    }
  }
//
//  We assume that ATAN2 is correct when both arguments are positive.
//
  else
  {
    abs_y = r8_abs ( y );
    abs_x = r8_abs ( x );

    theta_0 = atan2 ( abs_y, abs_x );

    if ( 0.0 < x && 0.0 < y )
    {
      theta = theta_0;
    }
    else if ( x < 0.0 && 0.0 < y )
    {
      theta = pi - theta_0;
    }
    else if ( x < 0.0 && y < 0.0 )
    {
      theta = pi + theta_0;
    }
    else if ( 0.0 < x && y < 0.0 )
    {
      theta = 2.0 * pi - theta_0;
    }
  }

  return theta;
}
//****************************************************************************80

double r8_cas ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CAS returns the "casine" of an R8.
//
//  Discussion:
//
//    The "casine", used in the discrete Hartley transform, is abbreviated
//    CAS(X), and defined by:
//
//      CAS(X) = cos ( X ) + sin( X )
//             = sqrt ( 2 ) * sin ( X + pi/4 )
//             = sqrt ( 2 ) * cos ( X - pi/4 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose casine is desired.
//
//    Output, double R8_CAS, the casine of X, which will be between
//    plus or minus the square root of 2.
//
{
  double value;

  value = cos ( x ) + sin ( x );

  return value;
}
//****************************************************************************80

double r8_ceiling ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CEILING rounds an R8 "up" (towards +infinity) to the next integer.
//
//  Example:
//
//    X        R8_CEILING(X)
//
//   -1.1      -1.0
//   -1.0      -1.0
//   -0.9       0.0
//   -0.1       0.0
//    0.0       0.0
//    0.1       1.0
//    0.9       1.0
//    1.0       1.0
//    1.1       2.0
//    2.9       3.0
//    3.0       3.0
//    3.14159   4.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose ceiling is desired.
//
//    Output, double R8_CEILING, the ceiling of X.
//
{
  double value;

  value = ( int ) x;

  if ( value < x )
  {
    value = value + 1.0;
  }

  return value;
}
//****************************************************************************80

double r8_choose ( int n, int k )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CHOOSE computes the binomial coefficient C(N,K) as an R8.
//
//  Discussion:
//
//    The value is calculated in such a way as to avoid overflow and
//    roundoff.  The calculation is done in R8 arithmetic.
//
//    The formula used is:
//
//      C(N,K) = N// / ( K// * (N-K)// )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 March 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    ML Wolfson, HV Wright,
//    Algorithm 160:
//    Combinatorial of M Things Taken N at a Time,
//    Communications of the ACM,
//    Volume 6, Number 4, April 1963, page 161.
//
//  Parameters:
//
//    Input, int N, K, the values of N and K.
//
//    Output, double R8_CHOOSE, the number of combinations of N
//    things taken K at a time.
//
{
  int i;
  int mn;
  int mx;
  int value;

  mn = i4_min ( k, n - k );

  if ( mn < 0 )
  {
    value = 0.0;
  }
  else if ( mn == 0 )
  {
    value = 1.0;
  }
  else
  {
    mx = i4_max ( k, n - k );
    value = ( double ) ( mx + 1 );

    for ( i = 2; i <= mn; i++ )
    {
      value = ( value * ( double ) ( mx + i ) ) / ( double ) i;
    }
  }

  return value;
}
//****************************************************************************80

double r8_chop ( int place, double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CHOP chops an R8 to a given number of binary places.
//
//  Example:
//
//    3.875 = 2 + 1 + 1/2 + 1/4 + 1/8.
//
//    The following values would be returned for the 'chopped' value of
//    3.875:
//
//    PLACE  Value
//
//       1      2
//       2      3     = 2 + 1
//       3      3.5   = 2 + 1 + 1/2
//       4      3.75  = 2 + 1 + 1/2 + 1/4
//       5+     3.875 = 2 + 1 + 1/2 + 1/4 + 1/8
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int PLACE, the number of binary places to preserve.
//    PLACE = 0 means return the integer part of X.
//    PLACE = 1 means return the value of X, correct to 1/2.
//    PLACE = 2 means return the value of X, correct to 1/4.
//    PLACE = -1 means return the value of X, correct to 2.
//
//    Input, double X, the number to be chopped.
//
//    Output, double R8_CHOP, the chopped number.
//
{
  double fac;
  int temp;
  double value;

  temp = ( int ) ( r8_log_2 ( x ) );
  fac = pow ( 2.0, ( temp - place + 1 ) );
  value = ( double ) ( ( int ) ( x / fac ) ) * fac;

  return value;
}
//****************************************************************************80

complex <double> r8_csqrt ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CSQRT returns the complex square root of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose square root is desired.
//
//    Output, complex <double> R8_CSQRT, the square root of X:
//
{
  double argument;
  double magnitude;
  double pi = 3.141592653589793;
  complex <double> value;

  if ( 0.0 < x )
  {
    magnitude = x;
    argument = 0.0;
  }
  else if ( 0.0 == x )
  {
    magnitude = 0.0;
    argument = 0.0;
  }
  else if ( x < 0.0 )
  {
    magnitude = -x;
    argument = pi;
  }

  magnitude = sqrt ( magnitude );
  argument = argument / 2.0;

  value = magnitude * complex <double> ( cos ( argument ), sin ( argument ) );

  return value;
}
//****************************************************************************80

double r8_cube_root ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_CUBE_ROOT returns the cube root of an R8.
//
//  Discussion:
//
//    This routine is designed to avoid the possible problems that can occur
//    when formulas like 0.0**(1/3) or (-1.0)**(1/3) are to be evaluated.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input double X, the number whose cube root is desired.
//
//    Output, double R8_CUBE_ROOT, the cube root of X.
//
{
  double value;

  if ( 0.0 < x )
  {
    value = pow ( ( double ) x, (1.0/3.0) );
  }
  else if ( x == 0.0 )
  {
    value = 0.0;
  }
  else
  {
    value = - pow ( ( double ) ( r8_abs ( x ) ), (1.0/3.0) );
  }

  return value;
}
//****************************************************************************80

double r8_diff ( double x, double y, int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8_DIFF computes (X-Y) to a specified accuracy.
//
//  Discussion:
//
//    The user controls how many binary digits of accuracy
//    are to be used.
//
//    N determines the accuracy of the value.  If N = 10, 
//    for example, only 11 binary places will be used in the arithmetic.  
//    In general, only N+1 binary places will be used.
//
//    N may be zero.  However, a negative value of N should
//    not be used, since this will cause both X and Y to look like 0.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the two values whose difference is desired.
//
//    Input, int N, the number of binary digits to use.
//
//    Output, double R8_DIFF, the value of X-Y.
//
{
  double cx;
  double cy;
  double pow2;
  double size;
  double value;

  if ( x == y )
  {
    value = 0.0;
    return value;
  }

  pow2 = pow ( 2.0, n );
//
//  Compute the magnitude of X and Y, and take the larger of the
//  two.  At least one of the two values is not zero//
//
  size = r8_max ( r8_abs ( x ), r8_abs ( y ) );
//
//  Make normalized copies of X and Y.  One of the two values will
//  actually be equal to 1.
//
  cx = x / size;
  cy = y / size;
//
//  Here's where rounding comes in.  We know that the larger of the
//  the two values equals 1.  We multiply both values by 2**N,
//  where N+1 is the number of binary digits of accuracy we want
//  to use, truncate the values, and divide back by 2**N.
//
  cx = ( double ) ( ( int ) ( cx * pow2 + 0.5 * r8_sign ( cx ) ) ) / pow2;
  cy = ( double ) ( ( int ) ( cy * pow2 + 0.5 * r8_sign ( cy ) ) ) / pow2;
//
//  Take the difference now.
//
  value = cx - cy;
//
//  Undo the scaling.
//
  value = value * size;

  return value;
}
//****************************************************************************80

int r8_digit ( double x, int idigit )

//****************************************************************************80
//
//  Purpose:
//
//    R8_DIGIT returns a particular decimal digit of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose IDIGIT-th decimal digit is desired.
//    Note that if X is zero, all digits will be returned as 0.
//
//    Input, int IDIGIT, the position of the desired decimal digit.
//    A value of 1 means the leading digit, a value of 2 the second digit
//    and so on.
//
//    Output, int R8_DIGIT, the value of the IDIGIT-th decimal digit of X.
//
{
  int digit;
  int i;
  int ival;

  if ( x == 0.0 )
  {
    digit = 0;
    return digit;
  }

  if ( idigit <= 0 )
  {
    digit = 0;
    return digit;
  }
//
//  Force X to lie between 1 and 10.
//
  x = r8_abs ( x );

  while ( x < 1.0 )
  {
    x = x * 10.0;
  }

  while ( 10.0 <= x )
  {
    x = x / 10.0;
  }

  for ( i = 1; i <= idigit; i++ )
  {
    ival = ( int ) ( x );
    x = ( x - ( double ) ival ) * 10.0;
  }

  digit = ival;

  return digit;
}
//****************************************************************************80

double r8_epsilon ( )

//****************************************************************************80
//
//  Purpose:
//
//    R8_EPSILON returns the R8 roundoff unit.
//
//  Discussion:
//
//    The roundoff unit is a number R which is a power of 2 with the 
//    property that, to the precision of the computer's arithmetic,
//      1 < 1 + R
//    but 
//      1 = ( 1 + R / 2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 July 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_EPSILON, the R8 round-off unit.
//
{
  double value;

  value = 1.0;

  while ( 1.0 < ( double ) ( 1.0 + value )  )
  {
    value = value / 2.0;
  }

  value = 2.0 * value;

  return value;
}
//****************************************************************************80

double r8_exp ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_EXP computes the exponential function, avoiding overflow and underflow.
//
//  Discussion:
//
//    My experience with the G95 compiler has included many unpleasant
//    floating point exceptions when very small arguments are given to
//    the exponential function.
//
//    This routine is designed to avoid such problems.
//
//    Ideally, the rule would be:
//
//                    X <= log ( TINY ) => R8_EXP ( X ) = 0
//    log ( HUGE ) <= X                 => R8_EXP ( X ) = HUGE
//
//    However, the G95 math library seems to produce infinity for
//    EXP ( LOG ( HUGE ( X ) ), rather than HUGE ( X ), so we've
//    included a fudge factor.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 April 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the argument of the exponential function.
//
//    Output, double R8_EXP, the value of exp ( X ).
//
{
  double log_max = 709.711;
  double log_min = -708.467;
  double value;

  if ( x <= log_min )
  {
    value = 0.0;
  }
  else if ( x < log_max )
  {
    value = exp ( x );
  }
  else
  {
    value = r8_huge ( );
  }

  return value;
}
//****************************************************************************80

double r8_factorial ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8_FACTORIAL computes the factorial of N.
//
//  Discussion:
//
//    factorial ( N ) = product ( 1 <= I <= N ) I
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 January 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the factorial function.
//    If N is less than 1, the function value is returned as 1.
//
//    Output, double R8_FACTORIAL, the factorial of N.
//
{
  int i;
  double value;

  value = 1.0;

  for ( i = 1; i <= n; i++ )
  {
    value = value * ( double ) ( i );
  }

  return value;
}
//****************************************************************************80

double r8_factorial2 ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8_FACTORIAL2 computes the double factorial function.
//
//  Discussion:
//
//    FACTORIAL2( N ) = Product ( N * (N-2) * (N-4) * ... * 2 )  (N even)
//                    = Product ( N * (N-2) * (N-4) * ... * 1 )  (N odd)
//
//  Example:
//
//     N    Factorial2(N)
//
//     0     1
//     1     1
//     2     2
//     3     3
//     4     8
//     5    15
//     6    48
//     7   105
//     8   384
//     9   945
//    10  3840
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 January 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the double factorial 
//    function.  If N is less than 1, R8_FACTORIAL2 is returned as 1.0.
//
//    Output, double R8_FACTORIAL2, the value of Factorial2(N).
//
{
  int n_copy;
  double value;

  value = 1.0;

  if ( n < 1 )
  {
    return value;
  }

  n_copy = n;

  while ( 1 < n_copy )
  {
    value = value * ( double ) n_copy;
    n_copy = n_copy - 2;
  }

  return value;
}
//****************************************************************************80

double r8_floor ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_FLOOR rounds an R8 "down" (towards -infinity) to the next integer.
//
//  Example:
//
//    X        R8_FLOOR(X)
//
//   -1.1      -2.0
//   -1.0      -1.0
//   -0.9      -1.0
//   -0.1      -1.0
//    0.0       0.0
//    0.1       0.0
//    0.9       0.0
//    1.0       1.0
//    1.1       1.0
//    2.9       2.0
//    3.0       3.0
//    3.14159   3.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose floor is desired.
//
//    Output, double R8_FLOOR, the floor of X.
//
{
  double value;

  value = ( int ) x;

  if ( x < value )
  {
    value = value - 1.0;
  }

  return value;
}
//****************************************************************************80

double r8_fraction ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_FRACTION returns the fraction part of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 January 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the argument.
//
//    Output, double R8_FRACTION, the fraction part of X.
//
{
  double value;

  value = r8_abs ( x ) - ( double ) ( ( int ) r8_abs ( x ) );

  return value;
}
//****************************************************************************80

double r8_huge ( )

//****************************************************************************80
//
//  Purpose:
//
//    R8_HUGE returns a "huge" R8.
//
//  Discussion:
//
//    The value returned by this function is NOT required to be the
//    maximum representable R8.  This value varies from machine to machine,
//    from compiler to compiler, and may cause problems when being printed.
//    We simply want a "very large" but non-infinite number.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 October 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_HUGE, a "huge" R8 value.
//
{
  double value;

  value = 1.0E+30;

  return value;
}
//****************************************************************************80

bool r8_in_01 ( double a )

//****************************************************************************80
//
//  Purpose:
//
//    R8_IN_01 is TRUE if an R8 is in the range [0,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A, the value.
//
//    Output, bool R8_IN_01, is TRUE if A is between 0 and 1.
//
{
  if ( a < 0.0 || 1.0 < a )
  {
    return false;
  }

  return true;
}
//****************************************************************************80

bool r8_is_int ( double r )

//****************************************************************************80
//
//  Purpose:
//
//    R8_IS_INT determines if an R8 represents an integer value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, the number to be checked.
//
//    Output, bool R8_IS_INT, is TRUE if R is an integer value.
//
{
  int i4_huge = 2147483647;

  if ( ( double ) ( i4_huge ) < r )
  {
    return false;
  }
  else if ( r < - ( double ) ( i4_huge ) ) 
  {
    return false;
  }
  else if ( r == ( double ) ( ( int ) ( r ) ) )
  {
    return true;
  }
  else
  {
    return false;
  }

}
//****************************************************************************80

double r8_log_10 ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_LOG_10 returns the logarithm base 10 of the absolute value of an R8.
//
//  Discussion:
//
//    value = Log10 ( |X| )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose base 2 logarithm is desired.
//    X should not be 0.
//
//    Output, double R8_LOG_10, the logarithm base 10 of the absolute
//    value of X.  It should be true that |X| = 10**R_LOG_10.
//
{
  double value;

  if ( x == 0.0 )
  {
    value = - r8_huge ( );
  }
  else
  {
    value = log10 ( r8_abs ( x ) );
  }

  return value;
}
//****************************************************************************80

double r8_log_2 ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_LOG_2 returns the logarithm base 2 of the absolute value of an R8.
//
//  Discussion:
//
//    value = Log ( |X| ) / Log ( 2.0 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose base 2 logarithm is desired.
//    X should not be 0.
//
//    Output, double R8_LOG_2, the logarithm base 2 of the absolute
//    value of X.  It should be true that |X| = 2**R_LOG_2.
//
{
  double value;

  if ( x == 0.0 )
  {
    value = - r8_huge ( );
  }
  else
  {
    value = log ( r8_abs ( x ) ) / log ( 2.0 );
  }

  return value;
}
//****************************************************************************80

double r8_log_b ( double x, double b )

//****************************************************************************80
//
//  Purpose:
//
//    R8_LOG_B returns the logarithm base B of an R8.
//
//  Discussion:
//
//    value = log ( |X| ) / log ( |B| )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose base B logarithm is desired.
//    X should not be 0.
//
//    Input, double B, the base, which should not be 0, 1 or -1.
//
//    Output, double R8_LOG_B, the logarithm base B of the absolute
//    value of X.  It should be true that |X| = |B|**R_LOG_B.
//
{
  double value;

  if ( b == 0.0 | b == 1.0 | b == -1.0 )
  {
    value = - r8_huge ( );
  }
  else if ( r8_abs ( x ) == 0.0 )
  {
    value = - r8_huge ( );
  }
  else
  {
    value = log ( r8_abs ( x ) ) / log ( r8_abs ( b ) );
  }

  return value;
}
//****************************************************************************80

void r8_mant ( double x, int *s, double *r, int *l )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MANT computes the "mantissa" or "fraction part" of an R8.
//
//  Discussion:
//
//    X = S * R * 2^L
//
//    S is +1 or -1,
//    R is a real between 1.0 and 2.0,
//    L is an integer.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the real number to be decomposed.
//
//    Output, int *S, the "sign" of the number.
//    S will be -1 if X is less than 0, and +1 if X is greater
//    than or equal to zero.
//
//    Output, double *R, the mantissa of X.  R will be greater
//    than or equal to 1, and strictly less than 2.  The one
//    exception occurs if X is zero, in which case R will also
//    be zero.
//
//    Output, int *L, the integer part of the logarithm (base 2) of X.
//
{
//
//  Determine the sign.
//
  if ( x < 0.0 )
  {
    *s = -1;
  }
  else
  {
    *s = 1;
  }
//
//  Set R to the absolute value of X, and L to zero.
//  Then force R to lie between 1 and 2.
//
  if ( x < 0.0 )
  {
    *r = -x;
  }
  else
  {
    *r = x;
  }

  *l = 0;
//
//  Time to bail out if X is zero.
//
  if ( x == 0.0 )
  {
    return;
  }

  while ( 2.0 <= *r )
  {
    *r = *r / 2.0;
    *l = *l + 1;
  }

  while ( *r < 1.0 )
  {
    *r = *r * 2.0;
    *l = *l - 1;
  }

  return;
}
//****************************************************************************80

double r8_max ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MAX returns the maximum of two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the quantities to compare.
//
//    Output, double R8_MAX, the maximum of X and Y.
//
{
  double value;

  if ( y < x )
  {
    value = x;
  } 
  else
  {
    value = y;
  }
  return value;
}
//****************************************************************************80

double r8_min ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MIN returns the minimum of two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the quantities to compare.
//
//    Output, double R8_MIN, the minimum of X and Y.
//
{
  double value;

  if ( y < x )
  {
    value = y;
  } 
  else
  {
    value = x;
  }
  return value;
}
//****************************************************************************80

double r8_mod ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MOD returns the remainder of R8 division.
//
//  Discussion:
//
//    If
//      REM = R8_MOD ( X, Y )
//      RMULT = ( X - REM ) / Y
//    then
//      X = Y * RMULT + REM
//    where REM has the same sign as X, and abs ( REM ) < Y.
//
//  Example:
//
//        X         Y     R8_MOD   R8_MOD  Factorization
//
//      107        50       7     107 =  2 *  50 + 7
//      107       -50       7     107 = -2 * -50 + 7
//     -107        50      -7    -107 = -2 *  50 - 7
//     -107       -50      -7    -107 =  2 * -50 - 7
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number to be divided.
//
//    Input, double Y, the number that divides X.
//
//    Output, double R8_MOD, the remainder when X is divided by Y.
//
{
  double value;

  if ( y == 0.0 )
  {
    cout << "\n";
    cout << "R8_MOD - Fatal error!\n";
    cout << "  R8_MOD ( X, Y ) called with Y = " << y << "\n";
    exit ( 1 );
  }

  value = x - ( ( double ) ( ( int ) ( x / y ) ) ) * y;

  if ( x < 0.0 && 0.0 < value )
  {
    value = value - r8_abs ( y );
  }
  else if ( 0.0 < x && value < 0.0 )
  {
    value = value + r8_abs ( y );
  }

  return value;
}
//****************************************************************************80

double r8_modp ( double x, double y )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MODP returns the nonnegative remainder of R8 division.
//
//  Discussion:
//
//    The MOD function computes a result with the same sign as the
//    quantity being divided.  Thus, suppose you had an angle A,
//    and you wanted to ensure that it was between 0 and 360.
//    Then mod(A,360.0) would do, if A was positive, but if A
//    was negative, your result would be between -360 and 0.
//
//    On the other hand, R8_MODP(A,360.0) is between 0 and 360, always.
//
//    If
//      REM = R8_MODP ( X, Y )
//      RMULT = ( X - REM ) / Y
//    then
//      X = Y * RMULT + REM
//    where REM is always nonnegative.
//
//  Example:
//
//        I         J     MOD  R8_MODP   R8_MODP Factorization
//
//      107        50       7       7    107 =  2 *  50 + 7
//      107       -50       7       7    107 = -2 * -50 + 7
//     -107        50      -7      43   -107 = -3 *  50 + 43
//     -107       -50      -7      43   -107 =  3 * -50 + 43
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number to be divided.
//
//    Input, double Y, the number that divides X.
//
//    Output, double R8_MODP, the nonnegative remainder when X is divided by Y.
//
{
  double value;

  if ( y == 0.0 )
  {
    cout << "\n";
    cout << "R8_MODP - Fatal error!\n";
    cout << "  R8_MODP ( X, Y ) called with Y = " << y << "\n";
    exit ( 1 );
  }

  value = x - ( ( double ) ( ( int ) ( x / y ) ) ) * y;

  if ( value < 0.0 )
  {
    value = value + r8_abs ( y );
  }

  return value;
}
//****************************************************************************80

double r8_mop ( int i )

//****************************************************************************80
//
//  Purpose:
//
//    R8_MOP returns the I-th power of -1 as an R8 value.
//
//  Discussion:
//
//    An R8 is an double value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 November 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, the power of -1.
//
//    Output, double R8_MOP, the I-th power of -1.
//
{
  double value;

  if ( ( i % 2 ) == 0 )
  {
    value = 1.0;
  }
  else
  {
    value = -1.0;
  }

  return value;
}
//****************************************************************************80

int r8_nint ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_NINT returns the nearest integer to an R8.
//
//  Example:
//
//        X         Value
//
//      1.3         1
//      1.4         1
//      1.5         1 or 2
//      1.6         2
//      0.0         0
//     -0.7        -1
//     -1.1        -1
//     -1.6        -2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the value.
//
//    Output, int R8_NINT, the nearest integer to X.
//
{
  int value;

  if ( x < 0.0 )
  {
    value = - ( int ) ( r8_abs ( x ) + 0.5 );
  }
  else
  {
    value =   ( int ) ( r8_abs ( x ) + 0.5 );
  }

  return value;
}
//****************************************************************************80

double r8_normal_01 ( int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8_NORMAL_01 samples the standard normal probability distribution.
//
//  Discussion:
//
//    The standard normal probability distribution function (PDF) has 
//    mean 0 and standard deviation 1.
//
//    The Box-Muller method is used, which is efficient, but 
//    generates two values at a time.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8_NORMAL_01, a normally distributed random value.
//
{
  double pi = 3.141592653589793;
  double r1;
  double r2;
  static int used = -1;
  double x;
  static double y = 0.0;

  if ( used == -1 )
  {
    used = 0;
  }
//
//  If we've used an even number of values so far, generate two more, return one,
//  and save one.
//
  if ( ( used % 2 )== 0 )
  {
    for ( ; ; )
    {
      r1 = r8_uniform_01 ( seed );
      if ( r1 != 0.0 )
      {
        break;
      }
    }

    r2 = r8_uniform_01 ( seed );

    x = sqrt ( -2.0 * log ( r1 ) ) * cos ( 2.0 * pi * r2 );
    y = sqrt ( -2.0 * log ( r1 ) ) * sin ( 2.0 * pi * r2 );
  }
  else
  {

    x = y;

  }

  used = used + 1;

  return x;
}
//****************************************************************************80

double r8_pi ( )

//****************************************************************************80
//
//  Purpose:
//
//    R8_PI returns the value of PI to 16 digits.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_PI, the value of PI.
//
{
  double value = 3.141592653589793;

  return value;
}
//****************************************************************************80

double r8_power ( double r, int p )

//****************************************************************************80
//
//  Purpose:
//
//    R8_POWER computes an integer power of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, the base.
//
//    Input, int P, the power, which may be negative.
//
//    Output, double R8_POWER, the value of R^P.
//
{
  double value;
//
//  Special case.  R^0 = 1.
//
  if ( p == 0 )
  {
    value = 1.0;
  }
//
//  Special case.  Positive powers of 0 are 0.
//  We go ahead and compute negative powers, relying on the software to complain.
//
  else if ( r == 0.0 )
  {
    if ( 0 < p )
    {
      value = 0.0;
    }
    else
    {
      value = pow ( r, p );
    }
  }
  else if ( 1 <= p )
  {
    value = pow ( r, p );
  }
  else
  {
    value = pow ( r, p );
  }

  return value;
}
//****************************************************************************80

double r8_power_fast ( double r, int p, int *mults )

//****************************************************************************80
//
//  Purpose:
//
//    R8_POWER_FAST computes the P-th power of R, for real R and integer P.
//
//  Discussion:
//
//    Obviously, R^P can be computed using P-1 multiplications.
//
//    However, R^P can also be computed using at most 2*LOG2(P) multiplications.
//    To do the calculation this way, let N = LOG2(P).
//    Compute A, A^2, A^4, ..., A^N by N-1 successive squarings.
//    Start the value of R^P at A, and each time that there is a 1 in
//    the binary expansion of P, multiply by the current result of the squarings.
//
//    This algorithm is not optimal.  For small exponents, and for special
//    cases, the result can be computed even more quickly.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, the base.
//
//    Input, int P, the power, which may be negative.
//
//    Output, int *MULTS, the number of multiplications and divisions.
//
//    Output, double R8_POWER_FAST, the value of R^P.
//
{
  int p_mag;
  int p_sign;
  double r2;
  double value;

  *mults = 0;
//
//  Special bases.
//
  if ( r == 1.0 )
  {
    value = 1.0;
    return value;
  }

  if ( r == -1.0 )
  {
    if ( ( p % 2 ) == 1 )
    {
      value = -1.0;
    }
    else
    {
      value = 1.0;
    }
    return value;
  }

  if ( r == 0.0 )
  {
    if ( p <= 0 )
    {
      cout << "\n";
      cout << "R8_POWER_FAST - Fatal error!\n";
      cout << "  Base is zero, and exponent is negative.\n";
      exit ( 1 );
    }

    value = 0.0;
    return value;
  }
//
//  Special powers.
//
  if ( p == -1 )
  {
    value = 1.0 / r;
    *mults = *mults + 1;
    return value;
  }
  else if ( p == 0 )
  {
    value = 1.0;
    return value;
  }
  else if ( p == 1 )
  {
    value = r;
    return value;
  }
//
//  Some work to do.
//
  p_mag = abs ( p );
  p_sign = i4_sign ( p );

  value = 1.0;
  r2 = r;

  while ( 0 < p_mag )
  {
    if ( ( p_mag % 2 ) == 1 )
    {
      value = value * r2;
      *mults = *mults + 1;
    }

    p_mag = p_mag / 2;
    r2 = r2 * r2;
    *mults = *mults + 1;
  }

  if ( p_sign == -1 )
  {
    value = 1.0 / value;
    *mults = *mults + 1;
  }

  return value;
}
//****************************************************************************80

double r8_pythag ( double a, double b )

//****************************************************************************80
//
//  Purpose:
//
//    R8_PYTHAG computes sqrt ( A**2 + B**2 ), avoiding overflow and underflow.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 January 2002
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A, B, the values for which sqrt ( A**2 + B**2 ) is desired.
//
//    Output, double R8_PYTHAG, the value of sqrt ( A**2 + B**2 ).
//
{
  double a_abs;
  double b_abs;
  double result;

  a_abs = r8_abs ( a );
  b_abs = r8_abs ( b );

  if ( b_abs < a_abs )
  {
    result = a_abs * sqrt ( 1.0 + pow ( b_abs / a_abs, 2 ) );
  }
  else if ( b_abs == 0.0 )
  {
    result = 0.0;
  } 
  else if ( a_abs <= b_abs )
  {
    result = b_abs * sqrt ( 1.0 + pow ( a_abs / b_abs, 2 ) );
  }

  return result;
}
//****************************************************************************80

double r8_reverse_bytes ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_REVERSE_BYTES reverses the bytes in an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, a value whose bytes are to be reversed.
//
//    Output, R8_REVERSE_BYTES, a value with bytes in reverse order;
//
{
  char c;
  union
  {
    double ydouble;
    char ychar[8];
  } y;

  y.ydouble = x;
  
  c = y.ychar[0];
  y.ychar[0] = y.ychar[7];
  y.ychar[7] = c;

  c = y.ychar[1];
  y.ychar[1] = y.ychar[6];
  y.ychar[6] = c;

  c = y.ychar[2];
  y.ychar[2] = y.ychar[5];
  y.ychar[5] = c;

  c = y.ychar[3];
  y.ychar[3] = y.ychar[4];
  y.ychar[4] = c;

  return ( y.ydouble );
}
//****************************************************************************80

double r8_round2 ( int nplace, double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ROUND2 rounds an R8 to a specified number of binary digits.
//
//  Discussion:
//
//    Assume that the input quantity X has the form
//
//      X = S * J * 2**L
//
//    where S is plus or minus 1, L is an integer, and J is a binary
//    mantissa which is either exactly zero, or greater than or equal
//    to 0.5 and less than 1.0.
//
//    Then on return, XROUND = R8_ROUND2 ( NPLACE, X ) will satisfy
//
//      XROUND = S * K * 2**L
//
//    where S and L are unchanged, and K is a binary mantissa which
//    agrees with J in the first NPLACE binary digits and is zero
//    thereafter.
//
//    If NPLACE is 0, XROUND will always be zero.
//
//    If NPLACE is 1, the mantissa of XROUND will be 0 or 0.5.
//
//    If NPLACE is 2, the mantissa of XROUND will be 0, 0.25, 0.50,
//    or 0.75.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPLACE, the number of binary digits to
//    preserve.  NPLACE should be 0 or positive.
//
//    Input, double X, the real number to be decomposed.
//
//    Output, double R8_ROUND2, the rounded value of X.
//
{
  int iplace;
  int l;
  int s;
  double xmant;
  double xround;
  double xtemp;
  double value;

  value = 0.0;
//
//  1: Handle the special case of 0.
//
  if ( x == 0.0 )
  {
    return value;
  }

  if ( nplace <= 0 )
  {
    return value;
  }
//
//  2: Determine the sign S.
//
  if ( 0.0 < x )
  {
    s = 1;
    xtemp = x;
  }
  else
  {
    s = -1;
    xtemp = -x;
  }
//
//  3: Force XTEMP to lie between 1 and 2, and compute the
//  logarithm L.
//
  l = 0;

  while ( 2.0 <= xtemp )
  {
    xtemp = xtemp / 2.0;
    l = l + 1;
  }

  while ( xtemp < 1.0 )
  {
    xtemp = xtemp * 2.0;
    l = l - 1;
  }
//
//  4: Strip out the digits of the mantissa as XMANT, and decrease L.
//
  xmant = 0.0;
  iplace = 0;

  for ( ; ; )
  {
    xmant = 2.0 * xmant;

    if ( 1.0 <= xtemp )
    {
      xmant = xmant + 1.0;
      xtemp = xtemp - 1.0;
    }

    iplace = iplace + 1;

    if ( xtemp == 0.0 || nplace <= iplace )
    {
      value = s * xmant * pow ( 2.0, l );
      break;
    }

    l = l - 1;
    xtemp = xtemp * 2.0;
  }

  return value;
}
//****************************************************************************80

double r8_roundb ( int base, int nplace, double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ROUNDB rounds a number to a given number of digits in a given base.
//
//  Discussion:
//
//    The code does not seem to do a good job of rounding when
//    the base is negative//
//
//    Assume that the input quantity X has the form
//
//      X = S * J * BASE**L
//
//    where S is plus or minus 1, L is an integer, and J is a
//    mantissa base BASE which is either exactly zero, or greater
//    than or equal to (1/BASE) and less than 1.0.
//
//    Then on return, XROUND will satisfy
//
//      XROUND = S * K * BASE**L
//
//    where S and L are unchanged, and K is a mantissa base BASE
//    which agrees with J in the first NPLACE digits and is zero
//    thereafter.
//
//    Note that because of rounding, for most bases, most numbers
//    with a fractional quantities cannot be stored exactly in the
//    computer, and hence will have trailing "bogus" digits.
//
//    If NPLACE is 0, XROUND will always be zero.
//
//    If NPLACE is 1, the mantissa of XROUND will be 0,
//    1/BASE, 2/BASE, ..., (BASE-1)/BASE.
//
//    If NPLACE is 2, the mantissa of XROUND will be 0,
//    BASE/BASE**2, (BASE+1)/BASE**2, ...,
//    BASE**2-2/BASE**2, BASE**2-1/BASE**2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int BASE, the base of the arithmetic.
//    BASE must not be zero.  Theoretically, BASE may be negative.
//
//    Input, int NPLACE, the number of digits base BASE to
//    preserve.  NPLACE should be 0 or positive.
//
//    Input, double X, the number to be decomposed.
//
//    Output, double R8_ROUNDB, the rounded value of X.
//
{
  int iplace;
  int is;
  int js;
  int l;
  double value;
  double xmant;
  double xround;
  double xtemp;

  value = 0.0;
//
//  0: Error checks.
//
  if ( base == 0 )
  {
    cout << "\n";
    cout << "R8_ROUNDB - Fatal error!\n";
    cout << "  The base BASE cannot be zero.\n";
    exit ( 1 );
  }
//
//  1: Handle the special case of 0.
//
  if ( x == 0.0 )
  {
    return value;
  }

  if ( nplace <= 0 )
  {
    return value;
  }
//
//  2: Determine the sign IS.
//
  if ( 0.0 < x )
  {
    is = 1;
    xtemp = x;
  }
  else
  {
    is = -1;
    xtemp = -x;
  }
//
//  3: Force XTEMP to lie between 1 and ABS(BASE), and compute the
//  logarithm L.
//
  l = 0;

  while ( ( double ) abs ( base ) <= r8_abs ( xtemp ) )
  {
    xtemp = xtemp / ( double ) ( base );

    if ( xtemp < 0.0 )
    {
      is = -is;
      xtemp = -xtemp;
    }
    l = l + 1;
  }

  while ( r8_abs ( xtemp ) < 1.0 )
  {
    xtemp = xtemp * ( double ) base;

    if ( xtemp < 0.0 )
    {
      is = -is;
      xtemp = -xtemp;
    }

    l = l - 1;
  }
//
//  4: Now strip out the digits of the mantissa as XMANT, and
//  decrease L.
//
  xmant = 0.0;
  iplace = 0;
  js = is;

  for ( ; ; )
  {
    xmant = ( double ) base * xmant;

    if ( xmant < 0.0 )
    {
      js = -js;
      xmant = -xmant;
    }

    if ( 1.0 <= xtemp )
    {
      xmant = xmant + ( int ) ( xtemp );
      xtemp = xtemp - ( int ) ( xtemp );
    }

    iplace = iplace + 1;

    if ( xtemp == 0.0 || nplace <= iplace ) 
    {
      value = ( double ) js * xmant * ( double ) i4_power ( base, l );
      break;
    }

    l = l - 1;
    xtemp = xtemp * ( double ) base;

    if ( xtemp < 0.0 )
    {
      is = -is;
      xtemp = -xtemp;
    }
  }

  return value;
}
//****************************************************************************80

double r8_roundx ( int nplace, double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ROUNDX rounds a double precision number.
//
//  Discussion:
//
//    Assume that the input quantity X has the form
//
//      X = S * J * 10^L
//
//    where S is plus or minus 1, L is an integer, and J is a decimal
//    mantissa which is either exactly zero, or greater than or equal
//    to 0.1 and less than 1.0.
//
//    Then on return, XROUND will satisfy
//
//      XROUND = S * K * 10^L
//
//    where S and L are unchanged, and K is a decimal mantissa which
//    agrees with J in the first NPLACE decimal digits and is zero
//    thereafter.
//
//    Note that because of rounding, most decimal fraction quantities
//    cannot be stored exactly in the computer, and hence will have
//    trailing "bogus" digits.
//
//    If NPLACE is 0, XROUND will always be zero.
//
//    If NPLACE is 1, the mantissa of XROUND will be 0, 0.1,
//    0.2, ..., or 0.9.
//
//    If NPLACE is 2, the mantissa of XROUND will be 0, 0.01, 0.02,
//    0.03, ..., 0.98, 0.99.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPLACE, the number of decimal digits to
//    preserve.  NPLACE should be 0 or positive.
//
//    Input, double X, the number to be decomposed.
//
//    Output, double R8_ROUNDX, the rounded value of X.
//
{
  int iplace;
  int is;
  int l;
  double xmant;
  double xround;
  double xtemp;

  xround = 0.0;
//
//  1: Handle the special case of 0.
//
  if ( x == 0.0 )
  {
    return xround;
  }

  if ( nplace <= 0 )
  {
    return xround;
  }
//
//  2: Determine the sign IS.
//
  if ( 0.0 < x )
  {
    is = 1;
    xtemp = x;
  }
  else
  {
    is = -1;
    xtemp = -x;
  }
//
//  3: Force XTEMP to lie between 1 and 10, and compute the
//  logarithm L.
//
  l = 0;

  while ( 10.0 <= x )
  {
    xtemp = xtemp / 10.0;
    l = l + 1;
  }

  while ( xtemp < 1.0 )
  {
    xtemp = xtemp * 10.0;
    l = l - 1;
  }
//
//  4: Now strip out the digits of the mantissa as XMANT, and
//  decrease L.
//
  xmant = 0.0;
  iplace = 0;

  for ( ; ; )
  {
    xmant = 10.0 * xmant;

    if ( 1.0 <= xtemp )
    {
      xmant = xmant + ( int ) xtemp;
      xtemp = xtemp - ( int ) xtemp;
    }

    iplace = iplace + 1;

    if ( xtemp == 0.0 || nplace <= iplace )
    {
      xround = is * xmant * pow ( 10.0, l );
      break;
    }

    l = l - 1;
    xtemp = xtemp * 10.0;
  }

  return xround;
}
//****************************************************************************80

double r8_sign ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_SIGN returns the sign of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose sign is desired.
//
//    Output, double R8_SIGN, the sign of X.
//
{
  double value;

  if ( x < 0.0 )
  {
    value = -1.0;
  } 
  else
  {
    value = 1.0;
  }
  return value;
}
//****************************************************************************80

void r8_swap ( double *x, double *y )

//****************************************************************************80
//
//  Purpose:
//
//    R8_SWAP switches two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, double *X, *Y.  On output, the values of X and
//    Y have been interchanged.
//
{
  double z;

  z = *x;
  *x = *y;
  *y = z;
 
  return;
}
//****************************************************************************80

void r8_swap3 ( double *x, double *y, double *z )

//****************************************************************************80
//
//  Purpose:
//
//    R8_SWAP3 swaps three R8's.
//
//  Example:
//
//    Input:
//
//      X = 1, Y = 2, Z = 3
//
//    Output:
//
//      X = 2, Y = 3, Z = 1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, double *X, *Y, *Z, three values to be swapped.
//
{
  double w;

   w = *x;
  *x = *y;
  *y = *z;
  *z =  w;

  return;
}
//****************************************************************************80

double r8_tiny ( )

//****************************************************************************80
//
//  Purpose:
//
//    R8_TINY returns a "tiny" R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 March 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, float R8_TINY, a "tiny" R8 value.
//
{
  float value;

  value = 0.4450147717014E-307;

  return value;
}
//****************************************************************************80

double r8_to_r8_discrete ( double r, double rmin, double rmax, int nr )

//****************************************************************************80
//
//  Purpose:
//
//    R8_TO_R8_DISCRETE maps R to RD in [RMIN, RMAX] with NR possible values.
//
//  Formula:
//
//    if ( R < RMIN ) then
//      RD = RMIN
//    else if ( RMAX < R ) then
//      RD = RMAX
//    else
//      T = nint ( ( NR - 1 ) * ( R - RMIN ) / ( RMAX - RMIN ) )
//      RD = RMIN + T * ( RMAX - RMIN ) / real ( NR - 1 )
//
//    In the special case where NR = 1, when
//
//      XD = 0.5 * ( RMAX + RMIN )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, the number to be converted.
//
//    Input, double RMAX, RMIN, the maximum and minimum
//    values for RD.
//
//    Input, int NR, the number of allowed values for XD.
//    NR should be at least 1.
//
//    Output, double RD, the corresponding discrete value.
//
{
  int f;
  double rd;
//
//  Check for errors.
//
  if ( nr < 1 )
  {
    cout << "\n";
    cout << "R8_TO_R8_DISCRETE - Fatal error!\n";
    cout << "  NR = " << nr << "\n";
    cout << "  but NR must be at least 1.\n";
    exit ( 1 );
  }

  if ( nr == 1 )
  {
    rd = 0.5 * ( rmin + rmax );
    return rd;
  }

  if ( rmax == rmin )
  {
    rd = rmax;
    return rd;
  }

  f = r8_nint ( ( double ) ( nr ) * ( rmax - r ) / ( rmax - rmin ) );
  f = i4_max ( f, 0 );
  f = i4_min ( f, nr );

  rd = ( ( double ) (      f ) * rmin   
       + ( double ) ( nr - f ) * rmax ) 
       / ( double ) ( nr     );

  return rd;
}
//****************************************************************************80

void r8_to_dhms ( double r, int *d, int *h, int *m, int *s )

//****************************************************************************80
//
//  Purpose:
//
//    R8_TO_DHMS converts an R8 day value into days, hours, minutes, seconds.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, a real number representing a time period measured in days.
//
//    Output, int D, H, M, S, the equivalent number of days, hours,
//    minutes and seconds.
//
{
  int sign;

  if ( 0.0 <= r )
  {
    sign = 1;
  }
  else if ( r < 0.0 ) 
  {
    sign = -1;
    r = -r;
  }

  *d = ( int ) r;

  r = r - ( double ) *d;
  r = 24.0 * r;
  *h = ( int ) r;

  r = r - ( double ) *h;
  r = 60.0 * r;
  *m = ( int ) r;

  r = r - ( double ) *m;
  r = 60.0 * r;
  *s = ( int ) r;

  if ( sign == -1 )
  {
    *d = -(*d);
    *h = -(*h);
    *m = -(*m);
    *s = -(*s);
  }

  return;
}
//****************************************************************************80

int r8_to_i4 ( double x, double xmin, double xmax, int ixmin, int ixmax )

//****************************************************************************80
//
//  Purpose:
//
//    R8_TO_I4 maps real X in [XMIN, XMAX] to integer IX in [IXMIN, IXMAX].
//
//  Formula:
//
//    IX := IXMIN + ( IXMAX - IXMIN ) * ( X - XMIN ) / ( XMAX - XMIN )
//    IX := min ( IX, max ( IXMIN, IXMAX ) )
//    IX := max ( IX, min ( IXMIN, IXMAX ) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the real number to be converted.
//
//    Input, double XMIN, XMAX, the real range.  XMAX and XMIN must not be
//    equal.  It is not necessary that XMIN be less than XMAX.
//
//    Input, int IXMIN, IXMAX, the allowed range of the output
//    variable.  IXMAX corresponds to XMAX, and IXMIN to XMIN.
//    It is not necessary that IXMIN be less than IXMAX.
//
//    Output, int R8_TO_I4, the value in the range [IXMIN,IXMAX] that
//    corresponds to X.
//
{
  int ix;
  double temp;

  if ( xmax == xmin )
  {
    cout << "\n";
    cout << "R8_TO_I4 - Fatal error!\n";
    cout << "  XMAX = XMIN, making a zero divisor.\n";
    cout << "  XMAX = " << xmax << "\n";
    cout << "  XMIN = " << xmin << "\n";
    exit ( 1 );
  }

  temp =
      ( ( xmax - x        ) * ( double ) ixmin  
      + (        x - xmin ) * ( double ) ixmax )
      / ( xmax     - xmin );

  if ( 0.0 <= temp )
  {
    temp = temp + 0.5;
  }
  else
  {
    temp = temp - 0.5;
  }

  ix = ( int ) temp;

  return ix;
}
//****************************************************************************80

double r8_uniform ( double b, double c, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8_UNIFORM returns a scaled pseudorandom R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double B, C, the minimum and maximum values.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8_UNIFORM, the randomly chosen value.
//
{
  double t;

  t = r8_uniform_01 ( seed );

  t = ( 1.0 - t ) * b + t * c;

  return t;
}
//****************************************************************************80

double r8_uniform_01 ( int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8_UNIFORM_01 is a unit pseudorandom R8.
//
//  Discussion:
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      unif = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2004
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//  Parameters:
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8_UNIFORM_01, a new pseudorandom variate, strictly between
//    0 and 1.
//
{
  int k;
  double r;

  k = *seed / 127773;

  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

  if ( *seed < 0 )
  {
    *seed = *seed + 2147483647;
  }

  r = ( double ) ( *seed ) * 4.656612875E-10;

  return r;
}
//****************************************************************************80

void r8_unswap3 ( double *x, double *y, double *z )

//****************************************************************************80
//
//  Purpose:
//
//    R8_UNSWAP3 unswaps three real items.
//
//  Example:
//
//    Input:
//
//      X = 2, Y = 3, Z = 1
//
//    Output:
//
//      X = 1, Y = 2, Z = 3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, double *X, *Y, *Z, three values to be swapped.
//
{
  double w;

   w = *z;
  *z = *y;
  *y = *x;
  *x =  w;

  return;
}
//****************************************************************************80

double r8_walsh_1d ( double x, int digit )

//****************************************************************************80
//
//  Purpose:
//
//    R8_WALSH_1D evaluates the Walsh function of a real scalar argument.
//
//  Discussion:
//
//    Consider the binary representation of X, and number the digits 
//    in descending order, from leading to lowest, with the units digit
//    being numbered 0.
//
//    The Walsh function W(J)(X) is equal to the J-th binary digit of X.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the argument of the Walsh function.
//
//    Input, int DIGIT, the index of the Walsh function.
//
//    Output, double R8_WALSH_1D, the value of the Walsh function.
//
{
  int n;
  double value;
//
//  Hide the effect of the sign of X.
//
  x = r8_abs ( x );
//
//  If DIGIT is positive, divide by 2 DIGIT times.
//  If DIGIT is negative, multiply by 2 (-DIGIT) times.
//
  x = x / pow ( 2.0, digit );
//
//  Make it an integer.
//  Because it's positive, and we're using INT, we don't change the
//  units digit.
//
  n = ( int ) x;
//
//  Is the units digit odd or even?
//
  if ( ( n % 2 ) == 0 )
  {
    value = 0.0;
  }
  else
  {
    value = 1.0;
  }

  return value;
}
//****************************************************************************80

double *r82_cheby ( int n, double alo, double ahi )

//****************************************************************************80
//
//  Purpose:
//
//    R82_CHEBY sets up the Chebyshev abscissas in a double precision interval.
//
//  Discussion:
//
//    The routine sets up a vector of X values spaced between the values
//    XLO and XHI in a similar way to the spacing of the Chebyshev
//    points of the same order in the interval [-1,1].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of points to compute.
//
//    Input, double ALO, AHI, the range.
//
//    Output, double R82_CHEBY[N], the computed X values.
//
{
  double *a;
  double arg;
  int i;
  double pi = 3.141592653589793;

  a = new double[n];

  if ( n == 1 )
  {
    a[0] = 0.5 * ( alo + ahi );
  }
  else if ( 1 < n )
  {
    for ( i = 0; i < n; i++ )
    {
      arg = ( double ) ( 2 * i + 1 ) * pi / ( double ) ( 2 * n );

      a[i] = 0.5 * ( ( 1.0 + cos ( arg ) ) * alo 
                   + ( 1.0 - cos ( arg ) ) * ahi );

    }
  }

  return a;
}
//****************************************************************************80

void r82_print ( double a[2], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R82_PRINT prints an R82.
//
//  Discussion:
//
//    An R82 is a vector of type double precision, with two entries.
//
//    A format is used which suggests a coordinate pair:
//
//  Example:
//
//    Center : ( 1.23, 7.45 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[2], the coordinates of the vector.
//
//    Input, char *TITLE, an optional title.
//
{
  if ( s_len_trim ( title ) == 0 )
  {
    cout << "  ( " << setw(12) << a[0]
         << ", "   << setw(12) << a[1] << " )\n";
  }
  else
  {
    cout << "  " << title << " : "
         << "  ( " << setw(12) << a[0]
         << ", "   << setw(12) << a[1] << " )\n";
  }

  return;
}
//****************************************************************************80

void r82_uniform ( double b, double c, int *seed, double r[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82_UNIFORM returns a random R82 value in a given range.
//
//  Discussion:
//
//    An R82 is a vector of type double precision with two entries.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double B, C, the minimum and maximum values.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R[2], the randomly chosen value.
//
{
  int i;

  for ( i = 0; i < 2; i++ )
  {
    r[i] = r8_uniform ( b, c, seed );
  }

  return;
}
//****************************************************************************80

void r82poly2_print ( double a, double b, double c, double d, double e, 
  double f )

//****************************************************************************80
//
//  Purpose:
//
//    R82POLY2_PRINT prints a second order polynomial in two variables.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A, B, C, D, E, F, the coefficients.
//
{
  cout << "  " << setw(8) << a
       << " * x^2 + " << setw(8) << b
       << " * y^2 + " << setw(8) << c
       << " * xy  + " << "\n";
  cout << "  " << setw(8) << d
       << " * x   + " << setw(8) << e
       << " * y   + " << setw(8) << f << "\n";

  return;
}
//****************************************************************************80

int r82poly2_type ( double a, double b, double c, double d, double e, double f )

//****************************************************************************80
//
//  Purpose:
//
//    R82POLY2_TYPE analyzes a second order polynomial in two variables.
//
//  Discussion:
//
//    The polynomial has the form
//
//      A x^2 + B y^2 + C xy + Dx + Ey + F = 0
//
//    The possible types of the solution set are:
//
//     1: a hyperbola;
//        9x^2 -  4y^2       -36x - 24y -  36 = 0
//     2: a parabola;
//        4x^2 +  1y^2 - 4xy + 3x -  4y +   1 = 0;
//     3: an ellipse;
//        9x^2 + 16y^2       +36x - 32y -  92 = 0;
//     4: an imaginary ellipse (no real solutions);
//         x^2 +   y^2       - 6x - 10y + 115 = 0;
//     5: a pair of intersecting lines;
//                        xy + 3x -   y -   3 = 0
//     6: one point;
//         x^2 +  2y^2       - 2x + 16y +  33 = 0;
//     7: a pair of distinct parallel lines;
//                 y^2            -  6y +   8 = 0
//     8: a pair of imaginary parallel lines (no real solutions);
//                 y^2            -  6y +  10 = 0
//     9: a pair of coincident lines.
//                 y^2            -  2y +   1 = 0
//    10: a single line;
//                             2x -   y +   1 = 0;
//    11; all space;
//                                          0 = 0;
//    12; no solutions;
//                                          1 = 0;
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Daniel Zwillinger, editor,
//    CRC Standard Mathematical Tables and Formulae,
//    CRC Press, 30th Edition, 1996, pages 282-284.
//
//  Parameters:
//
//    Input, double A, B, C, D, E, F, the coefficients.
//
//    Output, int TYPE, indicates the type of the solution set.
//
{
  double delta;
  double j;
  double k;
  int type;
//
//  Handle the degenerate case.
//
  if ( a == 0.0 && b == 0.0 && c == 0.0 )
  {
    if ( d == 0.0 && e == 0.0 )
    {
      if ( f == 0.0 )
      {
        type = 11;
      }
      else
      {
        type = 12;
      }
    }
    else
    {
      type = 10;
    }
    return type;
  }

  delta = 
      8.0 * a * b * f 
    + 2.0 * c * e * d 
    - 2.0 * a * e * e 
    - 2.0 * b * d * d 
    - 2.0 * f * c * c;

  j = 4.0 * a * b - c * c;

  if ( delta != 0.0 )
  {
    if ( j < 0.0 )
    {
      type = 1;
    }
    else if ( j == 0.0 )
    {
      type = 2;
    }
    else if ( 0.0 < j )
    {
      if ( r8_sign ( delta ) != r8_sign ( a + b ) )
      {
        type = 3;
      }
      else if ( r8_sign ( delta ) == r8_sign ( a + b ) )
      {
        type = 4;
      }
    }
  }
  else if ( delta == 0.0 )
  {
    if ( j < 0.0 )
    {
      type = 5;
    }
    else if ( 0.0 < j )
    {
      type = 6;
    }
    else if ( j == 0.0 )
    {
      k = 4.0 * ( a + b ) * f - d * d - e * e;

      if ( k < 0.0 )
      {
        type = 7;
      }
      else if ( 0.0 < k )
      {
        type = 8;
      }
      else if ( k == 0.0 )
      {
        type = 9;
      }
    }
  }

  return type;
}
//****************************************************************************80

void r82poly2_type_print ( int type )

//****************************************************************************80
//
//  Purpose:
//
//    R82POLY2_TYPE_PRINT prints the meaning of the output from R82POLY2_TYPE.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int TYPE, the type index returned by R82POLY2_TYPE.
//
{
  if ( type == 1 )
  {
    cout << "  The set of solutions forms a hyperbola.\n";
  }
  else if ( type == 2 )
  {
    cout << "  The set of solutions forms a parabola.\n";
  }
  else if ( type == 3 )
  {
    cout << "  The set of solutions forms an ellipse.\n";
  }
  else if ( type == 4 )
  {
    cout << "  The set of solutions forms an imaginary ellipse.\n";
    cout << "  (There are no real solutions).\n";
  }
  else if ( type == 5 )
  {
    cout << "  The set of solutions forms a pair of intersecting lines.\n";
  }
  else if ( type == 6 )
  {
    cout << "  The set of solutions is a single point.\n";
  }
  else if ( type == 7 )
  {
    cout << "  The set of solutions form a pair of distinct parallel lines.\n";
  }
  else if ( type == 8 )
  {
    cout << "  The set of solutions forms a pair of imaginary parallel lines.\n";
    cout << "  (There are no real solutions).\n";
  }
  else if ( type == 9 )
  {
    cout << "  The set of solutions forms a pair of coincident lines.\n";
  }
  else if ( type == 10 )
  {
    cout << "  The set of solutions forms a single line.\n";
  }
  else if ( type == 11 )
  {
    cout << "  The set of solutions is all space.\n";
  }
  else if ( type == 12 )
  {
    cout << "  The set of solutions is empty.\n";
  }
  else
  {
    cout << "  This type index is unknown.\n";
  }
  return;
}
//****************************************************************************80

double *r82vec_max ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_MAX returns the maximum value in an R82VEC.
//
//  Discussion:
//
//    An R82VEC is an array of pairs of double precision real values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 July 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[2*N], the array.
//
//    Output, double R82VEC_MAX[2]; the largest entries in each row.
//
{
# define DIM_NUM 2

  double *amax = NULL;
  int i;
  int j;

  if ( n <= 0 )
  {
    return NULL;
  }

  amax = new double[DIM_NUM];

  for ( i = 0; i < DIM_NUM; i++ )
  {
    amax[i] = a[i+0*DIM_NUM];
    for ( j = 1; j < n; j++ )
    {
      if ( amax[i] < a[0+j*DIM_NUM] )
      {
        amax[i] = a[0+j*DIM_NUM];
      }
    }
  }
  return amax;
# undef DIM_NUM
}
//****************************************************************************80

double *r82vec_min ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_MIN returns the minimum value in an R82VEC.
//
//  Discussion:
//
//    An R82VEC is an array of pairs of double precision real values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 July 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[2*N], the array.
//
//    Output, double R82VEC_MIN[2]; the smallest entries in each row.
//
{
# define DIM_NUM 2

  double *amin = NULL;
  int i;
  int j;

  if ( n <= 0 )
  {
    return NULL;
  }

  amin = new double[DIM_NUM];

  for ( i = 0; i < DIM_NUM; i++ )
  {
    amin[i] = a[i+0*DIM_NUM];
    for ( j = 1; j < n; j++ )
    {
      if ( a[0+j*DIM_NUM] < amin[i] )
      {
        amin[i] = a[0+j*DIM_NUM];
      }
    }
  }
  return amin;
# undef DIM_NUM
}
//****************************************************************************80

int r82vec_order_type ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_ORDER_TYPE finds if an R82VEC is (non)strictly ascending/descending.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//    The dictionary or lexicographic ordering is used.
//
//    (X1,Y1) < (X2,Y2)  <=>  X1 < X2 or ( X1 = X2 and Y1 < Y2).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the array.
//
//    Input, double A[2*N], the array to be checked.
//
//    Output, int R82VEC_ORDER_TYPE, order indicator:
//    -1, no discernable order;
//    0, all entries are equal;
//    1, ascending order;
//    2, strictly ascending order;
//    3, descending order;
//    4, strictly descending order.
//
{
  int i;
  int order;
//
//  Search for the first value not equal to A(1,1).
//
  i = 0;

  for ( ; ; ) 
  {
    i = i + 1;

    if ( n <= i )
    {
      order = 0;
      return order;
    }

    if ( a[0+0*2] < a[0+i*2] || ( a[0+0*2] == a[0+i*2] && a[1+0*2] < a[1+i*2] ) )
    {
      if ( i == 2 )
      {
        order = 2;
      }
      else
      {
        order = 1;
      }
      break;
    }
    else if ( a[0+i*2] < a[0+0*2] || ( a[0+i*2] == a[0+0*2] && a[1+i*2] < a[1+0*2] ) )
    {
      if ( i == 2 )
      {
        order = 4;
      }
      else
      {
        order = 3;
      }
      break;
    }
  }
//
//  Now we have a "direction".  Examine subsequent entries.
//
  for ( ; ; )
  {
    i = i + 1;
    if ( n <= i )
    {
      break;
    }

    if ( order == 1 )
    {
      if ( a[0+i*2] < a[0+(i-1)*2] ||
        ( a[0+i*2] == a[0+(i-1)*2] && a[1+i*2] < a[1+(i-1)*2] ) )
      {
        order = -1;
        break;
      }
    }
    else if ( order == 2 )
    {
      if ( a[0+i*2] < a[0+(i-1)*2] || 
        ( a[0+i*2] == a[0+(i-1)*2] && a[1+i*2] < a[1+(i-1)*2] ) )
      {
        order = -1;
        break;
      }
      else if ( a[0+i*2] == a[0+(i-1)*2] && a[1+i*2] == a[1+(i-1)*2] )
      {
        order = 1;
      }
    }
    else if ( order == 3 )
    {
      if ( a[0+(i-1)*2] < a[0+i*2] || 
        ( a[0+(i-1)*2] == a[0+i*2] && a[1+(i-1)*2] < a[1+i*2] ) )
      {
        order = -1;
        break;
      }
    }
    else if ( order == 4 )
    {
      if ( a[0+(i-1)*2] < a[0+i*2] ||
        ( a[0+(i-1)*2] == a[0+i*2] && a[1+(i-1)*2] < a[1+i*2] ) )
      {
        order = -1;
        break;
      }
      else if ( a[0+i*2] == a[0+(i-1)*2] && a[1+i*2] == a[1+(i-1)*2] )
      {
        order = 3;
      }
    }
  }
  return order;
}
//****************************************************************************80

void r82vec_part_quick_a ( int n, double a[], int *l, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_PART_QUICK_A reorders an R82VEC as part of a quick sort.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//    The routine reorders the entries of A.  Using A(1:2,1) as a
//    key, all entries of A that are less than or equal to the key will
//    precede the key, which precedes all entries that are greater than the key.
//
//  Example:
//
//    Input:
//
//      N = 8
//
//      A = ( (2,4), (8,8), (6,2), (0,2), (10,6), (10,0), (0,6), (4,8) )
//
//    Output:
//
//      L = 2, R = 4
//
//      A = ( (0,2), (0,6), (2,4), (8,8), (6,2), (10,6), (10,0), (4,8) )
//             -----------          ----------------------------------
//             LEFT          KEY    RIGHT
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, double A[N*2].  On input, the array to be checked.
//    On output, A has been reordered as described above.
//
//    Output, int *L, *R, the indices of A that define the three segments.
//    Let KEY = the input value of A(1:2,1).  Then
//    I <= L                 A(1:2,I) < KEY;
//         L < I < R         A(1:2,I) = KEY;
//                 R <= I    A(1:2,I) > KEY.
//
{
  int i;
  int j;
  double key[2];
  int ll;
  int m;
  int rr;
//
  if ( n < 1 )
  {
    cout << "\n";
    cout << "R82VEC_PART_QUICK_A - Fatal error!\n";
    cout << "  N < 1.\n";
    exit ( 1 );
  }

  if ( n == 1 )
  {
    *l = 0;
    *r = 2;
    return;
  }

  key[0] = a[2*0+0];
  key[1] = a[2*0+1];
  m = 1;
//
//  The elements of unknown size have indices between L+1 and R-1.
//
  ll = 1;
  rr = n + 1;

  for ( i = 2; i <= n; i++ )
  {
    if ( r8vec_gt ( 2, a+2*ll, key ) )
    {
      rr = rr - 1;
      r8vec_swap ( 2, a+2*(rr-1), a+2*ll );
    }
    else if ( r8vec_eq ( 2, a+2*ll, key ) )
    {
      m = m + 1;
      r8vec_swap ( 2, a+2*(m-1), a+2*ll );
      ll = ll + 1;
    }
    else if ( r8vec_lt ( 2, a+2*ll, key ) )
    {
      ll = ll + 1;
    }

  }
//
//  Now shift small elements to the left, and KEY elements to center.
//
  for ( i = 0; i < ll - m; i++ )
  {
    for ( j = 0; j < 2; j++ )
    {
      a[2*i+j] = a[2*(i+m)+j];
    }
  }

  ll = ll - m;

  for ( i = ll; i < ll+m; i++ )
  {
    for ( j = 0; j < 2; j++ )
    {
      a[2*i+j] = key[j];
    }
  }

  *l = ll;
  *r = rr;

  return;
}
//****************************************************************************80

void r82vec_permute ( int n, int p[], int base, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_PERMUTE permutes an R82VEC in place.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//    This routine permutes an array of real "objects", but the same
//    logic can be used to permute an array of objects of any arithmetic
//    type, or an array of objects of any complexity.  The only temporary
//    storage required is enough to store a single object.  The number
//    of data movements made is N + the number of cycles of order 2 or more,
//    which is never more than N + N/2.
//
//  Example:
//
//    Input:
//
//      N = 5
//      P = (   2,    4,    5,    1,    3 )
//      A = ( 1.0,  2.0,  3.0,  4.0,  5.0 )
//          (11.0, 22.0, 33.0, 44.0, 55.0 )
//
//    Output:
//
//      A    = (  2.0,  4.0,  5.0,  1.0,  3.0 )
//             ( 22.0, 44.0, 55.0, 11.0, 33.0 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int P[N], the permutation.  P(I) = J means
//    that the I-th element of the output array should be the J-th
//    element of the input array. 
//
//    Input, int BASE, is 0 for a 0-based permutation and 1 for a 1-based permutation.
//
//    Input/output, double A[2*N], the array to be permuted.
//
{
  double a_temp[2];
  int i;
  int iget;
  int iput;
  int istart;

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "R82VEC_PERMUTE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }
//
//  In order for the sign negation trick to work, we need to assume that the
//  entries of P are strictly positive.  Presumably, the lowest number is BASE.
//  So temporarily add 1-BASE to each entry to force positivity.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] + 1 - base;
  }
//
//  Search for the next element of the permutation that has not been used.
//
  for ( istart = 1; istart <= n; istart++ )
  {
    if ( p[istart-1] < 0 )
    {
      continue;
    }
    else if ( p[istart-1] == istart )
    {
      p[istart-1] = - p[istart-1];
      continue;
    }
    else
    {
      a_temp[0] = a[0+(istart-1)*2];
      a_temp[1] = a[1+(istart-1)*2];
      iget = istart;
//
//  Copy the new value into the vacated entry.
//
      for ( ; ; )
      {
        iput = iget;
        iget = p[iget-1];

        p[iput-1] = - p[iput-1];

        if ( iget < 1 || n < iget )
        {
          cout << "\n";
          cout << "R82VEC_PERMUTE - Fatal error!\n";
          cout << "  Entry IPUT = " << iput << " of the permutation has\n";
          cout << "  an illegal value IGET = " << iget << ".\n";
          exit ( 1 );
        }

        if ( iget == istart )
        {
          a[0+(iput-1)*2] = a_temp[0];
          a[1+(iput-1)*2] = a_temp[1];
          break;
        }
        a[0+(iput-1)*2] = a[0+(iget-1)*2];
        a[1+(iput-1)*2] = a[1+(iget-1)*2];
      }
    }
  }
//
//  Restore the signs of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = - p[i];
  }
//
//  Restore the base of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] - 1 + base;
  }
  return;
}
//****************************************************************************80

void r82vec_print ( int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_PRINT prints an R82VEC.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 November 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A[2*N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int j;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( j = 0; j < n; j++ ) 
  {
    cout << setw(6)  << j    << "  " 
         << setw(14) << a[0+j*2] << "  " 
         << setw(14) << a[1+j*2] << "\n";
  }

  return;
}
//****************************************************************************80

int *r82vec_sort_heap_index_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R82VEC.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(*,indx(*))
//
//    or explicitly, by the call
//
//      r82vec_permute ( n, indx, 0, a )
//
//    after which a(*,*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[2*N], an array to be index-sorted.
//
//    Output, int R82VEC_SORT_HEAP_INDEX_A[N], the sort index.  The
//    I-th element of the sorted array is A(0:1,R8VEC_SORT_HEAP_INDEX_A(I)).
//
{
  double aval[2];
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval[0] = a[0+indxt*2];
      aval[1] = a[1+indxt*2];
    }
    else
    {
      indxt = indx[ir-1];
      aval[0] = a[0+indxt*2];
      aval[1] = a[1+indxt*2];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }
    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if (   a[0+indx[j-1]*2] <  a[0+indx[j]*2] ||
             ( a[0+indx[j-1]*2] == a[0+indx[j]*2] &&
               a[1+indx[j-1]*2] <  a[1+indx[j]*2] ) )
        {
          j = j + 1;
        }
      }

      if (   aval[0] <  a[0+indx[j-1]*2] ||
           ( aval[0] == a[0+indx[j-1]*2] &&
             aval[1] <  a[1+indx[j-1]*2] ) )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

void r82vec_sort_quick_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R82VEC_SORT_QUICK_A ascending sorts an R82VEC using quick sort.
//
//  Discussion:
//
//    An R82VEC is a vector whose entries are R82's.
//    An R82 is a vector of type double precision with two entries.
//    An R82VEC may be stored as a 2 by N array.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N*2].
//    On input, the array to be sorted.
//    On output, the array has been sorted.
//
{
# define LEVEL_MAX 30

  int base;
  int l_segment;
  int level;
  int n_segment;
  int rsave[LEVEL_MAX];
  int r_segment;

  if ( n < 1 )
  {
    cerr << "\n";
    cerr << "R82VEC_SORT_QUICK_A - Fatal error!\n";
    cerr << "  N < 1.\n";
    exit ( 1 );
  }

  if ( n == 1 )
  {
    return;
  }

  level = 1;
  rsave[level-1] = n + 1;
  base = 1;
  n_segment = n;

  while ( 0 < n_segment )
  {
//
//  Partition the segment.
//
    r82vec_part_quick_a ( n_segment, a+2*(base-1)+0, &l_segment, &r_segment );
//
//  If the left segment has more than one element, we need to partition it.
//
    if ( 1 < l_segment )
    {
      if ( LEVEL_MAX < level )
      {
        cerr << "\n";
        cerr<< "R82VEC_SORT_QUICK_A - Fatal error!\n";
        cerr << "  Exceeding recursion maximum of " << LEVEL_MAX << "\n";
        exit ( 1 );
      }

      level = level + 1;
      n_segment = l_segment;
      rsave[level-1] = r_segment + base - 1;
    }
//
//  The left segment and the middle segment are sorted.
//  Must the right segment be partitioned?
//
    else if ( r_segment < n_segment )
    {
      n_segment = n_segment + 1 - r_segment;
      base = base + r_segment - 1;
    }
//
//  Otherwise, we back up a level if there is an earlier one.
//
    else
    {
      for ( ; ; )
      {
        if ( level <= 1 )
        {
          n_segment = 0;
          break;
        }

        base = rsave[level-1];
        n_segment = rsave[level-2] - rsave[level-1];
        level = level - 1;

        if ( 0 < n_segment )
        {
          break;
        }
      }
    }
  }
  return;
# undef LEVEL_MAX
}
//****************************************************************************80

double *r83vec_max ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R83VEC_MAX returns the maximum value in an R83VEC.
//
//  Discussion:
//
//    An R83VEC is an array of triples of double precision real values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 January 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[3*N], the array.
//
//    Output, double R83VEC_MAX[3]; the largest entries in each row.
//
{
# define DIM_NUM 3

  double *amax = NULL;
  int i;
  int j;

  if ( n <= 0 )
  {
    return NULL;
  }

  amax = new double[DIM_NUM];

  for ( i = 0; i < DIM_NUM; i++ )
  {
    amax[i] = a[i+0*DIM_NUM];
    for ( j = 1; j < n; j++ )
    {
      if ( amax[i] < a[i+j*DIM_NUM] )
      {
        amax[i] = a[i+j*DIM_NUM];
      }
    }
  }
  return amax;
# undef DIM_NUM
}
//****************************************************************************80

double *r83vec_min ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R83VEC_MIN returns the minimum value in an R83VEC.
//
//  Discussion:
//
//    An R83VEC is an array of triples of double precision real values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 January 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[3*N], the array.
//
//    Output, double R83VEC_MIN[3]; the smallest entries in each row.
//
{
# define DIM_NUM 3

  double *amin = NULL;
  int i;
  int j;

  if ( n <= 0 )
  {
    return NULL;
  }

  amin = new double[DIM_NUM];

  for ( i = 0; i < DIM_NUM; i++ )
  {
    amin[i] = a[i+0*DIM_NUM];
    for ( j = 1; j < n; j++ )
    {
      if ( a[i+j*DIM_NUM] < amin[i] )
      {
        amin[i] = a[i+j*DIM_NUM];
      }
    }
  }
  return amin;
# undef DIM_NUM
}
//****************************************************************************80

void r83vec_part_quick_a ( int n, double a[], int *l, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    R83VEC_PART_QUICK_A reorders an R83VEC as part of a quick sort.
//
//  Discussion:
//
//    An R83VEC is a vector whose entries are R83's.
//    An R83 is a vector of type double precision with three entries.
//    An R83VEC may be stored as a 3 by N array.
//
//    The routine reorders the entries of A.  Using A(1:3,1) as a
//    key, all entries of A that are less than or equal to the key will
//    precede the key, which precedes all entries that are greater than the key.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, double A[3*N].  On input, the array to be checked.
//    On output, A has been reordered as described above.
//
//    Output, int *L, *R, the indices of A that define the three segments.
//    Let KEY = the input value of A(1:3,1).  Then
//    I <= L                 A(1:3,I) < KEY;
//         L < I < R         A(1:3,I) = KEY;
//                 R <= I    A(1:3,I) > KEY.
//
{
  int i;
  int j;
  double key[3];
  int ll;
  int m;
  int rr;

  if ( n < 1 )
  {
    cout << "\n";
    cout << "R83VEC_PART_QUICK_A - Fatal error!\n";
    cout << "  N < 1.\n";
    exit ( 1 );
  }

  if ( n == 1 )
  {
    *l = 0;
    *r = 2;
    return;
  }

  key[0] = a[3*0+0];
  key[1] = a[3*0+1];
  key[2] = a[3*0+2];
  m = 1;
//
//  The elements of unknown size have indices between L+1 and R-1.
//
  ll = 1;
  rr = n + 1;

  for ( i = 2; i <= n; i++ )
  {
    if ( r8vec_gt ( 3, a+3*ll, key ) )
    {
      rr = rr - 1;
      r8vec_swap ( 3, a+3*(rr-1), a+3*ll );
    }
    else if ( r8vec_eq ( 3, a+3*ll, key ) )
    {
      m = m + 1;
      r8vec_swap ( 3, a+3*(m-1), a+3*ll );
      ll = ll + 1;
    }
    else if ( r8vec_lt ( 3, a+3*ll, key ) )
    {
      ll = ll + 1;
    }
  }
//
//  Now shift small elements to the left, and KEY elements to center.
//
  for ( i = 0; i < ll - m; i++ )
  {
    for ( j = 0; j < 3; j++ )
    {
      a[3*i+j] = a[3*(i+m)+j];
    }
  }

  ll = ll - m;

  for ( i = ll; i < ll+m; i++ )
  {
    for ( j = 0; j < 3; j++ )
    {
      a[3*i+j] = key[j];
    }
  }

  *l = ll;
  *r = rr;

  return;
}
//****************************************************************************80

void r83vec_sort_quick_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R83VEC_SORT_QUICK_A ascending sorts an R83VEC using quick sort.
//
//  Discussion:
//
//    An R83VEC is a vector whose entries are R83's.
//    An R83 is a vector of type double precision with three entries.
//    An R83VEC may be stored as a 3 by N array.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N*3].
//    On input, the array to be sorted.
//    On output, the array has been sorted.
//
{
# define LEVEL_MAX 30

  int base;
  int l_segment;
  int level;
  int n_segment;
  int rsave[LEVEL_MAX];
  int r_segment;

  if ( n < 1 )
  {
    cerr << "\n";
    cerr << "R83VEC_SORT_QUICK_A - Fatal error!\n";
    cerr << "  N < 1.\n";
    exit ( 1 );
  }

  if ( n == 1 )
  {
    return;
  }

  level = 1;
  rsave[level-1] = n + 1;
  base = 1;
  n_segment = n;

  while ( 0 < n_segment )
  {
//
//  Partition the segment.
//
    r83vec_part_quick_a ( n_segment, a+3*(base-1)+0, &l_segment, &r_segment );
//
//  If the left segment has more than one element, we need to partition it.
//
    if ( 1 < l_segment )
    {
      if ( LEVEL_MAX < level )
      {
        cerr << "\n";
        cerr << "R83VEC_SORT_QUICK_A - Fatal error!\n";
        cerr << "  Exceeding recursion maximum of " << LEVEL_MAX << "\n";
        exit ( 1 );
      }
      level = level + 1;
      n_segment = l_segment;
      rsave[level-1] = r_segment + base - 1;
    }
//
//  The left segment and the middle segment are sorted.
//  Must the right segment be partitioned?
//
    else if ( r_segment < n_segment )
    {
      n_segment = n_segment + 1 - r_segment;
      base = base + r_segment - 1;
    }
//
//  Otherwise, we back up a level if there is an earlier one.
//
    else
    {
      for ( ; ; )
      {
        if ( level <= 1 )
        {
          n_segment = 0;
          break;
        }

        base = rsave[level-1];
        n_segment = rsave[level-2] - rsave[level-1];
        level = level - 1;

        if ( 0 < n_segment )
        {
          break;
        }
      }
    }
  }
  return;
# undef LEVEL_MAX
}
//****************************************************************************80

double *r8block_expand_linear ( int l, int m, int n, double x[], int lfat, 
  int mfat, int nfat )

//****************************************************************************80
//
//  Purpose:
//
//    R8BLOCK_EXPAND_LINEAR linearly interpolates new data into a 3D block.
//
//  Discussion:
//
//    In this routine, the expansion is specified by giving the number
//    of intermediate values to generate between each pair of original
//    data rows and columns.
//
//    The interpolation is not actually linear.  It uses the functions
//
//      1, x, y, z, xy, xz, yz, xyz.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int L, M, N, the dimensions of the input data.
//
//    Input, double X[L*M*N], the original data.
//
//    Input, int LFAT, MFAT, NFAT, the number of data values to interpolate
//    original data values in the first, second and third dimensions.
//
//    Output, double XFAT[L2*M2*N2], the fattened data, where
//    L2 = (L-1)*(LFAT+1)+1,
//    M2 = (M-1)*(MFAT+1)+1,
//    N2 = (N-1)*(NFAT+1)+1.
//
{
  int i;
  int ihi;
  int ii;
  int iii;
  int ip1;
  int j;
  int jhi;
  int jj;
  int jjj;
  int jp1;
  int k;
  int khi;
  int kk;
  int kkk;
  int kp1;
  int l2;
  int m2;
  int n2;
  double r;
  double s;
  double t;
  double x000;
  double x001;
  double x010;
  double x011;
  double x100;
  double x101;
  double x110;
  double x111;
  double *xfat;

  l2 = ( l - 1 ) * ( lfat + 1 ) + 1;
  m2 = ( m - 1 ) * ( mfat + 1 ) + 1;
  n2 = ( n - 1 ) * ( nfat + 1 ) + 1;

  xfat = new double[l2*m2*n2];
  
  for ( i = 1; i <= l; i++ )
  {
    if ( i < l )
    {
      ihi = lfat;
    }
    else
    {
      ihi = 0;
    }

    for ( j = 1; j <= m; j++ )
    {
      if ( j < m )
      {
        jhi = mfat;
      }
      else
      {
        jhi = 0;
      }

      for ( k = 1; k <= n; k++ )
      {
        if ( k < n )
        {
          khi = nfat;
        }
        else
        {
          khi = 0;
        }

        if ( i < l )
        {
          ip1 = i + 1;
        }
        else
        {
          ip1 = i;
        }

        if ( j < m )
        {
          jp1 = j + 1;
        }
        else
        {
          jp1 = j;
        }

        if ( k < n )
        {
          kp1 = k + 1;
        }
        else
        {
          kp1 = k;
        }

        x000 = x[i-1+(j-1)*l+(k-1)*l*m];
        x001 = x[i-1+(j-1)*l+(kp1-1)*l*m];
        x100 = x[ip1-1+(j-1)*l+(k-1)*l*m];
        x101 = x[ip1-1+(j-1)*l+(kp1-1)*l*m];
        x010 = x[i-1+(jp1-1)*l+(k-1)*l*m];
        x011 = x[i-1+(jp1-1)*l+(kp1-1)*l*m];
        x110 = x[ip1-1+(jp1-1)*l+(k-1)*l*m];
        x111 = x[ip1-1+(jp1-1)*l+(kp1-1)*l*m];

        for ( ii = 0; ii <= ihi; ii++ )
        {
          r = ( double ) ( ii ) / ( double ) ( ihi + 1 );

          for ( jj = 0; jj <= jhi; jj++ )
          {
            s = ( double ) ( jj ) / ( double ) ( jhi + 1 );

            for ( kk = 0; kk <= khi; kk++ )
            {
              t = ( double ) ( kk ) / ( double ) ( khi + 1 );

              iii = 1 + ( i - 1 ) * ( lfat + 1 ) + ii;
              jjj = 1 + ( j - 1 ) * ( mfat + 1 ) + jj;
              kkk = 1 + ( k - 1 ) * ( nfat + 1 ) + kk;

              xfat[iii-1+(jjj-1)*l2+(kkk-1)*l2*m2] = 
                  x000 * ( 1.0 - r ) * ( 1.0 - s ) * ( 1.0 - t ) 
                + x001 * ( 1.0 - r ) * ( 1.0 - s ) * (       t ) 
                + x010 * ( 1.0 - r ) * (       s ) * ( 1.0 - t ) 
                + x011 * ( 1.0 - r ) * (       s ) * (       t ) 
                + x100 * (       r ) * ( 1.0 - s ) * ( 1.0 - t ) 
                + x101 * (       r ) * ( 1.0 - s ) * (       t ) 
                + x110 * (       r ) * (       s ) * ( 1.0 - t ) 
                + x111 * (       r ) * (       s ) * (       t );
            }
          }
        }
      }
    }
  }

  return xfat;
}
//****************************************************************************80

void r8block_print ( int l, int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8BLOCK_PRINT prints a double precision block (a 3D matrix).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int L, M, N, the dimensions of the block.
//
//    Input, double A[L*M*N], the matrix to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;
  int j;
  int jhi;
  int jlo;
  int k;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  for ( k = 1; k <= n; k++ )
  {
    cout << "\n";
    cout << "  K = " << k << "\n";
    cout << "\n";
    for ( jlo = 1; jlo <= m; jlo = jlo + 5 )
    {
      jhi = i4_min ( jlo + 4, m );
      cout << "\n";
      cout << "      ";
      for ( j = jlo; j <= jhi; j++ )
      {
        cout << setw(7) << j << "       ";
      }
      cout << "\n";
      cout << "\n";
      for ( i = 1; i <= l; i++ )
      {
        cout << "  " << setw(4) << i;
        for ( j = jlo; j <= jhi; j++ )
        {
          cout << "  " << setw(12) << a[i-1+(j-1)*l+(k-1)*l*m];
        }
        cout << "\n";
      }
    }
  }

  return;
}
//****************************************************************************80

int r8col_compare ( int m, int n, double a[], int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_COMPARE compares two columns in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, I = 2, J = 4
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//    Output:
//
//      R8COL_COMPARE = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the M by N array.
//
//    Input, int I, J, the columns to be compared.
//    I and J must be between 1 and N.
//
//    Output, int R8COL_COMPARE, the results of the comparison:
//    -1, column I < column J,
//     0, column I = column J,
//    +1, column J < column I.
//
{
  int k;
  int value;
//
//  Check.
//
  if ( i < 1 || n < i )
  {
    cout << "\n";
    cout << "R8COL_COMPARE - Fatal error!\n";
    cout << "  Column index I is out of bounds.\n";
    cout << "  I = " << i << "\n";
    exit ( 1 );
  }

  if ( j < 1 || n < j )
  {
    cout << "\n";
    cout << "R8COL_COMPARE - Fatal error!\n";
    cout << "  Column index J is out of bounds.\n";
    cout << "  J = " << j << "\n";
    exit ( 1 );
  }

  value = 0;

  if ( i == j )
  {
    return value;
  }

  k = 0;

  while ( k < m )
  {
    if ( a[k+(i-1)*m] < a[k+(j-1)*m] )
    {
      value = -1;
      return value;
    }
    else if ( a[k+(j-1)*m] < a[k+(i-1)*m] )
    {
      value = +1;
      return value;
    }
    k = k + 1;
  }

  return value;
}
//****************************************************************************80

int r8col_find ( int m, int n, double a[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_FIND seeks a column value in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      M = 3,
//      N = 4,
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//      x = ( 3.,
//            7.,
//           11. )
//
//    Output:
//
//      R8COL_FIND = 3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], a table of numbers, regarded as
//    N columns of vectors of length M.
//
//    Input, double X[M], a vector to be matched with a column of A.
//
//    Output, int R8COL_FIND, the (one-based) index of the first column of A
//    which exactly matches every entry of X, or -1 if no match
//    could be found.  
//
{
  int col;
  int i;
  int j;

  col = -1;

  for ( j = 1; j <= n; j++ )
  {
    col = j;

    for ( i = 1; i <= m; i++ )
    {
      if ( x[i-1] != a[i-1+(j-1)*m] )
      {
        col = -1;
        break;
      }
    }
    if ( col != -1 )
    {
      return col;
    }
  }
  return col;
}
//****************************************************************************80

int *r8col_first_index ( int m, int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_FIRST_INDEX indexes the first occurrence of values in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    For element A(1:M,J) of the matrix, FIRST_INDEX(J) is the index in A of
//    the first column whose entries are equal to A(1:M,J).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//    The length of an "element" of A, and the number of "elements".
//
//    Input, double A[M*N], the array.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_FIRST_INDEX[N], the first occurrence index.
//
{
  double diff;
  int *first_index;
  int i;
  int j1;
  int j2;

  first_index = new int[n];

  for ( j1 = 0; j1 < n; j1++ )
  {
    first_index[j1] = -1;
  }
  for ( j1 = 0; j1 < n; j1++ )
  {
    if ( first_index[j1] == -1 )
    {
      first_index[j1] = j1;

      for ( j2 = j1 + 1; j2 < n; j2++ )
      {
        diff = 0.0;
        for ( i = 0; i < m; i++ )
        {
          diff = r8_max ( diff, r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
        }
        if ( diff <= tol )
        {
          first_index[j2] = j1;
        }
      }
    }
  }
  return first_index;
}
//****************************************************************************80

int r8col_insert ( int n_max, int m, int n, double a[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_INSERT inserts a column into an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      N_MAX = 10,
//      M = 3,
//      N = 4,
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//      X = ( 3., 4., 18. )
//
//    Output:
//
//      N = 5,
//
//      A = (
//        1.  2.  3.  3.  4.
//        5.  6.  4.  7.  8.
//        9. 10. 18. 11. 12. )
//
//      R8COL_INSERT = 3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N_MAX, the maximum number of columns in A.
//
//    Input, int M, the number of rows.
//
//    Input/output, int N, the number of columns.
//    If the new column is inserted into the table, then the output
//    value of N will be increased by 1.
//
//    Input/output, double A[M*N_MAX], a table of numbers, regarded
//    as an array of columns.  The columns must have been sorted
//    lexicographically.
//
//    Input, double X[M], a vector of data which will be inserted
//    into the table if it does not already occur.
//
//    Output, int R8COL_INSERT.
//    I, X was inserted into column I.
//    -I, column I was already equal to X.
//    0, N = N_MAX.
//
{
  int col;
  int high;
  int i;
  int isgn;
  int j;
  int low;
  int mid;
//
//  Refuse to work if N_MAX <= N.
//
  if ( n_max <= n )
  {
    col = 0;
    return col;
  }
//
//  Stick X temporarily in column N+1, just so it's easy to use R8COL_COMPARE.
//
  for ( i = 0; i < m; i++ )
  {
    a[i+n*m] = x[i];
  }
//
//  Do a binary search.
//
  low = 1;
  high = n;

  for ( ; ; )
  {
    if ( high < low )
    {
      col = low;
      break;
    }

    mid = ( low + high ) / 2;

    isgn = r8col_compare ( m, n+1, a, mid, n+1 );

    if ( isgn == 0 )
    {
      col = -mid;
      return col;
    }
    else if ( isgn == -1 )
    {
      low = mid + 1;
    }
    else if ( isgn == +1 )
    {
      high = mid - 1;
    }
  }
//
//  Shift part of the table up to make room.
//
  for ( j = n-1; col-1 <= j; j-- )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+(j+1)*m] = a[i+j*m];
    }
  }
//
//  Insert the new column.
//
  for ( i = 0; i < m; i++ )
  {
    a[i+(col-1)*m] = x[i];
  }

  n = n + 1;

  return col;
}
//****************************************************************************80

double *r8col_max ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_MAX returns the column maximums of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8COL_MAX[N], the maximums of the columns.
//
{
  double *amax;
  int i;
  int j;

  amax = new double[n];

  for ( j = 0; j < n; j++ )
  {
    amax[j] = a[0+j*m];
    for ( i = 0; i < m; i++ )
    {
      amax[j] = r8_max ( amax[j], a[i+j*m] );
    }
  }

  return amax;
}
//****************************************************************************80

int *r8col_max_index ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_MAX_INDEX returns the indices of column maximums in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, int R8COL_MAX_INDEX[N]; entry I is the row of A in which
//    the maximum for column I occurs.
//
{
  double amax;
  int i;
  int *imax;
  int j;

  imax = new int[n];

  for ( j = 0; j < n; j++ )
  {
    imax[j] = 1;
    amax = a[0+j*m];

    for ( i = 1; i < m; i++ )
    {
      if ( amax < a[i+j*m] )
      {
        imax[j] = i+1;
        amax = a[i+j*m];
      }
    }
  }

  return imax;
}
//****************************************************************************80

double *r8col_mean ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_MEAN returns the column means of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    R8COL_MEAN =
//      1.5  4.0  5.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8COL_MEAN[N], the means, or averages, of the columns.
//
{
  int i;
  int j;
  double *mean;

  mean = new double[n];

  for ( j = 0; j < n; j++ )
  {
    mean[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      mean[j] = mean[j] + a[i+j*m];
    }
    mean[j] = mean[j] / ( double ) ( m );
  }

  return mean;
}
//****************************************************************************80

double *r8col_min ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_MIN returns the column minimums of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8COL_MIN[N], the minimums of the columns.
//
{
  double *amin;
  int i;
  int j;

  amin = new double[n];

  for ( j = 0; j < n; j++ )
  {
    amin[j] = a[0+j*m];
    for ( i = 0; i < m; i++ )
    {
      amin[j] = r8_min ( amin[j], a[i+j*m] );
    }
  }

  return amin;
}
//****************************************************************************80

int *r8col_min_index ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_MIN_INDEX returns the indices of column minimums in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, int R8COL_MIN_INDEX[N]; entry I is the row of A in which
//    the minimum for column I occurs.
//
{
  double amin;
  int i;
  int *imin;
  int j;

  imin = new int[n];

  for ( j = 0; j < n; j++ )
  {
    imin[j] = 1;
    amin = a[0+j*m];

    for ( i = 1; i < m; i++ )
    {
      if ( a[i+j*m] < amin )
      {
        imin[j] = i+1;
        amin = a[i+j*m];
      }
    }
  }

  return imin;
}
//****************************************************************************80

void r8col_part_quick_a ( int m, int n, double a[], int *l, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_PART_QUICK_A reorders the columns of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The routine reorders the columns of A.  Using A(1:M,1) as a
//    key, all entries of A that are less than or equal to the key will
//    precede the key, which precedes all entries that are greater than the key.
//
//  Example:
//
//    Input:
//
//      M = 2, N = 8
//      A = ( 2  8  6  0 10 10  0  5
//            4  8  2  2  6  0  6  8 )
//
//    Output:
//
//      L = 2, R = 4
//
//      A = (  0  0  2  8  6 10 10  4
//             2  6  4  8  2  6  0  8 )
//             ----     -------------
//             LEFT KEY     RIGHT
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the row dimension of A, and the length of a column.
//
//    Input, int N, the column dimension of A.
//
//    Input/output, double A[M*N].  On input, the array to be checked.
//    On output, A has been reordered as described above.
//
//    Output, int *L, *R, the indices of A that define the three segments.
//    Let KEY = the input value of A(1:M,1).  Then
//    I <= L                 A(1:M,I) < KEY;
//         L < I < R         A(1:M,I) = KEY;
//                 R <= I    KEY < A(1:M,I).
//
{
  int i;
  int j;
  int k;
  double *key;

  if ( n < 1 )
  {
    cout << "\n";
    cout << "R8COL_PART_QUICK_A - Fatal error!\n";
    cout << "  N < 1.\n";
    *l = -1;
    *r = -1;
    return;
  }

  if ( n == 1 )
  {
    *l = 0;
    *r = 2;
    return;
  }

  key = new double[m];

  for ( i = 0; i < m; i++ )
  {
    key[i] = a[i+0*m];
  }
  k = 1;
//
//  The elements of unknown size have indices between L+1 and R-1.
//
  *l = 1;
  *r = n + 1;

  for ( j = 1; j < n; j++ )
  {
    if ( r8vec_gt ( m, a+(*l)*m, key ) )
    {
      *r = *r - 1;
      r8vec_swap ( m, a+(*r-1)*m, a+(*l)*m );
    }
    else if ( r8vec_eq ( m, a+(*l)*m, key ) )
    {
      k = k + 1;
      r8vec_swap ( m, a+(k-1)*m, a+(*l)*m );
      *l = *l + 1;
    }
    else if ( r8vec_lt ( m, a+(*l)*m, key ) )
    {
      *l = *l + 1;
    }
  }
//
//  Shift small elements to the left.
//
  for ( j = 0; j < *l - k; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = a[i+(j+k)*m];
    }
  }
//
//  Shift KEY elements to center.
//
  for ( j = *l-k; j < *l; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = key[i];
    }
  }
//
//  Update L.
//
  *l = *l - k;

  delete [] key;

  return;
}
//****************************************************************************80

void r8col_permute ( int m, int n, int p[], int base, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_PERMUTE permutes an R8COL in place.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    This routine permutes an array of real "objects", but the same
//    logic can be used to permute an array of objects of any arithmetic
//    type, or an array of objects of any complexity.  The only temporary
//    storage required is enough to store a single object.  The number
//    of data movements made is N + the number of cycles of order 2 or more,
//    which is never more than N + N/2.
//
//  Example:
//
//    Input:
//
//      M = 2
//      N = 5
//      P = (   2,    4,    5,    1,    3 )
//      A = ( 1.0,  2.0,  3.0,  4.0,  5.0 )
//          (11.0, 22.0, 33.0, 44.0, 55.0 )
//      BASE = 1
//
//    Output:
//
//      A    = (  2.0,  4.0,  5.0,  1.0,  3.0 )
//             ( 22.0, 44.0, 55.0, 11.0, 33.0 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 December 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the length of objects.
//
//    Input, int N, the number of objects.
//
//    Input, int P[N], the permutation.  P(I) = J means
//    that the I-th element of the output array should be the J-th
//    element of the input array.  
//
//    Input, int BASE, is 0 for a 0-based permutation and 1 for a 1-based permutation.
//
//    Input/output, double A[M*N], the array to be permuted.
//
{
  double *a_temp;
  int i;
  int iget;
  int iput;
  int istart;
  int j;

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "R8COL_PERMUTE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }
//
//  In order for the sign negation trick to work, we need to assume that the
//  entries of P are strictly positive.  Presumably, the lowest number is BASE.
//  So temporarily add 1-BASE to each entry to force positivity.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] + 1 - base;
  }

  a_temp = new double[m];
//
//  Search for the next element of the permutation that has not been used.
//
  for ( istart = 1; istart <= n; istart++ )
  {
    if ( p[istart-1] < 0 )
    {
      continue;
    }
    else if ( p[istart-1] == istart )
    {
      p[istart-1] = - p[istart-1];
      continue;
    }
    else
    {
      for ( i = 0; i < m; i++ )
      {
        a_temp[i] = a[i+(istart-1)*m];
      }
      iget = istart;
//
//  Copy the new value into the vacated entry.
//
      for ( ; ; )
      {
        iput = iget;
        iget = p[iget-1];

        p[iput-1] = - p[iput-1];

        if ( iget < 1 || n < iget )
        {
          cout << "\n";
          cout << "R8COL_PERMUTE - Fatal error!\n";
          cout << "  Entry IPUT = " << iput << " of the permutation has\n";
          cout << "  an illegal value IGET = " << iget << ".\n";
          exit ( 1 );
        }

        if ( iget == istart )
        {
          for ( i = 0; i < m; i++ )
          {
            a[i+(iput-1)*m] = a_temp[i];
          }
          break;
        }
        for ( i = 0; i < m; i++ )
        {
          a[i+(iput-1)*m] = a[i+(iget-1)*m];
        }
      }
    }
  }
//
//  Restore the signs of the entries.
//
  for ( j = 0; j < n; j++ )
  {
    p[j] = - p[j];
  }
//
//  Restore the base of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] - 1 +  base;
  }

  delete [] a_temp;

  return;
}
//****************************************************************************80

void r8col_sort_heap_a ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORT_HEAP_A ascending heapsorts an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    In lexicographic order, the statement "X < Y", applied to two real
//    vectors X and Y of length M, means that there is some index I, with
//    1 <= I <= M, with the property that
//
//      X(J) = Y(J) for J < I,
//    and
//      X(I) < Y(I).
//
//    In other words, the first time they differ, X is smaller.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N].
//    On input, the array of N columns of M-vectors.
//    On output, the columns of A have been sorted in lexicographic order.
//
{
  int i;
  int indx;
  int isgn;
  int j;

  if ( m <= 0 )
  {
    return;
  }

  if ( n <= 1 )
  {
    return;
  }
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      r8col_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = r8col_compare ( m, n, a, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

int *r8col_sort_heap_index_a ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    A(*,J1) < A(*,J2) if the first nonzero entry of A(*,J1)-A(*,J2) is negative.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      A(*,INDX(*)) is sorted,
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in each column of A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the array.
//
//    Output, int R8COL_SORT_HEAP_INDEX_A[N], contains the sort index.  The
//    I-th column of the sorted array is A(*,INDX(I)).
//
{
  double *column;
  int i;
  int *indx;
  int indxt;
  int ir;
  int isgn;
  int j;
  int k;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  column = new double[m];

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      for ( k = 0; k < m; k++ )
      {
        column[k] = a[k+indxt*m];
      }
    }
    else
    {
      indxt = indx[ir-1];
      for ( k = 0; k < m; k++ )
      {
        column[k] = a[k+indxt*m];
      }
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        isgn = r8vec_compare ( m, a+indx[j-1]*m, a+indx[j]*m );

        if ( isgn < 0 )
        {
          j = j + 1;
        }
      }

      isgn = r8vec_compare ( m, column, a+indx[j-1]*m );

      if ( isgn < 0 )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  delete [] column;

  return indx;
}
//****************************************************************************80

void r8col_sort_quick_a ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORT_QUICK_A ascending quick sorts an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the row order of A, and the length of a column.
//
//    Input, int N, the number of columns of A.
//
//    Input/output, double A[M*N].
//    On input, the array to be sorted.
//    On output, the array has been sorted.
//
{
# define LEVEL_MAX 30

  int base;
  int l_segment;
  int level;
  int n_segment;
  int rsave[LEVEL_MAX];
  int r_segment;

  if ( m <= 0 )
  {
    return;
  }

  if ( n < 1 )
  {
    cerr << "\n";
    cerr << "R8COL_SORT_QUICK_A - Fatal error!\n";
    cerr << "  N < 1.\n";
    cerr << "  N = " << n << "\n";
    exit ( 1 );
  }

  if ( n == 1 )
  {
    return;
  }

  level = 1;
  rsave[level-1] = n + 1;
  base = 1;
  n_segment = n;

  for ( ; ; )
  {
//
//  Partition the segment.
//
    r8col_part_quick_a ( m, n_segment, a+(base-1)*m, &l_segment, &r_segment );
//
//  If the left segment has more than one element, we need to partition it.
//
    if ( 1 < l_segment )
    {
      if ( LEVEL_MAX < level )
      {
        cerr << "\n";
        cerr << "R8COL_SORT_QUICK_A - Fatal error!\n";
        cerr << "  Exceeding recursion maximum of " << LEVEL_MAX << "\n";
        exit ( 1 );
      }

      level = level + 1;
      n_segment = l_segment;
      rsave[level-1] = r_segment + base - 1;
    }
//
//  The left segment and the middle segment are sorted.
//  Must the right segment be partitioned?
//
    else if ( r_segment < n_segment )
    {
      n_segment = n_segment + 1 - r_segment;
      base = base + r_segment - 1;
    }
//
//  Otherwise, we back up a level if there is an earlier one.
//
    else
    {
      for ( ; ; )
      {
        if ( level <= 1 )
        {
          return;
        }

        base = rsave[level-1];
        n_segment = rsave[level-2] - rsave[level-1];
        level = level - 1;

        if ( 0 < n_segment )
        {
          break;
        }
      }
    }
  }
  return;
# undef LEVEL_MAX
}
//****************************************************************************80

void r8col_sorted_undex ( int x_dim, int x_num, double x_val[], 
  int x_unique_num, double tol, int undx[], int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORTED_UNDEX returns unique sorted indexes for a sorted R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    Assuming X is already sorted, we examine the entries of X in order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(*) = X(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     X  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_DIM, the dimension of the data values.
//    (the number of rows in the R8COL).
//
//    Input, int X_NUM, the number of data values,
//    (the number of columns in the R8COL).
//
//    Input, double X_VAL[X_DIM*X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  double diff;
  int i;
  int j;
  int k;
//
//  Walk through the sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = i;

  xdnu[i] = j;

  for ( i = 1; i < x_num; i++ )
  {
    diff = 0.0;
    for ( k = 0; k < x_dim; k++ )
    {
      diff = r8_max ( diff, r8_abs ( x_val[k+i*x_dim] - x_val[k+undx[j]*x_dim] ) );
    }
    if ( tol < diff )
    {
      j = j + 1;
      undx[j] = i;
    }
    xdnu[i] = j;
  }

  return;
}
//****************************************************************************80

int r8col_sorted_unique ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORTED_UNIQUE keeps unique elements in a sorted R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The columns of the array can be ascending or descending sorted.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A(M,N).
//    On input, the sorted array of N columns of M-vectors.
//    On output, a sorted array of columns of M-vectors.
//
//    Output, int UNIQUE_NUM, the number of unique columns.
//
{
  bool any_ne;
  int i;
  int j1;
  int j2;
  int unique_num;

  if ( n <= 0 )
  {
    unique_num = 0;
    return unique_num;
  }

  j1 = 0;

  for ( j2 = 1; j2 < n; j2++ )
  {
    any_ne = false;
    for ( i = 0; i < m; i++ )
    {
      if ( a[i+j1*m] != a[i+j2*m] )
      {
        any_ne = true;
        break;
      }
    }
    if ( any_ne )
    {
      j1 = j1 + 1;
      for ( i = 0; i < m; i++ )
      {
        a[i+j1*m] = a[i+j2*m];
      }
    }
  }

  unique_num = j1;

  return unique_num;
}
//****************************************************************************80

int r8col_sorted_unique_count ( int m, int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORTED_UNIQUE_COUNT counts unique elements in a sorted R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The columns of the array may be ascending or descending sorted.
//
//    If the tolerance is large enough, then the concept of uniqueness
//    can become ambiguous.  If we have a tolerance of 1.5, then in the
//    list ( 1, 2, 3, 4, 5, 6, 7, 8, 9 ) is it fair to say we have only
//    one unique entry?  That would be because 1 may be regarded as unique,
//    and then 2 is too close to 1 to be unique, and 3 is too close to 2 to
//    be unique and so on.
//
//    This seems wrongheaded.  So I prefer the idea that an item is not
//    unique under a tolerance only if it is close to something that IS unique.
//    Thus, the unique items are guaranteed to cover the space if we include
//    a disk of radius TOL around each one.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], a sorted array, containing
//    N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_SORTED_UNIQUE_COUNT, the number of unique columns.
//
{
  double diff;
  int i;
  int j1;
  int j2;
  int unique_num;

  unique_num = 0;

  if ( n <= 0 )
  {
    return unique_num;
  }

  unique_num = 1;
  j1 = 0;

  for ( j2 = 1; j2 < n; j2++ )
  {
    diff = 0.0;
    for ( i = 0; i < m; i++ )
    {
      diff = r8_max ( diff,  r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
    }
    if ( tol < diff )
    {
      unique_num = unique_num + 1;
      j1 = j2;
    }
  }

  return unique_num;
}
//****************************************************************************80

void r8col_sortr_a ( int m, int n, double a[], int key )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SORTR_A ascending sorts one column of an R8COL, adjusting all entries.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N].
//    On input, an unsorted M by N array.
//    On output, rows of the array have been shifted in such
//    a way that column KEY of the array is in nondecreasing order.
//
//    Input, int KEY, the column in which the "key" value
//    is stored.  On output, column KEY of the array will be
//    in nondecreasing order.
//
{
  int i;
  int indx;
  int isgn;
  int j;

  if ( m <= 0 )
  {
    return;
  }

  if ( key < 1 || n < key )
  {
    cout << "\n";
    cout << "R8COL_SORTR_A - Fatal error!\n";
    cout << "  The value of KEY is not a legal column index.\n";
    cout << "  KEY = " << key << "\n";
    cout << "  N = " << n << "\n";
    exit ( 1 );
  }
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( m, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      r8row_swap ( m, n, a, i, j );
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      if ( a[i-1+(key-1)*m] < a[j-1+(key-1)*m] )
      {
        isgn = -1;
      }
      else
      {
        isgn = +1;
      }
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

double *r8col_sum ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SUM sums the columns of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8COL_SUM[N], the sums of the columns.
//
{
  double *colsum;
  int i;
  int j;

  colsum = new double[n];

  for ( j = 0; j < n; j++ )
  {
    colsum[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      colsum[j] = colsum[j] + a[i+j*m];
    }
  }
  return colsum;
}
//****************************************************************************80

void r8col_swap ( int m, int n, double a[], int j1, int j2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_SWAP swaps columns J1 and J2 of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Example:
//
//    Input:
//
//      M = 3, N = 4, J1 = 2, J2 = 4
//
//      A = (
//        1.  2.  3.  4.
//        5.  6.  7.  8.
//        9. 10. 11. 12. )
//
//    Output:
//
//      A = (
//        1.  4.  3.  2.
//        5.  8.  7.  6.
//        9. 12. 11. 10. )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N], the M by N array.
//
//    Input, int J1, J2, the columns to be swapped.
//    These columns are 1-based.
//
{
  int i;
  double temp;

  if ( j1 < 1 || n < j1 || j2 < 1 || n < j2 )
  {
    cout << "\n";
    cout << "R8COL_SWAP - Fatal error!\n";
    cout << "  J1 or J2 is out of bounds.\n";
    cout << "  J1 =   " << j1 << "\n";
    cout << "  J2 =   " << j2 << "\n";
    cout << "  NCOL = " << n << "\n";
    exit ( 1 );
  }

  if ( j1 == j2 )
  {
    return;
  }

  for ( i = 0; i < m; i++ )
  {
    temp          = a[i+(j1-1)*m];
    a[i+(j1-1)*m] = a[i+(j2-1)*m];
    a[i+(j2-1)*m] = temp;
  }

  return;
}
//****************************************************************************80

double *r8col_to_r8vec ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_TO_R8VEC converts an R8COL to an R8VEC.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    This routine is not really useful in our C++ implementation, since
//    we actually store an M by N matrix exactly as a vector already.
//
//  Example:
//
//    M = 3, N = 4
//
//    A =
//      11 12 13 14
//      21 22 23 24
//      31 32 33 34
//
//    R8COL_TO_R8VEC = ( 11, 21, 31, 12, 22, 32, 13, 23, 33, 14, 24, 34 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the M by N array.
//
//    Output, double X[M*N], a vector containing the N columns of A.
//
{
  int i;
  int j;
  int k;
  double *x;

  x = new double[m*n];

  k = 0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      x[k] = a[i+j*m];
      k = k + 1;
    }
  }

  return x;
}
//****************************************************************************80

void r8col_undex ( int x_dim, int x_num, double x_val[], int x_unique_num, 
  double tol, int undx[], int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_UNDEX returns unique sorted indexes for an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of X, which creates arrays INDX and XDNI.  (If all the entries
//    of X are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of X in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(*) = X(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     X  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_DIM, the dimension of the data values.
//    (the number of rows in the R8COL).
//
//    Input, int X_NUM, the number of data values,
//    (the number of columns in the R8COL).
//
//    Input, double X_VAL[X_DIM*X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  double diff;
  int i;
  int *indx;
  int j;
  int k;
//
//  Implicitly sort the array.
//
  indx = r8col_sort_heap_index_a ( x_dim, x_num, x_val );
//
//  Walk through the implicitly sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = indx[i];

  xdnu[indx[i]] = j;

  for ( i = 1; i < x_num; i++ )
  {
    diff = 0.0;
    for ( k = 0; k < x_dim; k++ )
    {
      diff = r8_max ( diff, r8_abs ( x_val[k+indx[i]*x_dim] - x_val[k+undx[j]*x_dim] ) );
    }
    if ( tol < diff )
    {
      j = j + 1;
      undx[j] = indx[i];
    }
    xdnu[indx[i]] = j;
  }
  delete [] indx;

  return;
}
//****************************************************************************80

int r8col_unique_count ( int m, int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_UNIQUE_COUNT counts unique columns in an unsorted R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    The columns of the array may be ascending or descending sorted.
//
//    If the tolerance is large enough, then the concept of uniqueness
//    can become ambiguous.  If we have a tolerance of 1.5, then in the
//    list ( 1, 2, 3, 4, 5, 6, 7, 8, 9 ) is it fair to say we have only
//    one unique entry?  That would be because 1 may be regarded as unique,
//    and then 2 is too close to 1 to be unique, and 3 is too close to 2 to
//    be unique and so on.
//
//    This seems wrongheaded.  So I prefer the idea that an item is not
//    unique under a tolerance only if it is close to something that IS unique.
//    Thus, the unique items are guaranteed to cover the space if we include
//    a disk of radius TOL around each one.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the array of N columns of data.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_UNIQUE_COUNT, the number of unique columns.
//
{
  double diff;
  int i;
  int j1;
  int j2;
  bool *unique;
  int unique_num;

  unique_num = 0;

  unique = new bool[n];

  for ( j1 = 0; j1 < n; j1++ )
  {
    unique_num = unique_num + 1;
    unique[j1] = true;

    for ( j2 = 0; j2 < j1; j2++ )
    {
      diff = 0.0;
      for ( i = 0; i < m; i++ )
      {
        diff = r8_max ( diff, r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
      }
      if ( diff <= tol )
      {
        unique_num = unique_num - 1;
        unique[j1] = false;
        break;
      }
    }
  }

  delete [] unique;

  return unique_num;
}
//****************************************************************************80

int *r8col_unique_index ( int m, int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_UNIQUE_INDEX indexes the unique occurrence of values in an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//    For element A(1:M,J) of the matrix, UNIQUE_INDEX(J) is the uniqueness index
//    of A(1:M,J).  That is, if A_UNIQUE contains the unique elements of A, 
//    gathered in order, then 
//
//      A_UNIQUE ( 1:M, UNIQUE_INDEX(J) ) = A(1:M,J)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//    The length of an "element" of A, and the number of "elements".
//
//    Input, double A[M*N], the array.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8COL_UNIQUE_INDEX[N], the unique index.
//
{
  double diff;
  int i;
  int j1;
  int j2;
  int *unique_index;
  int unique_num;

  unique_index = new int[n];

  for ( j1 = 0; j1 < n; j1++ )
  {
    unique_index[j1] = -1;
  }
  unique_num = 0;

  for ( j1 = 0; j1 < n; j1++ )
  {
    if ( unique_index[j1] == -1 )
    {
      unique_index[j1] = unique_num;

      for ( j2 = j1 + 1; j2 < n; j2++ )
      {
        diff = 0.0;
        for ( i = 0; i < m; i++ )
        {
          diff = r8_max ( diff, r8_abs ( a[i+j1*m] - a[i+j2*m] ) );
        }
        if ( diff <= tol )
        {
          unique_index[j2] = unique_num;
        }
      }
      unique_num = unique_num + 1;
    }
  }
  return unique_index;
}
//****************************************************************************80

double *r8col_variance ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8COL_VARIANCE returns the variances of an R8COL.
//
//  Discussion:
//
//    An R8COL is an M by N array of R8's, regarded as an array of N columns,
//    each of length M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, double A[M*N], the array whose variances are desired.
//
//    Output, double R8COL_VARIANCE[N], the variances of the rows.
//
{
  int i;
  int j;
  double mean;
  double *variance;

  variance = new double[n];

  for ( j = 0; j < n; j++ )
  {
    mean = 0.0;
    for ( i = 0; i < m; i++ )
    {
      mean = mean = a[i+j*m];
    }
    mean = mean / ( double ) ( m );

    variance[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      variance[j] = variance[j] + pow ( a[i+j*m] - mean, 2 );
    }

    if ( 1 < m )
    {
      variance[j] = variance[j] / ( double ) ( m - 1 );
    }
    else
    {
      variance[j] = 0.0;
    }
  }

  return variance;
}
//****************************************************************************80

int r8r8_compare ( double x1, double y1, double x2, double y2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8_COMPARE compares two R8R8's.
//
//  Discussion:
//
//    An R8R8 is simply a pair of R8 values, stored separately.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, the first vector.
//
//    Input, double X2, Y2, the second vector.
//
//    Output, int R8R8_COMPARE: 
//    -1, (X1,Y1) < (X2,Y2);
//     0, (X1,Y1) = (X2,Y2);
//    +1, (X1,Y1) > (X2,Y2).
//
{
  int value;

  if ( x1 < x2 )
  {
    value = -1;
  }
  else if ( x2 < x1 )
  {
    value = +1;
  }
  else if ( y1 < y2 )
  {
    value = -1;
  }
  else if ( y2 < y1 )
  {
    value = +1;
  }
  else
  {
    value = 0;
  }

  return value;
}
//****************************************************************************80

void r8r8_print ( double a1, double a2, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8_PRINT prints an R8R8.
//
//  Discussion:
//
//    An R8R8 is a pair of R8 values, regarded as a single item.
//
//    A format is used which suggests a coordinate pair:
//
//  Example:
//
//    Center : ( 1.23, 7.45 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A1, A2, the coordinates of the vector.
//
//    Input, char *TITLE, an optional title.
//
{
  if ( s_len_trim ( title ) == 0 )
  {
    cout << "  ( " << setw(12) << a1
         << ", "   << setw(12) << a2 << " )\n";
  }
  else
  {
    cout << "  " << title << " : "
         << "  ( " << setw(12) << a1
         << ", "   << setw(12) << a2 << " )\n";
  }

  return;
}
//****************************************************************************80

int r8r8r8_compare ( double x1, double y1, double z1, double x2, double y2, 
  double z2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8R8_COMPARE compares two R8R8R8's.
//
//  Discussion:
//
//    An R8R8R8 is simply 3 R8 values, stored as scalars.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, Z1, the first vector.
//
//    Input, double X2, Y2, Z2, the second vector.
//
//    Output, int R8R8R8_COMPARE: 
//    -1, (X1,Y1,Z1) < (X2,Y2,Z2);
//     0, (X1,Y1,Z1) = (X2,Y2,Z2);
//    +1, (X1,Y1,Z1) > (X2,Y2,Z2).
//
{
  int value;

  if ( x1 < x2 )
  {
    value = -1;
  }
  else if ( x2 < x1 )
  {
    value = +1;
  }
  else if ( y1 < y2 )
  {
    value = -1;
  }
  else if ( y2 < y1 )
  {
    value = +1;
  }
  else if ( z1 < z2 )
  {
    value = -1;
  }
  else if ( z2 < z1 )
  {
    value = +1;
  }
  else
  {
    value = 0;
  }

  return value;
}
//****************************************************************************80

void r8r8r8vec_index_insert_unique ( int maxn, int *n, double x[], double y[], 
  double z[], int indx[], double xval, double yval, double zval, int *ival, 
  int *ierror )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8R8VEC_INDEX_INSERT_UNIQUE inserts a unique R8R8R8 value in an indexed sorted list.
//
//  Discussion:
//
//    If the input value does not occur in the current list, it is added,
//    and N, X, Y, Z and INDX are updated.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int MAXN, the maximum size of the list.
//
//    Input/output, int *N, the size of the list.
//
//    Input/output, double X[N], Y[N], Z[N], the R8R8R8 vector.
//
//    Input/output, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, YVAL, ZVAL, the value to be inserted 
//    if it is not already in the list.
//
//    Output, int *IVAL, the index in X, Y, Z corresponding to the
//    value XVAL, YVAL, ZVAL.
//
//    Output, int *IERROR, 0 for no error, 1 if an error occurred.
//
{
  int equal;
  int i;
  int less;
  int more;

  *ierror = 0;

  if ( *n <= 0 )
  {
    if ( maxn <= 0 )
    {
      *ierror = 1;
      cout << "\n";
      cout << "R8R8R8VEC_INDEX_INSERT_UNIQUE - Fatal error!\n";
      cout << "  Not enough space to store new data.\n";
      return;
    }
    *n = 1;
    x[0] = xval;
    y[0] = yval;
    z[0] = zval;
    indx[0] = 1;
    *ival = 1;
    return;
  }
//
//  Does ( XVAL, YVAL, ZVAL ) already occur in ( X, Y, Z)?
//
  r8r8r8vec_index_search ( *n, x, y, z, indx, xval, yval, zval, 
    &less, &equal, &more );

  if ( equal == 0 )
  {
    if ( maxn <= *n )
    {
      *ierror = 1;
      cout << "\n";
      cout << "R8R8R8VEC_INDEX_INSERT_UNIQUE - Fatal error!\n";
      cout << "  Not enough space to store new data.\n";
      return;
    }

    x[*n] = xval;
    y[*n] = yval;
    z[*n] = zval;
    *ival = *n + 1;
    for ( i = *n-1; more-1 <= i; i-- )
    {
      indx[i+1] = indx[i];
    }
    indx[more-1] = *n + 1;
    *n = *n + 1;
  }
  else
  {
    *ival = indx[equal-1];
  }

  return;
}
//****************************************************************************80

void r8r8r8vec_index_search ( int n, double x[], double y[], double z[], 
  int indx[], double xval, double yval, double zval, int *less, int *equal, 
  int *more )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8R8VEC_INDEX_SEARCH searches for an R8R8R8 value in an indexed sorted list.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the list.
//
//    Input, double X[N], Y[N], Z[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, YVAL, ZVAL, the value to be sought.
//
//    Output, int *LESS, *EQUAL, *MORE, the indexes in INDX of the
//    entries of X that are just less than, equal to, and just greater
//    than XVAL.  If XVAL does not occur in X, then EQUAL is zero.
//    If XVAL is the minimum entry of X, then LESS is 0.  If XVAL
//    is the greatest entry of X, then MORE is N+1.
//
{
  int compare;
  int hi;
  int lo;
  int mid;
  double xhi;
  double xlo;
  double xmid;
  double yhi;
  double ylo;
  double ymid;
  double zhi;
  double zlo;
  double zmid;

  if ( n <= 0 )
  {
    *less = 0;
    *equal = 0;
    *more = 0;
    return;
  }

  lo = 1;
  hi = n;

  xlo = x[indx[lo-1]-1];
  ylo = y[indx[lo-1]-1];
  zlo = z[indx[lo-1]-1];

  xhi = x[indx[hi-1]-1];
  yhi = y[indx[hi-1]-1];
  zhi = z[indx[hi-1]-1];

  compare = r8r8r8_compare ( xval, yval, zval, xlo, ylo, zlo );

  if ( compare == -1 )
  {
    *less = 0;
    *equal = 0;
    *more = 1;
    return;
  }
  else if ( compare == 0 )
  {
    *less = 0;
    *equal = 1;
    *more = 2;
    return;
  }

  compare = r8r8r8_compare ( xval, yval, zval, xhi, yhi, zhi );

  if ( compare == 1 )
  {
    *less = n;
    *equal = 0;
    *more = n + 1;
    return;
  }
  else if ( compare == 0 )
  {
    *less = n - 1;
    *equal = n;
    *more = n + 1;
    return;
  }

  for ( ; ; )
  {
    if ( lo + 1 == hi )
    {
      *less = lo;
      *equal = 0;
      *more = hi;
      return;
    }

    mid = ( lo + hi ) / 2;
    xmid = x[indx[mid-1]-1];
    ymid = y[indx[mid-1]-1];
    zmid = z[indx[mid-1]-1];

    compare = r8r8r8_compare ( xval, yval, zval, xmid, ymid, zmid );

    if ( compare == 0 )
    {
      *equal = mid;
      *less = mid - 1;
      *more = mid + 1;
      return;
    }
    else if ( compare == -1 )
    {
      hi = mid;
    }
    else if ( compare == +1 )
    {
      lo = mid;
    }
  }

  return;
}
//****************************************************************************80

void r8r8vec_index_insert_unique ( int maxn, int *n, double x[], double y[], 
  int indx[], double xval, double yval, int *ival, int *ierror )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8VEC_INDEX_INSERT_UNIQUE inserts a unique R8R8 value in an indexed sorted list.
//
//  Discussion:
//
//    If the input value does not occur in the current list, it is added,
//    and N, X, Y and INDX are updated.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int MAXN, the maximum size of the list.
//
//    Input/output, int *N, the size of the list.
//
//    Input/output, double X[N], Y[N], the list of R8R8 vectors.
//
//    Input/output, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, YVAL, the value to be inserted if it is
//    not already in the list.
//
//    Output, int *IVAL, the index in X, Y corresponding to the
//    value XVAL, YVAL.
//
//    Output, int *IERROR, 0 for no error, 1 if an error occurred.
//
{
  int equal;
  int i;
  int less;
  int more;

  *ierror = 0;

  if ( *n <= 0 )
  {
    if ( maxn <= 0 )
    {
      *ierror = 1;
      cout << "\n";
      cout << "R8R8VEC_INDEX_INSERT_UNIQUE - Fatal error!\n";
      cout << "  Not enough space to store new data.\n";
      return;
    }

    *n = 1;
    x[0] = xval;
    y[0] = yval;
    indx[0] = 1;
    *ival = 1;
    return;
  }
//
//  Does ( XVAL, YVAL ) already occur in ( X, Y )?
//
  r8r8vec_index_search ( *n, x, y, indx, xval, yval, &less, &equal, &more );

  if ( equal == 0 )
  {
    if ( maxn <= *n )
    {
      *ierror = 1;
      cout << "\n";
      cout << "R8R8VEC_INDEX_INSERT_UNIQUE - Fatal error!\n";
      cout << "  Not enough space to store new data.\n";
      return;
    }

    x[*n] = xval;
    y[*n] = yval;
    *ival = *n + 1;
    for ( i = *n-1; more-1 <= i; i-- )
    {
      indx[i+1] = indx[i];
    }
    indx[more-1] = *n + 1;
    *n = *n + 1;
  }
  else
  {
    *ival = indx[equal-1];
  }

  return;
}
//****************************************************************************80

void r8r8vec_index_search ( int n, double x[], double y[], int indx[], 
  double xval, double yval, int *less, int *equal, int *more )

//****************************************************************************80
//
//  Purpose:
//
//    R8R8VEC_INDEX_SEARCH searches for an R8R8 value in an indexed sorted list.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, double X[N], Y[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, YVAL, the value to be sought.
//
//    Output, int *LESS, *EQUAL, *MORE, the indexes in INDX of the
//    entries of X that are just less than, equal to, and just greater
//    than XVAL.  If XVAL does not occur in X, then EQUAL is zero.
//    If XVAL is the minimum entry of X, then LESS is 0.  If XVAL
//    is the greatest entry of X, then MORE is N+1.
//
{
  int compare;
  int hi;
  int lo;
  int mid;
  double xhi;
  double xlo;
  double xmid;
  double yhi;
  double ylo;
  double ymid;

  if ( n <= 0 )
  {
    *less = 0;
    *equal = 0;
    *more = 0;
    return;
  }

  lo = 1;
  hi = n;

  xlo = x[indx[lo-1]-1];
  ylo = y[indx[lo-1]-1];

  xhi = x[indx[hi-1]-1];
  yhi = y[indx[hi-1]-1];

  compare = r8r8_compare ( xval, yval, xlo, ylo );

  if ( compare == -1 )
  {
    *less = 0;
    *equal = 0;
    *more = 1;
    return;
  }
  else if ( compare == 0 )
  {
    *less = 0;
    *equal = 1;
    *more = 2;
    return;
  }

  compare = r8r8_compare ( xval, yval, xhi, yhi );

  if ( compare == 1 )
  {
    *less = n;
    *equal = 0;
    *more = n + 1;
    return;
  }
  else if ( compare == 0 )
  {
    *less = n - 1;
    *equal = n;
    *more = n + 1;
    return;
  }

  for ( ; ; )
  {
    if ( lo + 1 == hi )
    {
      *less = lo;
      *equal = 0;
      *more = hi;
      return;
    }

    mid = ( lo + hi ) / 2;
    xmid = x[indx[mid-1]-1];
    ymid = y[indx[mid-1]-1];

    compare = r8r8_compare ( xval, yval, xmid, ymid );

    if ( compare == 0 )
    {
      *equal = mid;
      *less = mid - 1;
      *more = mid + 1;
      return;
    }
    else if ( compare == -1 )
    {
      hi = mid;
    }
    else if ( compare == +1 )
    {
      lo = mid;
    }
  }

  return;
}
//****************************************************************************80

double r8int_to_r8int ( double rmin, double rmax, double r, double r2min, 
  double r2max )

//****************************************************************************80
//
//  Purpose:
//
//    R8INT_TO_R8INT maps one R8 interval to another.
//
//  Discussion:
//
//    The formula used is
//
//      R2 := R2MIN + ( R2MAX - R2MIN ) * ( R - RMIN ) / ( RMAX - RMIN )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 January 2001
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double RMIN, RMAX, the first range.
//
//    Input, double R, the number to be converted.
//
//    Input, double R2MAX, R2MIN, the second range.
//
//    Output, double R8INT_TO_R8INT, the corresponding value in 
//    the range [R2MIN,R2MAX].
//
{
  double  r2;

  if ( rmax == rmin )
  {
    r2 = ( r2max + r2min ) / 2.0;
  }
  else
  {
    r2 = ( ( ( rmax - r        ) * r2min   
           + (        r - rmin ) * r2max ) 
           / ( rmax     - rmin ) );
  }

  return r2;
}
//****************************************************************************80

int r8int_to_i4int ( double rmin, double rmax, double r, int imin, int imax )

//****************************************************************************80
//
//  Purpose:
//
//    R8INT_TO_I4INT maps an R8 interval to an integer interval.
//
//  Discussion:
//
//    The formula used is
//
//      I := IMIN + ( IMAX - IMIN ) * ( R - RMIN ) / ( RMAX - RMIN )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 January 2001
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double RMIN, RMAX, the range.
//
//    Input, double R, the number to be converted.
//
//    Input, int IMAX, IMIN, the integer range.
//
//    Output, int I, the corresponding value in the range [IMIN,IMAX].
//
{
  int i;

  if ( rmax == rmin )
  {
    i = ( imax + imin ) / 2;
  }
  else
  {
    i = r8_nint ( 
      ( ( rmax - r        ) * ( double ) ( imin )   
      + (        r - rmin ) * ( double ) ( imax ) ) 
      / ( rmax     - rmin ) );
  }

  return i;
}
//****************************************************************************80

double *r8mat_cholesky_factor ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_CHOLESKY_FACTOR computes the Cholesky factor of a symmetric R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The matrix must be symmetric and positive semidefinite.
//
//    For a positive semidefinite symmetric matrix A, the Cholesky factorization
//    is a lower triangular matrix L such that:
//
//      A = L * L'
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix A.
//
//    Input, double A[N*N], the N by N matrix.
//
//    Output, double R8MAT_CHOLESKY_FACTOR[N*N], the N by N lower triangular
//    Cholesky factor.  However, a NULL pointer will be returned if
//    the matrix is not positive definite.
//
{
  double *c;

  int i;
  int j;
  int k;
  double sum2;

  c = r8mat_copy_new ( n, n, a );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < j; i++ )
    {
      c[i+j*n] = 0.0;
    }
    for ( i = j; i < n; i++ )
    {
      sum2 = c[j+i*n];
      for ( k = 0; k < j; k++ )
      {
        sum2 = sum2 - c[j+k*n] * c[i+k*n];
      }
      if ( i == j )
      {
        if ( sum2 <= 0.0 )
        {
          delete [] c;
          return NULL;
        }
        c[i+j*n] = sqrt ( sum2 );
      }
      else
      {
        if ( c[j+j*n] != 0.0 )
        {
          c[i+j*n] = sum2 / c[j+j*n];
        }
        else
        {
          c[i+j*n] = 0.0;
        }
      }
    }
  }
 
  return c;
}
//****************************************************************************80

double *r8mat_cholesky_solve ( int n, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_CHOLESKY_SOLVE solves a Cholesky factored linear system A * x = b.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix A.
//
//    Input, double A[N*N], the N by N Cholesky factor of the
//    system matrix.
//
//    Input, double B[N], the right hand side of the linear system.
//
//    Output, double R8MAT_CHOLESKY_SOLVE[N], the solution of the linear system.
//
{
  double *x;
  double *y;
//
//  Solve L * y = b.
//
  y = r8mat_l_solve ( n, a, b );
//
//  Solve L' * x = y.
//
  x = r8mat_lt_solve ( n, a, y );

  delete [] y;

  return x;
}
//****************************************************************************80

void r8mat_copy ( int m, int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_COPY copies one R8MAT to another.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A1[M*N], the matrix to be copied.
//
//    Output, double A2[M*N], the copy of A1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return;
}
//****************************************************************************80

double *r8mat_copy_new ( int m, int n, double a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_COPY_NEW copies one R8MAT to a "new" R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of double precision values, which
//    may be stored as a vector in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A1[M*N], the matrix to be copied.
//
//    Output, double R8MAT_COPY_NEW[M*N], the copy of A1.
//
{
  double *a2;
  int i;
  int j;

  a2 = new double[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }
  return a2;
}
//****************************************************************************80

double r8mat_det ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DET computes the determinant of an R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 October 2005
//
//  Author:
//
//    Original FORTRAN77 version by Helmut Spaeth
//    C++ version by John Burkardt
//
//  Reference:
//
//    Helmut Spaeth,
//    Cluster Analysis Algorithms
//    for Data Reduction and Classification of Objects,
//    Ellis Horwood, 1980, page 125-127.
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input, double A[N*N], the matrix whose determinant is desired.
//
//    Output, double R8MAT_DET, the determinant of the matrix.
//
{
  double *b;
  double det;
  int i;
  int j;
  int k;
  int kk;
  int m;
  int pivot;
  double temp;

  b = new double[n*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      b[i+j*n] = a[i+j*n];
    }
  }

  det = 1.0;

  for ( k = 1; k <= n; k++ )
  {
    m = k;
    for ( kk = k+1; kk <= n; kk++ )
    {
      if ( r8_abs ( b[m-1+(k-1)*n] ) < r8_abs ( b[kk-1+(k-1)*n] ) )
      {
        m = kk;
      }
    }

    if ( m != k )
    {
      det = -det;

      temp = b[m-1+(k-1)*n];
      b[m-1+(k-1)*n] = b[k-1+(k-1)*n];
      b[k-1+(k-1)*n] = temp;
    }

    det = det * b[k-1+(k-1)*n];

    if ( b[k-1+(k-1)*n] != 0.0 )
    {
      for ( i = k+1; i <= n; i++ )
      {
        b[i-1+(k-1)*n] = -b[i-1+(k-1)*n] / b[k-1+(k-1)*n];
      }

      for ( j = k+1; j <= n; j++ )
      {
        if ( m != k )
        {
          temp = b[m-1+(j-1)*n];
          b[m-1+(j-1)*n] = b[k-1+(j-1)*n];
          b[k-1+(j-1)*n] = temp;
        }
        for ( i = k+1; i <= n; i++ )
        {
          b[i-1+(j-1)*n] = b[i-1+(j-1)*n] + b[i-1+(k-1)*n] * b[k-1+(j-1)*n];
        }
      }
    }
  }

  delete [] b;

  return det;
}
//****************************************************************************80

double r8mat_det_2d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DET_2D computes the determinant of a 2 by 2 R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Discussion:
//
//    The determinant of a 2 by 2 matrix is
//
//      a11 * a22 - a12 * a21.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[2*2], the matrix whose determinant is desired.
//
//    Output, double R8MAT_DET_2D, the determinant of the matrix.
//
{
  double det;

  det = a[0+0*2] * a[1+1*2] - a[0+1*2] * a[1+0*2];

  return det;
}
//****************************************************************************80

double r8mat_det_3d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DET_3D computes the determinant of a 3 by 3 R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The determinant of a 3 by 3 matrix is
//
//        a11 * a22 * a33 - a11 * a23 * a32
//      + a12 * a23 * a31 - a12 * a21 * a33
//      + a13 * a21 * a32 - a13 * a22 * a31
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[3*3], the matrix whose determinant is desired.
//
//    Output, double R8MAT_DET_3D, the determinant of the matrix.
//
{
  double det;

  det =
      a[0+0*3] * ( a[1+1*3] * a[2+2*3] - a[1+2*3] * a[2+1*3] )
    + a[0+1*3] * ( a[1+2*3] * a[2+0*3] - a[1+0*3] * a[2+2*3] )
    + a[0+2*3] * ( a[1+0*3] * a[2+1*3] - a[1+1*3] * a[2+0*3] );

  return det;
}
//****************************************************************************80

double r8mat_det_4d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DET_4D computes the determinant of a 4 by 4 R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[4*4], the matrix whose determinant is desired.
//
//    Output, double R8MAT_DET_4D, the determinant of the matrix.
//
{
  double det;

  det =
      a[0+0*4] * (
          a[1+1*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
        - a[1+2*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
        + a[1+3*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] ) )
    - a[0+1*4] * (
          a[1+0*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
        - a[1+2*4] * ( a[2+0*4] * a[3+3*4] - a[2+3*4] * a[3+0*4] )
        + a[1+3*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] ) )
    + a[0+2*4] * (
          a[1+0*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
        - a[1+1*4] * ( a[2+0*4] * a[3+3*4] - a[2+3*4] * a[3+0*4] )
        + a[1+3*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] ) )
    - a[0+3*4] * (
          a[1+0*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
        - a[1+1*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] )
        + a[1+2*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] ) );

  return det;
}
//****************************************************************************80

double r8mat_det_5d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DET_5D computes the determinant of a 5 by 5 R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[5*5], the matrix whose determinant is desired.
//
//    Output, double R8MAT_DET_5D, the determinant of the matrix.
//
{
  double b[4*4];
  double det;
  int i;
  int inc;
  int j;
  int k;
  double sign;
//
//  Expand the determinant into the sum of the determinants of the
//  five 4 by 4 matrices created by dropping row 1, and column k.
//
  det = 0.0;
  sign = 1.0;

  for ( k = 0; k < 5; k++ ) 
  {
    for ( i = 0; i < 4; i++ )
    {
      for ( j = 0; j < 4; j++ )
      {
        if ( j < k )
        {
          inc = 0;
        }
        else
        {
          inc = 1;
        }
        b[i+j*4] = a[i+1+(j+inc)*5];
      }
    }

    det = det + sign * a[0+k*5] * r8mat_det_4d ( b );

    sign = - sign;
  }

  return det;
}
//****************************************************************************80

void r8mat_diag_add_scalar ( int n, double a[], double s )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DIAG_ADD_SCALAR adds a scalar to the diagonal of an R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix.
//
//    Input/output, double A[N*N], the N by N matrix to be modified.
//
//    Input, double S, the value to be added to the diagonal
//    of the matrix.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i+i*n] = a[i+i*n] + s;
  }

  return;
}
//****************************************************************************80

void r8mat_diag_add_vector ( int n, double a[], double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DIAG_ADD_VECTOR adds a vector to the diagonal of an R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix.
//
//    Input/output, double A[N*N], the N by N matrix.
//
//    Input, double V[N], the vector to be added to the diagonal of A.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i+i*n] = a[i+i*n] + v[i];
  }

  return;
}
//****************************************************************************80

double *r8mat_diag_get_vector ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DIAG_GET_VECTOR gets the value of the diagonal of an R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix.
//
//    Input, double A[N*N], the N by N matrix.
//
//    Output, double R8MAT_DIAG_GET_VECTOR[N], the diagonal entries
//    of the matrix.
//
{
  int i;
  double *v;

  v = new double[n];

  for ( i = 0; i < n; i++ )
  {
    v[i] = a[i+i*n];
  }

  return v;
}
//****************************************************************************80

void r8mat_diag_set_scalar ( int n, double a[], double s )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DIAG_SET_SCALAR sets the diagonal of an R8MAT to a scalar value.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix.
//
//    Input/output, double A[N*N], the N by N matrix to be modified.
//
//    Input, double S, the value to be assigned to the diagonal
//    of the matrix.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i+i*n] = s;
  }

  return;
}
//****************************************************************************80

void r8mat_diag_set_vector ( int n, double a[], double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_DIAG_SET_VECTOR sets the diagonal of an R8MAT to a vector.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix.
//
//    Input/output, double A[N*N], the N by N matrix.
//
//    Input, double V[N], the vector to be assigned to the
//    diagonal of A.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i+i*n] = v[i];
  }

  return;
}
//****************************************************************************80

double *r8mat_expand_linear ( int m, int n, double x[], int mfat, int nfat )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_EXPAND_LINEAR linearly interpolates new data into an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    In this routine, the expansion is specified by giving the number
//    of intermediate values to generate between each pair of original
//    data rows and columns.
//
//    The interpolation is not actually linear.  It uses the functions
//
//      1, x, y, and xy.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of input data.
//
//    Input, double X[M*N], the original data.
//
//    Input, int MFAT, NFAT, the number of data values to interpolate
//    between each row, and each column, of original data values.
//
//    Output, double XFAT(M2,N2), the fattened data, where
//    M2 = (M-1)*(MFAT+1)+1,
//    N2 = (N-1)*(NFAT+1)+1.
//
{
  int i;
  int ihi;
  int ii;
  int iii;
  int ip1;
  int j;
  int jhi;
  int jj;
  int jjj;
  int jp1;
  int m2;
  int n2;
  double s;
  double t;
  double x00;
  double x01;
  double x10;
  double x11;
  double *xfat;

  m2 = ( m - 1 ) * ( mfat + 1 ) + 1;
  n2 = ( n - 1 ) * ( nfat + 1 ) + 1;

  xfat = new double[m2*n2];

  for ( i = 1; i <= m; i++ )
  {
    if ( i < m )
    {
      ihi = mfat;
    }
    else
    {
      ihi = 0;
    }

    for ( j = 1; j <= n; j++ )
    {
      if ( j < n )
      {
        jhi = nfat;
      }
      else
      {
        jhi = 0;
      }

      if ( i < m )
      {
        ip1 = i + 1;
      }
      else
      {
        ip1 = i;
      }

      if ( j < n )
      {
        jp1 = j + 1;
      }
      else
      {
        jp1 = j;
      }

      x00 = x[i-1+(j-1)*m];
      x10 = x[ip1-1+(j-1)*m];
      x01 = x[i-1+(jp1-1)*m];
      x11 = x[ip1-1+(jp1-1)*m];

      for ( ii = 0; ii <= ihi; ii++ )
      {
        s = ( double ) ( ii ) / ( double ) ( ihi + 1 );

        for ( jj = 0; jj <= jhi; jj++ )
        {
          t = ( double ) ( jj ) / ( double ) ( jhi + 1 );

          iii = 1 + ( i - 1 ) * ( mfat + 1 ) + ii;
          jjj = 1 + ( j - 1 ) * ( nfat + 1 ) + jj;

          xfat[iii-1+(jjj-1)*m2] = 
                                            x00   
              + s     * (       x10       - x00 ) 
              + t     * (             x01 - x00 ) 
              + s * t * ( x11 - x10 - x01 + x00 );
        }
      }
    }
  }

  return xfat;
}
//****************************************************************************80

double *r8mat_expand_linear2 ( int m, int n, double a[], int m2, int n2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_EXPAND_LINEAR2 expands an R8MAT by linear interpolation.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    In this version of the routine, the expansion is indicated
//    by specifying the dimensions of the expanded array.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in A.
//
//    Input, double A(M,N), a "small" M by N array.
//
//    Input, int M2, N2, the number of rows and columns in A2.
//
//    Output, double R8MAT_EXPAND_LINEAR2[M2*N2], the expanded array, 
//    which contains an interpolated version of the data in A.
//
{
  double *a2;
  int i;
  int i1;
  int i2;
  int j;
  int j1;
  int j2;
  double r;
  double r1;
  double r2;
  double s;
  double s1;
  double s2;

  a2 = new double[m2*n2];

  for ( i = 1; i <= m2; i++ )
  {
    if ( m2 == 1 )
    {
      r = 0.5;
    }
    else
    {
      r = ( double ) ( i - 1 ) / ( double ) ( m2 - 1 );
    }

    i1 = 1 + ( int ) ( r * ( double ) ( m - 1 ) );
    i2 = i1 + 1;

    if ( m < i2 )
    {
      i1 = m - 1;
      i2 = m;
    }

    r1 = ( double ) ( i1 - 1 ) / ( double ) ( m - 1 );
    r2 = ( double ) ( i2 - 1 ) / ( double ) ( m - 1 );

    for ( j = 1; j <= n2; j++ )
    {
      if ( n2 == 1 )
      {
        s = 0.5;
      }
      else
      {
        s = ( double ) ( j - 1 ) / ( double ) ( n2 - 1 );
      }

      j1 = 1 + ( int ) ( s * ( double ) ( n - 1 ) );
      j2 = j1 + 1;

      if ( n < j2 )
      {
        j1 = n - 1;
        j2 = n;
      }

      s1 = ( double ) ( j1 - 1 ) / ( double ) ( n - 1 );
      s2 = ( double ) ( j2 - 1 ) / ( double ) ( n - 1 );

      a2[i-1+(j-1)*m2] = 
        ( ( r2 - r ) * ( s2 - s ) * a[i1-1+(j1-1)*m] 
        + ( r - r1 ) * ( s2 - s ) * a[i2-1+(j1-1)*m] 
        + ( r2 - r ) * ( s - s1 ) * a[i1-1+(j2-1)*m] 
        + ( r - r1 ) * ( s - s1 ) * a[i2-1+(j2-1)*m] ) 
        / ( ( r2 - r1 ) * ( s2 - s1 ) );
    }
  }

  return a2;
}
//****************************************************************************80

double *r8mat_givens_post ( int n, double a[], int row, int col )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_GIVENS_POST computes the Givens postmultiplier rotation matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Givens post-multiplier matrix G(ROW,COL) has the property that
//    the (ROW,COL)-th entry of A*G is zero.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    223 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrices A and G.
//
//    Input, double A[N*N], the matrix to be operated upon.
//
//    Input, int ROW, COL, the row and column of the
//    entry of A*G which is to be zeroed out.
//
//    Output, double R8MAT_GIVENS_POST[N*N], the Givens rotation matrix.
//    G is an orthogonal matrix, that is, the inverse of
//    G is the transpose of G.
//
{
  double *g;
  double theta;

  g = r8mat_identity ( n );

  theta = atan2 ( a[row-1+(col-1)*n], a[row-1+(row-1)*n] );

  g[row-1+(row-1)*n] =  cos ( theta );
  g[row-1+(col-1)*n] = -sin ( theta );
  g[col-1+(row-1)*n] =  sin ( theta );
  g[col-1+(col-1)*n] =  cos ( theta );

  return g;
}
//****************************************************************************80

double *r8mat_givens_pre ( int n, double a[], int row, int col )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_GIVENS_PRE computes the Givens premultiplier rotation matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Givens premultiplier rotation matrix G(ROW,COL) has the
//    property that the (ROW,COL)-th entry of G*A is zero.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrices A and G.
//
//    Input, double A[N*N], the matrix to be operated upon.
//
//    Input, int ROW, COL, the row and column of the
//    entry of the G*A which is to be zeroed out.
//
//    Output, double R8MAT_GIVENS_PRE[N*N], the Givens rotation matrix.
//    G is an orthogonal matrix, that is, the inverse of
//    G is the transpose of G.
//
{
  double *g;
  double theta;

  g = r8mat_identity ( n );

  theta = atan2 ( a[row-1+(col-1)*n], a[col-1+(col-1)*n] );

  g[row-1+(row-1)*n] =  cos ( theta );
  g[row-1+(col-1)*n] = -sin ( theta );
  g[col-1+(row-1)*n] =  sin ( theta );
  g[col-1+(col-1)*n] =  cos ( theta );

  return g;
}
//****************************************************************************80

double *r8mat_hess ( double (*fx) ( int n, double x[] ), int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HESS approximates a Hessian matrix via finite differences.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    H(I,J) = d2 F / d X(I) d X(J)
//
//    The values returned by this routine will be only approximate.
//    In some cases, they will be so poor that they are useless.
//    However, one of the best applications of this routine is for
//    checking your own Hessian calculations, since as Heraclitus
//    said, you'll never get the same result twice when you differentiate
//    a complicated expression by hand.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double *FX ( int N, double X[] ), the name of the user 
//    function routine.
//
//    Input, int N, the number of variables.
//
//    Input, double X[N], the values of the variables.
//
//    Output, double H[N*N], the approximated N by N Hessian matrix.
//
{
  double eps;
  double f00;
  double fmm;
  double fmp;
  double fpm;
  double fpp;
  double *h;
  int i;
  int j;
  double *s;
  double xi;
  double xj;
//
//  Choose the stepsizes.
//
  s = new double[n];

  eps = pow ( r8_epsilon ( ), 0.33 );

  for ( i = 0; i < n; i++ )
  {
    s[i] = eps * r8_max ( r8_abs ( x[i] ), 1.0 );
  }
//
//  Calculate the diagonal elements.
//
  h = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    xi = x[i];

    f00 = fx ( n, x );

    x[i] = xi + s[i];
    fpp = fx ( n, x );

    x[i] = xi - s[i];
    fmm = fx ( n, x );

    h[i+i*n] = ( ( fpp - f00 ) + ( fmm - f00 ) ) / s[i] / s[i];

    x[i] = xi;
  }
//
//  Calculate the off diagonal elements.
//
  for ( i = 0; i < n; i++ )
  {
    xi = x[i];

    for ( j = i+1; j < n; j++ )
    {
      xj = x[j];

      x[i] = xi + s[i];
      x[j] = xj + s[j];
      fpp = fx ( n, x );

      x[i] = xi + s[i];
      x[j] = xj - s[j];
      fpm = fx ( n, x );

      x[i] = xi - s[i];
      x[j] = xj + s[j];
      fmp = fx ( n, x );

      x[i] = xi - s[i];
      x[j] = xj - s[j];
      fmm = fx ( n, x );

      h[j+i*n] = ( ( fpp - fpm ) + ( fmm - fmp ) ) / ( 4.0 * s[i] * s[j] );

      h[i+j*n] = h[j+i*n];

      x[j] = xj;
    }
    x[i] = xi;
  }

  return h;
}
//****************************************************************************80

void r8mat_house_axh ( int n, double a[], double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_AXH computes A*H where H is a compact Householder matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Householder matrix H(V) is defined by
//
//      H(V) = I - 2 * v * v' / ( v' * v )
//
//    This routine is not particularly efficient.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input/output, double A[N*N], on input, the matrix to be postmultiplied.
//    On output, A has been replaced by A*H.
//
//    Input, double V[N], a vector defining a Householder matrix.
//
{
  int i;
  int j;
  int k;
  double v_normsq;

  v_normsq = 0.0;
  for ( i = 0; i < n; i++ )
  {
    v_normsq = v_normsq + v[i] * v[i];
  }
//
//  Compute A*H' = A*H
//
  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      for ( k = 0; k < n; k++ )
      {
        a[i+j*n] = a[i+j*n] - 2.0 * a[i+k*n] * v[k] * v[j] / v_normsq;
      }
    }
  }

  return;
}
//****************************************************************************80

double *r8mat_house_axh_new ( int n, double a[], double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_AXH_NEW computes A*H where H is a compact Householder matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Householder matrix H(V) is defined by
//
//      H(V) = I - 2 * v * v' / ( v' * v )
//
//    This routine is not particularly efficient.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input, double A[N*N], the matrix to be postmultiplied.
//
//    Input, double V[N], a vector defining a Householder matrix.
//
//    Output, double R8MAT_HOUSE_AXH[N*N], the product A*H.
//
{
  double *ah;
  int i;
  int j;
  int k;
  double v_normsq;

  v_normsq = 0.0;
  for ( i = 0; i < n; i++ )
  {
    v_normsq = v_normsq + v[i] * v[i];
  }
//
//  Compute A*H' = A*H
//
  ah = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      ah[i+j*n] = a[i+j*n];
      for ( k = 0; k < n; k++ )
      {
        ah[i+j*n] = ah[i+j*n] - 2.0 * a[i+k*n] * v[k] * v[j] / v_normsq;
      }
    }
  }

  return ah;
}
//****************************************************************************80

double *r8mat_house_form ( int n, double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_FORM constructs a Householder matrix from its compact form.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    H(v) = I - 2 * v * v' / ( v' * v )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input, double V[N], the vector defining the Householder matrix.
//
//    Output, double R8MAT_HOUSE_FORM[N*N], the Householder matrix.
//
{
  double beta;
  double *h;
  int i;
  int j;
//
//  Compute the L2 norm of V.
//
  beta = 0.0;
  for ( i = 0; i < n; i++ )
  {
    beta = beta + v[i] * v[i];
  }
//
//  Form the matrix H.
//
  h = r8mat_identity ( n );

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      h[i+j*n] = h[i+j*n] - 2.0 * v[i] * v[j] / beta;
    }
  }

  return h;
}
//****************************************************************************80

double *r8mat_house_hxa ( int n, double a[], double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_HXA computes H*A where H is a compact Householder matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Householder matrix H(V) is defined by
//
//      H(V) = I - 2 * v * v' / ( v' * v )
//
//    This routine is not particularly efficient.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input, double A[N*N], the matrix to be premultiplied.
//
//    Input, double V[N], a vector defining a Householder matrix.
//
//    Output, double R8MAT_HOUSE_HXA[N*N], the product H*A.
//
{
  double *ha;
  int i;
  int j;
  int k;
  double v_normsq;

  v_normsq = 0.0;
  for ( i = 0; i < n; i++ )
  {
    v_normsq = v_normsq + v[i] * v[i];
  }
//
//  Compute A*H' = A*H
//
  ha = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      ha[i+j*n] = a[i+j*n];
      for ( k = 0; k < n; k++ )
      {
        ha[i+j*n] = ha[i+j*n] - 2.0 * v[i] * v[k] * a[k+j*n] / v_normsq;
      }
    }
  }

  return ha;
}
//****************************************************************************80

double *r8mat_house_post ( int n, double a[], int row, int col )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_POST computes a Householder post-multiplier matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    H(ROW,COL) has the property that the ROW-th column of
//    A*H(ROW,COL) is zero from entry COL+1 to the end.
//
//    In the most common case, where a QR factorization is being computed,
//    ROW = COL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrices.
//
//    Input, double A[N*N], the matrix whose Householder matrix 
//    is to be computed.
//
//    Input, int ROW, COL, specify the location of the
//    entry of the matrix A which is to be preserved.  The entries in
//    the same row, but higher column, will be zeroed out if
//    A is postmultiplied by H.
//
//    Output, double R8MAT_HOUSE_POST[N*N], the Householder matrix.
//
{
  double *h;
  int j;
  double *v;
  double *w;
//
//  Set up the vector V.
//
  w = new double[n];

  for ( j = 0; j < col-1; j++ )
  {
    w[j] = 0.0;
  }
  for ( j = col-1; j < n; j++ )
  {
    w[j] = a[row+j*n];
  }

  v = r8vec_house_column ( n, w, col );
//
//  Form the matrix H(V).
//
  h = r8mat_house_form ( n, v );

  delete [] v;
  delete [] w;

  return h;
}
//****************************************************************************80

double *r8mat_house_pre ( int n, double a[], int row, int col )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_HOUSE_PRE computes a Householder pre-multiplier matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    H(ROW,COL) has the property that the COL-th column of
//    H(ROW,COL)*A is zero from entry ROW+1 to the end.
//
//    In the most common case, where a QR factorization is being computed,
//    ROW = COL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrices.
//
//    Input, double A[N*N], the matrix whose Householder matrix
//    is to be computed.
//
//    Input, int ROW, COL, specify the location of the
//    entry of the matrix A which is to be preserved.  The entries in
//    the same column, but higher rows, will be zeroed out if A is
//    premultiplied by H.
//
//    Output, double R8MAT_HOUSE_PRE[N*N], the Householder matrix.
//
{
  double *h;
  int i;
  double *v;
  double *w;

  w = new double[n];
//
//  Set up the vector V.
//
  for ( i = 0; i < row-1; i++ )
  {
    w[i] = 0.0;
  }
  for ( i = row-1; i < n; i++ )
  {
    w[i] = a[i+col*n];
  }

  v = r8vec_house_column ( n, w, row );
//
//  Form the matrix H(V).
//
  h = r8mat_house_form ( n, v );

  delete [] v;
  delete [] w;

  return h;
}
//****************************************************************************80

double *r8mat_identity ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_IDENTITY sets the square matrix A to the identity.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Output, double A[N*N], the N by N identity matrix.
//
{
  double *a;
  int i;
  int j;
  int k;

  a = new double[n*n];

  k = 0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      if ( i == j )
      {
        a[k] = 1.0;
      }
      else
      {
        a[k] = 0.0;
      }
      k = k + 1;
    }
  }

  return a;
}
//****************************************************************************80

bool r8mat_in_01 ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_IN_01 is TRUE if the entries of an R8MAT are in the range [0,1].
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix.
//
//    Output, bool R8MAT_IN_01, is TRUE if every entry of A is
//    between 0 and 1.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( a[i+j*m] < 0.0 || 1.0 < a[i+j*m] )
      {
        return false;
      }
    }
  }

  return true;
}
//****************************************************************************80

double *r8mat_indicator ( int m, int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_INDICATOR sets up an "indicator" R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The value of each entry suggests its location, as in:
//
//      11  12  13  14
//      21  22  23  24
//      31  32  33  34
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 January 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Output, double R8MAT_INDICATOR[M*N], the table.
//
{
  double *a;
  int fac;
  int i;
  int j;

  a = new double[m*n];

  fac = i4_power ( 10, i4_log_10 ( n ) + 1 );

  for ( i = 1; i <= m; i++ )
  {
    for ( j = 1; j <= n; j++ )
    {
      a[i-1+(j-1)*m] = ( double ) ( fac * i + j );
    }
  }
  return a;
}
//****************************************************************************80

double *r8mat_inverse_2d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_INVERSE_2D inverts a 2 by 2 matrix using Cramer's rule.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[2*2], the matrix to be inverted.
//
//    Output, double R8MAT_INVERSE_2D[2*2], the inverse of the matrix A.
//
{
  double *b;
  double det;
  int i;
  int j;
//
//  Compute the determinant of A.
//
  det = a[0+0*2] * a[1+1*2] - a[0+1*2] * a[1+0*2];
//
//  If the determinant is zero, bail out.
//
  if ( det == 0.0 )
  {
    return NULL;
  }
//
//  Compute the entries of the inverse matrix using an explicit formula.
//
  b = new double[2*2];

  b[0+0*2] = + a[1+1*2] / det;
  b[0+1*2] = - a[0+1*2] / det;
  b[1+0*2] = - a[1+0*2] / det;
  b[1+1*2] = + a[0+0*2] / det;

  return b;
}
//****************************************************************************80

double *r8mat_inverse_3d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_INVERSE_3D inverts a 3 by 3 matrix using Cramer's rule.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    If the determinant is zero, A is singular, and does not have an
//    inverse.  In that case, the output is set to NULL.
//
//    If the determinant is nonzero, its value is an estimate
//    of how nonsingular the matrix A is.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[3*3], the matrix to be inverted.
//
//    Output, double R8MAT_INVERSE_3D[3*3], the inverse of the matrix A.
//
{
  double *b;
  double det;
  int i;
  int j;
//
//  Compute the determinant of A.
//
  det =
     a[0+0*3] * ( a[1+1*3] * a[2+2*3] - a[1+2*3] * a[2+1*3] )
   + a[0+1*3] * ( a[1+2*3] * a[2+0*3] - a[1+0*3] * a[2+2*3] )
   + a[0+2*3] * ( a[1+0*3] * a[2+1*3] - a[1+1*3] * a[2+0*3] );

  if ( det == 0.0 )
  {
    return NULL; 
  }

  b = new double[3*3];

  b[0+0*3] =   ( a[1+1*3] * a[2+2*3] - a[1+2*3] * a[2+1*3] ) / det;
  b[0+1*3] = - ( a[0+1*3] * a[2+2*3] - a[0+2*3] * a[2+1*3] ) / det;
  b[0+2*3] =   ( a[0+1*3] * a[1+2*3] - a[0+2*3] * a[1+1*3] ) / det;

  b[1+0*3] = - ( a[1+0*3] * a[2+2*3] - a[1+2*3] * a[2+0*3] ) / det;
  b[1+1*3] =   ( a[0+0*3] * a[2+2*3] - a[0+2*3] * a[2+0*3] ) / det;
  b[1+2*3] = - ( a[0+0*3] * a[1+2*3] - a[0+2*3] * a[1+0*3] ) / det;

  b[2+0*3] =   ( a[1+0*3] * a[2+1*3] - a[1+1*3] * a[2+0*3] ) / det;
  b[2+1*3] = - ( a[0+0*3] * a[2+1*3] - a[0+1*3] * a[2+0*3] ) / det;
  b[2+2*3] =   ( a[0+0*3] * a[1+1*3] - a[0+1*3] * a[1+0*3] ) / det;

  return b;
}
//****************************************************************************80

double *r8mat_inverse_4d ( double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_INVERSE_4D inverts a 4 by 4 matrix using Cramer's rule.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[4][4], the matrix to be inverted.
//
//    Output, double R8MAT_INVERSE_4D[4][4], the inverse of the matrix A.
//
{
  double *b;
  int i;
  int j;
  double det;
//
//  Compute the determinant of A.
//
  det = r8mat_det_4d ( a );
//
//  If the determinant is zero, bail out.
//
  if ( det == 0.0 )
  {
    return NULL;
  }
//
//  Compute the entries of the inverse matrix using an explicit formula.
//
  b = new double[4*4];

  b[0+0*4] =
    +(
    + a[1+1*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
    + a[1+2*4] * ( a[2+3*4] * a[3+1*4] - a[2+1*4] * a[3+3*4] )
    + a[1+3*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
    ) / det;

  b[1+0*4] =
    -(
    + a[1+0*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
    + a[1+2*4] * ( a[2+3*4] * a[3+0*4] - a[2+0*4] * a[3+3*4] )
    + a[1+3*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] )
    ) / det;

  b[2+0*4] =
    +(
    + a[1+0*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
    + a[1+1*4] * ( a[2+3*4] * a[3+0*4] - a[2+0*4] * a[3+3*4] )
    + a[1+3*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] )
    ) / det;

  b[3+0*4] =
    -(
    + a[1+0*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
    + a[1+1*4] * ( a[2+2*4] * a[3+0*4] - a[2+0*4] * a[3+2*4] )
    + a[1+2*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] )
    ) / det;

  b[0+1*4] =
    -(
    + a[0+1*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
    + a[0+2*4] * ( a[2+3*4] * a[3+1*4] - a[2+1*4] * a[3+3*4] )
    + a[0+3*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
    ) / det;

  b[1+1*4] =
    +(
    + a[0+0*4] * ( a[2+2*4] * a[3+3*4] - a[2+3*4] * a[3+2*4] )
    + a[0+2*4] * ( a[2+3*4] * a[3+0*4] - a[2+0*4] * a[3+3*4] )
    + a[0+3*4] * ( a[2+0*4] * a[3+2*4] - a[2+2*4] * a[3+0*4] )
    ) / det;

  b[2+1*4] =
    -(
    + a[0+0*4] * ( a[2+1*4] * a[3+3*4] - a[2+3*4] * a[3+1*4] )
    + a[0+1*4] * ( a[2+3*4] * a[3+0*4] - a[2+0*4] * a[3+3*4] )
    + a[0+3*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] )
    ) / det;

  b[3+1*4] =
    +(
    + a[0+0*4] * ( a[2+1*4] * a[3+2*4] - a[2+2*4] * a[3+1*4] )
    + a[0+1*4] * ( a[2+2*4] * a[3+0*4] - a[2+0*4] * a[3+2*4] )
    + a[0+2*4] * ( a[2+0*4] * a[3+1*4] - a[2+1*4] * a[3+0*4] )
    ) / det;

  b[0+2*4] =
    +(
    + a[0+1*4] * ( a[1+2*4] * a[3+3*4] - a[1+3*4] * a[3+2*4] )
    + a[0+2*4] * ( a[1+3*4] * a[3+1*4] - a[1+1*4] * a[3+3*4] )
    + a[0+3*4] * ( a[1+1*4] * a[3+2*4] - a[1+2*4] * a[3+1*4] )
    ) / det;

  b[1+2*4] =
    -(
    + a[0+0*4] * ( a[1+2*4] * a[3+3*4] - a[1+3*4] * a[3+2*4] )
    + a[0+2*4] * ( a[1+3*4] * a[3+0*4] - a[1+0*4] * a[3+3*4] )
    + a[0+3*4] * ( a[1+0*4] * a[3+2*4] - a[1+2*4] * a[3+0*4] )
    ) / det;

  b[2+2*4] =
    +(
    + a[0+0*4] * ( a[1+1*4] * a[3+3*4] - a[1+3*4] * a[3+1*4] )
    + a[0+1*4] * ( a[1+3*4] * a[3+0*4] - a[1+0*4] * a[3+3*4] )
    + a[0+3*4] * ( a[1+0*4] * a[3+1*4] - a[1+1*4] * a[3+0*4] )
    ) / det;

  b[3+2*4] =
    -(
    + a[0+0*4] * ( a[1+1*4] * a[3+2*4] - a[1+2*4] * a[3+1*4] )
    + a[0+1*4] * ( a[1+2*4] * a[3+0*4] - a[1+0*4] * a[3+2*4] )
    + a[0+2*4] * ( a[1+0*4] * a[3+1*4] - a[1+1*4] * a[3+0*4] )
    ) / det;

  b[0+3*4] =
    -(
    + a[0+1*4] * ( a[1+2*4] * a[2+3*4] - a[1+3*4] * a[2+2*4] )
    + a[0+2*4] * ( a[1+3*4] * a[2+1*4] - a[1+1*4] * a[2+3*4] )
    + a[0+3*4] * ( a[1+1*4] * a[2+2*4] - a[1+2*4] * a[2+1*4] )
    ) / det;

  b[1+3*4] =
    +(
    + a[0+0*4] * ( a[1+2*4] * a[2+3*4] - a[1+3*4] * a[2+2*4] )
    + a[0+2*4] * ( a[1+3*4] * a[2+0*4] - a[1+0*4] * a[2+3*4] )
    + a[0+3*4] * ( a[1+0*4] * a[2+2*4] - a[1+2*4] * a[2+0*4] )
    ) / det;

  b[2+3*4] =
    -(
    + a[0+0*4] * ( a[1+1*4] * a[2+3*4] - a[1+3*4] * a[2+1*4] )
    + a[0+1*4] * ( a[1+3*4] * a[2+0*4] - a[1+0*4] * a[2+3*4] )
    + a[0+3*4] * ( a[1+0*4] * a[2+1*4] - a[1+1*4] * a[2+0*4] )
    ) / det;

  b[3+3*4] =
    +(
    + a[0+0*4] * ( a[1+1*4] * a[2+2*4] - a[1+2*4] * a[2+1*4] )
    + a[0+1*4] * ( a[1+2*4] * a[2+0*4] - a[1+0*4] * a[2+2*4] )
    + a[0+2*4] * ( a[1+0*4] * a[2+1*4] - a[1+1*4] * a[2+0*4] )
    ) / det;

  return b;
}
//****************************************************************************80

double *r8mat_jac ( int m, int n, double eps, 
  double *(*fx) ( int m, int n, double x[] ), double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_JAC estimates a dense jacobian matrix of the function FX.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    FPRIME(I,J) = d F(I) / d X(J).
//
//    The jacobian is assumed to be dense, and the LINPACK/LAPACK
//    double precision general matrix storage mode ("DGE") is used.
//
//    Forward differences are used, requiring N+1 function evaluations.
//
//    Values of EPS have typically been chosen between
//    sqrt ( EPSMCH ) and sqrt ( sqrt ( EPSMCH ) ) where EPSMCH is the
//    machine tolerance.
//
//    If EPS is too small, then F(X+EPS) will be the same as
//    F(X), and the jacobian will be full of zero entries.
//
//    If EPS is too large, the finite difference estimate will
//    be inaccurate.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of functions.
//
//    Input, int N, the number of variables.
//
//    Input, double EPS, a tolerance to be used for shifting the
//    X values during the finite differencing.  No single value
//    of EPS will be reliable for all vectors X and functions FX.
//
//    Input, double *(*FX) ( int m, int n, double x[] ), the name of 
//    the user written routine which evaluates the M-dimensional
//    function at a given N-dimensional point X.
//
//    Input, double X[N], the point where the jacobian
//    is to be estimated.
//
//    Output, double R8MAT_JAC[M*N], the estimated jacobian matrix.
//
{
  double del;
  double *fprime;
  int i;
  int j;
  double xsave;
  double *work1;
  double *work2;

  fprime = new double[m*n];
//
//  Evaluate the function at the base point, X.
//
  work2 = fx ( m, n, x );
//
//  Now, one by one, vary each component J of the base point X, and
//  estimate DF(I)/DX(J) = ( F(X+) - F(X) )/ DEL.
//
  for ( j = 0; j < n; j++ )
  {
    xsave = x[j];
    del = eps * ( 1.0 + r8_abs ( x[j] ) );
    x[j] = x[j] + del;
    work1 = fx ( m, n, x );
    x[j] = xsave;
    for ( i = 0; i < m; i++ )
    {
      fprime[i+j*m] = ( work1[i] - work2[i] ) / del;
    }
    delete [] work1;
  }
  delete [] work2;

  return fprime;
}
//****************************************************************************80

double *r8mat_l_inverse ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_L_INVERSE inverts a lower triangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    A lower triangular matrix is a matrix whose only nonzero entries
//    occur on or below the diagonal.
//
//    The inverse of a lower triangular matrix is a lower triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, double A[N*N], the lower triangular matrix.
//
//    Output, double R8MAT_L_INVERSE[N*N], the inverse matrix.
//
{
  double *b;
  int i;
  int j;
  int k;
  double temp;

  b = new double[n*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      if ( i < j )
      {
        b[i+j*n] = 0.0;
      }
      else if ( j == i )
      {
        b[i+j*n] = 1.0 / a[i+j*n];
      }
      else
      {
        temp = 0.0;
        for ( k = 0; k < i; k++ )
        {
          temp = temp + a[i+k*n] * b[k+j*n];
        }
        b[i+j*n] = -temp / a[i+i*n];
      }
    }
  }

  return b;
}
//****************************************************************************80

void r8mat_l_print ( int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_L_PRINT prints a lower triangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Example:
//
//    M = 5, N = 5
//    A = (/ 11, 21, 31, 41, 51, 22, 32, 42, 52, 33, 43, 53, 44, 54, 55 /)
//
//    11
//    21 22
//    31 32 33
//    41 42 43 44
//    51 52 53 54 55
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[*], the M by N matrix.  Only the lower
//    triangular elements are stored, in column major order.
//
//    Input, char *TITLE, a title to be printed.
//
{
  int i;
  int indx[10];
  int j;
  int jhi;
  int jlo;
  int jmax;
  int nn;
  int size;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  jmax = i4_min ( n, m );

  if ( m <= n )
  {
    size = ( m * ( m + 1 ) ) / 2;
  }
  else if ( n < m )
  {
    size = ( n * ( n + 1 ) ) / 2 + ( m - n ) * n;
  }

  if ( r8vec_is_int ( size, a ) )
  {
    nn = 10;
    for ( jlo = 1; jlo <= jmax; jlo = jlo + nn )
    {
      jhi = i4_min ( jlo + nn - 1, i4_min ( m, jmax ) );
      cout << "\n";
      cout << "  Col   ";
      for ( j = jlo; j <= jhi; j++ )
      {
        cout << setw(6) << j;
      }
      cout << "\n";
      cout << "  Row  \n";
      for ( i = jlo; i <= m; i++ )
      {
        jhi = i4_min ( jlo + nn - 1, i4_min ( i, jmax ) );
        for ( j = jlo; j <= jhi; j++ )
        {
          indx[j-jlo] = ( j - 1 ) * m + i - ( j * ( j - 1 ) ) / 2;
        }
        cout << "  " << setw(6) << i;
        for ( j = 0; j <= jhi-jlo; j++ )
        {
          cout << setw(6) << a[indx[j]-1];
        }
        cout << "\n";
      }
    }
  }
  else if ( r8vec_amax ( size, a ) < 1000000.0 )
  {
    nn = 5;
    for ( jlo = 1; jlo <= jmax; jlo = jlo + nn );
    {
      jhi = i4_min ( jlo + nn - 1, i4_min ( m - 1, jmax ) );
      cout << "\n";
      cout << "  Col ";
      for ( j = jlo; j <= jhi; j++ )
      {
        cout << setw(14) << j;
      }
      cout << "\n";
      cout << "  Row  \n";
      for ( i = jlo; i <= m; i++ )
      {
        jhi = i4_min ( jlo + nn - 1, i4_min ( i, jmax ) );
        for ( j = jlo; j <= jhi; j++ )
        {
          indx[j-jlo] = ( j - 1 ) * m + i - ( j * ( j - 1 ) ) / 2;
        }
        cout << "  " << setw(6) << i;
        for ( j = 0; j <= jhi-jlo; j++ )
        {
          cout << setw(14) << a[indx[j]-1];
        }
        cout << "\n";
      }
    }
  }
  else
  {
    nn = 5;

    for ( jlo = 1; jlo <= jmax; jlo = jlo + nn )
    {
      jhi = i4_min ( jlo + nn - 1, i4_min ( m - 1, jmax ) );
      cout << "\n";
      cout << "  Col ";
      for ( j = jlo; j <= jhi; j++ )
      {
        cout << setw(7) << j << "       ";
      }
      cout << "\n";
      cout << "  Row \n";
      for ( i = jlo; i <= m; i++ )
      {
        jhi = i4_min ( jlo + nn - 1, i4_min ( i, jmax ) );
        for ( j = jlo; j <= jhi; j++ )
        {
          indx[j-jlo] = ( j - 1 ) * m + i - ( j * ( j - 1 ) ) / 2;
        }
        cout << setw(6) << i;
        for ( j = 0; j <= jhi-jlo; j++ )
        {
          cout << setw(14) << a[indx[j]-1];
        }
      }
    }
  }

  return;
}
//****************************************************************************80

double *r8mat_l_solve ( int n, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_L_SOLVE solves a lower triangular linear system.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix A.
//
//    Input, double A[N*N], the N by N lower triangular matrix.
//
//    Input, double B[N], the right hand side of the linear system.
//
//    Output, double R8MAT_L_SOLVE[N], the solution of the linear system.
//
{
  int i;
  int j;
  double temp;
  double *x;

  x = new double[n];
//
//  Solve L * x = b.
//
  for ( i = 0; i < n; i++ )
  {
    temp = 0.0;
    for ( j = 0; j < i; j++ )
    {
      temp = temp + a[i+j*n] * x[j];
    }
    x[i] = ( b[i] - temp ) / a[i+i*n];
  }

  return x;
}
//****************************************************************************80

double *r8mat_l1_inverse ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_L1_INVERSE inverts a unit lower triangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    A unit lower triangular matrix is a matrix with only 1's on the main
//    diagonal, and only 0's above the main diagonal.
//
//    The inverse of a unit lower triangular matrix is also
//    a unit lower triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, double A[N*N], the unit lower triangular matrix.
//
//    Output, double R8MAT_L1_INVERSE[N*N], the inverse matrix.
//
{
  double *b;
  int i;
  int j;
  int k;

  b = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      if ( i < j )
      {
        b[i+j*n] = 0.0;
      }
      else if ( j == i )
      {
        b[i+j*n] = 1.0;
      }
      else
      {
        b[i+j*n] = 0.0;
        for ( k = 0; k < i; k++ )
        {
          b[i+j*n] = b[i+j*n] - a[i+k*n] * b[k+j*n];
        }
      }
    }
  }

  return b;
}
//****************************************************************************80

void r8mat_lu ( int m, int n, double a[], double l[], double p[], double u[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_LU computes the LU factorization of a rectangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The routine is given an M by N matrix A, and produces
//
//      L, an M by M unit lower triangular matrix,
//      U, an M by N upper triangular matrix, and
//      P, an M by M permutation matrix P,
//
//    so that
//
//      A = P' * L * U.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 November 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix to be factored.
//
//    Output, double L[M*M], the M by M unit lower triangular factor.
//
//    Output, double P[M*M], the M by M permutation matrix.
//
//    Output, double U[M*N], the M by N upper triangular factor.
//
{
  int i;
  int ipiv;
  int j;
  int k;
  double pivot;
//
//  Initialize:
//
//    U:=A
//    L:=Identity
//    P:=Identity
//
  r8mat_copy ( m, n, a, u );

  r8mat_zero ( m, m, l );
  r8mat_zero ( m, m, p );
  for ( i = 0; i < m; i++ )
  {
    l[i+i*m] = 1.0;
    p[i+i*m] = 1.0;
  }
//
//  On step J, find the pivot row, IPIV, and the pivot value PIVOT.
//
  for ( j = 0; j < i4_min ( m - 1, n ); j++ )
  {
    pivot = 0.0;
    ipiv = -1;

    for ( i = j; i < m; i++ )
    {
      if ( pivot < r8_abs ( u[i+j*m] ) )
      {
        pivot = r8_abs ( u[i+j*m] );
        ipiv = i;
      }
    }
//
//  Unless IPIV is zero, swap rows J and IPIV.
//
    if ( ipiv != -1 )
    {
      r8row_swap ( m, n, u, j+1, ipiv+1 );

      r8row_swap ( m, m, l, j+1, ipiv+1 );

      r8row_swap ( m, m, p, j+1, ipiv+1 );
//
//  Zero out the entries in column J, from row J+1 to M.
//
      for ( i = j+1; i < m; i++ )
      {
        if ( u[i+j*m] != 0.0 )
        {
          l[i+j*m] = u[i+j*m] / u[j+j*m];

          u[i+j*m] = 0.0;

          for ( k = j+1; k < n; k++ )
          {
            u[i+k*m] = u[i+k*m] - l[i+j*m] * u[j+k*m];
          }
        }
      }
    }
  }

  return;
}
//****************************************************************************80

double *r8mat_lt_solve ( int n, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_LT_SOLVE solves a transposed lower triangular linear system.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    Given the lower triangular matrix A, the linear system to be solved is:
//
//      A' * x = b
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix A.
//
//    Input, double A[N*N], the N by N lower triangular matrix.
//
//    Input, double B[N], the right hand side of the linear system.
//
//    Output, double R8MAT_LT_SOLVE[N], the solution of the linear system.
//
{
  int i;
  int j;
  double *x;

  x = new double[n];

  for ( j = n-1; 0 <= j; j-- )
  {
    x[j] = b[j];
    for ( i = j+1; i < n; i++ )
    {
      x[j] = x[j] - x[i] * a[i+j*n];
    }
    x[j] = x[j] / a[j+j*n];
  }

  return x;
}
//****************************************************************************80

double r8mat_max ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MAX returns the maximum entry of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix.
//
//    Output, double R8MAT_MAX, the maximum entry of A.
//
{
  int i;
  int j;
  double value;

  value = - r8_huge ( );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      value = r8_max ( value, a[i+j*m] );
    }
  }

  return value;
}
//****************************************************************************80

void r8mat_max_index ( int m, int n, double a[], int *i_max, int *j_max )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MAX_INDEX returns the location of the maximum entry of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix.
//
//    Output, int *I_MAX, *J_MAX, the indices of the maximum entry of A.
//
{
  int i;
  int i2;
  int j;
  int j2;

  i2 = -1;
  j2 = -1;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( i2 == -1 && j2 == -1 )
      {
        i2 = i;
        j2 = j;
      }
      else if ( a[i2+j2*m] < a[i+j*m] )
      {
        i2 = i;
        j2 = j;
      }
    }
  }

  *i_max = i2 + 1;
  *j_max = j2 + 1;

  return;
}
//****************************************************************************80

double r8mat_maxcol_minrow ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MAXCOL_MINROW gets the maximum column minimum row of an M by N matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    R8MAT_MAXCOL_MINROW = max ( 1 <= I <= N ) ( min ( 1 <= J <= M ) A(I,J) )
//
//    For a given matrix, R8MAT_MAXCOL_MINROW <= R8MAT_MINROW_MAXCOL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix.
//
//    Output, double R8MAT_MAXCOL_MINROW, the maximum column
//    minimum row entry of A.
//
{
  int i;
  int j;
  double minrow;
  double value;

  value = - r8_huge ( );

  for ( i = 0; i < m; i++ )
  {
    minrow = r8_huge ( );

    for ( j = 0; j < n; j++ )
    {
      minrow = r8_min ( minrow, a[i+j*m] );
    }
    value = r8_max ( value, minrow );
  }

  return value;
}
//****************************************************************************80

double r8mat_maxrow_mincol ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MAXROW_MINCOL gets the maximum row minimum column of an M by N matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    R8MAT_MAXROW_MINCOL = max ( 1 <= J <= N ) ( min ( 1 <= I <= M ) A(I,J) )
//
//    For a given matrix, R8MAT_MAXROW_MINCOL <= R8MAT_MINCOL_MAXROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix.
//
//    Output, double R8MAT_MAXROW_MINCOL, the maximum row 
//    minimum column entry of A.
//
{
  int i;
  int j;
  double mincol;
  double value;

  value = - r8_huge ( );

  for ( j = 0; j < n; j++ )
  {
    mincol = r8_huge ( );
    for ( i = 0; i < m; i++ )
    {
      mincol = r8_min ( mincol, a[i+j*m] );
    }
    value = r8_max ( value, mincol );
  }
  return value;
}
//****************************************************************************80

double r8mat_min ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MIN returns the minimum entry of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix.
//
//    Output, double DMIN_MAX, the minimum entry of A.
//
{
  int i;
  int j;
  double value;

  value = r8_huge ( );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      value = r8_min ( value, a[i+j*m] );
    }
  }
  return value;
}
//****************************************************************************80

void r8mat_min_index ( int m, int n, double a[], int *i_min, int *j_min )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MIN_INDEX returns the location of the minimum entry of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix.
//
//    Output, int *I_MIN, *J_MIN, the indices of the minimum entry of A.
//
{
  int i;
  int i2;
  int j;
  int j2;

  i2 = -1;
  j2 = -1;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( i2 == -1 && j2 == -1 )
      {
        i2 = i;
        j2 = j;
      }
      else if ( a[i+j*m] < a[i2+j2*m] )
      {
        i2 = i;
        j2 = j;
      }
    }
  }

  *i_min = i2 + 1;
  *j_min = j2 + 1;

  return;
}
//****************************************************************************80

double r8mat_mincol_maxrow ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MINCOL_MAXROW gets the minimum column maximum row of an M by N matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    R8MAT_MINCOL_MAXROW = min ( 1 <= I <= N ) ( max ( 1 <= J <= M ) A(I,J) )
//
//    For a given matrix, R8MAT_MAXROW_MINCOL <= R8MAT_MINCOL_MAXROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A(M,N), the matrix.
//
//    Output, double R8MAT_MINCOL_MAXROW, the minimum column
//    maximum row entry of A.
//
{
  int i;
  int j;
  double maxrow;
  double value;

  value = r8_huge ( );

  for ( i = 0; i < m; i++ )
  {
    maxrow = - r8_huge ( );
    for ( j = 0; j < n; j++ )
    {
      maxrow = r8_max ( maxrow, a[i+j*m] );
    }
    value = r8_min ( value, maxrow );
  }

  return value;
}
//****************************************************************************80

double r8mat_minrow_maxcol ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MINROW_MAXCOL gets the minimum row maximum column of an M by N matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    R8MAT_MINROW_MAXCOL = min ( 1 <= J <= N ) ( max ( 1 <= I <= M ) A(I,J) )
//
//    For a given matrix, R8MAT_MAXCOL_MINROW <= R8MAT_MINROW_MAXCOL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix.
//
//    Output, double R8MAT_MINROW_MAXCOL, the minimum row 
//    maximum column entry of A.
//
{
  int i;
  int j;
  double maxcol;
  double value;;

  value = r8_huge ( );

  for ( j = 0; j < n; j++ )
  {
    maxcol = - r8_huge ( );
    for ( i = 0; i < m; i++ )
    {
      maxcol = r8_max ( maxcol, a[i+j*m] );
    }
    value = r8_min ( value, maxcol );
  }

  return value;
}
//****************************************************************************80

double *r8mat_mm ( int n1, int n2, int n3, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MM multiplies two matrices.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as the function value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, N3, the order of the matrices.
//
//    Input, double A[N1*N2], double B[N2*N3], the matrices to multiply.
//
//    Output, double R8MAT_MM[N1*N3], the product matrix C = A * B.
//
{
  double *c;
  int i;
  int j;
  int k;

  c = new double[n1*n3];

  for ( i = 0; i < n1; i ++ )
  {
    for ( j = 0; j < n3; j++ )
    {
      c[i+j*n1] = 0.0;
      for ( k = 0; k < n2; k++ )
      {
        c[i+j*n1] = c[i+j*n1] + a[i+k*n1] * b[k+j*n2];
      }
    }
  }

  return c;
}
//****************************************************************************80

double *r8mat_mtv ( int m, int n, double a[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MTV multiplies a transposed matrix times a vector.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as the function value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of the matrix.
//
//    Input, double A[M,N], the M by N matrix.
//
//    Input, double X[M], the vector to be multiplied by A.
//
//    Output, double R8MAT_MTV[N], the product A'*X.
//
{
  int i;
  int j;
  double *y;

  y = new double[n];

  for ( j = 0; j < n; j++ )
  {
    y[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      y[j] = y[j] + a[i+j*m] * x[i];
    }
  }

  return y;
}
//****************************************************************************80

void r8mat_mtxv ( int m, int n, double a[], double x[], double atx[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MTXV multiplies a transposed matrix times a vector.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as an argument.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of the matrix.
//
//    Input, double A[M,N], the M by N matrix.
//
//    Input, double X[M], the vector to be multiplied by A.
//
//    Output, double ATX[N], the product A'*X.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    atx[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      atx[j] = atx[j] + a[i+j*m] * x[i];
    }
  }

  return;
}
//****************************************************************************80

double *r8mat_mv ( int m, int n, double a[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MV multiplies a matrix times a vector.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as the function value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of the matrix.
//
//    Input, double A[M,N], the M by N matrix.
//
//    Input, double X[N], the vector to be multiplied by A.
//
//    Output, double R8MAT_MV[M], the product A*X.
//
{
  int i;
  int j;
  double *y;

  y = new double[m];

  for ( i = 0; i < m; i++ )
  {
    y[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      y[i] = y[i] + a[i+j*m] * x[j];
    }
  }

  return y;
}
//****************************************************************************80

void r8mat_mxm ( int n1, int n2, int n3, double a[], double b[], double c[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MXM multiplies two matrices.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as an argument.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, N3, the order of the matrices.
//
//    Input, double A[N1*N2], double B[N2*N3], the matrices to multiply.
//
//    Output, double C[N1*N3], the product matrix C = A * B.
//
{
  int i;
  int j;
  int k;

  for ( i = 0; i < n1; i ++ )
  {
    for ( j = 0; j < n3; j++ )
    {
      c[i+j*n1] = 0.0;
      for ( k = 0; k < n2; k++ )
      {
        c[i+j*n1] = c[i+j*n1] + a[i+k*n1] * b[k+j*n2];
      }
    }
  }

  return;
}
//****************************************************************************80

double *r8mat_mxm_new ( int n1, int n2, int n3, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MXM_NEW multiplies two matrices.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N1, N2, N3, the order of the matrices.
//
//    Input, double A[N1*N2], double B[N2*N3], the matrices to multiply.
//
//    Output, double R8MAT_MXM_NEW[N1*N3], the product matrix C = A * B.
//
{
  double *c;
  int i;
  int j;
  int k;

  c = new double[n1*n3];

  for ( i = 0; i < n1; i ++ )
  {
    for ( j = 0; j < n3; j++ )
    {
      c[i+j*n1] = 0.0;
      for ( k = 0; k < n2; k++ )
      {
        c[i+j*n1] = c[i+j*n1] + a[i+k*n1] * b[k+j*n2];
      }
    }
  }

  return c;
}
//****************************************************************************80

void r8mat_mxv ( int m, int n, double a[], double x[], double ax[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_MXV multiplies a matrix times a vector.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    For this routine, the result is returned as an argument.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of the matrix.
//
//    Input, double A[M,N], the M by N matrix.
//
//    Input, double X[N], the vector to be multiplied by A.
//
//    Output, double AX[M], the product A*X.
//
{
  int i;
  int j;

  for ( i = 0; i < m; i++ )
  {
    ax[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      ax[i] = ax[i] + a[i+j*m] * x[j];
    }
  }

  return;
}
//****************************************************************************80

void r8mat_nint ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_NINT rounds the entries of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of A.
//
//    Input/output, double A[M*N], the matrix to be NINT'ed.
//
{
  int i;
  int j;
  int s;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      if ( a[i+j*m] < 0.0 )
      {
        s = -1;
      }
      else
      {
        s = 1;
      }
      a[i+j*m] = s * ( int ) ( r8_abs ( a[i+j*m] ) + 0.5 );
    }
  }

  return;
}
//****************************************************************************80

double r8mat_norm_eis ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_NORM_EIS returns the EISPACK norm of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The EISPACK norm is defined as:
//
//      R8MAT_NORM_EIS =
//        sum ( 1 <= I <= M ) sum ( 1 <= J <= N ) abs ( A(I,J) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix whose EISPACK norm is desired.
//
//    Output, double R8MAT_NORM_EIS, the EISPACK norm of A.
//
{
  int i;
  int j;
  double value;

  value = 0.0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      value = value + r8_abs ( a[i+j*m] );
    }
  }

  return value;
}
//****************************************************************************80

double r8mat_norm_fro ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_NORM_FRO returns the Frobenius norm of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The Frobenius norm is defined as
//
//      R8MAT_NORM_FRO = sqrt (
//        sum ( 1 <= I <= M ) sum ( 1 <= j <= N ) A(I,J)**2 )
//    The matrix Frobenius norm is not derived from a vector norm, but
//    is compatible with the vector L2 norm, so that:
//
//      r8vec_norm_l2 ( A * x ) <= r8mat_norm_fro ( A ) * r8vec_norm_l2 ( x ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix whose Frobenius
//    norm is desired.
//
//    Output, double R8MAT_NORM_FRO, the Frobenius norm of A.
//
{
  int i;
  int j;
  double value;

  value = 0.0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      value = value + pow ( a[i+j*m], 2 );
    }
  }
  value = sqrt ( value );

  return value;
}
//****************************************************************************80

double *r8mat_nullspace ( int m, int n, double a[], int nullspace_size )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_NULLSPACE computes the nullspace of a matrix.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    Let A be an MxN matrix.
//
//    If X is an N-vector, and A*X = 0, then X is a null vector of A.
//
//    The set of all null vectors of A is called the nullspace of A.
//
//    The 0 vector is always in the null space.
//
//    If the 0 vector is the only vector in the nullspace of A, then A
//    is said to have maximum column rank.  (Because A*X=0 can be regarded
//    as a linear combination of the columns of A).  In particular, if A
//    is square, and has maximum column rank, it is nonsingular.
//
//    The dimension of the nullspace is the number of linearly independent
//    vectors that span the nullspace.  If A has maximum column rank,
//    its nullspace has dimension 0.  
//
//    This routine uses the reduced row echelon form of A to determine
//    a set of NULLSPACE_SIZE independent null vectors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of
//    the matrix A.
//
//    Input, double A[M*N], the matrix to be analyzed. 
//
//    Input, int NULLSPACE_SIZE, the size of the nullspace.
//
//    Output, double R8MAT_NULLSPACE[N*NULLSPACE_SIZE], vectors that
//    span the nullspace.
//
{
  int *col;
  int i;
  int i2;
  int j;
  int j2;
  double *nullspace;
  int *row;
  double *rref;
//
//  Make a copy of A.
//
  rref = r8mat_copy_new ( m, n, a );
//
//  Get the reduced row echelon form of A.
//
  r8mat_rref ( m, n, rref );
//
//  Note in ROW the columns of the leading nonzeros.
//  COL(J) = +J if there is a leading 1 in that column, and -J otherwise.
//
  row = new int[m];
  for ( i = 0; i < m; i++ )
  {
    row[i] = 0;
  }

  col = new int[n];
  for ( j = 0; j < n; j++ )
  {
    col[j] = - ( j + 1 );
  }

  for ( i = 0; i < m; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      if ( rref[i+j*m] == 1.0 )
      {
        row[i] = ( j + 1 );
        col[j] = ( j + 1 );
        break;
      }
    }
  }

  nullspace = r8mat_zero_new ( n, nullspace_size );

  j2 = 0;
//
//  If column J does not contain a leading 1, then it contains
//  information about a null vector.
//
  for ( j = 0; j < n; j++ )
  {
    if ( col[j] < 0 )
    {
      for ( i = 0; i < m; i++ )
      {
        if ( rref[i+j*m] != 0.0 )
        {
          i2 = row[i] - 1;
          nullspace[i2+j2*n] = - rref[i+j*m];
        }
      }
      nullspace[j+j2*n] = 1.0;
      j2 = j2 + 1;
    } 
  }
  delete [] col;
  delete [] row;
  delete [] rref;

  return nullspace;
}
//****************************************************************************80

int r8mat_nullspace_size ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_NULLSPACE_SIZE computes the size of the nullspace of a matrix.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    Let A be an MxN matrix.
//
//    If X is an N-vector, and A*X = 0, then X is a null vector of A.
//
//    The set of all null vectors of A is called the nullspace of A.
//
//    The 0 vector is always in the null space.
//
//    If the 0 vector is the only vector in the nullspace of A, then A
//    is said to have maximum column rank.  (Because A*X=0 can be regarded
//    as a linear combination of the columns of A).  In particular, if A
//    is square, and has maximum column rank, it is nonsingular.
//
//    The dimension of the nullspace is the number of linearly independent
//    vectors that span the nullspace.  If A has maximum column rank,
//    its nullspace has dimension 0.  
//
//    This routine ESTIMATES the dimension of the nullspace.  Cases of
//    singularity that depend on exact arithmetic will probably be missed.
//
//    The nullspace will be estimated by counting the leading 1's in the
//    reduced row echelon form of A, and subtracting this from N.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of
//    the matrix A.
//
//    Input, double A[M*N], the matrix to be analyzed. 
//
//    Output, int R8MAT_NULLSPACE_SIZE, the estimated size 
//    of the nullspace.
//
{
  int i;
  int j;
  int leading;
  int nullspace_size;
  double *rref;
//
//  Make a copy of A.
//
  rref = r8mat_copy_new ( m, n, a );
//
//  Get the reduced row echelon form of A.
//
  r8mat_rref ( m, n, rref );
//
//  Count the leading 1's in A.
//
  leading = 0;
  for ( i = 0; i < m; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      if ( rref[i+j*m] == 1.0 )
      {
        leading = leading + 1;
        break;
      }
    }
  }
  nullspace_size = n - leading;

  return nullspace_size;
}
//****************************************************************************80

double *r8mat_orth_uniform ( int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_ORTH_UNIFORM returns a random orthogonal matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The inverse of A is equal to A'.
//
//    A * A'  = A' * A = I.
//
//    Columns and rows of A have unit Euclidean norm.
//
//    Distinct pairs of columns of A are orthogonal.
//
//    Distinct pairs of rows of A are orthogonal.
//
//    The L2 vector norm of A*x = the L2 vector norm of x for any vector x.
//
//    The L2 matrix norm of A*B = the L2 matrix norm of B for any matrix B.
//
//    The determinant of A is +1 or -1.
//
//    All the eigenvalues of A have modulus 1.
//
//    All singular values of A are 1.
//
//    All entries of A are between -1 and 1.
//
//  Discussion:
//
//    Thanks to Eugene Petrov, B I Stepanov Institute of Physics,
//    National Academy of Sciences of Belarus, for convincingly
//    pointing out the severe deficiencies of an earlier version of
//    this routine.
//
//    Essentially, the computation involves saving the Q factor of the
//    QR factorization of a matrix whose entries are normally distributed.
//    However, it is only necessary to generate this matrix a column at
//    a time, since it can be shown that when it comes time to annihilate
//    the subdiagonal elements of column K, these (transformed) elements of
//    column K are still normally distributed random values.  Hence, there
//    is no need to generate them at the beginning of the process and
//    transform them K-1 times.
//
//    For computational efficiency, the individual Householder transformations
//    could be saved, as recommended in the reference, instead of being
//    accumulated into an explicit matrix format.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 November 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Pete Stewart,
//    Efficient Generation of Random Orthogonal Matrices With an Application
//    to Condition Estimators,
//    SIAM Journal on Numerical Analysis,
//    Volume 17, Number 3, June 1980, pages 403-409.
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8MAT_ORTH_UNIFORM[N*N], the orthogonal matrix.
//
{
  double *a;
  double *a2;
  int i;
  int j;
  double *v;
  double *x;
//
//  Start with A = the identity matrix.
//
  a = r8mat_identity ( n );
//
//  Now behave as though we were computing the QR factorization of
//  some other random matrix.  Generate the N elements of the first column,
//  compute the Householder matrix H1 that annihilates the subdiagonal elements,
//  and set A := A * H1' = A * H.
//
//  On the second step, generate the lower N-1 elements of the second column,
//  compute the Householder matrix H2 that annihilates them,
//  and set A := A * H2' = A * H2 = H1 * H2.
//
//  On the N-1 step, generate the lower 2 elements of column N-1,
//  compute the Householder matrix HN-1 that annihilates them, and
//  and set A := A * H(N-1)' = A * H(N-1) = H1 * H2 * ... * H(N-1).
//  This is our random orthogonal matrix.
//
  x = new double[n];

  for ( j = 1; j < n; j++ )
  {
//
//  Set the vector that represents the J-th column to be annihilated.
//
    for ( i = 1; i < j; i++ )
    {
      x[i-1] = 0.0;
    }
    for ( i = j; i <= n; i++ )
    {
      x[i-1] = r8_normal_01 ( seed );
    }
//
//  Compute the vector V that defines a Householder transformation matrix
//  H(V) that annihilates the subdiagonal elements of X.
//
    v = r8vec_house_column ( n, x, j );
//
//  Postmultiply the matrix A by H'(V) = H(V).
//
    a2 = r8mat_house_axh_new ( n, a, v );

    delete [] v;

    r8mat_copy ( n, n, a2, a );

    delete [] a2;
  }

  delete [] x;

  return a;
}
//****************************************************************************80

void r8mat_plot ( int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_PLOT "plots" an R8MAT, with an optional title.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the matrix.
//
//    Input, char *TITLE, a title to be printed.
//
{
  int i;
  int j;
  int jhi;
  int jlo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  for ( jlo = 1; jlo <= n; jlo = jlo + 70 )
  {
    jhi = i4_min ( jlo + 70-1, n );
    cout << "\n";
    cout << "          ";
    for ( j = jlo; j <= jhi; j++ )
    {
      cout <<  ( j % 10 );
    }
    cout << "\n";
    cout << "\n";

    for ( i = 1; i <= m; i++ )
    {
      cout << setw(6) << i << "    ";
      for ( j = jlo; j <= jhi; j++ )
      {
        cout << r8mat_plot_symbol ( a[i-1+(j-1)*m] );
      }
      cout << "\n";
    }
  }

  return;
}
//****************************************************************************80

char r8mat_plot_symbol ( double r )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_PLOT_SYMBOL returns a symbol for a double precision number.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double R, a value whose symbol is desired.
//
//    Output, char R8MAT_PLOT_SYMBOL, is
//    '-' if R is negative,
//    '0' if R is zero,
//    '+' if R is positive.
//
{
  char c;

  if ( r < 0.0 )
  {
    c = '-';
  }
  else if ( r == 0.0 )
  {
    c = '0';
  }
  else if ( 0.0 < r )
  {
    c = '+';
  }

  return c;
}
//****************************************************************************80

double *r8mat_poly_char ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_POLY_CHAR computes the characteristic polynomial of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix A.
//
//    Input, double A[N*N], the N by N matrix.
//
//    Output, double R8MAT_POLY_CHAR[N+1], the coefficients of the characteristic
//    polynomial of A.  P(N) contains the coefficient of X**N
//    (which will be 1), P(I) contains the coefficient of X**I,
//    and P(0) contains the constant term.
//
{
  int i;
  int order;
  double *p;
  double trace;
  double *work1;
  double *work2;

  p = new double[n+1];
//
//  Initialize WORK1 to the identity matrix.
//
  work1 = r8mat_identity ( n );

  p[n] = 1.0;

  for ( order = n-1; 0 <= order; order-- )
  {
//
//  Work2 = A * WORK1.
//
    work2 = r8mat_mm ( n, n, n, a, work1 );
//
//  Take the trace.
//
    trace = r8mat_trace ( n, work2 );
//
//  P(ORDER) = -Trace ( WORK2 ) / ( N - ORDER )
//
    p[order] = -trace / ( double ) ( n - order );
//
//  WORK1 := WORK2 + P(IORDER) * Identity.
//
    delete [] work1;

    r8mat_copy ( n, n, work2, work1 );

    delete [] work2;

    for ( i = 0; i < n; i++ )
    {
      work1[i+i*n] = work1[i+i*n] + p[order];
    }
  }

  delete [] work1;

  return p;
}
//****************************************************************************80

double *r8mat_power ( int n, double a[], int npow )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_POWER computes a nonnegative power of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The algorithm is:
//
//      B = I
//      do NPOW times:
//        B = A * B
//      end
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input, double A[N*N], the matrix to be raised to a power.
//
//    Input, int NPOW, the power to which A is to be raised.
//    NPOW must be nonnegative.
//
//    Output, double B[N*N], the value of A**NPOW.
//
{
  double *b;
  double *c;
  int ipow;

  if ( npow < 0 )
  {
    cout << "\n";
    cout << "R8MAT_POWER - Fatal error!\n";
    cout << "  Input value of NPOW < 0.\n";
    cout << "  NPOW = " << npow << "\n";
    exit ( 1 );
  }

  b = r8mat_identity ( n );

  for ( ipow = 1; ipow <= npow; ipow++ )
  {
    c = r8mat_mm ( n, n, n, a, b );
    r8mat_copy ( n, n, c, b );
    delete [] c;
  }

  return b;
}
//****************************************************************************80

void r8mat_power_method ( int n, double a[], double *r, double v[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_POWER_METHOD applies the power method to a matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    If the power method has not converged, then calling the routine
//    again immediately with the output from the previous call will
//    continue the iteration.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input, double A[N*N], the matrix.
//
//    Output, double *R, the estimated eigenvalue.
//
//    Input/output, double V[N], on input, an estimate
//    for the eigenvector.  On output, an improved estimate for the
//    eigenvector.
//
{
  double *av;
  double eps;
  int i;
  int it;
  double it_eps = 0.0001;
  int it_max = 100;
  int it_min = 10;
  int j;
  double r2;
  double r_old;

  eps = sqrt ( r8_epsilon ( ) );

  *r = r8vec_length ( n, v );

  if ( *r == 0.0 )
  {
    for ( i = 0; i < n; i++ )
    {
      v[i] = 1.0;
    }
    *r = sqrt ( ( double ) n );
  }

  for ( i = 0; i < n; i++ )
  {
    v[i] = v[i] / *r;
  }

  for ( it = 1; it <= it_max; it++ )
  {
    av = r8mat_mv ( n, n, a, v );

    r_old = *r;
    *r = r8vec_length ( n, av );

    if ( it_min < it )
    {
      if ( r8_abs ( *r - r_old ) <= it_eps * ( 1.0 + r8_abs ( *r ) ) )
      {
        break;
      }
    }

    r8vec_copy ( n, av, v );

    delete [] av;

    if ( *r != 0.0 )
    {
      for ( i = 0; i < n; i++ )
      {
        v[i] = v[i] / *r;
      }
    }
//
//  Perturb V a bit, to avoid cases where the initial guess is exactly
//  the eigenvector of a smaller eigenvalue.
//
    if ( it < it_max / 2 )
    {
      j = ( ( it - 1 ) % n );
      v[j] = v[j] + eps * ( 1.0 + r8_abs ( v[j] ) );
      r2 = r8vec_length ( n, v );
      for ( i = 0; i < n; i++ )
      {
        v[i] = v[i] / r2;
      }
    }
  }
  return;
}
//****************************************************************************80

void r8mat_print ( int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_PRINT prints an R8MAT, with an optional title.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    Entry A(I,J) is stored as A[I+J*M]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[M*N], the M by N matrix.
//
//    Input, char *TITLE, a title to be printed.
//
{
  r8mat_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//****************************************************************************80

void r8mat_print_some ( int m, int n, double a[], int ilo, int jlo, int ihi, 
  int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_PRINT_SOME prints some of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows of the matrix.
//    M must be positive.
//
//    Input, int N, the number of columns of the matrix.
//    N must be positive.
//
//    Input, double A[M*N], the matrix.
//
//    Input, int ILO, JLO, IHI, JHI, designate the first row and
//    column, and the last row and column to be printed.
//
//    Input, char *TITLE, a title for the matrix.
//
{
# define INCX 5

  int i;
  int i2hi;
  int i2lo;
  int j;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }
//
//  Print the columns of the matrix, in strips of 5.
//
  for ( j2lo = jlo; j2lo <= jhi; j2lo = j2lo + INCX )
  {
    j2hi = j2lo + INCX - 1;
    j2hi = i4_min ( j2hi, n );
    j2hi = i4_min ( j2hi, jhi );

    cout << "\n";
//
//  For each column J in the current range...
//
//  Write the header.
//
    cout << "  Col:    ";
    for ( j = j2lo; j <= j2hi; j++ )
    {
      cout << setw(7) << j << "       ";
    }
    cout << "\n";
    cout << "  Row\n";
    cout << "\n";
//
//  Determine the range of the rows in this strip.
//
    i2lo = i4_max ( ilo, 1 );
    i2hi = i4_min ( ihi, m );

    for ( i = i2lo; i <= i2hi; i++ )
    {
//
//  Print out (up to) 5 entries in row I, that lie in the current strip.
//
      cout << setw(5) << i << "  ";
      for ( j = j2lo; j <= j2hi; j++ )
      {
        cout << setw(12) << a[i-1+(j-1)*m] << "  ";
      }
      cout << "\n";
    }

  }

  return;
# undef INCX
}
//****************************************************************************80

void r8mat_ref ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_REF computes the row echelon form of a matrix.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    A matrix is in row echelon form if:
//
//    * The first nonzero entry in each row is 1.
//
//    * The leading 1 in a given row occurs in a column to
//      the right of the leading 1 in the previous row.
//
//    * Rows which are entirely zero must occur last.
//
//  Example:
//
//    Input matrix:
//
//     1.0  3.0  0.0  2.0  6.0  3.0  1.0
//    -2.0 -6.0  0.0 -2.0 -8.0  3.0  1.0
//     3.0  9.0  0.0  0.0  6.0  6.0  2.0
//    -1.0 -3.0  0.0  1.0  0.0  9.0  3.0
//
//    Output matrix:
//
//     1.0  3.0  0.0  2.0  6.0  3.0  1.0
//     0.0  0.0  0.0  1.0  2.0  4.5  1.5
//     0.0  0.0  0.0  0.0  0.0  1.0  0.3
//     0.0  0.0  0.0  0.0  0.0  0.0  0.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of 
//    the matrix A.
//
//    Input/output, double A[M*N].  On input, the matrix to be
//    analyzed.  On output, the REF form of the matrix.
//
{
  int i;
  int j;
  int lead;
  int r;
  double temp;

  lead = 0;

  for ( r = 0; r < m; r++ )
  {
    if ( n - 1 < lead )
    {
      break;
    }

    i = r;

    while ( a[i+lead*m] == 0.0 )
    {
      i = i + 1;

      if ( m - 1 < i )
      {
        i = r;
        lead = lead + 1;
        if ( n - 1 < lead )
        {
          lead = -1;
          break;
         }
      }
    }

    if ( lead < 0 )
    {
      break;
    }

    for ( j = 0; j < n; j++ )
    {
      temp     = a[i+j*m];
      a[i+j*m] = a[r+j*m];
      a[r+j*m] = temp;
    }

    temp = a[r+lead*m];

    for ( j = 0; j < n; j++ )
    {
      a[r+j*m] = a[r+j*m] / temp;
    }

    for ( i = r + 1; i < m; i++ )
    {
      temp = a[i+lead*m];
      for ( j = 0; j < n; j++ )
      {
        a[i+j*m] = a[i+j*m] - temp * a[r+j*m];
      }
    }
    lead = lead + 1;
  }
  return;
}
//****************************************************************************80

void r8mat_rref ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_RREF computes the reduced row echelon form of a matrix.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    A matrix is in row echelon form if:
//
//    * The first nonzero entry in each row is 1.
//
//    * The leading 1 in a given row occurs in a column to
//      the right of the leading 1 in the previous row.
//
//    * Rows which are entirely zero must occur last.
//
//    The matrix is in reduced row echelon form if, in addition to
//    the first three conditions, it also satisfies:
//
//    * Each column containing a leading 1 has no other nonzero entries.
//
//  Example:
//
//    Input matrix:
//
//     1.0  3.0  0.0  2.0  6.0  3.0  1.0
//    -2.0 -6.0  0.0 -2.0 -8.0  3.0  1.0
//     3.0  9.0  0.0  0.0  6.0  6.0  2.0
//    -1.0 -3.0  0.0  1.0  0.0  9.0  3.0
//
//    Output matrix:
//
//     1.0  3.0  0.0  0.0  2.0  0.0  0.0
//     0.0  0.0  0.0  1.0  2.0  0.0  0.0
//     0.0  0.0  0.0  0.0  0.0  1.0  0.3
//     0.0  0.0  0.0  0.0  0.0  0.0  0.0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of 
//    the matrix A.
//
//    Input/output, double A[M*N].  On input, the matrix to be
//    analyzed.  On output, the RREF form of the matrix.
//
{
  int i;
  int j;
  int lead;
  int r;
  double temp;

  lead = 0;

  for ( r = 0; r < m; r++ )
  {
    if ( n - 1 < lead )
    {
      break;
    }

    i = r;

    while ( a[i+lead*m] == 0.0 )
    {
      i = i + 1;

      if ( m - 1 < i )
      {
        i = r;
        lead = lead + 1;
        if ( n - 1 < lead )
        {
          lead = -1;
          break;
         }
      }
    }

    if ( lead < 0 )
    {
      break;
    }

    for ( j = 0; j < n; j++ )
    {
      temp     = a[i+j*m];
      a[i+j*m] = a[r+j*m];
      a[r+j*m] = temp;
    }

    temp = a[r+lead*m];

    for ( j = 0; j < n; j++ )
    {
      a[r+j*m] = a[r+j*m] / temp;
    }

    for ( i = 0; i < m; i++ )
    {
      if ( i != r )
      {
        temp = a[i+lead*m];
        for ( j = 0; j < n; j++ )
        {
          a[i+j*m] = a[i+j*m] - temp * a[r+j*m];
        }
      }
    }
    lead = lead + 1;

  }
  return;
}
//****************************************************************************80

int r8mat_solve ( int n, int rhs_num, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SOLVE uses Gauss-Jordan elimination to solve an N by N linear system.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    Entry A(I,J) is stored as A[I+J*N]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input, int RHS_NUM, the number of right hand sides.  RHS_NUM
//    must be at least 0.
//
//    Input/output, double A[N*(N+RHS_NUM)], contains in rows and columns 1
//    to N the coefficient matrix, and in columns N+1 through
//    N+RHS_NUM, the right hand sides.  On output, the coefficient matrix
//    area has been destroyed, while the right hand sides have
//    been overwritten with the corresponding solutions.
//
//    Output, int R8MAT_SOLVE, singularity flag.
//    0, the matrix was not singular, the solutions were computed;
//    J, factorization failed on step J, and the solutions could not
//    be computed.
//
{
  double apivot;
  double factor;
  int i;
  int ipivot;
  int j;
  int k;
  double temp;

  for ( j = 0; j < n; j++ )
  {
//
//  Choose a pivot row.
//
    ipivot = j;
    apivot = a[j+j*n];

    for ( i = j; i < n; i++ )
    {
      if ( r8_abs ( apivot ) < r8_abs ( a[i+j*n] ) )
      {
        apivot = a[i+j*n];
        ipivot = i;
      }
    }

    if ( apivot == 0.0 )
    {
      return j;
    }
//
//  Interchange.
//
    for ( i = 0; i < n + rhs_num; i++ )
    {
      temp          = a[ipivot+i*n];
      a[ipivot+i*n] = a[j+i*n];
      a[j+i*n]      = temp;
    }
//
//  A(J,J) becomes 1.
//
    a[j+j*n] = 1.0;
    for ( k = j; k < n + rhs_num; k++ )
    {
      a[j+k*n] = a[j+k*n] / apivot;
    }
//
//  A(I,J) becomes 0.
//
    for ( i = 0; i < n; i++ )
    {
      if ( i != j )
      {
        factor = a[i+j*n];
        a[i+j*n] = 0.0;
        for ( k = j; k < n + rhs_num; k++ )
        {
          a[i+k*n] = a[i+k*n] - factor * a[j+k*n];
        }
      }
    }
  }

  return 0;
}
//****************************************************************************80

double *r8mat_solve_2d ( double a[], double b[], double *det )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SOLVE_2D solves a 2 by 2 linear system using Cramer's rule.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    If the determinant DET is returned as zero, then the matrix A is
//    singular, and does not have an inverse.  In that case, X is
//    returned as the NULL vector.
//
//    If DET is nonzero, then its value is roughly an estimate
//    of how nonsingular the matrix A is.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 November 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[2*2], the matrix.
//
//    Input, double B[2], the right hand side.
//
//    Output, double *DET, the determinant of the system.
//
//    Output, double R8MAT_SOLVE_2D[2], the solution of the system, 
//    if DET is nonzero.  Otherwise, the NULL vector.
//
{
  double *x;
//
//  Compute the determinant.
//
  *det = a[0+0*2] * a[1+1*2] - a[0+1*2] * a[1+0*2];
//
//  If the determinant is zero, bail out.
//
  if ( *det == 0.0 )
  {
    return NULL;
  }
//
//  Compute the solution.
//
  x = new double[2];

  x[0] = (  a[1+1*2] * b[0] - a[0+1*2] * b[1] ) / ( *det );
  x[1] = ( -a[1+0*2] * b[0] + a[0+0*2] * b[1] ) / ( *det );

  return x;
}
//****************************************************************************80

double *r8mat_solve_3d ( double a[], double b[], double *det )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SOLVE_3D solves a 3 by 3 linear system using Cramer's rule.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    If the determinant DET is returned as zero, then the matrix A is
//    singular, and does not have an inverse.  In that case, X is
//    returned as the NULL vector.
//
//    If DET is nonzero, then its value is roughly an estimate
//    of how nonsingular the matrix A is.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 December 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A[3*3], the matrix.
//
//    Input, double B[3], the right hand side.
//
//    Output, double *DET, the determinant of the system.
//
//    Output, double R8MAT_SOLVE_3D[3], the solution of the system, 
//    if DET is nonzero.  Otherwise, the NULL vector.
//
{
  double *x;
//
//  Compute the determinant.
//
  *det =  a[0+0*3] * ( a[1+1*3] * a[2+2*3] - a[1+2*3] * a[2+1*3] ) 
        + a[0+1*3] * ( a[1+2*3] * a[2+0*3] - a[1+0*3] * a[2+2*3] ) 
        + a[0+2*3] * ( a[1+0*3] * a[2+1*3] - a[1+1*3] * a[2+0*3] );
//
//  If the determinant is zero, bail out.
//
  if ( *det == 0.0 )
  {
    return NULL;
  }
//
//  Compute the solution.
//
  x = new double[3];

  x[0] = (   ( a[1+1*3] * a[2+2*3] - a[1+2*3] * a[2+1*3] ) * b[0] 
           - ( a[0+1*3] * a[2+2*3] - a[0+2*3] * a[2+1*3] ) * b[1] 
           + ( a[0+1*3] * a[1+2*3] - a[0+2*3] * a[1+1*3] ) * b[2] ) / ( *det );

  x[1] = ( - ( a[1+0*3] * a[2+2*3] - a[1+2*3] * a[2+0*3] ) * b[0] 
           + ( a[0+0*3] * a[2+2*3] - a[0+2*3] * a[2+0*3] ) * b[1] 
           - ( a[0+0*3] * a[1+2*3] - a[0+2*3] * a[1+0*3] ) * b[2] ) / ( *det );

  x[2] = (   ( a[1+0*3] * a[2+1*3] - a[1+1*3] * a[2+0*3] ) * b[0] 
           - ( a[0+0*3] * a[2+1*3] - a[0+1*3] * a[2+0*3] ) * b[1] 
           + ( a[0+0*3] * a[1+1*3] - a[0+1*3] * a[1+0*3] ) * b[2] ) / ( *det );

  return x;
}
//****************************************************************************80

double *r8mat_solve2 ( int n, double a[], double b[], int *ierror )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SOLVE2 computes the solution of an N by N linear system.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The linear system may be represented as
//
//      A*X = B
//
//    If the linear system is singular, but consistent, then the routine will
//    still produce a solution.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of equations.
//
//    Input/output, double A[N*N].
//    On input, A is the coefficient matrix to be inverted.
//    On output, A has been overwritten.
//
//    Input/output, double B[N].
//    On input, B is the right hand side of the system.
//    On output, B has been overwritten.
//
//    Output, double R8MAT_SOLVE2[N], the solution of the linear system.
//
//    Output, int *IERROR.
//    0, no error detected.
//    1, consistent singularity.
//    2, inconsistent singularity.
//
{
  double amax;
  int i;
  int imax;
  int *ipiv;
  int j;
  int k;
  double *x;

  *ierror = 0;

  ipiv = i4vec_zero_new ( n );
  x = r8vec_zero_new ( n );
//
//  Process the matrix.
//
  for ( k = 1; k <= n; k++ )
  {
//
//  In column K:
//    Seek the row IMAX with the properties that:
//      IMAX has not already been used as a pivot;
//      A(IMAX,K) is larger in magnitude than any other candidate.
//
    amax = 0.0;
    imax = 0;
    for ( i = 1; i <= n; i++ )
    {
      if ( ipiv[i-1] == 0 )
      {
        if ( amax < r8_abs ( a[i-1+(k-1)*n] ) )
        {
          imax = i;
          amax = r8_abs ( a[i-1+(k-1)*n] );
        }
      }
    }
//
//  If you found a pivot row IMAX, then,
//    eliminate the K-th entry in all rows that have not been used for pivoting.
//
    if ( imax != 0 )
    {
      ipiv[imax-1] = k;
      for ( j = k+1; j <= n; j++ )
      {
        a[imax-1+(j-1)*n] = a[imax-1+(j-1)*n] / a[imax-1+(k-1)*n];
      }
      b[imax-1] = b[imax-1] / a[imax-1+(k-1)*n];
      a[imax-1+(k-1)*n] = 1.0;

      for ( i = 1; i <= n; i++ )
      {
        if ( ipiv[i-1] == 0 )
        {
          for ( j = k+1; j <= n; j++ )
          {
            a[i-1+(j-1)*n] = a[i-1+(j-1)*n] - a[i-1+(k-1)*n] * a[imax-1+(j-1)*n];
          }
          b[i-1] = b[i-1] - a[i-1+(k-1)*n] * b[imax-1];
          a[i-1+(k-1)*n] = 0.0;
        }
      }
    }
  }
//
//  Now, every row with nonzero IPIV begins with a 1, and
//  all other rows are all zero.  Begin solution.
//
  for ( j = n; 1 <= j; j-- )
  {
    imax = 0;
    for ( k = 1; k <= n; k++ )
    {
      if ( ipiv[k-1] == j )
      {
        imax = k;
      }
    }

    if ( imax == 0 )
    {
      x[j-1] = 0.0;

      if ( b[j-1] == 0.0 )
      {
        *ierror = 1;
        cout << "\n";
        cout << "R8MAT_SOLVE2 - Warning:\n";
        cout << "  Consistent singularity, equation = " << j << "\n";
      }
      else
      {
        *ierror = 2;
        cout << "\n";
        cout << "R8MAT_SOLVE2 - Error:\n";
        cout << "  Inconsistent singularity, equation = " << j << "\n";
      }
    }
    else
    {
      x[j-1] = b[imax-1];

      for ( i = 1; i <= n; i++ )
      {
        if ( i != imax )
        {
          b[i-1] = b[i-1] - a[i-1+(j-1)*n] * x[j-1];
        }
      }
    }
  }

  delete [] ipiv;

  return x;
}
//****************************************************************************80

double *r8mat_symm_eigen ( int n, double x[], double q[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SYMM_EIGEN returns a symmetric matrix with given eigensystem.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The user must supply the desired eigenvalue vector, and the desired
//    eigenvector matrix.  The eigenvector matrix must be orthogonal.  A
//    suitable random orthogonal matrix can be generated by R8MAT_ORTH_UNIFORM. 
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input, double X[N], the desired eigenvalues for the matrix.
//
//    Input, double Q[N*N], the eigenvector matrix of A.
//
//    Output, double R8MAT_SYMM_EIGEN[N*N], a symmetric N by N matrix with
//    eigenvalues X and eigenvectors the columns of Q.
//
{
  double *a;
  int i;
  int j;
  int k;
//
//  Set A = Q * Lambda * Q'.
//
  a = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      a[i+j*n] = 0.0;
      for ( k = 0; k < n; k++ )
      {
        a[i+j*n] = a[i+j*n] + q[i+k*n] * x[k] * q[j+k*n];
      }
    }
  }

  return a;
}
//****************************************************************************80

void r8mat_symm_jacobi ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_SYMM_JACOBI applies Jacobi eigenvalue iteration to a symmetric matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    This code was modified so that it treats as zero the off-diagonal
//    elements that are sufficiently close to, but not exactly, zero.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of A.
//
//    Input/output, double A[N*N], a symmetric N by N matrix.
//    On output, the matrix has been overwritten by an approximately
//    diagonal matrix, with the eigenvalues on the diagonal.
//
{
  double c;
  double eps = 0.00001;
  int i;
  int it;
  int it_max = 100;
  int j;
  int k;
  double norm_fro;
  double s;
  double sum2;
  double t;
  double t1;
  double t2;
  double u;

  norm_fro = r8mat_norm_fro ( n, n, a );

  it = 0;

  for ( ; ; )
  {
    it = it + 1;

    for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < i; j++ )
      {
        if ( eps * norm_fro < r8_abs ( a[i+j*n] ) + r8_abs ( a[j+i*n] ) )
        {
          u = ( a[j+j*n] - a[i+i*n] ) / ( a[i+j*n] + a[j+i*n] );

          t = r8_sign ( u ) / ( r8_abs ( u ) + sqrt ( u * u + 1.0 ) );
          c = 1.0 / sqrt ( t * t + 1.0 );
          s = t * c;
//
//  A -> A * Q.
//
          for ( k = 0; k < n; k++ )
          {
            t1 = a[i+k*n];
            t2 = a[j+k*n];
            a[i+k*n] = t1 * c - t2 * s;
            a[j+k*n] = t1 * s + t2 * c;
          }
//
//  A -> QT * A
//
          for ( k = 0; k < n; k++ )
          {
            t1 = a[k+i*n];
            t2 = a[k+j*n];
            a[k+i*n] = c * t1 - s * t2;
            a[k+j*n] = s * t1 + c * t2;
          }
        }
      }
    }
//
//  Test the size of the off-diagonal elements.
//
    sum2 = 0.0;
    for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < i; j++ )
      {
        sum2 = sum2 + r8_abs ( a[i+j*n] );
      }
    }

    if ( sum2 <= eps * ( norm_fro + 1.0 ) )
    {
      break;
    }

    if ( it_max <= it )
    {
      break;
    }

  }

  return;
}
//****************************************************************************80

int r8mat_to_r8plu ( int n, double a[], int pivot[], double lu[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TO_R8PLU factors a general matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    This routine is a simplified version of the LINPACK routine DGEFA.
//    Fortran conventions are used to index doubly-dimensioned arrays.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Jack Dongarra, Jim Bunch, Cleve Moler, Pete Stewart,
//    LINPACK User's Guide,
//    SIAM, 1979
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input, double A[N*N], the matrix to be factored.
//
//    Output, int PIVOT(N), a vector of pivot indices.
//
//    Output, double LU[N*N], an upper triangular matrix U and the multipliers
//    L which were used to obtain it.  The factorization can be written
//    A = L * U, where L is a product of permutation and unit lower
//    triangular matrices and U is upper triangular.
//
//    Output, int R8MAT_TO_R8PLU, singularity flag.
//    0, no singularity detected.
//    nonzero, the factorization failed on the R8MAT_TO_R8PLU-th step.
//
{
  int i;
  int info;
  int j;
  int k;
  int l;
  double temp;
//
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      lu[i+j*n] = a[i+j*n];
    }
  }
  info = 0;

  for ( k = 1; k <= n-1; k++ )
  {
//
//  Find L, the index of the pivot row.
//
    l = k;
    for ( i = k+1; i <= n; i++ )
    {
      if ( r8_abs ( lu[l-1+(k-1)*n] ) < r8_abs ( lu[i-1+(k-1)*n] ) )
      {
        l = i;
      }
    }

    pivot[k-1] = l;
//
//  If the pivot index is zero, the algorithm has failed.
//
    if ( lu[l-1+(k-1)*n] == 0.0 )
    {
      info = k;
      return info;
    }
//
//  Interchange rows L and K if necessary.
//
    if ( l != k )
    {
      temp            = lu[l-1+(k-1)*n];
      lu[l-1+(k-1)*n] = lu[k-1+(k-1)*n];
      lu[k-1+(k-1)*n] = temp;
    }
//
//  Normalize the values that lie below the pivot entry A(K,K).
//
    for ( i = k+1; i <= n; i++ )
    {
      lu[i-1+(k-1)*n] = -lu[i-1+(k-1)*n] / lu[k-1+(k-1)*n];
    }
//
//  Row elimination with column indexing.
//
    for ( j = k+1; j <= n; j++ )
    {
      if ( l != k )
      {
        temp            = lu[l-1+(j-1)*n];
        lu[l-1+(j-1)*n] = lu[k-1+(j-1)*n];
        lu[k-1+(j-1)*n] = temp;
      }

      for ( i = k+1; i <= n; i++ )
      {
        lu[i-1+(j-1)*n] = lu[i-1+(j-1)*n] + lu[i-1+(k-1)*n] * lu[k-1+(j-1)*n];
      }
    }
  }

  pivot[n-1] = n;

  if ( lu[n-1+(n-1)*n] == 0.0 )
  {
    info = n;
  }

  return info;
}
//****************************************************************************80

double r8mat_trace ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRACE computes the trace of an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The trace of a square matrix is the sum of the diagonal elements.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix A.
//
//    Input, double A[N*N], the matrix whose trace is desired.
//
//    Output, double R8MAT_TRACE, the trace of the matrix.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a[i+i*n];
  }

  return value;
}
//****************************************************************************80

double *r8mat_transpose ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE returns the transpose of a matrix.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns of the matrix A.
//
//    Input, double A[M*N], the matrix whose transpose is desired.
//
//    Output, double R8MAT_TRANSPOSE[N*M], the transposed matrix.
//
{
  double *b;
  int i;
  int j;

  b = new double[n*m];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      b[j+i*n] = a[i+j*m];
    }
  }
  return b;
}
//****************************************************************************80

void r8mat_transpose_in_place ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE_IN_PLACE transposes a square matrix in place.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of rows and columns of the matrix A.
//
//    Input/output, double A[N*N], the matrix to be transposed.  
//
{
  int i;
  int j;
  double t;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < j; i++ )
    {
      t        = a[i+j*n];
      a[i+j*n] = a[j+i*n];
      a[j+i*n] = t;
    }
  }
  return;
}
//****************************************************************************80

void r8mat_transpose_print ( int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE_PRINT prints an R8MAT, transposed.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], an M by N matrix to be printed.
//
//    Input, char *TITLE, an optional title.
//
{
  r8mat_transpose_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
//****************************************************************************80

void r8mat_transpose_print_some ( int m, int n, double a[], int ilo, int jlo, 
  int ihi, int jhi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_TRANSPOSE_PRINT_SOME prints some of an R8MAT, transposed.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], an M by N matrix to be printed.
//
//    Input, int ILO, JLO, the first row and column to print.
//
//    Input, int IHI, JHI, the last row and column to print.
//
//    Input, char *TITLE, an optional title.
//
{
# define INCX 5

  int i;
  int i2;
  int i2hi;
  int i2lo;
  int inc;
  int j;
  int j2hi;
  int j2lo;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  for ( i2lo = i4_max ( ilo, 1 ); i2lo <= i4_min ( ihi, m ); i2lo = i2lo + INCX )
  {
    i2hi = i2lo + INCX - 1;
    i2hi = i4_min ( i2hi, m );
    i2hi = i4_min ( i2hi, ihi );

    inc = i2hi + 1 - i2lo;

    cout << "\n";
    cout << "  Row: ";
    for ( i = i2lo; i <= i2hi; i++ )
    {
      cout << setw(7) << i << "       ";
    }
    cout << "\n";
    cout << "  Col\n";
    cout << "\n";

    j2lo = i4_max ( jlo, 1 );
    j2hi = i4_min ( jhi, n );

    for ( j = j2lo; j <= j2hi; j++ )
    {
      cout << setw(5) << j << " ";
      for ( i2 = 1; i2 <= inc; i2++ )
      {
        i = i2lo - 1 + i2;
        cout << setw(14) << a[(i-1)+(j-1)*m];
      }
      cout << "\n";
    }
  }
  cout << "\n";

  return;
# undef INCX
}
//****************************************************************************80

double *r8mat_u_inverse ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_U_INVERSE inverts an upper triangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    An upper triangular matrix is a matrix whose only nonzero entries
//    occur on or above the diagonal.
//
//    The inverse of an upper triangular matrix is an upper triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2005
//
//  Author:
//
//    FORTRAN77 original version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, double A[N*N], the upper triangular matrix.
//
//    Output, double R8MAT_U_INVERSE[N*N], the inverse matrix.
//
{
  double *b;
  int i;
  int j;
  int k;

  b = new double[n*n];

  for ( j = n-1; 0 <= j; j-- )
  {
    for ( i = n-1; 0 <= i; i-- )
    {
      if ( j < i )
      {
        b[i+j*n] = 0.0;
      }
      else if ( i == j )
      {
        b[i+j*n] = 1.0 / a[i+j*n];
      }
      else
      {
        b[i+j*n] = 0.0;
        for ( k = i+1; k <= j; k++ )
        {
          b[i+j*n] = b[i+j*n] - a[i+k*n] * b[k+j*n];
        }
       b[i+j*n] = b[i+j*n] / a[i+i*n];
      }
    }
  }

  return b;
}
//****************************************************************************80

double *r8mat_u1_inverse ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_U1_INVERSE inverts a unit upper triangular R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    A unit upper triangular matrix is a matrix with only 1's on the main
//    diagonal, and only 0's below the main diagonal.
//
//    The inverse of a unit upper triangular matrix is also
//    a unit upper triangular matrix.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2005
//
//  Author:
//
//    C++ translation by John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, number of rows and columns in the matrix.
//
//    Input, double A[N*N], the unit upper triangular matrix.
//
//    Output, double R8MAT_U1_INVERSE[N*N), the inverse matrix.
//
{
  double *b;
  int i;
  int j;
  int k;

  b = new double[n*n];

  for ( j = n-1; 0 <= j; j-- )
  {
    for ( i = n-1; 0 <= i; i-- )
    {
      if ( j < i )
      {
        b[i+j*n] = 0.0;
      }
      else if ( i == j )
      {
        b[i+j*n] = 1.0;
      }
      else
      {
        b[i+j*n] = 0.0;
        for ( k = i+1; k <= j; k++ )
        {
          b[i+j*n] = b[i+j*n] - a[i+k*n] * b[k+j*n];
        }
       b[i+j*n] = b[i+j*n] / a[i+i*n];
      }
    }
  }

  return b;
}
//****************************************************************************80

double *r8mat_uniform ( int m, int n, double a, double b, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_UNIFORM fills a double precision array with scaled pseudorandom values.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      unif = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Philip Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A, B, the limits of the pseudorandom values.
//
//    Input/output, int *SEED, the "seed" value.  Normally, this
//    value should not be 0, otherwise the output value of SEED
//    will still be 0, and R8_UNIFORM will be 0.  On output, SEED has 
//    been updated.
//
//    Output, double R8MAT_UNIFORM[M*N], a matrix of pseudorandom values.
//
{
  int i;
  int j;
  int k;
  double *r;

  r = new double[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      k = *seed / 127773;

      *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

      if ( *seed < 0 )
      {
        *seed = *seed + 2147483647;
      }
//
//  Although SEED can be represented exactly as a 32 bit integer,
//  it generally cannot be represented exactly as a 32 bit real number//
//
      r[i+j*m] = a + ( b - a ) * ( double ) ( *seed ) * 4.656612875E-10;
    }
  }

  return r;
}
//****************************************************************************80

double *r8mat_uniform_01 ( int m, int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_UNIFORM_01 fills a double precision array with unit pseudorandom values.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      unif = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//    Philip Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, pages 136-143, 1969.
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, int *SEED, the "seed" value.  Normally, this
//    value should not be 0, otherwise the output value of SEED
//    will still be 0, and R8_UNIFORM will be 0.  On output, SEED has 
//    been updated.
//
//    Output, double R8MAT_UNIFORM_01[M*N], a matrix of pseudorandom values.
//
{
  int i;
  int j;
  int k;
  double *r;

  r = new double[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      k = *seed / 127773;

      *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

      if ( *seed < 0 )
      {
        *seed = *seed + 2147483647;
      }
//
//  Although SEED can be represented exactly as a 32 bit integer,
//  it generally cannot be represented exactly as a 32 bit real number//
//
      r[i+j*m] = ( double ) ( *seed ) * 4.656612875E-10;
    }
  }

  return r;
}
//****************************************************************************80

double *r8mat_vand2 ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_VAND2 returns the N by N row Vandermonde matrix A.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//    The row Vandermonde matrix returned by this routine reads "across"
//    rather than down.  In particular, each row begins with a 1, followed by
//    some value X, followed by successive powers of X.
//
//  Formula:
//
//    A(I,J) = X(I)**(J-1)
//
//  Properties:
//
//    A is nonsingular if, and only if, the X values are distinct.
//
//    The determinant of A is
//
//      det(A) = product ( 2 <= I <= N ) (
//        product ( 1 <= J <= I-1 ) ( ( X(I) - X(J) ) ) ).
//
//    The matrix A is generally ill-conditioned.
//
//  Example:
//
//    N = 5, X = (2, 3, 4, 5, 6)
//
//    1 2  4   8   16
//    1 3  9  27   81
//    1 4 16  64  256
//    1 5 25 125  625
//    1 6 36 216 1296
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix desired.
//
//    Input, double X[N], the values that define A.
//
//    Output, double R8MAT_VAND2[N*N], the N by N row Vandermonde matrix.
//
{
  double *a;
  int i;
  int j;

  a = new double[n*n];

  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      if ( j == 0 && x[i] == 0.0 )
      {
        a[i+j*n] = 1.0;
      }
      else
      {
        a[i+j*n] = pow ( x[i], j );
      }
    }
  }

  return a;
}
//****************************************************************************80

void r8mat_zero ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_ZERO zeroes an R8MAT.
//
//  Discussion: 							    
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Output, double A[M*N], a matrix of zeroes.
//
{
  int i;
  int j;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = 0.0;
    }
  }
  return;
}
//****************************************************************************80

double *r8mat_zero_new ( int m, int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8MAT_ZERO_NEW returns a new zeroed R8MAT.
//
//  Discussion:
//
//    An R8MAT is a doubly dimensioned array of R8 values,  stored as a vector 
//    in column-major order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Output, double R8MAT_ZERO[M*N], the new zeroed matrix.
//
{
  double *a;
  int i;
  int j;

  a = new double[m*n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a[i+j*m] = 0.0;
    }
  }
  return a;
}
//*****************************************************************************

double r8plu_det ( int n, int pivot[], double lu[] )

//*****************************************************************************
//
//  Purpose:
//
//    R8PLU_DET computes the determinant of a real PLU matrix.
//
//  Discussion:
//
//    The matrix should have been factored by R8MAT_TO_R8PLU.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Jack Dongarra, Jim Bunch, Cleve Moler, Pete Stewart,
//    LINPACK User's Guide,
//    SIAM, 1979
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input, int PIVOT[N], the pivot vector computed by R8MAT_TO_R8PLU.
//
//    Input, double LU[N*N], the LU factors computed by R8MAT_TO_R8PLU.
//
//    Output, double R8PLU_DET, the determinant of the matrix.
//
{
  double det;
  int i;

  det = 1.0;

  for ( i = 0; i < n; i++ )
  {
    det = det * lu[i+i*n];
    if ( pivot[i] != i+1 )
    {
      det = -det;
    }
  }

  return det;
}
//****************************************************************************80

void r8plu_inverse ( int n, int pivot[], double lu[], double a_inverse[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8PLU_INVERSE computes the inverse of a real PLU matrix.
//
//  Discussion:
//
//    The matrix should have been factored by R8MAT_TO_R8PLU.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix A.
//
//    Input, int PIVOT[N], the pivot vector from R8MAT_TO_R8PLU.
//
//    Input, double LU[N*N], the LU factors computed by R8MAT_TO_R8PLU.
//
//    Output, double A_INVERSE[N*N], the inverse of the original matrix
//    A that was factored by R8MAT_TO_R8PLU.
//
{
  int i;
  int j;
  int k;
  double temp;
  double *work;
//
  work = new double[n];

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      a_inverse[i+j*n] = lu[i+j*n];
    }
  }
//
//  Compute Inverse(U).
//
  for ( k = 1; k <= n; k++ )
  {
    a_inverse[k-1+(k-1)*n]     = 1.0 / a_inverse[k-1+(k-1)*n];
    for ( i = 1; i <= k-1; i++ )
    {
      a_inverse[i-1+(k-1)*n] = -a_inverse[i-1+(k-1)*n] * a_inverse[k-1+(k-1)*n];
    }

    for ( j = k+1; j <= n; j++ )
    {
      temp                     = a_inverse[k-1+(j-1)*n];
      a_inverse[k-1+(j-1)*n]   = 0.0;
      for ( i = 1; i <= k; i++ )
      {
        a_inverse[i-1+(j-1)*n] = a_inverse[i-1+(j-1)*n] 
          + temp * a_inverse[i-1+(k-1)*n];
      }
    }
  }
//
//  Form Inverse(U) * Inverse(L).
//
  for ( k = n-1; 1 <= k; k-- )
  {
    for ( i = k+1; i <= n; i++ )
    {
      work[i-1] = a_inverse[i-1+(k-1)*n];
      a_inverse[i-1+(k-1)*n] = 0.0;
    }

    for ( j = k+1; j <= n; j++ )
    {
      for ( i = 1; i <= n; i++ )
      {
        a_inverse[i-1+(k-1)*n] = a_inverse[i-1+(k-1)*n] 
          + a_inverse[i-1+(j-1)*n] * work[j-1];
      }
    }

    if ( pivot[k-1] != k )
    {
      for ( i = 1; i <= n; i++ )
      {
        temp                            = a_inverse[i-1+(k-1)*n];
        a_inverse[i-1+(k-1)*n]          = a_inverse[i-1+(pivot[k-1]-1)*n];
        a_inverse[i-1+(pivot[k-1]-1)*n] = temp;
      }
    }
  }

  delete [] work;

  return;
}
//****************************************************************************80

void r8plu_mul ( int n, int pivot[], double lu[], double x[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8PLU_MUL computes A * x using the PLU factors of A.
//
//  Discussion:
//
//    It is assumed that R8MAT_TO_R8PLU has computed the PLU factors of
//    the matrix A.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input, int PIVOT(N), the pivot vector computed by R8MAT_TO_R8PLU.
//
//    Input, double LU(N,N), the matrix factors computed by R8MAT_TO_R8PLU.
//
//    Input, double X(N), the vector to be multiplied.
//
//    Output, double B(N), the result of the multiplication.
//
{
  int i;
  int j;
  int k;
  double temp;
//
  for ( i = 0; i < n; i++ )
  {
    b[i] = x[i];
  }
//
//  Y = U * X.
//
  for ( j = 1; j <= n; j++ )
  {
    for ( i = 0; i < j-1; i++ )
    {
      b[i] = b[i] + lu[i+(j-1)*n] * b[j-1];
    }
    b[j-1] = lu[j-1+(j-1)*n] * b[j-1];
  }
//
//  B = PL * Y = PL * U * X = A * x.
//
  for ( j = n-1; 1 <= j; j-- )
  {
    for ( i = j; i < n; i++ )
    {
      b[i] = b[i] - lu[i+(j-1)*n] * b[j-1];
    }

    k = pivot[j-1];

    if ( k != j )
    {
      temp = b[k-1];
      b[k-1] = b[j-1];
      b[j-1] = temp;
    }
  }

  return;
}
//****************************************************************************80

void r8plu_sol ( int n, int pivot[], double lu[], double b[], double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8PLU_SOL solves a linear system A*x=b from the PLU factors.
//
//  Discussion:
//
//    The PLU factors should have been computed by R8MAT_TO_R8PLU.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input, int PIVOT[N], the pivot vector from R8MAT_TO_R8PLU.
//
//    Input, double LU[N*N], the LU factors from R8MAT_TO_R8PLU.
//
//    Input, double B[N], the right hand side vector.
//
//    Output, double X[N], the solution vector.
//
{
  int i;
  int j;
  int k;
  double temp;
//
//  Solve PL * Y = B.
//
  for ( i = 0; i < n; i++ )
  {
    x[i] = b[i];
  }

  for ( k = 1; k <= n-1; k++ )
  {
    j = pivot[k-1];

    if ( j != k )
    {
      temp   = x[j-1];
      x[j-1] = x[k-1];
      x[k-1] = temp;
    }

    for ( i = k+1; i <= n; i++ )
    {
      x[i-1] = x[i-1] + lu[i-1+(k-1)*n] * x[k-1];
    }
  }
//
//  Solve U * X = Y.
//
  for ( k = n; 1 <= k; k-- )
  {
    x[k-1] = x[k-1] / lu[k-1+(k-1)*n];
    for ( i = 1; i <= k-1; i++ )
    {
      x[i-1] = x[i-1] - lu[i-1+(k-1)*n] * x[k-1];
    }
  }

  return;
}
//****************************************************************************80

void r8plu_to_r8mat ( int n, int pivot[], double lu[], double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8PLU_TO_R8MAT recovers the matrix A that was factored by R8MAT_TO_R8PLU.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input, int PIVOT[N], the pivot vector computed by R8MAT_TO_R8PLU.
//
//    Input, double LU[N*N], the matrix factors computed by R8MAT_TO_R8PLU.
//
//    Output, double A[N*N], the matrix whose factors are represented by
//    LU and PIVOT.
//
{
  int i;
  int j;
  int k;
  double temp;

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      if ( i == j )
      {
        a[i+j*n] = 1.0;
      }
      else
      {
        a[i+j*n] = 0.0;
      }
    }
  }

  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= n; i++ )
    {
      for ( k = 1; k <= i-1; k++ )
      {
        a[k-1+(j-1)*n] = a[k-1+(j-1)*n] + lu[k-1+(i-1)*n] * a[i-1+(j-1)*n];
      }
      a[i-1+(j-1)*n] = lu[i-1+(i-1)*n] * a[i-1+(j-1)*n];
    }
//
//  B = PL * Y = PL * U * X = A * x.
//
    for ( i = n-1; 1 <= i; i-- )
    {
      for ( k = i+1; k <= n; k++ )
      {
        a[k-1+(j-1)*n] = a[k-1+(j-1)*n] - lu[k-1+(i-1)*n] * a[i-1+(j-1)*n];
      }

      k = pivot[i-1];

      if ( k != i )
      {
        temp           = a[k-1+(j-1)*n];
        a[k-1+(j-1)*n] = a[i-1+(j-1)*n];
        a[i-1+(j-1)*n] = temp;
      }
    }
  }

  return;
}
//****************************************************************************80

int r8poly_degree ( int na, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_DEGREE returns the degree of a polynomial.
//
//  Discussion:
//
//    The degree of a polynomial is the index of the highest power
//    of X with a nonzero coefficient.
//
//    The degree of a constant polynomial is 0.  The degree of the
//    zero polynomial is debatable, but this routine returns the
//    degree as 0.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NA, the dimension of A.
//
//    Input, double A[NA+1], the coefficients of the polynomials.
//
//    Output, int R8POLY_DEGREE, the degree of A.
//
{
  int degree;

  degree = na;

  while ( 0 < degree )
  {
    if ( a[degree] != 0.0 )
    {
      return degree;
    }
    degree = degree - 1;
  }

  return degree;
}
//****************************************************************************80

double *r8poly_deriv ( int n, double c[], int p )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_DERIV returns the derivative of a polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the degree of the polynomial.
//
//    Input, double C[N+1], the polynomial coefficients.
//    C(I) is the coefficient of X**I.
//
//    Input, int P, the order of the derivative.
//    0 means no derivative is taken.
//    1 means first derivative,
//    2 means second derivative and so on.
//    Values of P less than 0 are meaningless.  Values of P greater
//    than N are meaningful, but the code will behave as though the
//    value of P was N+1.
//
//    Output, double R8POLY_DERIV CP[N-P+1], the polynomial coefficients of
//    the derivative.
//
{
  double *cp;
  double *cp_temp;
  int d;
  int i;

  if ( n < p )
  {
    return NULL;
  }
  cp_temp = r8vec_copy_new ( n+1, c );

  for ( d = 1; d <= p; d++ )
  {
    for ( i = 0; i <= n-d; i++ )
    {
      cp_temp[i] = ( double ) ( i + 1 ) * cp_temp[i+1];
    }
    cp_temp[n-d+1] = 0.0;
  }

  cp = r8vec_copy_new ( n - p + 1, cp_temp );

  delete [] cp_temp;

  return cp;
}
//****************************************************************************80

double r8poly_lagrange_0 ( int npol, double xpol[], double xval )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_0 evaluates the Lagrange factor at a point.
//
//  Formula:
//
//    W(X) = Product ( 1 <= I <= NPOL ) ( X - XPOL(I) )
//
//  Discussion:
//
//    For a set of points XPOL(I), 1 <= I <= NPOL, the IPOL-th Lagrange basis
//    polynomial L(IPOL)(X), has the property:
//
//      L(IPOL)( XPOL(J) ) = delta ( IPOL, J )
//
//    and may be expressed as:
//
//      L(IPOL)(X) = W(X) / ( ( X - XPOL(IPOL) ) * W'(XPOL(IPOL)) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//    NPOL must be at least 1.
//
//    Input, double XPOL[NPOL], the abscissas, which should be distinct.
//
//    Input, double XVAL, the point at which the Lagrange factor is to be
//    evaluated.
//
//    Output, double R8POLY_LAGRANGE_0, the value of the Lagrange factor at XVAL.
//
{
  int i;
  double wval;

  wval = 1.0;
  for ( i = 0; i < npol; i++ )
  {
    wval = wval * ( xval - xpol[i] );
  }

  return wval;
}
//****************************************************************************80

double r8poly_lagrange_1 ( int npol, double xpol[], double xval )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_1 evaluates the first derivative of the Lagrange factor.
//
//  Formula:
//
//    W(XPOL(1:NPOL))(X) = Product ( 1 <= I <= NPOL ) ( X - XPOL(I) )
//
//    W'(XPOL(1:NPOL))(X)
//      = Sum ( 1 <= J <= NPOL ) Product ( I /= J ) ( X - XPOL(I) )
//
//    We also have the recursion:
//
//      W'(XPOL(1:NPOL))(X) = d/dX ( ( X - XPOL(NPOL) ) * W(XPOL(1:NPOL-1))(X) )
//                    = W(XPOL(1:NPOL-1))(X)
//                    + ( X - XPOL(NPOL) ) * W'(XPOL(1:NPOL-1))(X)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//
//    Input, double XPOL[NPOL], the abscissas, which should be distinct.
//
//    Input, double XVAL, the point at which the Lagrange factor is to be
//    evaluated.
//
//    Output, double R8POLY_LAGRANGE_1, the derivative of W with respect to XVAL.
//
{
  double dwdx;
  int i;
  double w;

  dwdx = 0.0;
  w = 1.0;

  for ( i = 0; i < npol; i++ )
  {
    dwdx = w + ( xval - xpol[i] ) * dwdx;
    w = w * ( xval - xpol[i] );
  }

  return dwdx;
}
//****************************************************************************80

double r8poly_lagrange_2 ( int npol, double xpol[], double xval )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_2 evaluates the second derivative of the Lagrange factor.
//
//  Formula:
//
//    W(X)  = Product ( 1 <= I <= NPOL ) ( X - XPOL(I) )
//
//    W'(X) = Sum ( 1 <= J <= NPOL )
//            Product ( I /= J ) ( X - XPOL(I) )
//
//    W"(X) = Sum ( 1 <= K <= NPOL )
//            Sum ( J =/ K )
//            Product ( I /= K, J ) ( X - XPOL(I) )
//
//    For a set of points XPOL(I), 1 <= I <= NPOL, the IPOL-th Lagrange basis
//    polynomial L(IPOL)(X), has the property:
//
//      L(IPOL)( XPOL(J) ) = delta ( IPOL, J )
//
//    and may be expressed as:
//
//      L(IPOL)(X) = W(X) / ( ( X - XPOL(IPOL) ) * W'(XPOL(IPOL)) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//    NPOL must be at least 1.
//
//    Input, double XPOL[NPOL], the abscissas, which should be distinct.
//
//    Input, double XVAL, the point at which the Lagrange factor is to be
//    evaluated.
//
//    Output, double R8POLY_LAGRANGE_2, the second derivative of W with respect to XVAL.
//
{
  double dw2dx2;
  int i;
  int j;
  int k;
  double term;

  dw2dx2 = 0.0;

  for ( k = 0; k < npol; k++ )
  {
    for ( j = 0; j < npol; j++ )
    {
      if ( j != k )
      {
        term = 1.0;
        for ( i = 0; i < npol; i++ )
        {
          if ( i != j && i != k )
          {
            term = term * ( xval - xpol[i] );
          }
        }
        dw2dx2 = dw2dx2 + term;
      }
    }
  }

  return dw2dx2;
}
//****************************************************************************80

double *r8poly_lagrange_coef ( int npol, int ipol, double xpol[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_COEF returns the coefficients of a Lagrange polynomial.
//
//  Discussion:
//
//    Given NPOL distinct abscissas, XPOL(*), the IPOL-th Lagrange
//    polynomial P(IPOL)(X) is defined as the polynomial of degree
//    NPOL - 1 which is 1 at XPOL(IPOL) and 0 at the NPOL - 1 other
//    abscissas.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//    NPOL must be at least 1.
//
//    Input, int IPOL, the index of the polynomial to evaluate.
//    IPOL must be between 1 and NPOL.
//
//    Input, double XPOL[NPOL], the abscissas of the Lagrange polynomials.  
//    The entries in XPOL must be distinct.
//
//    Output, double R8POLY_LAGRANGE_COEF[NPOL], the polynomial coefficients 
//    of the IPOL-th Lagrange polynomial.  
//
{
  int i;
  int index;
  int j;
  double *pcof;
//
//  Make sure IPOL is legal.
//
  if ( ipol < 1 || npol < ipol )
  {
    cout << "\n";
    cout << "R8POLY_LAGRANGE_COEF - Fatal error!\n";
    cout << "  1 <= IPOL <= NPOL is required.\n";
    cout << "  but IPOL = " << ipol << "\n";
    cout << "  and NPOL = " << npol << "\n";
    exit ( 1 );
  }
//
//  Check that the abscissas are distinct.
//
  if ( !r8vec_distinct ( npol, xpol ) )
  {
    cout << "\n";
    cout << "R8POLY_LAGRANGE_COEF - Fatal error!\n";
    cout << "  Two entries of XPOL are equal:\n";
    exit ( 1 );
  }

  pcof = new double[npol];

  pcof[0] = 1.0;
  for ( i = 1; i < npol; i++ )
  {
    pcof[i] = 0.0;
  }

  index = 0;

  for ( i = 1; i <= npol; i++ )
  {
    if ( i != ipol )
    {
      index = index + 1;

      for ( j = index; 0 <= j; j-- )
      {
        pcof[j] = - xpol[i-1] * pcof[j] / ( xpol[ipol-1] - xpol[i-1] );

        if ( 0 < j )
        {
          pcof[j] = pcof[j] + pcof[j-1] / ( xpol[ipol-1] - xpol[i-1] );
        }
      }
    }
  }
 
  return pcof;
}
//****************************************************************************80

void r8poly_lagrange_factor ( int npol, double xpol[], double xval, 
  double *wval, double *dwdx )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_FACTOR evaluates the polynomial Lagrange factor at a point.
//
//  Formula:
//
//    W(X) = Product ( 1 <= I <= NPOL ) ( X - XPOL(I) )
//
//  Discussion:
//
//    Suppose F(X) is at least N times continuously differentiable in the
//    interval [A,B].  Pick NPOL distinct points XPOL(I) in [A,B] and compute
//    the interpolating polynomial P(X) of order NPOL ( and degree NPOL-1) 
//    which passes through all the points ( XPOL(I), F(XPOL(I)) ).
//    Then in the interval [A,B], the maximum error 
//
//      abs ( F(X) - P(X) )
//
//    is bounded by:
//
//      C * FNMAX * W(X)
//
//    where 
//
//      C is a constant, 
//      FNMAX is the maximum value of the NPOL-th derivative of F in [A,B], 
//      W(X) is the Lagrange factor.
//
//    Thus, the value of W(X) is useful as part of an estimated bound
//    for the interpolation error.
//
//    Note that the Chebyshev abscissas have the property that they minimize 
//    the value of W(X) over the interval [A,B].  Hence, if the abscissas may 
//    be chosen arbitrarily, the Chebyshev abscissas have this advantage over 
//    other choices.
//
//    For a set of points XPOL[I], 0 <= I <= NPOL-1, the IPOL-th Lagrange basis 
//    polynomial L(IPOL)(X), has the property:
//
//      L(IPOL)( XPOL(J) ) = delta ( IPOL, J )
//
//    and may be expressed as:
//
//      L(IPOL)(X) = W(X) / ( ( X - XPOL[IPOL] ) * W'(XPOL[IPOL]) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//    NPOL must be at least 1.
//
//    Input, double XPOL[NPOL], the abscissas, which should be distinct.
//
//    Input, double XVAL, the point at which the Lagrange factor is to be evaluated.
//
//    Output, double *WVAL, the value of the Lagrange factor at XVAL.
//
//    Output, double *DWDX, the derivative of W with respect to XVAL.
//
{
  int i;
  int j;
  double term;

  *wval = 1.0;
  for ( i = 0; i < npol; i++ )
  {
    *wval = *wval * ( xval - xpol[i] );
  }

  *dwdx = 0.0;

  for ( i = 0; i < npol; i++ )
  {
    term = 1.0;

    for ( j = 0; j < npol; j++ )
    {
      if ( i != j )
      {
        term = term * ( xval - xpol[j] );
      }
    }
    *dwdx = *dwdx + term;
  }

  return;
}
//****************************************************************************80

int r8poly_lagrange_val ( int npol, int ipol, double xpol[], double xval, 
  double *pval, double *dpdx )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_LAGRANGE_VAL evaluates the IPOL-th Lagrange polynomial.
//
//  Discussion:
//
//    Given NPOL distinct abscissas, XPOL[*], the IPOL-th Lagrange
//    polynomial P(IPOL)(X) is defined as the polynomial of degree
//    NPOL - 1 which is 1 at XPOL[IPOL] and 0 at the NPOL - 1 other
//    abscissas.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NPOL, the number of abscissas.
//    NPOL must be at least 1.
//
//    Input, int IPOL, the index of the polynomial to evaluate.
//    IPOL must be between 0 and NPOL-1.
//
//    Input, double XPOL[NPOL], the abscissas of the Lagrange polynomials.  
//    The entries in XPOL must be distinct.
//
//    Input, double XVAL, the point at which the IPOL-th Lagrange polynomial 
//    is to be evaluated.
//
//    Output, double *PVAL, the value of the IPOL-th Lagrange polynomial at XVAL.
//
//    Output, double *DPDX, the derivative of the IPOL-th Lagrange polynomial at XVAL.
//
//    Output, int R8POLY_LAGRANGE_VAL, 0 if no error.
{
  int i;
  int j;
  double p2;
//
//  Make sure IPOL is legal.
//
  if ( ipol < 0 || npol-1 < ipol )
  {
    cout << "\n";
    cout << "R8POLY_LAGRANGE_VAL - Fatal error!\n";
    cout << "  0 <= IPOL <= NPOL-1 is required.\n";
    return 1;
  }
//
//  Check that the abscissas are distinct.
//
  for ( i = 1; i < npol; i++ )
  {
    for ( j = 0; j < i; j++ )
    {
      if ( xpol[i] == xpol[j] )
      {
        cout << "\n";
        cout << "R8POLY_LAGRANGE_VAL - Fatal error!\n";
        cout << "  Two entries of XPOL are equal:\n";
        cout << "  XPOL(" << i << ") = " << xpol[i] << ".\n";
        cout << "  XPOL(" << j << ") = " << xpol[j] << ".\n";
        return 1;
      }
    }
  }
//
//  Evaluate the polynomial.
//
  *pval = 1.0;

  for ( i = 0; i < npol; i++ )
  {
    if ( i != ipol )
    {
      *pval = *pval * ( xval - xpol[i] ) / ( xpol[ipol] - xpol[i] );
    }
  }
//
//  Evaluate the derivative, which can be found by summing up the result
//  of differentiating one factor at a time, successively.
//
  *dpdx = 0.0;

  for ( i = 0; i < npol; i++ )
  {
    if ( i != ipol )
    {
      p2 = 1.0;

      for ( j = 0; j < npol; j++ )
      {
        if ( j == i )
        {
          p2 = p2                      / ( xpol[ipol] - xpol[j] );
        }
        else if ( j != ipol )
        {
          p2 = p2 * ( xval - xpol[j] ) / ( xpol[ipol] - xpol[j] );
        }
      }
      *dpdx = *dpdx + p2;
    }
  }

  return 0;
}
//****************************************************************************80

int r8poly_order ( int na, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_ORDER returns the order of a polynomial.
//
//  Discussion:
//
//    The order of a polynomial is one more than the degree.
//
//    The order of a constant polynomial is 1.  The order of the
//    zero polynomial is debatable, but this routine returns the
//    order as 1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NA, the dimension of A.
//
//    Input, double A[NA+1], the coefficients of the polynomials.
//
//    Output, int R8POLY_ORDER, the order of A.
//
{
  int order;

  order = na + 1;

  while ( 1 < order )
  {
    if ( a[order-1] != 0.0 )
    {
      return order;
    }
    order = order - 1;
  }

  return order;
}
//****************************************************************************80

void r8poly_print ( int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_PRINT prints out a polynomial.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of A.
//
//    Input, double A[N+1], the polynomial coefficients.
//    A(0) is the constant term and
//    A(N) is the coefficient of X**N.
//
//    Input, char *TITLE, an optional title.
//
{
  int i;
  double mag;
  int n2;
  char plus_minus;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";

  n2 = r8poly_degree ( n, a );

  if ( n2 <= 0 ) 
  {
    cout << "  p(x) = 0\n";
    return;
  }

  if ( a[n2] < 0.0 )
  {
    plus_minus = '-';
  }
  else
  {
    plus_minus = ' ';
  }

  mag = r8_abs ( a[n2] );

  if ( 2 <= n2 )
  {
    cout << "  p(x) = " << plus_minus 
         << setw(14) << mag << " * x ^ " << n2 << "\n";
  }
  else if ( n2 == 1 )
  {
    cout << "  p(x) = " << plus_minus 
         << setw(14) << mag << " * x\n";
  }
  else if ( n2 == 0 )
  {
    cout << "  p(x) = " << plus_minus 
         << setw(14) << mag << "\n";
  }

  for ( i = n2-1; 0 <= i; i-- )
  {
    if ( a[i] < 0.0 )
    {
      plus_minus = '-';
    }
    else
    {
      plus_minus = '+';
    }

    mag = r8_abs ( a[i] );

    if ( mag != 0.0 )
    {
      if ( 2 <= i )
      {
        cout << "         " << plus_minus 
             << setw(14) << mag << " * x ^ " << i << "\n";
      }
      else if ( i == 1 )
      {
        cout << "         " << plus_minus 
             << setw(14) << mag << " * x\n";
      }
      else if ( i == 0 )
      {
        cout << "         " << plus_minus 
             << setw(14) << mag << "\n";
      }
    }
  }

  return;
}
//****************************************************************************80

void r8poly_shift ( double scale, double shift, int n, double poly_cof[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_SHIFT adjusts the coefficients of a polynomial for a new argument.
//
//  Discussion:
//
//    Assuming P(X) is a polynomial in the argument X, of the form:
//
//      P(X) =
//          C(N) * X**N
//        + ...
//        + C(1) * X
//        + C(0),
//
//    and that Z is related to X by the formula:
//
//      Z = SCALE * X + SHIFT
//
//    then this routine computes coefficients C for the polynomial Q(Z):
//
//      Q(Z) =
//          C(N) * Z**N
//        + ...
//        + C(1) * Z
//        + C(0)
//
//    so that:
//
//      Q(Z(X)) = P(X)
//
//  Example:
//
//    P(X) = 2 * X**2 - X + 6
//
//    Z = 2.0D+00 * X + 3.0D+00
//
//    Q(Z) = 0.5 *         Z**2 -  3.5 * Z + 12
//
//    Q(Z(X)) = 0.5 * ( 4.0D+00 * X**2 + 12.0D+00 * X +  9 )
//            - 3.5 * (               2.0D+00 * X +  3 )
//                                            + 12
//
//            = 2.0D+00         * X**2 -  1.0D+00 * X +  6
//
//            = P(X)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 September 2005
//
//  Reference:
//
//    Press, Flannery, Teukolsky, Vetterling,
//    Numerical Recipes: The Art of Scientific Computing,
//    Cambridge University Press.
//
//  Parameters:
//
//    Input, double SHIFT, SCALE, the shift and scale applied to X,
//    so that Z = SCALE * X + SHIFT.
//
//    Input, int N, the number of coefficients.
//
//    Input/output, double POLY_COF[N+1].
//    On input, the coefficient array in terms of the X variable.
//    On output, the coefficient array in terms of the Z variable.
//
{
  int i;
  int j;

  for ( i = 1; i <= n; i++ )
  {
    for ( j = i; j <= n; j++ )
    {
      poly_cof[j] = poly_cof[j] / scale;
    }
  }

  for ( i = 0; i <= n-1; i++ )
  {
    for ( j = n-1; i <= j; j-- )
    {
      poly_cof[j] = poly_cof[j] - shift * poly_cof[j+1];
    }
  }

  return;
}
//****************************************************************************80

double r8poly_val_horner ( int n, double c[], double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY_VAL_HORNER evaluates a polynomial using Horner's method.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of C.
//
//    Input, double C[N+1], the polynomial coefficients.
//    C(I) is the coefficient of X**I.
//
//    Input, double X, the point at which the polynomial is 
//    to be evaluated.
//
//    Output, double R8POLY_VAL_HORNER, the value of the polynomial at X.
//
{
  int i;
  double value;

  value = c[n];
  for ( i = n-1; 0 <= i; i-- )
  {
    value = value * x + c[i];
  }

  return value;
}
//****************************************************************************80

int r8poly2_ex ( double x1, double y1, double x2, double y2, double x3, 
  double y3, double *x, double *y )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY2_EX finds the extremal point of a parabola determined by three points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 1998
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, X2, Y2, X3, Y3, the coordinates of three points
//    on the parabola.  X1, X2 and X3 must be distinct.
//
//    Output, double *X, *Y, the X coordinate of the extremal point of the
//    parabola, and the value of the parabola at that point.
//
//    Output, int R8POLY2_EX, error flag.
//    0, no error.
//    1, two of the X values are equal.
//    2, the data lies on a straight line; there is no finite extremal
//    point.
//    3, the data lies on a horizontal line; every point is "extremal".
//
{
  double bot;

  *x = 0.0;
  *y = 0.0;

  if ( x1 == x2 || x2 == x3 || x3 == x1 )
  {
    return 1;
  }

  if ( y1 == y2 && y2 == y3 && y3 == y1 )
  {
    *x = x1;
    *y = y1;
    return 3;
  }

  bot = ( x2 - x3 ) * y1 + ( x3 - x1 ) * y2 + ( x1 - x2 ) * y3;

  if ( bot == 0.0 )
  {
    return 2;
  }

  *x = 0.5 * ( 
      x1 * x1 * ( y3 - y2 )
    + x2 * x2 * ( y1 - y3 )
    + x3 * x3 * ( y2 - y1 ) ) /
    ( ( x2 - x3 ) * y1 + ( x3 - x1 ) * y2 + ( x1 - x2 ) * y3 );

  *y = - (
      ( *x - x2 ) * ( *x - x3 ) * ( x2 - x3 ) * y1
    + ( *x - x1 ) * ( *x - x3 ) * ( x3 - x1 ) * y2
    + ( *x - x1 ) * ( *x - x2 ) * ( x1 - x2 ) * y3 ) /
    ( ( x1 - x2 ) * ( x2 - x3 ) * ( x3 - x1 ) );

  return 0;
}
//****************************************************************************80

int r8poly2_ex2 ( double x1, double y1, double x2, double y2, double x3,
  double y3, double *x, double *y, double *a, double *b, double *c )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY2_EX2 finds the extremal point of a parabola determined by three points.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, X2, Y2, X3, Y3, the coordinates of three points
//    on the parabola.  X1, X2 and X3 must be distinct.
//
//    Output, double *X, *Y, the X coordinate of the extremal point of the
//    parabola, and the value of the parabola at that point.
//
//    Output, double *A, *B, *C, the coefficients that define the parabola:
//    P(X) = A * X**2 + B * X + C.
//
//    Output, int R8POLY2_EX2, error flag.
//    0, no error.
//    1, two of the X values are equal.
//    2, the data lies on a straight line; there is no finite extremal
//    point.
//    3, the data lies on a horizontal line; any point is an "extremal point".
//
{
  double v[3*3];
  double *w;

  *a = 0.0;
  *b = 0.0;
  *c = 0.0;
  *x = 0.0;
  *y = 0.0;

  if ( x1 == x2 || x2 == x3 || x3 == x1 )
  {
    return 1;
  }

  if ( y1 == y2 && y2 == y3 && y3 == y1 )
  {
    *x = x1;
    *y = y1;
    return 3;
  }
//
//  Set up the Vandermonde matrix.
//
  v[0+0*3] = 1.0;
  v[0+1*3] = x1;
  v[0+2*3] = x1 * x1;

  v[1+0*3] = 1.0;
  v[1+1*3] = x2;
  v[1+2*3] = x2 * x2;

  v[2+0*3] = 1.0;
  v[2+1*3] = x3;
  v[2+2*3] = x3 * x3;
//
//  Get the inverse.
//
  w = r8mat_inverse_3d ( v );
//
//  Compute the parabolic coefficients.
//
  *c = w[0+0*3] * y1 + w[0+1*3] * y2 + w[0+2*3] * y3;
  *b = w[1+0*3] * y1 + w[1+1*3] * y2 + w[1+2*3] * y3;
  *a = w[2+0*3] * y1 + w[2+1*3] * y2 + w[2+2*3] * y3;
//
//  Determine the extremal point.
//
  if ( *a == 0.0 )
  {
    return 2;
  }

  *x = - *b / ( 2.0 * *a );
  *y = *a * *x * *x + *b * *x + *c;

  return 0;
}
//****************************************************************************80

void r8poly2_root ( double a, double b, double c, complex <double> *r1, 
  complex <double> *r2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY2_ROOT returns the two roots of a quadratic polynomial.
//
//  Discussion:
//
//    The polynomial has the form:
//
//      A * X**2 + B * X + C = 0 
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2005
//
//  Parameters:
//
//    Input, double A, B, C, the coefficients of the polynomial.
//    A must not be zero.
//
//    Output, complex <double> *R1, *R2, the roots of the polynomial, which
//    might be real and distinct, real and equal, or complex conjugates.
//
{
  double disc;
  complex <double> q;

  if ( a == 0.0 )
  {
    cout << "\n";
    cout << "R8POLY2_ROOT - Fatal error!\n";
    cout << "  The coefficient A is zero.\n";
    exit ( 1 );
  }

  disc = b * b - 4.0 * a * c;
  q = -0.5 * ( b + r8_sign ( b ) * r8_csqrt ( disc ) );
  *r1 = q / a;
  *r2 = c / q;

  return;
}
//****************************************************************************80

void r8poly2_rroot ( double a, double b, double c, double *r1, double *r2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY2_RROOT returns the real parts of the roots of a quadratic polynomial.
//
//  Example:
//
//    A    B    C       roots              R1   R2
//   --   --   --     ------------------   --   --
//    1   -4    3     1          3          1    3
//    1    0    4     2*i      - 2*i        0    0
//    2   -6    5     3 +   i    3 -   i    3    3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double A, B, C, the coefficients of the quadratic
//    polynomial A * X**2 + B * X + C = 0 whose roots are desired.
//    A must not be zero.
//
//    Output, double *R1, *R2, the real parts of the roots
//    of the polynomial.
//
{
  double disc;
  double q;

  if ( a == 0.0 )
  {
    cout << "\n";
    cout << "R8POLY2_RROOT - Fatal error!\n";
    cout << "  The coefficient A is zero.\n";
    exit ( 1 );
  }

  disc = b * b - 4.0 * a * c;
  disc = r8_max ( disc, 0.0 );

  q = ( b + r8_sign ( b ) * sqrt ( disc ) );
  *r1 = -0.5 * q / a;
  *r2 = -2.0 * c / q;

  return;
}
//****************************************************************************80

void r8poly2_val ( double x1, double y1, double x2, double y2, 
  double x3, double y3, double x, double *y, double *yp, double *ypp )

//****************************************************************************80
//
//  Purpose:
// 
//    R8POLY2_VAL evaluates a parabola defined by three data values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 March 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, X2, Y2, X3, Y3, three pairs of data values.
//    If the X values are distinct, then all the Y values represent
//    actual values of the parabola.
//
//    Three special cases are allowed:
//
//      X1 = X2 =/= X3: Y2 is the derivative at X1;
//      X1 =/= X2 = X3: Y3 is the derivative at X3;
//      X1 = X2 = X3:   Y2 is the derivative at X1, and
//                      Y3 is the second derivative at X1.
//
//    Input, double X, an abscissa at which the parabola is to be
//    evaluated.
//
//    Output, double *Y, *YP, *YPP, the values of the parabola and
//    its first and second derivatives at X.
//
{
  int distinct;
  double dif1;
  double dif2;
  double temp;
// 
//  If any X's are equal, put them and the Y data first. 
//
  if ( x1 == x2 && x2 == x3 )
  {
    distinct = 1;
  }
  else if ( x1 == x2 )
  {
    distinct = 2;
  }
  else if ( x1 == x3 )
  {
    cout << "\n";
    cout << "R8POLY2_VAL - Fatal error!\n";
    cout << "  X1 = X3 =/= X2.\n";
    return;
  }
  else if ( x2 == x3 )
  {
    distinct = 2;
    temp = x1;
    x1 = x3;
    x3 = temp;
    temp = y1;
    y1 = y2;
    y2 = y3;
    y3 = y1;
  }
  else
  {
    distinct = 3;
  }
//  
//  Set up the coefficients. 
//
  if ( distinct == 1 )
  {
    dif1 = y2;
    dif2 = 0.5 * y3;
  }
  else if ( distinct == 2 )
  {
    dif1 = y2;
    dif2 = ( ( y3 - y1 ) / ( x3 - x1 )
             - y2 ) / ( x3 - x2 );
  }
  else if ( distinct == 3 )
  {
    dif1 = ( y2 - y1 ) / ( x2 - x1 );
    dif2 = ( ( y3 - y1 ) / ( x3 - x1 )
           - ( y2 - y1 ) / ( x2 - x1 ) ) / ( x3 - x2 );
  }
// 
//  Evaluate. 
//
  *y = y1 + ( x - x1 ) * dif1 + ( x - x1 ) * ( x - x2 ) * dif2;
  *yp = dif1 + ( 2.0 * x - x1 - x2 ) * dif2;
  *ypp = 2.0 * dif2;

  return;
}
//****************************************************************************80

void r8poly2_val2 ( int ndata, double tdata[], 
  double ydata[], int left, double tval, double *yval )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY2_VAL2 evaluates a parabolic function through 3 points in a table.
//
//  Discussion:
//
//    This routine is a utility routine used by OVERHAUSER_SPLINE_VAL.
//    It constructs the parabolic interpolant through the data in
//    3 consecutive entries of a table and evaluates this interpolant
//    at a given abscissa value.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    04 March 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NDATA, the number of data points.
//    NDATA must be at least 3.
//
//    Input, double TDATA[NDATA], the abscissas of the data points.  The
//    values in TDATA must be in strictly ascending order.
//
//    Input, double YDATA[NDATA], the data points corresponding to
//    the abscissas.
//
//    Input, int LEFT, the location of the first of the three
//    consecutive data points through which the parabolic interpolant
//    must pass.  0 <= LEFT <= NDATA - 3.
//
//    Input, double TVAL, the value of T at which the parabolic interpolant
//    is to be evaluated.  Normally, TDATA[0] <= TVAL <= T[NDATA-1], and 
//    the data will be interpolated.  For TVAL outside this range, 
//    extrapolation will be used.
//
//    Output, double *YVAL, the value of the parabolic interpolant 
//    at TVAL.
//
{
  double dif1;
  double dif2;
  double t1;
  double t2;
  double t3;
  double y1;
  double y2;
  double y3;
// 
//  Check. 
//
  if ( left < 0 || ndata-3 < left )
  {
    cout << "\n";
    cout << "RPOLY2_VAL2 - Fatal error!\n";
    cout << "  LEFT < 0 or NDATA-3 < LEFT.\n";
    exit ( 1 );
  }
// 
//  Copy out the three abscissas. 
//
  t1 = tdata[left];
  t2 = tdata[left+1];
  t3 = tdata[left+2];

  if ( t2 <= t1 || t3 <= t2 )
  {
    cout << "\n";
    cout << "RPOLY2_VAL2 - Fatal error!\n";
    cout << "  T2 <= T1 or T3 <= T2.\n";
    cout << "  T1 = " << t1 << "\n";
    cout << "  T2 = " << t2 << "\n";
    cout << "  T3 = " << t3 << "\n";
    exit ( 1 );
  }
// 
//  Construct and evaluate a parabolic interpolant for the data. 
//
  y1 = ydata[left];
  y2 = ydata[left+1];
  y3 = ydata[left+2];

  dif1 = ( y2 - y1 ) / ( t2 - t1 );
  dif2 =
    ( ( y3 - y1 ) / ( t3 - t1 )
    - ( y2 - y1 ) / ( t2 - t1 ) ) / ( t3 - t2 );

  *yval = y1 + ( tval - t1 ) * ( dif1 + ( tval - t2 ) * dif2 );

  return;
}
//****************************************************************************80

void r8poly3_root ( double a, double b, double c, double d, 
  complex <double> *r1, complex <double> *r2, complex <double> *r3 )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY3_ROOT returns the three roots of a cubic polynomial.
//
//  Discussion:
//
//    The polynomial has the form
//
//      A * X**3 + B * X**2 + C * X + D = 0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    25 October 2005
//
//  Parameters:
//
//    Input, double A, B, C, D, the coefficients of the polynomial.
//    A must not be zero.
//
//    Output, complex <double> *R1, *R2, *R3, the roots of the polynomial, which
//    will include at least one real root.
//
{
  complex <double> i;
  double pi = 3.141592653589793;
  double q;
  double r;
  double s1;
  double s2;
  double temp;
  double theta;

  if ( a == 0.0 )
  {
    cout << "\n";
    cout << "R8POLY3_ROOT - Fatal error!\n";
    cout << "  A must not be zero.\n";
    exit ( 1 );
  }

  i = complex <double> ( 0.0, 1.0 );

  q = ( pow ( b / a, 2 ) - 3.0 * ( c / a ) ) / 9.0;

  r = ( 2.0 * pow ( b / a, 3 ) - 9.0 * ( b / a ) * ( c / a ) 
      + 27.0 * ( d / a ) ) / 54.0;

  if ( r * r < q * q * q )
  {
    theta = acos ( r / sqrt ( pow ( q, 3 ) ) );
    *r1 = -2.0 * sqrt ( q ) * cos (   theta              / 3.0 );
    *r2 = -2.0 * sqrt ( q ) * cos ( ( theta + 2.0 * pi ) / 3.0 );
    *r3 = -2.0 * sqrt ( q ) * cos ( ( theta + 4.0 * pi ) / 3.0 );
  }
  else if ( q * q * q <= r * r )
  {
    temp = -r + sqrt ( r * r - q * q * q );
    s1 = r8_sign ( temp ) * pow ( r8_abs ( temp ), 1.0 / 3.0 );

    temp = -r - sqrt ( r * r - q * q * q );
    s2 = r8_sign ( temp ) * pow ( r8_abs ( temp ), 1.0 / 3.0 );

    *r1 = s1 + s2;
    *r2 = -0.5 * ( s1 + s2 ) + i * 0.5 * sqrt ( 3.0 ) * ( s1 - s2 );
    *r3 = -0.5 * ( s1 + s2 ) - i * 0.5 * sqrt ( 3.0 ) * ( s1 - s2 );
  }

  *r1 = *r1 - b / ( 3.0 * a );
  *r2 = *r2 - b / ( 3.0 * a );
  *r3 = *r3 - b / ( 3.0 * a );

  return;
}
//****************************************************************************80

void r8poly4_root ( double a, double b, double c, double d, double e, 
  complex <double> *r1, complex <double> *r2, complex <double> *r3, 
  complex <double> *r4 )

//****************************************************************************80
//
//  Purpose:
//
//    R8POLY4_ROOT returns the four roots of a quartic polynomial.
//
//  Discussion:
//
//    The polynomial has the form:
//
//      A * X**4 + B * X**3 + C * X**2 + D * X + E = 0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 October 2005
//
//  Parameters:
//
//    Input, double A, B, C, D, the coefficients of the polynomial.
//    A must not be zero.
//
//    Output, complex <double> *R1, *R2, *R3, *R4, the roots of the polynomial.
//
{
  double a3;
  double a4;
  double b3;
  double b4;
  double c3;
  double c4;
  double d3;
  double d4;
  complex <double> p;
  complex <double> q;
  complex <double> r;
  complex <double> zero;

  zero = 0.0;

  if ( a == 0.0 )
  {
    cout << "\n";
    cout << "R8POLY4_ROOT - Fatal error!\n";
    cout << "  A must not be zero.\n";
    exit ( 1 );
  }

  a4 = b / a;
  b4 = c / a;
  c4 = d / a;
  d4 = e / a;
//
//  Set the coefficients of the resolvent cubic equation.
//
  a3 = 1.0;
  b3 = -b4;
  c3 = a4 * c4 - 4.0 * d4;
  d3 = -a4 * a4 * d4 + 4.0 * b4 * d4 - c4 * c4;
//
//  Find the roots of the resolvent cubic.
//
  r8poly3_root ( a3, b3, c3, d3, r1, r2, r3 );
//
//  Choose one root of the cubic, here R1.
//
//  Set R = sqrt ( 0.25 * A4**2 - B4 + R1 )
//
  r = c8_sqrt ( 0.25 * a4 * a4 - b4  + (*r1) );

  if ( real ( r ) != 0.0 || imag ( r ) != 0.0 )
  {
    p = c8_sqrt ( 0.75 * a4 * a4 - r * r - 2.0 * b4 
        + 0.25 * ( 4.0 * a4 * b4 - 8.0 * c4 - a4 * a4 * a4 ) / r );

    q = c8_sqrt ( 0.75 * a4 * a4 - r * r - 2.0 * b4 
        - 0.25 * ( 4.0 * a4 * b4 - 8.0 * c4 - a4 * a4 * a4 ) / r );
  }
  else
  {
    p = c8_sqrt ( 0.75 * a4 * a4 - 2.0 * b4 
      + 2.0 * c8_sqrt ( (*r1) * (*r1) - 4.0 * d4 ) );

    q = c8_sqrt ( 0.75 * a4 * a4 - 2.0 * b4 
      - 2.0 * c8_sqrt ( (*r1) * (*r1) - 4.0 * d4 ) );
  }
//
//  Set the roots.
//
  *r1 = -0.25 * a4 + 0.5 * r + 0.5 * p;
  *r2 = -0.25 * a4 + 0.5 * r - 0.5 * p;
  *r3 = -0.25 * a4 - 0.5 * r + 0.5 * q;
  *r4 = -0.25 * a4 - 0.5 * r - 0.5 * q;

  return;
}
//****************************************************************************80

double *r8row_max ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_MAX returns the row maximums of an R8ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MAX =
//      3
//      7
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8ROW_MAX[M], the maximums of the rows.
//
{
  int i;
  int j;
  double *amax;

  amax = new double[m];

  for ( i = 0; i < m; i++ )
  {
    amax[i] = a[i+0*m];

    for ( j = 1; j < n; j++ )
    {
      if ( amax[i] < a[i+j*m] )
      {
        amax[i] = a[i+j*m];
      }
    }
  }

  return amax;
}
//****************************************************************************80

double *r8row_mean ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_MEAN returns the row means of an R8ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MEAN =
//      2
//      5
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8ROW_MEAN[M], the means, or averages, of the rows.
//
{
  int i;
  int j;
  double *mean;

  mean = new double[m];

  for ( i = 0; i < m; i++ )
  {
    mean[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      mean[i] = mean[i] + a[i+j*m];
    }
    mean[i] = mean[i] / ( double ) ( n );
  }

  return mean;
}
//****************************************************************************80

double *r8row_min ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_MIN returns the row minimums of an R8ROW.
//
//  Example:
//
//    A =
//      1  2  3
//      2  6  7
//
//    MIN =
//      1
//      2
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, double A[M*N], the array to be examined.
//
//    Output, double R8ROW_MIN[M], the minimums of the rows.
//
{
  int i;
  int j;
  double *amin;

  amin = new double[m];

  for ( i = 0; i < m; i++ )
  {
    amin[i] = a[i+0*m];
    for ( j = 1; j < n; j++ )
    {
      if ( a[i+j*m] < amin[i] )
      {
        amin[i] = a[i+j*m];
      }
    }
  }

  return amin;
}
//****************************************************************************80

double *r8row_sum ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_SUM returns the sums of the rows of an R8ROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the M by N array.
//
//    Output, double ROWSUM[M], the sum of the entries of 
//    each row.
//
{
  int i;
  int j;
  double *rowsum;

  rowsum = new double[m];

  for ( i = 0; i < m; i++ )
  {
    rowsum[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      rowsum[i] = rowsum[i] + a[i+j*m];
    }
  }

  return rowsum;
}
//****************************************************************************80

void r8row_swap ( int m, int n, double a[], int irow1, int irow2 )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_SWAP swaps two rows of an R8ROW.
//
//  Discussion:
//
//    The two dimensional information is stored as a one dimensional
//    array, by columns.
//
//    The row indices are 1 based, NOT 0 based//  However, a preprocessor
//    variable, called OFFSET, can be reset from 1 to 0 if you wish to
//    use 0-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input/output, double A[M*N], an array of data.
//
//    Input, int IROW1, IROW2, the two rows to swap.
//    These indices should be between 1 and M.
//
{
  int j;
  double t;
//
//  Check.
//
  if ( irow1 < 1 || m < irow1 )
  {
    cout << "\n";
    cout << "R8ROW_SWAP - Fatal error!\n";
    cout << "  IROW1 is out of range.\n";
    exit ( 1 );
  }

  if ( irow2 < 1 || m < irow2 )
  {
    cout << "\n";
    cout << "R8ROW_SWAP - Fatal error!\n";
    cout << "  IROW2 is out of range.\n";
    exit ( 1 );
  }

  if ( irow1 == irow2 )
  {
    return;
  }

  for ( j = 0; j < n; j++ )
  {
    t              = a[irow1-1+j*m];
    a[irow1-1+j*m] = a[irow2-1+j*m];
    a[irow2-1+j*m] = t;
  }

  return;
# undef OFFSET
}
//****************************************************************************80

double *r8row_to_r8vec ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_TO_R8VEC converts an R8ROW into an R8VEC.
//
//  Example:
//
//    M = 3, N = 4
//
//    A =
//      11 12 13 14
//      21 22 23 24
//      31 32 33 34
//
//    R8ROW_TO_R8VEC = ( 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns.
//
//    Input, double A[M*N], the M by N array.
//
//    Output, double R8ROW_TO_R8VEC[M*N], a vector containing the M rows of A.
//
{
  int i;
  int j;
  int k;
  double *x;

  x = new double[m*n];

  k = 0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      x[k] = a[i+j*m];
      k = k + 1;
    }
  }

  return x;
}
//****************************************************************************80

double *r8row_variance ( int m, int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8ROW_VARIANCE returns the variances of an R8ROW.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the number of rows and columns in the array.
//
//    Input, double A[M*N], the array whose variances are desired.
//
//    Output, double R8ROW_VARIANCE[M], the variances of the rows.
//
{
  int i;
  int j;
  double mean;
  double *variance;

  variance = new double[m];

  for ( i = 0; i < m; i++ )
  {
    mean = 0.0;
    for ( j = 0; j < n; j++ )
    {
      mean = mean + a[i+j*m];
    }
    mean = mean / ( double ) ( n );

    variance[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      variance[i] = variance[i] + pow ( ( a[i+j*m] - mean ), 2 );
    }

    if ( 1 < n )
    {
      variance[i] = variance[i] / ( double ) ( n - 1 );
    }
    else
    {
      variance[i] = 0.0;
    }

  }

  return variance;
}
//****************************************************************************80

void r8slmat_print ( int m, int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8SLMAT_PRINT prints a strict lower triangular R8MAT.
//
//  Example:
//
//    M = 5, N = 5
//    A = (/ 21, 31, 41, 51, 32, 42, 52, 43, 53, 54 /)
//
//    21
//    31 32
//    41 42 43
//    51 52 53 54
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    21 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the number of rows in A.
//
//    Input, int N, the number of columns in A.
//
//    Input, double A[*], the M by N matrix.  Only the strict
//    lower triangular elements are stored, in column major order.
//
//    Input, char *TITLE, a title to be printed.
//
{
  int i;
  int indx;
  int j;
  int jhi;
  int jlo;
  int jmax;
  int nn;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  jmax = i4_min ( n, m - 1 );

  nn = 5;

  for ( jlo = 1; jlo <= jmax; jlo = jlo + nn )
  {
    jhi = i4_min ( jlo + nn - 1, i4_min ( m - 1, jmax ) );
    cout << "\n";
    cout << "  Col   ";
    for ( j = jlo; j <= jhi; j++ )
    {
      cout << setw(7) << j << "       ";
    }
    cout << "\n";
    cout << "  Row\n";
    for ( i = jlo + 1; i <= m; i++ )
    {
      cout << "  " << setw(6) << i;
      jhi = i4_min ( jlo + nn - 1, i4_min ( i - 1, jmax ) );
      for ( j = jlo; j <= jhi; j++ )
      {
        indx = ( j - 1 ) * m + i - ( j * ( j + 1 ) ) / 2;
        cout << " " << setw(12) << a[indx-1];
      }
      cout << "\n";
    }
  }

  return;
}
//****************************************************************************80

void r8vec_01_to_ab ( int n, double a[], double amax, double amin )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_01_TO_AB shifts and rescales data to lie within given bounds.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    On input, A contains the original data, which is presumed to lie
//    between 0 and 1.  However, it is not necessary that this be so.
//
//    On output, A has been shifted and rescaled so that all entries which
//    on input lay in [0,1] now lie between AMIN and AMAX.  Other entries will
//    be mapped in a corresponding way.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of data values.
//
//    Input/output, double A[N], the vector to be rescaled.
//
//    Input, double AMAX, AMIN, the maximum and minimum values 
//    allowed for A.
//
{
  double amax2;
  double amax3;
  double amin2;
  double amin3;
  int i;

  if ( amax == amin )
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = amin;
    }
    return;
  }

  amax2 = r8_max ( amax, amin );
  amin2 = r8_min ( amax, amin );

  amin3 = r8vec_min ( n, a );
  amax3 = r8vec_max ( n, a );

  if ( amax3 != amin3 )
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = ( ( amax3 - a[i]         ) * amin2   
             + (         a[i] - amin3 ) * amax2 ) 
             / ( amax3          - amin3 );
    }
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = 0.5 * ( amax2 + amin2 );
    }
  }

  return;
}
//****************************************************************************80

void r8vec_ab_to_01 ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AB_TO_01 shifts and rescales data to lie within [0,1].
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    On input, A contains the original data.  On output, A has been shifted
//    and scaled so that all entries lie between 0 and 1.
//
//  Formula:
//
//    A(I) := ( A(I) - AMIN ) / ( AMAX - AMIN )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of data values.
//
//    Input/output, double A[N], the data to be rescaled.
//
{
  double amax;
  double amin;
  int i;

  amax = r8vec_max ( n, a );
  amin = r8vec_min ( n, a );

  if ( amin == amax )
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = 0.5;
    }
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = ( a[i] - amin ) / ( amax - amin );
    }
  }

  return;
}
//****************************************************************************80

double *r8vec_ab_to_cd ( int n, double a[], double bmin, double bmax )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AB_TO_CD shifts and rescales data to lie within a given pair of bounds.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The mininum entry of A is mapped to BMIN, the maximum entry
//    to BMAX, and values in between are mapped linearly.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of data values.
//
//    Input, double A[N], the data to be remapped.
//
//    Input, double BMIN, BMAX, the values to which min(A) and max(A)
//    are to be assigned.
//
//    Output, double R8VEC_AB_TO_CD[N], the remapped data.
//
{
  double amax;
  double amin;
  double *b;
  int i;

  b = new double[n];

  if ( bmax == bmin )
  {
    for ( i = 0; i < n; i++ )
    {
      b[i] = bmin;
    }
    return b;
  }

  amax = r8vec_max ( n, a );
  amin = r8vec_min ( n, a );

  if ( amin == amax )
  {
    for ( i = 0; i < n; i++ )
    {
      b[i] = 0.5 * ( bmax + bmin );
    }
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      b[i] = ( ( amax - a[i]        ) * bmin
             + (        a[i] - amin ) * bmax )
             / ( amax        - amin );
    }
  }

  return b;
}
//****************************************************************************80

double r8vec_amax ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AMAX returns the maximum absolute value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, double AMAX, the value of the entry
//    of largest magnitude.
//
{
  double amax;
  int i;

  amax = 0.0;
  for ( i = 0; i < n; i++ )
  {
    if ( amax < r8_abs ( a[i] ) )
    {
      amax = r8_abs ( a[i] );
    }
  }

  return amax;
}
//****************************************************************************80

int r8vec_amax_index ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AMAX_INDEX returns the index of the maximum absolute value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, int R8VEC_AMAX_INDEX, the index of the entry of largest magnitude.
//
{
  double amax;
  int amax_index;
  int i;

  if ( n <= 0 )
  {
    amax_index = -1;
  }
  else
  {
    amax_index = 1;
    amax = r8_abs ( a[0] );

    for ( i = 2; i <= n; i++ )
    {
      if ( amax < r8_abs ( a[i-1] ) )
      {
        amax_index = i;
        amax = r8_abs ( a[i-1] );
      }
    }
  }

  return amax_index;
}
//****************************************************************************80

double r8vec_amin ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AMIN returns the minimum absolute value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, double R8VEC_AMIN, the value of the entry
//    of smallest magnitude.
//
{
  double amin;
  int i;

  amin = r8_huge ( );
  for ( i = 0; i < n; i++ )
  {
    if ( r8_abs ( a[i] ) < amin )
    {
      amin = r8_abs ( a[i] );
    }
  }

  return amin;
}
//****************************************************************************80

int r8vec_amin_index ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_AMIN_INDEX returns the index of the minimum absolute value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, int R8VEC_AMIN_INDEX, the index of the entry of smallest magnitude.
//
{
  double amin;
  int amin_index;
  int i;

  if ( n <= 0 )
  {
    amin_index = -1;
  }
  else
  {
    amin_index = 1;
    amin = r8_abs ( a[0] );

    for ( i = 2; i <= n; i++ )
    {
      if ( r8_abs ( a[i-1] ) < amin )
      {
        amin_index = i;
        amin = r8_abs ( a[i-1] );
      }
    }
  }

  return amin_index;
}
//****************************************************************************80

void r8vec_bracket ( int n, double x[], double xval, int *left, 
  int *right )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_BRACKET searches a sorted array for successive brackets of a value.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If the values in the vector are thought of as defining intervals
//    on the real line, then this routine searches for the interval
//    nearest to or containing the given value.
//
//    It is always true that RIGHT = LEFT+1.
//
//    If XVAL < X[0], then LEFT = 1, RIGHT = 2, and
//      XVAL   < X[0] < X[1];
//    If X(1) <= XVAL < X[N-1], then
//      X[LEFT-1] <= XVAL < X[RIGHT-1]; 
//    If X[N-1] <= XVAL, then LEFT = N-1, RIGHT = N, and
//      X[LEFT-1] <= X[RIGHT-1] <= XVAL.
//
//    For consistency, this routine computes indices RIGHT and LEFT
//    that are 1-based, although it would be more natural in C and
//    C++ to use 0-based values.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 February 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input, double X[N], an array that has been sorted into ascending order.
//
//    Input, double XVAL, a value to be bracketed.
//
//    Output, int *LEFT, *RIGHT, the results of the search.
//
{
  int i;

  for ( i = 2; i <= n - 1; i++ ) 
  {
    if ( xval < x[i-1] ) 
    {
      *left = i - 1;
      *right = i;
      return;
    }

   }

  *left = n - 1;
  *right = n;

  return;
}
//****************************************************************************80

void r8vec_bracket2 ( int n, double x[], double xval, int start, int *left, 
  int *right )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_BRACKET2 searches a sorted array for successive brackets of a value.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If the values in the vector are thought of as defining intervals
//    on the real line, then this routine searches for the interval
//    containing the given value.
//
//    R8VEC_BRACKET2 is a variation on R8VEC_BRACKET.  It seeks to reduce
//    the search time by allowing the user to suggest an interval that
//    probably contains the value.  The routine will look in that interval
//    and the intervals to the immediate left and right.  If this does
//    not locate the point, a binary search will be carried out on
//    appropriate subportion of the sorted array.
//
//    In the most common case, 1 <= LEFT < LEFT + 1 = RIGHT <= N,
//    and X(LEFT) <= XVAL <= X(RIGHT).
//
//    Special cases:
//      Value is less than all data values:
//    LEFT = -1, RIGHT = 1, and XVAL < X(RIGHT).
//      Value is greater than all data values:
//    LEFT = N, RIGHT = -1, and X(LEFT) < XVAL.
//      Value is equal to a data value:
//    LEFT = RIGHT, and X(LEFT) = X(RIGHT) = XVAL.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of the input array.
//
//    Input, double X[N], an array that has been sorted into
//    ascending order.
//
//    Input, double XVAL, a value to be bracketed by entries of X.
//
//    Input, int START, between 1 and N, specifies that XVAL
//    is likely to be in the interval:
//      [ X(START), X(START+1) ]
//    or, if not in that interval, then either
//      [ X(START+1), X(START+2) ]
//    or
//      [ X(START-1), X(START) ].
//
//    Output, int *LEFT, *RIGHT, the results of the search.
//
{
  int high;
  int low;
//
//  Check.
//
  if ( n < 1 )
  {
    cout << "\n";
    cout << "R8VEC_BRACKET2 - Fatal error!\n";
    cout << "  N < 1.\n";
    exit ( 1 );
  }

  if ( start < 1 || n < start )
  {
    start = ( n + 1 ) / 2;
  }
//
//  XVAL = X(START)?
//
  if ( x[start-1] == xval )
  {
    *left = start;
    *right = start;
    return;
  }
//
//  X(START) < XVAL?
//
  else if ( x[start-1] < xval )
  {
//
//  X(START) = X(N) < XVAL < Infinity?
//
    if ( n < start + 1 )
    {
      *left = start;
      *right = -1;
      return;
    }
//
//  XVAL = X(START+1)?
//
    else if ( xval == x[start] )
    {
      *left = start + 1;
      *right = start + 1;
      return;
    }
//
//  X(START) < XVAL < X(START+1)?
//
    else if ( xval < x[start] )
    {
      *left = start;
      *right = start + 1;
      return;
    }
//
//  X(START+1) = X(N) < XVAL < Infinity?
//
    else if ( n < start + 2 )
    {
      *left = start + 1;
      *right = -1;
      return;
    }
//
//  XVAL = X(START+2)?
//
    else if ( xval == x[start+1] )
    {
      *left = start + 2;
      *right = start + 2;
      return;
    }
//
//  X(START+1) < XVAL < X(START+2)?
//
    else if ( xval < x[start+1] )
    {
      *left = start + 1;
      *right = start + 2;
      return;
    }
//
//  Binary search for XVAL in [ X(START+2), X(N) ],
//  where XVAL is guaranteed to be greater than X(START+2).
//
    else
    {
      low = start + 2;
      high = n;

      r8vec_bracket ( high + 1 - low, x+low-1, xval, left, right );

      *left = *left + low - 1;
      *right = *right + low - 1;
    }
  }
//
//  -Infinity < XVAL < X(START) = X(1).
//
  else if ( start == 1 )
  {
    *left = -1;
    *right = start;
    return;
  }
//
//  XVAL = X(START-1)?
//
  else if ( xval == x[start-2] )
  {
    *left = start - 1;
    *right = start - 1;
    return;
  }
//
//  X(START-1) < XVAL < X(START)?
//
  else if ( x[start-2] <= xval )
  {
    *left = start - 1;
    *right = start;
    return;
  }
//
//  Binary search for XVAL in [ X(1), X(START-1) ],
//  where XVAL is guaranteed to be less than X(START-1).
//
  else
  {
    low = 1;
    high = start - 1;
    r8vec_bracket ( high + 1 - low, x, xval, left, right );
  }

  return;
}
//****************************************************************************80

void r8vec_bracket3 ( int n, double t[], double tval, int *left )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_BRACKET3 finds the interval containing or nearest a given value.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The routine always returns the index LEFT of the sorted array
//    T with the property that either
//    *  T is contained in the interval [ T[LEFT], T[LEFT+1] ], or
//    *  T < T[LEFT] = T[0], or
//    *  T > T[LEFT+1] = T[N-1].
//
//    The routine is useful for interpolation problems, where
//    the abscissa must be located within an interval of data
//    abscissas for interpolation, or the "nearest" interval
//    to the (extreme) abscissa must be found so that extrapolation
//    can be carried out.
//
//    For consistency with other versions of this routine, the
//    value of LEFT is assumed to be a 1-based index.  This is
//    contrary to the typical C and C++ convention of 0-based indices.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 February 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of the input array.
//
//    Input, double T[N], an array that has been sorted into ascending order.
//
//    Input, double TVAL, a value to be bracketed by entries of T.
//
//    Input/output, int *LEFT.
//
//    On input, if 1 <= LEFT <= N-1, LEFT is taken as a suggestion for the
//    interval [ T[LEFT-1] T[LEFT] ] in which TVAL lies.  This interval
//    is searched first, followed by the appropriate interval to the left
//    or right.  After that, a binary search is used.
//
//    On output, LEFT is set so that the interval [ T[LEFT-1], T[LEFT] ]
//    is the closest to TVAL; it either contains TVAL, or else TVAL
//    lies outside the interval [ T[0], T[N-1] ].
//
{
  int high;
  int low;
  int mid;
//  
//  Check the input data.
//
  if ( n < 2 ) 
  {
    cout << "\n";
    cout << "R8VEC_BRACKET3 - Fatal error!\n";
    cout << "  N must be at least 2.\n";
    exit ( 1 );
  }
//
//  If *LEFT is not between 1 and N-1, set it to the middle value.
//
  if ( *left < 1 || n - 1 < *left ) 
  {
    *left = ( n + 1 ) / 2;
  }

//
//  CASE 1: TVAL < T[*LEFT]:
//  Search for TVAL in (T[I],T[I+1]), for I = 1 to *LEFT-1.
//
  if ( tval < t[*left] ) 
  {

    if ( *left == 1 ) 
    {
      return;
    }
    else if ( *left == 2 ) 
    {
      *left = 1;
      return;
    }
    else if ( t[*left-2] <= tval )
    {
      *left = *left - 1;
      return;
    }
    else if ( tval <= t[1] ) 
    {
      *left = 1;
      return;
    }
// 
//  ...Binary search for TVAL in (T[I-1],T[I]), for I = 2 to *LEFT-2.
//
    low = 2;
    high = *left - 2;

    for ( ; ; )
    {
      if ( low == high )
      {
        *left = low;
        return;
      }

      mid = ( low + high + 1 ) / 2;

      if ( t[mid-1] <= tval ) 
      {
        low = mid;
      }
      else 
      {
        high = mid - 1;
      }
    }
  }
// 
//  CASE 2: T[*LEFT] < TVAL:
//  Search for TVAL in (T[I-1],T[I]) for intervals I = *LEFT+1 to N-1.
//
  else if ( t[*left] < tval ) 
  {

    if ( *left == n - 1 ) 
    {
      return;
    }
    else if ( *left == n - 2 ) 
    {
      *left = *left + 1;
      return;
    }
    else if ( tval <= t[*left+1] )
    {
      *left = *left + 1;
      return;
    }
    else if ( t[n-2] <= tval ) 
    {
      *left = n - 1;
      return;
    }
// 
//  ...Binary search for TVAL in (T[I-1],T[I]) for intervals I = *LEFT+2 to N-2.
//
    low = *left + 2;
    high = n - 2;

    for ( ; ; ) 
    {

      if ( low == high ) 
      {
        *left = low;
        return;
      }

      mid = ( low + high + 1 ) / 2;

      if ( t[mid-1] <= tval ) 
      {
        low = mid;
      }
      else 
      {
        high = mid - 1;
      }
    }
  }
//
//  CASE 3: T[*LEFT-1] <= TVAL <= T[*LEFT]:
//  T is just where the user said it might be.
//
  else 
  {
  }

  return;
}
//****************************************************************************80

double r8vec_circular_variance ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_CIRCULAR_VARIANCE returns the circular variance of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double X(N), the vector whose variance is desired.
//
//    Output, double R8VEC_CIRCULAR VARIANCE, the circular variance
//    of the vector entries.
//
{
  int i;
  double mean;
  double sum_c;
  double sum_s;
  double value;

  mean = r8vec_mean ( n, x );

  sum_c = 0.0;
  for ( i = 0; i < n; i++ )
  {
    sum_c = sum_c + cos ( x[i] - mean );
  }

  sum_s = 0.0;
  for ( i = 0; i < n; i++ )
  {
    sum_s = sum_s + sin ( x[i] - mean );
  }

  value = sqrt ( sum_c * sum_c + sum_s * sum_s ) / ( double ) n;

  value = 1.0 - value;

  return value;
}
//****************************************************************************80

int r8vec_compare ( int n, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_COMPARE compares two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The lexicographic ordering is used.
//
//  Example:
//
//    Input:
//
//      A1 = ( 2.0, 6.0, 2.0 )
//      A2 = ( 2.0, 8.0, 12.0 )
//
//    Output:
//
//      ISGN = -1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A[N], B[N], the vectors to be compared.
//
//    Output, int R8VEC_COMPARE, the results of the comparison:
//    -1, A is lexicographically less than B,
//     0, A is equal to B,
//    +1, A is lexicographically greater than B.
//
{
  int isgn;
  int k;

  isgn = 0;

  for ( k = 0; k < n; k++ )
  {
    if ( a[k] < b[k] )
    {
      isgn = -1;
      return isgn;
    }
    else if ( b[k] < a[k] )
    {
      isgn = +1;
      return isgn;
    }
  }
  return isgn;
}
//****************************************************************************80

double *r8vec_convolve_circ ( int n, double x[], double y[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_CONVOLVE_CIRC returns the discrete circular convolution of two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    z(1+m) = xCCy(m) = sum ( 0 <= k <= n-1 ) x(1+k) * y(1+m-k)
//
//    Here, if the index of Y becomes nonpositive, it is "wrapped around"
//    by having N added to it.
//
//    The circular convolution is equivalent to multiplication of Y by a
//    circulant matrix formed from the vector X.
//
//  Example:
//
//    Input:
//
//      X = (/ 1, 2, 3, 4 /)
//      Y = (/ 1, 2, 4, 8 /)
//
//    Output:
//
//      Circulant form:
//
//      Z = ( 1 4 3 2 )   ( 1 )
//          ( 2 1 4 3 )   ( 2 )
//          ( 3 2 1 4 ) * ( 4 )
//          ( 4 3 2 1 )   ( 8 )
//
//      The formula:
//
//      Z = (/ 1*1 + 2*8 + 3*4 + 4*2,
//             1*2 + 2*1 + 3*8 + 4*4,
//             1*4 + 2*2 + 3*1 + 4*8,
//             1*8 + 2*4 + 3*2 + 4*1 /)
//
//      Result:
//
//      Z = (/ 37, 44, 43, 26 /)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the vectors.
//
//    Input, double X[N], Y[N], the vectors to be convolved.
//
//    Output, double R8VEC_CONVOLVE_CIRC[N], the circular convolution of X and Y.
//
{
  int i;
  int m;
  double *z;

  z = new double[n];

  for ( m = 1; m <= n; m++ )
  {
    z[m-1] = 0.0;
    for ( i = 1; i <= m; i++ )
    {
      z[m-1] = z[m-1] + x[i-1] * y[m-i];
    }
    for ( i = m+1; i <= n; i++ )
    {
      z[m-1] = z[m-1] + x[i-1] * y[n+m-i];
    }
  }

  return z;
}
//****************************************************************************80

void r8vec_copy ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_COPY copies an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], the vector to be copied.
//
//    Output, double A2[N], the copy of A1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return;
}
//****************************************************************************80

double *r8vec_copy_new ( int n, double a1[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_COPY_NEW copies an R8VEC to a "new" R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], the vector to be copied.
//
//    Output, double R8VEC_COPY_NEW[N], the copy of A1.
//
{
  double *a2;
  int i;

  a2 = new double[n];

  for ( i = 0; i < n; i++ )
  {
    a2[i] = a1[i];
  }
  return a2;
}
//****************************************************************************80

double *r8vec_cross_3d ( double v1[3], double v2[3] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_CROSS_3D computes the cross product of two R8VEC's in 3D.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    07 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double V1[3], V2[3], the coordinates of the vectors.
//
//    Output, double R8VEC_CROSS_3D[3], the cross product vector.
//
{
  double *v3;

  v3 = new double[3];

  v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
  v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
  v3[2] = v1[0] * v2[1] - v1[1] * v2[0];

  return v3;
}
//****************************************************************************80

double *r8vec_dif ( int n, double h )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIF computes coefficients for estimating the N-th derivative.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The routine computes the N+1 coefficients for a centered finite difference
//    estimate of the N-th derivative of a function.
//
//    The estimate has the form
//
//      FDIF(N,X) = Sum (I = 0 to N) COF(I) * F ( X(I) )
//
//    To understand the computation of the coefficients, it is enough
//    to realize that the first difference approximation is
//
//      FDIF(1,X) = F(X+DX) - F(X-DX) ) / (2*DX)
//
//    and that the second difference approximation can be regarded as
//    the first difference approximation repeated:
//
//      FDIF(2,X) = FDIF(1,X+DX) - FDIF(1,X-DX) / (2*DX)
//         = F(X+2*DX) - 2 F(X) + F(X-2*DX) / (4*DX)
//
//    and so on for higher order differences.
//
//    Thus, the next thing to consider is the integer coefficients of
//    the sampled values of F, which are clearly the Pascal coefficients,
//    but with an alternating negative sign.  In particular, if we
//    consider row I of Pascal's triangle to have entries j = 0 through I,
//    then P(I,J) = P(I-1,J-1) - P(I-1,J), where P(*,-1) is taken to be 0,
//    and P(0,0) = 1.
//
//       1
//      -1  1
//       1 -2   1
//      -1  3  -3   1
//       1 -4   6  -4   1
//      -1  5 -10  10  -5  1
//       1 -6  15 -20  15 -6 1
//
//    Next, note that the denominator of the approximation for the
//    N-th derivative will be (2*DX)**N.
//
//    And finally, consider the location of the N+1 sampling
//    points for F:
//
//      X-N*DX, X-(N-2)*DX, X-(N-4)*DX, ..., X+(N-4)*DX, X+(N-2*DX), X+N*DX.
//
//    Thus, a formula for evaluating FDIF(N,X) is
//
//      fdif = 0.0
//      do i = 0, n
//        xi = x + (2*i-n) * h
//        fdif = fdif + cof(i) * f(xi)
//      end do
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the derivative to be approximated.
//    N must be 0 or greater.
//
//    Input, double H, the half spacing between points. 
//    H must be positive.
//
//    Output, double R8VEC_DIF[N+1], the coefficients needed to approximate
//    the N-th derivative of a function F.
//
{
  double *cof;
  int i;
  int j;

  if ( n < 0 )
  {
    cout << "\n";
    cout << "R8VEC_DIF - Fatal error!\n";
    cout << "  Derivative order N = " << n << "\n";
    cout << "  but N must be at least 0.\n";
    return NULL;
  }

  if ( h <= 0.0 )
  {
    cout << "\n";
    cout << "R8VEC_DIF - Fatal error!\n";
    cout << "  The half sampling spacing is H = " << h << "\n";
    cout << "  but H must be positive.\n";
    return NULL;
  }

  cof = new double[n+1];

  for ( i = 0; i <= n; i++ )
  {
    cof[i] = 1.0;

    for ( j = i-1; 1 <= j; j-- )
    {
      cof[j] = -cof[j] + cof[j-1];
    }

    if ( 0 < i )
    {
      cof[0] = -cof[0];
    }
  }

  for ( i = 0; i <= n; i++ )
  {
    cof[i] = cof[i] / pow ( 2.0 * h, n );
  }

  return cof;
}
//****************************************************************************80

void r8vec_direct_product ( int factor_index, int factor_order, 
  double factor_value[], int factor_num, int point_num, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIRECT_PRODUCT creates a direct product of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    To explain what is going on here, suppose we had to construct
//    a multidimensional quadrature rule as the product of K rules
//    for 1D quadrature.
//
//    The product rule will be represented as a list of points and weights.
//
//    The J-th item in the product rule will be associated with
//      item J1 of 1D rule 1,
//      item J2 of 1D rule 2, 
//      ..., 
//      item JK of 1D rule K.
//
//    In particular, 
//      X(J) = ( X(1,J1), X(2,J2), ..., X(K,JK))
//    and
//      W(J) = W(1,J1) * W(2,J2) * ... * W(K,JK)
//
//    So we can construct the quadrature rule if we can properly
//    distribute the information in the 1D quadrature rules.
//
//    This routine carries out that task.
//
//    Another way to do this would be to compute, one by one, the
//    set of all possible indices (J1,J2,...,JK), and then index
//    the appropriate information.  An advantage of the method shown
//    here is that you can process the K-th set of information and
//    then discard it.
//
//  Example:
//
//    Rule 1: 
//      Order = 4
//      X(1:4) = ( 1, 2, 3, 4 )
//
//    Rule 2:
//      Order = 3
//      X(1:3) = ( 10, 20, 30 )
//
//    Rule 3:
//      Order = 2
//      X(1:2) = ( 100, 200 )
//
//    Product Rule:
//      Order = 24
//      X(1:24) = 
//        ( 1, 10, 100 )
//        ( 2, 10, 100 )
//        ( 3, 10, 100 )
//        ( 4, 10, 100 )
//        ( 1, 20, 100 )
//        ( 2, 20, 100 )
//        ( 3, 20, 100 )
//        ( 4, 20, 100 )
//        ( 1, 30, 100 )
//        ( 2, 30, 100 )
//        ( 3, 30, 100 )
//        ( 4, 30, 100 )
//        ( 1, 10, 200 )
//        ( 2, 10, 200 )
//        ( 3, 10, 200 )
//        ( 4, 10, 200 )
//        ( 1, 20, 200 )
//        ( 2, 20, 200 )
//        ( 3, 20, 200 )
//        ( 4, 20, 200 )
//        ( 1, 30, 200 )
//        ( 2, 30, 200 )
//        ( 3, 30, 200 )
//        ( 4, 30, 200 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int FACTOR_INDEX, the index of the factor being processed.
//    The first factor processed must be factor 0.
//
//    Input, int FACTOR_ORDER, the order of the factor.
//
//    Input, double FACTOR_VALUE[FACTOR_ORDER], the factor values
//    for factor FACTOR_INDEX.
//
//    Input, int FACTOR_NUM, the number of factors.
//
//    Input, int POINT_NUM, the number of elements in the direct product.
//
//    Input/output, double X[FACTOR_NUM*POINT_NUM], the elements of the
//    direct product, which are built up gradually.  Before the first call,
//    X might be set to 0.  After each factor has been input, X should
//    have the correct value.
//
//  Local Parameters:
//
//    Local, int START, the first location of a block of values to set.
//
//    Local, int CONTIG, the number of consecutive values to set.
//
//    Local, int SKIP, the distance from the current value of START
//    to the next location of a block of values to set.
//
//    Local, int REP, the number of blocks of values to set.
//
{
  static int contig = 0;
  int i;
  int j;
  int k;
  static int rep = 0;
  static int skip = 0;
  int start;

  if ( factor_index == 0 )
  {
    contig = 1;
    skip = 1;
    rep = point_num;
  }

  rep = rep / factor_order;
  skip = skip * factor_order;

  for ( j = 0; j < factor_order; j++ )
  {
    start = 0 + j * contig;

    for ( k = 1; k <= rep; k++ )
    {
      for ( i = start; i < start + contig; i++ )
      {
        x[factor_index+i*factor_num] = factor_value[j];
      }
      start = start + skip;
    }
  }
  contig = contig * factor_order;

  return;
}
//****************************************************************************80

void r8vec_direct_product2 ( int factor_index, int factor_order, 
  double factor_value[], int factor_num, int point_num, double w[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIRECT_PRODUCT2 creates a direct product of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    To explain what is going on here, suppose we had to construct
//    a multidimensional quadrature rule as the product of K rules
//    for 1D quadrature.
//
//    The product rule will be represented as a list of points and weights.
//
//    The J-th item in the product rule will be associated with
//      item J1 of 1D rule 1,
//      item J2 of 1D rule 2, 
//      ..., 
//      item JK of 1D rule K.
//
//    In particular, 
//      X(J) = ( X(1,J1), X(2,J2), ..., X(K,JK))
//    and
//      W(J) = W(1,J1) * W(2,J2) * ... * W(K,JK)
//
//    So we can construct the quadrature rule if we can properly
//    distribute the information in the 1D quadrature rules.
//
//    This routine carries out that task for the weights W.
//
//    Another way to do this would be to compute, one by one, the
//    set of all possible indices (J1,J2,...,JK), and then index
//    the appropriate information.  An advantage of the method shown
//    here is that you can process the K-th set of information and
//    then discard it.
//
//  Example:
//
//    Rule 1: 
//      Order = 4
//      W(1:4) = ( 2, 3, 5, 7 )
//
//    Rule 2:
//      Order = 3
//      W(1:3) = ( 11, 13, 17 )
//
//    Rule 3:
//      Order = 2
//      W(1:2) = ( 19, 23 )
//
//    Product Rule:
//      Order = 24
//      W(1:24) =
//        ( 2 * 11 * 19 )
//        ( 3 * 11 * 19 )
//        ( 4 * 11 * 19 )
//        ( 7 * 11 * 19 )
//        ( 2 * 13 * 19 )
//        ( 3 * 13 * 19 )
//        ( 5 * 13 * 19 )
//        ( 7 * 13 * 19 )
//        ( 2 * 17 * 19 )
//        ( 3 * 17 * 19 )
//        ( 5 * 17 * 19 )
//        ( 7 * 17 * 19 )
//        ( 2 * 11 * 23 )
//        ( 3 * 11 * 23 )
//        ( 5 * 11 * 23 )
//        ( 7 * 11 * 23 )
//        ( 2 * 13 * 23 )
//        ( 3 * 13 * 23 )
//        ( 5 * 13 * 23 )
//        ( 7 * 13 * 23 )
//        ( 2 * 17 * 23 )
//        ( 3 * 17 * 23 )
//        ( 5 * 17 * 23 )
//        ( 7 * 17 * 23 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 May 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int FACTOR_INDEX, the index of the factor being processed.
//    The first factor processed must be factor 0.
//
//    Input, int FACTOR_ORDER, the order of the factor.
//
//    Input, double FACTOR_VALUE[FACTOR_ORDER], the factor values for
//    factor FACTOR_INDEX.
//
//    Input, int FACTOR_NUM, the number of factors.
//
//    Input, int POINT_NUM, the number of elements in the direct product.
//
//    Input/output, double W[POINT_NUM], the elements of the
//    direct product, which are built up gradually.  Before the first call,
//    W must be set to 1.
//
//  Local Parameters:
//
//    Local, integer START, the first location of a block of values to set.
//
//    Local, integer CONTIG, the number of consecutive values to set.
//
//    Local, integer SKIP, the distance from the current value of START
//    to the next location of a block of values to set.
//
//    Local, integer REP, the number of blocks of values to set.
//
{
  static int contig = 0;
  int i;
  int j;
  int k;
  static int rep = 0;
  static int skip = 0;
  int start;

  if ( factor_index == 0 )
  {
    contig = 1;
    skip = 1;
    rep = point_num;
  }

  rep = rep / factor_order;
  skip = skip * factor_order;

  for ( j = 0; j < factor_order; j++ )
  {
    start = 0 + j * contig;

    for ( k = 1; k <= rep; k++ )
    {
      for ( i = start; i < start + contig; i++ )
      {
        w[i] = w[i] * factor_value[j];
      }
      start = start + skip;
    }
  }

  contig = contig * factor_order;

  return;
}
//****************************************************************************80

double r8vec_distance ( int dim_num, double v1[], double v2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DISTANCE returns the Euclidean distance between two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, double V1[DIM_NUM], V2[DIM_NUM], the vectors.
//
//    Output, double R8VEC_DISTANCE, the Euclidean distance
//    between the vectors.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < dim_num; i++ )
  {
    value = pow ( v1[i] - v2[i], 2 );
  }
  value = sqrt ( value );

  return value;
}
//****************************************************************************80

bool r8vec_distinct ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DISTINCT is true if the entries in an R8VEC are distinct.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 January 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double X[N], the vector to be checked.
//
//    Output, bool R8VEC_DISTINCT is true if all N elements of X
//    are distinct.
//
{
  int i;
  int j;

  for ( i = 1; i <= n-1; i++ ) 
  {
    for ( j = 1; j <= i - 1; j++ ) 
    {
      if ( x[i] == x[j] ) 
      {
        return false;
      }
    }
  }
  return true;
}
//****************************************************************************80

void r8vec_divide ( int n, double a[], double s )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DIVIDE divides an R8VEC by a nonzero scalar.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, double A[N].  On input, the vector to be scaled.
//    On output, each entry has been divided by S.
//
//    Input, double S, the divisor.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = a[i] / s;
  }
  return;
}
//****************************************************************************80

double r8vec_dot ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DOT computes the dot product of a pair of R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], A2[N], the two vectors to be considered.
//
//    Output, double R8VEC_DOT, the dot product of the vectors.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a1[i] * a2[i];
  }
  return value;
}
//****************************************************************************80

bool r8vec_eq ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EQ is true if every pair of entries in two R8VEC's is equal.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], A2[N], two vectors to compare.
//
//    Output, bool R8VEC_EQ, is TRUE if every pair of elements A1(I) and A2(I) are equal,
//    and FALSE otherwise.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    if ( a1[i] != a2[i] )
    {
      return false;
    }
  }
  return true;
}
//****************************************************************************80

double *r8vec_even ( int n, double alo, double ahi )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EVEN returns N real values, evenly spaced between ALO and AHI.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 February 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of values.
//
//    Input, double ALO, AHI, the low and high values.
//
//    Output, double R8VEC_EVEN[N], N evenly spaced values.
//    Normally, A(1) = ALO and A(N) = AHI.
//    However, if N = 1, then A(1) = 0.5*(ALO+AHI).
//
{
  double *a;
  int i;

  a = new double[n];

  if ( n == 1 )
  {
    a[0] = 0.5 * ( alo + ahi );
  }
  else
  {
    for ( i = 1; i <= n; i++ )
    {
      a[i-1] = ( ( double ) ( n - i     ) * alo 
               + ( double ) (     i - 1 ) * ahi ) 
               / ( double ) ( n     - 1 );
    }
  }

  return a;
}
//****************************************************************************80

double r8vec_even_select ( int n, double xlo, double xhi, int ival )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EVEN_SELECT returns the I-th of N evenly spaced values in [ XLO, XHI ].
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    XVAL = ( (N-IVAL) * XLO + (IVAL-1) * XHI ) / ( N - 1 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 January 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of values.
//
//    Input, double XLO, XHI, the low and high values.
//
//    Input, int IVAL, the index of the desired point.
//    IVAL is normally between 1 and N, but may be any integer value.
//
//    Output, double R8VEC_EVEN_SELECT, the IVAL-th of N evenly spaced values
//    between XLO and XHI.
//    Unless N = 1, X(1) = XLO and X(N) = XHI.
//    If N = 1, then X(1) = 0.5*(XLO+XHI).
//
{
  double xval;

  if ( n == 1 )
  {
    xval = 0.5 * ( xlo + xhi );
  }
  else
  {
    xval = ( ( double ) ( n - ival     ) * xlo 
           + ( double ) (     ival - 1 ) * xhi ) 
           / ( double ) ( n        - 1 );
  }

  return xval;
}
//****************************************************************************80

void r8vec_even2 ( int maxval, int nfill[], int nold, double xold[], 
  int *nval, double xval[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EVEN2 linearly interpolates new numbers into an R8VECa.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The number of values created between two old values can vary from
//    one pair of values to the next.
//
//    The interpolated values are evenly spaced.
//
//    This routine is a generalization of R8VEC_EVEN.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 November 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int MAXVAL, the size of the XVAL array, as declared by the
//    user.  MAXVAL must be large enough to hold the NVAL values computed by
//    this routine.  In other words, MAXVAL must be at least equal to
//    NOLD + SUM (1 <= I <= NOLD-1) NFILL(I).
//
//    Input, int NFILL[NOLD-1], the number of values
//    to be interpolated between XOLD(I) and XOLD(I+1).
//    NFILL(I) does not count the endpoints.  Thus, if
//    NFILL(I) is 1, there will be one new point generated
//    between XOLD(I) and XOLD(I+1).
//    NFILL(I) must be nonnegative.
//
//    Input, int NOLD, the number of values XOLD,
//    between which extra values are to be interpolated.
//
//    Input, double XOLD[NOLD], the original vector of numbers
//    between which new values are to be interpolated.
//
//    Output, int *NVAL, the number of values computed
//    in the XVAL array.
//    NVAL = NOLD + SUM ( 1 <= I <= NOLD-1 ) NFILL(I)
//
//    Output, double XVAL[MAXVAL].  On output, XVAL contains the
//    NOLD values of XOLD, as well as the interpolated
//    values, making a total of NVAL values.
//
{
  int i;
  int j;
  int nadd;

  *nval = 1;

  for ( i = 1; i <= nold - 1; i++ )
  {

    if ( nfill[i-1] < 0 )
    {
      cout << "\n";
      cout << "R8VEC_EVEN2 - Fatal error!\n";
      cout << "  NFILL[I-1] is negative for I = " << i << "\n";
      cout << "  NFILL[I-1] = " << nfill[i-1] << "\n";
      exit ( 1 );
    }

    if ( maxval < *nval + nfill[i-1] + 1 )
    {
      cout << "\n";
      cout << "R8VEC_EVEN2 - Fatal error!\n";
      cout << "  MAXVAL = " << maxval << " is not large enough.\n";
      cout << "  for the storage for interval I = " << i << "\n";
      exit ( 1 );
    }

    nadd = nfill[i-1] + 2;

    for ( j = 1; j <= nadd; j++ )
    {
      xval[*nval+j-2] = ( ( double ) ( nadd - j     ) * xold[i-1] 
                        + ( double ) (        j - 1 ) * xold[i] ) 
                        / ( double ) ( nadd     - 1 );
    }

    *nval = *nval + nfill[i-1] + 1;
  }

  return;
}
//****************************************************************************80

void r8vec_even3 ( int nold, int nval, double xold[], double xval[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EVEN3 evenly interpolates new data into an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    This routine accepts a short vector of numbers, and returns a longer
//    vector of numbers, created by interpolating new values between
//    the given values.
//
//    Between any two original values, new values are evenly interpolated.
//
//    Over the whole vector, the new numbers are interpolated in
//    such a way as to try to minimize the largest distance interval size.
//
//    The algorithm employed is not "perfect".
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NOLD, the number of values XOLD, between which extra
//    values are to be interpolated.
//
//    Input, int NVAL, the number of values to be computed
//    in the XVAL array.  NVAL should be at least NOLD.
//
//    Input, double XOLD[NOLD], the original vector of numbers
//    between which new values are to be interpolated.
//
//    Output, double XVAL[NVAL].  On output, XVAL contains the
//    NOLD values of XOLD, as well as interpolated
//    values, making a total of NVAL values.
//
{
  double density;
  int i;
  int ival;
  int j;
  int nmaybe;
  int npts;
  int ntemp;
  int ntot;
  double xlen;
  double xleni;
  double xlentot;

  xlen = 0.0;
  for ( i = 1; i <= nold - 1; i++ )
  {
    xlen = xlen + r8_abs ( xold[i] - xold[i-1] );
  }

  ntemp = nval - nold;

  density = ( double ) ( ntemp ) / xlen;

  ival = 1;
  ntot = 0;
  xlentot = 0.0;

  for ( i = 1; i <= nold - 1; i++ )
  {
    xleni = r8_abs ( xold[i] - xold[i-1] );
    npts = ( int ) ( density * xleni );
    ntot = ntot + npts;
//
//  Determine if we have enough left-over density that it should
//  be changed into a point.  A better algorithm would agonize
//  more over where that point should go.
//
    xlentot = xlentot + xleni;
    nmaybe = r8_nint ( xlentot * density );

    if ( ntot < nmaybe )
    {
      npts = npts + nmaybe - ntot;
      ntot = nmaybe;
    }
    for ( j = 1; j <= npts + 2; j++ )
    {
      xval[ival+j-2] = ( ( double ) ( npts+2 - j     ) * xold[i-1]   
                       + ( double ) (          j - 1 ) * xold[i] ) 
                       / ( double ) ( npts+2     - 1 );
    }
    ival = ival + npts + 1;
  }

  return;
}
//****************************************************************************80

double *r8vec_expand_linear ( int n, double x[], int fat )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_EXPAND_LINEAR linearly interpolates new data into an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of input data values.
//
//    Input, double X[N], the original data.
//
//    Input, int FAT, the number of data values to interpolate
//    between each pair of original data values.
//
//    Output, double R8VEC_EXPAND_LINEAR[(N-1)*(FAT+1)+1], the "fattened" data.
//
{
  int i;
  int j;
  int k;
  double *xfat;

  xfat = new double[(n-1)*(fat+1)+1];

  k = 0;

  for ( i = 0; i < n-1; i++ )
  {
    xfat[k] = x[i];
    k = k + 1;

    for ( j = 1; j <= fat; j++ )
    {
      xfat[k] = ( ( double ) ( fat - j + 1 ) * x[i]     
                + ( double ) (       j     ) * x[i+1] ) 
                / ( double ) ( fat     + 1 );
      k = k + 1;
    }
  }

  xfat[k] = x[n-1];
  k = k + 1;

  return xfat;
}
//****************************************************************************80

int *r8vec_first_index ( int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_FIRST_INDEX indexes the first occurrence of values in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    For element A(I) of the vector, FIRST_INDEX(I) is the index in A of
//    the first occurrence of the value A(I).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the unsorted array to examine.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8VEC_FIRST_INDEX[N], the first occurrence index.
//
{
  int *first_index;
  int i;
  int j;

  first_index = new int[n];

  for ( i = 0; i < n; i++ )
  {
    first_index[i] = -1;
  }
  for ( i = 0; i < n; i++ )
  {
    if ( first_index[i] == -1 )
    {
      first_index[i] = i;
      for ( j = i + 1; j < n; j++ )
      {
        if ( r8_abs ( a[i] - a[j] ) <= tol )
        {
          first_index[j] = i;
        }
      }
    }
  }
  return first_index;
}
//****************************************************************************80

double r8vec_frac ( int n, double a[], int k )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_FRAC searches for the K-th smallest entry in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Hoare's algorithm is used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 August 2004
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, double A[N].
//    On input, A is the array to search.
//    On output, the elements of A have been somewhat rearranged.
//
//    Input, int K, the fractile to be sought.  If K = 1, the minimum
//    entry is sought.  If K = N, the maximum is sought.  Other values
//    of K search for the entry which is K-th in size.  K must be at
//    least 1, and no greater than N.
//
//    Output, double R8VEC_FRAC, the value of the K-th fractile of A.
//
{
  double frac;
  int i;
  int iryt;
  int j;
  int left;
  double temp;
  double w;
  double x;

  if ( n <= 0 )
  {
    cout << "\n";
    cout << "R8VEC_FRAC - Fatal error!\n";
    cout << "  Illegal nonpositive value of N = " << n << "\n";
    exit ( 1 );
  }

  if ( k <= 0 )
  {
    cout << "\n";
    cout << "R8VEC_FRAC - Fatal error!\n";
    cout << "  Illegal nonpositive value of K = " << k << "\n";
    exit ( 1 );
  }

  if ( n < k )
  {
    cout << "\n";
    cout << "R8VEC_FRAC - Fatal error!\n";
    cout << "  Illegal N < K, K = " << k << "\n";
    exit ( 1 );
  }

  left = 1;
  iryt = n;

  for ( ; ; )
  {
    if ( iryt <= left )
    {
      frac = a[k-1];
      break;
    }

    x = a[k-1];
    i = left;
    j = iryt;

    for ( ; ; )
    {
      if ( j < i )
      {
        if ( j < k )
        {
          left = i;
        }
        if ( k < i )
        {
          iryt = j;
        }
        break;
      }
//
//  Find I so that X <= A(I).
//
      while ( a[i-1] < x )
      {
        i = i + 1;
      }
//
//  Find J so that A(J) <= X.
//
      while ( x < a[j-1] )
      {
        j = j - 1;
      }

      if ( i <= j )
      {
        temp   = a[i-1];
        a[i-1] = a[j-1];
        a[j-1] = temp;
        i = i + 1;
        j = j - 1;
      }
    }
  }

  return frac;
}
//****************************************************************************80

double *r8vec_fraction ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_FRACTION returns the fraction parts of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If we regard a real number as
//
//      R8 = SIGN * ( WHOLE + FRACTION )
//
//    where
//
//      SIGN is +1 or -1,
//      WHOLE is a nonnegative integer
//      FRACTION is a nonnegative real number strictly less than 1,
//
//    then this routine returns the value of FRACTION.
//
//  Example:
//
//     R8    R8_FRACTION
//
//    0.00      0.00
//    1.01      0.01
//    2.02      0.02
//   19.73      0.73
//   -4.34      0.34
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 April 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of arguments.
//
//    Input, double X[N], the arguments.
//
//    Output, double R8_FRACTION[N], the fraction parts.
//
{
  double *fraction;
  int i;

  fraction = new double[n];

  for ( i = 0; i < n; i++ )
  {
    fraction[i] = r8_abs ( x[i] ) - ( double ) ( ( int ) ( r8_abs ( x[i] ) ) );
  }

  return fraction;
}
//****************************************************************************80

bool r8vec_gt ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_GT == ( A1 > A2 ) for two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The comparison is lexicographic.
//
//    A1 > A2  <=>                              A1(1) > A2(1) or
//                 ( A1(1)     == A2(1)     and A1(2) > A2(2) ) or
//                 ...
//                 ( A1(1:N-1) == A2(1:N-1) and A1(N) > A2(N)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the vectors.
//
//    Input, double A1[N], A2[N], the vectors to be compared.
//
//    Output, bool R8VEC_GT, is TRUE if and only if A1 > A2.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {

    if ( a2[i] < a1[i] )
    {
       return true;
    }
    else if ( a1[i] < a2[i] )
    {
      return false;
    }

  }

  return false;
}
//****************************************************************************80

void r8vec_heap_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_HEAP_A reorders an R8VEC into a ascending heap.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An ascending heap is an array A with the property that, for every index J,
//    A[J] <= A[2*J+1] and A[J] <= A[2*J+2], (as long as the indices
//    2*J+1 and 2*J+2 are legal).
//
//  Diagram:
//
//                  A(0)
//                /      \
//            A(1)         A(2)
//          /     \        /  \
//      A(3)       A(4)  A(5) A(6)
//      /  \       /   \
//    A(7) A(8)  A(9) A(10)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the size of the input array.
//
//    Input/output, double A[N].
//    On input, an unsorted array.
//    On output, the array has been reordered into a heap.
//
{
  int i;
  int ifree;
  double key;
  int m;
//
//  Only nodes (N/2)-1 down to 0 can be "parent" nodes.
//
  for ( i = (n/2)-1; 0 <= i; i-- )
  { 
//
//  Copy the value out of the parent node.
//  Position IFREE is now "open".
//
    key = a[i];
    ifree = i;

    for ( ; ; )
    {
//
//  Positions 2*IFREE + 1 and 2*IFREE + 2 are the descendants of position
//  IFREE.  (One or both may not exist because they equal or exceed N.)
//
      m = 2 * ifree + 1;
//
//  Does the first position exist?
//
      if ( n <= m )
      {
        break;
      }
      else
      {
//
//  Does the second position exist?
//
        if ( m + 1 < n )
        {
//
//  If both positions exist, take the larger of the two values,
//  and update M if necessary.
//
          if ( a[m+1] < a[m] )
          {
            m = m + 1;
          }
        }
//
//  If the large descendant is larger than KEY, move it up,
//  and update IFREE, the location of the free position, and
//  consider the descendants of THIS position.
//
        if ( a[m] <= key )
        {
          break;
        }
        a[ifree] = a[m];
        ifree = m;
      }
    }
//
//  When you have stopped shifting items up, return the item you
//  pulled out back to the heap.
//
    a[ifree] = key;
  }

  return;
}
//****************************************************************************80

void r8vec_heap_d ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_HEAP_D reorders an R8VEC into a descending heap.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    A heap is an array A with the property that, for every index J,
//    A[J] >= A[2*J+1] and A[J] >= A[2*J+2], (as long as the indices
//    2*J+1 and 2*J+2 are legal).
//
//  Diagram:
//
//                  A(0)
//                /      \
//            A(1)         A(2)
//          /     \        /  \
//      A(3)       A(4)  A(5) A(6)
//      /  \       /   \
//    A(7) A(8)  A(9) A(10)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the size of the input array.
//
//    Input/output, double A[N].
//    On input, an unsorted array.
//    On output, the array has been reordered into a heap.
//
{
  int i;
  int ifree;
  double key;
  int m;
//
//  Only nodes (N/2)-1 down to 0 can be "parent" nodes.
//
  for ( i = (n/2)-1; 0 <= i; i-- ) 
  { 
//
//  Copy the value out of the parent node.
//  Position IFREE is now "open".
//
    key = a[i];
    ifree = i;

    for ( ; ; ) 
    {
//
//  Positions 2*IFREE + 1 and 2*IFREE + 2 are the descendants of position
//  IFREE.  (One or both may not exist because they equal or exceed N.)
//
      m = 2 * ifree + 1;
//
//  Does the first position exist?
//
      if ( n <= m ) 
      {
        break;
      }
      else 
      {
//
//  Does the second position exist?
//
        if ( m + 1 < n ) 
        {
//
//  If both positions exist, take the larger of the two values,
//  and update M if necessary.
//
          if ( a[m] < a[m+1] ) 
          {
            m = m + 1;
          }
        }
//
//  If the large descendant is larger than KEY, move it up,
//  and update IFREE, the location of the free position, and
//  consider the descendants of THIS position.
//
        if ( key < a[m] ) 
        {
          a[ifree] = a[m];
          ifree = m;
        }
        else 
        {
          break;
        }
      }
    }
//
//  When you have stopped shifting items up, return the item you
//  pulled out back to the heap.
//
    a[ifree] = key;
  }

  return;
}
//****************************************************************************80

int *r8vec_histogram ( int n, double a[], double a_lo, double a_hi, 
  int histo_num )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_HISTOGRAM histograms an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Values between A_LO and A_HI will be histogrammed into the bins
//    1 through HISTO_NUM.  Values below A_LO are counted in bin 0,
//    and values greater than A_HI are counted in bin HISTO_NUM+1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A(N), the array to examine.
//
//    Input, double A_LO, A_HI, the lowest and highest
//    values to be histogrammed.  These values will also define the bins.
//
//    Input, int HISTO_NUM, the number of bins to use.
//
//    Output, int HISTO_GRAM[HISTO_NUM+2], contains the number of
//    entries of A in each bin.
//
{
  double delta;
  int *histo_gram;
  int i;
  int j;

  histo_gram = new int[histo_num+2];

  i4vec_zero ( histo_num+2, histo_gram );

  delta = ( a_hi - a_lo ) / ( double ) ( 2 * histo_num );

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] < a_lo )
    {
      histo_gram[0] = histo_gram[0] + 1;
    }
    else if ( a[i] <= a_hi )
    {
      j = r8_nint ( 
        ( ( a_hi -       delta - a[i]        ) * ( double ) ( 1         )   
        + (      -       delta + a[i] - a_lo ) * ( double ) ( histo_num ) ) 
        / ( a_hi - 2.0 * delta        - a_lo ) );

      histo_gram[j] = histo_gram[j] + 1;
    }
    else if ( a_hi < a[i] )
    {
      histo_gram[histo_num+1] = histo_gram[histo_num+1] + 1;
    }
  }

  return histo_gram;
}
//****************************************************************************80

double *r8vec_house_column ( int n, double a[], int k )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_HOUSE_COLUMN defines a Householder premultiplier that "packs" a column.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The routine returns a vector V that defines a Householder
//    premultiplier matrix H(V) that zeros out the subdiagonal entries of
//    column K of the matrix A.
//
//       H(V) = I - 2 * v * v'
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix A.
//
//    Input, double A[N], column K of the matrix A.
//
//    Input, int K, the column of the matrix to be modified.
//
//    Output, double R8VEC_HOUSE_COLUMN[N], a vector of unit L2 norm which
//    defines an orthogonal Householder premultiplier matrix H with the property
//    that the K-th column of H*A is zero below the diagonal.
//
{
  int i;
  double s;
  double *v;

  v = r8vec_zero_new ( n );

  if ( k < 1 || n <= k )
  {
    return v;
  }

  s = r8vec_norm_l2 ( n+1-k, a+k-1 );

  if ( s == 0.0 )
  {
    return v;
  }

  v[k-1] = a[k-1] + r8_abs ( s ) * r8_sign ( a[k-1] );

  r8vec_copy ( n-k, a+k, v+k );

  s = r8vec_norm_l2 ( n-k+1, v+k-1 );

  for ( i = k-1; i < n; i++ )
  {
    v[i] = v[i] / s;
  }

  return v;
}
//****************************************************************************80

bool r8vec_in_01 ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_IN_01 is TRUE if the entries of an R8VEC are in the range [0,1].
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector
//
//    Output, bool R8VEC_IN_01, is TRUE if every entry of A is
//    between 0 and 1.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] < 0.0 || 1.0 < a[i] )
    {
      return false;
    }
  }

  return true;
}
//****************************************************************************80

void r8vec_index_delete_all ( int n, double x[], int indx[], double xval, 
  int *n2, double x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_DELETE_ALL deletes all occurrences of a value from an indexed sorted list.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Note that the value of N is adjusted because of the deletions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    23 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, double X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, the value to be sought.
//
//    Output, int *N2, the size of the current list.
//
//    Output, double X2[N2], the list.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int equal;
  int equal1;
  int equal2;
  int get;
  int i;
  int less;
  int more;
  int put;

  if ( n < 1 )
  {
    *n2 = 0;
    return;
  }

  i4vec_copy ( n, indx, indx2 );
  r8vec_copy ( n, x, x2 );
  *n2 = n;

  r8vec_index_search ( *n2, x2, indx2, xval, &less, &equal, &more );

  if ( equal == 0 )
  {
    return;
  }

  equal1 = equal;

  for ( ; ; )
  {
    if ( equal1 <= 1 )
    {
      break;
    }

    if ( x2[indx2[equal1-2]-1] != xval )
    {
      break;
    }
    equal1 = equal1 - 1;
  }

  equal2 = equal;

  for ( ; ; )
  {
    if ( *n2 <= equal2 )
    {
      break;
    }

    if ( x2[indx2[equal2]-1] != xval )
    {
      break;
    }
    equal2 = equal2 + 1;
  }
//
//  Discard certain X values.
//
  put = 0;

  for ( get = 1; get <= *n2; get++ )
  {
    if ( x2[get-1] != xval )
    {
      put = put + 1;
      x2[put-1] = x2[get-1];
    }
  }
//
//  Adjust the INDX values.
//
  for ( equal = equal1; equal <= equal2; equal++ )
  {
    for ( i = 1; i <= *n2; i++ )
    {
      if ( indx2[equal-1] < indx2[i-1] )
      {
        indx2[i-1] = indx2[i-1] - 1;
      }
    }
  }
//
//  Discard certain INDX values.
//
  for ( i = 0; i <= *n2 - equal2 - 1; i++ )
  {
    indx2[equal1+i-1] = indx2[equal2+i];
  }
  for ( i = *n2 + equal1 - equal2; i <= *n2; i++ )
  {
    indx2[i-1] = 0;
  }
//
//  Adjust N.
//
  *n2 = put;

  return;
}
//****************************************************************************80

void r8vec_index_delete_dupes ( int n, double x[], int indx[], 
  int *n2, double x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_DELETE_DUPES deletes duplicates from an indexed sorted list.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The output quantities N2, X2, and INDX2 are computed from the
//    input quantities by sorting, and eliminating duplicates.
//
//    The output arrays should be dimensioned of size N, unless the user
//    knows in advance what the value of N2 will be.
//
//    The output arrays may be identified with the input arrays.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the input list.
//
//    Input, double X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Output, int *N2, the number of unique entries in X.
//
//    Output, double X2[N2], a copy of the list which has
//    been sorted, and made unique.
//
//    Output, int INDX2[N2], the sort index of the new list.
//
{
  int i;
  int n3;
  double *x3;

  i = 0;
  n3 = 0;
  x3 = new double[n];

  for ( ; ; )
  {
    i = i + 1;

    if ( n < i )
    {
      break;
    }

    if ( 1 < i )
    {
      if ( x[indx[i-1]-1] == x3[n3-1] )
      {
        continue;
      }
    }
    n3 = n3 + 1;
    x3[n3-1] = x[indx[i-1]-1];
  }
//
//  Set the output data.
//
  *n2 = n3;
  r8vec_copy ( n3, x3, x2 );
  for ( i = 0; i < n3; i++ )
  {
    indx2[i] = i + 1;
  }

  delete [] x3;

  return;
}
//****************************************************************************80

void r8vec_index_delete_one ( int n, double x[], int indx[], double xval, 
  int *n2, double x2[], int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_DELETE_ONE deletes one copy of a value from an indexed sorted list.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If the value occurs in the list more than once, only one copy is deleted.
//
//    Note that the value of N is adjusted because of the deletions.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 October 2000
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, double X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, the value to be sought.
//
//    Output, int *N2, the size of the current list.
//
//    Output, double X2[N2], the list.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int equal;
  int i;
  int j;
  int less;
  int more;

  if ( n < 1 )
  {
    *n2 = 0;
    return;
  }

  *n2 = n;
  i4vec_copy ( *n2, indx, indx2 );
  r8vec_copy ( *n2, x, x2 );

  r8vec_index_search ( *n2, x2, indx2, xval, &less, &equal, &more );

  if ( equal != 0 )
  {
    j = indx2[equal-1];
    for ( i = j; i <= *n2-1; i++ )
    {
      x2[i-1] = x[i];
    }
    for ( i = equal; i <= *n2-1; i++ )
    {
      indx2[i-1] = indx2[i];
    }
    for ( i = 1; i <= *n2 - 1; i++ )
    {
      if ( j < indx2[i-1] )
      {
        indx2[i-1] = indx2[i-1] - 1;
      }
    }
    *n2 = *n2 - 1;
  }

  return;
}
//****************************************************************************80

void r8vec_index_insert ( int *n, double x[], int indx[], double xval )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_INSERT inserts a value in an indexed sorted list.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *N, the size of the current list.
//
//    Input, double X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, the value to be sought.
//
{
  int equal;
  int i;
  int less;
  int more;

  if ( *n <= 0 )
  {
    *n = 1;
    x[0] = xval;
    indx[0] = 1;
    return;
  }

  r8vec_index_search ( *n, x, indx, xval, &less, &equal, &more );

  x[*n] = xval;
  for ( i = *n; more <= i; i-- )
  {
    indx[i] = indx[i-1];
  }
  indx[more-1] = *n + 1;
  *n = *n + 1;

  return;
}
//****************************************************************************80

void r8vec_index_insert_unique ( int *n, double x[], int indx[], double xval )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_INSERT_UNIQUE inserts a unique value in an indexed sorted list.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input/output, int *N, the size of the current list.
//    If the input value XVAL does not already occur in X, then N is increased.
//
//    Input/output, double X[N], the list.
//    If the input value XVAL does not already occur in X, then it is added
//    to X.
//
//    Input/output, int INDX[N], the sort index of the list.
//    If the input value XVAL does not already occur in X, then INDX is updated.
//
//    Input, double XVAL, the value which will be inserted into the X
//    vector if it is not there already.
//
{
  int equal;
  int i;
  int less;
  int more;

  if ( *n <= 0 )
  {
    *n = 1;
    x[0] = xval;
    indx[0] = 1;
    return;
  }
//
//  Does XVAL already occur in X?
//
  r8vec_index_search ( *n, x, indx, xval, &less, &equal, &more );

  if ( equal == 0 )
  {
    x[*n] = xval;
    for ( i = *n; more <= i; i-- )
    {
      indx[i] = indx[i-1];
    }
    indx[more-1] = *n + 1;
    *n = *n + 1;
  }

  return;
}
//****************************************************************************80

void r8vec_index_order ( int n, double x[], int indx[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_ORDER sorts an R8VEC using an index vector.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The index vector itself is not modified.  Therefore, the pair
//    (X,INDX) no longer represents an index sorted vector.  
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input/output, double X[N], the list.  On output, the list
//    has been sorted.
//
//    Input, int INDX[N], the sort index of the list.
//
{
  int i;
  double *y;

  y = new double[n];

  for ( i = 0; i < n; i++ )
  {
    y[i] = x[indx[i]-1];
  }
  for ( i = 0; i < n; i++ )
  {
    x[i] = y[i];
  }
  delete [] y;

  return;
}
//****************************************************************************80

void r8vec_index_search ( int n, double x[], int indx[], double xval, int *less, 
  int *equal, int *more )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_SEARCH searches for a value in an indexed sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, double X[N], the list.
//
//    Input, int INDX[N], the sort index of the list.
//
//    Input, double XVAL, the value to be sought.
//
//    Output, int *LESS, *EQUAL, *MORE, the indexes in INDX of the
//    entries of X that are just less than, equal to, and just greater
//    than XVAL.  If XVAL does not occur in X, then EQUAL is zero.
//    If XVAL is the minimum entry of X, then LESS is 0.  If XVAL
//    is the greatest entry of X, then MORE is N+1.
//
{
  int hi;
  int lo;
  int mid;
  double xhi;
  double xlo;
  double xmid;

  if ( n <= 0 )
  {
    *less = 0;
    *equal = 0;
    *more = 0;
    return;
  }

  lo = 1;
  hi = n;
  xlo = x[indx[lo-1]-1];
  xhi = x[indx[hi-1]-1];

  if ( xval < xlo )
  {
    *less = 0;
    *equal = 0;
    *more = 1;
    return;
  }
  else if ( xval == xlo )
  {
    *less = 0;
    *equal = 1;
    *more = 2;
    return;
  }

  if ( xhi < xval )
  {
    *less = n;
    *equal = 0;
    *more = n + 1;
    return;
  }
  else if ( xval == xhi )
  {
    *less = n - 1;
    *equal = n;
    *more = n + 1;
    return;
  }

  for ( ; ; )
  {
    if ( lo + 1 == hi )
    {
      *less = lo;
      *equal = 0;
      *more = hi;
      return;
    }

    mid = ( lo + hi ) / 2;
    xmid = x[indx[mid-1]-1];

    if ( xval == xmid )
    {
      *equal = mid;
      *less = mid - 1;
      *more = mid + 1;
      return;
    }
    else if ( xval < xmid )
    {
      hi = mid;
    }
    else if ( xmid < xval )
    {
      lo = mid;
    }
  }
  return;
}
//****************************************************************************80

void r8vec_index_sort_unique ( int n, double x[], int *n2, double x2[], 
  int indx2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDEX_SORT_UNIQUE creates a sort index for an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the current list.
//
//    Input, double X[N], the list.  
//
//    Output, int *N2, the number of unique elements in X.
//
//    Output, double X2[N2], a list of the unique elements of X.
//
//    Output, int INDX2[N2], the sort index of the list.
//
{
  int i;

  *n2 = 0;

  for ( i = 0; i < n; i++ )
  {
    r8vec_index_insert_unique ( n2, x2, indx2, x[i] );
  }

  for ( i = *n2; i < n; i++ )
  {
    x2[i] = -1;
  }
  for ( i = *n2; i < n; i++ )
  {
    indx2[i] = -1;
  }

  return;
}
//****************************************************************************80

double *r8vec_indicator ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INDICATOR sets an R8VEC to the indicator vector {1,2,3...}.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Output, double R8VEC_INDICATOR[N], the array to be initialized.
//
{
  double *a;
  int i;

  a = new double[n];

  for ( i = 0; i <= n-1; i++ ) 
  {
    a[i] = ( double ) ( i + 1 );
  }

  return a;
}
//****************************************************************************80

void r8vec_insert ( int n, double a[], int pos, double value )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_INSERT inserts a value into an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the array on input.
//
//    Input/output, double A[N+1], the array.  On input, A is 
//    assumed to contain only N entries, while on output, A actually 
//    contains N+1 entries.
//
//    Input, int POS, the position to be assigned the new entry.
//    1 <= POS <= N+1.
//
//    Input, double VALUE, the value to be inserted.
//
{
  int i;

  if ( pos < 1 || n + 1 < pos )
  {
    cout << "\n";
    cout << "R8VEC_INSERT - Fatal error!\n";
    cout << "  Illegal insertion position = " << pos << "\n";;
    exit ( 1 );
  }
  else
  {
    for ( i = n+1; pos+1 <= i; i-- )
    {
      a[i-1] = a[i-2];
    }

    a[pos-1] = value;
  }

  return;
}
//****************************************************************************80

bool r8vec_is_int ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_IS_INT is TRUE if an R8VEC is integral.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector
//
//    Output, bool R8VEC_IS_INT, is TRUE if every entry of A is an integer.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] != ( double ) ( int ) a[i] )
    {
      return false;
    }
  }

  return true;
}
//****************************************************************************80

double r8vec_length ( int dim_num, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_LENGTH returns the Euclidean length of an R8VEC
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int DIM_NUM, the spatial dimension.
//
//    Input, double X[DIM_NUM], the vector.
//
//    Output, double R8VEC_LENGTH, the Euclidean length of the vector.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < dim_num; i++ )
  {
    value = value + pow ( x[i], 2 );
  }
  value = sqrt ( value );

  return value;
}
//****************************************************************************80

bool r8vec_lt ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_LT == ( A1 < A2 ) for two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The comparison is lexicographic.
//
//    A1 < A2  <=>                              A1(1) < A2(1) or
//                 ( A1(1)     == A2(1)     and A1(2) < A2(2) ) or
//                 ...
//                 ( A1(1:N-1) == A2(1:N-1) and A1(N) < A2(N)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the dimension of the vectors.
//
//    Input, double A1[N], A2[N], the vectors to be compared.
//
//    Output, bool R8VEC_LT, is TRUE if and only if A1 < A2.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    if ( a1[i] < a2[i] )
    {
      return true;
    }
    else if ( a2[i] < a1[i] )
    {
      return false;
    }

  }

  return false;
}
//****************************************************************************80

void r8vec_mask_print ( int n, double a[], int mask_num, int mask[], 
  char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MASK_PRINT prints a masked R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A[N], the vector to be printed.
//
//    Input, int MASK_NUM, the number of masked elements.
//
//    Input, int MASK[MASK_NUM], the indices of the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  cout << "\n";
  cout << "  Masked vector printout:\n";

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i < mask_num; i++ )
  {
    cout << "  " << setw(6)  << i+1
         << "  " << setw(6)  << mask[i]
         << "  " << setw(12) << a[mask[i]-1] << "\n";
  }

  return;
}
//****************************************************************************80

double r8vec_max ( int n, double r8vec[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MAX returns the value of the maximum element in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double R8VEC[N], a pointer to the first entry of the array.
//
//    Output, double R8VEC_MAX, the value of the maximum element.  This
//    is set to 0.0 if N <= 0.
//
{
  int i;
  double *r8vec_pointer;
  double value;

  value = - r8_huge ( );

  if ( n <= 0 ) 
  {
    return value;
  }

  for ( i = 0; i < n; i++ ) 
  {
    if ( value < r8vec[i] )
    {
      value = r8vec[i];
    }
  }
  return value;
}
//****************************************************************************80

int r8vec_max_index ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MAX_INDEX returns the index of the maximum value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, int R8VEC_MAX_INDEX, the index of the largest entry.
//
{
  int i;
  int max_index;

  if ( n <= 0 )
  {
    max_index = -1;
  }
  else
  {
    max_index = 0;

    for ( i = 1; i < n; i++ )
    {
      if ( a[max_index] < a[i] )
      {
        max_index = i;
      }
    }
  }

  return max_index;
}
//****************************************************************************80

double r8vec_mean ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MEAN returns the mean of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double X[N], the vector whose mean is desired.
//
//    Output, double R8VEC_MEAN, the mean, or average, of the vector entries.
//
{
  int i;
  double mean;

  mean = 0.0;
  for ( i = 0; i < n; i++ ) 
  {
    mean = mean + x[i];
  }

  mean = mean / ( double ) n;

  return mean;
}
//****************************************************************************80

double r8vec_median ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MEDIAN returns the median of an unsorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Hoare's algorithm is used.  The values of the vector are
//    rearranged by this routine.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 September 2005
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, double A[N], the array to search.  On output,
//    the order of the elements of A has been somewhat changed.
//
//    Output, double R8VEC_MEDIAN, the value of the median of A.
//
{
  int k;
  double median;

  k = ( n + 1 ) / 2;

  median = r8vec_frac ( n, a, k );

  return median;
}
//****************************************************************************80

double r8vec_min ( int n, double r8vec[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MIN returns the value of the minimum element in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double R8VEC[N], the array to be checked.
//
//    Output, double R8VEC_MIN, the value of the minimum element.
//
{
  int i;
  double *r8vec_pointer;
  double value;

  value = r8_huge ( );

  if ( n <= 0 ) 
  {
    return value;
  }

  for ( i = 0; i < n; i++ ) 
  {
    if ( r8vec[i] < value )
    {
      value = r8vec[i];
    }
  }
  return value;
}
//****************************************************************************80

int r8vec_min_index ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MIN_INDEX returns the index of the minimum value in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Output, int R8VEC_MIN_INDEX, the index of the smallest entry.
//
{
  int i;
  int min_index;

  if ( n <= 0 )
  {
    min_index = -1;
  }
  else
  {
    min_index = 0;

    for ( i = 1; i < n; i++ )
    {
      if ( a[i] < a[min_index] )
      {
        min_index = i;
      }
    }
  }

  return min_index;
}
//****************************************************************************80

bool r8vec_mirror_next ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_MIRROR_NEXT steps through all sign variations of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    In normal use, the user would set every element of A to be positive.
//    The routine will take the input value of A, and output a copy in
//    which the signs of one or more entries have been changed.  Repeatedly
//    calling the routine with the output from the previous call will generate
//    every distinct "variation" of A; that is, all possible sign variations.
//
//    When the output variable DONE is TRUE (or equal to 1), then the
//    output value of A_NEW is the last in the series.
//
//    Note that A may have some zero values.  The routine will essentially
//    ignore such entries; more exactly, it will not stupidly assume that -0
//    is a proper "variation" of 0.
//
//    Also, it is possible to call this routine with the signs of A set
//    in any way you like.  The routine will operate properly, but it
//    will nonethess terminate when it reaches the value of A in which
//    every nonzero entry has negative sign.
//
//
//    More efficient algorithms using the Gray code seem to require internal
//    memory in the routine, which is not one of MATLAB's strong points,
//    or the passing back and forth of a "memory array", or the use of
//    global variables, or unnatural demands on the user.  This form of
//    the routine is about as clean as I can make it.
//
//  Example:
//
//      Input         Output
//    ---------    --------------
//    A            A         DONE
//    ---------    --------  ----
//     1  2  3     -1  2  3  false
//    -1  2  3      1 -2  3  false
//     1 -2  3     -1 -2  3  false
//    -1 -2  3      1  2 -3  false
//     1  2 -3     -1  2 -3  false
//    -1  2 -3      1 -2 -3  false
//     1 -2 -3     -1 -2 -3  false
//    -1 -2 -3      1  2  3  true
//
//     1  0  3     -1  0  3  false
//    -1  0  3      1  0 -3  false
//     1  0 -3     -1  0 -3  false
//    -1  0 -3      1  0  3  true
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    12 March 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, double A[N], a vector of real numbers.  On 
//    output, some signs have been changed.
//
//    Output, bool R8VEC_MIRROR_NEXT, is TRUE if the input vector A was 
//    the last element
//    in the series (every entry was nonpositive); the output vector is reset
//    so that all entries are nonnegative, but presumably the ride is over.
//
{
  bool done;
  int i;
  int positive;
//
//  Seek the first strictly positive entry of A.
//
  positive = -1;
  for ( i = 0; i < n; i++ )
  {
    if ( 0.0 < a[i] )
    {
      positive = i;
      break;
    }
  }
//
//  If there is no strictly positive entry of A, there is no successor.
//
  if ( positive == -1 )
  {
    for ( i = 0; i < n; i++ )
    {
      a[i] = - a[i];
    }
    done = true;
    return done;
  }
//
//  Otherwise, negate A up to the positive entry.
//
  for ( i = 0; i <= positive; i++ )
  {
    a[i] = - a[i];
  }
  done = false;

  return done;
}
//****************************************************************************80

double *r8vec_nint ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NINT rounds the entries of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    06 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector to be rounded.
//
//    Output, double B[N], the rounded values.
//
{
  double *b;
  int i;
  int s;

  b = new double[n];

  for ( i = 0; i < n; i++ )
  {
    if ( a[i] < 0.0 )
    {
      s = -1;
    }
    else
    {
      s = 1;
    }
    b[i] = ( double ) ( s * ( int ) ( r8_abs ( a[i] ) + 0.5 ) );
  }

  return b;
}
//****************************************************************************80

double r8vec_norm_l1 ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NORM_L1 returns the L1 norm of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The vector L1 norm is defined as:
//
//      R8VEC_NORM_L1 = sum ( 1 <= I <= N ) abs ( A(I) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector whose L1 norm is desired.
//
//    Output, double R8VEC_NORM_L1, the L1 norm of A.
//
{
  int i;
  double v;

  v = 0.0;

  for ( i = 0; i < n; i++ )
  {
    v = v + r8_abs ( a[i] );
  }

  return v;
}
//****************************************************************************80

double r8vec_norm_l2 ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NORM_L2 returns the L2 norm of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The vector L2 norm is defined as:
//
//      R8VEC_NORM_L2 = sqrt ( sum ( 1 <= I <= N ) A(I)**2 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector whose L2 norm is desired.
//
//    Output, double R8VEC_NORM_L2, the L2 norm of A.
//
{
  int i;
  double v;

  v = 0.0;

  for ( i = 0; i < n; i++ )
  {
    v = v + a[i] * a[i];
  }
  v = sqrt ( v );

  return v;
}
//****************************************************************************80

double r8vec_norm_li ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NORM_LI returns the L-infinity norm of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The vector L-infinity norm is defined as:
//
//      R8VEC_NORM_LI = max ( 1 <= I <= N ) abs ( A(I) ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector whose L-infinity norm is desired.
//
//    Output, double R8VEC_NORM_LI, the L-infinity norm of A.
//
{
  int i;
  double v1;
  double v2;

  v1 = 0.0;

  for ( i = 0; i < n; i++ )
  {
    v2 = r8_abs ( a[i] );

    if ( v1 < v2 )
    {
      v1 = v2;
    }
  }

  return v1;
}
//****************************************************************************80

double r8vec_norm_lp ( int n, double a[], double p )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NORM_LP returns the LP norm of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The vector LP norm is defined as:
//
//      R8VEC_NORM_LP = ( sum ( 1 <= I <= N ) ( abs ( A(I) ) )**P )**(1/P).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], the vector whose LP norm is desired.
//
//    Input, double P, the index of the norm.  
//
//    Output, double R8VEC_NORML_LP, the LP norm of A.
//
{
  int i;
  double v;

  v = 0.0;

  if ( p == 1.0 )
  {
    for ( i = 0; i < n; i++ )
    {
      v = v + r8_abs ( a[i] );
    }
  }
  else if ( p == 2.0 )
  {
    for ( i = 0; i < n; i++ )
    {
      v = v + a[i] * a[i];
    }
    v = sqrt ( v );
  }
  else
  {
    for ( i = 0; i < n; i++ )
    {
      v = v + pow ( r8_abs ( a[i] ), p );
    }
    v = pow (  ( double ) v, 1.0 / p );
  }

  return v;
}
//****************************************************************************80

double *r8vec_normal_01 ( int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_NORMAL_01 returns a unit pseudonormal R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The standard normal probability distribution function (PDF) has
//    mean 0 and standard deviation 1.
//
//    This routine can generate a vector of values on one call.  It
//    has the feature that it should provide the same results
//    in the same order no matter how we break up the task.
//
//    Before calling this routine, the user may call RANDOM_SEED
//    in order to set the seed of the random number generator.
//
//    The Box-Muller method is used, which is efficient, but
//    generates an even number of values each time.  On any call
//    to this routine, an even number of new values are generated.
//    Depending on the situation, one value may be left over.
//    In that case, it is saved for the next call.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 February 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of values desired.  If N is negative,
//    then the code will flush its internal memory; in particular,
//    if there is a saved value to be used on the next call, it is
//    instead discarded.  This is useful if the user has reset the
//    random number seed, for instance.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double X(N), a sample of the standard normal PDF.
//
//  Local parameters:
//
//    Local, int MADE, records the number of values that have
//    been computed.  On input with negative N, this value overwrites
//    the return value of N, so the user can get an accounting of
//    how much work has been done.
//
//    Local, double R[N+1], is used to store some uniform random values.
//    Its dimension is N+1, but really it is only needed to be the
//    smallest even number greater than or equal to N.
//
//    Local, int SAVED, is 0 or 1 depending on whether there is a
//    single saved value left over from the previous call.
//
//    Local, int X_LO, X_HI, records the range of entries of
//    X that we need to compute.  This starts off as 1:N, but is adjusted
//    if we have a saved value that can be immediately stored in X(1),
//    and so on.
//
//    Local, double Y, the value saved from the previous call, if
//    SAVED is 1.
//
{
  int i;
  int m;
  static int made = 0;
  double pi = 3.141592653589793;
  double *r;
  static int saved = 0;
  double *x;
  int x_hi;
  int x_lo;
  static double y = 0.0;
//
//  I'd like to allow the user to reset the internal data.
//  But this won't work properly if we have a saved value Y.
//  I'm making a crock option that allows the user to signal
//  explicitly that any internal memory should be flushed,
//  by passing in a negative value for N.
//
  if ( n < 0 )
  {
    made = 0;
    saved = 0;
    y = 0.0;
    return NULL;
  }
  else if ( n == 0 )
  {
    return NULL;
  }

  x = new double[n];
//
//  Record the range of X we need to fill in.
//
  x_lo = 1;
  x_hi = n;
//
//  Use up the old value, if we have it.
//
  if ( saved == 1 )
  {
    x[0] = y;
    saved = 0;
    x_lo = 2;
  }
//
//  Maybe we don't need any more values.
//
  if ( x_hi - x_lo + 1 == 0 )
  {
  }
//
//  If we need just one new value, do that here to avoid null arrays.
//
  else if ( x_hi - x_lo + 1 == 1 )
  {
    r = r8vec_uniform_01 ( 2, seed );

    x[x_hi-1] = sqrt ( -2.0 * log ( r[0] ) ) * cos ( 2.0 * pi * r[1] );
    y =         sqrt ( -2.0 * log ( r[0] ) ) * sin ( 2.0 * pi * r[1] );

    saved = 1;

    made = made + 2;

    delete [] r;
  }
//
//  If we require an even number of values, that's easy.
//
  else if ( ( x_hi - x_lo + 1 ) % 2 == 0 )
  {
    m = ( x_hi - x_lo + 1 ) / 2;

    r = r8vec_uniform_01 ( 2*m, seed );

    for ( i = 0; i <= 2*m-2; i = i + 2 )
    {
      x[x_lo+i-1] = sqrt ( -2.0 * log ( r[i] ) ) * cos ( 2.0 * pi * r[i+1] );
      x[x_lo+i  ] = sqrt ( -2.0 * log ( r[i] ) ) * sin ( 2.0 * pi * r[i+1] );
    }
    made = made + x_hi - x_lo + 1;

    delete [] r;
  }
//
//  If we require an odd number of values, we generate an even number,
//  and handle the last pair specially, storing one in X(N), and
//  saving the other for later.
//
  else
  {
    x_hi = x_hi - 1;

    m = ( x_hi - x_lo + 1 ) / 2 + 1;

    r = r8vec_uniform_01 ( 2*m, seed );

    for ( i = 0; i <= 2*m-4; i = i + 2 )
    {
      x[x_lo+i-1] = sqrt ( -2.0 * log ( r[i] ) ) * cos ( 2.0 * pi * r[i+1] );
      x[x_lo+i  ] = sqrt ( -2.0 * log ( r[i] ) ) * sin ( 2.0 * pi * r[i+1] );
    }

    i = 2*m - 2;

    x[x_lo+i-1] = sqrt ( -2.0 * log ( r[i] ) ) * cos ( 2.0 * pi * r[i+1] );
    y           = sqrt ( -2.0 * log ( r[i] ) ) * sin ( 2.0 * pi * r[i+1] );

    saved = 1;

    made = made + x_hi - x_lo + 2;

    delete [] r;
  }

  return x;
}
//****************************************************************************80

int r8vec_order_type ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_ORDER_TYPE determines if an R8VEC is (non)strictly ascending/descending.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 September 2000
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the array.
//
//    Input, double X[N], the array to be checked.
//
//    Output, int R8VEC_ORDER_TYPE, order indicator:
//    -1, no discernable order;
//    0, all entries are equal;
//    1, ascending order;
//    2, strictly ascending order;
//    3, descending order;
//    4, strictly descending order.
//
{
  int i;
  int order;
// 
//  Search for the first value not equal to X(0). 
//
  i = 0;

  for ( ; ; ) 
  {
    i = i + 1;
    if ( n-1 < i ) 
    {
      order = 0;
      return order;
    }

    if ( x[0] < x[i] ) 
    {
      if ( i == 1 ) 
      {
        order = 2;
        break;
      }
      else
      {
        order = 1;
        break;
      }
    }
    else if ( x[i] < x[0] ) 
    {
      if ( i == 1 ) 
      {
        order = 4;
        break;
      }
      else
      {
        order = 3;
        break;
      }
    }
  }
// 
//  Now we have a "direction".  Examine subsequent entries. 
//
  for ( ; ; ) 
  {
    i = i + 1;
    if ( n - 1 < i ) 
    {
      break;
    }

    if ( order == 1 )
    {
      if ( x[i] < x[i-1] )
      {
        order = -1;
        break;
      }
    }
    else if ( order == 2 ) 
    {
      if ( x[i] < x[i-1] )
      {
        order = -1;
        break;
      }
      else if ( x[i] == x[i-1] ) 
      {
        order = 1;
      }
    }
    else if ( order == 3 ) 
    {
      if ( x[i-1] < x[i] ) 
      {
        order = -1;
        break;
      }
    }
    else if ( order == 4 ) 
    {
      if ( x[i-1] < x[i] ) 
      {
        order = -1;
        break;
      }
      else if ( x[i] == x[i-1] ) 
      {
        order = 3;
      }
    }
  }
  return order;
}
//****************************************************************************80

void r8vec_part_quick_a ( int n, double a[], int *l, int *r )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PART_QUICK_A reorders an R8VEC as part of a quick sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The routine reorders the entries of A.  Using A[0] as a
//    key, all entries of A that are less than or equal to A[0] will
//    precede A[0] which precedes all entries that are greater than A[0].
//
//  Example:
//
//    Input:
//
//  N = 8
//
//  A = ( 6, 7, 3, 1, 6, 8, 2, 9 )
//
//    Output:
//
//  L = 3, R = 6
//
//  A = ( 3, 1, 2, 6, 6, 8, 9, 7 )
//        -------        -------
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, double A[N].  On input, the array to be checked.
//    On output, A has been reordered as described above.
//
//    Output, int L, R, the indices of A that define the three segments.
//    Let KEY = the input value of A[0].  Then
//    I <= L             A(I) < KEY;
//     L < I < R         A(I) = KEY;
//             R <= I    A(I) > KEY.
//
{
  int i;
  double key;
  int m;
  double temp;

  if ( n < 1 ) 
  {
    cout << "\n";
    cout << "R8VEC_PART_QUICK_A - Fatal error!\n";
    cout << "  N < 1.\n";
    exit ( 1 );
  }
  else if ( n == 1 ) 
  {
    *l = 0;
    *r = 2;
    return;
  }

  key = a[0];
  m = 1;
//
//  The elements of unknown size have indices between L+1 and R-1.
//
  *l = 1;
  *r = n + 1;

  for ( i = 2; i <= n; i++ ) 
  {

    if ( key < a[*l] ) 
    {
      *r = *r - 1;
      temp = a[*r-1];
      a[*r-1] = a[*l];
      a[*l] = temp;
    }
    else if ( a[*l] == key ) 
    {
      m = m + 1;
      temp = a[m-1];
      a[m-1] = a[*l];
      a[*l] = temp;
      *l = *l + 1;
    }
    else if ( a[*l] < key ) 
    {
      *l = *l + 1;
    }

  }
//
//  Now shift small elements to the left, and KEY elements to center.
//
  for ( i = 1; i <= *l -m; i++ )
  {
    a[i-1] = a[i+m-1];
  }

  *l = *l - m;

  for ( i = *l+1; i <= *l+m; i++ ) 
  {
    a[i-1] = key;
  }

  return;
}
//****************************************************************************80

void r8vec_permute ( int n, int p[], int base, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PERMUTE permutes an R8VEC in place.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    This routine permutes an array of real "objects", but the same
//    logic can be used to permute an array of objects of any arithmetic
//    type, or an array of objects of any complexity.  The only temporary
//    storage required is enough to store a single object.  The number
//    of data movements made is N + the number of cycles of order 2 or more,
//    which is never more than N + N/2.
//
//  Example:
//
//    Input:
//
//      N = 5
//      P = (   2,   4,   5,   1,   3 )
//      A = ( 1.0, 2.0, 3.0, 4.0, 5.0 )
//      BASE = 1
//
//    Output:
//
//      A    = ( 2.0, 4.0, 5.0, 1.0, 3.0 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int P[N], the permutation.  
//
//    Input, int BASE, is 0 for a 0-based permutation and 1 for a 1-based permutation.
//
//    Input/output, double A[N], the array to be permuted.
//
{
  double a_temp;
  int i;
  int iget;
  int iput;
  int istart;

  if ( !perm_check ( n, p, base ) )
  {
    cerr << "\n";
    cerr << "R8VEC_PERMUTE - Fatal error!\n";
    cerr << "  PERM_CHECK rejects this permutation.\n";
    exit ( 1 );
  }
//
//  In order for the sign negation trick to work, we need to assume that the
//  entries of P are strictly positive.  Presumably, the lowest number is BASE.
//  So temporarily add 1-BASE to each entry to force positivity.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] + 1 - base;
  }
//
//  Search for the next element of the permutation that has not been used.
//
  for ( istart = 1; istart <= n; istart++ )
  {
    if ( p[istart-1] < 0 )
    {
      continue;
    }
    else if ( p[istart-1] == istart )
    {
      p[istart-1] = - p[istart-1];
      continue;
    }
    else
    {
      a_temp = a[istart-1];
      iget = istart;
//
//  Copy the new value into the vacated entry.
//
      for ( ; ; )
      {
        iput = iget;
        iget = p[iget-1];

        p[iput-1] = - p[iput-1];

        if ( iget < 1 || n < iget )
        {
          cout << "\n";
          cout << "R8VEC_PERMUTE - Fatal error!\n";
          cout << "  A permutation index is out of range.\n";
          cout << "  P(" << iput << ") = " << iget << "\n";
          exit ( 1 );
        }

        if ( iget == istart )
        {
          a[iput-1] = a_temp;
          break;
        }
        a[iput-1] = a[iget-1];
      }
    }
  }
//
//  Restore the signs of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = - p[i];
  }
//
//  Restore the base of the entries.
//
  for ( i = 0; i < n; i++ )
  {
    p[i] = p[i] - 1 +  base;
  }
  return;
}
//****************************************************************************80

void r8vec_permute_uniform ( int n, double a[], int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PERMUTE_UNIFORM randomly permutes an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input/output, double A[N], the array to be permuted.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
{
  int base = 0;
  int *p;

  p = perm_uniform ( n, base, seed );

  r8vec_permute ( n, p, base, a );

  delete [] p;

  return;
}
//****************************************************************************80

void r8vec_polarize ( int n, double a[], double p[], double a_normal[], 
  double a_parallel[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_POLARIZE decomposes an R8VEC into normal and parallel components.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The (nonzero) vector P defines a direction.
//
//    The vector A can be written as the sum
//
//      A = A_normal + A_parallel
//
//    where A_parallel is a linear multiple of P, and A_normal
//    is perpendicular to P.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the vector to be polarized.
//
//    Input, double P[N], the polarizing direction.
//
//    Output, double A_NORMAL[N], A_PARALLEL[N], the normal
//    and parallel components of A.
//
{
  double a_dot_p;
  int i;
  double p_norm;

  p_norm = 0.0;
  for ( i = 0; i < n; i++ )
  {
    p_norm = p_norm + pow ( p[i], 2 );
  }
  p_norm = sqrt ( p_norm );

  if ( p_norm == 0.0 )
  {
    for ( i = 0; i < n; i++ )
    {
      a_normal[i] = a[i];
    }
    for ( i = 0; i < n; i++ )
    {
      a_parallel[i] = 0.0;
    }
    return;
  }
  a_dot_p = 0.0;
  for ( i = 0; i < n; i++ )
  {
    a_dot_p = a_dot_p + a[i] * p[i];
  }
  a_dot_p = a_dot_p / p_norm;

  for ( i = 0; i < n; i++ )
  {
    a_parallel[i] = a_dot_p * p[i] / p_norm;
  }

  for ( i = 0; i < n; i++ )
  {
    a_normal[i] = a[i] - a_parallel[i];
  }

  return;
}
//****************************************************************************80

void r8vec_print ( int n, double a[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PRINT prints an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A[N], the vector to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i < n; i++ ) 
  {
    cout << setw(6)  << i << "  " 
         << setw(14) << a[i]  << "\n";
  }

  return;
}
//****************************************************************************80

void r8vec_print_some ( int n, double a[], int i_lo, int i_hi, char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PRINT_SOME prints "some" of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the vector.
//
//    Input, double A[N], the vector to be printed.
//
//    Input, integer I_LO, I_HI, the first and last indices to print.
//    The routine expects 1 <= I_LO <= I_HI <= N.
//
//    Input, char *TITLE, an optional title.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = i4_max ( 1, i_lo ); i <= i4_min ( n, i_hi ); i++ )
  {
    cout << "  " << setw(8)  << i       << "  "
         << "  " << setw(14) << a[i-1]  << "\n";
  }

  return;
}
//****************************************************************************80

double r8vec_product ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_PRODUCT returns the product of the entries of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A[N], the vector.
//
//    Output, double R8VEC_PRODUCT, the product of the vector.
//
{
  int i;
  double product;

  product = 1.0;
  for ( i = 0; i < n; i++ )
  {
    product = product * a[i];
  }

  return product;
}
//****************************************************************************80

void r8vec_range ( int n, double x[], double xmin, double xmax, double y[], 
  double *ymin, double *ymax )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_RANGE finds the range of Y's within a restricted X range.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The routine is given a set of pairs of points (X,Y), and a range
//    XMIN to XMAX of valid X values.  Over this range, it seeks
//    YMIN and YMAX, the minimum and maximum values of Y for
//    valid X's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double X[N], the X array.
//
//    Input, double XMIN, XMAX, the range of X values to check.
//
//    Input, double Y[N], the Y array.
//
//    Output, double *YMIN, *YMAX, the range of Y values whose
//    X value is within the X range.
//
{
  int i;

  *ymin =   r8_huge ( );
  *ymax = - r8_huge ( );

  for ( i = 0; i < n; i++ )
  {
    if ( xmin <= x[i] && x[i] <= xmax )
    {
      *ymin = r8_min ( *ymin, y[i] );
      *ymax = r8_max ( *ymax, y[i] );
    }
  }

  return;
}
//****************************************************************************80

void r8vec_range_2 ( int n, double a[], double *amin, double *amax )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_RANGE_2 updates a range to include a new R8VEC
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Given a range AMIN to AMAX, and an array A, the routine will
//    decrease AMIN if necessary, or increase AMAX if necessary, so that
//    every entry of A is between AMIN and AMAX.
//
//    However, AMIN will not be increased, nor AMAX decreased.
//
//    This routine may be used to compute the maximum and minimum of a
//    collection of arrays one at a time.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], the array.
//
//    Input/output, double *AMIN, *AMAX.  On input, the
//    current legal range of values for A.  On output, AMIN and AMAX
//    are either unchanged, or else "widened" so that all entries
//    of A are within the range.
//
{
  *amax = r8_max ( *amax, r8vec_max ( n, a ) );
  *amin = r8_min ( *amin, r8vec_min ( n, a ) );

  return;
}
//****************************************************************************80

double *r8vec_read ( char *input_file, int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_READ reads an R8VEC from a file.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 August 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *INPUT_FILE, the name of the file to be read.
//
//    Input, int N, the size of the R8VEC.
//
//    Output, double R(N), the R8VEC.
//
{
  ifstream input;
  int k;
  double *r;

  input.open ( input_file );

  if ( !input )
  {
    cout << "\n";
    cout << "R8VEC_READ - Fatal error!\n";
    cout << "  Could not open the input file: \"" << input_file << "\"\n";
    return NULL;
  }

  r = new double[n];

  for ( k = 0; k < n; k++ )
  {
    input >> r[k];
  }

  input.close ( );

  return r;
}
//****************************************************************************80

int r8vec_read_size ( char *input_file )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_READ_SIZE reads the size of an R8VEC from a file.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 August 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *INPUT_FILE, the name of the file to 
//    be read.
//
//    Output, int R8VEC_READ_SIZE, the size of the R8VEC.
//
{
  ifstream input;
  char line[100];
  int n;

  n = 0;

  input.open ( input_file );

  if ( !input )
  {
    cout << "\n";
    cout << "R8VEC_READ_SIZE - Fatal error!\n";
    cout << "  Could not open the input file: \"" << input_file << "\"\n";
    return n;
  }

  for ( ; ; )
  {
    input.getline ( line, sizeof ( line ) );

    if ( input.eof ( ) )
    {
      break;
    }

    n = n + 1;
  }

  input.close ( );

  return n;
}
//****************************************************************************80

void r8vec_reverse ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_REVERSE reverses the elements of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Example:
//
//    Input:
//
//      N = 5, A = ( 11.0, 12.0, 13.0, 14.0, 15.0 ).
//
//    Output:
//
//      A = ( 15.0, 14.0, 13.0, 12.0, 11.0 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N], the array to be reversed.
//
{
  int i;
  double temp;

  for ( i = 1; i <= n/2; i++ )
  {
    temp   = a[i-1];
    a[i-1] = a[n-i];
    a[n-i] = temp;
  }

  return;
}
//****************************************************************************80

void r8vec_rotate ( int n, double a[], int m )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_ROTATE "rotates" the entries of an R8VEC in place.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    This routine rotates an array of real "objects", but the same
//    logic can be used to permute an array of objects of any arithmetic
//    type, or an array of objects of any complexity.  The only temporary
//    storage required is enough to store a single object.  The number
//    of data movements made is N + the number of cycles of order 2 or more,
//    which is never more than N + N/2.
//
//  Example:
//
//    Input:
//
//      N = 5, M = 2
//      A    = ( 1.0, 2.0, 3.0, 4.0, 5.0 )
//
//    Output:
//
//      A    = ( 4.0, 5.0, 1.0, 2.0, 3.0 ).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of objects.
//
//    Input, int M, the number of positions to the right that
//    each element should be moved.  Elements that shift pass position
//    N "wrap around" to the beginning of the array.
//
//    Input/output, double A[N], the array to be rotated.
//
{
  int iget;
  int iput;
  int istart;
  int mcopy;
  int nset;
  double temp;
//
//  Force M to be positive, between 0 and N-1.
//
  mcopy = i4_modp ( m, n );

  if ( mcopy == 0 )
  {
    return;
  }

  istart = 0;
  nset = 0;

  for ( ; ; )
  {
    istart = istart + 1;

    if ( n < istart )
    {
      break;
    }

    temp = a[istart-1];
    iget = istart;
//
//  Copy the new value into the vacated entry.
//
    for ( ; ; )
    {
      iput = iget;

      iget = iget - mcopy;
      if ( iget < 1 )
      {
        iget = iget + n;
      }

      if ( iget == istart )
      {
        break;
      }

      a[iput-1] = a[iget-1];
      nset = nset + 1;
    }

    a[iput-1] = temp;
    nset = nset + 1;

    if ( n <= nset )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

int r8vec_search_binary_a ( int n, double a[], double aval )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SEARCH_BINARY_A searches an ascending sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Binary search is used.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    18 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.9,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 26.
//
//  Parameters:
//
//    Input, int N, the number of elements in the array.
//
//    Input, double A[N], the array to be searched.  The array must
//    be sorted in ascending order.
//
//    Input, double AVAL, the value to be searched for.
//
//    Output, int R8VEC_SEARCH_BINARY_A, the result of the search.
//    -1, AVAL does not occur in the array.
//    I, A(I) = AVAL.
//
{
  int high;
  int indx;
  int low;
  int mid;

  indx = -1;

  low = 1;
  high = n;

  while ( low <= high )
  {
    mid = ( low + high ) / 2;

    if ( a[mid-1] == aval )
    {
      indx = mid;
      break;
    }
    else if ( a[mid-1] < aval )
    {
      low = mid + 1;
    }
    else if ( aval < a[mid-1] )
    {
      high = mid - 1;
    }
  }

  return indx;
}
//****************************************************************************80

void r8vec_sort_bubble_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_BUBBLE_A ascending sorts an R8VEC using bubble sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input/output, double A[N].
//    On input, an unsorted array of doubles.
//    On output, A has been sorted.
//
{
  int i;
  int j;
  double temp;

  for ( i = 0; i < n-1; i++ ) 
  {
    for ( j = i+1; j < n; j++ ) 
    {
      if ( a[j] < a[i] ) 
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
  return;
}
//****************************************************************************80

void r8vec_sort_bubble_d ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_BUBBLE_D descending sorts an R8VEC using bubble sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, length of input array.
//
//    Input/output, double A[N].
//    On input, an unsorted array of doubles.
//    On output, A has been sorted.
//
{
  int i;
  int j;
  double temp;

  for ( i = 0; i < n-1; i++ ) 
  {
    for ( j = i+1; j < n; j++ ) 
    {
      if ( a[i] < a[j] ) 
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
  return;
}
//****************************************************************************80

void r8vec_sort_heap_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_A ascending sorts an R8VEC using heap sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N].
//    On input, the array to be sorted;
//    On output, the array has been sorted.
//
{
  int n1;
  double temp;

  if ( n <= 1 ) 
  {
    return;
  }
//
//  1: Put A into descending heap form.
//
  r8vec_heap_d ( n, a );
//
//  2: Sort A.
//
//  The largest object in the heap is in A[0].
//  Move it to position A[N-1].
//
  temp = a[0];
  a[0] = a[n-1];
  a[n-1] = temp;
//
//  Consider the diminished heap of size N1.
//
  for ( n1 = n-1; 2 <= n1; n1-- ) 
  {
//
//  Restore the heap structure of the initial N1 entries of A.
//
    r8vec_heap_d ( n1, a );
//
//  Take the largest object from A[0] and move it to A[N1-1].
//
    temp = a[0];
    a[0] = a[n1-1];
    a[n1-1] = temp;
  }

  return;
}
//****************************************************************************80

void r8vec_sort_heap_d ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_D descending sorts an R8VEC using heap sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms,
//    Academic Press, 1978, second edition,
//    ISBN 0-12-519260-6.
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N].
//    On input, the array to be sorted;
//    On output, the array has been sorted.
//
{
  int n1;
  double temp;

  if ( n <= 1 ) 
  {
    return;
  }
//
//  1: Put A into ascending heap form.
//
  r8vec_heap_a ( n, a );
//
//  2: Sort A.
//
//  The smallest object in the heap is in A[0].
//  Move it to position A[N-1].
//
  temp = a[0];
  a[0] = a[n-1];
  a[n-1] = temp;
//
//  Consider the diminished heap of size N1.
//
  for ( n1 = n-1; 2 <= n1; n1-- ) 
  {
//
//  Restore the heap structure of the initial N1 entries of A.
//
    r8vec_heap_a ( n1, a );
//
//  Take the largest object from A[0] and move it to A[N1-1].
//
    temp = a[0];
    a[0] = a[n1-1];
    a[n1-1] = temp;
  }

  return;
}
//****************************************************************************80

int *r8vec_sort_heap_index_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R8VEC
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      r8vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], an array to be index-sorted.
//
//    Output, int R8VEC_SORT_HEAP_INDEX_A[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  double aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j-1]] < a[indx[j]] )
        {
          j = j + 1;
        }
      }

      if ( aval < a[indx[j-1]] )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

int *r8vec_sort_heap_index_d ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_INDEX_D does an indexed heap descending sort of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    Once the index array is computed, the sorting can be carried out
//    "implicitly:
//
//      a(indx(*))
//
//    or explicitly, by the call
//
//      r8vec_permute ( n, indx, 0, a )
//
//    after which a(*) is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], an array to be index-sorted.
//
//    Output, int R8VEC_SORT_HEAP_INDEX_D[N], contains the sort index.  The
//    I-th element of the sorted array is A(INDX(I)).
//
{
  double aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[indx[j]] < a[indx[j-1]] )
        {
          j = j + 1;
        }
      }

      if ( a[indx[j-1]] < aval )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }

    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

int *r8vec_sort_heap_mask_a ( int n, double a[], int mask_num, int mask[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_HEAP_MASK_A: indexed heap ascending sort of a masked R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    An array A is given.  An array MASK of indices into A is given.
//    The routine produces a vector INDX, which is a permutation of the
//    entries of MASK, so that:
//
//      A(MASK(INDX(I)) <= A(MASK(INDX(J))
//
//    whenever
//
//      I <= J
//
//    In other words, only the elements of A that are indexed by MASK
//    are to be considered, and the only thing that happens is that
//    a rearrangment of the indices in MASK is returned that orders the
//    masked elements.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], an array to be index-sorted.
//
//    Input, int MASK_NUM, the number of mask elements.
//
//    Input, int MASK[MASK_NUM], the mask array.  This is
//    simply a list of indices of A.  The entries of MASK should
//    be unique, and each one should be between 1 and N.
//
//    Output, int INDX[MASK_NUM], the sort index.  There are MASK_NUM
//    elements of A selected by MASK.  If we want to list those elements
//    in order, then the I-th element is A(MASK(INDX(I))).
//
{
  double aval;
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;

  if ( n < 1 )
  {
    return NULL;
  }

  if ( mask_num < 1 )
  {
    return NULL;
  }

  if ( mask_num == 1 )
  {
    indx = new int[1];
    indx[0] = 1;
    return indx;
  }

  indx = i4vec_indicator ( mask_num );

  l = mask_num / 2 + 1;
  ir = mask_num;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      aval = a[mask[indxt-1]-1];
    }
    else
    {
      indxt = indx[ir-1];
      aval = a[mask[indxt-1]-1];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( a[mask[indx[j-1]-1]-1] < a[mask[indx[j]-1]-1] )
        {
          j = j + 1;
        }
      }

      if ( aval < a[mask[indx[j-1]-1]-1] )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }

  return indx;
}
//****************************************************************************80

void r8vec_sort_insert_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_INSERT_A ascending sorts an R8VEC using an insertion sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Algorithm 1.1,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 11.
//
//  Parameters:
//
//    Input, int N, the number of items in the vector.
//    N must be positive.
//
//    Input/output, double A[N].
//
//    On input, A contains data to be sorted.
//    On output, the entries of A have been sorted in ascending order.
//
{
  int i;
  int j;
  double x;

  for ( i = 1; i < n; i++ ) 
  {
    x = a[i];

    j = i;

    while ( 1 <= j && x < a[j-1] ) 
    {
      a[j] = a[j-1];
      j = j - 1;
    }
    a[j] = x;
  }

  return;
}
//****************************************************************************80

int *r8vec_sort_insert_index_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_INSERT_INDEX_A ascending index sorts an R8VEC using insertion.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Donald Kreher, Douglas Simpson,
//    Combinatorial Algorithms,
//    CRC Press, 1998, page 11.
//
//  Parameters:
//
//    Input, int N, the number of items in the vector.
//    N must be positive.
//
//    Input, double A[N], the array to be sorted.
//
//    Output, int R8VEC_SORT_INSET_INDEX_A[N], the sorted indices.  The array
//    is sorted when listed from A(INDX(1)) through A(INDX(N)).
//
{
  int i;
  int *indx;
  int j;
  double x;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = i4vec_indicator ( n );

  for ( i = 2; i <= n; i++ )
  {
    x = a[i-1];

    j = i - 1;

    while ( 1 <= j )
    {
      if ( a[indx[j-1]-1] <= x )
      {
        break;
      }

      indx[j] = indx[j-1];
      j = j - 1;
    }
    indx[j] = i;
  }

  return indx;
}
//****************************************************************************80

void r8vec_sort_quick_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_QUICK_A ascending sorts an R8VEC using quick sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Example:
//
//    Input:
//
//      N = 7
//
//      A = ( 6, 7, 3, 2, 9, 1, 8 )
//
//    Output:
//
//      A = ( 1, 2, 3, 6, 7, 8, 9 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 April 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of A.
//
//    Input/output, double A[N].  On input, the array to be sorted.
//    On output, A has been reordered into ascending order.
//
{
# define LEVEL_MAX 30

  int base;
  int l_segment;
  int level;
  int n_segment;
  int rsave[LEVEL_MAX];
  int r_segment;

  if ( n < 1 ) 
  {
    cerr << "\n";
    cerr << "R8VEC_SORT_QUICK_A - Fatal error!\n";
    cerr << "  N < 1.\n";
    exit ( 1 );
  }
  else if ( n == 1 ) 
  {
    return;
  }

  level = 1;
  rsave[0] = n + 1;
  base = 1;
  n_segment = n;

  while ( 0 < n_segment ) 
  {
//
//  Partition the segment.
//
    r8vec_part_quick_a ( n_segment, a+base-1, &l_segment, &r_segment );
//
//  If the left segment has more than one element, we need to partition it.
//
    if ( 1 < l_segment ) 
    {

      if ( LEVEL_MAX < level ) 
      {
        cerr << "\n";
        cerr << "R8VEC_SORT_QUICK_A - Fatal error!\n";
        cerr << "  Exceeding recursion maximum of " << LEVEL_MAX << "\n";
        exit ( 1 );
      }

      level = level + 1;
      n_segment = l_segment;
      rsave[level-1] = r_segment + base - 1;
    }
//
//  The left segment and the middle segment are sorted.
//  Must the right segment be partitioned?
//
    else if ( r_segment < n_segment ) 
    {
      n_segment = n_segment + 1 - r_segment;
      base = base + r_segment - 1;
    }
//
//  Otherwise, we back up a level if there is an earlier one.
//
    else 
    {
      for ( ; ; ) 
      {
        if ( 1 < level ) 
        {
          base = rsave[level-1];
          n_segment = rsave[level-2] - rsave[level-1];
          level = level - 1;
          if ( 0 < n_segment ) 
          {
            break;
          }
        }
        else 
        {
          n_segment = 0;
          break;
        }
      }
    }
  }

  return;
# undef LEVEL_MAX
}
//****************************************************************************80

void r8vec_sort_shell_a ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORT_SHELL_A ascending sorts an R8VEC using Shell's sort.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input/output, double A[N].
//    On input, an array to be sorted.
//    On output, the sorted array.
//
{
  double asave;
  int i;
  int ifree;
  int inc;
  int ipow;
  int j;
  int k;
  int maxpow;
  int test;

  if ( n <= 1 )
  {
    return;
  }
//
//  Determine the smallest MAXPOW so that
//    N <= ( 3**MAXPOW - 1 ) / 2
//
  maxpow = 1;
  test = 3;

  while ( test < 2 * n + 1 )
  {
    maxpow = maxpow + 1;
    test = test * 3;
  }

  if ( 1 < maxpow )
  {
    maxpow = maxpow - 1;
    test = test / 3;
  }
//
//  Now sort groups of size ( 3**IPOW - 1 ) / 2.
//
  for ( ipow = maxpow; 1 <= ipow; ipow-- )
  {
    inc = ( test - 1 ) / 2;
    test = test / 3;
//
//  Sort the values with indices equal to K mod INC.
//
    for ( k = 1; k <= inc; k++ )
    {
//
//  Insertion sort of the items with index
//  INC+K, 2*INC+K, 3*INC+K, ...
//
      for ( i = inc+k; i <= n; i = i + inc )
      {
        asave = a[i-1];
        ifree = i;
        j = i - inc;

        for ( ; ; )
        {
          if ( j < 1 )
          {
            break;
          }

          if ( a[j-1] <= asave )
          {
            break;
          }

          ifree = j;
          a[j+inc-1] = a[j-1];
          j = j - inc;
        }
        a[ifree-1] = asave;
      }
    }
  }

  return;
}
//****************************************************************************80

double *r8vec_sorted_merge_a ( int na, double a[], int nb, double b[], int *nc )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_MERGE_A merges two ascending sorted R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The elements of A and B should be sorted in ascending order.
//
//    The elements in the output array C will also be in ascending order,
//    and unique.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int NA, the dimension of A.
//
//    Input, double A[NA], the first sorted array.
//
//    Input, int NB, the dimension of B.
//
//    Input, double B[NB], the second sorted array.
//
//    Output, int *NC, the number of entries in the merged vector.
//
//    Output, double R8VEC_SORTED_MERGE_A[NC], the merged unique sorted array.
//
{
  double *c;
  double *d;
  int j;
  int ja;
  int jb;
  int na2;
  int nb2;
  int nd;
  int order;

  na2 = na;
  nb2 = nb;

  ja = 0;
  jb = 0;
  *nc = 0;
  nd = 0;
  d = new double[na+nb];

  order = r8vec_order_type ( na2, a );

  if ( order < 0 || 2 < order )
  {
    cout << "\n";
    cout << "R8VEC_SORTED_MERGE_A - Fatal error!\n";
    cout << "  The input array A is not ascending sorted.\n";
    return NULL;
  }

  order = r8vec_order_type ( nb2, b );

  if ( order < 0 || 2 < order )
  {
    cout << "\n";
    cout << "R8VEC_SORTED_MERGE_A - Fatal error!\n";
    cout << "  The input array B is not ascending sorted.\n";
    return NULL;
  }

  for ( ; ; )
  {
//
//  If we've used up all the entries of A, stick the rest of B on the end.
//
    if ( na2 <= ja )
    {
      for ( j = 1; j <= nb2 - jb; j++ )
      {
        jb = jb + 1;
        if ( nd == 0 )
        {
          nd = nd + 1;
          d[nd-1] = b[jb-1];
        }
        else if ( d[nd-1] < b[jb-1] )
        {
          nd = nd + 1;
          d[nd-1] = b[jb-1];
        }
      }
      break;
    }
//
//  If we've used up all the entries of B, stick the rest of A on the end.
//
    else if ( nb2 <= jb )
    {
      for ( j = 1; j <= na2 - ja; j++ )
      {
        ja = ja + 1;
        if ( nd == 0 )
        {
          nd = nd + 1;
          d[nd-1] = a[ja-1];
        }
        else if ( d[nd-1] < a[ja-1] )
        {
          nd = nd + 1;
          d[nd-1] = a[ja-1];
        }
      }
      break;
    }
//
//  Otherwise, if the next entry of A is smaller, that's our candidate.
//
    else if ( a[ja] <= b[jb] )
    {
      ja = ja + 1;
      if ( nd == 0 )
      {
        nd = nd + 1;
        d[nd-1] = a[ja-1];
      }
      else if ( d[nd-1] < a[ja-1] )
      {
        nd = nd + 1;
        d[nd-1] = a[ja-1];
      }
    }
//
//  ...or if the next entry of B is the smaller, consider that.
//
    else
    {
      jb = jb + 1;
      if ( nd == 0 )
      {
        nd = nd + 1;
        d[nd-1] = b[jb-1];
      }
      else if ( d[nd-1] < b[jb-1] )
      {
        nd = nd + 1;
        d[nd-1] = b[jb-1];
      }
    }
  }

  *nc = nd;

  c = r8vec_copy_new ( nd, d );

  delete [] d;

  return c;
}
//****************************************************************************80

int r8vec_sorted_nearest ( int n, double a[], double value )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_NEAREST returns the nearest element in a sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], a sorted vector.
//
//    Input, double VALUE, the value whose nearest vector entry is sought.
//
//    Output, int R8VEC_SORTED_NEAREST, the index of the nearest
//    entry in the vector.
//
{
  int hi;
  int i;
  int lo;
  int mid;

  if ( n < 1 )
  {
    return (-1);
  }

  if ( n == 1 )
  {
    return 1;
  }

  if ( a[0] < a[n-1] )
  {
    if ( value < a[0] )
    {
      return 1;
    }
    else if ( a[n-1] < value )
    {
      return n;
    }
//
//  Seek an interval containing the value.
//
    lo = 1;
    hi = n;

    while ( lo < hi - 1 )
    {
      mid = ( lo + hi ) / 2;

      if ( value == a[mid-1] )
      {
        return mid;
      }
      else if ( value < a[mid-1] )
      {
        hi = mid;
      }
      else
      {
        lo = mid;
      }
    }
//
//  Take the nearest.
//
    if ( r8_abs ( value - a[lo-1] ) < r8_abs ( value - a[hi-1] ) )
    {
      return lo;
    }
    else
    {
      return hi;
    }
  }
//
//  A descending sorted vector A.
//
  else
  {
    if ( value < a[n-1] )
    {
      return n;
    }
    else if ( a[0] < value )
    {
      return 1;
    }
//
//  Seek an interval containing the value.
//
    lo = n;
    hi = 1;

    while ( lo < hi - 1 )
    {
      mid = ( lo + hi ) / 2;

      if ( value == a[mid-1] )
      {
        return mid;
      }
      else if ( value < a[mid-1] )
      {
        hi = mid;
      }
      else
      {
        lo = mid;
      }
    }
//
//  Take the nearest.
//
    if ( r8_abs ( value - a[lo-1] ) < r8_abs ( value - a[hi-1] ) )
    {
      return lo;
    }
    else
    {
      return hi;
    }
  }
}
//****************************************************************************80

void r8vec_sorted_split ( int n, double a[], double split, int *i_lt, 
  int *i_gt )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_SPLIT "splits" a sorted R8VEC, given a splitting value.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Given a splitting value SPLIT, the routine seeks indices 
//    I_LT and I_GT so that
//
//      A(I_LT) < SPLIT < A(I_GT),
//
//    and if there are intermediate index values between I_LT and
//    I_GT, then those entries of A are exactly equal to SPLIT.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters
//
//    Input, int N, the number of entries in A.
//
//    Input, double A[N], a sorted array.
//
//    Input, double SPLIT, a value to which the entries in A are
//    to be compared.
//
//    Output, int *I_LT:
//    0 if no entries are less than SPLIT;
//    N if all entries are less than SPLIT;
//    otherwise, the index of the last entry in A less than SPLIT.
//
//    Output, int *I_GT:
//    1 if all entries are greater than SPLIT;
//    N+1 if no entries are greater than SPLIT;
//    otherwise the index of the first entry in A greater than SPLIT.
//
{
  int hi;
  int i;
  int lo;
  int mid;

  if ( n < 1 )
  {
    *i_lt = -1;
    *i_gt = -1;
    return;
  }

  if ( split < a[0] )
  {
    *i_lt = 0;
    *i_gt = 1;
    return;
  }

  if ( a[n-1] < split )
  {
    *i_lt = n;
    *i_gt = n + 1;
    return;
  }

  lo = 1;
  hi = n;

  for ( ; ; )
  {
    if ( lo + 1 == hi )
    {
      *i_lt = lo;
      break;
    }

    mid = ( lo + hi ) / 2;

    if ( split <= a[mid-1] )
    {
      hi = mid;
    }
    else
    {
      lo = mid;
    }
  }

  for ( i = *i_lt + 1; i <= n; i++ )
  {
    if ( split < a[i-1] )
    {
      *i_gt = i;
      return;
    }
  }

  *i_gt = n + 1;

  return;
}
//****************************************************************************80

void r8vec_sorted_undex ( int x_num, double x_val[], int x_unique_num, 
  double tol, int undx[], int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_UNDEX returns unique sorted indexes for a sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    Assuming X is already sorted, we examine the entries of X in order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(I) = X(UNDX(I)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I      X      XU  Undx  Xdnu
//    ----+------+------+-----+-----+
//      0 | 11.0 |  11.0    0     0
//      1 | 11.0 |  22.0    4     0
//      2 | 11.0 |  33.0    7     0
//      3 | 11.0 |  55.0    8     0
//      4 | 22.0 |                1
//      5 | 22.0 |                1
//      6 | 22.0 |                1
//      7 | 33.0 |                2
//      8 | 55.0 |                3
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 November 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_NUM, the number of data values.
//
//    Input, double X_VAL[X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  int i;
  int j;
//
//  Walk through the sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = i;

  xdnu[i] = j;

  for ( i = 1; i < x_num; i++ )
  {
    if ( tol < r8_abs ( x_val[i] - x_val[undx[j]] ) )
    {
      j = j + 1;
      undx[j] = i;
    }
    xdnu[i] = j;
  }

  return;
}
//****************************************************************************80

double *r8vec_sorted_unique ( int n, double a[], double tol, int *unique_num )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_UNIQUE finds the unique elements in a sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If the data is not sorted, the results of the routine will
//    be garbage.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the sorted array of N elements;
//
//    Input, double TOL, a tolerance for checking equality.
//
//    Output, int *UNIQUE_NUM, the number of unique elements of A.
//
//    Output, double R8VEC_SORTED_UNIQUE[UNIQUE_NUM], the unique elements of A.
//
{
  double *a_unique;
  int i;
  int iuniq;

  *unique_num = 0;

  if ( n <= 0 )
  {
    return NULL;
  }
//
//  Determine the number of unique elements.
//
  iuniq = 0;
  *unique_num = 1;

  for ( i = 1; i < n; i++ )
  {
    if ( tol < r8_abs ( a[i] - a[iuniq] ) )
    {
       iuniq = i;
      *unique_num = *unique_num + 1;
    }
  }
//
//  Set aside space for the unique elements.
//
  a_unique = new double[*unique_num];
//
//  Repeat the search, but now store the unique elements.
//
  *unique_num = 0;

  a_unique[*unique_num] = a[0];
  *unique_num = 1;

  for ( i = 1; i < n; i++ )
  {
    if ( tol < r8_abs ( a[i] - a_unique[*unique_num-1] ) )
    {
      a_unique[*unique_num] = a[i];
      *unique_num = *unique_num + 1;
    }
  }

  return a_unique;
}
//****************************************************************************80

int r8vec_sorted_unique_count ( int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_UNIQUE_COUNT counts unique elements in a sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Because the array is sorted, this algorithm is O(N).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the sorted array to examine.
//
//    Input, double TOL, a tolerance for checking equality.
//
//    Output, int R8VEC_SORTED_UNIQUE_COUNT, the number of unique elements of A.
//
{
  int i;
  int unique_num;

  unique_num = 0;

  if ( n < 1 )
  {
    return unique_num;
  }

  unique_num = 1;

  for ( i = 1; i < n; i++ )
  {
    if ( tol < r8_abs ( a[i-1] - a[i] ) )
    {
      unique_num = unique_num + 1;
    }
  }

  return unique_num;
}
//****************************************************************************80

void r8vec_sorted_unique_hist ( int n, double a[], double tol, int maxuniq, 
  int *unique_num, double auniq[], int acount[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SORTED_UNIQUE_HIST histograms unique elements of a sorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the array to examine, which must have been
//    sorted.
//
//    Input, double TOL, a tolerance for checking equality.
//
//    Input, int MAXUNIQ, the maximum number of unique elements
//    that can be handled.  If there are more than MAXUNIQ unique
//    elements in A, the excess will be ignored.
//
//    Output, int *UNIQUE_NUM, the number of unique elements of A.
//
//    Output, double AUNIQ[UNIQUE_NUM], the unique elements of A.
//
//    Output, int ACOUNT[UNIQUE_NUM], the number of times each element
//    of AUNIQ occurs in A.
//
{
  int i;
  int index;
//
//  Start taking statistics.
//
  index = -1;

  for ( i = 0; i < n; i++ ) 
  {

    if ( i == 0 ) 
    {
      index = 0;
      auniq[index] = a[0];
      acount[index] = 1;
    }
    else if ( r8_abs ( a[i] - auniq[index] ) <= tol ) 
    {
      acount[index] = acount[index] + 1;
    }
    else if ( index + 1 < maxuniq ) 
    {
      index = index + 1;
      auniq[index] = a[i];
      acount[index] = 1;
    }
  }

  *unique_num = index + 1;

  return;
}
//****************************************************************************80

int r8vec_split ( int n, double a[], double split )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SPLIT "splits" an unsorted R8VEC based on a splitting value.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    If the vector is already sorted, it is simpler to do a binary search
//    on the data than to call this routine.
//
//    The vector is not assumed to be sorted before input, and is not
//    sorted during processing.  If sorting is not needed, then it is
//    more efficient to use this routine.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input/output, double A[N], the array to split.  On output,
//    all the entries of A that are less than or equal to SPLIT
//    are in A(1:ISPLIT).
//
//    Input, double SPLIT, the value used to split the vector.
//    It is not necessary that any value of A actually equal SPLIT.
//
//    Output, int R8VEC_SPLIT, indicates the position of the last
//    entry of the split vector that is less than or equal to SPLIT.
//
{
  int i;
  int i1;
  int i2;
  int i3;
  int isplit;
  int j1;
  int j2;
  int j3;
  double temp;
//
//  Partition the vector into A1, A2, A3, where
//    A1 = A(I1:J1) holds values <= SPLIT,
//    A2 = A(I2:J2) holds untested values,
//    A3 = A(I3:J3) holds values > SPLIT.
//
  i1 = 1;
  j1 = 0;

  i2 = 1;
  j2 = n;

  i3 = n+1;
  j3 = n;
//
//  Pick the next item from A2, and move it into A1 or A3.
//  Adjust indices appropriately.
//
  for ( i = 1; i <= n; i++ )
  {
    if ( a[i2-1] <= split )
    {
      i2 = i2 + 1;
      j1 = j1 + 1;
    }
    else
    {
      temp = a[i2-1];
      a[i2-1] = a[i3-2];
      a[i3-2] = temp;
      i3 = i3 - 1;
      j2 = j2 - 1;
    }
  }

  isplit = j1;

  return isplit;
}
//****************************************************************************

double r8vec_std ( int n, double a[] )

//****************************************************************************
//
//  Purpose:
//
//    R8VEC_STD returns the standard deviation of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The standard deviation of a vector X of length N is defined as
//
//      mean ( X(1:n) ) = sum ( X(1:n) ) / n
//
//      std ( X(1:n) ) = sqrt ( sum ( ( X(1:n) - mean )**2 ) / ( n - 1 ) )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    14 April 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//    N should be at least 2.
//
//    Input, double A[N], the vector.
//
//    Output, double R8VEC_STD, the standard deviation of the vector.
//
{
  int i;
  double mean;
  double std;

  if ( n < 2 )
  {
    std = 0.0;
  }
  else
  {
    mean = 0.0;
    for ( i = 0; i < n; i++ )
    {
      mean = mean + a[i];
    }
    mean = mean / ( ( double ) n );

    std = 0.0;
    for ( i = 0; i < n; i++ )
    {
      std = std + ( a[i] - mean ) * ( a[i] - mean );
    }
    std = sqrt ( std / ( ( double ) ( n - 1 ) ) );
  }

  return std;
}
//****************************************************************************80

double r8vec_sum ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SUM returns the sum of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    15 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double A[N], the vector.
//
//    Output, double R8VEC_SUM, the sum of the vector.
//
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a[i];
  }

  return value;
}
//****************************************************************************80

void r8vec_swap ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_SWAP swaps the entries of two R8VEC's.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the arrays.
//
//    Input/output, double A1[N], A2[N], the vectors to swap.
//
{
  int i;
  double temp;

  for ( i = 0; i < n; i++ )
  {
    temp  = a1[i];
    a1[i] = a2[i];
    a2[i] = temp;
  }

  return;
}
//****************************************************************************80

void r8vec_undex ( int x_num, double x_val[], int x_unique_num, double tol,
  int undx[], int xdnu[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNDEX returns unique sorted indexes for an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    The goal of this routine is to determine a vector UNDX,
//    which points, to the unique elements of X, in sorted order,
//    and a vector XDNU, which identifies, for each entry of X, the index of
//    the unique sorted element of X.
//
//    This is all done with index vectors, so that the elements of
//    X are never moved.
//
//    The first step of the algorithm requires the indexed sorting
//    of X, which creates arrays INDX and XDNI.  (If all the entries
//    of X are unique, then these arrays are the same as UNDX and XDNU.)
//
//    We then use INDX to examine the entries of X in sorted order,
//    noting the unique entries, creating the entries of XDNU and
//    UNDX as we go.
//
//    Once this process has been completed, the vector X could be
//    replaced by a compressed vector XU, containing the unique entries
//    of X in sorted order, using the formula
//
//      XU(*) = X(UNDX(*)).
//
//    We could then, if we wished, reconstruct the entire vector X, or
//    any element of it, by index, as follows:
//
//      X(I) = XU(XDNU(I)).
//
//    We could then replace X by the combination of XU and XDNU.
//
//    Later, when we need the I-th entry of X, we can locate it as
//    the XDNU(I)-th entry of XU.
//
//    Here is an example of a vector X, the sort and inverse sort
//    index vectors, and the unique sort and inverse unique sort vectors
//    and the compressed unique sorted vector.
//
//      I     X  Indx  Xdni       XU  Undx  Xdnu
//    ----+-----+-----+-----+--------+-----+-----+
//      0 | 11.     0     0 |    11.     0     0
//      1 | 22.     2     4 |    22.     1     1
//      2 | 11.     5     1 |    33.     3     0
//      3 | 33.     8     7 |    55.     4     2
//      4 | 55.     1     8 |                  3
//      5 | 11.     6     2 |                  0
//      6 | 22.     7     5 |                  1
//      7 | 22.     3     6 |                  1
//      8 | 11.     4     3 |                  0
//
//    INDX(2) = 3 means that sorted item(2) is X(3).
//    XDNI(2) = 5 means that X(2) is sorted item(5).
//
//    UNDX(3) = 4 means that unique sorted item(3) is at X(4).
//    XDNU(8) = 2 means that X(8) is at unique sorted item(2).
//
//    XU(XDNU(I))) = X(I).
//    XU(I)        = X(UNDX(I)).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    28 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int X_NUM, the number of data values.
//
//    Input, double X_VAL[X_NUM], the data values.
//
//    Input, int X_UNIQUE_NUM, the number of unique values in X_VAL.
//    This value is only required for languages in which the size of
//    UNDX must be known in advance.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int UNDX[X_UNIQUE_NUM], the UNDX vector.
//
//    Output, int XDNU[X_NUM], the XDNU vector.
//
{
  int i;
  int *indx;
  int j;
//
//  Implicitly sort the array.
//
  indx = r8vec_sort_heap_index_a ( x_num, x_val );
//
//  Walk through the implicitly sorted array X.
//
  i = 0;

  j = 0;
  undx[j] = indx[i];

  xdnu[indx[i]] = j;

  for ( i = 1; i < x_num; i++ )
  {
    if ( tol < r8_abs ( x_val[indx[i]] - x_val[undx[j]] ) )
    {
      j = j + 1;
      undx[j] = indx[i];
    }
    xdnu[indx[i]] = j;
  }
  delete [] indx;

  return;
}
//****************************************************************************80

double *r8vec_uniform ( int n, double b, double c, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIFORM returns a scaled pseudorandom R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double B, C, the range allowed for the entries.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8VEC_UNIFORM[N], the pseudorandom vector.
//
{
  double *a;
  int i;

  a = new double[n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = r8_uniform ( b, c, seed );
  }

  return a; 
}
//****************************************************************************80

double *r8vec_uniform_01 ( int n, int *seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIFORM_01 returns a unit pseudorandom R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    This routine implements the recursion
//
//      seed = 16807 * seed mod ( 2**31 - 1 )
//      unif = seed / ( 2**31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Springer Verlag, pages 201-202, 1983.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, pages 362-376, 1986.
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int *SEED, a seed for the random number generator.
//
//    Output, double R8VEC_UNIFORM_01[N], the vector of pseudorandom values.
//
{
  int i;
  int k;
  double *r;

  r = new double[n];

  for ( i = 0; i < n; i++ )
  {
    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
      *seed = *seed + 2147483647;
    }

    r[i] = ( double ) ( *seed ) * 4.656612875E-10;
  }

  return r;
}
//****************************************************************************80

int r8vec_unique_count ( int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIQUE_COUNT counts the unique elements in an unsorted R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    Because the array is unsorted, this algorithm is O(N^2).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the array to examine, which does NOT have to
//    be sorted.
//
//    Input, double TOL, a tolerance for checking equality.
//
//    Output, int R8VEC_UNIQUE_COUNT, the number of unique elements of A.
//
{
  int i;
  int j;
  int unique_num;

  unique_num = 0;

  for ( i = 0; i < n; i++ )
  {
    unique_num = unique_num + 1;

    for ( j = 0; j < i; j++ )
    {
      if ( r8_abs ( a[i] - a[j] ) <= tol )
      {
        unique_num = unique_num - 1;
        break;
      }
    }
  }
  return unique_num;
}
//****************************************************************************80

int *r8vec_unique_index ( int n, double a[], double tol )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIQUE_INDEX indexes the unique occurrence of values in an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//    For element A(I) of the vector, UNIQUE_INDEX(I) is the uniqueness index
//    of A(I).  That is, if A_UNIQUE contains the unique elements of A, 
//    gathered in order, then 
//
//      A_UNIQUE ( UNIQUE_INDEX(I) ) = A(I)
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 August 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Input, double A[N], the unsorted array to examine.
//
//    Input, double TOL, a tolerance for equality.
//
//    Output, int R8VEC_UNIQUE_INDEX[N], the unique index.
//
{
  int i;
  int j;
  int *unique_index;
  int unique_num;

  unique_index = new int[n];

  for ( i = 0; i < n; i++ )
  {
    unique_index[i] = -1;
  }
  unique_num = 0;

  for ( i = 0; i < n; i++ )
  {
    if ( unique_index[i] == -1 )
    {
      unique_index[i] = unique_num;
      for ( j = i + 1; j < n; j++ )
      {
        if ( r8_abs ( a[i] - a[j] ) <= tol )
        {
          unique_index[j] = unique_num;
        }
      }
      unique_num = unique_num + 1;
    }
  }
  return unique_index;
}
//****************************************************************************80

void r8vec_unit_sum ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIT_SUM normalizes an R8VEC to have unit sum.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, double A[N], the vector to be normalized.
//    On output, the entries of A should have unit sum.  However, if
//    the input vector has zero sum, the routine halts.
//
{
  double a_sum;
  int i;

  a_sum = 0.0;
  for ( i = 0; i < n; i++ )
  {
    a_sum = a_sum + a[i];
  }

  if ( a_sum == 0.0 )
  {
    cout << "\n";
    cout << "R8VEC_UNIT_SUM - Fatal error!\n";
    cout << "  The vector entries sum to 0.\n";
    exit ( 1 );
  }

  for ( i = 0; i < n; i++ )
  {
    a[i] = a[i] / a_sum;
  }

  return;
}
//****************************************************************************80

double r8vec_variance ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_VARIANCE returns the variance of an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 May 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input, double X[N], the vector whose variance is desired.
//
//    Output, double R8VEC_VARIANCE, the variance of the vector entries.
//
{
  int i;
  double mean;
  double variance;

  mean = r8vec_mean ( n, x );

  variance = 0.0;
  for ( i = 0; i < n; i++ ) 
  {
    variance = variance + ( x[i] - mean ) * ( x[i] - mean );
  }

  if ( 1 < n ) 
  {
    variance = variance / ( double ) ( n - 1 );
  }
  else
  {
    variance = 0.0;
  }

  return variance;
}
//****************************************************************************80

void r8vec_write ( int n, double r[], char *output_file )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_WRITE writes an R8VEC to a file.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 August 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//
//    Input, double R[N], the vector to be written.
//
//    Input, char *OUTPUT_FILE, the name of the file to which
//    the information is to be written.
//
{
  int i;
  ofstream output;

  output.open ( output_file );

  if ( !output )
  {
    cout << "\n";
    cout << "R8VEC_WRITE - Fatal error!\n";
    cout << "  Could not open the output file.\n";
    return;
  }

  for ( i = 0; i < n; i++ )
  {
    output << "  " << setw(16) << r[i] << "\n";
  }

  output.close ( );

  return;
}
//****************************************************************************80

void r8vec_zero ( int n, double a[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_ZERO zeroes an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, double A[N], a vector of zeroes.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0.0;
  }
  return;
}
//****************************************************************************80

double *r8vec_zero_new ( int n )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_ZERO_NEW creates and zeroes an R8VEC.
//
//  Discussion:
//
//    An R8VEC is a vector of R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    10 July 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Output, double R8VEC_ZERO_NEW[N], a vector of zeroes.
//
{
  double *a;
  int i;

  a = new double[n];

  for ( i = 0; i < n; i++ )
  {
    a[i] = 0.0;
  }
  return a;
}
//****************************************************************************80

int r8vec2_compare ( int n, double a1[], double a2[], int i, int j )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_COMPARE compares two elements of an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of data items.
//
//    Input, double A1[N], A2[N], contain the two components of each item.
//
//    Input, int I, J, the items to be compared.  These values will be
//    1-based indices for the arrays A1 and A2.
//
//    Output, int R8VEC2_COMPARE, the results of the comparison:
//    -1, item I < item J,
//     0, item I = item J,
//    +1, item J < item I.
//
{
  int isgn;

  isgn = 0;

  if ( a1[i-1] < a1[j-1] )
  {
    isgn = -1;
  }
  else if ( a1[i-1] == a1[j-1] )
  {
    if ( a2[i-1] < a2[j-1] )
    {
      isgn = -1;
    }
    else if ( a2[i-1] < a2[j-1] )
    {
      isgn = 0;
    }
    else if ( a2[j-1] < a2[i-1] )
    {
      isgn = +1;
    }
  }
  else if ( a1[j-1] < a1[i-1] )
  {
    isgn = +1;
  }

  return isgn;
}
//****************************************************************************80

void r8vec2_print ( int n, double a1[], double a2[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_PRINT prints an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 November 2002
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A1[N], double A2[N], the vectors to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( 0 < s_len_trim ( title ) ) 
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i <= n-1; i++ ) 
  {
    cout << setw(6)  << i + 1 << "  " 
         << setw(14) << a1[i] << "  " 
         << setw(14) << a2[i] << "\n";
  }

  return;
}
//****************************************************************************80

void r8vec2_print_some ( int n, double x1[], double x2[], int max_print, 
  char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_PRINT_SOME prints "some" of an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    The user specifies MAX_PRINT, the maximum number of lines to print.
//
//    If N, the size of the vectors, is no more than MAX_PRINT, then
//    the entire vectors are printed, one entry of each per line.
//
//    Otherwise, if possible, the first MAX_PRINT-2 entries are printed,
//    followed by a line of periods suggesting an omission,
//    and the last entry.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 November 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries of the vectors.
//
//    Input, double X1[N], X2[N], the vector to be printed.
//
//    Input, int MAX_PRINT, the maximum number of lines to print.
//
//    Input, char *TITLE, an optional title.
//
{
  int i;

  if ( max_print <= 0 )
  {
    return;
  }

  if ( n <= 0 )
  {
    return;
  }

  if ( 0 < s_len_trim ( title ) )
  {
    cout << "\n";
    cout << title << "\n";
    cout << "\n";
  }

  if ( n <= max_print )
  {
    for ( i = 0; i < n; i++ )
    {
      cout << setw(6)  << i + 1 << "  "
           << setw(14) << x1[i] << "  "
           << setw(14) << x2[i] << "\n";
    }
  }
  else if ( 3 <= max_print )
  {
    for ( i = 0; i < max_print-2; i++ )
    {
      cout << setw(6)  << i + 1 << "  "
           << setw(14) << x1[i] << "  "
           << setw(14) << x2[i] << "\n";
    }
    cout << "......  ..............  ..............\n";
    i = n - 1;
    cout << setw(6)  << i + 1 << "  "
         << setw(14) << x1[i] << "  "
         << setw(14) << x2[i] << "\n";
  }
  else
  {
    for ( i = 0; i < max_print - 1; i++ )
    {
      cout << setw(6)  << i + 1 << "  "
           << setw(14) << x1[i] << "  "
           << setw(14) << x2[i] << "\n";
    }
    i = max_print - 1;
    cout << setw(6)  << i + 1 << "  "
         << setw(14) << x1[i] << "  "
         << setw(14) << x2[i] << "...more entries...\n";
  }

  return;
}
//****************************************************************************80

void r8vec2_sort_a ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SORT_A ascending sorts an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    Each item to be sorted is a pair of reals (X,Y), with the X
//    and Y values stored in separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items of data.
//
//    Input/output, double A1[N], A2[N], the data to be sorted.
//
{
  int i;
  int indx;
  int isgn;
  int j;
  double temp;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      temp    = a1[i-1];
      a1[i-1] = a1[j-1];
      a1[j-1] = temp;

      temp    = a2[i-1];
      a2[i-1] = a2[j-1];
      a2[j-1] = temp;
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = r8vec2_compare ( n, a1, a2, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

void r8vec2_sort_d ( int n, double a1[], double a2[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SORT_D descending sorts an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    Each item to be sorted is a pair of reals (X,Y), with the X
//    and Y values stored in separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items of data.
//
//    Input/output, double A1[N], A2[N], the data to be sorted.
//
{
  int i;
  int indx;
  int isgn;
  int j;
  double temp;
//
//  Initialize.
//
  i = 0;
  indx = 0;
  isgn = 0;
  j = 0;
//
//  Call the external heap sorter.
//
  for ( ; ; )
  {
    sort_heap_external ( n, &indx, &i, &j, isgn );
//
//  Interchange the I and J objects.
//
    if ( 0 < indx )
    {
      temp    = a1[i-1];
      a1[i-1] = a1[j-1];
      a1[j-1] = temp;

      temp    = a2[i-1];
      a2[i-1] = a2[j-1];
      a2[j-1] = temp;
    }
//
//  Compare the I and J objects.
//
    else if ( indx < 0 )
    {
      isgn = - r8vec2_compare ( n, a1, a2, i, j );
    }
    else if ( indx == 0 )
    {
      break;
    }
  }

  return;
}
//****************************************************************************80

int *r8vec2_sort_heap_index_a ( int n, double x[], double y[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SORT_HEAP_INDEX_A does an indexed heap ascending sort of an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    The sorting is not actually carried out.  Rather an index array is
//    created which defines the sorting.  This array may be used to sort
//    or index the array, or to sort or index related arrays keyed on the
//    original array.
//
//    ( X(I), Y(I) ) < ( X(J), Y(J) ) if:
//
//    * X(I) < X(J), or
//
//    * X(I) = X(J), and Y(I) < Y(J).
//
//    Once the index array is computed, the sorting can be carried out
//    implicitly:
//
//      ( x(indx(*)), y(indx(*) )
//
//    or explicitly, by the calls
//
//      r8vec_permute ( n, indx, 0, x )
//      r8vec_permute ( n, indx, 0, y )
//
//    after which ( x(*), y(*) ), is sorted.
//
//    Note that the index vector is 0-based.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 October 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double X[N], Y[N], pairs of X, Y coordinates of points.
//
//    Output, int INDX[N], the sort index.  The
//    I-th element of the sorted array has coordinates 
//    ( X(INDX(I)), Y(INDX(I) ).
//
{
  int i;
  int *indx;
  int indxt;
  int ir;
  int j;
  int l;
  double xval;
  double yval;

  if ( n < 1 )
  {
    return NULL;
  }

  indx = new int[n];

  for ( i = 0; i < n; i++ )
  {
    indx[i] = i;
  }

  if ( n == 1 )
  {
    return indx;
  }

  l = n / 2 + 1;
  ir = n;

  for ( ; ; )
  {
    if ( 1 < l )
    {
      l = l - 1;
      indxt = indx[l-1];
      xval = x[indxt];
      yval = y[indxt];
    }
    else
    {
      indxt = indx[ir-1];
      xval = x[indxt];
      yval = y[indxt];
      indx[ir-1] = indx[0];
      ir = ir - 1;

      if ( ir == 1 )
      {
        indx[0] = indxt;
        break;
      }
    }

    i = l;
    j = l + l;

    while ( j <= ir )
    {
      if ( j < ir )
      {
        if ( x[indx[j-1]] < x[indx[j]] || 
          ( x[indx[j-1]] == x[indx[j]] && y[indx[j-1]] < y[indx[j]] ) )
        {
          j = j + 1;
        }
      }

      if ( xval < x[indx[j-1]] || 
         ( xval == x[indx[j-1]] && yval < y[indx[j-1]] ) )
      {
        indx[i-1] = indx[j-1];
        i = j;
        j = j + j;
      }
      else
      {
        j = ir + 1;
      }
    }
    indx[i-1] = indxt;
  }
  return indx;
}
//****************************************************************************80

void r8vec2_sorted_unique ( int n, double a1[], double a2[], int *unique_num )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SORTED_UNIQUE keeps the unique elements in an R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    Item I is stored as the pair A1(I), A2(I).
//
//    The items must have been sorted, or at least it must be the
//    case that equal items are stored in adjacent vector locations.
//
//    If the items were not sorted, then this routine will only
//    replace a string of equal values by a single representative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items.
//
//    Input/output, double A1[N], A2[N].
//    On input, the array of N items.
//    On output, an array of UNIQUE_NUM unique items.
//
//    Output, int *UNIQUE_NUM, the number of unique items.
//
{
  int itest;

  *unique_num = 0;

  if ( n <= 0 )
  {
    return;
  }

  *unique_num = 1;

  for ( itest = 1; itest < n; itest++ )
  {
    if ( a1[itest] != a1[*unique_num-1] || 
         a2[itest] != a2[*unique_num-1] )
    {
      a1[*unique_num] = a1[itest];
      a2[*unique_num] = a2[itest];
      *unique_num = *unique_num + 1;
    }
  }

  return;
}
//****************************************************************************80

void r8vec2_sorted_unique_index ( int n, double a1[], double a2[], 
  int *unique_num, int indx[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SORTED_UNIQUE_INDEX indexes unique elements in a sorted R8VEC2.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//    Item I is stored as the pair A1(I), A2(I).
//
//    The items must have been sorted, or at least it should be the
//    case that equal items are stored in adjacent vector locations.
//
//    If the items are not sorted, then this routine will only
//    replace a string of equal values by a single representative.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of items.
//
//    Input/output, double A1[N], A2[N].
//    On input, the array of N items.
//    On output, an array of unique items.
//
//    Output, int *UNIQUE_NUM, the number of unique items.
//
//    Output, int INDX[N], contains in entries 1 through UNIQUE_NUM an index
//    array of the unique items.  To build new arrays with no repeated elements:
//      B1(*) = A1(INDX(*))
//
{
  int itest;

  if ( n <= 0 )
  {
    *unique_num = 0;
    return;
  }
  i4vec_zero ( n, indx );

  *unique_num = 1;
  indx[0] = 1;

  for ( itest = 2; itest <= n; itest++ )
  {
    if ( a1[itest-2] != a1[itest-1] || a2[itest-2] != a2[itest-1] )
    {
      *unique_num = *unique_num + 1;
      indx[*unique_num-1] = itest;
    }
  }

  return;
}
//****************************************************************************80

int r8vec2_sum_max_index ( int n, double a[], double b[] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC2_SUM_MAX_INDEX returns the index of the maximum sum of two R8VEC's.
//
//  Discussion:
//
//    An R8VEC2 is a dataset consisting of N pairs of real values, stored
//    as two separate vectors A1 and A2.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    17 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the array.
//
//    Input, double A[N], B[N], two arrays whose sum
//    is to be examined.
//
//    Output, int R8VEC2_SUM_MAX_INDEX, the index of the largest entry in A+B.
//
{
  int i;
  double sum_max;
  int sum_max_index;

  if ( n <= 0 )
  {
    sum_max_index = -1;
  }
  else
  {
    sum_max_index = 1;
    sum_max = a[0] + b[0];

    for ( i = 2; i <= n; i++ )
    {
      if ( sum_max < a[i-1] + b[i-1] )
      {
        sum_max = a[i-1] + b[i-1];
        sum_max_index = i;
      }
    }
  }
  return sum_max_index;
}
//****************************************************************************80

void r8vec3_print ( int n, double a1[], double a2[], double a3[], char *title )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC3_PRINT prints a triple of real vectors.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    01 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of components of the vector.
//
//    Input, double A1[N], double A2[N], double A3[N], the vectors 
//    to be printed.
//
//    Input, char *TITLE, a title to be printed first.
//    TITLE may be blank.
//
{
  int i;

  if ( strlen ( title ) != 0 ) 
  {
    cout << "\n";
    cout << title << "\n";
  }

  cout << "\n";
  for ( i = 0; i <= n-1; i++ ) 
  {
    cout << setw(4)  << i + 1 << "  " 
         << setw(10) << a1[i] << "  " 
         << setw(10) << a2[i] << "  " 
         << setw(10) << a3[i] << "\n";
  }

  return;
}
//****************************************************************************80

double *r8vec3_vtriple ( double v1[3], double v2[3], double v3[3] )

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC3_VTRIPLE computes the vector triple product of three vectors.
//
//  Discussion:
//
//    VTRIPLE = V1 x (V2 x V3)
//
//    VTRIPLE is a vector perpendicular to V1, lying in the plane
//    spanned by V2 and V3.  The norm of VTRIPLE is the product
//    of the norms of V1, V2 and V3.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, the coordinates
//    of the three vectors.
//
//    Output, double *X, *Y, *Z, the components of the vector triple product.
//
{
  double *v123;
  double *v23;

  v23 = r8vec_cross_3d ( v2, v3 );
 
  v123 = r8vec_cross_3d ( v1, v23 );
 
  delete [] v23;

  return v123;
}
//****************************************************************************80

double radians_to_degrees ( double angle )

//****************************************************************************80
//
//  Purpose:
//
//    RADIANS_TO_DEGREES converts an angle from radians to degrees.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double ANGLE, an angle in radians.
//
//    Output, double RADIANS_TO_DEGREES, the equivalent angle in degrees.
//
{
  double pi = 3.141592653589793;
  double value;

  value = ( angle / pi ) * 180.0;

  return value;
}
//****************************************************************************80

unsigned long rand_initialize ( unsigned long seed )

//****************************************************************************80
//
//  Purpose:
//
//    RAND_INITIALIZE initializes the random number generator.
//
//  Discussion:
//
//    If you don't initialize RAND, the random number generator, 
//    it will behave as though it were seeded with value 1.  
//    This routine will either take a user-specified seed, or
//    (if the user passes a 0) make up a "random" one.  In either
//    case, the seed is passed to SRAND (the appropriate routine 
//    to call when setting the seed for RAND).  The seed is also
//    returned to the user as the value of the function.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    29 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, unsigned long SEED, is either 0, which means that the user
//    wants this routine to come up with a seed, or nonzero, in which
//    case the user has supplied the seed.
//
//    Output, unsigned long RAND_INITIALIZE, is the value of the seed
//    passed to SRAND, which is either the user's input value, or if
//    that was zero, the value selected by this routine.
//
{
  if ( seed != 0 )
  {
    cout << "\n";
    cout << "RAND_INITIALIZE\n";
    cout << "  Initialize RAND with user SEED = " << seed << "\n";
  }
  else
  {
    seed = get_seed ( );

    cout << "\n";
    cout << "RAND_INITIALIZE\n";
    cout << "  Initialize RAND with arbitrary SEED = " << seed << "\n";
  }
//
//  Now set the seed.
//
  srand ( seed );

  return seed;
}
//****************************************************************************80

unsigned long random_initialize ( unsigned long seed )

//****************************************************************************80
//
//  Purpose:
//
//    RANDOM_INITIALIZE initializes the RANDOM random number generator.
//
//  Discussion:
//
//    If you don't initialize RANDOM, the random number generator, 
//    it will behave as though it were seeded with value 1.  
//    This routine will either take a user-specified seed, or
//    (if the user passes a 0) make up a "random" one.  In either
//    case, the seed is passed to SRANDOM (the appropriate routine 
//    to call when setting the seed for RANDOM).  The seed is also
//    returned to the user as the value of the function.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    11 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, unsigned long SEED, is either 0, which means that the user
//    wants this routine to come up with a seed, or nonzero, in which
//    case the user has supplied the seed.
//
//    Output, unsigned long RANDOM_INITIALIZE, is the value of the seed
//    passed to SRANDOM, which is either the user's input value, or if
//    that was zero, the value selected by this routine.
//
{
# define DEBUG 0

  if ( seed != 0 )
  {
    if ( DEBUG )
    {
      cout << "\n";
      cout << "RANDOM_INITIALIZE\n";
      cout << "  Initialize RANDOM with user SEED = " << seed << "\n";
    }
  }
  else
  {
    seed = get_seed ( );
    if ( DEBUG )
    {
      cout << "\n";
      cout << "RANDOM_INITIALIZE\n";
      cout << "  Initialize RANDOM with arbitrary SEED = " << seed << "\n";
    }
  }
//
//  Now set the seed.
//
  srandom ( seed );

  return seed;
# undef DEBUG
}
//****************************************************************************80

void rat_factor ( int m, int n, int factor_max, int *factor_num, int factor[], 
  int power[], int *mleft, int *nleft )

//****************************************************************************80
//
//  Purpose:
//
//    RAT_FACTOR factors a rational value into a product of prime factors.
//
//  Formula:
//
//    ( M / N ) = ( MLEFT / NLEFT ) * Product ( I = 1 to FACTOR_NUM )
//      FACTOR(I)**POWER(I).
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    16 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, N, the top and bottom of a rational value.
//    The ratio of M and N must be positive.
//
//    Input, int FACTOR_MAX, the maximum number of factors for
//    which storage has been allocated.
//
//    Output, int *FACTOR_NUM, the number of prime factors of M/N.
//
//    Output, int FACTOR[FACTOR_MAX], the prime factors of M/N.
//
//    Output, int POWER[FACTOR_MAX].  POWER(I) is the power of
//    the FACTOR(I) in the representation of M/N.
//
//    Output, int *MLEFT, *NLEFT, the top and bottom of the factor of
//    M / N that remains.  If ABS ( MLEFT / NLEFT ) is not 1, then
//    the rational value was not completely factored.
//
{
  int i;
  int p;
  int prime_max;

  *factor_num = 0;

  *mleft = m;
  *nleft = n;
//
//  NLEFT should be nonnegative.
//
  if ( *nleft < 0 )
  {
    *mleft = -(*mleft);
    *nleft = -(*nleft);
  }

  if ( m == 0 || n == 0 )
  {
    return;
  }

  if ( m == n )
  {
    *factor_num = 1;
    factor[0] = 1;
    power[0] = 1;
    return;
  }
//
//  Find out how many primes we stored.
//
  prime_max = prime ( -1 );

  for ( i = 1; i <= prime_max; i++ )
  {
    p = prime ( i );

    if ( ( *nleft % p ) == 0 || ( abs ( *mleft ) % p ) == 0 )
    {
      if ( *factor_num < factor_max )
      {
        *factor_num = *factor_num + 1;
        factor[*factor_num-1] = p;
        power[*factor_num-1] = 0;
//
//  Divide MLEFT by PRIME(I) as often as you can.
//
        if ( ( abs ( *mleft ) % p ) == 0  )
        {
          for ( ; ; )
          {
            power[*factor_num-1] = power[*factor_num-1] + 1;
            *mleft = *mleft / p;

            if ( ( abs ( *mleft ) % p ) != 0 )
            {
              break;
            }
          }
        }
//
//  Divide NLEFT by PRIME(I) as often as you can.
//
        if ( ( *nleft % p ) == 0  )
        {
          for ( ; ; )
          {
            power[*factor_num-1] = power[*factor_num-1] - 1;
            *nleft = *nleft / p;

            if ( ( *nleft % p ) != 0 )
            {
              break;
            }
          }
        }

        if ( power[*factor_num-1] == 0 )
        {
          *factor_num = *factor_num - 1;
        }
      }
    }
  }

  return;
}
//****************************************************************************80

double rickey ( int ab, int bb, int er, double f, int h, int hb, int hp, 
  int r, int so, int tb )

//****************************************************************************80
//
//  Purpose:
//
//    RICKEY evaluates Branch Rickey's baseball index.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 October 2005
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Alan Schwarz,
//    Looking Beyond the Batting Average,
//    The New York Times, 
//    Sunday, 1 August 2004.
//   
//    Branch Rickey,
//    Goodby to Some Old Baseball Ideas,
//    Life Magazine,
//    2 August 1954.
//
//  Parameters:
//
//    Input, int AB, number of at-bats.
//
//    Input, int BB, base on balls.
//
//    Input, int ER, earned runs.
//
//    Input, double F, the fielding rating.
//
//    Input, int H, number of hits.
//
//    Input, int HB, hit batsmen.
//
//    Input, int HP, hit by pitcher.
//
//    Input, int R, runs.
//
//    Input, int SO, strike outs.
//
//    Input, int TB, total bases.
//
//    Output, double RICKEY, the Branch Rickey index, an estimate for the
//    expected winning percentage of a team with the given statistics.
//    (0.5 has already been subtracted from this value.)
//
{
  double g;;
  double hitting;
  double pitching;

  hitting = 
      double ( h + bb + hp ) / double ( ab + bb + hp ) 
    + double ( 3 * ( tb - h ) ) / double ( 4 * ab ) 
    + double ( r ) / double ( h + bb + hp );

  pitching = 
      double ( h ) / double ( ab ) 
    + double ( bb + hb ) / double ( ab + bb + hb ) 
    + double ( er ) / double ( h + bb + hb ) 
    - double ( so ) / double ( 8 * ( ab + bb + hb ) );

  g = hitting - pitching - f;

  return g;
}
//****************************************************************************80

int *roots_to_i4poly ( int n, int x[] )

//****************************************************************************80
//
//  Purpose:
//
//    ROOTS_TO_I4POLY converts polynomial roots to polynomial coefficients.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    24 September 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of roots specified.
//
//    Input, int X[N], the roots.
//
//    Output, int ROOTS_TO_I4POLY[N+1], the coefficients of the polynomial.
//
{
  int *c;
  int i;
  int j;

  c = i4vec_zero_new ( n + 1 );
//
//  Initialize C to (0, 0, ..., 0, 1).
//  Essentially, we are setting up a divided difference table.
//
  c[n] = 1;
//
//  Convert to standard polynomial form by shifting the abscissas
//  of the divided difference table to 0.
//
  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= n+1-j; i++ )
    {
      c[n-i] = c[n-i] - x[n+1-i-j] * c[n-i+1];
    }
  }
  return c;
}
//****************************************************************************80

double *roots_to_r8poly ( int n, double x[] )

//****************************************************************************80
//
//  Purpose:
//
//    ROOTS_TO_R8POLY converts polynomial roots to polynomial coefficients.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    09 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of roots specified.
//
//    Input, double X[N], the roots.
//
//    Output, double ROOTS_TO_R8POLY[N+1], the coefficients of the polynomial.
//
{
  double *c;
  int i;
  int j;

  c = r8vec_zero_new ( n + 1 );
//
//  Initialize C to (0, 0, ..., 0, 1).
//  Essentially, we are setting up a divided difference table.
//
  c[n] = 1.0;
//
//  Convert to standard polynomial form by shifting the abscissas
//  of the divided difference table to 0.
//
  for ( j = 1; j <= n; j++ )
  {
    for ( i = 1; i <= n+1-j; i++ )
    {
      c[n-i] = c[n-i] - x[n+1-i-j] * c[n-i+1];
    }
  }
  return c;
}
//****************************************************************************80

bool s_eqi ( char *s1, char *s2 )

//****************************************************************************80
//
//  Purpose:
//
//    S_EQI reports whether two strings are equal, ignoring case.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    05 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S1, char *S2, pointers to two strings.
//
//    Output, bool S_EQI, is true if the strings are equal. 
//
{
  int i;
  int nchar;
  int nchar1;
  int nchar2;

  nchar1 = strlen ( s1 );
  nchar2 = strlen ( s2 );
  nchar = i4_min ( nchar1, nchar2 );

//
//  The strings are not equal if they differ over their common length.
//
  for ( i = 0; i < nchar; i++ ) 
  {

    if ( ch_cap ( s1[i] ) != ch_cap ( s2[i] ) ) 
    {
      return false;
    }
  }
//
//  The strings are not equal if the longer one includes nonblanks
//  in the tail.
//
  if ( nchar < nchar1 ) 
  {
    for ( i = nchar; i < nchar1; i++ ) 
    {
      if ( s1[i] != ' ' ) 
      {
        return false;
      }
    } 
  }
  else if ( nchar < nchar2 ) 
  {
    for ( i = nchar; i < nchar2; i++ )
    {
      if ( s2[i] != ' ' ) 
      {
        return false;
      }
    } 
  }

  return true;
}
//****************************************************************************80

int s_len_trim ( char *s )

//****************************************************************************80
//
//  Purpose:
//
//    S_LEN_TRIM returns the length of a string to the last nonblank.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    26 April 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S, a pointer to a string.
//
//    Output, int S_LEN_TRIM, the length of the string to the last nonblank.
//    If S_LEN_TRIM is 0, then the string is entirely blank.
//
{
  int n;
  char *t;

  n = strlen ( s );
  t = s + strlen ( s ) - 1;

  while ( 0 < n ) 
  {
    if ( *t != ' ' )
    {
      return n;
    }
    t--;
    n--;
  }

  return n;
}
//****************************************************************************80

int s_to_i4 ( char *s, int *last, bool *error )

//****************************************************************************80
//
//  Purpose:
//
//    S_TO_I4 reads an integer value from a string.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    13 June 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S, a string to be examined.
//
//    Output, int *LAST, the last character of S used to make IVAL.
//
//    Output, bool *ERROR is TRUE if an error occurred.
//
//    Output, int *S_TO_I4, the integer value read from the string.
//    If the string is blank, then IVAL will be returned 0.
//
{
  char c;
  int i;
  int isgn;
  int istate;
  int ival;

  *error = false;
  istate = 0;
  isgn = 1;
  i = 0;
  ival = 0;

  while ( *s ) 
  {
    c = s[i];
    i = i + 1;
//
//  Haven't read anything.
//
    if ( istate == 0 )
    {
      if ( c == ' ' )
      {
      }
      else if ( c == '-' )
      {
        istate = 1;
        isgn = -1;
      }
      else if ( c == '+' )
      {
        istate = 1;
        isgn = + 1;
      }
      else if ( '0' <= c && c <= '9' )
      {
        istate = 2;
        ival = c - '0';
      }
      else
      {
        *error = true;
        return ival;
      }
    }
//
//  Have read the sign, expecting digits.
//
    else if ( istate == 1 )
    {
      if ( c == ' ' )
      {
      }
      else if ( '0' <= c && c <= '9' )
      {
        istate = 2;
        ival = c - '0';
      }
      else
      {
        *error = true;
        return ival;
      }
    }
//
//  Have read at least one digit, expecting more.
//
    else if ( istate == 2 )
    {
      if ( '0' <= c && c <= '9' )
      {
        ival = 10 * (ival) + c - '0';
      }
      else
      {
        ival = isgn * ival;
        *last = i - 1;
        return ival;
      }

    }
  }
//
//
  {
  }
  {