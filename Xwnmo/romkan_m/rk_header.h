/*
 * $Id: rk_header.h,v 1.2 2001/06/14 18:16:09 ura Exp $
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
                        rk_header.h
                                                87.11.17  �� ��

        rk_main.c rk_read.c rk_modread.c rk_bltinfn.c�ζ��̥إå���
        ��Ǽ�äƤ���������礭���ʤɤ������
***********************************************************************/
/*      Version 3.1     88/06/14        H.HASHIMOTO
 */
/*      make����ɬ�פ�define

        BSD42           BSD�ˤ�strings.h����ѡ�string.h��Ȥ��������ס�
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

#ifdef MVUX
#       define IKIS
#endif

#include <stdio.h>
#include <X11/Xos.h>
#include "rk_macros.h"

#define fast register           /* V3.1 */

#define ESCCHR  '\033'
#define BASEMX  (26 + 10)

#define LINSIZ  512 /* �б�ɽ�ΰ�Ԥκ��祵���� */      /* V3.1 1000 --> 512 */
#define TRMSIZ  256 /* �б�ɽ�ΰ���ܤκ��祵������
                        �⡼��̾�κ�Ĺ�Ȥ��Ƥ�ȤäƤ� */       /* V3.1 500 --> 256 */
#define KBFSIZ  100 /* �ܽ����Хåե��Υ����� */        /* V3.1 100 --> 128 */
#ifdef KDSP
#define DSPLIN  256             /* �ǥХå��� */
#endif
#define OUTSIZ  200 /* ���ϥХåե��Υ����� */  /* V3.1 200 --> 256 */
#define RSLMAX  10              /* �ؿ����ͤȤ����֤�ʸ����κ�Ĺ */

        /* rk_modread.c�ǻȤ�define */

#define REALFN  256 /* ɽ�Υե�͡���κ���Ĺ */        /* V3.1 200 --> 256 */

#define MDT1LN  256 /* �⡼������ꥹ��1�Ĥκ���Ĺ */   /* V3.1 200 -->256 */

#define ARGMAX  2               /* ���Ƚ�Ǵؿ��ΰ����κ���Ŀ� */

 /* �ǥ��쥯�ȥ�̾�ζ��ڤ��define��UNIX�ѡˡ�UNIX�ʳ��δĶ��ǻȤ��ˤ�
    ����ȡ�fixednamep()���ѹ���ɬ�פ������readmode()��getenv�ط��������ˡ� */
#define KUGIRI '/'

/* 88/06/07 V3.1 */
#define RK_DEFMODE_MAX_DEF              16      /*  */
#define RK_DEFMODE_MAX_LOT              16      /*  */
#define RK_DEFMODE_MEM_DEF              128     /*  */
#define RK_DEFMODE_MEM_LOT              128     /*  */

#define RK_DSPMODE_MAX_DEF              16      /*  */
#define RK_DSPMODE_MAX_LOT              16      /*  */
#define RK_DSPMODE_MEM_DEF              128     /*  */
#define RK_DSPMODE_MEM_LOT              128     /*  */

#define RK_PATH_MAX_DEF                 16      /*  */
#define RK_PATH_MAX_LOT                 16      /*  */
#define RK_PATH_MEM_DEF                 256     /*  */
#define RK_PATH_MEM_LOT                 256     /*  */

#define RK_TAIOUHYO_MAX_DEF             16      /*  */
#define RK_TAIOUHYO_MAX_LOT             16      /*  */
#define RK_TAIOUHYO_MEM_DEF             128     /*  */
#define RK_TAIOUHYO_MEM_LOT             128     /*  */

#define RK_HENSUU_MAX_DEF               32      /*  */
#define RK_HENSUU_MAX_LOT               32      /*  */
#define RK_HENSUU_MEM_DEF               256     /*  */
#define RK_HENSUU_MEM_LOT               256     /*  */
#define RK_HENSUUDEF_MAX_DEF            64      /*  */
#define RK_HENSUUDEF_MAX_LOT            64      /*  */

#define RK_HYO_SIZE_DEF                 4096    /*  */

 /* ���顼������ */
#ifndef         _WNN_SETJMP
#       define  _WNN_SETJMP
#include <setjmp.h>
#endif /* _WNN_SETJMP */
