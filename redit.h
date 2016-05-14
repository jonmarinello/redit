/*********************************************************************/
/*                                                                   */
/*              REDIT Full Screen Editor Version 4.01                */
/*                                                                   */
/*              Property of Radix Systems Software Inc.              */
/*                                                                   */
/*              Copyright 1986, Radix Systems Software               */
/*                                                                   */
/*      This version was derived from an earlier version (3.11)      */
/*      written in MS-BASIC. Both the BASIC and C versions were      */
/*      written by Jon Marinello. The C version was developed        */
/*      under UNIX System V and then ported to MS-DOS.               */
/*                                                                   */
/*********************************************************************/

#define IBMPC	1

#define DEBUGF  0

#include <stdio.h>
#include <ctype.h>

#ifdef UNIX
#include <termio.h>
extern  struct  termio  arg, sarg;
#endif

#ifdef UNIX
#define MAXNAME 14
#else
#define MAXNAME 12
#endif 

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
#define CTRLDEL 0222
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

#define	LOCUP		1
#define LOCDOWN		0

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
extern	char	*rrealloc();
extern	char	*malloc();
extern	char	*realloc();
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

extern	char    *WHITE;
extern	char    *YELLOW;
extern	char    *GREEN;
extern	char    *RED;
extern  char    *BLUE;
extern	char    *CLRHM;
extern	char    *ERSEND;
extern	char    *ERSLN;
extern	char    BLANK;
extern	char    BS;
extern	char    DUBQ;
extern	char    BELL;
extern	char    CR;
extern	int     en;
extern	int     recall;
extern	int     nchars;
extern	int     cml;
extern	int     pcl;
extern	int     pcc;
extern	int     insert;
extern	int     added;
extern	int     curl;
extern	int     curc;
extern	int     ncurl;
extern	int     ll;
extern  int     color;
extern  int     bnn;
extern  int     icl;
extern  int     cl;
extern  int     fl;
extern  int     start;
extern  int     ncl;
extern  char    *prompt;
extern  char    cpos[];
extern  char    ss[];
extern  char    wstring[];
extern  char    fnstring[];
extern  char    history[][128];
extern  int     hptr;
extern  char    *msg;
extern  char    *fill;
extern  char    COMMAND[];
extern  int     ftrap;
extern  char    sn[];
extern  char    bn[];
extern	char    *asm_tabs;
extern	char    *bas_tabs;
extern	char    *cob_tabs;
extern	char    *for_tabs;
extern	char    *pli_tabs;
extern	char    *c_tabs;

extern	char    **l;

extern	char    cm[][8];
extern  char    ty2red[];
extern  char    ty3red[];
extern  int     fkeys[];
extern  int     rkeys[];
extern  char    xave[][CMAX];
extern  char    fv[][72];
extern  int     tabs[];
extern  char    curtab[];
extern  char    sa[];
extern  char    iobuf[];
extern  int     notf;
extern	char    delim;
extern	int     lm;
extern	int     v1;
extern	int     v2;
extern  char    ruler[];
extern  char    lruler[]; /* Last ruler */
extern  int     nn;
extern  char    *m00;
extern  char    *m01;
extern  char    *m02;
extern  char    *m03;
extern  char    *m04;
extern  char    *m05;
extern  char    *m06;
extern  char    *m07;
extern  char    *m08;
extern  char    *m09;
extern  char    *m10;
extern  char    *m11;
extern  char    *m12;
extern	char    *m13;
extern	char    *m14;
extern	char    *m15;
extern	char    *m16;
extern	char    *m19;
extern	char    *m20;
extern  char    *m21;
extern	char    *m23;
extern	char    *m24;
extern	char    *m25;
extern	char    *m26;
extern	char    *m27;
extern	char    *m29;
extern	char    *m30;
#ifdef UNIX
extern	char    *m31;
#else
extern	char    *m31;
#endif
extern	char    *m32;
extern	char    *m33;
extern	char    *m34;
extern	char    *m35;
extern	char    *m36;
#ifdef UNIX
extern	char    *m37;
#else
extern  char    *m37;
#endif
extern	char    *m38;
extern	char    *m39;
extern	char    *m40;
extern  char    *file;
