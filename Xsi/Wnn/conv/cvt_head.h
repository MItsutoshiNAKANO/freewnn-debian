/*
 *  $Id: cvt_head.h $
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

/***********************************************************************
                        cvt_head.h                       ������
                                                87/11/24�ò�����
                                                         ������
        convert_key.c �ڤ� convert_read.c �ǻȤ���
        �إå��ե����롣�ơ��֥�Υ�����������ʤɡ�
***********************************************************************/
/*      Version 4.0
 */
#ifdef  nodef
#if defined(luna)
#if defined(uniosu)
# ifndef        SYSVR2
#  define       SYSVR2
# endif
#else /* if defined(MACH) || defined(uniosb) */
# ifndef        BSD42
#  define       BSD42
# endif
#endif
#else
#if defined(sun)
# ifndef        BSD42
#  define       BSD42
# endif
#endif
#endif
#endif
#include "commonhd.h"

#if defined(luna) || defined(DGUX)
#ifndef SUPPORT_TWODIGIT_FUNCTIONS
#define SUPPORT_TWODIGIT_FUNCTIONS
#endif /* SUPPORT_TWODIGIT_FUNCTIONS */
#endif /* defined(luna) || defined(DGUX) */

#define BITSIZ (sizeof(int) * 8)
#define CHANGE_MAX 4            /* �Ѵ��ơ��֥�κ���Ŀ���BITSIZ�򲼲��ʤ����� */
#define AREASIZE 1024

#ifdef WNNDEFAULT
#  include "wnn_config.h"
 /* �ޥ���CONVERT_FILENAME������ʤΤ�������ˡ�����ѥ�����ϡ��إå��ե�����
    �Υ������ѥ��ˡ�Wnn�Υ��󥯥롼�ɥե�����Τ��꤫�����ꤷ�Ƥ������ȡ� */
#else
#  define CONVERT_FILENAME "cvt_key_tbl"
#endif

#define div_up(a, b) ((a + b - 1) / b)
 /* a,b������������a/b���ڤ�夲�������ˤ��� */

struct CONVCODE
{
  int tokey;                    /* �Ѵ����줿������ */
  char *fromkey;                /* �Ѵ����륳���� */
};
