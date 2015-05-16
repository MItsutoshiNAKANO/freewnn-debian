/*
 *  $Id: fzk.h,v 1.6 2003/05/11 18:29:20 hiroo Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2003
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

#ifndef JSERVER_FZK_H
#define JSERVER_FZK_H 1

#include "ddefine.h"

#define YOMI_L  8               /* ��°����ɤߤ�Ĺ���κ����� */

#define STRK_L  128             /* ��°����ϤΤ���κ���ΰ���礭�� */
                                /* ���줬����°��ʸ�����Ĺ���κ����� */

/*
 * Don't know why word_vector (former kangovect) and fzkvect
 * should differ. Can not they be merged?  (2003.04.06 Hiroo Ono)
 */
/* ���졦��°��٥��ȥ� */
typedef struct
{
  int vector[VECT_L];		/* ��°��٥��ȥ� */
} word_vector;

/* ��°��٥��ȥ�ι�¤ */
typedef struct fzkvect
{
  int no;                       /* ��°��٥��ȥ���ֹ� */
  int vector[VECT_L];           /* ��°��٥��ȥ� */
} fzkvect;

/* ��°�쥨��ȥ꡼�ι�¤ */
struct fzkentry
{
  w_char yomi[YOMI_L];          /* ��°����ɤߤ�ʸ���� */
  int yomi_su;                  /* ��°����ɤߤ�ʸ�����Ĺ�� */
  int kosu;                     /* ��°��٥��ȥ�θĿ� */
  struct fzkvect *pter;         /* ��°��٥��ȥ�ؤΥݥ��� */
  struct fzkentry *link;        /* ������°�쥨��ȥ꡼�ؤΥݥ��� */
};

/* ��°��θ�����̤ξ���ι�¤ */
struct fzkken
{
  struct fzkentry *ent_ptr;     /* ������°�쥨��ȥ꡼�ؤΥݥ��� */
  int vector[VECT_L];           /* ��°��٥��ȥ� */
};

/*
        structure of FZK TABLE
 */
struct FT
{
#ifndef NO_FZK
  struct fzkvect *vect_area;
#endif
  word_vector *kango_vect_area;
#ifndef NO_FZK
  struct fzkentry *tablefuzokugo;
#endif
  int *kango_hinsi_area;
  int fzkvect_l;                /* ��³�٥�����Ĺ�� * 32 bit */
  int kango_vect_l;             /* ����٥�����Ĺ�� * 32 bit */
#ifndef NO_FZK
  int fzklength;                /* ��°��� */
#endif
};

#endif /* JSERVER_FZK_H */

