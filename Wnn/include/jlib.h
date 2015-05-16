/*
 *  $Id: jlib.h,v 1.4 2001/06/18 09:09:33 ura Exp $
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

/*
        Nihongo Henkan  Library Header File

        08/Jul/1987     (wnn_errorno)
        24/Apr/1987
        09/Mar/1987     (update server_dead)
        25/Feb/1987     (update wchar_t)
        18/Feb/1987     (update Comment)
        28/Nov/1986
        23/Oct/1986
*/

#ifndef WNN_JLIB_H
#define WNN_JLIB_H

#include <setjmp.h>

#ifndef w_char
#define w_char  unsigned short
#endif /* w_char */
#ifndef WCHAR_T
#define wchar_t w_char
#define wchar   w_char
#define WCHAR_T
#endif /* WCHAR_T */

#define JLIB

#define S_BUF_SIZ       1024    /* NEVER change this */
#define R_BUF_SIZ       1024    /* NEVER change this */



struct kouho_entry
{
  int s_ichi;                   /*      ���ʥХåե��ؤΥ��ե��å�      */
  int jl;                       /*      ��Ω���Ĺ��                    */
  int fl;                       /*      ��°���Ĺ��                    */
  int pl;                       /*      �ǥ����ץ쥤����Ĺ��            */
  int jishono;                  /*      ��Ω��μ����ֹ�                */
  int serial;                   /*      ��Ω����ֹ�                    */
  w_char *k_data;               /*      �����Хåե��ؤΥݥ���        */
};
#define KOUHO_ENT       struct  kouho_entry


struct jikouho_entry
{
  int jl;                       /*      ��Ω���Ĺ��                    */
  int jishono;                  /*      ��Ω��μ����ֹ�                */
  int serial;                   /*      ��Ω����ֹ�                    */
  w_char *k_data;               /*      �����Хåե��ؤΥݥ���        */
};
#define JIKOUHO_ENT     struct  jikouho_entry


struct bunjoho
{
  w_char *kana_buf;             /*      ���ʥХåե�                    */
  int kana_size;                /*      ���ʥХåե���Ĺ��              */
  KOUHO_ENT *klist;             /*      ����ꥹ�ȡ��Хåե�            */
  int klist_size;               /*      ����ꥹ�ȡ��Хåե���Ĺ��      */
  w_char *kanji_buf;            /*      �����Хåե�                    */
  int kanji_buf_size;           /*      �����Хåե� ��Ĺ��             */
};


struct jikouhojoho
{
  JIKOUHO_ENT *jlist;           /*      ������Хåե�                  */
  int jlist_size;               /*      ������Хåե���Ĺ��            */
  w_char *kanji_buf;            /*      �����Хåե�                    */
  int kanji_buf_size;           /*      �����Хåե���Ĺ��              */
};
#define JIKOUHOJOHO             struct  jikouhojoho


struct jishojoho
{
  w_char *k_data;               /*      �����Хåե��ؤΥݥ���        */
  int bumpo;                    /*      ʸˡ����                        */
  int hindo;                    /*      ����                            */
  int jisho;                    /*      �����ֹ�                        */
  int serial;                   /*      �����ֹ�                        */
};
#define JISHOJOHO       struct  jishojoho


struct dicinfo
{
  int dic_no;                   /*      �����ֹ�                        */
  int dic_size;                 /*      ñ���                          */
  int ttl_hindo;                /*      �������                        */
  int dic_type;                 /*      1:�桼��,2:�����ƥ�             */
  int udp;                      /*      0:������Ǥʤ�,1:������         */
  int prio;                     /*      ����ץ饤����ƥ�              */
  int rdonly;                   /*      0:�饤�Ȳ�ǽ,1:�꡼�ɥ���꡼   */
  char *file_name;              /*      ����ե�����̾                  */
  char *h_file_name;            /*      ���٥ե�����̾                  */
};
#define DICINFO struct  dicinfo

extern jmp_buf jd_server_dead_env;      /* �����Ф�������������Ǥ���env */
extern int jd_server_dead_env_flg;      /* jd_server_dead_env��ͭ�����ݤ�  */
extern int wnn_errorno;         /* Wnn�Υ��顼�Ϥ����ѿ�����𤵤�� */

/* header file for dic_syurui */
#define NO_EXIST -1
#define NOT_A_JISHO 0
#define USER_DIC 1
#define SYSTEM_DIC 3

struct jwho
{
  int sd;                       /* socket discripter in jserver */
  char *user_name;              /* user name */
  char *host_name;              /* host name */
};

#define JWHO struct jwho


#endif  /* WNN_JLIB_H */
