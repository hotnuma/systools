#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define APPNAME "lcr"

static void usage_exit()
{
    printf("*** usage :\n");
    printf("Calibration : %s cal f1 f2\n", APPNAME);
    printf("C           : %s c f1 f2\n", APPNAME);
    printf("L           : %s l f1 f2\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

void print_result(char *fname, double result)
{
    if (result > -1 && result < 1)
        printf("%s: %.6e\n", fname, result);
    else
        printf("%s: %lf\n", fname, result);
}

int func_cal(int argc, char **argv)
{
    if (argc != 4)
        usage_exit();
    
    double Cref = 9.066e-9;
    double f1 = strtod(argv[2], NULL);
    double f2 = strtod(argv[3], NULL);
    double temp = pow(f2, 2);
    double result = Cref*temp/(pow(f1, 2)-temp);

    print_result(argv[1], result);
    
    return EXIT_SUCCESS;
}

double C1 = 1.076879e-09;

int func_c(int argc, char **argv)
{
    if (argc != 4)
        usage_exit();
    
    double f1 = strtod(argv[2], NULL);
    double f3 = strtod(argv[3], NULL);
    double result = C1*((pow(f1, 2)/pow(f3, 2)) - 1);
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int func_l(int argc, char **argv)
{
    if (argc != 4)
        usage_exit();
    
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usage_exit();

    if (strcmp(argv[1], "cal") == 0)
        return func_cal(argc, argv);
    
    if (strcmp(argv[1], "c") == 0)
        return func_c(argc, argv);
    
    if (strcmp(argv[1], "l") == 0)
        return func_l(argc, argv);
    
    return EXIT_FAILURE;
}

