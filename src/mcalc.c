#include <cinifile.h>
#include <cfile.h>
#include <libapp.h>
#include <math.h>

#define APPNAME "mcalc"

typedef struct _MCalc
{
    double C1;
    double L1;

} MCalc;

static void usage_exit()
{
    printf("*** usage :\n");
    printf("Calibration : %s cal f1 f2 cref\n", APPNAME);
    printf("C           : %s c f1 f2\n", APPNAME);
    printf("L           : %s l f1 f2\n", APPNAME);
    printf("RC          : %s rc R C\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

static void _mcalc_get_path(CString *result)
{
    get_homedir(result);
    cstr_append(result, "/.config/mmcalc.conf");
}

static void _read_key(CIniSection *section, const char *key, double *result)
{
    CStringAuto *temp = cstr_new_size(12);
    
    if (!cinisection_key_value(section, temp, key, ""))
        return;
    
    double value = strtod(c_str(temp), NULL);
    if (value > 0)
        *result = value;
}

static bool mcalc_read(MCalc *mcalc)
{
    CStringAuto *inipath = cstr_new_size(64);
    _mcalc_get_path(inipath);

    CIniFileAuto *inifile = cinifile_new();

    if (!cinifile_read(inifile, c_str(inipath)))
        return false;

    CIniSection *section = cinifile_section(inifile, "MCalc");
    if (!section)
        return false;

    _read_key(section, "C1", &(mcalc->C1));
    _read_key(section, "L1", &(mcalc->L1));
    
    // add bridge resistors
    //~ _read_key(section, "C1", &(mcalc->C1));
    //~ _read_key(section, "C1", &(mcalc->C1));
    //~ _read_key(section, "C1", &(mcalc->C1));
    //~ _read_key(section, "C1", &(mcalc->C1));

    return true;
}

static bool mcalc_save(MCalc *mcalc)
{
    CStringAuto *inipath = cstr_new_size(64);
    _mcalc_get_path(inipath);
    
    CFileAuto *file = cfile_new();

    if (!cfile_open(file, c_str(inipath), "wb"))
        return false;

    CStringAuto *temp = cstr_new_size(32);
    cfile_write(file, "[MCalc]\n");
    cstr_fmt(temp, "C1=%.6e\n", mcalc->C1);
    cfile_write(file, c_str(temp));
    cstr_fmt(temp, "L1=%.6e\n", mcalc->L1);
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

int mcalc_cal(MCalc *mcalc, int argc, char **argv)
{
    // f1 f2 Cref
    
    if (argc != 5)
        usage_exit();
    
    double f1 = strtod(argv[2], NULL);
    double f2 = strtod(argv[3], NULL);
    double f1_2 = pow(f1, 2);
    double f2_2 = pow(f2, 2);
    double Cref = strtod(argv[4], NULL);
    
    mcalc->C1 = Cref*f2_2/(f1_2-f2_2);
    mcalc->L1 = 1/(4*pow(M_PI, 2)*f1_2*mcalc->C1);

    print_result("C1", mcalc->C1);
    print_result("L1", mcalc->L1);
    
    mcalc_save(mcalc);
    
    return EXIT_SUCCESS;
}

int mcalc_c(MCalc *mcalc, int argc, char **argv)
{
    if (argc != 4)
        usage_exit();
    
    double f1 = strtod(argv[2], NULL);
    double f3 = strtod(argv[3], NULL);
    double result = mcalc->C1*((pow(f1, 2)/pow(f3, 2)) - 1);
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int mcalc_l(MCalc *mcalc, int argc, char **argv)
{
    if (argc != 4)
        usage_exit();
    
    double f1 = strtod(argv[2], NULL);
    double f3 = strtod(argv[3], NULL);
    double result = mcalc->L1*((pow(f1, 2)/pow(f3, 2)) - 1);
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int mcalc_rc(MCalc *mcalc, int argc, char **argv)
{
    (void) mcalc;
    
    if (argc != 4)
        usage_exit();
    
    double R = strtod(argv[2], NULL);
    double C = strtod(argv[3], NULL);
    double result = 1/(2*M_PI*R*C);
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int mcalc_ip(MCalc *mcalc, int argc, char **argv)
{
    (void) mcalc;
    
    if (argc != 4)
        usage_exit();
    
    double v1 = strtod(argv[2], NULL);
    double v2 = strtod(argv[3], NULL);
    
    if (v1 == 0)
        return EXIT_FAILURE;
    
    double r = v2 / v1;
    double result = (r - 1.0) * 100.0;
    
    print_result(argv[1], result);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    MCalc mcalc;
    mcalc.C1 = 1.076879e-09;
    mcalc.L1 = 100e-6;
    
    mcalc_read(&mcalc);

    if (argc < 2)
        usage_exit();

    if (strcmp(argv[1], "cal") == 0)
        return mcalc_cal(&mcalc, argc, argv);
    
    if (strcmp(argv[1], "c") == 0)
        return mcalc_c(&mcalc, argc, argv);
    
    if (strcmp(argv[1], "l") == 0)
        return mcalc_l(&mcalc, argc, argv);
    
    if (strcmp(argv[1], "rc") == 0)
        return mcalc_rc(&mcalc, argc, argv);
    
    if (strcmp(argv[1], "ip") == 0)
        return mcalc_ip(&mcalc, argc, argv);
    
    return EXIT_FAILURE;
}

