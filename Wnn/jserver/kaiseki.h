/*
 *  $Id: kaiseki.h,v 1.5 2003/05/11 18:35:54 hiroo Exp $
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

#ifndef JSERVER_KAISEKI_H
#define JSERVER_KAISEKI_H 1

#ifdef STDC_HEADERS
# include <sys/types.h>
#endif /* STDC_HEADERS */

#ifdef  GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v)  = (v)
#else
#define GLOBAL  extern
#define GLOBAL_VAL(v)
#endif /* GLOBAL_VALUE_DEFINE */

#include "commonhd.h"
#include "ddefine.h"

/*********************
 *  header of daemon 
**********************/
#define WNN_BUN		0
#define WNN_ZENKOUHO	1
#define WNN_SHO		0
#define WNN_DAI		1


#define _MININT 0x80000000
#define HIRA_HYOUKA -2000;      /* ��������Ф��λ���ʿ��̾��ɾ���� */
#define KATA_HYOUKA -1000;      /* ��������Ф��λ����Ҳ�̾��ɾ���� */

#define _DIVID(val, div)        (((int)(val) > (int)0) ? ((val) / (div)) : ((val) * (div)))

extern struct fzkkouho *getfzkoh_body ();
extern struct fzkkouho *getfzkoh1_body ();

#define getfzkoh(ichbnp_p,no) \
        (((int)(no) < (int)FZKIBNO) ? &((ichbnp_p)->fzkib[no]) : getfzkoh_body(ichbnp_p,no))

#define getfzkoh1(ichbnp_p,no) \
        (((int)(no) < (int)FZKIBNO) ? &((ichbnp_p)->fzkib1[no]) : getfzkoh1_body(ichbnp_p,no))

#define kan_ck_vector(h, v) \
    (((((int)*((v) + ((h) / (sizeof(int)<<3))) >> (int)((int)(h) % (int)((int)sizeof(int)<<(int)3))) & 1) == 1) ? \
        WNN_CONNECT_BK : WNN_NOT_CONNECT_BK)

GLOBAL struct FT *ft;           /* �����Ȥ���°��ơ��֥� */

/* ���ϥ��ꥢ�δ����ѤΥꥹ�� */
struct free_list
{
  struct free_list *lnk;
  int num;
};

#define FIRST_BZD_KOSUU 200     /* �ǽ�˼��Ф���ʸ����ϥ��ꥢ */
#define BZD_KOSUU       20      /* ­��󤫤ä����դ��ä����礭�� */

#define FIRST_SBN_KOSUU 200     /* �ǽ�˼��Ф���ʸ����ϥ��ꥢ */
#define SBN_KOSUU       20      /* ­��󤫤ä����դ��ä����礭�� */

#define FIRST_ICHBN_KOSUU       20      /* �ǽ�˼��Ф���°����ϥ��ꥢ */
#define ICHBN_KOSUU     4       /* ­��󤫤ä����դ��ä����礭�� */

#define FIRST_JKDBN_KOSUU       50      /* �ǽ�˼��Ф���ʸ�ἡ���䥨�ꥢ */
#define JKDBN_KOSUU     10      /* ­��󤫤ä����դ��ä����礭�� */

#define FIRST_JKSBN_KOSUU       50      /* �ǽ�˼��Ф���ʸ�ἡ���䥨�ꥢ */
#define JKSBN_KOSUU     10      /* ­��󤫤ä����դ��ä����礭�� */

#define FIRST_JKSONE_KOSUU      300     /* �ǽ�˼��Ф���ʸ�ἡ���䥨�ꥢ */
#define JKSONE_KOSUU    20      /* ­��󤫤ä����դ��ä����礭�� */

/************************************************
 *      ����ʸ�����ɾ���롼����                *
 ************************************************/
/*
#define HYOKAVAL(jiritugo_len, bnst_len) \
                hyoka1(0x80, 0, (jiritugo_len), (bnst_len), 0)
*/
/*
#define HYOKAVAL(hindo, jiritugo_len, bnst_len) \
                hyoka1(0, hindo, (jiritugo_len), (bnst_len), 0)
*/
#define HYOKAVAL(hindo, jiritugo_len, bnst_len) \
  (c_env->hindoval * (hindo) \
   + c_env->jirival * (jiritugo_len) \
   + c_env->lenval * (bnst_len) )

/************************************************
 *      ��ʸ�����ɾ���롼����                  *
 ************************************************/
#define hyoka1(HINDO, HINDO_IN, LENGTH, BUNLNGTH, PRIO) \
  (c_env->hindoval * (((HINDO) & 0x7f) + ((HINDO_IN) & 0x7f)) \
   + c_env->jirival * (LENGTH) \
   + c_env->lenval * (BUNLNGTH) \
   + c_env->jishoval * (PRIO)\
   + c_env->flagval * ((int)((HINDO) & 0x80) >> (int)7))

/************************************************
 *      ��ʸ�����ɾ���롼����                  *
 ************************************************/
/* ��ʸ���ɾ����
        sbn_val * ��ʸ���ɾ���ͤ��� / ��ʸ���
           + sbn_cnt_val * ��ʸ���
           + dbn_len_val * ��ʸ��Ĺ
*/
#define hyoka_dbn(SON_V, SBNCNT, DAILEN) \
        ((c_env->sbn_val * (SON_V)) / (SBNCNT) \
         + c_env->sbn_cnt_val * (SBNCNT) \
         + c_env->dbn_len_val * (DAILEN))

/* ��ʸ���ɾ���ͤ�����ʸ�����³�Ǥ��뤫�ݤ����Ѥ��롣
   divid :  1   ����ʸ�����³�Ǥ���    hyouka * 2
            0   ��ʸ�����Ƭ            hyouka
           -1   ����ʸ�����³�Ǥ��ʤ�  hyouka / 2
*/
#define DIVID_HYOUKA(hyouka, divid) \
        (((divid) == 0) ? (hyouka) : \
                (((divid)) > 0 ? ((hyouka) << 1) : ((hyouka) >> 1)))

/*********************
 * kaiseki work area
**********************/
GLOBAL w_char *bun;		/* D */
GLOBAL w_char giji_eisuu[20];	/* �����ֱѿ��פ���� */
GLOBAL size_t maxchg;
GLOBAL int *maxj;		/* maxj is counts to entries in jmt_ */

/* jmt_ptr is used in jmt0.c only. (untrue! do_dic_no.c use it 2003.05.09)
   but it must be changed by clients.
   jishobiki does not use it. this is sent to jishobiki by arguments
   */

GLOBAL struct jdata *j_e_p;	/* holds the current point to which jmtw_ is used */
GLOBAL struct jdata **jmt_;
GLOBAL struct jdata *jmtw_;
GLOBAL struct jdata **jmt_end;
GLOBAL struct jdata *jmtw_end;
GLOBAL struct jdata **jmt_ptr;
GLOBAL struct jdata ***jmtp;


/********************************
 *      ������Ω����ʻ�        *
 ********************************/
#ifdef nodef
GLOBAL int sentou_no;		/* ����Ƭ��ʸ����Ƭ���������	*/
GLOBAL int suuji_no;		/* �ֿ����׿�������		*/
GLOBAL int katakanago_no;	/* �֥��ʡ׳����ʤ� ̾������	*/
GLOBAL int eisuu_no;		/* �ֱѿ���			*/
GLOBAL int kigou_no;		/* �ֵ����			*/
GLOBAL int toji_kakko_no;	/* ���ĳ�̡�			*/
GLOBAL int fuzokugo_no;		/*   ��°�����			*/
GLOBAL int kai_kakko_no;	/* �ֳ���̡�			*/
GLOBAL int giji_no;		/*   ����			*/
#endif /* nodef */

#define sentou_no       0	/* ����Ƭ��ʸ����Ƭ���������	*/
#define suuji_no        1	/* �ֿ����׿�������		*/
#define katakanago_no   2	/* �֥��ʡ׳����ʤ� ̾������	*/
#define eisuu_no        3	/* �ֱѿ���			*/
#define kigou_no        4	/* �ֵ����			*/
#define toji_kakko_no   5	/* ���ĳ�̡�			*/
#define fuzokugo_no     6	/*   ��°�����			*/
#define kai_kakko_no    7	/* �ֳ���̡�			*/
#define giji_no         8	/*   ����			*/

#endif /* JSERVER_KAISEKI_H */

