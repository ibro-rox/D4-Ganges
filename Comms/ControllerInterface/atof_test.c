// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
// int decimal_to_binary(int n)
// {
// 	int remainder;
// 	int binary = 0, i = 1;
//    while(n != 0) 
//    {
// 	    remainder = n%2;
// 	    n = n/2;
// 	    binary= binary + (remainder*i);
// 	    i = i*10;
//     }
//     return binary;
// }
// float float_8bit(float ki)
// {
// 	double fractional, integer;

//     fractional = modf(ki, &integer);
//     printf ("Floating: %g\nInteger: %g\nFractional: %g", ki, integer, fractional);
//     // now conver the int part to binary
//     printf("\n binary value for %g is %d \n",integer,decimal_to_binary(integer));

//     double f1,f2,f3;
//     double frac_1,frac_2,f
//     f1 = fractional*2;
//     //printf("\nf1 = %g \n", f1);


// }
// int main() 
// {
//     char s[6] ="2.625" ; 
//     float f;
//     f = atof(s);
//     //int ten_bit;
//     float_8bit(f);
//     //ten_bit = (int)f*(1023/10.23);
//     //printf("float value : %4.8f\n" ,f); 
//     //printf("decimal to convert to 10-bit value: %d \n",ten_bit);
//     return 0; 
// }
#include <stdio.h>
#include <conio.h>

int main()
{
    int  integral, binaryInt = 0, i = 1;
    float  binaryFract = 0, k =0.1f, fractional, temp1, binaryTotal, f;

    printf("***** Convert float to binary *******\n");
    printf("\nEnter float value : ");
    scanf("%f",&f);

    //Separating the integral value from the floating point variable
    integral = (int)f;

    //Separating the fractional value from the variable
    fractional = f - (int)f;

    //Loop for converting decimal to binary
    while(integral>0)
    {
        binaryInt = binaryInt + integral % 2 * i;
        i = i * 10;
        integral = integral / 2;
    }

    //Loop for converting Fractional value to binary
    while(k>0.00000001)
    {
        temp1 = fractional *2;
        binaryFract = binaryFract+((int)temp1)*k;
        fractional = temp1 - (int)temp1;
        k = k / 10;
    }

    //Combining both the integral and fractional binary value.
    binaryTotal = binaryInt +binaryFract;
    printf(" \nbinary equivalent = %lf\n\n\n\n\n", binaryTotal);
}