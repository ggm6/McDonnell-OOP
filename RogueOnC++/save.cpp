/*
 * save and restore routines
 *
 * @(#)save.c	3.9 (Berkeley) 6/16/81
 */

#include "curses.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include "rogue.hpp"
#include <fcntl.h>
#include <time.h>

typedef struct stat STAT;

extern char version[], encstr[];
extern int errno;

STAT sbuf;

bool save_game()
{
    FILE *savef;
    int c;
    char buf[80];

    /*
     * get file name
     */
    mpos = 0;
    if (file_name[0] != '\0')
    {
	msg("Save file (%s)? ", file_name);
	do
	{
	    c = getchar();
	} while (c != 'n' && c != 'N' && c != 'y' && c != 'Y');
	mpos = 0;
	if (c == 'y' || c == 'Y')
	{
	    msg("File name: %s", file_name);
	    goto gotfile;
	}
    }

    do
    {
	msg("File name: ");
	mpos = 0;
	buf[0] = '\0';
	if (get_str(buf, cw) == QUIT)
	{
	    msg("");
	    return FALSE;
	}
	strcpy(file_name, buf);
gotfile:
	if ((savef = fopen(file_name, "w")) == NULL)
	    msg(strerror(errno));	/* fake perror() */
    } while (savef == NULL);

    /*
     * write out encrpyted file (after a stat)
     * The fwrite is to force allocation of the buffer before the write
     */
    save_file(savef);
    return TRUE;
}

/*
 * automatically save a file.  This is used if a HUP signal is
 * recieved
 */
void auto_save(int p)
{
    FILE *savef;
    int i;

    for (i = 0; i < NSIG; i++)
	signal(i, SIG_IGN);
    if (file_name[0] != '\0' && (savef = fopen(file_name, "w")) != NULL)
	save_file(savef);
    endwin();
    exit(1);
}

/*
 * write the saved game on the file
 */
void save_file(FILE *savef)
{
    int slines = LINES;
    int scols = COLS;
    
    wmove(cw, LINES-1, 0);
    draw(cw);
    fstat(fileno(savef), &sbuf);
    fwrite("junk", 1, 5, savef);
    fseek(savef, 0L, 0);
	
    encwrite(version,strlen(version)+1,savef);
    encwrite(&sbuf.st_ino,sizeof(sbuf.st_ino),savef);
    encwrite(&sbuf.st_dev,sizeof(sbuf.st_dev),savef);
    encwrite(&sbuf.st_ctime,sizeof(sbuf.st_ctime),savef);
    encwrite(&sbuf.st_mtime,sizeof(sbuf.st_mtime),savef);
    encwrite(&slines,sizeof(slines),savef);
    encwrite(&scols,sizeof(scols),savef);
    
    rs_save_file(savef);

    fclose(savef);
}

bool restore(char *file, char **envp)
{
    int inf;
    extern char **environ;
    char buf[80];
    STAT sbuf2;
    int slines, scols;
    
    if (strcmp(file, "-r") == 0)
	file = file_name;
    if ((inf = open(file, 0)) < 0)
    {
	perror(file);
	return FALSE;
    }

    fflush(stdout);
    encread(buf, strlen(version) + 1, inf);
    if (strcmp(buf, version) != 0)
    {
	printf("Sorry, saved game is out of date.\n");
	return FALSE;
    }

    fstat(inf, &sbuf2);
    fflush(stdout);

/*    lseek(inf, 0L, 0);*/

    encread(&sbuf.st_ino,sizeof(sbuf.st_ino), inf);
    encread(&sbuf.st_dev,sizeof(sbuf.st_dev), inf);
    encread(&sbuf.st_ctime,sizeof(sbuf.st_ctime), inf);
    encread(&sbuf.st_mtime,sizeof(sbuf.st_mtime), inf);
    encread(&slines,sizeof(slines),inf);
    encread(&scols,sizeof(scols),inf);

    /*
     * we do not close the file so that we will have a hold of the
     * inode for as long as possible
     */

    if (!wizard)
	if (sbuf2.st_ino != sbuf.st_ino || sbuf2.st_dev != sbuf.st_dev)
	{
	    printf("Sorry, saved game is not in the same file.\n");
	    return FALSE;
	}
	else if (sbuf2.st_ctime - sbuf.st_ctime > 15)
	{
	    printf("Sorry, file has been touched.\n");
	    return FALSE;
	}

    initscr();
	
    if (slines > LINES)
    {
    	printf("Sorry, original game was played on a screen with %d lines.\n",slines);
		printf("Current screen only has %d lines. Unable to restore game\n",LINES);
		return(FALSE);
    }
	
    if (scols > COLS)
    {
    	printf("Sorry, original game was played on a screen with %d columns.\n",scols);
		printf("Current screen only has %d columns. Unable to restore game\n",COLS);
		return(FALSE);
    }
    
    cw = newwin(LINES, COLS, 0, 0);
    mw = newwin(LINES, COLS, 0, 0);
    hw = newwin(LINES, COLS, 0, 0);
    nocrmode();    
    mpos = 0;
    mvwprintw(cw, 0, 0, "%s: %s", file, (const time_t*) ctime(&sbuf2.st_mtime));

    /*
     * defeat multiple restarting from the same place
     */
    if (!wizard	&& (sbuf2.st_nlink != 1))
    {
		printf("Cannot restore from a linked file\n");
		return FALSE;
    }

    if (rs_restore_file(inf) == FALSE)
    {
		printf("Cannot restore file\n");
    	return(FALSE);
    }
	
    if (!wizard && (unlink(file) < 0))
    {
    	printf("Cannot unlink file\n");
		return FALSE;
    }

    environ = envp;
    strcpy(file_name, file);
    setup();
    clearok(curscr, TRUE);
    touchwin(cw);
    srand(getpid());
    status();
    playit();
    /*NOTREACHED*/
}

/*
 * perform an encrypted write
 */
void encwrite(void *starta, unsigned int size, FILE *outf)
{
    char *ep;
    char *start = (char* ) starta;

    ep = encstr;

    while (size--)
    {
	putc(*start++ ^ *ep++, outf);
	if (*ep == '\0')
	    ep = encstr;
    }
}

/*
 * perform an encrypted read
 */
int encread(void *starta, unsigned int size, int inf)
{
    char *ep;
    int read_size;
    char *start = (char* ) starta;

    if ((read_size = read(inf, start, size)) == -1 || read_size == 0)
	return read_size;

    ep = encstr;

    while (size--)
    {
	*start++ ^= *ep++;
	if (*ep == '\0')
	    ep = encstr;
    }
    return read_size;
}
