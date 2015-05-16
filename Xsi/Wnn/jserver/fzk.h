/*
 *  $Id: fzk.h $
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

#define YOMI_L  8               /* ��°����ɤߤ�Ĺ���κ����� */

#define STRK_L  128             /* ��°����ϤΤ���κ���ΰ���礭�� */
                                /* ���줬����°��ʸ�����Ĺ���κ����� */

/* ��°��٥��ȥ�ι�¤ */
struct fzkvect
{
  int no;                       /* ��°��٥��ȥ���ֹ� */
  int vector[VECT_L];           /* ��°��٥��ȥ� */
};

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

/* ��°����ϤΤ���κ���ΰ�ι�¤ */
struct fzkwork
{
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
  struct kangovect *kango_vect_area;
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
