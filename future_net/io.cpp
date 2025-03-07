#include "stdafx.h"

#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
//#include <unistd.h>
#include <signal.h>

#define MAX_LINE_LEN 4000

#define INLINE  static __inline
#ifdef _DEBUG
#define PRINT   printf
#else
#define PRINT(...)
#endif

static char g_result[2][MAX_LINE_LEN] = {"NA", ""};
static int relult_len[2] = {0, 0};

INLINE void write_file(const bool cover, const char * const buff, const char * const filename);

void record_result(const PATH_ID path_id, unsigned short edge)
{
    if (relult_len[path_id - 1] > (MAX_LINE_LEN - 10))
        return;
    if (relult_len[path_id - 1] > 0)
        relult_len[path_id - 1] += sprintf(g_result[path_id - 1] + relult_len[path_id - 1], "|");
    relult_len[path_id - 1] += sprintf(g_result[path_id - 1] + relult_len[path_id - 1], "%d", edge);
}

void clear_result()
{
    relult_len[0]
 = 0;
    relult_len[1] = 0;
    sprintf(g_result[0], "NA");
}

void print_time(const char *head)
{
#ifdef _DEBUG
    struct timeb rawtime;
    struct tm * timeinfo;
    ftime(&rawtime);
    timeinfo = localtime(&rawtime.time);

    static int ms = rawtime.millitm;
    static unsigned long s = rawtime.time;
    int out_ms = rawtime.millitm - ms;
    unsigned long out_s = rawtime.time - s;
    ms = rawtime.millitm;
    s = rawtime.time;

    if (out_ms < 0)
    {
        out_ms += 1000;
        out_s -= 1;
    }
    printf("%s date/time is: %s \tused time is %lu s %d ms.\n", head, asctime(timeinfo), out_s, out_ms);
#endif
}

int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        PRINT("Fail to open file %s, %s.\n", filename, strerror(errno));
        return 0;
    }
    PRINT("Open file %s OK.\n", filename);

    char line[MAX_LINE_LEN + 2];
    unsigned int cnt = 0;
    while ((cnt < spec) && !feof(fp))
    {
        line[0] = 0;
        if (fgets(line, MAX_LINE_LEN + 2, fp) == NULL)  continue;
        if (line[0] == 0)   continue;
        buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
        strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
        buff[cnt][MAX_LINE_LEN + 1] = 0;
        cnt++;
    }
    fclose(fp);
    PRINT("There are %d lines in file %s.\n", cnt, filename);

    return cnt;
}

void write_result(const char * const filename)
{
    write_file(1, g_result[0], filename);
    if (g_result[0][0] == 'N')
        return;
    write_file(0, g_result[1], filename);
}

void release_buff(char ** const buff, const int valid_item_num)
{
    for (int i = 0; i < valid_item_num; i++)
        free(buff[i]);
}

INLINE void write_file(const bool cover, const char * const buff, const char * const filename)
{
    if (buff == NULL)
        return;

    const char *write_type = cover ? "w" : "a";//1:覆盖写文件，0:追加写文件
    FILE *fp = fopen(filename, write_type);
    if (fp == NULL)
    {
        PRINT("Fail to open file %s, %s.\n", filename, strerror(errno));
        return;
    }
    PRINT("Open file %s OK.\n", filename);
    fputs(buff, fp);
    fputs("\n", fp);
    fclose(fp);
}

