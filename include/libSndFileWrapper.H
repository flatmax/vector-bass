/* Copyright 2001 Matt Flax <flatmax@ieee.org>
   This file is part of MFFM Time Scale Modification for Audio.

   MFFM Time Scale Modification for Audio is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   MFFM Time Scale Modification for Audio is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   A copy of the GPL license is available here :
   http://www.gnu.org/licenses/licenses.html#GPL
   http://www.gnu.org/licenses/gpl.html
 */

/*
Version 1.2
*/

#ifndef LIBSNDFILEWRAPPER_H_
#define LIBSNDFILEWRAPPER_H_
/*
  C++ Wrapper for the libsndfile library
  http://www.zip.com.au/~erikd/libsndfile/
  Author: Matt Flax <flatmax@ieee.org>
*/

#include <sndfile.h>
#include <string.h>

//#define DEBUG_LSFW
#ifdef DEBUG_LSFW
#include <iostream>
#endif


/** Opens a file for reading, on failure returns NULL */
SNDFILE* openAudioRead(const char *name, SF_INFO *sfinfo){
  sfinfo->format=0;
  SNDFILE *file=sf_open(name, SFM_READ,sfinfo);
  if (file==NULL){
    perror("couldn't open input file\n");
    sf_perror(file);
    return NULL;
  }
  return file;
}

/** Opens a WAV file for writing, on failure returns NULL */
SNDFILE* openWavWrite(SF_INFO *sfInfo, const char *name, int freq, int channelCnt, int bitCnt){
  printf("libSndFileWrapper::openWavWrite : opening %s\n %dHz\t%d channel(s)\t%d bits per word\n", name, freq, channelCnt, bitCnt);
    sfInfo->samplerate=freq;
    sfInfo->channels=channelCnt;
    /*    sfInfo->pcmbitwidth=bitCnt; from version 0*/
    switch (bitCnt){
    case 8:
      sfInfo->format=(SF_FORMAT_WAV | SF_FORMAT_PCM_S8);
    case 16:
      sfInfo->format=(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
    case 24:
      sfInfo->format=(SF_FORMAT_WAV | SF_FORMAT_PCM_24);
    case 32:
      sfInfo->format=(SF_FORMAT_WAV | SF_FORMAT_PCM_32);
    default:
      sfInfo->format=(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
    }

    return sf_open(name, SFM_WRITE, sfInfo);
}

/** Opens an AIFF sound file for writing, on failure returns NULL */
SNDFILE* openAiffWrite(SF_INFO *sfInfo, const char *name, int freq, int channelCnt, int bitCnt){
  printf("libSndFileWrapper::openAiffWrite : opening %s\n %dHz\t%d channel(s)\t%d bits per word\n", name, freq, channelCnt, bitCnt);
  sfInfo->samplerate=freq;
    sfInfo->channels=channelCnt;
    switch (bitCnt){
    case 8:
      sfInfo->format=(SF_FORMAT_AIFF | SF_FORMAT_PCM_S8);
    case 16:
      sfInfo->format=(SF_FORMAT_AIFF | SF_FORMAT_PCM_16);
    case 24:
      sfInfo->format=(SF_FORMAT_AIFF | SF_FORMAT_PCM_24);
    case 32:
      sfInfo->format=(SF_FORMAT_AIFF | SF_FORMAT_PCM_32);
    default:
      sfInfo->format=(SF_FORMAT_AIFF | SF_FORMAT_PCM_16);
    }
    return sf_open(name, SFM_WRITE, sfInfo);
}

/**Closes a sound file*/
void closeSndFile(SNDFILE* file){
  sf_close(file);
}

/** writes N samples to file
   returns number written */
int writeNSndFile(SNDFILE *file, int N, short int *output){
  int written;
#ifdef DEBUG_LSFW
  printf("about to write : %d\n",N);
#endif
  /*sfInfo->samples+=N;*/
  written=sf_write_short(file, output, N);
#ifdef DEBUG_LSFW
  printf("written : %d\n",written);
#endif
  return written;
}

/** writes an entire wav file */
void writeWav(const char *name, int cnt, short int *output, int freq, int channelCnt, int bitCnt){
  /*Write out to file ...*/
  int written;
  SF_INFO sfInfo;
  memset(&sfInfo, 0, sizeof (sfInfo)) ;
  SNDFILE *file=openWavWrite(&sfInfo, name, freq, channelCnt, bitCnt);
  if (file==NULL){
    perror("couldn't open output file\n");
    sf_perror(file);
    exit(-1);
  }
  sfInfo.frames=cnt;
#ifdef DEBUG_LSFW
  printf("about to write : %d\n",cnt);
#endif
  written=sf_write_short(file, output, channelCnt*cnt);
  /*  printf("written : %d\n",written);*/
#ifdef DEBUG_LSFW
  printf("written : %d\n",written);
#endif
  closeSndFile(file);
}

/** Read N (cnt) units from the WAV file to a supplied array, returns the number read.
*/
int readNWav(int start, int cnt, int channels, SNDFILE *file, short int* input){
  /*Read from file ...*/
  int readn;

  if (file==NULL){
    perror("supplied input file handle is NULL\n");
    sf_perror(file);
    exit(-1);
  }

  //locate to the correct file location
  if (sf_seek  (file, start, SEEK_SET)<0){
    perror("seek error. Tried to seek to loction ");
    sf_perror(file);
    return -1;
  }

#ifdef DEBUG_LSFW
  printf("about to read N= %d\n",cnt);
#endif
  readn=sf_read_short(file, input, cnt*channels) ;
#ifdef DEBUG_LSFW
  printf("read : %d\n",readn);
#endif
  return readn/channels;
}


/** Read an entire WAV file, allocating the memory array returned.
    You must de-allocate yourself.
*/
short int* readWav(const char *name, int *cnt, int *freq, int *channels){
  /*Read from file ...*/
  short int *input;
  int readn;
  SF_INFO sfinfo;
  sfinfo.format=0;
  SNDFILE *file=sf_open(name, SFM_READ,&sfinfo);
  if (file==NULL){
    perror("couldn't open input file\n");
    sf_perror(file);
    exit(-1);
  }

  *cnt=sfinfo.frames;
  *freq=sfinfo.samplerate;
  *channels=sfinfo.channels;

  input=(short int *)malloc(sizeof(short int)*(*cnt)*(*channels));
  if (!input){
    perror("readWav:: input malloc error - out of memory ?");
    printf("\t requested %ld bytes",sizeof(short int)*(*cnt)*(*channels));
    exit(-1);
  }

#ifdef DEBUG_LSFW
  printf("about to read : %d\n",*cnt);
#endif
  readn=sf_read_short(file, input, (*cnt)*(*channels)) ;
#ifdef DEBUG_LSFW
  printf("read : %d\n",readn);
#endif
  closeSndFile(file);
  return input;
}
#endif /*LIBSNDFILEWRAPPER_H_*/
