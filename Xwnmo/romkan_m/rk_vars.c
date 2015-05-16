/*
 * $Id: rk_vars.c,v 1.2 2001/06/14 18:16:10 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 *
 * Author: OMRON SOFTWARE Co., Ltd. <freewnn@rd.kyoto.omronsoft.co.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Commentary:
 *
 * Change log:
 *
 * Last modified date: 8,Feb.1999
 *
 * Code:
 *
 */
/***********************************************************************
                        rk_vars.c
                                                87.12. 3  �� ��

        �ץ���������İʾ�Υե�����ˤޤ����äƻȤ��ѿ���
        �ޤȤ��������Ƥ��롣
***********************************************************************/
/*      Version 3.1     88/06/14        H.HASHIMOTO
 */
#ifndef MULTI
#ifndef OMRON_LIB
#include "rk_header.h"
#endif

char rk_errstat = 0;            /* (error)�ˤ�äƥ��顼�������������줿��1��â��
                                   (eofflg)��Ω�äƤ������������ޤǤζ������Ͻ�����
                                   �����ä�����2�ˤˤʤ롣romkan_henkan()�¹Ԥ��Ȥ�
                                   ��������롣����ϥ桼���ˤⳫ���� */

#ifdef OMRON_LIB
static
#endif
int flags = 0;
 /* �ʲ��Υե饰��OR��
    RK_CHMOUT �⡼�ɥ����󥸤��Τ餻�뤫�Υե饰 
    RK_KEYACK ����������Ф�ɬ�������֤����Υե饰 
    RK_DSPNIL romkan_disp(off)mode�����ͤΥǥե���Ȥ���ʸ����Ǥ��뤫��
    �ե饰��Ω���ʤ���Хǥե���Ȥ�NULL���ߴ����Τ���romkan_
    init2�ޤǤϤ��줬Ω�ġ�
    RK_NONISE �������ɤ�Ф��ʤ��褦�ˤ��뤫�Υե饰
    RK_REDRAW Wnn�ǻȤ��ü�ʥե饰�����줬Ω�äƤ����硢romkan_henkan
    �η�̤Ȥ���disout��nisedl�ʵ�ʪ��del�˰ʸ夬�ü쥳���ɤΤ�
    �ǽ����ʸ�����֤äƤ����顢���θ���REDRAW��Ĥʤ���Wnn
    ���Ѵ��Ԥ�redraw�򤹤뤿���ɬ�פ����֡�
    RK_SIMPLD delete��ư���ñ��ˤ��뤫�Υե饰
    RK_VERBOS verbose�ǵ��������Υե饰
  */

#ifdef OMRON_LIB
static
#endif
jmp_buf env0;

#ifdef OMRON_LIB
static
#endif
char *dspmod[2][2] = { NULL, NULL, NULL, NULL };
         /* ���ߵڤӰ�����Ρ�romkan�򤽤줾��on��off���Ƥ�����Υ⡼��ɽ��
            ʸ����ؤΥݥ��󥿡�romkan_dispmode()���֤��ͤ�dspmod[0][0]�� */


/* 88/06/07 V3.1 */
struct modetable
{
  int max;
  int count;
  char **point;
  int size;
  char *org;
  char *next;
};

#ifdef OMRON_LIB
static
#endif
struct modetable rk_defmode;
#ifdef OMRON_LIB
static
#endif
struct modetable rk_dspmode;
#ifdef OMRON_LIB
static
#endif
struct modetable rk_taiouhyo;
#ifdef OMRON_LIB
static
#endif
struct modetable rk_path;


 /* �⡼�ɤξ��֡��ơ�1��0���ͤ�Ȥ� */
struct modeswtable
{
  int max;
  int count;
  int *point;
};
#ifdef OMRON_LIB
static
#endif
struct modeswtable rk_modesw;

 /* �⡼�����ɽ���ɤ߹��ߤΤ���ΥХåե� */
struct modebuftable
{
  int size;
  char *org;
};
#ifdef OMRON_LIB
static
#endif
struct modebuftable rk_modebuf;

 /* �⡼�����ɽ�����������Υǡ��� */
struct modenaibutable
{
  int size;
  int *org;
  int *next;
};
#ifdef OMRON_LIB
static
#endif
struct modenaibutable rk_modenaibu;

 /* �����ܡ������ɽ�Τɤ줬���򤵤�Ƥ��뤫�Υǡ��� */
struct usehyotable
{
  int size;
  int *usemaehyo;
  int *usehyo;
  int *useatohyo;
};
#ifdef OMRON_LIB
static
#endif
struct usehyotable rk_usehyo;

struct dat                      /* ���ϡ����ϡ��Хåե��Ĥ����ؤΥݥ��� */
{
  letter *code[3];
};

 /* ��Ĥ��б�ɽ�Ρ��Ѵ��ǡ����ڤӳ��ѿ����Ѱ�Ρ���Ǽ���ؤΥݥ��󥿡� */
struct hyo
{
  int hyoshu;                   /* ɽ�μ���(�����ܡ������ɽ) */
  struct dat *data;             /* �Ѵ��ǡ����γ������� */
  letter **hensudef;            /* �Ѱ�ɽ�γ������� */
  int size;                     /* ��Ǽ���Υ����� *//* V3.1 */
  letter *memory;               /* ��Ǽ���ؤΥݥ��� *//* V3.1 */
};

struct hyotable
{
  int size;
  struct hyo *point;
};
#ifdef OMRON_LIB
static
#endif
struct hyotable rk_hyo;

struct hyobuftable
{
  int size;
  char *org;
  char *next;
};
#ifdef OMRON_LIB
static
#endif
struct hyobuftable rk_hyobuf;

struct henikitable
{
  int size;
  letter *org;
  letter *next;
};
#ifdef OMRON_LIB
static
#endif
struct henikitable rk_heniki;

struct hensuset
{
  unsigned regdflg:1;           /* ����Ͽ���ѿ���ɽ�� */
  unsigned curlinflg:1;         /* ���߹Ԥ˴��Ф��ѿ���ɽ�� */
  letter *name;
  letter *range;
};

struct hensuutable
{
  int max;
  int count;
  struct hensuset *point;
  int size;
  letter *org;
  letter *next;
};
#ifdef OMRON_LIB
static
#endif
struct hensuutable rk_hensuu;

struct hensuudeftable
{
  int max;
  int count;
  letter **point;
};
#ifdef OMRON_LIB
static
#endif
struct hensuudeftable rk_hensuudef;

/* ���ߥޥå�����Ƥ����ѿ����ֹ�ȡ��ޥå�����ʸ�����Ȥ��ݻ���*/
struct matchpair
{
  int hennum;
  letter ltrmch;
};
/* �ѿ��Υޥå������򤿤�Ƥ��� */
struct matchtable
{
  int size;
  struct matchpair *point;
};
#ifdef OMRON_LIB
static
#endif
struct matchtable rk_henmatch;
#else /*!MULTI */
#include "rk_header.h"
letter *lptr;
#endif /*!MULTI */
