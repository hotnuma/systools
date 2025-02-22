#include <cfile.h>
#include <libstr.h>
#include <string.h>
#include <print.h>

// cbuild restyle restyle.c tinyc && sudo cp ./restyle /usr/local/bin/

static void usage_exit()
{
    printf("*** usage :\n");
    printf("restyle file.c\n");
    printf("abort...\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usage_exit();

    const char *filepath = argv[1];

    CFileAuto *file = cfile_new();
    if (!cfile_read(file, filepath))
        return EXIT_FAILURE;

    if (!cfile_open(file, filepath, "wb"))
        return EXIT_FAILURE;

    CStringAuto *line = cstr_new_size(32);
    while (cfile_getline(file, line))
    {
        char *str = cstr_data(line);
        int len = cstr_size(line);

        if (len > 3 && cstr_endswith(line, "*/", true))
        {
            char *start = strstr(str, "/*");
            char *mid = strstr(str, "*/");
            char *end = strrstr(str, "*/");

            if (start && mid && end
                && end[2] == '\0' && end > start + 1
                && mid == end)
            {
                start[0] = '/';
                start[1] = '/';

                cstr_chop(line, 2);
                while (cstr_last(line) == ' ')
                    cstr_chop(line, 1);

                str = cstr_data(line);
            }
        }

        cfile_write(file, str);
        cfile_write(file, "\n");
    }

    return EXIT_SUCCESS;
}


