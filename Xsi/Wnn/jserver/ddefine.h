/*
 *  $Id: ddefine.h $
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

/************************
 * define of daemon
 ************************/

#include <ctype.h>

#define SIZE_JISHOTABLE 1000
#define SIZE_JISHOHEAP 2000

#define KANGO_HINSI_MX  256     /* ������ʻ���κ���� */
#define KANGO_VECT_L (KANGO_HINSI_MX+8*4-1)/(8*4)       /* ������³�٥�����Ĺ�� */
#define FZKVECT_L       5       /* ��°��٥�����Ĺ�� */
#define VECT_L  KANGO_VECT_L + FZKVECT_L        /* ��³�٥�����Ĺ�� */
#ifdef nodef                    /* This define doesn't need anymore, KUWARI */
#ifdef  CHINESE
#define KANGO_VECT_KOSUU        256     /* ������³�٥����κ���� */
#else
#define KANGO_VECT_KOSUU        20      /* ������³�٥����κ���� */
#endif /* CHINESE */
#endif /* nodef */

struct fzkkouho
{                               /* used in reffering to fuzokugo jouhou 
                                   fuzokugo jouhou is stored using this struct */
  int offset;
  int vector[KANGO_VECT_L];     /* ���콪ü�٥��� */
};

/* macro C(i , j)  is useful for finding a pointer which points out 
   the first jishoentry whose yomi starts at i and endes at j
   which returns NULL if there is no entry which matches this
*/

#define C(i,j)  (j > maxj[i] ? (struct jdata *)NULL : *(jmtp[i] + (j - i)))


/* #define      FZKIBNO 10 */
#define FZKIBNO 5

/* ��°����Ϸ�� */
struct ICHBNP
{
  struct ICHBNP *next_p;
  struct fzkkouho fzkib[FZKIBNO];       /* vec �β��Ϸ�� */
  struct fzkkouho fzkib1[FZKIBNO];      /* vec1 �β��Ϸ�� */
};
/* �¤ϡ�fzkib1->offset �ϡ����ߤ����äƤ��� */

#define SV_KOSUU        3

#define UJIS_A          0xa3c1  /* �� */
#define UJIS_Z          0xa3da  /* �� */
#define UJIS_a          0xa3e1  /* �� */
#define UJIS_z          0xa3fa  /* �� */
#ifdef CHINESE
#define UJIS_comma      0xa3ac  /* �� */
#define S_PAREN         0xa1ae  /* �� */
#define E_PAREN         0xa1bf  /* �� */
#else
# ifdef KOREAN
#define UJIS_comma      0xa3ac  /* �� */
#define S_PAREN         0xa1ae  /* �� */
#define E_PAREN         0xa1bd  /* �� */
# else
#define UJIS_comma      0xa1a4  /* �� */
#define S_PAREN         0xa1c6  /* �� */
#define E_PAREN         0xa1db  /* �� */
# endif
#endif /* CHINESE */
/*
#define isdigit(x)      ((unsigned)((x) - '0') <= ('9' - '0'))
#define isalpha(x)      ((unsigned)((x) - 'A') <= ('Z' - 'A') || \
                            (unsigned)((x) - 'a') <= ('z' - 'a'))
#define isalnum(x)      (isalpha(x) || isdigit(x))
*/
#ifdef  isspace
#       undef   isspace         /* ctype.h �� isspace() �ϻȤ�ʤ� */
#endif
#define isspace(x)      (((x) == ' ') || ((x) == 0x09))
#define isjspace(x)     ((x) == 0xa1a1)
#define isparen_s(x)    (/* (x) == '\"' || */ (x) == '`' || \
/* ����� */             (x) == '(' || (x) == '<' || \
                         (x) == '[' || (x) == '{')
#define isparen_e(x)    ((x) == '\"' || (x) == '\'' || \
/* �ĳ�� */             (x) == ')' || (x) == '>' || \
                         (x) == ']' || (x) == '}')

#define isjdigit(x)     ((unsigned)((x) - S_NUM) <= (E_NUM - S_NUM))
#define isjalpha(x)     ((unsigned)((x) - UJIS_A) <= (UJIS_Z - UJIS_A) || \
                         (unsigned)((x) - UJIS_a) <= (UJIS_z - UJIS_a))
#define isjalnum(x)     (isjalpha(x) || isjdigit(x))
#define isjhira(x)      ((unsigned)((x) - S_HIRA) <= (E_HIRA - S_HIRA))
#define isjkata(x)      ((unsigned)((x) - S_KATA) <= (E_KATA - S_KATA))
#define isj1bytekana(x) ((unsigned)((x) - S_HANKATA) <= (E_HANKATA - S_HANKATA))
# define S_KANJI        0xb0a1  /* 16�� */
# define E_KANJI        0xfefe  /* 94�� */
#ifdef  CHINESE
# define S_HANJI        0x20a1  /* 16�� */
# define E_HANJI        0x7efe  /* 94�� */
#define isjkanji(x)     (((unsigned)((x) - S_KANJI) <= (E_KANJI - S_KANJI)) || \
                         ((unsigned)((x) - S_HANJI) <= (E_HANJI - S_HANJI)))
#define isjparen(x)     ((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN) || \
                         (x)==0xa3a8 || (x)==0xa3a9 ||  \
                         (x)==0xa3bc || (x)==0xa3be || \
                         (x)==0xa3db || (x)==0xa3dd || \
                         (x)==0xa3fb || (x)==0xa3fd )
                        /*  for (  )  <  >  [  ]  { } */
#define isjparen_s(x)   (((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN)) && \
                             (((x) & 0x01) == 0) || \
                         (x)==0xa3a8 || (x)==0xa3bc || \
                         (x)==0xa3db || (x)==0xa3fb )

#define isjparen_e(x)   (((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN)) && \
                             (((x) & 0x01) == 1) || \
                         (x)==0xa3a9 || (x)==0xa3be || \
                         (x)==0xa3dd || (x)==0xa3fd )
#else
# ifdef KOREAN
#define S_HANJA 0xcaa1          /* ʡ */
#define E_HANJA 0xfdfe          /* �� */
#define isjkanji(x)     ((unsigned)((x) - S_HANJA) <= (E_HANJA - S_HANJA))
#define isjparen(x)     ((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN) || \
                         (x)==0xa3a8 || (x) ==0xa3a9 || \
                         (x)==0xa3bc || (x) ==0xa3be || \
                         (x)==0xa3db || (x) ==0xa3dd || \
                         (x)==0xa3fb || (x) ==0xa3fc )
#define isjparen_s(x)   (((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN)) && \
                            (((x) & 0x01) == 0) || \
                         (x)==0xa3a8 || (x)==0xa3bc || \
                         (x)==0xa3db || (x)==0xa3fb)
#define isjparen_e(x)   (((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN)) && \
                            (((x) & 0x01) == 1) || \
                         (x) ==0xa3a9 || (x) ==0xa3be || \
                         (x) ==0xa3dd || (x) ==0xa3fc )
#define ishangul(x)     ((unsigned)((x) - S_HANGUL) <= (E_HANGUL - S_HANGUL))
# else /* JAPANESE */
#define isjkanji(x)     ((unsigned)((x) - S_KANJI) <= (E_KANJI - S_KANJI))
#define isjparen(x)     ((unsigned)((x) - S_PAREN) <= (E_PAREN - S_PAREN))
#define isjparen_s(x)   (isjparen(x) && (((x) & 0x01) == 0))
#define isjparen_e(x)   (isjparen(x) && ((x) & 0x01))
# endif /* KOREAN */
#endif /* CHINESE */

#define UJIS_DKT        0xa1ab  /* �� */
#define UJIS_HDK        0xa1ac  /* �� */

#define BAR_CODE(x)     ((x) == BAR)
#define TOKU(x) (((x) == UJIS_DKT) || ((x) == UJIS_HDK))

#define TOKUSYU(x)      (TOKU(x) || isjkata(x) || isj1bytekana(x))
        /* this resembles macro before */

#define kuten(x)        (((x)==KUTEN_NUM)? True:False)
#define touten(x)       (((x)==TOUTEN_NUM)? True:False)

/* �������ü��³�٥��ȥ�ι�¤ */
struct kangovect
{
  unsigned int vector[VECT_L];  /* ����٥��ȥ� */
};

struct SYO_BNSETSU
{
  struct SYO_BNSETSU *lnk_br;   /* ¾�ξ�ʸ��ؤΥݥ��� */
  struct SYO_BNSETSU *parent;   /* ������ξ�ʸ��ؤΥݥ��� */
  short j_c;                    /* ���� top index */
  short i_jc;                   /* ���� end index */
  short bend_m;                 /* ��ʸ�� end char index */
  int v_jc;                     /* ����ʸ���ɾ���� */
  int son_v;                    /* ���ξ�ʸ��ޤǤ�ɾ���ͤι�� */
  struct jdata *jentptr;        /* ����ǡ����ؤΥݥ��� */
  int t_jc;                     /* �ʻ졢���پ���Υ���ǥå��� */
  int kangovect;                /* ��³�٥��ȥ�ơ��֥�Υ���ǥå��� */
  int hinsi_fk;                 /* ������ʻ�No.(ʣ���ʻ��ޤ�) */
  short kbcnt;                  /* ��ʸ����ǲ���ʸ���ܤ� */
  short reference;              /* ���ξ�ʸ��Υ�ե���󥹿� */
  short status;                 /* ����ʸ�����³�Ǥ��뤫 */
  short status_bkwd;            /* ���ʸ��Ȥ���³�Ǥ��뤫 */
};

struct BZD
{
  struct BZD *lnk_br;           /* ¾����ʸ����� */
  struct BZD *lnk_son;          /* ������ʸ��ؤ� pointer */
  short j_c;                    /* ���� top index */
  short bend_m;                 /* ������ʸ��� end char index */
  int v_jc;                     /* ������ʸ���ɾ���� */
  int son_v;                    /* ������ʸ���ɾ���� */
  struct SYO_BNSETSU *sbn;      /* ��Ƭ�ξ�ʸ�� */
  short sbn_cnt;                /* ������ʸ��������뾮ʸ��ο� */
  short kbcnt;                  /* ������ʸ������˲�ʸ����Ϥ�����  */
};

struct JKT_SONE
{
  struct JKT_SONE *lnk_br;      /* ¾�ξ�ʸ��ؤΥݥ��� */
  short i_jc;                   /* ���� end index */
  short v_jc;                   /* ����ʸ���ɾ���� */
  struct jdata *jentptr;        /* ����ǡ����ؤΥݥ��� */
  int t_jc;                     /* �ʻ졢���پ���Υ���ǥå��� */
  int hinsi_fk;                 /* ������ʻ�No.(ʣ���ʻ��ޤ�) */
  short status_bkwd;            /* ���ʸ��Ȥ���³�Ǥ��뤫 */
};

struct JKT_SBN
{
  struct JKT_SBN *lnk_br;       /* ¾�ξ�ʸ��ؤΥݥ��� */
  struct JKT_SBN *parent;       /* ������ξ�ʸ��ؤΥݥ��� */
  short j_c;                    /* ���� top index */
  short bend_m;                 /* ��ʸ�� end char index */
  struct JKT_SONE *sbn;         /* ¾�ξ�ʸ��ؤΥݥ��� */
  int kangovect;                /* ��³�٥��ȥ�ơ��֥�Υ���ǥå��� */
  short kbcnt;                  /* ��ʸ����ǲ���ʸ���ܤ�  */
  short reference;              /* ���ξ�ʸ��Υ�ե���󥹿� */
  short status;                 /* ����ʸ�����³�Ǥ��뤫 */
};

struct JKT_DBN
{
  struct JKT_DBN *lnk_br;       /* ¾�ξ�ʸ��ؤΥݥ��� */
  short j_c;                    /* ���� top index */
  short bend_m;                 /* ��ʸ�� end char index */
  struct JKT_SBN *sbn;          /* ¾�ξ�ʸ��ؤΥݥ��� */
  int v_jc;                     /* ����ɾ����ɾ���� */
  short sbn_cnt;                /* ������ʸ��������뾮ʸ��ο� */
};

struct DSD_SBN
{
  short bun_m;                  /* ����ʸ��� end char index */
  short bun_jc;                 /* ����ʸ��� top char index */
  short i_jc;                   /* ����ʸ��μ�Ω�� end char index */
  struct jdata *jentptr;        /* ����ʸ��μ��� entry pointer */
  int t_jc;                     /* ���伫Ω����ʻ� & ���� index */
  int hinsi;                    /* �ʻ� */
  int kangovect;                /* ��³�٥��ȥ�ơ��֥�Υ���ǥå��� */
  int v_jc;                     /* ��ʸ���ɾ���� */
  short status;                 /* ����ʸ�����³�Ǥ��뤫 */
  short status_bkwd;            /* ���ʸ��Ȥ���³�Ǥ��뤫 */
};

struct DSD_DBN
{
  short bun_m;                  /* ����ʸ��� end char index */
  short bun_jc;                 /* ����ʸ��� top char index */
  struct DSD_SBN *sbn;          /* ��ʸ����Ϸ�̤ؤΥݥ��� */
  short sbncnt;                 /* ��ʸ��� (������ξ��ϡ��������)
                                   DSD_SBN�ϡ�*sbn ���� sbncnt �������� */
  int v_jc;                     /* ��ʸ���ɾ���� */
};

#ifdef  nodef                   /* move to jslib.h */
/* ����ʸ���ľ�������򤷤����� */
#define HIRAGANA        -1      /* �Ҥ餬�� */  /* �ɤߤΤޤ� */
#define KATAKANA        -11     /* �������� */
/* ���� */
#define NUM_HAN         -2      /* Ⱦ�ѿ��� */  /* �ɤߤΤޤ� */
#define NUM_ZEN         -12     /* ���ѿ��� */  /* ������ */
#define NUM_KAN         -13     /* ������ */    /* ���� */
#define NUM_KANSUUJI    -15     /* ������ */    /* ɴ�󽽻� */
#define NUM_KANOLD      -16     /* ������ */    /* ��ɴ������ */
#define NUM_HANCAN      -17     /* Ⱦ�ѿ��� */  /* 1,234 */
#define NUM_ZENCAN      -18     /* ���ѿ��� */  /* ���������� */
/* �ѿ� */
#define ALP_HAN         -4      /* Ⱦ�� */      /* �ɤߤΤޤ� */
#define ALP_ZEN         -30     /* ���� */
/* ���� */
#define KIG_HAN         -5      /* Ⱦ�� */      /* �ɤߤΤޤ� */
#define KIG_JIS         -40     /* ����(JIS) */
#define KIG_ASC         -41     /* ����(ASC) */
#endif

#define YES             1
#define NO              0
#define UN_KNOWN        -1

#define NOTHING         0
