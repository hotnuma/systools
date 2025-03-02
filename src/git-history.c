#include <cprocess.h>
#include <cfile.h>
#include <libpath.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <print.h>

// cbuild git-history git-history.c tinyc

#define APPNAME "git-history"
#define REPOS "https://gitlab.xfce.org/xfce/thunar"

bool get_history(const char *localdir, const char *url, const char *from);
void get_comment(CString *result, const char *comment);

static void error_exit(const char *msg)
{
    if (!msg || *msg == '\0')
    {
        msg = "an error occurred";
    }

    printf("*** %s\nabort...\n", msg);

    exit(EXIT_FAILURE);
}

static void usage_exit()
{
    printf("*** usage :\n");
    printf("%s param\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

bool get_history(const char *localdir, const char *url, const char *range)
{
    if (chdir(localdir) != 0)
        error_exit("chdir error");

    CStringAuto *cmd = cstr_new_size(96);
    cstr_copy(cmd, "git log");

    cstr_append(cmd, " --date=format:'%Y-%m-%d'");
    cstr_append(cmd, " --pretty='%cd %H %s'");
    cstr_append(cmd, " --reverse");

    if (range && *range != '\0')
    {
        cstr_append(cmd, " ");
        cstr_append(cmd, range);
    }

    CProcessAuto *process = cprocess_new();
    if (!cprocess_start(process, c_str(cmd), CP_PIPEOUT))
    {
        print("start failed");

        return false;
    }

    int status = cprocess_exitstatus(process);

    if (status != 0)
    {
        print("program returned : %d", status);

        return false;
    }

    const char *result = c_str(cprocess_outbuff(process));
    CStringAuto *line = cstr_new_size(1024);
    CStringAuto *cmlink = cstr_new_size(512);

    print("<html>");
    print("  <head>");
    print("    <style>");
    print("      td {font-family: monospace;}");
    print("      table {");
    print("        border-collapse: collapse;");
    print("        width: 100%;");
    print("      }");
    print("      td, th {");
    print("        border: 1px solid #ddd;");
    print("        padding: 8px;");
    print("      }");
    print("      tr:nth-child(even){background-color: #f2f2f2;}");
    print("      tr:hover {background-color: #ddd;}}");
    print("    </style>");
    print("  </head>");
    print("  <body>");
    print("    <table>");

    print("      <tr>");
    print("        <th>N°</th>");
    print("        <th>Date</th>");
    print("        <th>Hash</th>");
    print("        <th>Comment</th>");
    print("      </tr>\n");

    int count = 1;

    while (file_getline(&result, line))
    {
        if (cstr_startswith(line, "I18n:", false))
            continue;

        if (cstr_size(line) < 53)
            continue;

        char *p = cstr_data(line);
        p[10] = '\0';
        char *hash = p+11;
        p[51] = '\0';
        char *comment = p+52;

        if (strncmp(comment, "I18n:", 5) == 0)
            continue;

        if (strcmp(comment, "Back to development") == 0)
            continue;

        print("      <tr>");
        print("        <td>%d</td>", count);
        print("        <td>%s</td>", p);

        print("        <td><a href=\"%s/-/commit/%s\">", url, hash);
        hash[8] = '\0';
        print("        %s</a></td>", hash);

        print("        <td>");
        get_comment(cmlink, comment);
        print("        %s", c_str(cmlink));
        print("        </td>");

        print("      </tr>\n");

        //print("%s", c_str(line));

        ++count;
    }

    print("    </table>");
    print("  </body>");
    print("</html>");

    return true;
}

void get_comment(CString *result, const char *comment)
{
    if (!comment || !result)
        return;

    cstr_clear(result);
    CString *link = cstr_new_size(128);

    const char *p = comment;
    while (*p)
    {
        if (*p == '#' && isdigit(*(p+1)))
        {
            ++p;
            int val = atoi(p);

            cstr_fmt(link,
                "<a href=\""
                "https://gitlab.xfce.org/xfce/thunar/-/issues/%d\">", val);
            cstr_append(result, c_str(link));
            cstr_append_c(result, '#');
            while (isdigit(*p))
            {
                cstr_append_c(result, *p);
                ++p;
            }
            cstr_append(result, "</a>");

            continue;
        }

        cstr_append_c(result, *p);

        ++p;
    }
}

int main(int argc, const char **argv)
{
    if (argc < 2)
        usage_exit();

    const char *opt_repdir = NULL;
    const char *opt_range = NULL;

    int n = 1;

    while (n < argc)
    {
        if (strcmp(argv[n], "-range") == 0)
        {
            if (++n >= argc)
                usage_exit();

            opt_range = argv[n];
        }
        else
        {
            opt_repdir = argv[n];
        }

        ++n;
    }

    if (!dir_exists(opt_repdir))
    {
        error_exit("invalid directory");
    }

    get_history(opt_repdir, REPOS, opt_range);

    return EXIT_SUCCESS;
}


