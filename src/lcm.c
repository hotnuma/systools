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

static bool config_read(double *C1, double *L1)
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
    double value;
    
    if (!cinisection_key_value(section, temp, "C1", ""))
        return false;
    value = strtod(c_str(temp), NULL);
    if (value == 0)
        return false;
    *C1 = value;

    if (!cinisection_key_value(section, temp, "L1", ""))
        return false;
    value = strtod(c_str(temp), NULL);
    if (value == 0)
        return false;
    *L1 = value;

    return true;
}

static bool config_save(double C1, double L1)
{
    CStringAuto *inipath = cstr_new_size(64);
    _config_get_path(inipath);
    
    CFileAuto *file = cfile_new();

    if (!cfile_open(file, c_str(inipath), "wb"))
        return false;

    CStringAuto *temp = cstr_new_size(32);
    cfile_write(file, "[LCM]\n");
    cstr_fmt(temp, "C1=%ld\n", C1);
    cfile_write(file, c_str(temp));
    cstr_fmt(temp, "L1=%ld\n", L1);
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
    
    // external reference capacitor
    double Cref = 9.066e-9;
    
    double f1 = strtod(argv[2], NULL);
    double f2 = strtod(argv[3], NULL);
    double f1_2 = pow(f1, 2);
    double f2_2 = pow(f2, 2);
    double C1 = Cref*f2_2/(f1_2-f2_2);
    double L1 = 1/(4*pow(M_PI, 2)*f1_2*C1);

    print_result("C1", C1);
    print_result("L1", L1);
    
    config_save(C1, L1);
    
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
    double L1 = 100e-6;
    
    config_read(&C1, &L1);

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

