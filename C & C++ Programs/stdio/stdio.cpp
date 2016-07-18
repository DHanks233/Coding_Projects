//-----------------------------------------------------------------------------
// File:        stdio.cpp
// Classes:     FILE
//
// Methods:     recursive_itoa()
//              itoa()
//              printf()
//              setvbuf()
//              setbuf()
//              fopen()
//              fpurge()
//              fflush()
//              fread()
//              fwrite()
//              fgetc()
//              fputc()
//              fgets()
//              fputs()
//              feof()
//              fseek()
//              fclose()
//
// Written by:  Professor Munehiro Fukuda (recursive_itoa, itoa, printf, fopen,
//                                          setvbuf, setbuf)
//
//              Daniel Hanks (fpurge, fflush, fread, fwrite, fgetc, fputc,
//                            fgets, fputs, feof, fseek, fclose)
//
// Date:        05/23/2015 - last updated
//----------------------------------------------------------------------------
#include <fcntl.h>     // open
#include <sys/types.h> // read
#include <sys/uio.h>   // read
#include <unistd.h>    // read, close
#include <strings.h>   // bcopy
#include <string.h>    // strlen
#include <stdarg.h>    // format, ...
#include <stdlib.h>    // abs
#include <cstddef>     // std
#include <errno.h>     // errno

using namespace std;

char decimal[100];
const char CR_LF = '\n';
const char NULL_CHAR = '\0';
const int THRESHHOLD = 105;

int fgetc( FILE *stream );
int fseek( FILE *stream, long offset, int whence );
int fputc( int c, FILE *stream );


int recursive_itoa( int arg ) {
  int div = arg / 10;
  int mod = arg % 10;
  int index = 0; //Test
  if ( div > 0 )
    index = recursive_itoa( div );
  decimal[index] = mod + '0';
  return ++index;
}


char *itoa( const int arg ) {
  bzero( decimal, 100 );
  int order = recursive_itoa( arg );
  char *new_decimal = new char[order + 1];
  bcopy( decimal, new_decimal, order + 1 );
  return new_decimal;
}

//-----------------------------------------------------------------------------
// printf
// Writes the array *format to the console with as many variables to be printed
// as provided as additional parameters
//
// @pre:   None
// @post:  format is sent to console as output
// @param  format:    A pointer to an array to be output to console
// @param  ...:       Any additional parameters to be output to console
// @returns:          The total number of characters written if successful,
//                    a negative number otherwise
//-----------------------------------------------------------------------------
int printf( const void *format, ... ) {
  va_list list;
  va_start( list, format );

  char *msg = ( char * )format;
  char buf[1024];
  int nWritten = 0;

  int i = 0, j = 0;
  while ( msg[i] != '\0') {
    if ( msg[i] == '%' && msg[i + 1] == 'd' ) {
      buf[j] = '\0';
      nWritten += write( 1, buf, j );
      j = 0;
      i += 2;

      int int_val = va_arg( list, int );
      char *dec = itoa( abs( int_val ) );
      if ( int_val < 0 )
  nWritten += write( 1, "-", 1 );
      nWritten += write( 1, dec, strlen( dec ) );
      delete dec;
    }
    else
      buf[j++] = msg[i++];
  }
  if ( j > 0 )
    nWritten += write( 1, buf, j );
  va_end( list );
}

//-----------------------------------------------------------------------------
// setvbuf
// Writes a single character (int c) to the stream->buffer, and calls fflush()
// if that buffer is full to output to file pointed to by stream
//
// @pre:   stream represents an open FILE, buf is not NULL, size > 0
// @post:  stream->buffer is allocated
// @param  stream:    A pointer to an open FILE object
// @param  buf:       A buffer to be copied into stream->buffer if not empty
// @param  mode:      The mode of the file (unbuffered, line or fully buffered
// @param  size:      The size of the buffer to be created
// @returns:          0 if successful, -1 otherwise
//-----------------------------------------------------------------------------
int setvbuf( FILE *stream, char *buf, int mode, size_t size ) {
  if ( mode != _IONBF && mode != _IOLBF && mode != _IOFBF )
    return -1;
  stream->mode = mode;
  stream->pos = 0;

  if ( stream->buffer != (char *)0 && stream->bufown == true )
    delete stream->buffer;

  switch ( mode ) {
  case _IONBF:
    stream->buffer = (char *)0;
    stream->size = 0;
    stream->bufown = false;
    break;
  case _IOLBF:
  case _IOFBF:
    if ( buf != (char *)0 ) {
      stream->buffer = buf;
      stream->size   = size;
      stream->bufown = false;
    }
    else {
      stream->buffer = new char[BUFSIZ];
      stream->size = BUFSIZ;
      stream->bufown = true;
    }
    break;
  }

  return 0;
}

//-----------------------------------------------------------------------------
// setbuf
// Calls setvbuf using a ternary operation to represent unbuffered vs fully
// buffered and uses a default value for buffer size
//
// @pre:   stream represents an open FILE, buf is not NULL
// @post:  stream->buffer is allocated
// @param  stream:    A pointer to an open FILE object
// @param  buf:       A buffer to be copied into stream->buffer if not empty
//-----------------------------------------------------------------------------
void setbuf( FILE *stream, char *buf ) {
  setvbuf( stream, buf, ( buf != (char *)0 ) ? _IOFBF : _IONBF , BUFSIZ );
}

//-----------------------------------------------------------------------------
// fopen
// Opens a file at location *path in the mode *mode, ie: r, r+, w, w+, a, a+
//
// @pre:   *path and *mode are not NULL and represent correct information
// @post:  The file at *path is opened in the mode specified by *mode
// @param  path:     The path to the file to be opened
// @param  mode:     The mode to open the file in
// @returns:         A pointer to the open file location
//-----------------------------------------------------------------------------
FILE *fopen( const char *path, const char *mode ) {
  FILE *stream = new FILE( );
  setvbuf( stream, (char *)0, _IOFBF, BUFSIZ );

  // fopen() mode
  // r or rb           =  O_RDONLY
  // w or wb           =  O_WRONLY | O_CREAT | O_TRUNC
  // a or ab           =  O_WRONLY | O_CREAT | O_APPEND
  // r+ or rb+ or r+b  =  O_RDWR
  // w+ or wb+ or w+b  =  O_RDWR   | O_CREAT | O_TRUNC
  // a+ or ab+ or a+b  =  O_RDWR   | O_CREAT | O_APPEND

  switch( mode[0] ) {
  case 'r':
    if ( mode[1] == '\0' )            // r
      stream->flag = O_RDONLY;
    else if ( mode[1] == 'b' ) {
      if ( mode[2] == '\0' )          // rb
  stream->flag = O_RDONLY;
      else if ( mode[2] == '+' )      // rb+
  stream->flag = O_RDWR;
    }
    else if ( mode[1] == '+' )        // r+  r+b
      stream->flag = O_RDWR;
    break;
  case 'w':
    if ( mode[1] == '\0' )            // w
      stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
    else if ( mode[1] == 'b' ) {
      if ( mode[2] == '\0' )          // wb
  stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
      else if ( mode[2] == '+' )      // wb+
  stream->flag = O_RDWR | O_CREAT | O_TRUNC;
    }
    else if ( mode[1] == '+' )        // w+  w+b
      stream->flag = O_RDWR | O_CREAT | O_TRUNC;
    break;
  case 'a':
    if ( mode[1] == '\0' )            // a
      stream->flag = O_WRONLY | O_CREAT | O_APPEND;
    else if ( mode[1] == 'b' ) {
      if ( mode[2] == '\0' )          // ab
  stream->flag = O_WRONLY | O_CREAT | O_APPEND;
      else if ( mode[2] == '+' )      // ab+
  stream->flag = O_RDWR | O_CREAT | O_APPEND;
    }
    else if ( mode[1] == '+' )        // a+  a+b
      stream->flag = O_RDWR | O_CREAT | O_APPEND;
    break;
  }

  mode_t open_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  if ( ( stream->fd = open( path, stream->flag, mode ) ) == -1 ) {
    delete stream;
    printf( "fopen failed\n" );
    stream = NULL;
  }
  return stream;
}

//-----------------------------------------------------------------------------
// fpurge
// Fills the FILE buffer with '\0' to reset it.
//
// @pre:    stream represents an open FILE
// @post:   stream->buffer is reset by being filled with '\0'
// @param stream:     A pointer to an open FILE object
// @returns:          0 if successful, EOF (-1) if it fails
//-----------------------------------------------------------------------------
int fpurge( FILE *stream ) {
  if(stream == NULL || stream->buffer == NULL) {
    errno = EBADF;
    printf("fpurge error: %d\n", strerror(errno));
    return EOF;
  }
  memset(stream->buffer, 0, stream->size);
  stream->pos = 0;
  stream->actual_size = 0;
  return 0;
}

//-----------------------------------------------------------------------------
// fflush
// If the last operation performed on the FILE * stream was a write, the
// contensts of stream->buffer are output to stream. Else, purges buffer.
//
// @pre:    stream represents an open FILE
// @post:   stream->pos is reset to 0, stream->buffer is reset
// @param stream:     A pointer to an open FILE object
// @returns:          0 if successful, EOF (-1) if it fails
//-----------------------------------------------------------------------------
int fflush( FILE *stream ) {
  if(stream == NULL || stream->buffer == NULL) {
    errno = EBADF;
    printf("fflush error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream->lastop == 'r') {
    fpurge(stream);
  }
  else {
    write(stream->fd, stream->buffer, stream->pos);
    printf("writeBuffer = %d\n", stream->pos);
  }
  fpurge(stream);
  return 0;
}

//-----------------------------------------------------------------------------
// fread
// Reads data from a FILE object in the amount of the number of bytes (size)
// for each number of blocks (nmemb) into a buffer and returns the number of
// blocks read.
//
// @pre:   stream represents an open FILE, size == sizeof(char)
// @post:  stream->buffer is filled with the last chars read up to stream->size
// @param *ptr:       Buffer to be used to store chars read
// @param size:       Number of bytes to read per block
// @param nmemb:      Number of blocks to read
// @param stream:     An open FILE pointer
// @returns:          Returns the number of blocks read successfully
//-----------------------------------------------------------------------------
size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream ) {
  if (nmemb <= 0 || ptr == NULL || size != sizeof(char)) {
    errno = EBADFD;
    printf("fread error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream == NULL) {
    errno = EBADF;
    printf("fread error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream->eof) {
    return 0;
  }
  if (stream->lastop == 'w') {
     if (fflush(stream) == EOF){
        return EOF;
     }
  }
  char *buffer = (char *)ptr;
  size_t totalToRead = size * nmemb;
  int numberRead = 0;
  if(stream->mode == _IONBF) {
    return(read(stream->fd, buffer, totalToRead));
  }
  int sizeLeft = 0;
  int bytesRead = 0;

  while(numberRead < totalToRead) {
    if(stream->pos == 0) {
      bytesRead = read(stream->fd, stream->buffer, stream->size);
      if(bytesRead < 0) {
        return EOF;
      }
      if(bytesRead == 0) {
        stream->eof = true;
        return numberRead;
      }
      if(bytesRead < stream->size) {
        stream->actual_size = bytesRead;
      }
    }
    sizeLeft = (stream->actual_size ? stream->actual_size : stream->size)
        - stream->pos;
    if((totalToRead - numberRead) <= sizeLeft) {
      memcpy(&buffer[numberRead], &stream->buffer[stream->pos],
          (totalToRead - numberRead));
      stream->pos += (totalToRead - numberRead);
      numberRead += (totalToRead - numberRead);

    }
    else {
      memcpy(&buffer[numberRead], &stream->buffer[stream->pos], sizeLeft);
      numberRead += sizeLeft;
      stream->pos += sizeLeft;
    }
    if(stream->pos == stream->actual_size) {
      stream->eof = true;
      break;
    }
    if(stream->pos == stream->size) {
      stream->pos = 0;
      fpurge(stream);
    }
  }
  stream->lastop = 'r';
  return (numberRead / size);
}

//-----------------------------------------------------------------------------
// fwrite
// Writes data from a buffer (str) in the amount of the number of bytes (size)
// for each number of blocks (nmemb) into stream->buffer and returns the number
// of blocks read.
//
// @pre:   stream represents an open FILE, size == sizeof(char)
// @post:  stream->buffer is filled with contents of ptr of size * nmemb bytes
// @param *ptr:       Buffer to write to a file
// @param size:       Number of bytes to write per block
// @param nmemb:      Number of blocks to write
// @param stream:     An open FILE pointer
// @returns:          Returns the number of blocks written successfully
//-----------------------------------------------------------------------------
size_t fwrite( const void *ptr, size_t size, size_t nmemb, FILE *stream ) {
  if (nmemb <= 0 || ptr == NULL || size != sizeof(char)) {
    errno = EBADFD;
    printf("fread error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream == NULL) {
    errno = EBADF;
    printf("fread error: %d\n", strerror(errno));
    return EOF;
  }
  int sizeLeft = 0;
  int numberWritten = 0;
  size_t totalToWrite = size * nmemb;
  char *buffer = (char*)ptr;

  if(stream->mode == _IONBF) {
    return (write(stream->fd, buffer, (totalToWrite)));
  }
  if(stream->pos == stream->size) {
    fflush(stream);
  }
  while(numberWritten < totalToWrite) {
    sizeLeft = stream->size - stream->pos;
    if((totalToWrite - numberWritten) <= sizeLeft) {
      memcpy(&stream->buffer[stream->pos], buffer,
          (totalToWrite - numberWritten));
      stream->pos += (totalToWrite  - numberWritten);
      numberWritten += (totalToWrite - numberWritten);
    }
    else {
      memcpy(&stream->buffer[stream->pos], buffer, sizeLeft);
      stream->pos = stream->size;
      numberWritten += sizeLeft;
    }
    if(stream->pos == stream->size && numberWritten < totalToWrite) {
      fflush(stream);
    }
  }
  stream->lastop = 'w';
  return (numberWritten / size);
}

//-----------------------------------------------------------------------------
// fgetc
// Reads a single character from a FILE object, stores the character in
// stream->buffer if fully-buffered, and returns that character.
//
// @pre:   stream represents an open FILE
// @post:  stream->buffer is filled with the last chars read up to stream->size
// @param  stream:     A pointer to an open FILE object
// @returns:          The character read if successful, EOF if it fails
//-----------------------------------------------------------------------------
int fgetc( FILE *stream ) {
  unsigned char charRead = '\0';
  int bytesRead;

  if (stream == NULL) {
    errno = EBADF;
    printf("fgetc error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream->eof) {
    return EOF;
  }
  if(fread(&charRead, sizeof(char), 1, stream) < 0) {
   return EOF;
  }
  return charRead;
}

//-----------------------------------------------------------------------------
// fputc
// Writes a single character (int c) to the stream->buffer, and calls fflush()
// if that buffer is full to output to file pointed to by stream
//
// @pre:   stream represents an open FILE and c is not a NULL character
// @post:  int c parameter is written into stream->buffer
// @param  stream:    A pointer to an open FILE object
// @param  c:         A single character to be written
// @returns:          The character written if successful, EOF if it fails
//-----------------------------------------------------------------------------
int fputc( int c, FILE *stream ) {
  if(stream == NULL) {
    errno = EBADF;
    printf("fputc error: %d\n", strerror(errno));
    return EOF;
  }
  if(stream->mode == _IONBF) {
    write(stream->fd, &c, sizeof(char));
  }
  else {
    if(fwrite(&c, sizeof(char), 1, stream) < 0) {
      return EOF;
    }
  }
  stream->lastop = 'w';
  return c;
}

//-----------------------------------------------------------------------------
// fgets
// Reads from the file pointed to by FILE *stream and stores characters in *str
// up to int size, or until it encounters a '\n'.
//
// @pre:   stream represents an open FILE and *str is not NULL
// @post:  *str buffer is filled up to size or until a '\n'
// @param  stream:    A pointer to an open FILE object
// @param: size:      The number of characters to read into the buffer
// @param *str:       The buffer to read characters into
// @returns:          *str if successful, or NULL if it fails
//-----------------------------------------------------------------------------
char *fgets( char *str, int size, FILE *stream ) {
  if(stream == NULL || str == NULL) {
    errno = EBADF;
    printf("fgets error: %d\n", strerror(errno));
    return NULL;
  }
  if(stream->eof) {
    return NULL;
  }
  char charRead = NULL_CHAR;
  int numberRead = 0;
  for(int i = 0; i < (size - 1); i++, numberRead++) {
    charRead = fgetc(stream);
    if (stream->eof) {
      str[i] = charRead;
      numberRead++;
      break;
    }
    else {
      str[i] = charRead;
      if(charRead == CR_LF) {
        numberRead++;
        break;
      }
    }
  }
  str[numberRead] = NULL_CHAR;
  stream->lastop = 'r';
  return str;
}

//-----------------------------------------------------------------------------
// fputs
// Writes all chars contained in const char *str to stream->buffer up until it
// encounters the char '\0', and flushes stream->buffer if it becomes full
//
// @pre:   stream represents an open FILE, *str is not NULL and has a '\0' char
// @post:  stream->buffer is filled with contents of *str, and flushed if full
// @param  stream:    A pointer to an open FILE object
// @param  *str:      The char buffer to write to file
// @returns:          0 if successful, EOF if it fails
//-----------------------------------------------------------------------------
int fputs( const char *str, FILE *stream ) {
  if(stream == NULL || str == NULL) {
    errno = EBADF;
    printf("fputs error: %d\n", strerror(errno));
    return EOF;
  }
  char charRead = NULL_CHAR;
  int currentIndex = 0;
  do {
    charRead = str[currentIndex];
    if(charRead != NULL_CHAR) {
      if(fputc(charRead, stream) == EOF) {
        errno = EBADFD;
        printf("fputc error: %d\n", strerror(errno));
        return EOF;
      }
    }
    currentIndex++;
  }while(charRead != NULL_CHAR);
  stream->lastop = 'w';
  return 0;
}

//-----------------------------------------------------------------------------
// feof
// Writes a single character (int c) to the stream->buffer, and calls fflush()
// if that buffer is full to output to file pointed to by stream
//
// @pre:   None
// @post:  None
// @param stream:     A pointer to an open FILE object
// @returns:          true if stream->eof is true, false otherwise
//-----------------------------------------------------------------------------
int feof( FILE *stream ) {
  return stream->eof == true;
}

//-----------------------------------------------------------------------------
// fseek
// Changes the current position pointer within FILE stream based on parameters
//
// @pre:   stream is an open FILE.
// @post:  The current position pointer in FILE stream is changed accordingly
// @param  stream:    A pointer to an open FILE object
// @param  offset:    The number of bytes to seek in the file
// @param  whence:    The starting position before offset
// @returns:          0 if seek is successful, false otherwise
//-----------------------------------------------------------------------------
int fseek( FILE *stream, long offset, int whence ) {
  if(stream == NULL) {
    errno = EBADF;
    printf("fseek error: %d\n", strerror(errno));
    return EOF;
  }
  fflush(stream);
  stream->eof = false;
  stream->actual_size = 0;
  stream->pos = 0;
  return lseek(stream->fd, offset, whence);
}

//-----------------------------------------------------------------------------
// fclose
// Closes a file pointed to by FILE *stream
//
// @pre:   stream represents an open FILE
// @post:  File pointed to by stream is closed and stream->buffer is flushed
// @param  stream:    A pointer to an open FILE object
// @returns:          0 if fclose is successful, EOF (-1) otherwise
//-----------------------------------------------------------------------------
int fclose( FILE *stream ) {
  if(stream != NULL) {
    fflush(stream);
    if(close(stream->fd) != 0) {
      return EOF;
    }
    if(stream->bufown) {
      delete [] stream;
      stream = NULL;
    }
  }
  else {
    errno = EBADF;
    printf("fclose error: %d\n", strerror(errno));
    return EOF;
  }
  return 0;
}
