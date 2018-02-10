#include <stdio.h>
#include <math.h>
/* This is the equation for the bisection method final = (x^2 +2)
    ignore t1 because it's just for a test case
    The entire program calculates the bisection method as follows
    taking the mid point from 2 points a and b by doing (b-a)/2 
    then the mid point is plugged into the function f(x) and if it's less than 0 
    a is set equal to the mid point, if it's greater than 0 then b is set equal to the mid point,
    if it is set equal to 0 then the loop is broken and mid is returned

*/
double f(double x)
{
    double final = (x*x) - 2;
    double t1 = (x-1)*(x-1)*(x-1);
    return final;
}
//is the actual bisection function that takes the calculations from f(x) and works with them
double bisection(double a, double b, double tol, int max_iters)
{
    double mid = 0;
    int iters = 0;
    double func = 0;
    while(iters<max_iters && fabs(b-a)>tol) {
       iters++;
       mid = (a + b) / 2;
       func = f(mid);
       if (func == 0) {
           break;

       } else if (func < 0) {
           a = mid;
       } else {
           b = mid;
       }

   }

   return mid;

}
//outputs the data to the console for input requests and output answers
int main(void) {
    double a;
    double b;
    double tol;
    int max_iters;
    printf("Please Enter a:\n");
    scanf("%lf",&a);
    printf("Please enter b:\n");
    scanf("%lf",&b);
    printf("Please Enter tol:\n");
    scanf("%lf",&tol);
    printf("Please Enter max iters:\n");
    scanf("%d",&max_iters);

    printf("c is equal to: \n");
    double c = bisection(a, b, tol, max_iters);
    printf("%f\n", c);
    return 0;
}
