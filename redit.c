/*********************************************************************/
/*                                                                   */
/*              REDIT Full Screen Editor Version 4.02                */
/*                                                                   */
/*      This version was derived from an earlier version (3.11)      */
/*      written in MS-BASIC. Both the BASIC and C versions were      */
/*      written by Jon Marinello. The C version was developed        */
/*      under UNIX System V and then ported to MS-DOS.               */
/*                                                                   */
/*********************************************************************/

#define IBMPC

#define DEBUGF  0

#include <stdio.h>
#include <ctype.h>

#ifdef UNIX
#include <termio.h>
extern  struct  termio  arg, sarg;
#endif

#define MAXNAME         70
#define CMAX            256
#define FALSE           0
#define TRUE            -1
#define HISTMAX 50

#ifdef UNIX

#define CURDEP          "\033[%02d;%02dH"
#define DEFWHITE        "\033[0m"
#define DEFYELLOW       "\033[0m\033[5m" /* Blinking */
#define DEFGREEN        "\033[0m\033[1m" /* Low */
#define DEFRED          "\033[0m\033[7m" /* Reverse */
#define DEFBLUE         "\033[0m\033[7m" /* REverse */
#define DEFCLRHM        "\033[H\033[J"
#define DEFERSEND       "\033[J"
#define DEFERSLN        "\033[K"
#define NULLCHAR        0134

#else

#ifdef getchar
#undef getchar
#define getchar getch
#endif
#define CURDEP          "\033[%02d;%02dH"
#define DEFWHITE        "\033[37m"
#define DEFYELLOW       "\033[33m"
#define DEFGREEN        "\033[32m"
#define DEFRED          "\033[31m"
#define DEFBLUE         "\033[34m"
#define DEFCLRHM        "\033[2J"
#define DEFERSEND       "\033[J"
#define DEFERSLN        "\033[K"
#define NULLCHAR        000

#endif

#define DEFDUBQ 042
#define DEFBELL 007
#define DEFBS           010
#define DEFCR           015
#define DEFBLANK        040
#define FTAB            011
#define ESC             033

#define PF1             0200
#define PF2             0201
#define PF3             0202
#define PF4             0203
#define PF5             0204
#define PF6             0205
#define PF7             0206
#define PF8             0207
#define PF9             0210
#define PF10            0211
#define PF11            0212
#define PF12            0213
#define RT              0214
#define LT              0215
#define UP              0216
#define DN              0217
#define INS             0220
#define DEL             0221
#define CTRLDEL         0222
#define CTRLRT          0223
#define CTRLUP          0224
#define CTRLDN          0225
#define CTRLLT          0226
#define HOME            0227
#define SHFTHOME        0230
#define CTRLHOME        0231
#define SHFTRT          0232
#define SHFTLT          0233
#define BTAB            0234
#define END             0235
#define PGUP            0236
#define PGDN            0237

#define LOCUP           1
#define LOCDOWN         0

extern  char    *fncurp();
extern  char    *fnmsg();
extern  char    *fnmsg2();
extern  char    *fnmode();
extern  char    *fnfs01();
extern  char    *fnfs02();
extern  char    *space();
extern  char    *string();
extern  int     fncl();
extern  int     fncc();
extern  int     exit();
extern  char    *rmalloc();
extern  char    *rrealloc();
extern  char    *malloc();
extern  char    *realloc();
extern  int     strcmp();
extern  int     strncmp();
extern  char    *strchr();
extern  char    *strrchr();
extern  char    *strcpy();
extern  char    *strncpy();
extern  char    *strcat();
extern  char    *strncat();
extern  char    *rmlwc();
extern  char    *parse();
extern  char    *file;
extern  int     errno;

#ifdef UNIX
struct  termio  arg, sarg;
#endif

char    *WHITE;
char    *YELLOW;
char    *GREEN;
char    *RED;
char    *BLUE;
char    *CLRHM;
char    *ERSEND;
char    *ERSLN;
char    BLANK;
char    BS;
char    DUBQ;
char    BELL;
char    CR;
int     en;
int     recall;
int     nchars;
int     cml;
int     pcl;
int     pcc;
int     insert;
int     added;
int     curl;
int     curc;
int     ncurl;
int     ll;
int     color = TRUE;
int     bnn;
int     icl;
int     cl;
int     fl;
int     start;
int     ncl;
char    *prompt = "";
char    cpos[64];
char    ss[128];
char    wstring[CMAX];
char    fnstring[128];
char    history[HISTMAX][128];
int     hptr;
char    *msg = "";
char    *fill = "";
char    COMMAND[128];
int     ftrap;
char    sn[MAXNAME];
char    bn[MAXNAME];
char    *asm_tabs = "10,20,30,40,50";
char    *bas_tabs = "10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250";
char    *cob_tabs = "8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72";
char    *for_tabs = "7,72";
char    *pli_tabs = "5,10,15,20,25,30,35,40,45,50,55,60,65,70";
char    *c_tabs = "8,16,24,32,40,48,56,64,72";

char    **l;

char    cm[20][8];
char    ty2red[20];
char    ty3red[20];
int     fkeys[30];
int     rkeys[30];
char    xave[20][CMAX];
char    fv[12][72];
int     tabs[25];
char    curtab[128];
char    sa[CMAX];
char    iobuf[CMAX];
int     notf = FALSE;
char    delim = '/';
int     lm = 72;
int     v1 = 1;
int     v2 = 72;
char    ruler[128];
char    lruler[128]; /* Last ruler */
int     nn;
char    *m00 = "workspace empty";
char    *m01 = "WT";
char    *m02 = "file not found";
char    *m03 = "invalid edit command";
char    *m04 = "?error in REDIT.RC";
char    *m05 = "missing argument";
char    *m06 = "at viewport boundry";
char    *m07 = "function key not available";
char    *m08 = "invalid tabs";
char    *m09 = "error in type II area";
char    *m10 = "action will exceed workspace";
char    *m11 = "only A,C,D,I,K,M,/,\",[1-9]";
char    *m12 = "out of memory";
char    *m13 = "saved";
char    *m14 = "replaced";
char    *m15 = "pattern null";
char    *m16 = "pattern not found";
char    *m19 = "*====* ";
char    *m20 = "***** TOP OF FILE *****";
char    *m21 = "***** END OF FILE *****";
char    *m23 = "__STACK";
char    *m24 = "__TEMP";
char    *m25 = "REDIT.RC";
char    *m26 = "new file";
char    *m27 = "end insert";
char    *m29 = "FS";
char    *m30 = "can't create __STACK, errno=%d";
#ifdef UNIX
char    *m31 = "/REDIT.RC";
#else
char    *m31 = "\\LIB\\REDIT.RC";
#endif
char    *m32 = "can't create __TEMP, errno=%d";
char    *m33 = "can't create %s, errno=%d";
char    *m34 = "write error on __STACK, errno=%d";
char    *m35 = "write error on __TEMP, errno=%d";
char    *m36 = "write error on %s, errno=%d";
#ifdef UNIX
char    *m37 = "/REDIT.HLP";
#else
char    *m37 = "\\LIB\\REDIT.HLP";
#endif
char    *m38 = "can't find \\LIB\\REDIT.HLP";
char    *m39 = "Press any key to continue...";
char    *m40 = "T(op of file), Q(uit) or any key to continue...";
char    *file;

main(argc, argv)
int     argc;
char    **argv;
{
	if (argc > 1)   /* If argument passed, use as file to edit */
                file = argv[1];
        init();         /* Initialize edit */
        lp();           /* Load edit profile */
        printf(CLRHM);  /* Clear screen */
#ifdef UNIX
        initio();       /* Put terminal in raw mode */
#endif
        pcli();         /* Print command line */
        prss();         /* Print RADIX SYSTEMS SOFTWARE */
        rd();           /* Load text file */
        pchw();         /* Print command header and window */
        pc();           /* Parse commands */
        printf("%s%s", CLRHM, WHITE); /* all done clear screen */
#ifdef UNIX
	endio();        /* Put terminal back in canonical mode */
#endif
        exit(0);
}

#ifdef UNIX

initio() {      /* System III or system IV */
        ioctl(0, TCGETA, &arg);
        ioctl(0, TCGETA, &sarg);
        arg.c_iflag |= IGNBRK;
        arg.c_iflag &= ~BRKINT;
        arg.c_iflag &= ~ICRNL;
        arg.c_oflag |= ONLRET;
        arg.c_lflag &= ~ICANON;
        arg.c_lflag &= ~ECHO;
        arg.c_lflag &= ~ECHOE;
        arg.c_lflag &= ~ECHOK;
        arg.c_cc[0] = (char)127;
        arg.c_cc[1] = (char)28;
        arg.c_cc[2] = (char)8;
	arg.c_cc[3] = (char)127;
        arg.c_cc[4] = (char)1;
        arg.c_cc[5] = (char)1;
        ioctl(0, TCSETAW, &arg);
}

endio() {
        ioctl(0, TCSETAF, &sarg);
}

#endif

sleep(seconds)
unsigned	seconds;
{
	long	stime, ctime;

	time(&stime);
	for (;;) {
		time(&ctime);
		if ((unsigned)(ctime - stime) > seconds)
			return;
	}
}

addlines() {
	int     i;
	int     lf;

#if DEBUGF
    printf("\n\n\n\n\007Allocating to l pointer %d * %d = %d\n\n",
	fl + bnn + 1, sizeof(char *), (fl+bnn+1)*sizeof(char *));
    getchar();
#endif
	l = (char **)rrealloc((char *)l, ((fl + bnn + 1) * sizeof(char *)));
	for (i = fl; i >= icl + 1; --i) {
		lf = i + bnn;
		l[lf] = l[i];
	}
	lf = strlen(fill);
	for (i = icl + 1; i <= icl + bnn; ++i) {
		l[i] = rmalloc(lf + 1);
		if (!lf)
                        l[i][0] = '\0';
                else
                        strcpy(l[i], fill);
        }
        fl += bnn;
        return(0);
}

c2stack() {
        FILE    *fd;
        int     i;
        char    mbuf[64];

        if (!fl)
                return(-1);
        if (!(fd = fopen(m23, "w"))) {
                sprintf(mbuf, m30, errno);
                msg = fnmsg(mbuf);
                return(-1);
        }
	swork();
        sprintf(iobuf, "%d\n", en - bnn + 1);
        nchars = strlen(iobuf);
        if (fwrite(iobuf, 1, nchars, fd) != nchars) {
                sprintf(mbuf, m34, errno);
                msg = fnmsg(mbuf);
                return(-1);
        }
        for (i = bnn; i <= en; ++i) {
                sprintf(iobuf, "%s\n", l[i]);
                nchars = strlen(iobuf);
                if (fwrite(iobuf, 1, nchars, fd) != nchars) {
                        sprintf(mbuf, m34, errno);
                        msg = fnmsg(mbuf);
                        return(-1);
                }
        }
        fclose(fd);
        return(0);
}

chknum() {
        if (!*sa) {
                nn = 1;
                return(0);
        }
        nn = atoi(sa);
        if (nn < 1 || nn > 9999) {
                msg = fnmsg(m09);
                return(-1);
        }
        return(0);
}

compadd() {
        if (!added && ((icl - cl) < 19)) {
                added = TRUE;
                ncurl = icl - cl + 5;
        }
}

compred() {
	int     i;
        int     j;

        for (i = 0; i <= 19; ++i) {
                ty2red[i] = ty3red[i] = FALSE;
                for (j = 0; j <= 7; ++j)
                        if (cm[i][j] && cm[i][j] != ' ')
                                ty2red[i] = TRUE;
        }
        for (i = cl; i <= cl + ll - 4; ++i) {
                if (i > fl)
                        ty3red[i - cl] = TRUE;
                else if (strcmp(l[i], xave[i - cl])) {
                        ty3red[i - cl] = TRUE;
                }
	}
}

int     dellines() {
	int     i;
	int     r;

	if (!fl) {
		msg = fnmsg(m00);
		return(-1);
	}
	if (!icl)
		++icl;
	if (!strcmp(COMMAND, "DEL *")) {
		en = fl;
		goto L5270;
	}
	en = atoi(&COMMAND[4]);
L5270:  if (en <= 0)
		en = 1;
	en = en + icl - 1;
	bnn = icl;
	if (en > fl)
		en = fl;
	r = en - bnn + 1;
	for (i = bnn; i < bnn + r; ++i) { /* free deleted lines */
		free(l[i]);
	}
	for (i = bnn; i <= fl - r; ++i) { /* shift up existing lines to old l[i] slots that were just free'd */
		l[i] = l[i + r];
	}
	fl -= r;
	l = (char **)rrealloc((char *)l, ((fl + 1) * sizeof(char *)));
	return(0);
}

displ() {
	strncpy(sa, &l[pcl][pcc - 1], v2 - pcc + 1);
	sa[v2 - pcc + 1] = '\0';
	if (strlen(sa) == 72)
		printf("%s%s", GREEN, sa);
        else
                printf("%s%s%s", GREEN, sa, ERSLN);
        if (insert)
                bnn = 3;
        else
                bnn = 2;
}

displc(off, spchar)
int     off;
char    *spchar;
{
        printf("%s%s%s", fncurp(1, off + 6), &COMMAND[off], spchar);
        printf("%s", fncurp(1, off + 6));
}

dty2() {
        int     i;

        for (i = 0; i <= 19; ++i)
                ty2red[i] = TRUE;
}

dty3() {
        int     i;

        for (i = 0; i <= 19; ++i)
                ty3red[i] = TRUE;
}

int     fncc(y)
int     y;
{
        return(v1 + y - 8);
}

int     fncl(x)
int     x;
{
        return(cl + x - 4);
}

char    *fncurp(x, y)
int     x;
int     y;
{
        sprintf(cpos, CURDEP, x, y);
        return(cpos);
}

char    *fnfs01(m, y)
char    *m;
int     y;
{
        sprintf(wstring, "%s%s", m, space(y - strlen(m)));
        return(wstring);
}

char    *fnfs02(m, y)
char    *m;
int     y;
{
        sprintf(wstring, "%s%s", m, space(y - strlen(m) - 1));
        return(wstring);
}

char    *fnmode(m)
char    *m;
{
        char    CO[64];

        if (!strcmp(m, "WT"))
                strcpy(CO, RED);
        else
                strcpy(CO, BLUE);
        sprintf(fnstring, "%s%s%s%s", fncurp(3, 78), CO, m, GREEN);
        return(fnstring);
}

char    *fnmsg(m)
char    *m;
{
        sprintf(wstring, "&*msg=*%s%c", m, BELL);
        return(wstring);
}

char    *fnmsg2(m)
char    *m;
{
        sprintf(wstring, "*msg=*%s", m);
        return(wstring);
}

fullscr() {
        int     i;
        int     idx;
        char    pcombuf[16];
        int     k;
        int     t;
        char    *p;

        if (!added) {
                curl = 4;
                curc = 1;
        }
        insert = FALSE;
        printf(fnmode(m29));
        bnn = 2;
L3210:  pcl = fncl(curl);
        pcc = fncc(curc);
        printf(fncurp(curl, curc));
        t = readchar();
	if (t == END)
                return(0);
        if (t == ESC) {
                compred();
                for (i = cl; i <= cl + ll - 4 && i <= fl; ++i) {
                        free(l[i]);
                        l[i] = (char *)rmalloc(strlen(xave[i - cl]) + 1);
                        strcpy(l[i], xave[i - cl]);
                }
                nullcmd();
                added = FALSE;
                return(-1);
        }
        if (t == DEL)
                goto L3510;
        if (t == FTAB) {
                if (strcmp(curtab, "off"))
                        ptabs();
                goto L3210;
        }
        if (t == BTAB) {
		if (strcmp(curtab, "off"))
                        pbtabs();
                goto L3210;
        }
L3360:  if (t == RT) {
                if (curc == 79)
                        putchar(BELL);
                else {
                        ++curc;
                        if (curc == 7)
                                ++curc;
                }
                goto L3210;
        }
        if (t == LT || t == BS) {
                if (curc == 1)
                        curc = 8 + v2 - v1;
                else
                        --curc;
                if (curc == 7)
                        curc = 6;
		goto L3210;
        }
        if (t == DN) {
                if (curl == ll)
                        curl = 4;
                else
                        ++curl;
                goto L3210;
        }
        if (t == CR) {
                pcr();
                goto L3210;
        }
        if (t == HOME) {
                curc = 1;
                curl = 4;
                goto L3210;
        }
        if (t == CTRLHOME) {
                curc = 1;
                goto L3210;
	}
/*      This area is commented out as it does not work on the IBM PC
        because the SHIFT key will not work with ARROWS, HOME, etc.
        if (t == SHFTHOME) {
                if (v1 == 1) {
                        msg = fnmsg(m06);
                        pcli();
                        printf(fnmode(m29));
                        goto L3210;
                }
                v1 = 1;
                v2 = 72;
                sprintf(buf, "Viewport=(%03d, %03d)",
                        v1, v2);
                msg = fnmsg2(buf);
                v1 = 1;
                v2 = 72;
                curc = 8;
                pcli();
                pw();
                printf(fnmode(m29));
		goto L3210;
        }
        if (t == SHFTRT) {
                if (v1 > 1) {
                        v1 -= 36;
                        v2 -= 36;
                        sprintf(buf, "Viewport=(%03d, %03d)",
                                v1, v2);
                        msg = fnmsg2(buf);
                        pcli();
                        dty3();
                        pw();
                        printf(fnmode(m29));
                        goto L3210;
                }
                else {
                        msg = fnmsg(m06);
                        pcli();
                        printf(fnmode(m29));
                        goto L3210;
                }
	}
        if (t == SHFTLT) {
                if (v2 < 252) {
                        v1 += 36;
                        v2 += 36;
                        sprintf(buf, "Viewport=(%03d, %03d)",
                                v1, v2);
                        msg = fnmsg2(buf);
                        pcli();
                        dty3();
                        pw();
                        printf(fnmode(m29));
                        goto L3210;
                }
                else {
                        msg = fnmsg(m06);
                        pcli();
                        printf(fnmode(m29));
                        goto L3210;
                }
        }
*/
        if (t == UP) {
                if (curl == 4)
                        curl = ll;
                else
                        --curl;
                goto L3210;
        }
        if (t == INS) {
                if (!insert) {
                        insert = TRUE;
                        bnn = 3;
                        printf("%s%c", fncurp(1, 78), '+');
                }
                else {
                        insert = FALSE;
                        bnn = 2;
                        printf("%s ", fncurp(1, 78));
                }
                goto L3210;
        }
	if (t == CTRLUP) {
                curl = 4;
                goto L3210;
        }
        if (t == CTRLDN) {
                curl = ll;
                goto L3210;
        }
        if (t == CTRLRT) {
                if (curc < 8) {
                        curc = 8;
                        goto L3210;
                }
                if (curc == 79) {
                        curc = 1;
                        goto L3210;
                }
                bnn = strlen(l[pcl]);
                if (bnn > v2)
                        goto L3920;
                en = lm - (v2 - bnn) + 7;
		if (curc < en) {
                        curc = en;
                        goto L3210;
                }
L3920:          if (curc < 79) {
                        curc = 79;
                        goto L3210;
                }
                goto L3210;
        }
        if (t == CTRLLT) {
                if (curc == 1) {
                        curc = 79;
                        goto L3210;
                }
                if (curc < 9) {
                        curc = 1;
                        goto L3210;
                }
                bnn = strlen(l[pcl]);
                if (bnn > v2)
			goto L4000;
                en = lm - (v2 - bnn) + 7;
                if (en == 7) {
                        curc = 8;
                        goto L3210;
                }
                if (curc > en) {
                        curc = en;
                        goto L3210;
                }
L4000:          if (curc > 8) {
                        curc = 8;
                        goto L3210;
                }
                goto L3210;
        }
L3510:  if (curc < 8)
                goto L3650;

/* FULL SCREEN EDIT AREA TYPE III */

	if (pcl == 0) { /* Don't enter characters on "TOF" marker */
                putchar(BELL);
                fflush(stdout);
                goto L3210;
        }
        if (t == DEL) {
                strcpy(wstring, l[pcl]);
                wstring[pcc - 1] = '\0';
                strcat(wstring, &wstring[pcc]);
                free(l[pcl]);
                l[pcl] = rmalloc(strlen(wstring) + 1);
                strcpy(l[pcl], wstring);
                displ();
                goto L3210;
        }
        if (t == CTRLDEL) {
                strcpy(wstring, l[pcl]);
                for (k = pcc - 1; k < CMAX; ++k)
                        wstring[k] = '\0';
		free(l[pcl]);
		l[pcl] = rmalloc(strlen(wstring) + 1);
		strcpy(l[pcl], wstring);
		displ();
		goto L3210;
	}
	if (insert) {
		if (strlen(l[pcl]) == CMAX - 4) {
			putchar(BELL);
			goto L3210;
		}
/* Always allocate another character to the string, thanx c-terp */
/* OLD/BUG      if (strlen(l[pcl]) < pcc) { */
			p = fnfs02(l[pcl], pcc);
			free(l[pcl]);
			l[pcl] = rmalloc(strlen(p) + 2);
			strcpy(l[pcl], p);
/* OLD          } */
		idx = strlen(l[pcl]);
		for (i = idx; i >= pcc - 1; --i)
			l[pcl][i] = l[pcl][i - 1];
		l[pcl][pcc - 1] = t;
		l[pcl][idx + 1] = '\0';
		displ();
		t = RT;
		goto L3360;
	}
	if (ftrap) {
		putchar(BELL);
                fflush(stdout);
                goto L3210;
        }
        /* Replace characters */
        if (strlen(l[pcl]) < pcc) {
                p = fnfs01(l[pcl], pcc);
                free(l[pcl]);
                l[pcl] = rmalloc(strlen(p) + 1);
                strcpy(l[pcl], p);
        }
        if (!l[pcl][pcc - 1])
                l[pcl][pcc] = '\0'; /* Ensure null termination */
        l[pcl][pcc - 1] = t;
        putchar(t);
        t = RT;
	goto L3360;

/* TYPE II COMMAND AREA */

L3650:  cml = curl - 4;
        if (t == DEL) {
                cm[cml][curc - 1] = '\0';
                strcat(cm[cml], &cm[cml][curc]);
                strcpy(pcombuf, m19);
                for (k = 0; k < 7; ++k) {
                        if (cm[cml][k] && cm[cml][k] != ' ')
                                pcombuf[k] = cm[cml][k];
                }
                printf("%s%s%s", fncurp(curl, 1), GREEN, pcombuf);
                goto L3210;
        }
        if (ftrap) {
                msg = fnmsg(m11);
                pcli();
                printf(fnmode(m29));
                goto L3210;
	}
        t = toupper(t);
        switch (t) {
                case '/':
                case 'C':
                case 'M':
                case 'K':
                case 'A':
                case 'D':
                case '"':
                case 'I':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
		case ' ':
                        break;
                default:
                        msg = fnmsg(m11);
                        pcli();
                        printf(fnmode(m29));
                        goto L3210;
        }
        if (insert)
                goto L3730;
        if (strlen(cm[cml]) < curc)
                strcpy(cm[cml], fnfs01(cm[cml], curc - 1));
        cm[cml][curc - 1] = t;
        cm[cml][curc] = '\0';
        if (t != ' ') {
                ty2red[cml] = TRUE;
                putchar(t);
        }
        t = RT;
        goto L3360;
L3730:  /* Insert in type II area */
	cm[cml][6] = '\0';
        for (k = 6; k >= 0; --k) {
                if (cm[cml][k]) {
                        if (cm[cml][k] == ' ') {
                                cm[cml][k] = '\0';
                                continue;
                        }
                        while(k) {
                                if (!cm[cml][k])
                                        cm[cml][k] = ' ';
                                --k;
                        }
                        goto STOPFILL;
                }
        }
STOPFILL:
        if (strlen(cm[cml]) == 6) {
                putchar(BELL);
                goto L3210;
        }
        for (i = 6; i >= curc - 1; --i)
		cm[cml][i] = cm[cml][i - 1];
        cm[cml][curc - 1] = t;
        if (t != ' ') {
                ty2red[cml] = TRUE;
        }
        strcpy(pcombuf, m19);
        for (k = 0; k < 7; ++k) {
                if (cm[cml][k] && cm[cml][k] != ' ')
                        pcombuf[k] = cm[cml][k];
        }
        printf("%s%s%s", fncurp(curl, 1), GREEN, pcombuf);
        ++curc;
        if (curc == 7)
                ++curc;
        goto L3210;
}

getline() {
        int     t;
        int     i;
        int     j;
	int     len;

        insert = FALSE;
        if (!recall) {
                for (i = 0; i < 128; ++i)
                        COMMAND[i] = '\0';
        }
        else
                displc(0, ERSLN);
        i = 0;
L6640:
        t = readchar();
        if (t == ESC) {         /* Clear command line */
                for (i = 0; i < 128; ++i)
                        COMMAND[i] = '\0';
                i = 0;
                printf("%s%s%s", fncurp(1, 6), ERSLN, WHITE);
                if (insert) {
                        printf("%s%c", fncurp(1, 78), '+');
                }
                printf(fncurp(1, 6));
		fflush(stdout);
                goto L6640;
        }
        if (t == CTRLDEL) {
                for (j = i; j < 128; ++j)
                        COMMAND[j] = '\0';
                printf("%s%s%s", fncurp(1, 6 + i), ERSLN, WHITE);
                goto L6640;
        }
        if (t == CTRLLT) {
                i = 0;
                printf("%s", fncurp(1, 6));
                goto L6640;
        }
        if (t == CTRLRT) {
                i = strlen(COMMAND) - 1;
                printf("%s", fncurp(1, 6 + i));
                goto L6640;
        }
        if (t == CR || t == END) {
                strcpy(sa, parse(COMMAND));
		strcpy(COMMAND, sa);
                printf("%s%s%s%s",
                        fncurp(1, 6), WHITE, COMMAND, ERSLN);
                ftrap = 0;
                return;
        }
        if (t == DEL) {
                COMMAND[i] = '\0';
                strcat(COMMAND, &COMMAND[i + 1]);
                displc(i, " ");
                goto L6640;
        }
        if (t == LT || t == BS) {
                if (!i) {
                        putchar(BELL);
                        goto L6640;
                }
                printf("%s", fncurp(1, --i + 6));
                goto L6640;
        }
        if (t == INS) {
		if (!insert) {
                        insert = TRUE;
                        printf("%s%c", fncurp(1, 78), '+');
                }
                else {
                        insert = FALSE;
                        printf("%s ", fncurp(1, 78));
                }
                printf("%s", fncurp(1, i + 6));
                goto L6640;
        }
        if (t == '\t') {
                putchar(BELL);
                goto L6640;
        }
        len = strlen(COMMAND);
        if (t == RT) {
                if (i == len) {
                        putchar(BELL);
                        goto L6640;
                }
		printf("%s", fncurp(1, ++i + 6));
                goto L6640;
        }
        if (t >= 128) {         /* function key detected */
                return;         /* value in ftrap */
        }
        if (insert)
                goto INSERT;
        if (len == 72 && i == 72) {
                putchar(BELL);
                goto L6640;
        }
        COMMAND[i++] = t;
        if (i == len)
                COMMAND[i] = '\0';
        putchar(t);
        goto L6640;
INSERT: if (len == 72) {
                putchar(BELL);
                goto L6640;
        }
	if (insert) {
                for (j = len; j >= i; --j)
                        COMMAND[j] = COMMAND[j - 1];
                COMMAND[i] = t;
                COMMAND[len + 1] = '\0';
                displc(i, "");
		printf("%s", fncurp(1, ++i + 6));
		goto L6640;
	}
}

ifstack() {
	FILE    *fd;
	int     i;
	int     r;

	if (!fl)
		icl = 0;
	bnn = icl;
	if (!(fd = fopen(m23, "r"))) {
		return(-1);
	}
	fgets(COMMAND, CMAX, fd);
	r = atoi(COMMAND);
	fl += r;
	l = (char **)rrealloc((char *)l, ((fl + 1) * sizeof(char *)));
	en = bnn + r;
	for (i = fl; i >= en; --i)
		l[i] = l[i - r];
	++bnn;
	en = bnn + r - 1;
	for (i = bnn; i <= en; ++i) {
		fgets(sa, CMAX, fd);
		rmlf(sa);
		l[i] = rmalloc(strlen(sa) + 1);
		if (strlen(sa))
			strcpy(l[i], sa);
		else
			l[i][0] = '\0';
	}
	fclose(fd);
	msg = fnmsg2(m27);
	nn = r;
	return(0);
}

init() {
	int     i;
	int     j = 0;

	setcolor();
	CLRHM = DEFCLRHM;
	ERSEND = DEFERSEND;
	ERSLN = DEFERSLN;
	BLANK = DEFBLANK;
	DUBQ = DEFDUBQ;
	BELL = DEFBELL;
	CR = DEFCR;
	BS = DEFBS;
	sprintf(ruler, "*----* %s", string(72, '.'));
	strcpy(curtab, "tabs off");
	strcpy(bn, "?.?");
	strcpy(fv[0], "FO");
	strcpy(fv[1], "BA");
	strcpy(fv[2], "N");
	strcpy(fv[3], "U");
	strcpy(fv[4], "CURLIN");
	strcpy(fv[5], "VIEW");
	strcpy(fv[6], "LD");
	strcpy(fv[7], "LU");
	fkeys[0] = 0073;
	fkeys[1] = 0074;
	fkeys[2] = 0075;
	fkeys[3] = 0076;
	fkeys[4] = 0077;
	fkeys[5] = 0100;
	fkeys[6] = 0101;
	fkeys[7] = 0102;
	fkeys[8] = 0103;
	fkeys[9] = 0104;
	fkeys[10] = 0105;
	fkeys[11] = 0106;
	fkeys[12] = 0115;       /* RIGHT ARROW                  */
	fkeys[13] = 0113;       /* LEFT ARROW                   */
	fkeys[14] = 0110;       /* UP ARROW                     */
	fkeys[15] = 0120;       /* DOWN ARROW                   */
	fkeys[16] = 0122;       /* INSERT KEY                   */
	fkeys[17] = 0123;       /* DELETE KEY                   */
	fkeys[18] = 0071;       /* CTRL-DELETE KEY              */
	fkeys[19] = 0164;       /* CTRL-RIGHT ARROW             */
	fkeys[20] = 0204;       /* CTRL-UP ARROW                */
	fkeys[21] = 0166;       /* CTRL-DN ARROW                */
	fkeys[22] = 0163;       /* CTRL-LT ARROW                */
	fkeys[23] = 0107;       /* HOME KEY                     */
#ifdef UNIX
	fkeys[24] = 0133;       /* SHIFT-HOME KEY               */
#else
	fkeys[24] = 0206;       /* SHIFT-HOME KEY               */
#endif
	fkeys[25] = 0167;       /* CTRL-HOME KEY                */
#ifdef UNIX
	fkeys[26] = 0175;       /* SHIFT-RT                     */
	fkeys[27] = 0173;       /* SHIFT-LT                     */
#else
	fkeys[26] = 0213;       /* SHIFT-RT                     */
	fkeys[27] = 0212;       /* SHIFT-LT                     */
#endif
	fkeys[28] = 0017;       /* BACK-TAB                     */
	fkeys[29] = 0117;       /* END KEY                      */
	for (i = 128; i < 158; ++i)
		rkeys[j++] = i;
	dty3();
}

instr(s, t)
char    *s;
char    *t;
{
	int     i, j, k;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;
		if (t[k] == '\0')
			return(i);
	}
	return(-1);
}

k2stack() {
	FILE    *fd;
	FILE    *fdt;
	int     i;
	char    mbuf[64];

	if (!fl)
		return(-1);
	if (!(fd = fopen(m23, "r"))) {
		c2stack();
		return(0);
	}
	if (!(fdt = fopen(m24, "w"))) {
		sprintf(mbuf, m32, errno);
		msg = fnmsg(mbuf);
		return(-1);
	}
	swork();
	fgets(COMMAND, CMAX, fd);
	sprintf(iobuf, "%d\n", atoi(COMMAND) + (en - bnn + 1));
	nchars = strlen(iobuf);
	if (fwrite(iobuf, 1, nchars, fdt) != nchars) {
		sprintf(mbuf, m35, errno);
		msg = fnmsg(mbuf);
		return(-1);
	}
	while (fgets(COMMAND, CMAX, fd)) {
		rmlf(COMMAND);
		sprintf(iobuf, "%s\n", COMMAND);
		nchars = strlen(iobuf);
		if (fwrite(iobuf, 1, nchars, fdt) != nchars) {
			sprintf(mbuf, m35, errno);
			msg = fnmsg(mbuf);
			return(-1);
		}
	}
	fclose(fd);
	for (i = bnn; i <= en; ++i) {
		sprintf(iobuf, "%s\n", l[i]);
		nchars = strlen(iobuf);
		if (fwrite(iobuf, 1, nchars, fdt) != nchars) {
			sprintf(mbuf, m35, errno);
			msg = fnmsg(mbuf);
			return(-1);
		}
	}
	fclose(fdt);
#ifdef UNIX
	unlink(m23);
	link(m24, m23);
	unlink(m24);
#else
	rename(m23, m24);
#endif
	return(0);
}

loc(direct)
int     direct;
{
        char    *p;
        int     i;
        int     oldclp = cl;
        int     sret;

        if (!fl) {
                msg = fnmsg(m00);
                return(-1);
        }
        if (COMMAND[2] == ' ' && COMMAND[3]) {
                p = &COMMAND[3];
                if ((*p != delim) || (p[strlen(p) - 1] != delim)) {
                        msg = fnmsg(m05);
			return(-1);
                }
                ++p;
                p[strlen(p) - 1] = '\0';
                strcpy(ss, p);
        }
        if (!*ss) {
                msg = fnmsg(m15);
                return(-1);
        }
        printf(fnmode(m01));
        fflush(stdout);
        switch (direct) {
                case LOCUP:
                        if (cl <= 1)
                                cl = fl + 1;
                        for (i = cl - 1; i >= 1; --i)
                                if ((sret = locwork(i, oldclp)) <= 0)
                                        return(sret);
                        cl = 0;
                        break;
		case LOCDOWN:
                        if (cl == fl)
                                cl = 0;
                        for (i = cl + 1; i <= fl; ++i)
                                if ((sret = locwork(i, oldclp)) <= 0)
                                        return(sret);
                        cl = fl;
                        break;
        }
        msg = fnmsg(m16);
        return(0);
}
