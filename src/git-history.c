#include <cfile.h>
#include <cstringlist.h>
#include <cprocess.h>
#include <libapp.h>
#include <libpath.h>
#include <sys/stat.h>
#include <ctype.h>
#include <print.h>

int write_diff(const char *outdir, const char *fname, const char *commit)
{
    CStringAuto *filepath = cstr_new_size(64);
    path_join(filepath, outdir, fname);
    cstr_append(filepath, ".diff");

    print(c_str(filepath));
    //print(fname);

    CStringAuto *cmd = cstr_new_size(96);
    cstr_fmt(cmd, "git diff %s~ %s", commit, commit);

    CProcessAuto *process = cprocess_new();
    if (!cprocess_start(process, c_str(cmd), CP_PIPEOUT))
    {
        print("CProcess : start failed");

        return -1;
    }

    int status = cprocess_exitstatus(process);

    if (status != 0)
    {
        print("program returned : %d", status);

        return -1;
    }

    const char *result = c_str(cprocess_outbuff(process));

    CStringAuto *outpath = cstr_new(c_str(filepath));

    CFileAuto *outfile = cfile_new();
    if (!cfile_open(outfile, c_str(outpath), "wb"))
    {
        print("write diff file failed");
        return -1;
    }

    cfile_write(outfile, result);
    cfile_flush(outfile);

    return 0;
}

int get_history(const char *outdir, const char *from, const char *exclude)
{
    CStringAuto *cmd = cstr_new_size(96);
    cstr_copy(cmd, "git log --date=format:'%Y-%m-%d' --pretty='%cd %H %s'");

    if (from != NULL)
    {
        cstr_append(cmd, " --since=");
        cstr_append(cmd, from);
    }

    if (!dir_exists(outdir) && mkdir(outdir, 0755) != 0)
    {
        print("create dir error");

        return -1;
    }

    CProcessAuto *process = cprocess_new();
    if (!cprocess_start(process, c_str(cmd), CP_PIPEOUT))
    {
        print("start failed");

        return -1;
    }

    int status = cprocess_exitstatus(process);

    if (status != 0)
    {
        print("program returned : %d", status);

        return -1;
    }

    const char *result = c_str(cprocess_outbuff(process));
    CStringAuto *line = cstr_new_size(32);
    CStringListAuto *list = cstrlist_new_size(128);

    while (file_getline(&result, line))
    {
        if (cstr_contains(line, exclude, false))
            continue;

        cstrlist_append_len(list, c_str(line), cstr_size(line));
    }

    CStringAuto *outpath = cstr_new_size(32);
    path_join(outpath, outdir, "history.txt");

    CFileAuto *outfile = cfile_new();
    if (!cfile_open(outfile, c_str(outpath), "wb"))
        return -1;

    CStringAuto *commit = cstr_new_size(41);
    CStringAuto *tempfmt = cstr_new_size(64);
    CStringAuto *fname = cstr_new_size(64);

    int count = 0;
    int size = cstrlist_size(list);
    for (int i = size - 1; i > -1; --i)
    {
        const char *str = c_str(cstrlist_at(list, i));
        cfile_write(outfile, str);
        cfile_write(outfile, "\n");

        if (strlen(str) > 51)
        {
            cstr_copy_len(commit, str + 11, 40);

            cstr_fmt(tempfmt, "%04d-%s", ++count, str + 52);

            if (strlen(c_str(tempfmt)) > 48)
                cstr_terminate(tempfmt, 48);

            cstr_clear(fname);
            int nsize = cstr_size(tempfmt);
            for (int j = 0; j < nsize; ++j)
            {
                char c = cstr_at(tempfmt, j);

                if (c == ' ')
                    c = '_';

                if (isalnum(c) || c == '-' || c == '_')
                    cstr_append_c(fname, c);
            }

            cstr_append_c(fname, '\0');

            write_diff(outdir, c_str(fname), c_str(commit));
        }
    }

    return 0;
}

int main()
{
    //CStringAuto *userdirs = cstr_new_size(32);
    //get_configdir(userdirs);
    //cstr_append(userdirs, "/user-dirs.dirs");

    //print(c_str(userdirs));

    //return 0;

    const char *outdir = "/home/hotnuma/Downloads/thunar-history/";

    get_history(outdir, "2021-03-21", "I18n: Update translation");

    return 0;
}


