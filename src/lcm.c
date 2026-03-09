#include <cinifile.h>
#include <cfile.h>
#include <libapp.h>
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

static void _config_get_path(CString *result)
{
    get_homedir(result);
    cstr_append(result, "/.config/lcm.conf");
}

static bool config_read(double *result)
{
    CStringAuto *inipath = cstr_new_size(64);
    _config_get_path(inipath);

    CIniFileAuto *inifile = cinifile_new();

    if (!cinifile_read(inifile, c_str(inipath)))
        return false;

    CIniSection *section = cinifile_section(inifile, "LCM");
    if (!section)
        return false;

    CStringAuto *temp = cstr_new_size(12);
    if (!cinisection_key_value(section, temp, "cal", ""))
        return false;

    double val = strtod(c_str(temp), NULL);
    if (val == 0)
        return false;
    
    *result = val;

    return true;
}

static bool config_save(double value)
{
    CStringAuto *inipath = cstr_new_size(64);
    _config_get_path(inipath);
    
    CFileAuto *file = cfile_new();

    if (!cfile_open(file, c_str(inipath), "wb"))
        return false;

    CStringAuto *temp = cstr_new_size(32);
    cfile_write(file, "[LCM]\n");
    cstr_fmt(temp, "cal=%ld\n", value);
    cfile_write(file, c_str(temp));
    cfile_write(file, "\n");

    return true;
}

static void print_result(char *fname, double result)
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
    
    config_save(result);
    
    return EXIT_SUCCESS;
}

int func_c(int argc, char **argv, double C1)
{
    if (argc != 4)
        usage_exit();
    
    double f1 = strtod(argv[2], NULL);
    double f3 = strtod(argv[3], NULL);
    double result = C1*((pow(f1, 2)/pow(f3, 2)) - 1);
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int func_l(int argc, char **argv, double C1)
{
    (void) argv;
    (void) C1;
    
    if (argc != 4)
        usage_exit();
    
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    double C1 = 1.076879e-09;
    
    config_read(&C1);

    if (argc < 2)
        usage_exit();

    if (strcmp(argv[1], "cal") == 0)
        return func_cal(argc, argv);
    
    if (strcmp(argv[1], "c") == 0)
        return func_c(argc, argv, C1);
    
    if (strcmp(argv[1], "l") == 0)
        return func_l(argc, argv, C1);
    
    return EXIT_FAILURE;
}

