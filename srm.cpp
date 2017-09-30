/**
 * @file saferm.cpp
 * @brief same rm by mv
 * @author JiaoLinfeng
 * @date 06/23/2017
 */

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

const int MAX_BUF_LEN = 500;
const int MAX_CMD_LEN = 1000;

/**
 * @brief 输出用法
 */
void print_usage();

/**
 * @brief 根据path递归创建文件夹
 *
 * @param path 文件夹路径
 *
 * @return 0 表示创建成功 -1 表示创建失败
 */
int createDir(char* path);

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, ":hv")) != -1)
    {
        switch (opt)
        {
        case 'h':
        case 'v':
            print_usage();
            return 0;
        case ':':
        default:
            print_usage();
            return 1;
        }

    }
    //validate arguments
    if (optind >= argc)
    {
        fprintf(stderr, "Error: No Files to Delete\n");
        return 1;
    }

    //create folder if target folder not exists
    time_t now = time(NULL);
    struct tm *p_tm = localtime(&now);
    char path[MAX_BUF_LEN];
    char *target_path = getenv("SRM_PATH");
    if (target_path != NULL) // if SRM_PATH exists
    {
        snprintf(path, MAX_BUF_LEN, "%s/%04d%02d%02d", target_path,
            p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday);
    }
    else // else move files into $HOME/garbage
    {
        snprintf(path, MAX_BUF_LEN, "%s/garbage/%04d%02d%02d", getenv("HOME"),
            p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday);
    }
    int ret = createDir(path);
    if (ret)
    {
        fprintf(stderr, "Error: Create Target Folder Failed\n");
        return 1;
    }
    // mv files into target folder
    char buf[MAX_BUF_LEN];
    char cmd[MAX_CMD_LEN];
    for (; optind < argc; ++optind)
    {
        char *file = argv[optind];
        size_t len = strlen(file);
        //if file ends with /, remove /
        if (file[len - 1] == '/')
        {
            file[len - 1] = '\0';
            --len;
        }
        // remove file prefix
        int pos = len - 1;
        while (pos >= 0)
        {
            if (file[pos] == '/')
                break;
            --pos;
        }
        if (pos >= 0)
        {
            file += pos + 1;
            len -= pos + 1;
        }
        // generate target file name
        snprintf(buf, MAX_BUF_LEN, "%s/%s", path, file);
        if (!access(buf, F_OK)) // if the target file exists, rename it
        {
            size_t pos = strcspn(file, ".");
            if (pos >= len)
            {
                snprintf(buf, MAX_BUF_LEN, "%s/%s_%02d%02d%02d", path, file,
                    p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
            }
            else
            {
                file[pos] = '\0';
                snprintf(buf, MAX_BUF_LEN, "%s/%s_%02d%02d%02d.%s", path, file,
                    p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec, file + pos + 1);
                file[pos] = '.';
            }
        }
        // generate system command
        int ret = rename(argv[optind], buf);
        if (ret)
        {
            fprintf(stderr, "Delete %s failed: %s\n", file, strerror(errno));
            continue;
        }
    }
    return 0;
}

int createDir(char* path)
{
    size_t len = strlen(path);
    if (!len)
        return -1;
    if (*(path+ len - 1) != '/') // 如果路径不是以/结尾，给尾部填充/
    {
        *(path + len) = '/';
        *(path + (++len)) = '\0';
    }
    size_t pos = strcspn(path, "/");
    while (pos < len)
    {
        if (pos) // 如果是/开头，第一个是根目录，不用创建
        {
            *(path + pos) = '\0';
            int ret = mkdir(path, 0755);
            if (ret != 0 && errno != EEXIST)
            {
                return -1;
            }
            *(path + pos) = '/';
        }
        while (pos < len && *(path + pos) == '/')
            ++pos;
        if (pos < len)
        {
            size_t new_pos = strcspn(path + pos, "/");
            pos += new_pos;
        }
    }
    return 0;
}

void print_usage()
{
    printf("========================================================\n"
           "Saferm\n"
           "--------------------------------------------------------\n"
           "version: 1.0\n"
           "--------------------------------------------------------\n"
           "brief:\n"
           "srm is a safely delete tool based on rename system call.\n"
           "srm command moves the files and folders to be deleted\n"
           "into dustbin instead of deleting from disk directly.\n"
           "--------------------------------------------------------\n"
           "usage:\n"
           "srm [-h] [-v]\n"
           "srm file1 file2 ...\n"
           "========================================================\n");
}

