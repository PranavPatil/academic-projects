
/* ANALYSIS OF ALGORITHMS */
/* PROJECT 1        */
/* Support Large File */
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#define _GNU_SOURCE      // Used GNU SOURCE as strndup is a GNU (glibc) extension.
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#define  TOTALBUCKETS 512

int* createbuckets(char* outfile) {

  int* files = (int*) malloc (TOTALBUCKETS * sizeof(int));
  int a=0;

  char *p;
  p = strrchr(outfile,'/');
  char* path = strndup(outfile, strlen(outfile) - strlen(p));
  char buf[5], filepath[50];

  for(a=0;a<TOTALBUCKETS;a++)   //creation of required buckets
  {
      memset(filepath, 0, 50*sizeof(char));
      strcpy(filepath, path);
      snprintf(buf,sizeof(buf),"%d",a+1);
      strcat(filepath, "/bucket");
      strcat(filepath, buf);

      files[a] = open (filepath, O_RDWR | O_CREAT | O_TRUNC, 0644);// | O_LARGEFILE);
      if (files[a] == -1) {
        perror ("open");
        return NULL;
      }
  }
  return files;
}

int deletebuckets(char* outfile) {

  int a=0, val=0;

  char *p;
  p = strrchr(outfile,'/');
  char* path = strndup(outfile, strlen(outfile) - strlen(p));
  char buf[5], filepath[50];

  for(a=0;a<TOTALBUCKETS;a++)   //creation of required buckets
  {
      memset(filepath, 0, 50*sizeof(char));
      strcpy(filepath, path);
      snprintf(buf,sizeof(buf),"%d",a+1);
      strcat(filepath, "/bucket");
      strcat(filepath, buf);

      val = unlink (filepath);
      if (val == -1) {
        perror ("delete");
        return -1;
      }
  }
  return 0;
}

int* bucketsort(char* infile, char* outfile) {

  double interval = (1.0 - 0.0)/TOTALBUCKETS; //number of buckets required
  int* files;
  files = (int*) createbuckets(outfile);

  struct stat sb;
  double *p;
  int fd;

  fd = open (infile, O_RDONLY);// | O_LARGEFILE);
  if (fd == -1) {
    perror ("open");
    return NULL;
  }

  if (fstat (fd, &sb) == -1) {
    perror ("fstat");
    return NULL;
  }

  if (errno == EOVERFLOW) {
     printf("\n can't handle file size \n");
     return NULL;
  }

  p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

  if (p == MAP_FAILED) {
    perror ("mmap");
    return NULL;
  }

  if (close (fd) == -1) {
    perror ("close");
    return NULL;
  }

  unsigned long len;
  ssize_t nr;
  unsigned long limit = sb.st_size / sizeof(double);

  for (len = 0; len < limit; len++) {

    nr = write(files[(int)(p[len]/interval)], &p[len], sizeof(double));

    if (nr == -1) {
      perror("error in writing");
      return NULL;
    }
  }

  if (munmap (p, sb.st_size) == -1) {
    perror ("munmap");
    return NULL;
  }

  return files;
}

void swap(double* a, double* b)
{
   double temp;
   temp = *a;
   *a = *b;
   *b = temp;
}

void quicksort(double* buffer,long start, long end) {

  if (start >= end)
    return;

  long i = 0, total = 0;
  long pivot = (long)(start + end) / 2;
  long * a1, *a2;

  a1 = (long*) malloc (pivot * sizeof(long));
  if (a1==NULL) return;

  a2 = (long*) malloc (pivot * sizeof(long));
  if (a2==NULL) return;

  long c1 = 0, c2 = 0;

  for(i = start; i < pivot; i++) {

        if(buffer[i] > buffer[pivot]) {
                a1[c1] = i;
                c1++;
        }
  }

  for(i = pivot+1; i < end; i++) {

        if(buffer[i] < buffer[pivot]) {
                a2[c2] = i;
                c2++;
        }
  }

  if((c1 == 0 && c2 != 0) || (c1 !=0 && c2 == 0) || (c1 != 0 && c2 != 0))
  {
    if(c1 > c2) {
     total = c2;
         c2 = 0;
    }
    else if(c2 > c1) {
     total = c1;
         c1 = 0;
    }
    else if(c1 == c2){
     total = c1;
    }

    for(i = 0; i < total; i++) {

           swap(&buffer[a1[i]], &buffer[a2[i]]);
    }

    if(c1==0) {

          long counter = total;
          c1 = c2-1;

          for(i = pivot + 1; c1 >= counter; i++) {

            if(i < a2[counter]) {
               swap(&buffer[i], &buffer[a2[c1]]);
               c1--;
                }
                else {
                   counter++;
                }
      }

      swap(&buffer[pivot], &buffer[i-1]);
          pivot = i-1;
    }
    else if(c2==0) {

          long counter = c1-1;
          c2 = total;

          for(i = pivot - 1; c2 <= counter; i--) {

            if(i > a1[counter]) {
               swap(&buffer[i], &buffer[a1[c2]]);
               c2++;
            }
            else {
           counter--;
            }
      }

          swap(&buffer[pivot], &buffer[i+1]);
          pivot = i+1;
    }
  }
  free(a1);
  free(a2);

  quicksort(buffer, start, pivot);
  quicksort(buffer, pivot+1, end);
}

int main(int argc, char *argv[]) {

  if(argc < 3) {
	  printf("\n  USAGE:  ./sorting inputfile outputfile\n");
	  return -1;
  }

  int* files;
  files = bucketsort(argv[1], argv[2]);

  double *buffer;
  struct stat sb;
  int i = 0; // to count the file dsecriptors
  int fw;

  fw = open (argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fw == -1) {
     perror ("open");
     return -1;
  }

  for(i = 0; i<TOTALBUCKETS; i++) {

        if (fstat (files[i], &sb) == -1) {
      perror ("fstat");
      return -1;
        }

    buffer = (double*) malloc (sb.st_size);
    if (buffer==NULL)
      return -1;

        ssize_t nr = read (files[i], buffer, sb.st_size);
    if (nr == -1) {
      perror ("error in read");
      return -1;
    }

        long total = (long) (sb.st_size / sizeof(double));
        quicksort(buffer,0, total);

    /*for(len = 0; len < total; len++) {
      printf("\n Num = %f ", buffer[len]);
        }
        printf("\n")*/;

        nr = write(fw, buffer, sb.st_size);
    if (nr == -1) {
      perror ("error in write");
      return -1;
    }

        free(buffer);
  }

  if (close (fw) == -1) {
    perror ("close");
    return -1;
  }

 int a;
 for(a=0;a<TOTALBUCKETS;a++)   //creation of required buckets
 {
    close (files[a]);
 }

 free(files);
 deletebuckets(argv[2]);   // could have used system( /usr/bin/rm /a1000/silaghicls/AA/students/dir/*);
 return 0;
}
