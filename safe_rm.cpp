/**
 * @file safe_rm.cpp
 * @brief rm safely
 * @author JiaoLinfeng
 * @date 07/15/2017
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include <string>

const int MAX_BUF_LEN = 1024;

void print_usage();

int main(int argc, char* argv[])
{
    int opt;
    const char* dest = "/tmp/garbge/";
    while ((opt = getopt(argc, argv, ":h")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage();
                exit(0);
                break;
            case ':':
            case '?':
            default:
                print_usage();
                exit(1);
                break;
        }
    }
    if (optind >= argc)
    {
        fprintf(stderr, "srm: no files to delete\n");
        exit(2);
    }
    //create folder if not exists
    int ret = mkdir(dest, 0755);
    if (ret && errno != EEXIST)
    {
        perror("srm");
        exit(3);
    }

    std::stringstream sin;
    sin << "mv";
    for (; optind < argc; ++optind)
    {
        sin << ' ' << argv[optind];
    }
    sin << ' ' << dest;

    FILE *fp = NULL;
    char buf[MAX_BUF_LEN];
    memset(buf, 0, sizeof(buf));
    fp = popen(sin.str().c_str(), "r");
    if (fp == NULL)
    {
        fprintf(stderr, "srm: delete failed\n");
        exit(4);
    }

    ret = fread(buf, 1, MAX_BUF_LEN, fp);
    if (ret > 0)
    {
        fprintf(stdout, "%s\n", buf);
    }
    pclose(fp);

    return 0;
}

void print_usage()
{
    fprintf(stdout, "srm -h\n"
                    "srm filename [filename2 filename3 ...]\n");
}
