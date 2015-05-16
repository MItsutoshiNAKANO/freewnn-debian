/*
 * $Id: README.j,v 1.3.2.1 1999/02/08 08:14:41 yamasita Exp $
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
		XWNMO - �ء�����ץåȡ��ޥ͡�����
	     (+ XJUTIL - ����桼�ƥ���ƥ����ޥ͡�����)

���Υǥ��쥯�ȥ�ˤϡ�XWNMO�Υ�������XJUTIL�Υ�����������ӡ��ޥ����������
�ޥȥ�Υ�����������ޤ���

	XWNMO - XIM(X Input Method)�Τ���Υ���ץåȡ��ޥ͡�����
	XJUTIL - (XWNMO���鵯ư�����)����桼�ƥ���ƥ����ޥ͡�����

XWNMO�ϡ�XIM�饤�֥����Ф���¿��������Ϥ��󶡤��륤��ץåȡ��ޥ͡�����Ǥ���
XWNMO��XIM�饤�֥��֤Υץ�ȥ���ϡ�X11R5�ξ��� Xsi Protocol �ˤ��
���ꤵ��Ƥ���ޤ������� Xsi Protocol �ϡ��Хå��������������Ѥ��Ƥ��ޤ���
�ʤ��ʤ顢�Хå�����������ϡ�������Ʊ���̿��Ǥ��뤿�ᡢ�����μ�ꤳ�ܤ���
���꤬�ʤ�����Ǥ���X11R6�ξ��� XIM Protocol �ˤ�구�ꤵ��Ƥ��ޤ���
���� XIM Protocol �ϡ�X Consortium Standard �Ǥ���XWNMO�ϡ�XIM Protocol��
�Хå�����������Τߤ���Ѥ��Ƥ��ޤ���

�����ˤĤ��Ƥξܺ٤ϡ�
	xwnmo/README �� xwnmo �Υޥ˥奢��	XWNMO �ˤĤ���
	xjutil/README �� xjutil �Υޥ˥奢��	XJUTIL �ˤĤ���
	X11R5/doc/I18N/Xsi/Xim/XimProto.man	Xsi Protocol �ˤĤ���
	X11R6/doc/specs/XIM/xim.ms		XIM Protocol �ˤĤ���
�򻲾Ȳ�������

����ץåȡ��ޥ͡������XIM�饤�֥��ι���

 +------------------------+         +----------------+  +----------------+
 |     Application        |     +---+     XWNMO      +--+     XJUTIL     |
 |   +--------------------+     |   +-------+--------+  +-------+--------+
 |   |     Widgets        |     |           |                   |
 |   +--------------------+     |           +-------------------+
 |   |      Toolkit       |     |           |
 +---+--------------------+     |           |   +---------+
 |       +------------+   |     |           +---+ Cserver |
 | Xlib  | XIM library+---+-----+           |  +---------++
 |       +------------+   |                 +--+ Jserver |
 +------------------------+                    +---------+


[X11R6�Ǥ�ư��ˤĤ���]
��������̾�ˤĤ���
  XWNMO�������Υ�����Ȥ��ơ�EUC�Υ��֥��åȤǤ���
      ja_JP.eucJP(���ܸ�EUC)
      zh_CN.eucCN(����ʴ��λ���EUC)
      zh_TW.eucTW(��������λ���EUC)
      ko_KR.eucKR(�ڹ��EUC)
  ����Ѥ��Ƥ��ޤ�����������OS�ˤ�äƤϡ�Ʊ�����ƤΥ�����Ǥ��äƤ�̾����
  ���Ȥʤ��Τ�����ޤ��Τǡ��嵭�Τ�ΰʳ��ξ��ϡ�ximconf���ѹ�����ɬ��
  ������ޤ���

����������ϤˤĤ���
  XWNMO(cWnn)�Ǥ�����ν������ȼ��Υ����ɥ��åȤǤ���Sisheng�Ȥ�����Τ�
  ���Ѥ��Ƥ��ޤ��������ɽ�����뤿��ˡ�Sisheng�ѤΥե���Ȥ��Ѱդ��ޤ�����
  Xsi/Xwnmo/X11R6/fonts�β��ˡ�26,24,18,16�ɥåȤΤ�Τ�����ޤ��Τǡ�ɬ��
  �˱����ƥ��󥹥ȡ��뤷�ƻ��Ѥ��Ƥ����������ޤ���������ǡ����١������ѹ�
  ��ɬ�פǤ���Xsi/Xwnmo/X11R6/locale/zh �� xc/nls/X11/locale/XLC_LOCALE/zh
  ���֤������ơ�����ѥ��뤷�ʤ����ƻ��Ѥ��Ƥ���������

Mr. Bill McMahon (Hewlett Packard) �˴��դΰդ�ɽ���ޤ�����ˤϡ�XWNMO�ΰܿ�����
����ӡ����������礤�˹׸����Ƥ��������ޤ�����

