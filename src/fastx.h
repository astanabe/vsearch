/*

  VSEARCH5D: a modified version of VSEARCH

  Copyright (C) 2016, Akifumi S. Tanabe

  Contact: Akifumi S. Tanabe
  https://github.com/astanabe/vsearch5d

  Original version of VSEARCH
  Copyright (C) 2014-2015, Torbjorn Rognes, Frederic Mahe and Tomas Flouri

  This software is dual-licensed and available under a choice
  of one of two licenses, either under the terms of the GNU
  General Public License version 3 or the BSD 2-Clause License.


  GNU General Public License version 3

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  The BSD 2-Clause License

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

*/

struct fastx_buffer_s
{
  char * data;
  unsigned long length;
  unsigned long alloc;
  unsigned long position;
};

void buffer_init(struct fastx_buffer_s * buffer);
void buffer_free(struct fastx_buffer_s * buffer);
void buffer_extend(struct fastx_buffer_s * dest_buffer,
                   char * source_buf,
                   unsigned long len);
void buffer_makespace(struct fastx_buffer_s * buffer, unsigned long x);
void buffer_truncate(struct fastx_buffer_s * buffer, bool truncateatspace);

struct fastx_s
{
  bool is_pipe;
  bool is_fastq;

  FILE * fp;

#ifdef HAVE_ZLIB_H
  gzFile fp_gz;
#endif

#ifdef HAVE_BZLIB_H
  BZFILE * fp_bz;
#endif

  struct fastx_buffer_s file_buffer;

  struct fastx_buffer_s header_buffer;
  struct fastx_buffer_s sequence_buffer;
  struct fastx_buffer_s quality_buffer;

  unsigned long file_size;
  unsigned long file_position;

  unsigned long lineno;
  unsigned long lineno_start;
  long seqno;

  unsigned long stripped_all;
  unsigned long stripped[256];

  int format;
};

typedef struct fastx_s * fastx_handle;


/* fastx input */

bool fastx_is_fastq(fastx_handle h);
fastx_handle fastx_open(const char * filename);
void fastx_close(fastx_handle h);
bool fastx_next(fastx_handle h,
                bool truncateatspace,
                const unsigned char * char_mapping);
unsigned long fastx_get_position(fastx_handle h);
unsigned long fastx_get_size(fastx_handle h);
unsigned long fastx_get_lineno(fastx_handle h);
unsigned long fastx_get_seqno(fastx_handle h);
char * fastx_get_header(fastx_handle h);
char * fastx_get_sequence(fastx_handle h);
unsigned long fastx_get_header_length(fastx_handle h);
unsigned long fastx_get_sequence_length(fastx_handle h);

char * fastx_get_quality(fastx_handle h);
long fastx_get_abundance(fastx_handle h);

unsigned long fastx_file_fill_buffer(fastx_handle h);
