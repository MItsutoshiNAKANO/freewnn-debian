/*
 * $Id: rk_spclval.h,v 1.2 2001/06/14 18:16:10 ura Exp $
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
/*      Version 3.0
 */
#ifndef EOLTTR

typedef unsigned int letter;    /* ʸ���ϰ��̤ˤ��η���ɽ�� */
typedef unsigned char uns_chr;

typedef unsigned char modetyp;
#define LTRHUG  (~(0xff000000))
        /* ����ɽ�� �ڤӵ�ʸ���������letter�μ����������� */


        /* �ʲ�����������ͤϡ��Ǿ�̥Х��Ȥ�255�Ǥʤ���Фʤ�ʤ���
           �����Τ��������˽Ф�Τ� EOLTTR��CHMSIG��NISEBP��LTREOF��
           REDRAW�ʵ��ǥ꡼�ȤȤ���NISEDL��Ȥ�����NISEDL��ˡ� */

#define EOLTTR  (0xffffffff)    /* ʸ����ν�ü��ɽ�������� */
                        /* �������Ϥ����ä���ɬ�����������ɤ��֤����֤ˤ���
                           �����ȡ��ä��֤��٤���Τ��ʤ����Ϥ�����֤��� */
#define ERRCOD  (EOLTTR-1)      /* (error)��ɽ������������ */
#define CHMSIG  (EOLTTR-2)      /* �⡼�ɥ����󥸤�ɽ�������� */
#define NISEBP  (EOLTTR-3)      /* ���顼�ٹ��Ѥε�BEL������ */
#define VARRNG  (EOLTTR-4)      /* �Ѱ褬��Ĥ�ʸ���δ֤Ǥ���Τ򼨤����������� */
#define UNUSDC  (EOLTTR-5)      /* �ޥå����Ԥ����뤿������������� */
#define REASIG  (EOLTTR-6)      /* ɽ�κ�������׵᤹�뤿������������� */
#define URBFCL  (EOLTTR-7)      /* ΢�Хåե�������Ū���ꥢ���׵᤹������������ */
#define LTREOF  (EOLTTR-8)      /* romkan_next()��Ʊgetc()��EOF��������֤����
                                   �ʤǤ����EOF��Ʊ���ˤ��������ġ� */
#define REDRAW  (EOLTTR-9)      /* Wnn���Ѵ��Ԥ�redraw���׵᤹���ü쥳���� */
#define NISEDL  (EOLTTR-10)     /* ���ǥ꡼�ȤȤ��������Ȥ������ͤΤ���˽���
                                   ���줿�����â�����Ȥ������ʤ���лȤ�ʤ��Ƥ�
                                   �����ˡ��ѿ�nisedl�˥��åȤ��ƻȤ� */
#define SENDCH  (EOLTTR-11)     /* �Ѵ��Хåե������ʤ顢����ʸ����̵����
                                   �������褦Wnn���׵᤹�륨�������ץ����ɡ� */


        /* romkan_init3()�ΰ����Υե饰�˻Ȥ��� */

#define RK_CHMOUT 01            /* �⡼�ɥ����󥸤��Τ餻�륳���ɤ��֤����� */
#define RK_KEYACK 02            /* ����������Ф�ɬ���������֤��� */
#define RK_DSPNIL 04            /* �⡼��ɽ��ʸ����̵����λ��˶�ʸ������֤����ʥǥ�
                                   ����Ȥ�NULL�ˡ��ߴ����ݻ��Τ��� */
#define RK_NONISE 010           /* �������ɤ�Ф��ʤ��褦�ˤ��뤫 */
#define RK_REDRAW 020           /* Wnn���ü�ե饰��redraw�ѤΥե饰��Ф����ɤ����� */
#define RK_SIMPLD 040           /* delete��ư���ñ��ˤ��뤫 */
#define RK_VERBOS 0100          /* ���Ѥ���ɽ�ΰ������ݡ��Ȥ��뤫 */


        /* �����ɤζ��̤˻Ȥ��ޥ��� */

#define HG1BIT  (0x80000000)    /* �Ǿ�̥ӥåȤ��� */
#define SHUBET(X) ((letter)(X) >> 24)
                              /* ����ɽ���ǡ��壱�Х��Ȥ����ɽ���˻ȤäƤ� */
#define LWRMSK(X) ((X) & ~(0xff000000)) /* �壱�Х��Ȥ������ */
#define LWRCUT(X) ((X) &= ~(0xff000000))        /* �壱�Х��Ȥ������ */

#define is_HON(X) (SHUBET(X) == 0)      /* ��ʪ��ʸ���� */
#define NAIBEX(X) ((int)0 < SHUBET(X) && (int)SHUBET(X) < 0x80) /* ����ɽ���򼨤��ͤ� */
#define isNISE(X) (SHUBET(X) == 0x80)   /* ��ʪ��ʸ�����ʺǾ�̥ӥåȤ�Ω�ġ� */
#define isSPCL(X) (SHUBET(X) == 0xff)   /* rk_spcl_val.h�����������ͤ��ɤ��� */
 /* NISEDL�ʤɤ�ޤ᤿����ʸ���Ǥ��뤫�ɤ�����Ƚ�ꤹ��ˤϡ�~is_HON(X) ����
    isNISE(X) || isSPCL(X) �Ȥ���Ƚ�ꤷ�ʤ��Ȥ����ʤ��� */

#define toNISE(X) ((X) | HG1BIT)
#define to_HON(X) ((X) & ~HG1BIT)

 /* �ߴ����Τ����Ѱդ��Ƥ�����̾ */
#define REALCD(X) is_HON(X)
#define NISECD(X) isNISE(X)
#define SPCLCD(X) isSPCL(X)

#ifndef SS2
#define SS2     0x8E
#endif
#ifndef SS3
#define SS3     0x8F
#endif

        /** rk_bltinfn.c ���䴰�Τ���Υޥ���
            �ʰ�����ʣ����ɾ�������Τ�¿���Τ���ա�*/

#define HKKBGN  (SS2 * 0x100 + 0xA1)    /* Ⱦ�ѥ��ʤζ��� */
#define HKKEND  (SS2 * 0x100 + 0xDF)    /*     ��    Ⱦ���� */
#define HIRBGN  (0xA4A1)        /* �� */
#define HIREND  (0xA4F3) /* �� */       /* �Ҥ餬�ʡ�"��"��"��" */
#define KATBGN  (0xA5A1)        /* �� */
#define KATEND  (0xA5F6) /* �� */       /* �������ʡ�"��"��"��"��"��" */

#define _to_kata(l) ((l) + (KATBGN - HIRBGN)) /** �������ʤء���������¡�*/
#define _to_hira(l) ((l) - (KATBGN - HIRBGN)) /** �Ҥ餬�ʤء���������¡�*/
#define is_hira(l) ((int)HIRBGN <= (int)(l) && (int)(l) <= (int)HIREND) /** �Ҥ餬�ʤ��� */
#define is_kata(l) ((int)KATBGN <= (int)(l) && (int)(l) <= (int)KATEND) /** �������ʤ��� */
#define is_kata2(l) ((int)_to_kata(HIRBGN) <= (int)(l) && (int)(l) <= (int)_to_kata(HIREND))
                                /** �б�����Ҥ餬�ʤΤ��륫�����ʤ��� */
#define to_kata(l) (is_hira(l) ? _to_kata(l) : (l)) /** �������ʤ� */
#define to_hira(l) (is_kata2(l) ? _to_hira(l) : (l))
                        /** �Ҥ餬�ʤء��֥������פϥ������ʤΤޤ޻Ĥ롣*/
#define is_hankata(l) ((int)HKKBGN <= (int)(l) && (int)(l) <= (int)HKKEND)
                                /** Ⱦ�ѥ��ʡʶ����ʤɤ�ޤ�ˤ��� */



        /* ����¾�Υޥ���ؿ����ʰ�����ʣ����ɾ�������Τ�¿���Τ���ա� */

#define numberof(array) (sizeof(array) / sizeof(*array))

 /* �ݥ��󥿤�letter����������ؤ�äƤ�����letter *lp */
#define totail(lp) {while(*(lp) != EOLTTR) (lp)++;}

 /* ʸ����ؤΥݥ��󥿤򤽤�ʸ����κǸ����ء�totail��char�ǡ�char *sp */
#define strtail(sp) {while(*(sp)) (sp)++;}

        /* ������romkan_init3 */
#define romkan_init4(delchr, nisedl, flg) \
        romkan_init3(delchr, nisedl, EOLTTR, flg)

#define romkan_init5(delchr, flg) \
        romkan_init4(delchr, toNISE(delchr), flg)


#endif /* of ifndef EOLTTR */
