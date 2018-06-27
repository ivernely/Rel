#include "utils.h"
#include <cassert>

#include "QColor"
#include "QPainter"
#include "QSvgGenerator"

#define LGM_LIM         7

LOCAL const double zero = 0.0;
LOCAL const double one = 1.0;

//---------------------------------------------------------
double Max(QVector<double> &vect)
{
    double max;

    max = vect.at(0);
    for(int i=0; i<vect.size(); i++)
        if(max < vect.at(i))
            max = vect.at(i);

    return max;
}
//---------------------------------------------------------
double Min(QVector<double> &vect)
{
    double min;

    min = vect.at(0);
    for(int i=0; i<vect.size(); i++)
        if(min > vect.at(i))
            min = vect.at(i);

    return min;
}
//---------------------------------------------------------
ENTRY double studentDF(double &n, double &x)
{
   assert(n > 0);

   if (x == 0.0)
      return 0.5;
   BetaDF b=BetaDF(0.5 * n, 0.5);
   double z = 0.5 * b.value(n / (n + x * x));
   return (x > 0.0) ? 1.0 - z : z;
}/*studentDF*/
//-----------------------------------------------------------------
ENTRY double inv_studentDF(double &n, double &p)
{
   if (p == 0.5) return 0.0;
   double z = 1.0 - 2.0 * p;
   BetaDF b=BetaDF(0.5, 0.5*n);
   //z = b.inv(fabs(z));
   z = b.inv(fabs(z));
   double q = sqrt(n*z/(1.0-z));
   return (p < 0.5) ? -q : q;
}
//-----------------------------------------------------------------
BetaDF::BetaDF(double u, double w):
   a(u), b(w), logBeta(logGamma(a) + logGamma(b) - logGamma(a + b))
{
   assert(a > 0 && b > 0);
}
//----------------------------------------------------------------------
double BetaDF::fraction(double a, double b, double x)
{
   double old_bta, factor = 1;
   double A0 = 0, A1 = 1, B0 = 1, B1 = 1;
   double bta = 1, am = a, ai = a;
   double iter = 0, r;

   do {
   // часть цикла, вычисл¤юща¤ нечетные подход¤щие дроби
   // начинаем с i = 1, iter = 0
      ai += 1;              // = a+i
      r = -am * (am + b) * x / ((ai - 1) * ai);
      //пересчет A и B в два шага
      A0 = (A1 + r * A0) * factor;  // i Ќ≈четно
      B0 = (B1 + r * B0) * factor;
   // часть цикла, вычисл¤юща¤ нечетные подход¤щие дроби
   // начинаем с i = 2, iter = 1
      am += 1;
      iter += 1;
      ai += 1;
      r = iter * (b - iter) * x * factor / ((ai - 1) * ai);
      A1 = A0 + r * A1;     // i четно, A0 и B0 уже шкалированы
      B1 = B0 + r * B1;
      old_bta = bta;
      factor = 1 / B1;
      bta = A1 * factor;
   } while (fabs(old_bta) != fabs(bta));
   return bta * exp(a * log(x) + b * log(1 - x) - logBeta) / a;
}/*incBeta_fraction*/
//----------------------------------------------------------------------
double BetaDF::value(double x)
{
   if (x <= 0)
      return 0;             // Ќ≈ ошибка!
   else if (x >= 1)
      return 1;             // Ќ≈ ошибка!
   if (x < (a + 1) / (a + b + 2))
      return fraction(a, b, x);
   else
      return 1 - fraction(b, a, 1 - x);
}/*value*/
//----------------------------------------------------------------------
double BetaDF::inv(const double &p)
{
   double fx, l = 0, r = 1, x = 0.5;

   assert(p >= 0 && p <= 1);
   if (p == 0 || p == 1) return p;

   do {
      fx = value(x);
      if (fx > p) r = x; else
      if (fx < p) l = x; else
         return x;
      x = (l + r)* 0.5;
   } while ((l!=x) && (r!=x));
   return x;
}/*inv*/
//----------------------------------------------------------------------
ENTRY double logGamma(double x)
{
   long double static c[20] =
   {
      /* Asymtotic expansion coefficients             */
      1.0 / 12.0, -1.0 / 360.0, 1.0 / 1260.0, -1.0 / 1680.0, 1.0 / 1188.0,
      -691.0 / 360360.0, 1.0 / 156.0, -3617.0 / 122400.0, 43867.0 / 244188.0,
      -174611.0 / 125400.0, 77683.0 / 5796.0, -236364091.0 / 1506960.0,
      657931.0 / 300.0, -3392780147.0 / 93960.0, 1723168255201.0 / 2492028.0,
      -7709321041217.0 / 505920.0, 151628697551.0 / 396.0,
      -26315271553053477373.0 / 2418179400.0, 154210205991661.0 / 444.0,
      - 261082718496449122051.0 / 21106800.0
   };


   double x2, presum, sum, den, z;
   int  i;

   assert(x > 0);                      /* Negative argument: Error!         */

   if (x == 1 || x == 2)
      return 0;

   for (z = 0; x < LGM_LIM; x += 1)    /* Increase argument if necessary.   */
      z += log(x);

   den = x;
   x2 = x * x;                         /* Compute the asymptotic expansion  */
   presum = (x - 0.5) * log(x) - x + 0.9189385332046727417803297364;
   for (i = 0; i < 20; i++) {
      sum = presum + c[i] / den;
      if (sum == presum) break;
      den = den * x2;
      presum = sum;
   }
   return sum - z;                     /* Fit the increased argument if any  */

}/*logGamma*/
//----------------------------------------------------------------------
ENTRY double chi2DF(double f, double x)
{
   assert(f > 0);
   return GammaDF(f/2.0).value(x/2.0);
}/*chi2DF*/
//----------------------------------------------------------------------
ENTRY double inv_chi2DF(double f, double q)
{
   assert(f > 0);
   return GammaDF(f/2.0).inv(q);
}/*inv_chi2DF*/
//----------------------------------------------------------------------
GammaDF::GammaDF(double shape, double scale):
    a(shape), shape(shape), scale(scale), lga(logGamma(shape))
{
  assert(shape > 0 && scale > 0);
}
//----------------------------------------------------------------------
double GammaDF::series(const double &x)
{
   double sum, prev_sum, term, aa = a;

   term = sum = one / a;
   do {
      aa += one;
      term *= x / aa;
      prev_sum = sum; sum += term;
   } while(fabs(prev_sum) != fabs(sum));
   return sum *= exp(-x + a * log(x) - lga);
}/* incGamma_series */
//----------------------------------------------------------------------
double GammaDF::fraction(const double &x)
{
   double old_sum=zero, factor=one;
   double A0=zero, A1=one, B0=one, B1=x;
   double sum=one/x, z=zero, ma=zero-a, rfact;

   do {
      z += one;
      ma += one;
      /* two steps of recurrence replace A's & B's */
      A0 = (A1 + ma * A0) * factor;	/* i even */
      B0 = (B1 + ma * B0) * factor;
      rfact = z * factor;
      A1 = x * A0 + rfact * A1;	/* i odd, A0 already rescaled */
      B1 = x * B0 + rfact * B1;
      if (B1) {
     factor = one / B1;
     old_sum = sum;
     sum = A1 * factor;
      }/* if */
   } while (fabs(sum) != fabs(old_sum));
   return exp(-x + a * log(x) - lga) * sum;
}/*fraction*/
//----------------------------------------------------------------------
double GammaDF::value(double x)
{
   x *= scale;
   if(x <= zero)
      return zero;            /* Ќ≈ ошибка! */
   else if(x < (a + one))
      return series(x);
   else
      return one - fraction(x);
}/*value*/
//----------------------------------------------------------------------
double GammaDF::inv(const double &p)
{
   double fx, l, r, x;

   if (p == 0) return 0;
   assert(p > 0 && p < 1);

   for(l=0, r=a/2; value(r) < p; r+=a/2) l=r;
   x=(l+r)/2;
   do {
      fx = value(x);
      if (fx > p) r = x; else
      if (fx < p) l = x; else
         break;
      x = (l + r)* 0.5;
    } while ((l!=x) && (r!=x));
    return x;

}/*inv*/
//----------------------------------------------------------------------
ENTRY double normalDF(double x)
{
   double dfg;
   if (x == zero)
      return 0.5;
   dfg = GammaDF(0.5).value(x*x/2.0)/2;
   return 0.5 + (x > zero ? dfg : -dfg);
}/*normalDF*/
//----------------------------------------------------------------------
ENTRY double inv_normalDF(const double &level)
{
   double q, x=0, r;
   double p_low = 0.02425,
          p_high = 1 - p_low;

   /*алгоритм вычисл¤ет значение с погрешностью 1.15*10^(-9) */
   double a1 = -3.969683028665376e+01, a2 =  2.209460984245205e+02,
          a3 = -2.759285104469687e+02, a4 =  1.383577518672690e+02,
          a5 = -3.066479806614716e+01, a6 =  2.506628277459239e+00,

          b1 = -5.447609879822406e+01, b2 =  1.615858368580409e+02,
          b3 = -1.556989798598866e+02, b4 =  6.680131188771972e+01,
          b5 = -1.328068155288572e+01,

          c1 = -7.784894002430293e-03, c2 = -3.223964580411365e-01,
          c3 = -2.400758277161838e+00, c4 = -2.549732539343734e+00,
          c5 =  4.374664141464968e+00, c6 =  2.938163982698783e+00,

         d1 =  7.784695709041462e-03, d2 =  3.224671290700398e-01,
         d3 =  2.445134137142996e+00, d4 =  3.754408661907416e+00;

   //нижн¤¤ область
   if(level>0 && level<p_low)
   {
     q = sqrt(-2 * log(level));
     x = (((((c1*q+c2)*q+c3)*q+c4)*q+c5)*q+c6)/
            ((((d1*q+d2)*q+d3)*q+d4)*q+1);
   }

   //средн¤¤ область
   if(level>=p_low && level<=p_high)
   {
     q = level-0.5;
     r = q*q;
     x = (((((a1*r+a2)*r+a3)*r+a4)*r+a5)*r+a6)*q/
           (((((b1*r+b2)*r+b3)*r+b4)*r+b5)*r+1);
   }

   //верхн¤¤ область
   if(level>p_high && level<1)
   {
     q = sqrt(-2 * log(1-level));
     x = -(((((c1*q+c2)*q+c3)*q+c4)*q+c5)*q+c6)/
             ((((d1*q+d2)*q+d3)*q+d4)*q+1);
   }
   return x;
}/*inv_normalDF*/
//----------------------------------------------------------------------
//F-распределение
double FDF(double n1, double n2, double x)
{
   assert(x >= 0.0);
   return 1.0 - BetaDF(0.5 * n2, 0.5 * n1).value(n2 / (n2 + n1 * x));
}/*FDF*/
//----------------------------------------------------------------------
double inv_FDF(double n1, double n2, double p)
{
   double y=BetaDF(0.5 * n1, 0.5 * n2).inv(p);
   return n2*y/n1/(1-y);
}/*inv_FDF*/
//----------------------------------------------------------------------
//доделать сохранение в SVG
void saveAsSvg(QCustomPlot* widget, const QString &filename)
{
    QSvgGenerator generator;
    int W, H;

    W = widget->width();
    H = widget->height();

    if(filename.isEmpty())
        return;

    generator.setViewBox(QRect(0, 0, -1, -1));
    generator.setFileName(filename);
    generator.setSize(QSize(W,H));

    QCPPainter painter;
    painter.begin(&generator);
    painter.setRenderHint(QPainter::Antialiasing);
    widget->toPainter(&painter, W, H);
    painter.end();
}
//----------------------------------------------------------------------
