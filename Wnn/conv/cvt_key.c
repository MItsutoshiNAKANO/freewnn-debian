/*
 *  $Id: cvt_key.c,v 1.5 2013/09/02 11:01:39 itisango Exp $
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
                        convert_key.c                    ������
                                                87/12/19�������
                                                         ������
        �����ॢ���Ƚ�����ȼ�����ե��󥯥���󥭡�����С��Ƚ�����
***********************************************************************/
/*      Version 4.0
 */
#include <signal.h>
#include <setjmp.h>
#include "cvt_head.h"

/*
        keyin1()��ƤӽФ�����ˤϡ����μ�³����ɬ�פǤ���

    (1) tgetent()��4.2BSD�ˤ⤷����setupterm()��System V�ˤˤ�äơ�
        �����ܡ��ɤ�term�������äƤ��륨��ȥ���ɤ߹��ߤޤ���
    (2) convert_getstrs() �ˤ�äơ��ɤ����������ѤΥ��ꥢ�˥��ԡ����ޤ���
        ����ˤ�äơ�¾�Υ���ȥ��term������ɤ߹���Ǥ⡢(1)����������
        �����ʤ��ʤ�ޤ���
    (3) convert_key_setup()��Ƥӡ��Ѵ��ν������򤷤ޤ���

        convert_getterm()�ˤ�äơ�(1)��(2)�����٤ˤǤ��ޤ���
        �ޤ���convert_key_init()�ˤ�ä�(2)��(3)����
        convert_getterm_init()�ˤ�ä�(1)����(3)�ޤǤ������줾����٤ˤǤ��ޤ���

        �ʤ��������� convert_ �ǻϤޤ�饤�֥��ؿ��ϡ�convert_read.c �ˤ�
        �������Ƥ��ޤ���

        4.2BSD�ξ��˵��ǤǸƤ�ɬ�פΤ��ä�getterm_fkeydata()�ϡ�
        ��ɬ�פˤʤä������ѻߤ���ޤ�����
*/

extern struct CONVCODE tbl[];
extern int cnv_tbl_cnt;         /* convert table count */


 /** int������ h ��ӥåȥ٥����Ȥߤʤ�����i�ӥåȤ�����å����뤤��Ω�Ƥ� */
#define BITONP(h, i) (h[i / BITSIZ] &  (1 << (i % BITSIZ)))
#define BITOFP(h, i) (!BITONP(h, i))
#define BIT_UP(h, i) (h[i / BITSIZ] |= (1 << (i % BITSIZ)))
#define BITDWN(h, i) (h[i / BITSIZ] &= ~(1 << (i % BITSIZ)))

 /** �Ѵ������ɤΥ����å��ȥ������Ѵ� */
int
key_check (inbuf, conv_tbl, tbl_cnt, check_flg)
     int inbuf[];               /* ���������ȥ�� */
     struct CONVCODE conv_tbl[];        /* �������Ѵ��ơ��֥� */
     int tbl_cnt;
     int check_flg[];
{
  int dist, base;
  char *code_p;
  int i;

  for (base = 0; inbuf[base] != -1; base++)
    {
      for (dist = 0; dist < tbl_cnt; dist++)
        {
          if (BITONP (check_flg, dist) && conv_tbl[dist].fromkey != 0)
            {
              code_p = conv_tbl[dist].fromkey + base;
              if (*code_p == (char) inbuf[base])
                {
                  if (*(code_p + 1) == (char) 0)
                    {
                      /* �ޥå����� */
                      for (i = 0, base++; (inbuf[i] = inbuf[base]) != -1; i++, base++);
                      return (conv_tbl[dist].tokey);
                    }
                  /* �ޤ��ޥå����Ƥ��ʤ� */
                }
              else
                {
                  BITDWN (check_flg, dist);     /* ̵�� */
                }
            }
        }
    }

  /* �ӥåȥ٥��� check_flg[] ����0��tblcnt�ӥåȤ�Ω�ä��ޤ޻ĤäƤ���
     ��Τ����뤫Ĵ�٤롣 */
  for (i = 0; i < tbl_cnt / BITSIZ; i++)
    {
      if (check_flg[i])
        return (-1);
    }
  if ((tbl_cnt %= BITSIZ) && (check_flg[i] & ~(~0 << tbl_cnt)))
    return (-1);
  /* return -1 �� �ޤ�̤�����ʪ������ */

  return (-2);                  /* �Ѵ��оݤȤʤ�ʪ�Ϥʤ� */
}

 /** ���ꤵ�줿�Ѵ��ơ��֥�˽��äƥ������Ѵ����롣*/
int
convert_key (inkey, conv_tbl, tbl_cnt, matching_flg, in_buf)
     int (*inkey) ();           /* �������ϴؿ� */
     struct CONVCODE conv_tbl[];        /* �Ѵ��ơ��֥� */
     int tbl_cnt;               /* conv_tbl[] �θĿ� */
     int matching_flg;          /* �ޥå��󥰤��ʤ��ä����ȥ�󥰤ν�������
                                   0 : ���ͤȤ����֤�
                                   1 : ���Υ��ȥ�󥰤ϼΤƤ�   */
     char *in_buf;
{
#define MAX     20              /* �������ϥХåե��κ����� */

  static int inbuf[MAX];        /* �������ϥХåե� */
  /* �Хåե��ν�ü�ϡ�-1 �Ǽ�����롣 */

  int out_cnt;                  /* ���ϥХåե��ν��ϥ������ */

  static int buf_cnt = 0;       /* inbuf �����ϻ��Υ�����   */

  int check_flg[CHANGE_MAX];
  /* �ӥåȥ٥����Ȥ��ư���졢�ޥå��󥰻����оݤȤʤäƤ���conv_tbl[]
     �򼨤���1�λ��оݤȤʤꡢ0�����о� */

  int i, c, flg = 0;            /* work */

  for (i = 0; i < div_up (tbl_cnt, BITSIZ); check_flg[i++] = ~0);
  /* ����check_flg��ӥåȥ٥�����������������0��tbl_cnt �ӥåȤ�Ω�Ƥ롣
     â�����ºݤϤ��ξ�����ޤ�Ω�� */

  for (;;)
    {
      if (flg != 0 || buf_cnt == 0)
        {
          inbuf[buf_cnt] = (*inkey) (); /* ��ʸ������ */
          in_buf[buf_cnt] = (char) (inbuf[buf_cnt] & 0xff);
          if (inbuf[buf_cnt] == -1)
            {
              if (buf_cnt > 0)
                {
                  c = -2;       /* �����ॢ���� */
                  goto LABEL;
                }
              else
                {
                  continue;
                }
            }
          else
            {
              inbuf[++buf_cnt] = -1;    /* �����ߥ͡��� */
            }
        }
      flg++;

      if (buf_cnt >= MAX - 1)
        {
          in_buf[0] = '\0';
          return (-1);          /* ERROR */
        }

      c = key_check (inbuf, conv_tbl, tbl_cnt, check_flg);
    LABEL:
      switch (c)
        {
        case -1:                /* ̤���� */
          continue;

        case -2:                /* �Ѵ��оݤǤʤ����Ȥ����ꤷ�� */
          buf_cnt--;
          out_cnt = 0;
          c = inbuf[out_cnt++];
          for (i = 0; inbuf[i] != -1; inbuf[i++] = inbuf[out_cnt++]);
          if (matching_flg != 0)
            {
              flg = 0;
              continue;
            }
          in_buf[0] = '\0';
          return (c);

        default:                /* �Ѵ����줿������ */
          in_buf[buf_cnt] = '\0';
          buf_cnt = 0;
          return (c);
        }
    }
}

 /** �������Ѵ���ȼ���������ϴؿ� */
int
keyin1 (get_ch, in_buf)
     int (*get_ch) (void);          /* getchar() ��Ʊ�ͤδؿ� */
     char *in_buf;
{
  int ret;

  for (;;)
    {
      if (cnv_tbl_cnt == 0)
        {
          ret = (*get_ch) ();
          if (ret >= 0)
            return (ret);
        }
      else
        {
          return (convert_key (get_ch, tbl, cnv_tbl_cnt, 0, in_buf));
        }
    }
}
