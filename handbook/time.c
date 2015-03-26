#include "stdio.h"
#include "time.h"

int main(        )
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    printf("%d\n",rawtime);
    printf ( "\007The current date/time is: %s", asctime (localtime(& rawtime)) );
    timeinfo = localtime(& rawtime);
    printf ( "%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
            timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
}
