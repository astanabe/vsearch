/*

  VSEARCH5D: a modified version of VSEARCH

  Copyright (C) 2016-2019, Akifumi S. Tanabe

  Contact: Akifumi S. Tanabe
  https://github.com/astanabe/vsearch5d

  Original version of VSEARCH
  Copyright (C) 2014-2019, Torbjorn Rognes, Frederic Mahe and Tomas Flouri
  All rights reserved.

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

#include "vsearch5d.h"

bool header_find_attribute(const char * header,
                           int header_length,
                           const char * attribute,
                           int * start,
                           int * end,
                           bool allow_decimal)
{
  /*
    Identify the first occurence of the pattern (^|;)size=([0-9]+)(;|$)
    in the header string, where "size=" is the specified attribute.
    If allow_decimal is true, a dot (.) is allowed within the digits.
  */

  const char * digit_chars = "0123456789";
  const char * digit_chars_decimal = "0123456789.";

  if ((! header) || (! attribute))
    return false;

  int hlen = header_length;
  int alen = strlen(attribute);

  int i = 0;

  while (i < hlen - alen)
    {
      char * r = (char *) strstr(header + i, attribute);

      /* no match */
      if (r == NULL)
        break;

      i = r - header;

      /* check for ';' in front */
      if ((i > 0) && (header[i-1] != ';'))
        {
          i += alen + 1;
          continue;
        }

      int digits
        = (int) strspn(header + i + alen,
                       (allow_decimal ? digit_chars_decimal : digit_chars));

      /* check for at least one digit */
      if (digits == 0)
        {
          i += alen + 1;
          continue;
        }

      /* check for ';' after */
      if ((i + alen + digits < hlen) && (header[i + alen + digits] != ';'))
        {
          i += alen + digits + 2;
          continue;
        }

      /* ok */
      * start = i;
      * end = i + alen + digits;
      return true;
    }
  return false;
}

int64_t header_get_size(char * header, int header_length)
{
  /* read size/abundance annotation */
  int64_t abundance = 0;
  int start = 0;
  int end = 0;
  if (header_find_attribute(header,
                            header_length,
                            "size=",
                            & start,
                            & end,
                            false))
    {
      int64_t number = atol(header + start + 5);
      if (number > 0)
        abundance = number;
      else
        fatal("Invalid (zero) abundance annotation in FASTA file header");
    }
  return abundance;
}

void header_fprint_strip_size_ee(FILE * fp,
                                 char * header,
                                 int header_length,
                                 bool strip_size,
                                 bool strip_ee)
{
  int attributes = 0;
  int attribute_start[2];
  int attribute_end[2];

  /* look for size attribute */

  int size_start = 0;
  int size_end = 0;
  bool size_found = false;
  if (strip_size)
    size_found = header_find_attribute(header,
                                       header_length,
                                       "size=",
                                       & size_start,
                                       & size_end,
                                       false);
  if (size_found)
    {
      attribute_start[attributes] = size_start;
      attribute_end[attributes] = size_end;
      attributes++;
    }

  /* look for ee attribute */

  int ee_start = 0;
  int ee_end = 0;
  bool ee_found = false;
  if (strip_ee)
    ee_found = header_find_attribute(header,
                                     header_length,
                                     "ee=",
                                     & ee_start,
                                     & ee_end,
                                     true);
  if (ee_found)
    {
      attribute_start[attributes] = ee_start;
      attribute_end[attributes] = ee_end;
      attributes++;
    }

  /* sort */

  if (attributes > 1)
    {
      if (attribute_start[0] > attribute_start[1])
        {
          /* swap */

          int s = attribute_start[0];
          int e = attribute_end[0];
          attribute_start[0] = attribute_start[1];
          attribute_end[0] = attribute_end[1];
          attribute_start[1] = s;
          attribute_end[1] = e;
        }
    }

  /* print */

  if (attributes == 0)
    {
      fprintf(fp, "%.*s", header_length, header);
    }
  else
    {
      int prev_end = 0;
      for (int i = 0; i < attributes; i++)
        {
          /* print part of header in front of this attribute */
          if (attribute_start[i] > prev_end + 1)
            {
              fprintf(fp, "%.*s",
                      attribute_start[i] - prev_end - 1,
                      header + prev_end);
            }
          prev_end = attribute_end[i];
        }

      /* print the rest, if any */
      if (header_length > prev_end + 1)
        {
          fprintf(fp, "%.*s",
                  header_length - prev_end,
                  header + prev_end);
        }
    }
}

void header_fprint_strip_size(FILE * fp,
                              char * header,
                              int header_length)
{
  header_fprint_strip_size_ee(fp,
                              header,
                              header_length,
                              true,
                              false);
}