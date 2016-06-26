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


/* fasta input */

void fasta_open_rest(fastx_handle h);
fastx_handle fasta_open(const char * filename);
void fasta_close(fastx_handle h);
bool fasta_next(fastx_handle h,
                bool truncateatspace,
                char * char_mapping);
unsigned long fasta_get_position(fastx_handle h);
unsigned long fasta_get_size(fastx_handle h);
unsigned long fasta_get_lineno(fastx_handle h);
unsigned long fasta_get_seqno(fastx_handle h);
char * fasta_get_header(fastx_handle h);
char * fasta_get_sequence(fastx_handle h);
unsigned long fasta_get_header_length(fastx_handle h);
unsigned long fasta_get_sequence_length(fastx_handle h);
long fasta_get_abundance(fastx_handle h);

/* fasta output */

void fasta_print_header(FILE * fp, const char * hdr);

void fasta_print_sequence(FILE * fp, char * seq,
                          unsigned long len, int width);

void fasta_print(FILE * fp, const char * hdr,
                 char * seq, unsigned long len);

void fasta_print_ee(FILE * fp, const char * hdr,
                    char * seq, unsigned long len,
                    double ee);

void fasta_print_relabel_cluster(FILE * fp,
                                 char * seq,
                                 int len,
                                 char * header,
                                 int header_len,
                                 int abundance,
                                 int ordinal,
                                 int clustersize,
                                 bool showclusterid,
                                 int clusterid);

void fasta_print_relabel_ee(FILE * fp,
                            char * seq,
                            int len,
                            char * header,
                            int header_len,
                            int abundance,
                            int ordinal,
                            double ee);

void fasta_print_relabel(FILE * fp,
                         char * seq,
                         int len,
                         char * header,
                         int header_len,
                         int abundance,
                         int ordinal);

void fasta_print_db(FILE * fp, unsigned long seqno);

void fasta_print_db_sequence(FILE * fp, unsigned long seqno);

void fasta_print_db_size(FILE * fp, unsigned long seqno,
                         unsigned long size);

void fasta_print_db_strip_size(FILE * fp, unsigned long seqno);

void fasta_print_db_relabel(FILE * fp, unsigned long seqno, int ordinal);
