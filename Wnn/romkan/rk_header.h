/*
 *  $Id: rk_header.h,v 1.6 2005/04/10 15:26:38 aonoto Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002
 *
 * Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/***********************************************************************
                        rk_header.h
                                                88. 5.20  �� ��

        rk_main.c rk_read.c rk_modread.c rk_bltinfn.c�ζ��̥إå���
        ��Ǽ�äƤ���������礭���ʤɤ������
***********************************************************************/
/*      Version 3.0
 */
/*      make����ɬ�פ�define

        SYSVR2          System V�ˤ����������¤��줿toupper��tolower�����
                        �ʤʤ��Ƥ�ư����
        MVUX            ECLIPSE MV�Ǥα�ž����define  IKIS����ưdefine�����

        RKMODPATH="ʸ����"
                        ����ʸ�����⡼�����ɽ�Υ������ѥ��δĶ��ѿ���
                        ̾���ˤ���
        WNNDEFAULT      ��@LIBDIR�פ�ɸ������ɽ�Τ���ǥ��쥯�ȥ��ɽ����
                        �褦�ˤ���      
        IKIS            Ⱦ�Ѳ�̾�Σ��Х����ܤ�0xA8�ʥǥե���Ȥ�0x8E�ˤˤ���

        ����¾ �ǥХå�����ɬ�פ˱����� KDSP��CHMDSP��define
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef MVUX
#  define IKIS
#endif

#include <stdio.h>
#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /* STDC_HEADERS */

#include "rk_macros.h"
#include "wnn_os.h"

#define ESCCHR  '\033'
#define BASEMX  (26 + 10)

#define REALFN  200             /* ɽ�Υե�͡���κ���Ĺ */

#ifdef KOREAN
#define LINALL  30000           /* �б�ɽ�����ιԿ���� */
#define SIZALL  300000          /* �б�ɽ������ɽ���κ��祵���� */
#else
#define LINALL  2000            /* �б�ɽ�����ιԿ���� */
#define SIZALL  20000           /* �б�ɽ������ɽ���κ��祵������
                                   ɽ��Ĥ��ѿ����Ѱ�Ĺ�ι�פȤ��Ƥ�ȤäƤ� */
#endif

#define LINSIZ  1000            /* �б�ɽ�ΰ�Ԥκ��祵���� */
#define TRMSIZ  500             /* �б�ɽ�ΰ���ܤκ��祵������
                                   �⡼��̾�κ�Ĺ�Ȥ��Ƥ�ȤäƤ� */
#define KBFSIZ  100             /* �ܽ����Хåե��Υ����� */
#define DSPLIN  256             /* �ǥХå��� */
#define OUTSIZ  200             /* ���ϥХåե��Υ����� */
#define RSLMAX  20              /* �ؿ����ͤȤ����֤�ʸ����κ�Ĺ */

#define VARMAX  50              /* ɽ��Ĥ��ѿ��Ŀ� */
#define VARTOT  2000            /* ��ɽ���ѿ��Ŀ��� */
#define VARLEN  500             /* �ѿ�̾��Ĺ���η� */

#define FILNST  20

        /* rk_modread.c�ǻȤ�define */

#define HYOMAX  40              /* �Ѵ��б�ɽ�κ���Ŀ� */
#define HYOMEI  500             /* ɽ̾��ʸ������ */
#define PTHMAX  30              /* �������ѥ��κ���Ŀ� */
#define PTHMEI  800             /* �������ѥ�̾��ʸ������ */
#define MODMAX  50              /* �⡼�ɤμ���� */
#define MODMEI  300             /* �⡼�ɤ���ʸ���� */
#define DMDMAX  40              /* �⡼��ɽ���μ���� */
#define DMDCHR  250             /* �⡼��ɽ������ʸ���� */
#define MDHMAX  2500            /* �⡼��ɽ�κ��祵���� */
  /* �⡼��ɽ�κǽ��listscan�λ��ϡ����顼���θ���ơ��ꥹ��1�Ĥ�buffer��
     ɽ�Υ�����ʬ��äƤ����� */
#define MDT1LN  200             /* �⡼������ꥹ��1�Ĥκ���Ĺ */
#define NAIBMX  400             /* �⡼�����ɽ������ɽ���κ��祵���� */
                        /* Change KURI 200 --> 400 */

#define ARGMAX  2               /* ���Ƚ�Ǵؿ��ΰ����κ���Ŀ� */

 /* �ǥ��쥯�ȥ�̾�ζ��ڤ��define��UNIX�ѡˡ�UNIX�ʳ��δĶ��ǻȤ��ˤ�
    ����ȡ�fixednamep()���ѹ���ɬ�פ������readmode()��getenv�ط��������ˡ� */
#define KUGIRI '/'

 /* ���顼������ */
#ifndef _WNN_SETJMP
#define _WNN_SETJMP
#include <setjmp.h>
#endif
