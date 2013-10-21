/* localtime example */
#include <stdio.h>
#include <time.h>

int main ()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  printf ( "Year: %u\n", timeinfo->tm_year+1900 );
  printf ( "Month: %u\n", timeinfo->tm_mon+1 );
  printf ( "Day: %u\n", timeinfo->tm_mday );
  printf ( "Hour: %u\n", timeinfo->tm_hour );
  printf ( "Min: %u\n", timeinfo->tm_min );
  printf ( "Sec: %u\n", timeinfo->tm_sec );

  FILE * pFile;
  pFile = fopen ("exp3/rtc_init.h","w");
  if (pFile!=NULL)
  {
    fputs ("#define INITYEAR 2013\n#define INITMON  05\n#define INITDAY  15\n#define INITHOUR 18\n#define INITMIN  19\n#define INITSEC 32",pFile);
    fclose (pFile);
  }



 // printf ( "Current local time and date: %s", asctime (timeinfo) );

  return 0;
}
