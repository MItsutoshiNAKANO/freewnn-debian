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
                                                87.12. 6  訂 補

        変換用の組み込み関数のうち複雑なものを定義してある。
        全角←→半角の変換が主体。
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

 /* 半角文字のコードのdefine */
#define HKCHOU  (HNKAK1 * 0x100 + 0xB0) /* ｰ */
#define HKDKTN  (HNKAK1 * 0x100 + 0xDE) /* ﾞ */
#define HKHNDK  (HNKAK1 * 0x100 + 0xDF) /* ﾟ */
#define HKMARU  (HNKAK1 * 0x100 + 0xA1) /* ｡ */
#define HKHRKG  (HNKAK1 * 0x100 + 0xA2) /* ｢ */
#define HKTJKG  (HNKAK1 * 0x100 + 0xA3) /* ｣ */
#define HKTTEN  (HNKAK1 * 0x100 + 0xA4) /* ､ */
#define HKNKPT  (HNKAK1 * 0x100 + 0xA5) /* ･ */

 /* 全角文字のコードのdefine */
#define CHOUON  (0xA1BC)        /* ー */
#define DAKUTN  (0xA1AB)        /* ゛ */
#define HNDAKU  (0xA1AC)        /* ゜ */
#define MNMARU  (0xA1A3) /* 。 */       /* 名前は MaNMARU（まんまる）の略 */
#define HRKKAG  (0xA1D6)        /* 「 */
#define TJIKAG  (0xA1D7)        /* 」 */
#define TOUTEN  (0xA1A2)        /* 、 */
#define NKPOTU  (0xA1A6)        /* ・ */

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

 /** ASCII文字→全角 */
/* *INDENT-OFF* */
letter
to_zenalpha (l)
    letter l;
/* *INDENT-ON* */
{
  letter retval;

  static uns_chr *data = (uns_chr *) "　！”＃＄％＆’（）＊＋，−．／０１２３４５６７８９：；＜＝＞？\
＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ［￥］＾＿｀ａｂｃｄｅ\
ｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ｛｜｝￣";

#ifdef  CHINESE
  static uns_chr *data_cn = (uns_chr *) "　�　隠�∞�ィΓВ┌�☆����！����０１２３４５６７８９�今察苅宗毅�\
�寸腺贈達庁釘藤韮硲稗複烹味唯裡錬丕傳劭咤圍妝孱廝悖截擅曖ぃ檻泯滷爍瓧癸磽筍�\
ｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ��������";
#endif /* CHINESE */
#ifdef  KOREAN
  static uns_chr *data_ko = (uns_chr *) "　�。■��ぃィΓВ┌�������������０１２３４５６７８９�今撮治州升�\
�寸腺贈達庁釘藤韮硲稗複烹味唯裡錬丕傳劭咤圍妝孱廝悖截擅曖棕檻泯滷爍瓧癸磽筍�\
ｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ��������";
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
  "ｧ", "ｱ", "ｨ", "ｲ", "ｩ", "ｳ", "ｪ", "ｴ", "ｫ", "ｵ",
  "ｶ", "ｶﾞ", "ｷ", "ｷﾞ", "ｸ", "ｸﾞ", "ｹ", "ｹﾞ", "ｺ", "ｺﾞ",
  "ｻ", "ｻﾞ", "ｼ", "ｼﾞ", "ｽ", "ｽﾞ", "ｾ", "ｾﾞ", "ｿ", "ｿﾞ",
  "ﾀ", "ﾀﾞ", "ﾁ", "ﾁﾞ", "ｯ", "ﾂ", "ﾂﾞ", "ﾃ", "ﾃﾞ", "ﾄ", "ﾄﾞ",
  "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ",
  "ﾊ", "ﾊﾞ", "ﾊﾟ", "ﾋ", "ﾋﾞ", "ﾋﾟ", "ﾌ", "ﾌﾞ", "ﾌﾟ",
  "ﾍ", "ﾍﾞ", "ﾍﾟ", "ﾎ", "ﾎﾞ", "ﾎﾟ",
  "ﾏ", "ﾐ", "ﾑ", "ﾒ", "ﾓ",
  "ｬ", "ﾔ", "ｭ", "ﾕ", "ｮ", "ﾖ",
  "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ",
  "ヮ", "ﾜ", "ヰ", "ヱ", "ｦ", "ﾝ",
  "ｳﾞ", "ヵ", "ヶ"
};                              /* 全角が混じってるので注意 */

 /**    上のhankdataが、実際に使う半角コードを表していないとき、実際のものに
        修正する。初期設定時に一回だけ呼ぶ */
void
hank_setup ()
{
  int i;
  char *s, orig_hnkak1;

  orig_hnkak1 = *hankdata[0];
  /*     *hankdata[] での半角文字の１バイトめ。半角文字の１バイトめだけが異なる
     ような他機種に移植するときは、HNKAK1のdefineを変えればOK。但しromkanの
     ソース中の半角文字（このファイルにのみ存在）もコンバートして、その機種
     に合わせるほうが望ましい。しかし、エディタでこのファイルを修正したり
     する場合に、半角文字の扱いがうまくいかない場合があるので、特に
     コンバートをしなくとも動作するように処置はしてある。それが、この
     hank_setup()である。hankdataは、初期設定時に hank_setup()によって
     実際の半角コードに直される。 */

  if (orig_hnkak1 == (char) HNKAK1)
    return;
  for (i = 0; i < numberof (hankdata); i++)
    {
      for (s = hankdata[i]; *s; s += 2)
        if (*s == orig_hnkak1)
          *s = HNKAK1;
    }
}

 /** かな→半角カタカナ。結果が二文字になることもある。*/
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

 /**    半角カタカナ→ひらがな。但し、濁点を持つ文字を一つにまとめては
        くれないので注意。*/
/* *INDENT-OFF* */
letter
to_zenhira (l)
    letter l;
/* *INDENT-ON* */
{
  letter retval;

  static uns_chr *data = (uns_chr *) "。「」、・をぁぃぅぇぉゃゅょっーあいうえおかきくけこさしすせそた\
ちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわん゛゜";

#ifdef  CHINESE
  static uns_chr *data_cn = (uns_chr *) "。仝々、，をぁぃぅぇぉゃゅょっーあいうえお宅きくけこ業しすせそた\
ちつてとなにぬね議はひふへほまみむめ匆やゆよらりるれろわん≦＜";
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

 /**    半角カタカナ→全角。但し、濁点を持つ文字を一つにまとめては
        くれないので注意。*/
/* *INDENT-OFF* */
letter
to_zenkata (l)
   letter l;
/* *INDENT-ON* */
{
  return (is_hankata (l) ? (l = to_zenhira (l), to_kata (l)) : l);
}

 /* ビットベクタの構成 */
#define bitvec(b0, b1, b2, b3, b4, b5, b6, b7) (                         \
        (char)b0 | ((char)b1 << 1) | ((char)b2 << 2) | ((char)b3 << 3) | ((char)b4 << 4) | ((char)b5 << 5) | \
        ((char)b6 << 6) | ((char)b7 << 7)                                                \
)

 /** charの配列 h をビットベクタと見てその第iビットをチェックする */
#define bitlook(h, i) (h[(i) >> 3] & (1 << ((i) & 7)))

#define KATRPT  0xA1B3          /* ヽ */
#define HIRRPT  0xA1B5          /* ゝ */
#define KATA_U  0xA5A6          /* ウ */
#define KAT_VU  0xA5F4          /* ヴ */
#define HIR_KA  0xA4AB          /* か */
#define HIR_HO  0xA4DB          /* ほ */
#define KAT_KA  0xA5AB          /* カ */
#define KAT_HO  0xA5DB          /* ホ */
#define HIR_HA  0xA4CF          /* は */
#define KAT_HA  0xA5CF          /* ハ */

 /**    後ろに半濁点をくっつける。結果は一又は二文字。*/
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

 /**    後ろに濁点をくっつける。結果は一又は二文字。*/
void
dakuadd (in, outp)
     letter in, **outp;
{
  static char flgbit[] = {
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* かがきぎくぐけげ */
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* こごさざしじすず */
    bitvec (1, 0, 1, 0, 1, 0, 1, 0),    /* せぜそぞただちぢ */
    bitvec (0, 1, 0, 1, 0, 1, 0, 0),    /* っつづてでとどな */
    bitvec (0, 0, 0, 0, 1, 0, 0, 1),    /* にぬねのはばぱひ */
    bitvec (0, 0, 1, 0, 0, 1, 0, 0),    /* びぴふぶぷへべぺ */
    bitvec (1, 0, 0, 0, 0, 0, 0, 0)     /* ほ */
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

 /** inで与えられたコードをbase進の数字にしてoutpに入れる。*/
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
