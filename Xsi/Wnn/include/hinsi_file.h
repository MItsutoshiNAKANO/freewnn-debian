/*
 *  $Id: hinsi_file.h $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000
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

/*    �ʻ�ե�����˴ؤ��롢���
*/


#define MAXHINSI        256
#define MAXFUKUGOU      256
#define MAXNODE         256
#define FUKUGOU_START   0xffff - RESERVE_FUKUGOU_USIRO
#define HEAP_LEN        (10 * (MAXHINSI + MAXFUKUGOU))
/* 10 bytes for each hinsi */
#define WHEAP_LEN       (10 * MAXFUKUGOU * sizeof(short))
/* 10 bytes for each fukugou hinsi */


#define RESERVE_FUKUGOU_USIRO 512       /* fukugou hinsi no usiro ni 
                                           koredake reserve wo site oku */
/* hitoru ha delete sareta to iu jouhou no 
   ta me*/
#define TERMINATE 0xffff
#define HINSI_ERR -2

#define WNN_NODE_SUFFIX '/'
#define NODE_CHAR '|'
#define COMMENT_CHAR ';'
#define DEVIDE_CHAR '$'
#define HINSI_SEPARATE_CHAR ':'
#define IGNORE_CHAR1 ' '
#define IGNORE_CHAR2 '\t'
#define YOYAKU_CHAR '@'
#define CONTINUE_CHAR '\\'

struct wnn_fukugou
{
  w_char *name;                 /* ʣ���ʻ��̾�� */
/* fukugou hinsi is held as a string */
  unsigned short *component;    /* ʣ���ʻ�ι������ǤȤʤ��ʻ���ֹ������
                                   (�Ǹ�� 0xffff �ǽ���)
                                 */
};

struct wnn_hinsi_node
{
  w_char *name;                 /* �ʻ�Ρ��ɤ�̾�� */
  int kosuu;                    /* �Ҥɤ�ο� */
  w_char *son;                  /* �ǽ�λҤɤ�ؤΥݥ��� */
};
