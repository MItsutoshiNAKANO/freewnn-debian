/*
 *  $Id: jhlp.c,v 1.26 2013/09/02 11:01:40 itisango Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2003, 2006, 2009, 2013, 2015
 * Copyright Canna Project 2002
 * Copyright Taketo Kabe 2003
 *
 * Maintainer:  FreeWnn Project
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

#ifndef lint
static char *rcs_id = "$Id: jhlp.c,v 1.26 2013/09/02 11:01:40 itisango Exp $";
#endif /* lint */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <setjmp.h>
#include <signal.h>
#if STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#  if HAVE_MALLOC_H
#    include <malloc.h>
#  endif
#endif /* STDC_HEADERS */
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif
#if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  if HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else
#    include <time.h>
#  endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
#if HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#include <pwd.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif
#ifdef HAVE_LIBUTIL_H
#  include <libutil.h>
#endif

/* Solaris 2.x needs termio.h , so include both. */
#if HAVE_TERMIOS_H
#  include <termios.h>
#endif
#if HAVE_TERMIO_H
#  include <termio.h>
#endif

#if defined(HAVE_TERMIOS_H)
/* #  include <termios.h> */
#  define USE_TERMIOS 1
#elif defined(HAVE_TERMIO_H)
/* #  include <termio.h> */
#  define USE_TERMIO  1
#elif defined(HAVE_SYS_TERMIO_H)
#  include <sys/termio.h>
#  define USE_TERMIO  1
#elif defined(HAVE_SGTTY_H)
#  include <sgtty.h>
#  define USE_SGTTY
#else
#  error "No termio header."
#endif /* HAVE_TERMIOS_H */

#ifdef HAVE_LIBSPT
#  include <libspt.h>	/* lib from Canna. not used w/FreeWnn */
#endif

/*
 * Enable old Linux specific code.
 * We hope you don't have to use this #define ...
 * (We will remove this #define and related code in near future.)
 */
/* #define USE_LINUX_TERM */


#include "sdefine.h"

#include "jllib.h"
#include "commonhd.h"
#include "sheader.h"
#include "wnn_config.h"
#include "wnn_os.h"

/* old system check (don't know if it works) */
#ifdef nec_ews_svr2
#  include <sys/jtermio.h>
#endif /* nec_ews_svr2 */
#if defined(uniosu)
#       include <sys/pty.h>
#endif /* defined(uniosu) */
#ifdef SVR4
#include <sys/stropts.h>
#include <sys/euc.h>
#include <sys/eucioctl.h>
#endif /* SVR4 */

#include "etc.h"

#define ERROR -1

jmp_buf kk_env;
#ifdef TIOCSSIZE
struct ttysize pty_rowcol;
#endif /* TIOCSSIZE */

int ttyfd;			/* master tty */

char *tname;                    /* terminal name */
char *cmdnm = "csh";            /* char *cmdnm = "csh"; */

int child_id;
char *prog;
#ifdef HAVE_LIBSPT
spt_handle *spth = NULL;
#endif
int need_utmp_clear = 0;

static void save_signals (void);
static void restore_signals (void);
static RETSIGTYPE terminate_handler (int);
static void do_end (void);
static void open_pty (void);
static void open_pty_traditional (void);
static void open_ttyp (void);
static void do_main (void);
static void exec_cmd (char **);
static void parse_options (int, char **);
static void setsize (void);
static void get_rubout (void);
static void usage (char *);
static void change_size (void);
static void default_usage (void);
static void j_term_save (void);
static unsigned char keyin0 (void);

int
main (int argc, char **argv)
{
  char *name;
  char *p;
  char nlspath[64];
  FuncDataBase *f;
  char *server_env;
  char errprefix[1024] = "error";
  int i;

  prog = argv[0];
  flow_control = FLOW_CONTROL;
  code_trans = default_code_trans;

  strcpy (username, getpwuid (getuid ())->pw_name);
  if ((name = getenv (WNN_USERNAME_ENV)) != NULL)
    {
      strncpy(username, name, PATHNAMELEN-1);
      username[PATHNAMELEN-1] = '\0';
    }
  for (i = 1; i < argc;)
    {
      if (!strcmp (argv[i++], "-L"))
        {
          if (i >= argc || argv[i][0] == '-')
            default_usage ();
          strncpy(lang_dir, argv[i++], LANGDIRLEN-1);
          lang_dir[LANGDIRLEN-1] = '\0';
          for (; i < argc; i++)
            {
              argv[i - 2] = argv[i];
            }
          argv[i - 2] = NULL;
          argc -= 2;
          break;
        }
    }

  if (*lang_dir == '\0')
    {
#ifndef CANNA
      if ((p = getenv ("LANG")) != NULL)
        {
          if (strlen (p) >= 4)
            {
              strncpy (lang_dir, p, 5);
              lang_dir[5] = '\0';
            }
          else
            {
              strcpy (lang_dir, p);
            }
        }
#else /* CANNA */
      strcpy (lang_dir, WNN_DEFAULT_LANG);
#endif /* CANNA */
    }
  for (f = function_db; *lang_dir && f && f->lang; f++)
    {
      if (!strcmp (f->lang, lang_dir))
        {
          lang_db = f;
          break;
        }
    }
  if (lang_db == NULL)
    {
      if (*lang_dir)
        fprintf (stderr, "Lang \"%s\" is wrong, use default lang \"%s\".\n", lang_dir, WNN_DEFAULT_LANG);
      strcpy (lang_dir, WNN_DEFAULT_LANG);
      for (f = function_db; *lang_dir && f && f->lang; f++)
        {
          if (!strcmp (f->lang, lang_dir))
            {
              lang_db = f;
              break;
            }
        }
      if (lang_db == NULL)
        {
          fprintf (stderr, "Default lang \"%s\" is wrong.\n", lang_dir);
          exit (1);
        }
    }
  f_table = &(lang_db->f_table);
  code_trans = lang_db->code_trans;
  tty_c_flag = lang_db->tty_code;
  pty_c_flag = lang_db->pty_code;
  internal_code = lang_db->internal_code;
  file_code = lang_db->file_code;

  parse_options (argc, argv);

  strcpy (nlspath, LIBDIR);
  strcat (nlspath, "/%L/%N");
  cd = msg_open ("uum.msg", nlspath, lang_dir);

  if (*def_servername == '\0')
    {
      if (!(server_env = get_server_env (lang_dir)))
        {
          server_env = WNN_DEF_SERVER_ENV;
        }
      if (name = getenv (server_env))
        {
          strncpy(def_servername, name, PATHNAMELEN-1);
          def_servername[PATHNAMELEN-1] = '\0';
          strcpy(def_reverse_servername, def_servername);
        }
    }

  if (!isatty (0))
    {
      fprintf (stderr, "Input must be a tty.\n");
      exit (1);
    }

  if ((p = getenv (WNN_COUNTDOWN_ENV)) == NULL)
    {
      if (setenv (WNN_COUNTDOWN_ENV, "0", 1) != 0)
	{
#if HAVE_SNPRINTF
	  snprintf (errprefix, sizeof (errprefix),
		    "error at %s (%d)", __FILE__, __LINE__); 
#endif /* HAVE_SNPRINTF */
	  perror (errprefix);
	  exit (1);
	}
    }
  else if (atoi (p) <= 0)
    {
      puteustring (MSG_GET (4), stdout);
      /*
         puteustring("�������������ϵ������ޤ���\n",stdout);
       */
      exit (126);
    }
  else
    {
      sprintf (p, "%d", atoi (p) - 1);
      if (setenv (WNN_COUNTDOWN_ENV, p, 1) != 0)
	{
#if HAVE_SNPRINTF
	  snprintf (errprefix, sizeof (errprefix),
		    "error at %s (%d)", __FILE__, __LINE__); 
#endif /* HAVE_SNPRINTF */
	  perror (errprefix);
	  exit (1);
	}
    }

  if ((tname = getenv ("TERM")) == NULL)
    {
      fprintf (stderr, "Sorry. Please set your terminal type.\r\n");
      exit (1);
    }

/* 2003.06 should delete this? */
#ifndef HAVE_LIBSPT
  /* saveutmp() does utmp handling if USE_UTMP is set to 1 */
  if (saveutmp () < 0)
    {
      puts ("Can't save utmp\n");
    }
#endif /* !HAVE_LIBSPT */


  if (optind)
    {
      optind--;
      argc -= optind;
      argv += optind;
    }
  if (argc > 1)
    {
      cmdnm = *++argv;
    }
  else
    {
      if ((name = getenv ("SHELL")) != NULL)
        {
          cmdnm = name;
        }
      argv[0] = cmdnm;
      argv[1] = NULL;
    }

  j_term_save ();
  /* do_end () is allowed from here */
  save_signals ();

#if HAVE_TERMINFO	/* TERMINFO */
  if (openTermData () == -1)
    {
      fprintf (stderr, "Sorry. Something is wrong with terminfo, maybe.\r\n");
      exit (21);
    }
  maxlength = columns;
  crow = lines - conv_lines;
#else			/* Assume TERMCAP */
  if (getTermData () == -1)
    {
      fprintf (stderr, "Sorry. Something is wrong with termcap, maybe.\r\n");
      exit (21);
    }
#endif /* HAVE_TERMINFO */
#if defined(BSD43) || defined(DGUX)
  setsize ();
#endif /* BSD43 */

#if !HAVE_TERMINFO	/* Assume TERMCAP */
  if (set_TERMCAP () == -1)
    {
      fprintf (stderr, "Sorry. Something is wrong with termcap, maybe.\r\n");
      exit (21);
    }
#endif /* !HAVE_TERMINFO */

  ttyfd = 0;
  open_pty ();
/* It is normal to open tty here, before fork().
   Don't know why linux is different.
   So temporally comment it out to make it as other OSes. */
#ifndef USE_LINUX_TERM
  open_ttyp ();
#endif
  exec_cmd (argv);

  get_rubout ();

  switch (init_uum ())
    {                           /* initialize of kana-kanji henkan */
    case -1:
      terminate_handler (-1);
      break;
    case -2:
      epilogue ();
      do_end ();
      break;
    }

  fcntl (ttyfd, F_SETFL, O_NDELAY);

  if (j_term_init () == ERROR)
    {
      uum_err ("term initialize fault.");
    }

#ifndef CANNA
  if (!jl_isconnect (bun_data_))
    {
      if (!servername || *servername == 0)
        {
          printf ("%s\r\n", wnn_perror ());
        }
      else
        {
          printf ("jserver(at %s):%s\r\n", servername, wnn_perror ());
        }
      flush ();
    }
#endif /* CANNA */

  puteustring (MSG_GET (1),
               /*
                  "\r������(���ʴ����Ѵ��ե��ȥ���ɥץ��å�)\r\n",
                */
               stdout);
  initial_message_out ();       /* print message if exists. */

#if defined(uniosu)
  if (setjmp (kk_env))
    {
      disconnect_jserver ();
      ioctl_off ();
      connect_jserver (0);
    }
#endif /* defined(uniosu) */

  do_main ();
  return 0;
}

/*
  each options handling functions
 */

static int
do_h_opt (void)
{
  henkan_off_flag = 1;
  defined_by_option |= OPT_WAKING_UP_MODE;
  return 0;
}

static int
do_H_opt (void)
{
  henkan_off_flag = 0;
  defined_by_option |= OPT_WAKING_UP_MODE;
  return 0;
}

#ifdef  JAPANESE
int
do_u_opt (void)
{
  pty_c_flag = J_EUJIS;
  return 0;
}

int
do_j_opt (void)
{
  pty_c_flag = J_JIS;
  return 0;
}

int
do_s_opt (void)
{
  pty_c_flag = J_SJIS;
  return 0;
}

int
do_U_opt (void)
{
  tty_c_flag = J_EUJIS;
  return 0;
}

int
do_J_opt (void)
{
  tty_c_flag = J_JIS;
  return 0;
}

int
do_S_opt (void)
{
  tty_c_flag = J_SJIS;
  return 0;
}
#endif /* JAPANESE */

#ifdef  CHINESE
int
do_b_opt (void)
{
  pty_c_flag = C_BIG5;
  return 0;
}

int
do_t_opt (void)
{
  pty_c_flag = C_ECNS11643;
  return 0;
}

int
do_B_opt (void)
{
  tty_c_flag = C_BIG5;
  return 0;
}

int
do_T_opt (void)
{
  tty_c_flag = C_ECNS11643;
  return 0;
}
#endif /* CHINESE */

#ifdef KOREAN
int
do_u_opt (void)
{
  pty_c_flag = K_EUKSC;
  return 0;
}

int
do_U_opt (void)
{
  tty_c_flag = K_EUKSC;
  return 0;
}
#endif /* KOREAN */

static int
do_P_opt (void)
{
  sleep (20);
  return 0;
}

static int
do_x_opt (void)
{
  flow_control = 0;
  defined_by_option |= OPT_FLOW_CTRL;
  return 0;
}

static int
do_X_opt (void)
{
  flow_control = 1;
  defined_by_option |= OPT_FLOW_CTRL;
  return 0;
}

static int
do_k_opt (void)
{
  strncpy(uumkey_name_in_uumrc, optarg, PATHNAMELEN-1);
  uumkey_name_in_uumrc[PATHNAMELEN-1] = '\0';
  if (*uumkey_name_in_uumrc == '\0')
    {
      return -1;
    }
  defined_by_option |= OPT_WNNKEY;
  return 0;
}

static int
do_c_opt (void)
{
  strncpy(convkey_name_in_uumrc, optarg, PATHNAMELEN-1);
  convkey_name_in_uumrc[PATHNAMELEN-1] = '\0';
  if (*convkey_name_in_uumrc == '\0')
    {
      return -1;
    }
  defined_by_option |= OPT_CONVKEY;
  return 0;
}

static int
do_r_opt (void)
{
  strncpy(rkfile_name_in_uumrc, optarg, PATHNAMELEN-1);
  rkfile_name_in_uumrc[PATHNAMELEN-1] = '\0';
  if (*rkfile_name_in_uumrc == '\0')
    {
      return -1;
    }
  defined_by_option |= OPT_RKFILE;
  return 0;
}

static int
do_l_opt (void)
{
  conv_lines = atoi (optarg);
  return 0;
}

static int
do_D_opt (void)
{
  strncpy(def_servername, optarg, PATHNAMELEN-1);
  def_servername[PATHNAMELEN-1] = '\0';
  strcpy(def_reverse_servername, def_servername);
  if (*def_servername == '\0')
    {
      return -1;
    }
  return 0;
}

static int
do_n_opt (void)
{
  strncpy(username, optarg, PATHNAMELEN-1);
  username[PATHNAMELEN-1] = '\0';
  if (*username == '\0')
    {
      return -1;
    }
  return 0;
}

static int
do_v_opt (void)
{
  defined_by_option |= OPT_VERBOSE;
  return 0;
}

static int (*do_opt[]) (void) =
{
  do_h_opt,                     /* 'h' : waking_up_in_henkan_mode */
    do_H_opt,                   /* 'H' : waking_up_no_henkan_mode */
    do_P_opt,                   /* 'P' : sleep 20 seconds (for debug) */
    do_x_opt,                   /* 'x' : disable tty's flow control */
    do_X_opt,                   /* 'X' : enable tty's flow control */
    do_k_opt,                   /* 'k' : specify uumkey file */
    do_c_opt,                   /* 'c' : specify convert_key file */
    do_r_opt,                   /* 'r' : specify romkan mode file */
    do_l_opt,                   /* 'l' : specify # of lines used for henkan */
    do_D_opt,                   /* 'D' : specify hostname of jserver */
    do_n_opt,                   /* 'n' : specify username for jserver */
    do_v_opt,                   /* 'v' : verbose */
};

static void
parse_options (argc, argv)
     int argc;
     char **argv;
{
  register int c;
  register char *default_getoptstr = GETOPTSTR;
  register char *default_ostr = OPTIONS;
  char ostr[64];
  register char *p;

  strcpy (ostr, default_getoptstr);
  strcat (ostr, lang_db->getoptstr);
  while ((c = getopt (argc, argv, ostr)) != EOF)
    {
      if (!(p = strchr (default_ostr, c)) || (*do_opt[p - default_ostr]) () < 0)
        {
          if (!(p = strchr (lang_db->ostr, c)) || (*lang_db->do_opt[p - lang_db->ostr]) () < 0)
            {
              strcpy (ostr, default_ostr);
              strcat (ostr, lang_db->ostr);
              usage (ostr);
            }
        }
    }
}

/** tty ���Ф��� ioctl �Υ��å� */

#ifdef USE_SGTTY
#  if defined(BSD43) || defined(DGUX) /* should be "defined(LPASS8)"? */
#    define SET_PASS8  1
#  endif

struct sgttyb savetmio;
struct sgttyb ttyb_def = { B9600, B9600, 0x7f, 0x15,
			   EVENP | ODDP | ECHO | CRMOD };
int local_mode_def = LCRTBS | LCRTERA | LCRTKIL | LCTLECH | LPENDIN | LDECCTQ;
/* added later */
struct tchars tcharsv;
struct ltchars ltcharsv;
struct sgttyb ttyb;
int local_mode;
#  ifdef SET_PASS8
static int local_mode_sv;
#  endif

static void
get_rubout (void)
{
  rubout_code = savetmio.sg_erase;
}

int
j_term_init (void)
{
  struct sgttyb buf;

  buf = savetmio;
  buf.sg_flags |= RAW;
  buf.sg_flags &= ~ECHO;
  ioctl (ttyfd, TIOCSETP, &buf);
#  ifdef SET_PASS8
  ioctl (ttyfd, TIOCLSET, &local_mode);
#  endif

  return 0;
}

static_void
j_term_save (void)
{
  ioctl (ttyfd, TIOCGETC, &tcharsv);
  ioctl (ttyfd, TIOCGLTC, &ltcharsv);
  if (ioctl (ttyfd, TIOCGETP, &ttyb))
    ttyb = ttyb_def;
  savetmio = ttyb;
#  ifdef SET_PASS8
  if (ioctl (ttyfd, TIOCLGET, &local_mode_sv))
    local_mode_sv = local_mode_def;
  local_mode = local_mode_sv | LPASS8;	/* set PASS8 */
#  else  /* !SET_PASS8 */
  if (ioctl (ttyfd, TIOCLGET, &local_mode))
    local_mode = local_mode_def;
#  endif /* !SET_PASS8 */
}

static void
j_term_restore (void)
{
  ioctl (ttyfd, TIOCSETP, &savetmio);
#  ifdef SET_PASS8
  ioctl (ttyfd, TIOCLSET, &local_mode_sv);
#  endif /* SET_PASS8 */
}

static void
j_term_p_init (int ttypfd)
{
  int word;
  ioctl (ttypfd, TIOCSETC, &tcharsv);
  ioctl (ttypfd, TIOCSLTC, &ltcharsv);
  ioctl (ttypfd, TIOCSETP, &ttyb);
  ioctl (ttypfd, TIOCLSET, &local_mode);
  if (pty_c_flag == J_JIS)
    {
      word = LCTLECH;
      ioctl (ttypfd, TIOCLBIC, &word);
    }
}
#endif /* USE_SGTTY */

#if defined(USE_TERMIO) || defined(USE_TERMIOS)

#ifdef USE_TERMIOS
#  define TERMIO termios
#  define GET_TERMATTR(fd, tio) tcgetattr(fd, tio)
#  define SET_TERMATTR(fd, tio) tcsetattr(fd, TCSADRAIN, tio)
#  define UNDEF_STTY _POSIX_VDISABLE
#  define SET_ATTR_ERROR "error in tcsetattr.\n"
#else /* !USE_TERMIOS */
#  define TERMIO termio
#  define GET_TERMATTR(fd, tio) ioctl(fd, TCGETA, tio)
#  ifdef TCSETAW
#    define SET_TERMATTR(fd, tio) ioctl(fd, TCSETAW, tio)
#    define SET_ATTR_ERROR "error in ioctl TCSETAW.\n"
#  else
#    define SET_TERMATTR(fd, tio) ioctl(fd, TCSETA, tio)
#    define SET_ATTR_ERROR "error in ioctl TCSETA.\n"
#  endif
#  define UNDEF_STTY 0xff
#endif /* !USE_TERMIOS */
#ifdef CERASE
#  define WNN_CERASE CERASE
#else
#  define WNN_CERASE 0x7f
#endif

struct TERMIO savetmio;

static void
set_default_termio (struct TERMIO *terms)
{
  bzero (terms, sizeof *terms);
  terms->c_iflag = IGNBRK | ICRNL | IXON;
  terms->c_oflag = OPOST;
#ifdef ONLCR
  terms->c_oflag |= ONLCR;
#endif
  terms->c_cflag = CS8 | CSTOPB | CREAD | CLOCAL;
#ifndef USE_TERMIOS
  terms->c_cflag |= B9600;
#endif
  terms->c_lflag = ISIG | ICANON | ECHO | ECHOE | ECHOK;
#ifdef USE_TERMIOS
  terms->c_cc[VINTR] = 0x3;
  terms->c_cc[VQUIT] = 0x1c;
  terms->c_cc[VERASE] = 0x8;
  terms->c_cc[VKILL] = 0x15;
  terms->c_cc[VEOF] = 0x4;
  terms->c_cc[VEOL] = _POSIX_VDISABLE;
#  ifdef VEOL2
  terms->c_cc[VEOL2] = _POSIX_VDISABLE;
#  endif
  cfsetospeed (terms, B9600);
  cfsetispeed (terms, B9600);
#else /* !USE_TERMIOS */
  terms->c_line = 0;
  terms->c_cc[0] = 0x3;
  terms->c_cc[1] = 0x1c;
  terms->c_cc[2] = 0x8;
  terms->c_cc[3] = 0x15;
  terms->c_cc[4] = 0x4;
  terms->c_cc[5] = 0;
  terms->c_cc[6] = 0;
  terms->c_cc[7] = 0;
#endif /* !USE_TERMIOS */
}

#if defined(uniosu)
struct jtermio savejtmio;
struct auxtermio auxterm = {
  0,                            /* -tostop */
  {0x1a, 0, 0, 0, 0, 0, 0, 0}   /* c_cc2 */
};
#endif /* defined(uniosu) */

static void
get_rubout (void)
{
  if (savetmio.c_cc[VERASE] == UNDEF_STTY)
    {
      rubout_code = RUBOUT;
    }
  else
    {
      rubout_code = savetmio.c_cc[VERASE];
    }
}

int
j_term_init (void)
{
  struct TERMIO buf1;
#if defined(uniosu)
  struct jtermio buf2;
#endif /* defined(uniosu) */

  buf1 = savetmio;
#ifdef linux
  buf1.c_lflag &= ~(ECHONL | ECHOK | ECHOE | ECHO | XCASE | ICANON | ISIG);
  buf1.c_iflag = 0;
  buf1.c_oflag &= ~OPOST;
  buf1.c_cflag |= CS8;
  buf1.c_cc[VMIN] = 1;          /* cf. ICANON */
  buf1.c_cc[VTIME] = 0;
  buf1.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IUCLC | IXON);
#else /* !linux */
  buf1.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON);
#  ifdef IUCLC
  buf1.c_iflag &= ~(IUCLC);
#  endif
  if (flow_control)
    {
      buf1.c_iflag |= IXON;
    }
  buf1.c_lflag &= ~(ECHONL | ECHOK | ECHOE | ECHO | ICANON | ISIG);
#  ifdef XCASE
  buf1.c_lflag &= ~(XCASE);
#  endif
#  ifdef IEXTEN
  buf1.c_lflag &= ~(IEXTEN);
#  endif
  buf1.c_oflag = OPOST;
#  ifdef USE_TERMIOS
  buf1.c_cc[VMIN] = 1;
  buf1.c_cc[VTIME] = 0;
  cfsetispeed(&buf1, cfgetispeed(&savetmio));
  cfsetospeed(&buf1, cfgetospeed(&savetmio));
#  else /* !USE_TERMIOS */
  buf1.c_cc[VEOF] = 1;          /* cf. ICANON */
  buf1.c_cc[VEOL] = 0;
  /* not needed? cf.ISIG*/
  buf1.c_cc[VINTR] = WNN_CERASE;
  buf1.c_cc[VQUIT] = WNN_CERASE;
  buf1.c_cc[VERASE] = WNN_CERASE;
  buf1.c_cc[VKILL] = WNN_CERASE;
#  endif /* !USE_TERMIOS */
#endif /* !linux */
  if (SET_TERMATTR (ttyfd, &buf1) < 0)
    {
      fprintf (stderr, SET_ATTR_ERROR);
      exit (1);
    }

#if defined(uniosu)
  buf2 = savejtmio;
  buf2.j_flg = CONVTOEXT | WNN_EXIST;
  buf2.j_level = jterm;
  switch (jcode_set)
    {
    case 0:
      buf2.j_ecode = JIS;
      break;
    case 1:
      buf2.j_ecode = SJIS;
      break;
    case 2:
      buf2.j_ecode = UJIS;
      break;
    default:
      fprintf (stderr, "uum: kanji code set not supported in terminfo\n");
      exit (1);
    }
  if (jis_kanji_in)
    {
      strcpy (buf2.j_jst, jis_kanji_in);
      buf2.j_jstl = strlen (jis_kanji_in);
    }
  if (jis_kanji_out)
    {
      strcpy (buf2.j_jend, jis_kanji_out);
      buf2.j_jendl = strlen (jis_kanji_out);
    }
  if (jgaiji_start_address)
    {
      *(short *) buf2.j_gcsa = jgaiji_start_address;
    }
  if (jgaiji_disp)
    {
      strcpy (buf2.j_gdsp, jgaiji_disp);
      buf2.j_gdspl = strlen (jgaiji_disp);
    }

  if (ioctl (ttyfd, JTERMSET, &buf2) < 0)
    {
      fprintf (stderr, "error in ioctl JTERMSET.\n");
      exit (1);
    }
#endif /* defined(uniosu) */

  return 0;
}

static void
j_term_save (void)
{
  if (GET_TERMATTR (ttyfd, &savetmio) < 0)
  {
    set_default_termio (&savetmio);
  }
#if defined(uniosu)
  if (ioctl (ttyfd, JTERMGET, &savejtmio) < 0)
    {
      fprintf (stderr, "uum: error in ioctl JTERMGET in open_ttyp.\n");
      exit (1);
    }
#endif /* defined(uniosu) */
}

static void
j_term_restore (void)
{
  if (SET_TERMATTR (ttyfd, &savetmio) < 0)
    {
      fprintf (stderr, SET_ATTR_ERROR);
      exit (1);
    }

#if defined(uniosu)
  if (ioctl (ttyfd, JTERMSET, &savejtmio) < 0)
    {
      fprintf (stderr, "error in ioctl JTERMSET.\n");
      exit (1);
    }
#endif /* defined(uniosu) */
}

static void
j_term_p_init (int ttypfd)
{
  struct TERMIO buf1;
#if defined(uniosu)
  struct TERMIO buf2;
#endif
  buf1 = savetmio;
#ifdef DGUX                     /* copied from JLS5.4.2 */
  /* should clear on all platforms? */
  buf1.c_iflag &= ~ISTRIP;
#endif /* DGUX */
#ifdef nec_ews_svr2
  buf1.c_line = JAPANLD;
#endif
#ifdef USE_TERMIOS
  cfsetispeed(&buf1, cfgetispeed(&savetmio));
  cfsetospeed(&buf1, cfgetospeed(&savetmio));
#endif
  if (SET_TERMATTR (ttypfd, &buf1) < 0)
    {
      fprintf (stderr, SET_ATTR_ERROR);
      exit (1);
    }
#if defined(uniosu)
  buf2 = savejtmio;
  buf2.j_flg = CONVTOEXT | KANJIINPUT;  /* kanji input & output ok */
  buf2.j_level = jterm;
  switch (jcode_set)
    {
    case 0:
      buf2.j_ecode = JIS;
      break;
    case 1:
      buf2.j_ecode = SJIS;
      break;
    case 2:
      buf2.j_ecode = UJIS;
      break;
    default:
      fprintf (stderr, "uum: kanji code set not supported in terminfo.\n");
      exit (1);
    }

  if (jis_kanji_in)
    {
      strcpy (buf2.j_jst, jis_kanji_in);
      buf2.j_jstl = strlen (jis_kanji_in);
    }
  if (jis_kanji_out)
    {
      strcpy (buf2.j_jend, jis_kanji_out);
      buf2.j_jendl = strlen (jis_kanji_out);
    }
  if (jgaiji_start_address)
    {
      *(short *) buf2.j_gcsa = jgaiji_start_address;
    }
  if (jgaiji_disp)
    {
      strcpy (buf2.j_gdsp, jgaiji_disp);
      buf2.j_gdspl = strlen (jgaiji_disp);
    }

  if (ioctl (ttypfd, JTERMSET, &buf2) < 0)
    {
      fprintf (stderr, "error in ioctl JTERMSET.\n");
      exit (1);
    }

  if (ioctl (ttypfd, TIOCSETAUX, &auxterm) < 0)
    {
      fprintf (stderr, "error in ioctl TIOCSETAUX.\n");
      exit (1);
    }

#endif /* defined(uniosu) */
}
#endif /* USE_TERMIO || USE_TERMIOS */

/** signal SIGCHLD ���������ν����򤹤롣*/
RETSIGTYPE
chld_handler (int sig)
{
#ifdef HAVE_WAIT3
#ifdef HAVE_UNION_WAIT
  union wait status;
#else /* POSIX */
  int status;
#endif
  int pid;

  if ((pid = wait3 (&status, WNOHANG | WUNTRACED, NULL)) == child_id)
    {
      if (WIFSTOPPED (status))
        {
#ifdef SIGCONT
          kill (pid, SIGCONT);
#ifdef GETPGID
	  KILLPG (GETPGID (pid), SIGCONT);
#endif
#endif
        }
      else
        {
          signal (SIGCHLD, SIG_IGN);
          printf ("%s", MSG_GET (3));
          /*
             printf("\r\n������򽪤��ޤ���\r\n");
           */
#ifdef HAVE_LIBSPT
	  if (spth)
	    spt_utmp_set_exit (spth, *(int *)&status);
#endif
          epilogue ();
          do_end ();
        }
    }
#else /* ! HAVE_WAIT3 */
  if (wait (0) == child_id)
    {
      signal (SIGCHLD, SIG_IGN);
      printf ("%s", MSG_GET (3));
      /*
         printf("\r\n������򽪤��ޤ���\r\n");
       */
      epilogue ();
      do_end ();
    }
#endif /* HAVE_WAIT3 */

  re_signal (SIGCHLD, chld_handler);

  /* not reached */
#ifndef RETSIGTYPE_VOID 
  return 0;
#endif
}

/** signal SIGTERM ����������ν����򤹤롣*/
static RETSIGTYPE
terminate_handler (int sig)
{
  signal (SIGCHLD, SIG_IGN);
  epilogue_no_close ();
  do_end ();

  /* not reached */
#ifndef RETSIGTYPE_VOID 
  return 0;
#endif
}

#ifdef  SIGWINCH
RETSIGTYPE
resize_handler (int sig)
{
  re_signal (SIGWINCH, resize_handler);
  change_size ();

  /* not reached */
#ifndef RETSIGTYPE_VOID 
  return 0;
#endif
}
#endif /* SIGWINCH */

/** �ᥤ��롼�� */

/* Assume fd_set, FD_ZERO() family.
   though we may use glue in jserver/de.c */
fd_set sel_ptn;
int ptyfd;

static void
do_main (void)
{
#ifndef CANNA
  unsigned char *buf;
  int ml;

  if ((buf = (unsigned char *) malloc (maxchg * 4)) == NULL)
    {
      printf ("%s", MSG_GET (2));
      printf ("%s", MSG_GET (3));
      /*
         printf("malloc �˼��Ԥ��ޤ�����������򽪤��ޤ���\r\n");
       */
      epilogue ();
      do_end ();
    }
#else  /* CANNA */
  extern void canna_mainloop();
#endif /* CANNA */

  FD_ZERO (&sel_ptn);
  FD_SET (ptyfd, &sel_ptn);
  FD_SET (ttyfd, &sel_ptn);

  if (henkan_off_flag == 0)
    {
      disp_mode ();
    }

#ifndef CANNA
  for (;;)
    {
      ml = kk ();
      make_history (return_buf, ml);
      ml = (*code_trans[(internal_code << 2) | pty_c_flag]) (buf, return_buf, sizeof (w_char) * ml);
      if (ml > 0)
        write (ptyfd, buf, ml);
    }
#else  /* CANNA */
  canna_mainloop();
#endif /* CANNA */
}

int
keyin2 (void)
{
  int total, ret;
  unsigned char in;

  in = keyin0 ();
  if (in == 0xff)
    return (-1);
  total = (int) (in & 0xff);
  if (henkan_off_flag == 0 || pty_c_flag != tty_c_flag)
    {
      ret = get_cswidth_by_char (in);
      for (; ret > 1; ret--)
        {
          total = ((total & 0xff) << 8) + (int) (keyin0 () & 0xff);
        }
    }
  return (total);
}

/** convert_key nomi okonau key-in function */
int
conv_keyin (char *inkey)
{
  return keyin1 (keyin2, inkey);
}

/** �������ϴؿ� 1 */
int
keyin (void)
{
  char inkey[16];
  return (conv_keyin (inkey));
}

/** �������ϴؿ� 2 */
static unsigned char
keyin0 (void)
{
  static unsigned char buf[BUFSIZ];
  static unsigned char outbuf[BUFSIZ];
  static unsigned char *bufend = outbuf;
  static unsigned char *bufstart = outbuf;
  int n;
  fd_set rfds, mask;
  int i, j;
  unsigned char *p;
  extern int henkan_off_flag;
  struct timeval time_out;      /* If your OS's select was implemented as 
                                   a pointer for int, you must modify the
                                   time_out variable to integer           */
  int sel_ret;

  if (bufstart < bufend)
    {
      return (*bufstart++);
    }
  for (;;)
    {
      if ((n = read (ttyfd, buf, BUFSIZ)) > 0)
        {
          if (henkan_off_flag == 1)
            {
              if (tty_c_flag == pty_c_flag)
                {
                  i = through (outbuf, buf, n);
                }
              else
                {
                  i = (*code_trans[(tty_c_flag << 2) | file_code]) (outbuf, buf, n);
                }
            }
          else
            {
              i = (*code_trans[(tty_c_flag << 2) | file_code]) (outbuf, buf, n);
            }
          if (i <= 0)
            continue;
          bufstart = outbuf;
          bufend = outbuf + i;
          return (*bufstart++);
        }

      time_out.tv_sec = 0;
      time_out.tv_usec = 200 * 1000;    /* 200 msec ���ԤĤΤ���! */
      for (rfds = sel_ptn;
#ifdef linux
           (sel_ret = select (FD_SETSIZE, &rfds, 0, 0, NULL)) < 0 && errno == EINTR;
#else
	   /* FD_SETSIZE should be 20 in classical env. */
           (sel_ret = select (FD_SETSIZE, &rfds, 0, 0, &time_out)) < 0 && errno == EINTR;
#endif
           rfds = sel_ptn)
        ;
      if (sel_ret == 0)
        {
          if ((tty_c_flag == J_JIS) && ((i = flush_designate ((w_char *) outbuf)) > 0))
            {
              /* ί�ޤäƤ���ţӣä��Ǥ��Ф� */
              bufstart = outbuf;
              bufend = outbuf + i;
              return (*bufstart++);
            }
          return (0xff);
        }

      if (FD_ISSET(ptyfd, &rfds))
        {
          if ((n = read (ptyfd, buf, BUFSIZ)) <= 0)
            {
              epilogue ();
              do_end ();
            }
#if defined(uniosu)
          if (*buf == PIOCPKT_IOCTL)
            {
              arrange_ioctl (1);
            }
          else if (*buf == 0)
#endif /* defined(uniosu) */
            {                   /* sequence of data */
#if defined(uniosu)
              i = (*code_trans[(pty_c_flag << 2) | tty_c_flag]) (outbuf, buf + 1, n - 1);
#else /* defined(uniosu) */
              i = (*code_trans[(pty_c_flag << 2) | tty_c_flag]) (outbuf, buf, n);
#endif /* defined(uniosu) */
              if (i <= 0)
                continue;
              p = outbuf;
              push_cursor ();
              kk_restore_cursor ();
              while ((j = write (ttyfd, p, i)) < i)
                {
                  if (j >= 0)
                    {
                      p += j;
                      i -= j;
                    }
		  FD_ZERO (&mask);
		  FD_SET (ttyfd, &mask);
                  select (32, 0, &mask, 0, 0);
                }
              pop_cursor ();
            }
        }
      if (FD_ISSET(ttyfd, &rfds))
        {
          if ((n = read (ttyfd, buf, BUFSIZ)) > 0)
            {
              if (henkan_off_flag == 1)
                {
                  if (tty_c_flag == pty_c_flag)
                    {
                      i = through (outbuf, buf, n);
                    }
                  else
                    {
                      i = (*code_trans[(tty_c_flag << 2) | file_code]) (outbuf, buf, n);
                    }
                }
              else
                {
                  i = (*code_trans[(tty_c_flag << 2) | file_code]) (outbuf, buf, n);
                }
              if (i <= 0)
                continue;
              bufstart = outbuf;
              bufend = outbuf + i;
              return (*bufstart++);
#ifdef nodef
            }
          else
            {                   /* Consider it as EOF */
              epilogue ();
              do_end ();
#endif /* It seems that select does not return EOF when  Non-brock
          What should I do? */
            }
        }
    }
}


#if defined(uniosu)
/** pty ���� ioctl �������ä����ν��� */
int
arrange_ioctl (int jflg)
/* jflg: jtermio �� j_flg ���Ѵ��ե饰�����դλ� 0 ����λ� 1 */
{
  struct jtermio jbuf1;
  struct TERMIO frombuf;
  struct TERMIO tobuf;
  int i;

  GET_TERMATTR (ptyfd, &frombuf);
  GET_TERMATTR (ttyfd, &frombuf);

  if ((i = (frombuf.c_iflag & IXON)) != (tobuf.c_iflag & IXON))
    {
      if (i == 0)
        {
          tobuf.c_iflag &= ~IXON;
        }
      else
        {
          tobuf.c_iflag |= IXON;
        }
    }
  if ((i = (frombuf.c_iflag & IXOFF)) != (tobuf.c_iflag & IXOFF))
    {
      if (i == 0)
        {
          tobuf.c_iflag &= ~IXOFF;
        }
      else
        {
          tobuf.c_iflag |= IXOFF;
        }
    }
  if ((i = (frombuf.c_oflag & OPOST)) != (tobuf.c_oflag & OPOST))
    {
      if (i == 0)
        {
          tobuf.c_oflag &= ~OPOST;
        }
      else
        {
          tobuf.c_oflag |= OPOST;
        }
    }
  tobuf.c_cflag = (tobuf.c_cflag & ~CBAUD) | (frombuf.c_cflag & CBAUD);

  SET_TERMATTR (ttyfd, &tobuf);		/* set again */

  ioctl (ptyfd, JTERMGET, &jbuf1);      /* about Japanease */

  if ((jflg) && ((jbuf1.j_flg & KANJIINPUT) == 0))
    {
      jbuf1.j_flg &= ~(KANJIINPUT | CONVTOINT); /* kanji henkan flg off */
      ioctl (ttyfd, JTERMSET, &jbuf1);
      kk_restore_cursor ();
      reset_cursor ();
      longjmp (kk_env, 1);
    }
  if ((!jflg) && ((jbuf1.j_flg & KANJIINPUT) != 0))
    {
      jbuf1.j_flg &= ~(KANJIINPUT | CONVTOINT); /* kanji henkan flg off */
      ioctl (ttyfd, JTERMSET, &jbuf1);
      return (1);
    }
  jbuf1.j_flg &= ~(KANJIINPUT | CONVTOINT);     /* kanji henkan flg off */
  ioctl (ttyfd, JTERMSET, &jbuf1);
  return (0);
}
#endif /* defined(uniosu) */

/** �ҥץ����򵯤�����*/

int ttypfd = -1;	/* slave tty */

static void
exec_cmd (char **argv)
{
  int i;
#if defined(HAVE_LIBSPT) && !defined(USE_LINUX_TERM)
  int r;
  const char *ttynm;
#elif !defined(HAVE_SETSID) || defined(USE_LINUX_TERM)
#ifdef BSD42
  int pid;
#endif
#endif /* (!HAVE_LIBSPT && !HAVE_SETSID) || USE_LINUX_TERM */
#if defined(SIGWINCH) && defined(TIOCSWINSZ) /* || defined(linux) */
  struct winsize win;
  extern int Term_RowWidth;
#endif

  child_id = fork ();
  if (child_id < 0)
    uum_err ("cannot fork.");
  if (!child_id)
    {
      /* --- start changing controlling tty --- */
#if defined(HAVE_LIBSPT) && !defined(USE_LINUX_TERM)
#if defined(SIGWINCH) && defined(TIOCSWINSZ)
      if (ioctl (ttyfd, TIOCGWINSZ, &win) == 0)
	ioctl (ttypfd, TIOCSWINSZ, &win);
#endif /* SIGWINCH && TIOCSWINSZ */
      spt_detach_handle (spth);
      spth = NULL;
      if (spt_detach_ctty () || spt_set_ctty2 (ttypfd))
	{
	  uum_err ("cannot change controlling tty.");
	}

#elif defined(HAVE_SETSID) && !defined(USE_LINUX_TERM) /* !HAVE_LIBSPT */

      int fd;
# if defined(SIGWINCH) && defined(TIOCSWINSZ)
      if (ioctl (ttyfd, TIOCGWINSZ, &win) == 0)
	ioctl (ttypfd, TIOCSWINSZ, &win);
# endif /* SIGWINCH && TIOCSWINSZ */
      setsid ();
# ifdef TIOCSCTTY
      ioctl (ttypfd, TIOCSCTTY, 0);
# else
      close (open (ttyname (ttypfd), O_WRONLY, 0));
# endif
      if ((fd = open("/dev/tty", O_WRONLY)) < 0)
	{
	  uum_err ("cannot change controlling tty.");
	}
      close (fd);
      /* disable utmp logging for now */

#else /* (!HAVE_LIBSPT && !HAVE_SETSID) || USE_LINUX_TERM */

#ifdef BSD42
#ifdef TIOCNOTTY
      /* set notty */
      {
        int fd;
        if ((fd = open ("/dev/tty", O_WRONLY)) >= 0)
          {
            (void) ioctl (fd, TIOCNOTTY, 0);
            close (fd);
          }
      }
#endif /* TIOCNOTTY */
#ifdef TIOCSCTTY
      setsid ();
      ioctl (ttypfd, TIOCSCTTY, 0);
#endif /* TIOCSCTTY */
      /* set tty process group */
      pid = getpid ();
      ioctl (ttypfd, TIOCSPGRP, &pid);
      setpgrp (0, 0);
      close (open (ttyname (ttypfd), O_WRONLY, 0));
      setpgrp (0, pid);
#endif /* BSD42 */
#if defined(SYSVR2) && !defined(linux)
      setpgrp (); /* should setpgrp() AFTER fork() */
      close (open (ttyname (ttypfd), O_WRONLY, 0));
#endif /* SYSVR2 */
 
#if 0
/* unneccessary? */
      /* setutmp() does utmp handling if USE_UTMP is set to 1 */
      if (setutmp (ttypfd) == ERROR)
        {
          puts ("Can't set utmp.");
        }
      else
	{
	  need_utmp_clear = 1;
	}
#endif

/* It is bizarre to open tty after fork().
   So, try to do same as other os.
   If it does work, we can remove this. */
# ifdef linux
      setsid ();
      open_ttyp ();
      close (ptyfd);
      ioctl (ttypfd, TCSETA, &savetmio);
# endif
# if defined(SIGWINCH) && defined(TIOCSWINSZ) /* || defined(linux) */
      ioctl (ttyfd, TIOCGWINSZ, &win);
# endif

#endif	/* (!HAVE_LIBSPT && !HAVE_SETSID) || USE_LINUX_TERM */
      /* --- finish changing controlling tty --- */

#ifndef linux
      setgid (getgid ());
      setuid (getuid ());
#endif
#ifdef HAVE_DUP2
      dup2 (ttypfd, 0);
      dup2 (ttypfd, 1);
      dup2 (ttypfd, 2);
#else /* !HAVE_DUP2 */
      close (0);
      close (1);
      close (2);
      if (dup (ttypfd) != 0 || dup (ttypfd) != 1 || dup (ttypfd) != 2)
        {
          uum_err ("redirection fault.");
        }
#endif /* !HAVE_DUP2 */
      for (i = WNN_NFD - 1; i > 2; i--)
        {
          close (i);
        }

      restore_signals ();

#ifdef SIGTSTP
      signal (SIGTSTP, SIG_IGN);
#endif
#ifdef SIGTTIN
      signal (SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTTOU
      signal (SIGTTOU, SIG_IGN);
#endif

#if defined(SIGWINCH) && defined(TIOCSWINSZ) /* || defined(linux) */
      crow = win.ws_row = Term_RowWidth = win.ws_row - conv_lines;
      ioctl (ttyfd, TIOCSWINSZ, &win);
#endif
#ifdef linux
      setgid (getgid ());
      setuid (getuid ());
#endif
      execvp (cmdnm, argv);
      uum_err ("exec fault.");
    }

  /* parent */
#ifdef HAVE_LIBSPT
  ttynm = ttyname (0);
  if (ttynm)
    {
      ttynm = strchr (ttynm + 1, '/');
      if (ttynm && ttynm[1])
	{
	  spt_utmp_set_host (spth, ttynm + 1);
	}
    }
  spt_utmp_set_pid (spth, child_id);
  r = spt_login_utmp (spth);
  if (!r)
    {
      need_utmp_clear = 1;
    }
  else
    {
      spt_perror ("exec_cmd (login_utmp)", r);
    }
#else	/* !HAVE_LIBSPT */
      /* setutmp() does utmp handling if USE_UTMP is set to 1 */
      if (setutmp (ttypfd) == ERROR)
        {
          puts ("Can't set utmp.");
        }
      else
	{
	  need_utmp_clear = 1;
	}
#endif /* HAVE_LIBSPT */
}

#if !(HAVE_SETENV)
/** �Ķ��ѿ��Υ��å� */
/*
 * This function causes memory leak, but I leave it as it is. Anyway,
 * this function is called only a few times at the startup of uum.
 * The 3rd parameter is ignored. It is added for compatibility only.
 */
int
setenv (char *var, char *value, int overwrite)
{
  extern char **environ;
  char **newenv;
  int i, j;

  j = strlen (var);
  for (i = 0; environ[i] != NULL; i++)
    {
      if (strncmp (var, environ[i], j) == 0 && environ[i][j] == '=')
        {
          break;
        }
    }
  if (environ[i] == NULL)
    {
      if ((newenv = (char **) malloc ((sizeof (char *)) * (i + 2))) == NULL)
        {
	  return (-1);
        }
      for (j = 0; j < i + 1; j++)
        {
          newenv[j] = environ[j];
        }
      newenv[i + 1] = NULL;
      environ = newenv;
    }
  if ((environ[i] = malloc (strlen (var) + strlen (value) + 2)) == NULL)
    {
      return (-1);
    }
  strcpy (environ[i], var);
  strcat (environ[i], "=");
  strcat (environ[i], value);
  return (0);
}
#endif /* !HAVE_SETENV */

#ifdef SVR4
static int
euc_set (eucioc_t *eucioc, int ttyfd)
{
  struct strioctl sb;

  sb.ic_cmd = EUC_WSET;
  sb.ic_timout = 0;
  sb.ic_len = sizeof (struct eucioc);
  sb.ic_dp = (char *) eucioc;
  if (ioctl (ttyfd, I_STR, &sb) < 0)
    {
      return (1);
    }
  return (0);
}

static int
set_euc_term (int ttyfd)
{
  eucioc_t eucioc;

  /* for Japanese EUC */
  eucioc.eucw[0] = 1;
  eucioc.eucw[1] = 2;
  eucioc.eucw[2] = 2;
  eucioc.eucw[3] = 3;
  eucioc.scrw[0] = 1;
  eucioc.scrw[1] = 2;
  eucioc.scrw[2] = 1;
  eucioc.scrw[3] = 2;
  if (euc_set (&eucioc, ttyfd) != 0)
    {
      fprintf (stderr, "eucwidth set failed\n");
      return (1);
    }
  return (0);
}

#endif /* SVR4 */

#ifdef nec_ews_svr2
static void
set_jterm (int ttyfd, int ttypfd)
{
  struct jtermio buf;

  if (ioctl (ttyfd, TCJGETA, &buf) == -1)
    {
      fprintf (stderr, "error in ioctl TCJGETA.\n");
      exit (1);
    }
  buf.c_iflag = 0;
  buf.c_oflag = 0;
  if (ioctl (ttypfd, TCJSETA, &buf) < 0)
    {
      fprintf (stderr, "error in ioctl TCSETA.\n");
      exit (1);
    }
}
#endif /* nec_ews_svr2 */

#ifdef sony
static void
set_sony_jterm (int ttyfd, int ttypfd)
{
#ifdef TIOCKGET
  int tmode, jmode = 0;
  struct jtchars jtc;

  if (ioctl(ttyfd, TIOCKGET, &tmode) < 0)
    {
      fprintf(stderr, "error in ioctl TIOCKGET.\n");
      exit (1);
    }
  jmode = tmode;
  tmode &= ~(KM_SYSCODE | KM_TTYPE);
  switch (pty_c_flag)
    {
    case J_EUJIS:
      tmode |= KM_EUC | KM_SYSEUC;
      break;
    case J_JIS:
      tmode |= KM_ASCII;
      break;
    case J_SJIS:
      tmode |= KM_SJIS | KM_SYSSJIS;
      break;
    }
  if (ioctl(ttypfd, TIOCKSET, &tmode) < 0)
    {
      fprintf(stderr, "error in ioctl TIOCKSET.\n");
      exit (1);
    }
#endif /* TIOCKGET */

#ifdef TIOCKGETC    
  if ((jmode & KM_TTYPE) == KM_JIS)
    {
      ioctl(ttyfd, TIOCKGETC, &jtc);
      jtc.t_ascii = 'B';
      jtc.t_kanji = 'B';
      if (ioctl(ttypfd, TIOCKSETC, &jtc) < 0)
	{
	  fprintf(stderr, "error in ioctl TIOCKSETC.\n");
	  exit(1);
	}
    }
#endif
}
#endif /* sony */

/** ttyp �Υ����ץ� */

#ifndef HAVE_LIBSPT
/* ways to get a pty'
 == Traditional
   Try opening each /dev/ptyXX and use the succeeded one
 == SGI IRIX
   use _getpty()
 == BSD? (HAVE_OPENPTY)
   use openpty()
 == STREAMS (Solaris) (HAVE_PTSNAME)
   use posix_openpt() (or open /dev/ptmx directly) and ptsname()
   (see pts(7D) for EXAMPLE)
*/

#if !defined(sgi) && !defined(HAVE_PTSNAME)
char tty_master[32]; /*VVVV overflow?*/
#endif
char tty_slave [32]; /*VVVV overflow?*/

#if defined(BSD43) || defined(DGUX)	/* FOR PASS8 */
static int local_mode_sv;
#endif
#endif /* !HAVE_LIBSPT */

/* open slave side of pty to ttypfd */
static void
open_ttyp (void)
{
#ifndef HAVE_LIBSPT	 /* && !defined(linux) ? */
  struct stat tstat;
  char chown_failed = 0;
  const char *msg_insecure = "Your ttyp (%s) may be insecure from other users, but continue anyway ...\r\n";
#endif	/* !HAVE_LIBSPT */

#ifdef HAVE_LIBSPT
  if ((ttypfd = spt_open_slave(spth)) == ERROR)
#else /* HAVE_LIBSPT */
# if defined(sgi)
  if ((ttypfd = open (tty_slave, O_RDWR)) == ERROR)
# elif defined(HAVE_PTSNAME)
  if ((ttypfd = open(tty_slave, O_RDWR)) == ERROR	/* open slave */
#   if defined(I_PUSH)
	/* for systems that have STREAMS */
       || ioctl (ttypfd, I_PUSH, "ptem") == ERROR	/* push ptem */
       || ioctl (ttypfd, I_PUSH, "ldterm") == ERROR	/* push ldterm */
#   endif
     )
# elif defined (HAVE_OPENPTY)
  if (ttypfd < 0)		/* already open */
# else
  if ((ttypfd = open (tty_slave, O_RDWR, 0)) == ERROR)	/* old method */
# endif /* sgi */
#endif /* HAVE_LIBSPT */
    {
      perror(tty_slave);	/* debug */
      uum_err ("Can't open ttyp.");
    }
#if defined(HAVE_LIBSPT)
  spt_init_slavefd(spth, ttypfd);
#else	/* HAVE_LIBSPT */
/* #if !defined(linux) */
  if(fstat(ttypfd, &tstat)==0) {
    if(tstat.st_uid != getuid()) {
      if(chown (tty_slave, getuid (), getgid ())!=0) {
	perror("Can't change owner of ttyp.");
	fprintf(stderr, msg_insecure, tty_slave);
	chown_failed = 1;
      }
    }
    if((tstat.st_mode & (S_IWGRP | S_IWOTH)) != 0) {
      /* Ignore message if chown failed (chmod will fail also ...) */
      if(chmod (tty_slave, 0622)!=0 && chown_failed == 0) {
	perror("Can't change permission of ttyp.");
	fprintf(stderr, msg_insecure, tty_slave);
      }
    }
  } else {
    perror("Can't stat ttyp.");
    fprintf(stderr, msg_insecure, tty_slave);
  }
/* #endif */ /* linux */
#endif /* HAVE_LIBSPT */

  /*
   * We save terminal settings in main() instead of here.
   * When defined(linux) open_ttyp() is invoked from child!
   */
#ifndef USE_LINUX_TERM
  j_term_p_init (ttypfd);
#endif

#ifdef TIOCSSIZE
  pty_rowcol.ts_lines = crow;   /* instead of lines */
  pty_rowcol.ts_cols = maxlength;       /* instead of columns */
  ioctl (ttypfd, TIOCSSIZE, &pty_rowcol);
#endif /* TIOCSSIZE */

#ifdef SVR4
  set_euc_term(ttypfd);
#endif
#if defined(nec_ews_svr2)
  set_jterm (ttyfd, ttypfd);
#endif
#ifdef sony
  set_sony_jterm(ttyfd, ttypfd);
#endif
}

#if defined(HAVE_PTSNAME) && ! defined(HAVE_POSIX_OPENPT)
/* Maybe harmless on defined(HAVE_LIBSPT) */
#if !defined(HAVE__DEV_PTMX)
#warning "If not cross compile, you must check pts master device (other than /dev/ptmx)."
/* but continue anyway. */
#endif

/* fallback function to posix_openpt */
static int
posix_openpt (int flags)
{
  return open("/dev/ptmx", flags);
}
#endif		/* HAVE_PTSNAME && !HAVE_POSIX_OPENPT */

/** pty �Υ����ץ� */
/* allocate a new pty master into int ptyfd */
static void
open_pty (void)
{
#if defined(HAVE_LIBSPT)
  int r;
  r = spt_open_pty(&spth, &ptyfd, NULL, NULL);
  if (r != SPT_E_NONE && r != SPT_E_CHOWN_FAIL)
    uum_err ("Can't get pty.");

#elif defined(sgi)
  char *tty_name_buff;
  extern char *_getpty (int *, int, mode_t, int);
  tty_name_buff = _getpty (&ptyfd, O_RDWR | O_NDELAY, 0600, 0);
  if (tty_name_buff == 0)
    uum_err ("Can't get pty.");
  strcpy (tty_slave, tty_name_buff);

#elif defined(HAVE_PTSNAME)
  char *p;
  ptyfd = posix_openpt(O_RDWR);	/* open master */
  if (ptyfd < 0)
    uum_err ("Could not get a pty.");
  grantpt (ptyfd);		/* change permission of slave */
  unlockpt (ptyfd);		/* unlock slave */

  p = ptsname (ptyfd);		/* get name of slave */
  strcpy(tty_slave, p);
  /* you can't get path of the "master" by ttyname;
   * only the open filedes ptyfd is available.
   */
  /* strcpy (tty_master, "/dev/ptm/x"); */

#elif HAVE_OPENPTY
  if (openpty(&ptyfd, &ttypfd, tty_slave, NULL, NULL) == ERROR)
    uum_err ("Could not get a pty.");
    /* Note: slave tty is ALREADY OPEN as ttypfd */
  strcpy(tty_master, ttyname(ptyfd));

#else
  /* Traditional method that search in /dev/ttyXX */
  open_pty_traditional ();
#endif /* HAVE_LIBSPT */

  return;
}

#if !(HAVE_LIBSPT) && !(sgi) && !(HAVE_PTSNAME) && !(HAVE_OPENPTY)
static void
open_pty_traditional (void)
{
  int ptyno;

#ifdef hpux
# define MAXPTYS (11*16)
# define PTYDEV ";/dev/ptym/pty"
# define TTYDEV "/dev/pty/tty"
#else
# define MAXPTYS 32
# define PTYDEV "/dev/pty"
# define TTYDEV "/dev/tty"
#endif

  for (ptyno = 0; ptyno < MAXPTYS; ptyno++)	/* sysconf? */
    {
/*
 * Change pseudo-devices.
 * Because FreeBSD's master pseudo-devices are pty[p-sP-S][0-9a-v].
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)         1996/8/20
 */
#if (defined(BSD) && (BSD >= 199306))   /* 4.4BSD-Lite by Taoka */
# define PTXNAME(b, PFX, no) \
    snprintf ((b), sizeof((b)), PFX "%1c%1c", "pqrsPQRS"[((no) >> 5)], "0123456789abcdefghijklmnopqrstuv"[(no) & 0x1f])
#elif defined(hpux)
#  define PTXNAME(b, PFX, no) \
    sprintf ((b), PFX "%1c%1c", "zyxwvutsrqp"[(no)>>4], "fedcba9876543210"[(no)&0x0f])
#else /* !4.4BSD-Lite, !hpux */
#  define PTXNAME(b, PFX, no) \
    sprintf ((b), PFX "%1c%1c", 'p' + ((no) >> 4), "0123456789abcdef"[(no) & 0x0f]);
#endif /* !4.4BSD-Lite */
#define PTMNAME(b, no) PTXNAME(b, PTYDEV, no)
#define PTSNAME(b, no) PTXNAME(b, TTYDEV, no)

      PTMNAME(tty_master, ptyno);
      fprintf(stderr, "trying master <%s>\n", tty_master);	/* debug */
      if ((ptyfd = open (tty_master, O_RDWR, 0)) != ERROR)
        {
# if defined(uniosu)
          if (ioctl (ptyfd, PIOCPKT, 1) < 0)
            {                   /* packet mode on */
              fprintf (stderr, "error in ioctl PIOCPKT.\n");
              exit (1);
            }
# endif /* uniosu */
	  PTSNAME(tty_slave, ptyno);
          return;
        }
    }
  uum_err ("Can't get pty.");
}
#endif /* !(HAVE_LIBSPT) && !(sgi) && !(HAVE_PTSNAME) && !(HAVE_OPENPTY) */

/** ���顼���衣���褦�ʤ顣 */
void
uum_err (char *s)
{
  puts (s);
  fclose (stdout);
  fclose (stderr);
  fclose (stdin);
  do_end ();
}

/** Ω��Ļ��������� �����ν��� */
static void
do_end (void)
{
#ifdef USE_LIBSPT
  int r;
#endif

  static int do_end_flg = 0;
  if (do_end_flg == 1)
    return;
  do_end_flg = 1;

  signal (SIGCHLD, SIG_DFL);
  fcntl (ttyfd, F_SETFL, 0);
  j_term_restore ();

#if !defined(HAVE_LIBSPT) && !defined(sgi)
# ifndef HAVE_PTSNAME
  if (chown (tty_master, 0, 0) == ERROR)
    {
      perror ("Can't restore owner of tty_master.");
    }
  if (chmod (tty_master, 0666) == ERROR)
    {
      perror ("Can't restore permission of tty_master.");
    }

  if (chown (tty_slave, 0, 0) == ERROR)
    {
      perror ("Can't restore owner of ttyp.");
    }
  if (chmod (tty_slave, 0666) == ERROR)
    {
      perror ("Can't restore permission of ttyp.");
    }
# endif /* HAVE_PTSNAME */

#endif /* !HAVE_LIBSPT && !sgi */
  close (ttyfd);
#ifdef HAVE_LIBSPT
  if (spth && need_utmp_clear && (r = spt_logout_utmp(spth)))
    spt_perror(NULL, r);
  if (spth && (r = spt_close_pty(spth)))
    spt_perror(NULL, r);
#else
/* needless? */
  /* resetutmp() does utmp handling if USE_UTMP is set to 1 */
  if (need_utmp_clear)
    {
      if (resetutmp (ttypfd) == ERROR)
	{
	  printf ("Can't reset utmp.");
	}
      need_utmp_clear = 0;	/* for safety */
    }
  close (ptyfd);
#endif /* HAVE_LIBSPT */

  chdir ("/tmp");               /* to avoid making too many mon.out files */

  KILLPG (child_id, SIGHUP);
  exit (0);
}

#if defined(uniosu)
/** ��̾�����Ѵ��� ioctl �ǥ��դ������� keyin ������ؿ� */
int
ioctl_off (void)
{
  static unsigned char buf[BUFSIZ];
  int n;
  fd_set rfds;

  kk_restore_cursor ();
  clr_line_all ();
  display_henkan_off_mode ();

  for (;;)
    {
      if ((n = read (ttyfd, buf, BUFSIZ)) > 0)
        {
          write (ptyfd, buf, n);
        }
      rfds = sel_ptn;
      select (FD_SETSIZE, &rfds, 0, 0, NULL);
      if (FD_ISSET(ptyfd, &rfds))
        {
          if ((n = read (ptyfd, buf, BUFSIZ)) <= 0)
            {
              epilogue ();
              do_end ();
            }
          if (*buf == 0)
            {                   /* sequence of data */
              write (ttyfd, buf + 1, n - 1);
            }
          else if (*buf == PIOCPKT_IOCTL)
            {
              if (arrange_ioctl (0) > 0)
                {
                  return;
                }
            }
        }
      if (FD_ISSET(ttyfd, &rfds))
        {
          if ((n = read (ttyfd, buf, BUFSIZ)) > 0)
            {
              write (ptyfd, buf, n);
            }
        }
    }
}
#endif /* defined(uniosu) */

static void
default_usage (void)
{
  fprintf (stderr, "%s: Bad -L option\n", prog);
  exit (0);
}

static void
usage (char *optstr)
{
  printf ("usage: prog %s by lang \"%s\"\n", optstr, lang_dir);
  exit (0);
}

/*
  save/restore signal settings
 */

intfnptr sigpipe, sighup, sigint, sigquit, sigterm, sigtstp, sigttin, sigttou, sigchld;
#ifdef SIGWINCH
intfnptr sigwinch;
#endif /* SIGWINCH */

static void
save_signals (void)
{
  sigpipe = signal (SIGPIPE, SIG_IGN);
#ifdef linux                    /* XXX */
  sighup = signal (SIGHUP, SIG_IGN);
#endif
  sighup = signal (SIGHUP, terminate_handler);
  sigint = signal (SIGINT, SIG_IGN);
  sigquit = signal (SIGQUIT, SIG_IGN);
  sigterm = signal (SIGTERM, terminate_handler);
  sigchld = signal (SIGCHLD, chld_handler);
#ifdef SIGWINCH
  sigwinch = signal (SIGWINCH, resize_handler);
#endif /* SIGWINCH */
#ifdef SIGTSTP
  sigtstp = signal (SIGTSTP, SIG_IGN);
  sigttin = signal (SIGTTIN, SIG_IGN);
  sigttou = signal (SIGTTOU, SIG_IGN);
#endif /* SIGTSTP */
}

static void
restore_signals (void)
{
  signal (SIGPIPE, sigpipe);
  signal (SIGHUP, sighup);
  signal (SIGINT, sigint);
  signal (SIGQUIT, sigquit);
  signal (SIGTERM, sigterm);
  signal (SIGCHLD, sigchld);
#ifdef SIGWINCH
  signal (SIGWINCH, sigwinch);
#endif /* SIGWINCH */
#ifdef SIGTSTP
  signal (SIGTSTP, sigtstp);
  signal (SIGTTIN, sigttin);
  signal (SIGTTOU, sigttou);
#endif /* SIGTSTP */
}

#if defined(BSD43) || defined(DGUX)
static void
setsize (void)
{
  int i;
  struct winsize win;
  extern int Term_LineWidth, Term_RowWidth, maxlength;

  if (ioctl (ttyfd, TIOCGWINSZ, &win) < 0)
    {
      /* Default set at getTermData() */
      return;
    }
  else
    {
      if ((i = win.ws_row) != 0)
        {
          crow = Term_RowWidth = i - conv_lines;
        }
      if ((i = win.ws_col) != 0)
        {
          maxlength = Term_LineWidth = i;
        }
    }
}
#endif /* BSD43 */

#ifdef  SIGWINCH
static void
change_size (void)
{
  register int i;
  struct winsize win;
  extern int Term_LineWidth, Term_RowWidth, maxlength;

  if (ioctl (ttyfd, TIOCGWINSZ, &win) < 0)
    {
      /* Default set at getTermData() */
      return;
    }
  else
    {
      throw_cur_raw (0, crow);
      clr_line ();

      if ((i = win.ws_row) != 0)
        {
          crow = Term_RowWidth = i - conv_lines;
#ifdef TIOCSWINSZ
          win.ws_row = crow;
#endif
        }
      if ((i = win.ws_col) != 0)
        {
          maxlength = Term_LineWidth = i;
        }
#ifdef TIOCSWINSZ
      ioctl (ttypfd, TIOCSWINSZ, &win);
#else /* !TIOCSWINSZ */
#ifdef  TIOCSSIZE
      pty_rowcol.ts_lines = crow;       /* instead of lines */
      pty_rowcol.ts_cols = maxlength;   /* instead of columns */
      ioctl (ttypfd, TIOCSSIZE, &pty_rowcol);
#endif /* TIOCSSIZE */
#ifdef  sun                     /* When your machine needs SIGWINCH, add your machine */
      {
        int grp;
        ioctl (ptyfd, TIOCGPGRP, &grp);
	KILLPG (grp, SIGWINCH);
      }
#endif /* sun */
#endif /* !TIOCSWINSZ */

#ifndef CANNA
      set_scroll_region (0, crow - 1);
      if (henkan_off_flag)
        {
          kk_restore_cursor ();
          throw_cur_raw (0, 0);
          kk_save_cursor ();
          display_henkan_off_mode ();
          set_screen_vars_default ();
          t_print_l ();
          kk_restore_cursor ();
        }
      else
        {
          kk_restore_cursor ();
          throw_cur_raw (0, 0);
          kk_save_cursor ();
          disp_mode ();
          set_screen_vars_default ();
          t_print_l ();
        }
#else /* CANNA */
      set_scroll_region(0, crow - 1);
      set_screen_vars_default();
      t_print_l();
#endif /* CANNA */
    }
}
#endif /* SIGWINCH */
