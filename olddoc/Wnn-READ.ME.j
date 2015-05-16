/*
 * $Id: READ.ME.j,v 1.10.2.1 1999/02/08 02:15:23 yamasita Exp $
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
 */

��	�ף�̾�����Ѵ������ƥ� �С������4.2

�ף�̾�����Ѵ������ƥ�ϡ�������ؿ������ϸ���ꡢ�����������ҡ�
������ҥ����ƥå��Σ��Ԥˤ�äƳ�ȯ���줿�ͥåȥ����̾�����Ѵ�����
�ƥ�Ǥ���

����ϡ��ף�̾�����Ѵ������ƥ�ΥС������4.2�Ǥ���

����Υ�꡼���Ǥϡ��ʲ��ε�����ư���ǧ����Ƥ��ޤ���

SUN �� SunOS 4.0.3�� SunOS 4.1.1�� SunOS 5.3

OMRON LUNA �� UniOS-B (BSD4.3)��UniOS-U (SystemV R2.1)��UniOS-Mach (MACH)
              DG/UX 5.4.2



��	���󥹥ȡ���

 �������ե�����ˤϡ���ƥ��ʸ���󡢤���ӡ������Ȥ���ECU�����ɤǽ񤫤��
 ���ޤ�������ѥ���κݤˤϡ�8�ӥåȤ��̤륳��ѥ���ǡ�make ���Ʋ�������

 ��Imake�ǥ���ѥ���
    (��)����ѥ����¹Ԥ������ˡ������Ρ֥���ѥ�������ˡפ��ɤ߲�������

    (X11R6�������ĥ꡼�ξ��)
      Xsi��contrib/programs/Xsi�Ȥ����֤�����ǡ�
	% cd ..
	% make World -f Makefile.ini

    (X11R6�����󥹥ȡ��뤵��Ƥ�����)
      Xsi�򹥤��ʥǥ��쥯�ȥ���֤�����ǡ�
	% cd ..
	% make World -f Makefile.inst

    (X11R5�ξ��)
      Xsi��contrib/im/Xsi�Ȥ����֤�����ǡ�
	% cd ..
	% make World -f Makefile.R5ini

 ��Imake�ǥ��󥹥ȡ���

	% make install

 
[����ѥ��������]

 Project.tmpl ($(TOP)/contrib/im/Xsi/config�ˤ���) �ˡ����󥹥ȡ��뤹��
 �ǥ��쥯�ȥ���ѿ��������Ĥ�����ޤ�(�ʤ���X11R5�ξ��ϡ�Project.tmpl
 �ϡ�$(TOP)/contrib/im/Xsi/configR5�ˤʤ�ޤ�)�����줾��ΰ�̣�ϰʲ���
 �̤�Ǥ���
 
������ѥ���δĶ�

IMTOP = $(CONTRIBSRC)/programs/Xsi
   Wnn���֤��������ĥ꡼��ΰ��֤���ꤷ�ޤ���

�����󥹥ȡ��뤹����ʬ����ꤹ���ѿ�
       �ѿ�                  ���󥹥ȡ��뤵�����
----------------------------------------------------------
#define BuildWnn	       ���ܸ��Wnn����ʬ
#define BuildCWnn	       �����Wnn����ʬ
#define BuildKWnn	       �ڹ���Wnn����ʬ
#define BuildXwnmo	       Xwnmo
#define BuildJserver	       ���ܸ��Ѵ�������
#define BuildJlib	       Wnn�Υ饤�֥��
#define BuildWnnClients	       ���ܸ��ѤΥ��饤�����
#define BuildWnnDicUtil	       ���ܸ��Ѥμ��񥳥ޥ��
#define BuildPubdic	       ���ܸ��Ѥμ���(Pubdic)
#define BuildWnnConsDic	       ���ܸ��Ѥ�ñ��������
#define BuildWnnMan	       ����饤��ޥ˥奢��
#define BuildJlibV3	       WnnV3�ߴ��ѥ饤�֥��
#define BuildCserver	       ����ʴ��λ����Ѵ�������
#define BuildTserver	       ��������λ����Ѵ�������
#define BuildCWnnClients       �����ѤΥ��饤�����
#define BuildCWnnDicUtil       �����Ѥμ��񥳥ޥ��
#define BuildCDic	       ����ʴ��λ����Ѥμ���
#define BuildTDic	       ��������λ����Ѥμ���
#define BuildKserver	       �ڹ���Ѵ�������
#define BuildKWnnClients       �ڹ���ѤΥ��饤�����
#define BuildKWnnDicUtil       �ڹ���Ѥμ��񥳥ޥ��
#define BuildKDic	       �ڹ���Ѥμ���

�����󥹥ȡ���ξ��
WNNWNNDIR = /usr/local/lib/wnn ����ե����롢����ʤɤΥ��󥹥ȡ������
WNNBINDIR = /usr/local/bin     ���ޥ����Υ��󥹥ȡ������
                               ���Υǥ��쥯�ȥ�β���Wnn4,cWnn4�ʤɤΥǥ�
                               �쥯�ȥ꤬�������졢�����˥��󥹥ȡ��뤵��롣
WNNINCDIR = $(INCROOT)         �إå��ե�����Υ��󥹥ȡ������
WNNLIBDIR = $(USRLIBDIR)       Wnn�Υ饤�֥��Υ��󥹥ȡ������

 ���󥹥ȡ���δĶ����ѹ����������ϡ����� Project.tmpl ���Խ����ޤ���

 �ޤ���Project.tmpl ($(TOP)/contrib/im/Xsi/config�ˤ���) �� �ѿ� WNNOWNER
 ������ޤ�(�ʤ���X11R5�ξ��ϡ�Project.tmpl �ϡ�
 $(TOP)/contrib/im/Xsi/configR5�ˤʤ�ޤ�)��
 ����ϡ�Wnn �Υץ����(uum �����) ��ɤΥ桼���θ��¤�ư����������
 ���ޤ����С������3�ޤǤǤϡ�jserver �ϡ�root �θ��¤�ư�����Ƥ����Τ�
 ������root ��ư�����Τϴ������⤤���ᡢwnn �ѤΥ桼�����äơ�����
 ���¤� jserver ������ư������ �Ȥ�侩���ޤ���
 �ǥե���ȤǤϡ�wnn �ˤʤäƤ��ޤ���

 uum �ϡ������ʤ� root �ǡ� s�ӥåȤ�Ω�äƤ���ɬ�פ�����ޤ���

 ���Ǥ˼��񤬥��󥹥ȡ��뤵��Ƥ���Ķ��˥��󥹥ȡ��뤷����硢������
 ���٥ե����뤬���ѤǤ��ʤ��ʤ�ޤ������Τ��ᡢ���󥹥ȡ�����˼���
 ���Ǥ�¸�ߤ�����ϡ�����Υ��󥹥ȡ���ϹԤʤ��ʤ��褦�ˤʤäƤ��ޤ���
 �⤷������Ū�˼�����񤭤��������ϡ�Project.tmpl ���뤤��¾��config
 �ե�����˲����ιԤ��ɲä��Ʋ�������

	#define WnnInstallDicForcibly	YES
     ( #�����˶���TAB������ʤ��ǲ�������)
 

 ���Ѥ���OS���ե��󥯥���󥭡���10���¿�����ݡ��Ȥ��Ƥ�����ϡ�
 Wnn/include/commonhd.h�˲����ιԤ��ɲä��Ʋ�������

	#define SUPPORT_TWODIGIT_FUNCTIONS
     ( #�����˶���TAB������ʤ��ǲ�������)

 �ʾ�[����ѥ��������]����

��	manual �ʳ��Υǥ��쥯�ȥ�ϡ��ʲ����̤�Ǥ���

conv
	����С��ȡ�����(�ե��󥯥���󥭡����Ǥ����������Ŭ���ʥ���
	�ɤ��Ѵ�����)�Υ�������¸�ߤ��ޤ���
etc
	ʣ���Υǥ��쥯�ȥ�Ƕ�ͭ����륽�����ե����뤬¸�ߤ��ޤ���
include 
	�إå����ե����뤬¸�ߤ��ޤ���
jd
	uum, jserver, ���޻���̾�Ѵ��ʤɤν�����ե����롢
	�ʻ�ե����뤬¸�ߤ��ޤ������Υǥ��쥯�ȥ�����Ƥϡ�
	/usr/local/lib/wnn/ja_JP �˥��󥹥ȡ��뤵��ޤ���

	    jserverrc - jserver ��Ω��������ե�����Ǥ���
	    uumrc	uum ��Ω��������ե�����Ǥ���
	    uumkey	uum �Υ����Х��������ե�����Ǥ���
	    wnnenvrc	uum ���ѻ��μ�����������ե�����Ǥ���
	jd �β��ˤϡ������������Τ�Τ��Ѱդ���Ƥ��ޤ���
	���ߤ˹�碌�Ƹ����Ѳ�������

jlib
	�饤�֥��Υ�������¸�ߤ��ޤ���
jlib.V3
	�С������3���Ф���ߴ��饤�֥��Υ�������¸�ߤ��ޤ���
jserver
	jserver �Υ�������¸�ߤ��ޤ���
jutil
	����桼�ƥ���ƥ��Υ�������¸�ߤ��ޤ���
		atod	 ����κ���
		dtoa	 ����Υƥ����Ȥؤ��ѹ�
		oldatonewa �С�����󣳤ޤǤμ���Υƥ����ȷ�����
			�С�����󣴤Υƥ����ȷ������ѹ�
		wnnstat	 jserver �ξ���(�桼�����Ķ����ե����롢����)��Ĵ�٤롣
		wnnkill	 jserver ��λ�����롣
		wnntouch ����ե�����ʤɤΥե������FID �����ե�����Υإå���
			���äƤ����ΤȰۤʤ���˰��פ����롣
		wddel	ñ������Хå�Ū�˹Ԥʤ�
		wdreg	ñ����Ͽ��Хå�Ū�˹Ԥʤ�
pubdic 			
	pubdic �μ���¸�ߤ��ޤ���
	����ϡ� �հ��������Ǻ�äƤ���ޤ���
romkan
	���޻���̾�Ѵ��Υ�������¸�ߤ��ޤ���
uum
	�ե��ȥ���ɡ��ץ��å� uum �Υ�������¸�ߤ��ޤ���
wnncons
	Wnn���󥽥����������ʪ��¸�ߤ��ޤ���
