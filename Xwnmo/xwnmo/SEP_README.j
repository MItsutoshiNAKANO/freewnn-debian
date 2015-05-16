/*
 * $Id: SEP_README.j,v 1.1.2.1 1999/02/08 08:08:36 yamasita Exp $
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
	SEP - SpotLocation��ĥ�ץ�ȥ���

SEP�ϡ����ݥåȥ��������ι�®��ư��¸�����ץ�ȥ���Ǥ���

�̾���ץꥱ�������ˤ����륫�������ư�ν����ϰʲ��褦�ˤʤ�ޤ���

	nested_list = XVaCreateNestedList(dummy, XNSpotLocation, spot NULL);
	XSetICValues(ic, XNPreeditAttributes, nested_list, NULL);
	XFree(dpy, nested_list);

�����ߥʥ륨�ߥ�졼���Τ褦�ʥ��ץꥱ�������ξ�硢���ˤ˥���������ư����
ɬ�פ�����ޤ�������椨�����Τ褦�ʽ����ϡ����ץꥱ����������ǽ�����������
���ޤ��ޤ���
SEP�ϡ�����������褷�ޤ������ץꥱ�������ϡ�SEP��Ȥ������ʲ��δؿ���Ƥ�
�����Ǥ���

	_XipChangeSpot(ic, spot_x, spot_y);

_XipChangeSpot()�ξܺ٤ϡ�xwnmo�Υޥ˥奢���SEP��ʬ�򻲾Ȳ�������

-------------------------------------------------------------------------
XIM�饤�֥���XWNMO�֤Υ����åȤ�Ȥä��̿��ǡ���

    �׵�
	�Х��ȿ�	��		����
    XIM -> XWNMO (ximChangeSpotReq)
	1	    XIM_ChangeSpot(20)	�ꥯ�����ȥ�����
	1				̤����
	2		12		�ǡ�����Ĺ��
	4		CARD32		xic
	2		INT16		spot location x
	2		INT16		spot location y

    ����
	�Х��ȿ�	��		����
    XIM <- XWNMO (sz_ximEventReply)
	2				����
			0		 ����
			-1		 �۾�
	6				̤����

