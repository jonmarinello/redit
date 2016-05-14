#include "redit.h"

locwork(i, oldclp)
int     i;
int     oldclp;
{
        if (instr(l[i], ss) > -1) {
                cl = i;
                if (oldclp == cl)
                        return(-1);
		else
                        return(0);
        }
        return(1);
}

lp() {
        FILE    *fd;
        char    *p;

        if (!(fd = fopen(m25, "r"))) {
                if (!(fd = fopen(m31, "r"))) {
                        return;
                }
        }
        start = TRUE;
        while (fgets(COMMAND, 128, fd)) {
                rmlf(COMMAND);
                /* Parse comments starting with '#' character */
                if ((p = strchr(COMMAND, '#'))) {
                        *p = '\0';
		}
                if (!strlen(COMMAND))
                        continue;
                shiftup();
                strcpy(sa, parse(COMMAND));
                strcpy(COMMAND, sa);
                pc();
        }
        fclose(fd);
        start = FALSE;
        msg = "";
        return;
}

clr(l1, l2)
int     l1;
int     l2;
{
        int     i;

        for (i = l1; i <= l2; ++i)
		printf("%s%s", fncurp(i, 1), ERSLN);
}

nullcmd() {
        int     i;
        int     j;

        for (i = 0; i <= 19; ++i)
                for (j = 0; j <= 7; ++j)
                        cm[i][j] = '\0';
}

char    *parse(com)
char    *com;
{
        char    *d;
        char    *p;
        char    *q;
        char    *z;
        int     len;

	p = strchr(com, '\t');          /* remove all tabs              */
        while (p) {
                *p = ' ';
                p = strchr(com, '\t');
        }
	while (*com == ' ')             /* remove all leading spaces    */
                ++com;
        p = strchr(com, ' ');
        while (p) {
                d = strchr(com, delim); /* find delimeter               */
                if (d && d < p)         /* if past it stop              */
                        break;
                ++p;
		if (*p == ' ') {        /* remove embedded succ. spaces */
                        q = p;
                        z = p - 1;
                        while (*z++ = *q++)
                                ;
                        --p;
                }
                p = strchr(p, ' ');
        }
        len = strlen(com) - 1;
	while (com[len] == ' ')         /* remove trailing spaces       */
                com[len--] = '\0';
	if (toupper(*com) == 'C') {     /* remove spaces inside (C)hange*/
                if ((d = strrchr(com, delim))) {
                        ++d;
                        if ((q = strchr(d, 'g'))) {
                                *d = 'g';
                        }
                        else if ((q = strchr(d, 'G'))) {
                                *d = 'G';
                        }
                        ++d;
                        *d = '\0';
                }
        }
        return(com);
}

pbtabs() {
        int     i;

        if (curc == 8) {
                curc = 1;
                return;
        }
        for (i = 24; i >= 0; --i) {
                if (!tabs[i])
                        continue;
                if ((pcc > tabs[i]) && (tabs[i] >= v1)) {
                        curc = tabs[i] - v1 + 8;
                        return;
                }
        }
        curc = 8;
}

pc() {
        char    buf[64];
        char    nss[51];
        int     oldclp;
        int     i;
        int     j;

        for (;;) {
                for (i = cl; i <= cl + ll - 4 && i <= fl; ++i) {
                        strcpy(xave[i - cl], l[i]);
                }
RECALL:         strcpy(sn, bn);
                if (ftrap) {
                        if (ftrap == UP) {
GOUP:                           if (hptr < HISTMAX) {
                                        if (!*history[hptr]) {
                                                goto L3200;
                                        }
                                        if (!strcmp(COMMAND, history[hptr])) {
                                                ++hptr;
                                                goto GOUP;
                                        }
                                        strcpy(COMMAND, history[hptr++]);
                                        recall = TRUE;
                                        getline();
                                        shiftup();
                                        recall = FALSE;
                                        goto RECALL;
                                }
                                else {
                                        goto L3200;
                                }
                        }
                        if (ftrap == DN) {
GODN:                           if (hptr) {
                                        --hptr;
                                        if (!strcmp(COMMAND, history[hptr]))
                                                goto GODN;
                                        strcpy(COMMAND, history[hptr]);
                                        recall = TRUE;
                                        getline();
                                        shiftup();
                                        recall = FALSE;
                                        goto RECALL;
                                }
                                else {
                                        goto L3200;
                                }
                        }
                        hptr = 0;
                        j = 128;
                        for (i = 0; i < 12; ++i) {
                                if (ftrap == j++) {
                                        strcpy(COMMAND, fv[i]);
                                        printf("%s%s%s%s",
                                                fncurp(1, 6), WHITE,
                                                COMMAND, ERSLN);
                                        fflush(stdout);
                                        goto L2770;
                                }
                        }
                        goto L3080;
                }
L2770:          if (!*COMMAND) {
                        pcli();
                        if (fullscr()) {
                                goto L3210;
                        }
                        type2();
                        compred();
                        nullcmd();
                        if (added) {
                                pw();
                                curl = ncurl;
                                curc = 8;
                                *COMMAND = '\0';
                                for (i = cl; i <= cl + ll - 4 && i <= fl; ++i) {
                                        strcpy(xave[i - cl], l[i]);
                                }
                                goto L2770;


                        }
                        else
                                goto L3210;
                }
                if (!strcmp(COMMAND, "Q"))
                        return;
                if (strcmp(history[0], COMMAND)) {
                        for (i = HISTMAX - 1; i > 0; --i) {
                                strcpy(history[i], history[i - 1]);
                        }
                        strcpy(history[0], COMMAND);
                }
                notf = FALSE;
                if (!strcmp(COMMAND, "<<")) {
                        if (v1 == 1) {
                                msg = fnmsg(m06);
                                goto L3100;
                        }
                        v1 = 1;
                        v2 = 72;
                        sprintf(buf, "Viewport=(%03d, %03d)",
                                v1, v2);
                        msg = fnmsg2(buf);
                        dty3();
                        goto L3210;
               }
               if (!strcmp(COMMAND, "<")) {
                        if (v1 > 1) {
                                v1 -= 36;
                                v2 -= 36;
                                sprintf(buf, "Viewport=(%03d, %03d)",
                                        v1, v2);
                                msg = fnmsg2(buf);
                                dty3();
                                goto L3210;
                        }
                        else {
                                msg = fnmsg(m06);
                                goto L3100;
                        }
                }
                if (!strcmp(COMMAND, ">")) {
                        if (v2 < CMAX - 4) {
                                v1 += 36;
                                v2 += 36;
                                sprintf(buf, "Viewport=(%03d, %03d)",
                                        v1, v2);
                                msg = fnmsg2(buf);
                                dty3();
                                goto L3210;
                        }
                        else {
                                msg = fnmsg(m06);
                                goto L3100;
                        }
                }
                if (!strcmp(COMMAND, "MONO")) {
                        if (!color)
                                goto L3100;
                        printf(fnmode(m01));
                        color = FALSE;
                        GREEN = YELLOW = RED = BLUE = WHITE;
                        dty2();
                        dty3();
                        goto L3210;
                }
                if (!strcmp(COMMAND, "COLOR")) {
                        if (color)
                                goto L3100;
                        printf(fnmode(m01));
                        color = TRUE;
                        setcolor();
                        dty2();
                        dty3();
                        goto L3210;
                }
                if (!strncmp(COMMAND, "DELIM", 5)) {
                        if (COMMAND[5] == ' ' && COMMAND[6])
                                delim = COMMAND[6];
                        else {
                                sprintf(buf, "delimeter=%c", delim);
                                msg = fnmsg2(buf);
                        }
                        goto L3100;
                }
                if (!strcmp(COMMAND, "BELL ON")) {
                        BELL = '\007';
                        goto L3100;
                }
                if (!strcmp(COMMAND, "BELL OFF")) {
                        BELL = '\0';
                        goto L3100;
                }
                if (!strcmp(COMMAND, "HELP")) {
                        dhelp();
                        dty2();
                        dty3();
                        goto L3210;
                }
                if (!strncmp(COMMAND, "TABS", 4)) {
                        soltabs(); /* show or load tabs */
                        goto L3100;
                }
                if (COMMAND[0] == 'F' && COMMAND[1] != 'O' && COMMAND[1] != 'I') {
                        solfk(); /* show or load function key */
                        goto L3100;
                }
                if (start) {
                        printf("%s: <<%s>>, %s ", m04, COMMAND, m39);
                        fflush(stdout);
                        readchar();
                        return;
                }
                if (*COMMAND == 'W') {
                        printf(fnmode(m01));
                        fflush(stdout);
                        if (COMMAND[1] == 'Q') {
                                if (COMMAND[2] == ' ' && COMMAND[3]) {
                                        strcpy(buf, &COMMAND[3]);
                                        COMMAND[1] = ' ';
                                        COMMAND[2] = '\0';
                                        strcat(COMMAND, buf);
                                }
                                else
                                        COMMAND[1] = '\0';
                                if (w2d())
                                        goto L3100;
                                return;
                        }
                        w2d();
                        goto L3100;
                }
                if (!strncmp(COMMAND, "NEW", 3)) {
                        if (COMMAND[3] == ' ' && COMMAND[4]) {
                                file = &COMMAND[4];
                                file[MAXNAME] = '\0';
                                rd();
                        }
                        else
                                rd();
                        goto L3100;
                }
                if (!strncmp(COMMAND, "FILE", 4)) {
                        if (COMMAND[4] == ' ' && COMMAND[5]) {
                                strncpy(bn, &COMMAND[5], MAXNAME);
                                bn[MAXNAME] = '\0';
                        }
                        else
                                msg = fnmsg(m05);
                        goto L3100;
                }
                if (!strcmp(COMMAND, "VIEW")) {
                        sprintf(buf, "Viewport=(%03d, %03d)", v1, v2);
                        msg = fnmsg2(buf);
                        goto L3100;
                }
                if (*COMMAND == 'N' || !strcmp(COMMAND, "B") || !strcmp(COMMAND, "T") ||
                        *COMMAND == 'U' || !strncmp(COMMAND, "FO", 2) ||
                        !strncmp(COMMAND, "BA", 2)) {
                        oldclp = cl;
                        setclp(); /* set current line pointer */
                        if (oldclp != cl) {
                                printf(fnmode(m01));
                                goto L3100;
                        }
                        else
                                goto L3100;
                }
                if (!strcmp(COMMAND, "?")) {
                        if (!*ss)
                                msg = fnmsg(m15);
                        else {
                                strncpy(nss, ss, 50);
                                nss[50] = '\0';
                                sprintf(buf, "%c%s%c", delim, nss, delim);
                                msg = fnmsg2(buf);
                        }
                        goto L3100;
                }
                if (!strcmp(COMMAND, "CURLIN")) {
                        sprintf(buf, "current line=%d", cl);
                        msg = fnmsg2(buf);
                        goto L3100;
                }
                if (!strncmp(COMMAND, "DEL", 3)) {
                        icl = cl;
                        if (dellines())
                                goto L3100;
                        else {
                                printf(fnmode(m01));
                                goto L3100;
                        }
                }
                if (!strncmp(COMMAND, "!DEL", 4)) {
                        if (COMMAND[4] != ' ' || !COMMAND[5]) {
                                msg = fnmsg(m05);
                                goto L3100;
                        }
                        printf(fnmode(m01));
                        if (unlink(&COMMAND[5])) {
                                msg = fnmsg(m02);
                                goto L3100;
                        }
                        goto L3100;
                }
                if (!strncmp(COMMAND, "LD", 2)) {
                        printf(fnmode(m01));
                        if (loc(LOCDOWN))
                                goto L3100;
                        goto L3100;
                }
                if (!strncmp(COMMAND, "LU", 2)) {
                        printf(fnmode(m01));
                        if (loc(LOCUP))
                                goto L3100;
                        goto L3100;
                }
                if (!strncmp(COMMAND, "C", 1)) {
                        printf(fnmode(m01));
                        if (replace())
                                goto L3100;
                        goto L3100;
                }
L3080:          msg = fnmsg(m03);
L3100:          if (start)
                        return;
                hptr = 0;
L3200:          compred();
L3210:          pchw();
        }
}

pchw() {
        pcli();         /* print command line */
        pw();           /* print window */
        printf("%s%s", fncurp(1, 6), WHITE);
        getline();
        shiftup();
}

pcli() {
        char    *nruler; /* New ruler */
        char    *bp;    /* BELL pointer */
        int     lmsg = 0;       /* Length of message */
        int     le = 0; /* Length of escape sequences */
        int     errmsg = 0;

        nruler = rmalloc(128);
        *nruler = '\0';
        if (*msg) {
                if (*msg == '&') {
                        ++errmsg;
                        ++msg;
                }
                if ((bp = strchr(msg, BELL))) {
                        putchar(BELL);
                        *bp = '\0';
                }
                if (strlen(msg) > 46)
                        msg[46] = '\0';
#ifdef UNIX
                if (errmsg) {
                        sprintf(nruler, "%s%s %s", YELLOW, msg, WHITE);
                        le = strlen(YELLOW) + strlen(WHITE);
                }
                else {
                        sprintf(nruler, "%s%s %s", RED, msg, WHITE);
                        le = strlen(RED) + strlen(WHITE);
                }
#else
                if (bp) {
                        sprintf(nruler, "%s%s %s", RED, msg, WHITE);
                        le = strlen(RED) + strlen(WHITE);
                }
                else {
                        sprintf(nruler, "%s%s %s", YELLOW, msg, WHITE);
                        le = strlen(YELLOW) + strlen(WHITE);
                }
#endif
                lmsg = strlen(nruler) - le;
        }
        strcat(nruler, &ruler[lmsg]);
/* DOS 2.0 & Later
        sprintf(&nruler[54 + le], " mem=%s", bn);
        nruler[strlen(nruler)] = ' ';
*/
                  printf("%s%s%s", fncurp(2, 1), bn, ERSLN);    
        printf("%s%s%s%s", fncurp(1, 1), WHITE, prompt, ERSLN);
        if (strcmp(lruler, nruler))
                printf("%s%s", fncurp(3, 1), nruler);
        printf(GREEN);
        strcpy(lruler, nruler);
        free(nruler);
        msg = "";
}

pcr() {
        if (curl == ll)
                curl = 4;
        else
                ++curl;
        if (curc < 8)
                curc = 1;
        else
		curc = 8;
}

prss() {
	printf("%s%sRedit Full Screen Editor V4.02",
		GREEN, fncurp(8, 23));
#ifdef TI
	printf("%sFor the Texas Instruments Professional Computer",
		fncurp(10, 15));
#endif
#ifdef IBMPC
	printf("%sFor the IBM PC/XT/AT and Compatibles",
		fncurp(10, 20));
#endif
	printf("%s(C) Copyright 1984 by Jon Marinello",
		fncurp(12, 20));
	fflush(stdout);
	sleep(2);
	clr(8, 14);
}

ptabs() {
        int     i;

        if (curc < 8) {
                curc = 8;
                return;
        }
        for (i = 0; i < 25; ++i) {
                if (pcc < tabs[i] && tabs[i] <= v2) {
                        curc = tabs[i] - v1 + 8;
                        return;
                }
        }
}

pw() {
        int     i;
        static  lastll = -1;

        ll = 4;
        printf(fnmode(m01));
        for (i = cl; i <= cl + 19; ++i) {
                if (!i) {
                        printf("%s%s%s%s%s%s",
                                fncurp(4, 1), m19, RED, m20, GREEN, ERSLN);
                        ++ll;
                        continue;
                }
                if (i > fl) {
                        printf("%s%s%s%s%s%s",
                                fncurp(ll, 1), m19, BLUE, m21, GREEN, ERSLN);
                        clr(ll + 1, lastll + 1);
                        break;
                }
                if (strlen(l[i]) < v1 - 1) {
                        *sa = '\0';
                }
                else {
                        strncpy(sa, l[i] + v1 - 1, lm);
                        sa[lm] = '\0';
                }
                if (lastll <= ll) {
                        printf("%s%s", fncurp(ll, 1), m19);
                        printf("%s%s%s", fncurp(ll, 8), sa, ERSLN);
                }
                else {
                        if (ty2red[ll - 4]) {
                                printf("%s%s", fncurp(ll, 1), m19);
                        }
                        if (!*sa)
                                printf("%s%s", fncurp(ll, 8), ERSLN);
                        else if (ty3red[ll - 4]) {
                                printf("%s%s%s", fncurp(ll, 8), sa, ERSLN);
                        }
                }
                ++ll;
        }
        lastll = --ll;
        bnn = 2;
        sprintf(sa, "%s..", WHITE);
        printf(fnmode(sa));
}

rd() {
        FILE    *fd;
        int     i;

        if (file) {
                strncpy(bn, file, MAXNAME);
                file = (char *)0;
                goto ARGFILE;
        }
GETFNAME:
        printf("%s%s%s%sMEM: ", GREEN,
                fncurp(1, 1), ERSLN, WHITE);
        getline();
        strcpy(bn, COMMAND);
        printf(fncurp(1, 1));
        if (*bn == '\0')
                goto GETFNAME;
ARGFILE:
        printf(fnmode(m01));
        fflush(stdout);
        bn[MAXNAME] = '\0';
        notf = FALSE;
        prompt = "===> ";
        bnn = 1;
        strcpy(COMMAND, bn);
        shiftup();
        strcpy(bn, COMMAND);
        if (l) {
                for (i = 0; i <= fl; ++i)
			free(l[i]);
                free((char *)l);
        }
        l = (char **)rmalloc(sizeof(char *));
        l[0] = rmalloc(1);
        l[0][0] = '\0';
        cl = 0;
        fd = fopen(bn, "r");
        if (!fd) {
                msg = fnmsg(m26);
                fl = 0;
                return;
        }
        i = 1;
        while (fgets(sa, CMAX - 2, fd))
                ++i;
        l = (char **)rrealloc((char *)l, sizeof(char *) * i);
        i = 1;
        rewind(fd);
        while (fgets(sa, CMAX - 2, fd)) {
                rmlf(sa);
                l[i] = rmalloc(strlen(sa) + 1);
                if (strlen(sa))
                        strcpy(l[i], sa);
                else
                        l[i][0] = '\0';
                ++i;
        }
        fl = i - 1;
}

readchar() {
        int     keystroke;
        int     i;

GET:    ftrap = 0;
        keystroke = getchar();
        if (keystroke != NULLCHAR)
                return(keystroke);
        keystroke = getchar();
        for (i = 0; i < 30; ++i) {
                if (keystroke == fkeys[i]) {
                        ftrap = rkeys[i];
                        return(rkeys[i]);
                }
        }
        putchar(BELL);
        fflush(stdout);
        goto GET;
}

replace() {
        int     i;
        char    *p;
        char    *dp;
        char    *s;
        char    *t;
        int     sx;
        int     ox;
        char    *buf;
        int     ccount = 0;
        char    *rstring;
        int     global = 0;

        if (!fl) {
                msg = fnmsg(m00);
                return(-1);
        }
        if (!cl)
                cl = 1;
        bnn = cl;
        if (COMMAND[1] != ' ' || !COMMAND[2]) {
                msg = fnmsg(m05);
                return(-1);
        }
        p = &COMMAND[2];
        if (*p == '*')
                en = fl;
        else
                en = atoi(&COMMAND[2]);
        if (en < 1)
                en = 1;
        en = bnn + en - 1;
        if (en > fl)
                en = fl;
        if (!(dp = strchr(p, delim))) {
                msg = fnmsg(m05);
                return(-1);
        }
        s = ++dp;
        if (!(dp = strchr(s, delim))) {
                msg = fnmsg(m05);
                return(-1);
        }
        *dp = '\0';
        t = ++dp;
        if (!(dp = strchr(t, delim))) {
                msg = fnmsg(m05);
                return(-1);
        }
        *dp++ = '\0';
        dp = rmlwc(dp);
        printf(fnmode(m01));
        fflush(stdout);
        buf = rmalloc(CMAX);
        rstring = rmalloc(1024);
        *buf = *rstring = '\0';
        if (*dp == 'g' || *dp == 'G')
                ++global;
        for (i = bnn; i <= en; ++i) {
                sx = instr(l[i], s);
                if (sx > -1) {
                        for (;;) {
                                ox = sx;
                                ++ccount;
                                strcpy(rstring, l[i]);
                                strcpy(buf, rstring);
                                rstring[sx] = '\0';
                                strcat(rstring, t);
                                strcat(rstring, &buf[sx + strlen(s)]);
                                rstring[CMAX - 4] = '\0';
                                l[i] = rrealloc(l[i], strlen(rstring) + 1);
                                strcpy(l[i], rstring);
                                if (!global)
                                        break;
                                if (((sx = instr(&rstring[ox + strlen(t)], s)) < 0))
                                        break;
                                sx += ox + strlen(t);
                        }
                }
        }
        if (!ccount) {
                msg = fnmsg(m16);
        }
        else {
                sprintf(buf, "changes made=%d", ccount);
                msg = fnmsg2(buf);
        }
        cl = en;
        free(buf);
        free(rstring);
        return(0);
}

rmlf(s)
char    *s;
{
        char    *p;

        if ((p = strchr(s, '\010')))
                *p = '\0';
        if ((p = strchr(s, '\013')))
                *p = '\0';
        if ((p = strchr(s, '\012')))
                *p = '\0';
        while ((p = strchr(s, '\t')))
                *p = ' ';
}

char    *rmlwc(targ)
char    *targ;
{
        while (*targ == ' ' || *targ == '\n' || *targ == '\t')
                ++targ;
        return(targ);
}

setclp() {
        bnn = cl;
        if (!strcmp(COMMAND, "T")) {
                cl = 0;
                return;
        }
        if (!strcmp(COMMAND, "B")) {
                cl = fl;
                return;
        }
        if (!strncmp(COMMAND, "U", 1)) {
                if (COMMAND[1] == ' ' && COMMAND[2])
                        cl -= atoi(&COMMAND[2]);
                if (cl == bnn)
                        --cl;
        }
        if (!strncmp(COMMAND, "N", 1)) {
                if (COMMAND[1] == ' ' && COMMAND[2])
                        cl += atoi(&COMMAND[2]);
                if (cl == bnn)
                        ++cl;
        }
        if (!strncmp(COMMAND, "FO", 2)) {
                if (COMMAND[2] == ' ' && COMMAND[3])
                        cl += atoi(&COMMAND[3]) * 18;
                if (cl == bnn)
                        cl += 18;
        }
        if (!strncmp(COMMAND, "BA", 2)) {
                if (COMMAND[2] == ' ' && COMMAND[3])
                        cl -= atoi(&COMMAND[3]) * 18;
                if (cl == bnn)
                        cl -= 18;
        }
        if (cl < 0)
                cl = 0;
        else if (cl > fl)
                cl = fl;
}

setcolor() {
        WHITE = DEFWHITE;
        YELLOW = DEFYELLOW;
        GREEN = DEFGREEN;
        RED = DEFRED;
        BLUE = DEFBLUE;
}

shiftup() {
        int     x = strlen(COMMAND);
        int     i;

        for (i = 0; i < x; ++i) {
                if (COMMAND[i] == delim)
                        return;
                COMMAND[i] = toupper(COMMAND[i]);
        }
}

solfk() {
        int     key;
        char    *p;
        char    buf[128];

        key = atoi(&COMMAND[1]);
        if (key < 1 || key > 12) {
                if (!isdigit(COMMAND[1])) {
                        msg = fnmsg(m03);
                        return;
                }
                msg = fnmsg(m07);
                return;
        }
        p = strchr(COMMAND, ' ');
        if (!p || !p[1]) {
                sprintf(buf, "F%d=%s", key, fv[key - 1]);
                msg = fnmsg2(buf);
                return;
        }
        strcpy(fv[key - 1], &p[1]);
        sprintf(buf, "F%d=%s", key, fv[key - 1]);
        msg = fnmsg2(buf);
}

soltabs() {
        char    *p;
        char    otabs[128];
        int     i;

        p = strchr(COMMAND, ' ');
        if (!p || !p[1]) {
                msg = fnmsg2(curtab);
                return;
        }
        ++p;
        for (i = 0; i < 25; ++i)
                tabs[i] = 0;
        if (!strcmp(p, "OFF")) {
                strcpy(curtab, "tabs off");
                return;
        }
        strcpy(otabs, curtab);
        if (!strcmp(p, "ASM")) {
                strcpy(curtab, asm_tabs);
                goto L7070;
        }
        if (!strcmp(p, "BAS")) {
                strcpy(curtab, bas_tabs);
                goto L7070;
        }
        if (!strcmp(p, "COB")) {
                strcpy(curtab, cob_tabs);
                goto L7070;
        }
        if (!strcmp(p, "FOR")) {
                strcpy(curtab, for_tabs);
                goto L7070;
        }
        if (!strcmp(p, "PLI")) {
                strcpy(curtab, pli_tabs);
                goto L7070;
        }
        if (!strcmp(p, "C")) {
                strcpy(curtab, c_tabs);
                goto L7070;
        }
        strcpy(curtab, p);
L7070:  i = 0;
        p = curtab;
L7100:  tabs[i] = atoi(p) + 1;
        if ((i && (tabs[i] <= tabs[i - 1] || tabs[i] > 252)) || !isdigit(*p)) {
                strcpy(curtab, otabs);
                p = curtab;
                msg = fnmsg(m08);
                return;
        }
        if (!(p = strchr(p, ',')))
                return;
        ++p;
        if (!*p)
                return;
        if (++i == 25)
                return;
        goto L7100;
}

char    *space(n)
int     n;
{
        static  char    *p;
        int     i;

        if (!p)
                p = (char *)rmalloc(256);
        if (n > 256)
                n = 256;
        for (i = 0; i < n; ++i)
                p[i] = BLANK;
        p[i] = '\0';
        return(p);
}

char    *string(n, car)
int     n;
char    car;
{
        static  char    *p;
        int     i;

        if (!p)
                p = (char *)rmalloc(256);
        if (n > 256)
                n = 256;
        for (i = 0; i < n; ++i)
                p[i] = car;
        p[i] = '\0';
        return(p);
}

swork() {
        en = nn;
        bnn = icl;
        if (en < 1)
                en = 1;
        if (!bnn)
                bnn = 1;
        en = bnn + en - 1;
        if (en > fl)
                en = fl;
}

type2() {
        int     i;
        int     j;
        int     k;
        int     x;
        int     r;
        int     di;
        char    buf[7];

        ncl = cl;
        added = FALSE;
        printf(fnmode(m01));
        fflush(stdout);
        for (i = 0; i <= 19; ++i) {
                if (!*cm[i])
                continue;
                x = strlen(cm[i]);
                *buf = '\0';
                k = 0;
                for (j = 0; j < 7 && x >= j; ++j) {
                        if (cm[i][j] != ' ')
                                buf[k++] = cm[i][j];
                }
                buf[k] = '\0';
                if (!*buf)
                        continue;
                strcpy(COMMAND, buf);
                shiftup();
                strcpy(buf, COMMAND);
                strcpy(sa, &buf[1]);
                icl = ncl + i;
                switch (COMMAND[0]) {
                        case '/':
                                if (!*sa)
                                        cl = ncl + i;
                                else {
                                        msg = fnmsg(m09);
                                        return;
                                }
                                break;
                        case 'C':
                                if (chknum())
                                        return;
                                c2stack();
                                break;
                        case 'M':
                                if (chknum())
                                        return;
                                if (c2stack())
                                        break;
				r = en - bnn + 1;
				for (di = bnn; di < bnn + r; ++di) { /* free deleted lines */
					free(l[di]);
				}
				for (di = bnn; di <= fl - r; ++di) {
					l[di] = l[di + r];
				}
				fl -= r;
				i += (nn - 1);
				ncl -= nn;
                                l = (char **)rrealloc((char *)l,
                                        (sizeof(char *) * (fl + 1)));
                                break;
                        case 'K':
                                if (chknum())
                                        return;
                                if (k2stack())
                                        return;
                                break;
                        case 'A':
                                if (chknum())
                                        return;
                                bnn = nn;
				if (addlines())
                                        return;
                                ncl += nn;
                                compadd();
                                break;
                        case 'D':
                                if (chknum())
                                        return;
                                sprintf(COMMAND, "DEL %d", nn);
                                dellines();
                                i = i + nn + 1;
                                ncl -= nn;
                                break;
                        case '"':
                                if (chknum())
                                        return;
                                bnn = nn;
                                fill = l[icl];
                                if (addlines()) {
                                        fill = "";
                                        return;
				}
                                ncl += nn;
                                compadd();
                                fill = "";
                                break;
                        case 'I':
                                if (!*sa) {
                                        if (ifstack())
                                                return;
                                        ncl += nn;
                                }
                                else {
                                        msg = fnmsg(m09);
                                        return;
                                }
                                break;
                        default:
                                msg = fnmsg(m09);
                                return;
                }
        }
	return;
}

w2d() {
        FILE    *fd;
        int     i;
        char    mbuf[64];

	if (!fl) {
		msg = fnmsg(m00);
		return(0);
	}
	if (strlen(COMMAND) > 1 && COMMAND[1] != ' ') {
		msg = fnmsg(m03);
		return(0);
	}
	bn[MAXNAME] = '\0';
	strcpy(sn, bn);
	if (strlen(COMMAND) > 2)
		strcpy(sn, &COMMAND[2]);
	fd = fopen(sn, "r");
	if (fd) {
		msg = fnmsg2(m14);
		fclose(fd);
	}
	else
		msg = fnmsg2(m13);
	if (!(fd = fopen(sn, "w"))) {
		sprintf(mbuf, m33, sn, errno);
		msg = fnmsg(mbuf);
		return(-1);
	}
	for (i = 1; i <= fl; ++i) {
		sprintf(iobuf, "%s\n", l[i]);
		nchars = strlen(iobuf);
		if (fwrite(iobuf, 1, nchars, fd) != nchars) {
			sprintf(mbuf, m36, sn, errno);
			msg = fnmsg(mbuf);
			return(-1);
		}
	}
	fclose(fd);
	return(0);
}

dhelp() {
        FILE    *fd;
        int     c;
        int     i = -1;

        if (!(fd = fopen(m37, "r"))) {
                msg = fnmsg(m38);
                return;
        }
        *lruler = '\0';
#ifdef UNIX
        printf("%s%s", CLRHM, WHITE);
#else
        printf("%s%s", CLRHM, YELLOW);
#endif
        while (fgets(sa, 80, fd)) {
                ++i;
                if (i && !(i % 21)) {
                        printf("\n%s%s ", GREEN, m40);
                        fflush(stdout);
                        c = getchar();
                        switch ((c = toupper(c))) {
                                case 'Q':
                                        goto HEXIT;
                                case 'T':
					i = -1;
                                        rewind(fd);
#ifdef UNIX
                                        printf("%s%s", CLRHM, WHITE);
#else
                                        printf("%s%s", CLRHM, YELLOW);
#endif
                                        continue;
                                default:
#ifdef UNIX
                                        printf("%s%s", CLRHM, WHITE);
#else
                                        printf("%s%s", CLRHM, YELLOW);
#endif
                                        break;
                        }
                }
                printf(sa);
        }
        printf("\n%s%s ", GREEN, m39);
        fflush(stdout);
	getchar();
HEXIT:  fclose(fd);
	printf(CLRHM);
}

char    *rmalloc(n)
unsigned        n;
{
	char    *p;

#if DEBUGF
    printf("\n\nrmalloc: entered, n = %u\n\n", n);
    getchar();
#endif
	if (!(p = malloc(n))) {
		rexit(m12);
	}
	return(p);
}

char    *rrealloc(t, n)
char    *t;
unsigned        n;
{
#if DEBUGF
    printf("\n\n\nrrealloc: enetred, t=%08lx, n = %u\n\n\n", t, n);
    getchar();
#endif
	if (!(t = realloc(t, n))) {
		rexit(m12);
	}
	return(t);
}

rexit(m)
char    *m;
{
	printf("%s%s%s", CLRHM, WHITE, m);
#ifdef UNIX
	endio();
#endif
	exit(1);
}
