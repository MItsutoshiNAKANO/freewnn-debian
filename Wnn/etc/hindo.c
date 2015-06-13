/*
 *  $Id: hindo.c,v 1.4 2013/09/02 11:01:39 itisango Exp $
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
                                                ���� 9/1/89

        a�������, b�Ჾ���١ʶ������������ˤΤȤ�

        b��[a��x]��2(x��1) where x��[( sqrt(1��2a)��1 )��2]
        �ʷ׻���̤�126��ۤ�������126�Ȥ��롣��

        (min)a��(b��2y)(y��1)         where y��[b��4]
        (mid)a��(b��2y)(y��1)��[y��2] where y��[b��4]
        ��b��126�Ȥ��롣��

        ������[ ]�ϥ���������
        b��Ϳ���Ƥ�a�ο�¬�ͤˤ��������ꡢ(min)a�ȤϤ��κǾ��͡�
        (mid)a�Ϥ��������
        b��4 �ʤ顢(min)a��(mid)a��b

        �����٤�b�ΤȤ������٤ι����γ�Ψ�� 1 / ([b��4]��1)
        â�� b��0�λ��ϡ����ٹ�����Ψ 0

    
         a == -1 <==> b == 0x7f = 127; 
        ���λ������Υ���ȥ꡼�ϡ��Ѵ��˷褷���Ѥ����ʤ�
        (�����ȥ����Ȥ���Ƥ���)���Ȥ�ɽ����
                        9/1/89 H.T.
*/

#include "etc.h"


 /** ����������ʿ�����ؿ���â������<0�λ��Υ��顼�����å��Ϥʤ���0���֤��ˡ�*/
static int
isqrt (i)
     int i;
{
  register int a, b;

  if (i <= 0)
    return (0);
  for (a = i, b = 1; b <<= 1, a >>= 2;);
  while ((a = i / b) < b)
    b = (b + a) >> 1;
  return (b);
}

 /** ������a��������b */
int
asshuku (hin)
     int hin;
{
  register int c;

  if (hin == -1)
    return (127);
  if (hin <= 4)
    return (hin);
  /* ��Ⱦ������0�����ꤷ�ƤΥ��ԡ��ɥ��åס�motoni1,2�Ǥ�Ʊ�� */

  c = (isqrt ((hin <<= 1) + 1) + 1) & ~1;
  c += hin / c - 2;
  return (c < 126 ? c : 126);
}

 /** ������b��������(min)a */
/*
int
motoni1(hin)
int     hin;
{
        register int    c;

        if(hin == 127) return(-1);
        if(hin <= 4) return(hin); 
        c = hin >> 2;
        return( (hin - (c << 1)) * (c + 1) );
}
*/

 /** ������b��������(mid)a */
int
motoni2 (hin)
     int hin;
{
  register int c;

  if (hin == 127)
    return (-1);
  if (hin <= 4)
    return (hin);
  c = hin >> 2;
  return ((hin - (c << 1)) * (c + 1) + (c >> 1));
}
