/*
 *  $Id: oldatonewa.c,v 1.6 2004/07/19 18:24:26 hiroo Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2004
 *
 * Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef lint
static char *rcs_id = "$Id: oldatonewa.c,v 1.6 2004/07/19 18:24:26 hiroo Exp $";
#endif /* lint */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#if STDC_HEADERS
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#endif /* STDC_HEADERS */
#include "commonhd.h"
#include "wnn_os.h"
#include "wnn_string.h"

static char *ohinsi[] = {
  "���Ը���",
  "����(�Ԥ�)",
  "���Ը���",
  "���Ը���",
  "���Ը���",
  "�ʹԸ���",
  "�йԸ���",
  "�޹Ը���",
  "��Ը���",
  "��Ը���",
  "����&̾��",
  "����",
  "����(����)&̾��",
  "����(����)",
  "����(����)",
  "��(��)",
  "���ƻ�",
  "����ư��",
  "̾��",
  "Ϣ�λ�",
  "����",
  "��³��,��ư��",
  "��(��)",
  "��(��)",
  "��Ƭ��",
  "������",
  "������",
  "����",
  "���(������)",
  "��ͭ̾��",
  "����ư��(����)",
  "ñ����"
};

#define LINE_SIZE 1024

extern int motoni2 ();
static int get_line1 (char* buffer);
static char* get_string (char* str, char* buf);

int
main (void)
{
  char buffer[LINE_SIZE];

  fgets (buffer, LINE_SIZE, stdin);
  printf ("\\total %s", buffer);
  for (; fgets (buffer, LINE_SIZE, stdin) != NULL;)
    {
      get_line1 (buffer);
    }
  exit (0);
}

static int
get_line1 (char* buffer)
{
  char *c = buffer;
  char tmp[LINE_SIZE];
  char kanji[LINE_SIZE];
  char yomi[LINE_SIZE];
  /*
     char comment[LINE_SIZE];
   */
  w_char wyomi[LINE_SIZE];
  w_char wyomi1[LINE_SIZE];
  int hinsi;
  int hindo;
  int k;
  int len;

  if ((c = get_string (yomi, c)) == NULL)
    return (1);
  wnn_Sstrcpy (wyomi, yomi);
  wnn_Sreverse (wyomi1, wyomi);
  wnn_sStrcpy (yomi, wyomi1);
  if ((c = get_string (kanji, c)) == NULL)
    return (-1);
  if ((c = get_string (tmp, c)) == NULL)
    return (-1);
  sscanf (tmp, "%x", &hinsi);
  if ((c = get_string (tmp, c)) == NULL)
    return (-1);
  sscanf (tmp, "%d", &hindo);
  hindo = motoni2 (hindo);
  for (k = 0; k < 32; k++)
    {
      if (hinsi & (1 << k))
        {
/*
      printf("%s\t\t%s\t\t%s\t\t%d\n",yomi,kanji,ohinsi[k],hindo);
*/
          printf ("%s\t", yomi);
          len = strlen (yomi);
          if (len < 8)
            printf ("\t");
          if (len < 16)
            printf ("\t");
          printf ("%s\t", kanji);
          len = strlen (kanji);
          if (len < 8)
            printf ("\t");
          if (len < 16)
            printf ("\t");
          printf ("%s\t", ohinsi[k]);
          len = strlen (ohinsi[k]);
          if (len < 8)
            printf ("\t");
          printf ("%d\n", hindo);
        }
    }
  return (0);
}

static char *
get_string (char* str, char* buf)
{
  char *c = buf;
  /*
     int k;
   */
  for (; *c == '\t' || *c == ' '; c++);
  if (*c == '\0' || *c == '\n')
    {
      return (NULL);
    }
  for (; *c != '\t' && *c != ' ' && *c != '\n' && *c != '\0'; c++)
    {
      *str++ = *c;
    }
  *str = 0;
  return (c);
}
