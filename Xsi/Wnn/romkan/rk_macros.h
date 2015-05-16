/*
 *  $Id: rk_macros.h $
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
                        rk_macros.h
                                                87.11.17  �� ��

    ���Ѵ��ǻȤäƤ���ޥ���ؿ�����rk_header.h ��include����Ƥ��롣
    �����include����ȡ���ưŪ��rk_spclval.h��include����롣�桼����
    �ץ����Ǥ����include����С�to_upper�ʤɤΥޥ����Ȥ��롣 
***********************************************************************/
/*      Version 3.0
 */
#ifndef RKMCRO

#define RKMCRO
#include "rk_spclval.h"         /* ctype.h��Ȥ�ʤ��ޥ���ϡ��������������Ƥ��� */

#ifndef MVUX
#  include <ctype.h>
#endif

        /* �ޥ���ؿ����ʰ�����ʣ����ɾ�������Τ�¿���Τ���ա� */

 /* 7�ӥåȥ�����char�ѥޥ��� */
#define isoctal(c) (isdigit(c) && (c) < '8')    /* 8�ʤο����� */
#define isnulsp(c) (isspace(c) || (c) == '\0')  /* EOL���϶���ʸ���Ǥ��뤫 */

  /* ��ʸ��������ʸ���Ѵ���������
     SYSVR2�����  toupdown   7�ӥå�char��isascii������Ω���ϰϡ�
     _toupdown  ��ʸ����isalpha������Ω���ϰϡ�
     ����¾�ξ��   toupdown                ��                          */
#ifdef SYSVR2
#  define _toupdown(c) (isupper(c) ? _tolower(c) : _toupper(c))
#  define toupdown(c) (isupper(c) ? _tolower(c) : toupper(c))
#else
#  define toupdown(c) (isupper(c) ? tolower(c) : toupper(c))
#endif

 /* ctype�ޥ�����������ĥ������Ρ�letter���������ˤ�Ŭ�Ѳġ�
    is_eolsp�������ȼ��Τ�Ρ� */
#define is_lower(l) (isascii(l) && islower(l))
#define is_upper(l) (isascii(l) && isupper(l))
#define is_alpha(l) (isascii(l) && isalpha(l))
#define is_alnum(l) (isascii(l) && isalnum(l))
#define is_digit(l) (isascii(l) && isdigit(l))
#define is_octal(l) (isascii(l) && isoctal(l))
#define is_xdigit(l) (isascii(l) && isxdigit(l))
#define is_space(l) (isascii(l) && isspace(l))
#define is_cntrl(l) (isascii(l) && iscntrl(l))
#define is_nulsp(l) (isascii(l) && isnulsp(l))
#define is_eolsp(l) (is_space(l) || (l) == EOLTTR)
#ifdef SYSVR2
#  define to_upper(l) (is_lower(l) ? _toupper(l) : (l))
#  define to_lower(l) (is_upper(l) ? _tolower(l) : (l))
#  define to_updown(l) (is_alpha(l) ? _toupdown(l) : (l))
#else
#  define to_upper(l) (is_lower(l) ? toupper(l) : (l))
#  define to_lower(l) (is_upper(l) ? tolower(l) : (l))
#  define to_updown(l) (is_alpha(l) ? toupdown(l) : (l))
#endif

#endif /* RKMCRO */
