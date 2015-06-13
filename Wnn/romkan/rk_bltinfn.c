/*
 *  $Id: rk_bltinfn.c,v 1.7 2013/09/02 11:01:39 itisango Exp $
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
                        rk_bltinfn.c
                                                87.12. 6  �� ��

        �Ѵ��Ѥ��Ȥ߹��ߴؿ��Τ���ʣ���ʤ�Τ�������Ƥ��롣
        ���Ѣ���Ⱦ�Ѥ��Ѵ������Ρ�
***********************************************************************/
/*  Version 3.0  */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /* STDC_HEADERS */

#include "commonhd.h"
#include "wnn_config.h"
#include "rk_header.h"
#include "jslib.h"

#include "romkan.h"

 /* Ⱦ��ʸ���Υ����ɤ�define */
#define HKCHOU  (HNKAK1 * 0x100 + 0xB0) /* �� */
#define HKDKTN  (HNKAK1 * 0x100 + 0xDE) /* �� */
#define HKHNDK  (HNKAK1 * 0x100 + 0xDF) /* �� */
#define HKMARU  (HNKAK1 * 0x100 + 0xA1) /* �� */
#define HKHRKG  (HNKAK1 * 0x100 + 0xA2) /* �� */
#define HKTJKG  (HNKAK1 * 0x100 + 0xA3) /* �� */
#define HKTTEN  (HNKAK1 * 0x100 + 0xA4) /* �� */
#define HKNKPT  (HNKAK1 * 0x100 + 0xA5) /* �� */

 /* ����ʸ���Υ����ɤ�define */
#define CHOUON  (0xA1BC)        /* �� */
#define DAKUTN  (0xA1AB)        /* �� */
#define HNDAKU  (0xA1AC)        /* �� */
#define MNMARU  (0xA1A3) /* �� */       /* ̾���� MaNMARU�ʤޤ�ޤ�ˤ�ά */
#define HRKKAG  (0xA1D6)        /* �� */
#define TJIKAG  (0xA1D7)        /* �� */
#define TOUTEN  (0xA1A2)        /* �� */
#define NKPOTU  (0xA1A6)        /* �� */

/* This is bad code .  It should be declared in the header . 
 * But where should it be declared ?  */
extern letter vtol FRWNN_PARAMS((letter));

static char _lang[6];

void
romkan_set_lang (lang)
     char *lang;
{
  strncpy (_lang, lang, 5);
  _lang[5] = 0;
}

 /** ASCIIʸ�������� */
/* *INDENT-OFF* */
letter
to_zenalpha (l)
    letter l;
/* *INDENT-ON* */
{
  letter retval;

  static uns_chr *data = (uns_chr *) "�����ɡ������ǡʡˡ��ܡ��ݡ������������������������������䡩\
�����£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣ڡΡ�ϡ�����������\
�����������������������������Сáѡ�";

#ifdef  CHINESE
  static uns_chr *data_cn = (uns_chr *) "���������磥����������������������������������������������������\
�����£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣ڣۣ��ݣޣߣ������\
������������������������������������";
#endif /* CHINESE */
#ifdef  KOREAN
  static uns_chr *data_ko = (uns_chr *) "����������������������������������������������������������������\
�����£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣ڣۣܣݣޣߣ������\
������������������������������������";
#endif /* KOREAN */

  if (' ' <= l && l <= '~')
    {
      l = (l - ' ') << 1;
#ifdef  CHINESE
      if (!strcmp (_lang, WNN_C_LANG) || !strcmp (_lang, WNN_T_LANG))
        {
          retval = data_cn[l] << 8;
          retval += data_cn[++l];
        }
      else
#endif
#ifdef  KOREAN
      if (!strcmp (_lang, WNN_K_LANG))
        {
          retval = data_ko[l] << 8;
          retval += data_ko[++l];
        }
      else
#endif /* KOREAN */
        {
          retval = data[l] << 8;
          retval += data[++l];
        }
      return (retval);
    }
  else
    return (l);
}

static char *hankdata[] = {
  "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
  "��", "����", "��", "����", "��", "����", "��", "����", "��", "����",
  "��", "����", "��", "����", "��", "����", "��", "����", "��", "����",
  "��", "����", "��", "����", "��", "��", "��", "��", "�Î�", "��", "�Ď�",
  "��", "��", "��", "��", "��",
  "��", "�ʎ�", "�ʎ�", "��", "�ˎ�", "�ˎ�", "��", "�̎�", "�̎�",
  "��", "�͎�", "�͎�", "��", "�Ύ�", "�Ύ�",
  "��", "��", "��", "��", "��",
  "��", "��", "��", "��", "��", "��",
  "��", "��", "��", "��", "��",
  "��", "��", "��", "��", "��", "��",
  "����", "��", "��"
};                              /* ���Ѥ������äƤ�Τ���� */

 /**    ���hankdata�����ºݤ˻Ȥ�Ⱦ�ѥ����ɤ�ɽ���Ƥ��ʤ��Ȥ����ºݤΤ�Τ�
        �������롣���������˰������Ƥ� */
void
hank_setup ()
{
  int i;
  char *s, orig_hnkak1;

  orig_hnkak1 = *hankdata[0];
  /*     *hankdata[] �Ǥ�Ⱦ��ʸ���Σ��Х��ȤᡣȾ��ʸ���Σ��Х��Ȥ�������ۤʤ�
     �褦��¾����˰ܿ�����Ȥ��ϡ�HNKAK1��define���Ѥ����OK��â��romkan��
     ���������Ⱦ��ʸ���ʤ��Υե�����ˤΤ�¸�ߡˤ⥳��С��Ȥ��ơ����ε���
     �˹�碌��ۤ���˾�ޤ����������������ǥ����Ǥ��Υե��������������
     ������ˡ�Ⱦ��ʸ���ΰ��������ޤ������ʤ���礬����Τǡ��ä�
     ����С��Ȥ򤷤ʤ��Ȥ�ư���褦�˽��֤Ϥ��Ƥ��롣���줬������
     hank_setup()�Ǥ��롣hankdata�ϡ����������� hank_setup()�ˤ�ä�
     �ºݤ�Ⱦ�ѥ����ɤ�ľ����롣 */

  if (orig_hnkak1 == (char) HNKAK1)
    return;
  for (i = 0; i < numberof (hankdata); i++)
    {
      for (s = hankdata[i]; *s; s += 2)
        if (*s == orig_hnkak1)
          *s = HNKAK1;
    }
}

 /** ���ʢ�Ⱦ�ѥ������ʡ���̤���ʸ���ˤʤ뤳�Ȥ⤢�롣*/
void
to_hankata (in, outp)
     letter in, **outp;
{
  uns_chr *p, c;
  letter *out;

  out = *outp;
  switch (in)
    {
    case CHOUON:
      *out++ = HKCHOU;
      break;
    case DAKUTN:
      *out++ = HKDKTN;
      break;
    case HNDAKU:
      *out++ = HKHNDK;
      break;
    case MNMARU:
      *out++ = HKMARU;
      break;
    case HRKKAG:
      *out++ = HKHRKG;
      break;
    case TJIKAG:
      *out++ = HKTJKG;
      break;
    case TOUTEN:
      *out++ = HKTTEN;
      break;
    case NKPOTU:
      *out++ = HKNKPT;
      break;
    default:
      if (is_kata (in))
        {
          for (p = (uns_chr *) hankdata[in - KATBGN]; c = *p; p++)
            *out++ = (c << 8) + *++p;
        }
      else if (is_hira (in))
        {
          for (p = (uns_chr *) hankdata[in - HIRBGN]; c = *p; p++)
            *out++ = (c << 8) + *++p;
        }
      else
        {
          *out++ = in;
        }
    }
  *out = EOLTTR;
  *outp = out;
}

 /**    Ⱦ�ѥ������ʢ��Ҥ餬�ʡ�â�������������ʸ�����ĤˤޤȤ�Ƥ�
        ����ʤ��Τ���ա�*/
/* *INDENT-OFF* */
letter
to_zenhira (l)
    letter l;
/* *INDENT-ON* */
{
  letter retval;

  static uns_chr *data = (uns_chr *) "���֡ס����򤡤�����������á���������������������������������\
���ĤƤȤʤˤ̤ͤΤϤҤդؤۤޤߤ������������󡫡�";

#ifdef  CHINESE
  static uns_chr *data_cn = (uns_chr *) "�����������򤡤�����������á������������𤭤������Ȥ���������\
���ĤƤȤʤˤ̤͵ĤϤҤդؤۤޤߤ��Ҳ�������������";
#endif /* CHINESE */

  if (is_hankata (l))
    {
      l = (l - HKKBGN) << 1;
#ifdef  CHINESE
      if (!strcmp (_lang, WNN_C_LANG) || !strcmp (_lang, WNN_T_LANG))
        {
          retval = data_cn[l] << 8;
          retval += data_cn[++l];
        }
      else
#endif
        {
          retval = data[l] << 8;
          retval += data[++l];
        }
      return (retval);
    }
  else
    return (l);
}

 /**    Ⱦ�ѥ������ʢ����ѡ�â�������������ʸ�����ĤˤޤȤ�Ƥ�
        ����ʤ��Τ���ա�*/
/* *INDENT-OFF* */
letter
to_zenkata (l)
   letter l;
/* *INDENT-ON* */
{
  return (is_hankata (l) ? (l = to_zenhira (l), to_kata (l)) : l);
}

 /* �ӥåȥ٥����ι��� */
#define bitvec(b0, b1, b2, b3, b4, b5, b6, b7) (                         \
        (char)b0 | ((char)b1 << 1) | ((char)b2 << 2) | ((char)b3 << 3) | ((char)b4 << 4) | ((char)b5 << 5) | \
        ((char)b6 << 6) | ((char)b7 << 7)                                                \
)

 /** char������ h ��ӥåȥ٥����ȸ��Ƥ�����i�ӥåȤ�����å����� */
#define bitlook(h, i) (h[(i) >> 3] & (1 << ((i) & 7)))

#define KATRPT  0xA1B3          /* �� */
#define HIRRPT  0xA1B5          /* �� */
#define KATA_U  0xA5A6          /* �� */
#define KAT_VU  0xA5F4          /* �� */
#define HIR_KA  0xA4AB          /* �� */
#define HIR_HO  0xA4DB          /* �� */
#define KAT_KA  0xA5AB          /* �� */
#define KAT_HO  0xA5DB          /* �� */
#define HIR_HA  0xA4CF          /* �� */
#define KAT_HA  0xA5CF          /* �� */

 /**    ����Ⱦ�����򤯤äĤ��롣��̤ϰ�������ʸ����*/
void
handakuadd (in, outp)
     letter in, **outp;
{
  if ((HIR_HA <= in && in <= HIR_HO) ? 0 == (in - HIR_HA) % 3 : (KAT_HA <= in && in <= KAT_HO && 0 == (in - KAT_HA) % 3))
    {
      *(*outp)++ = in + 2;
    }
  else
    {
      *(*outp)++ = in;
      *(*outp)++ = HNDAKU;
    }
  **outp = EOLTTR;
}

 /**    ���������򤯤äĤ��롣��̤ϰ�������ʸ����*/
void
dakuadd (in, outp)
     letter in, **outp;
{
  static char flgbit[] = {
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* ���������������� */
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* ���������������� */
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* ���������������� */
    bitvec (0, 1, 0, 1, 0, 1, 0, 0),    /* �äĤŤƤǤȤɤ� */
    bitvec (0, 0, 0, 0, 1, 0, 0, 1),    /* �ˤ̤ͤΤϤФѤ� */
    bitvec (0, 0, 1, 0, 0, 1, 0, 0),    /* �ӤԤդ֤פؤ٤� */
    bitvec (1, 0, 0, 0, 0, 0, 0, 0)     /* �� */
  };
  letter c;

  if ((HIR_KA <= in && in <= HIR_HO) ? (c = in - HIR_KA, 1) : (KAT_KA <= in && in <= KAT_HO && (c = in - KAT_KA, 1)))
    {
      if (bitlook (flgbit, c))
        {
          *(*outp)++ = in + 1;
        }
      else
        {
          *(*outp)++ = in;
          *(*outp)++ = DAKUTN;
        }
    }
  else
    switch (in)
      {
      case KATRPT:
      case HIRRPT:
        *(*outp)++ = in + 1;
        break;
      case KATA_U:
        *(*outp)++ = KAT_VU;
        break;
      default:
        *(*outp)++ = in;
        *(*outp)++ = DAKUTN;
      }
  **outp = EOLTTR;
}

 /** in��Ϳ����줿�����ɤ�base�ʤο����ˤ���outp������롣*/
void
to_digit (in, base, outp)
     letter in, base, **outp;
{
  letter c;

  if (c = in, c /= base)
    to_digit (c, base, outp);
  *(*outp)++ = vtol (in % base);
  **outp = EOLTTR;
}
