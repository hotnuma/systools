#include <cprocess.h>
#include <cfile.h>
#include <cstringlist.h>
#include <libpath.h>
#include <libstr.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <print.h>

#define APPNAME "git-history"

typedef enum
{
    REP_UNKNOWN = 0,
    REP_GITHUB,
    REP_GITLAB,

} RepType;

CString* get_url(const char *repdir);
bool get_history(const char *localdir, const char *range, bool reverse,
                 CStringList *filters);
bool filter_comment(CStringList *filters, const char *comment);
void get_comment(CString *result, const char *comment,
                 RepType type, const char *url);

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
    printf("%s -range \"<range>\" -url \"<url>\" local/dir\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

bool get_history(const char *localdir, const char *range, bool reverse,
                 CStringList *filters)
{
    bool ret = false;

    CString *url = get_url(localdir);

    if (!url)
    {
        printf("reading url failed\n");

        return ret;
    }

    if (chdir(localdir) != 0)
    {
        printf("chdir error\n");

        cstr_free(url);

        return ret;
    }

    CStringAuto *cmd = cstr_new_size(96);
    cstr_copy(cmd, "git log");

    cstr_append(cmd, " --date=format:'%Y-%m-%d'");
    cstr_append(cmd, " --pretty='%cd %H %s'");

    if (reverse)
        cstr_append(cmd, " --reverse");

    if (range && *range != '\0')
    {
        cstr_append(cmd, " ");
        cstr_append(cmd, range);
    }

    CStringAuto *line = cstr_new_size(1024);
    CStringAuto *cmlink = cstr_new_size(512);

    CProcessAuto *process = cprocess_new();
    if (!cprocess_start(process, c_str(cmd), CP_PIPEOUT))
    {
        print("start failed");

        goto out;
    }

    int status = cprocess_exitstatus(process);

    if (status != 0)
    {
        print("program returned : %d", status);

        goto out;
    }

    const char *result = c_str(cprocess_outbuff(process));

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
    RepType type = REP_UNKNOWN;
    const char *urlstr = c_str(url);

    if (strstr(urlstr, "github") != NULL)
        type = REP_GITHUB;
    else if (strstr(urlstr, "gitlab") != NULL)
        type = REP_GITLAB;

    while (file_getline(&result, line))
    {
        if (cstr_size(line) < 53)
            continue;

        char *p = cstr_data(line);
        p[10] = '\0';
        char *hash = p+11;
        p[51] = '\0';
        char *comment = p+52;

        if (filter_comment(filters, comment))
            continue;

        print("      <tr>");
        print("        <td>%d</td>", count);
        print("        <td>%s</td>", p);

        if (type == REP_GITHUB)
        {
            print("        <td><a href=\"%s/commit/%s\">", urlstr, hash);
            hash[8] = '\0';
            print("        %s</a></td>", hash);

            print("        <td>");
            get_comment(cmlink, comment, type, urlstr);
            print("        %s", c_str(cmlink));
            print("        </td>");
        }
        else if (type == REP_GITLAB)
        {
            print("        <td><a href=\"%s/-/commit/%s\">", urlstr, hash);
            hash[8] = '\0';
            print("        %s</a></td>", hash);

            print("        <td>");
            get_comment(cmlink, comment, type, urlstr);
            print("        %s", c_str(cmlink));
            print("        </td>");
        }
        else
        {
            hash[8] = '\0';
            print("        <td>%s</td>", hash);
            print("        <td>%s</td>", comment);
        }

        print("      </tr>\n");

        //print("%s", c_str(line));

        ++count;
    }

    print("    </table>");
    print("  </body>");
    print("</html>");

    ret = true;

out:
    cstr_free(url);

    return ret;
}

bool filter_comment(CStringList *filters, const char *comment)
{
    if (!filters)
        return false;

    int size = cstrlist_size(filters);

    for (int i = 0; i < size; ++i)
    {
        if (strstr(comment, cstrlist_at_str(filters, i)) != 0)
            return true;
    }

    return false;
}

void get_comment(CString *result, const char *comment,
                 RepType type, const char *url)
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

            if (type == REP_GITHUB)
            {
                cstr_fmt(link,
                    "<a href=\""
                    "%s/pull/%d\">", url, val);
            }
            else
            {
                cstr_fmt(link,
                    "<a href=\""
                    "%s/-/issues/%d\">", url, val);
            }

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

CString* get_url(const char *repdir)
{
    if (!repdir)
        return NULL;

    CStringAuto *repconf = cstr_new_size(64);
    cstr_fmt(repconf, "%s/.git/config", repdir);

    if (!file_exists(c_str(repconf)))
        return NULL;

    CStringAuto *buffer = cstr_new_size(512);

    if (!file_read(buffer, c_str(repconf)))
        return NULL;

    char *ptr = c_str(buffer);
    char *result;
    int length;

    while (str_getlineptr(&ptr, &result, &length))
    {
        result[length] = '\0';
        //print(result);

        char *p = strstr(result, "url = ");
        if (!p)
            continue;

        p += 6;

        if (*p == '\0')
            return NULL;

        CString *url = cstr_new(p);
        if (cstr_endswith(url, ".git", true))
            cstr_chop(url, 4);

        //print(c_str(url));

        return url;
    }

    return NULL;
}

int main(int argc, const char **argv)
{
    setbuf(stdout, NULL);

    if (argc < 2)
        usage_exit();

    const char *opt_repdir = NULL;
    const char *opt_range = NULL;
    bool opt_reverse = false;
    CStringListAuto *opt_filters = cstrlist_new_size(12);

    int n = 1;

    while (n < argc)
    {
        if (strcmp(argv[n], "-range") == 0)
        {
            if (++n >= argc)
                usage_exit();

            opt_range = argv[n];
        }
        else if (strcmp(argv[n], "-reverse") == 0)
        {
            opt_reverse = true;
        }
        else if (strcmp(argv[n], "-excl") == 0)
        {
            if (++n >= argc)
                usage_exit();

            if (strlen(argv[n]) >0)
                cstrlist_append(opt_filters, argv[n]);
        }
        else
        {
            opt_repdir = argv[n];
        }

        ++n;
    }

    if (!dir_exists(opt_repdir))
        error_exit("invalid directory");

    get_history(opt_repdir, opt_range, opt_reverse, opt_filters);

    return EXIT_SUCCESS;
}


