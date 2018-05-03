/*
 * global variable initializaton
 *
 * @(#)init.c	3.33 (Berkeley) 6/15/81
 */

#include "curses.h"
#include <ctype.h>
#include "rogue.hpp"

bool playing = TRUE, running = FALSE, wizard = FALSE;
bool notify = TRUE, fight_flush = FALSE, terse = FALSE, door_stop = FALSE;
bool jump = FALSE, slow_invent = FALSE, firstmove = FALSE, askme = FALSE;
bool amulet = FALSE, in_shell = FALSE;
struct linked_list *lvl_obj = NULL, *mlist = NULL;
struct object *cur_weapon = NULL;
int mpos = 0, no_move = 0, no_command = 0, level = 1, purse = 0, inpack = 0;
int total = 0, no_food = 0, count = 0, fung_hit = 0, quiet = 0;
int food_left = HUNGERTIME, group = 1, hungry_state = 0;
int lastscore = -1;

struct thing player;
struct room rooms[MAXROOMS];
struct room *oldrp;
struct stats max_stats; 
struct object *cur_armor;
struct object *cur_ring[2];
bool after;
bool waswizard;
coord oldpos;                            /* Position before last look() call */
coord delta;                             /* Change indicated to get_dir()    */

bool s_know[MAXSCROLLS];         /* Does he know what a scroll does */
bool p_know[MAXPOTIONS];         /* Does he know what a potion does */
bool r_know[MAXRINGS];                   /* Does he know what a ring does
 */
bool ws_know[MAXSTICKS];         /* Does he know what a stick does */

char take;                               /* Thing the rogue is taking */
char runch;                              /* Direction player is running */
char whoami[80];                 /* Name of player */
char fruit[80];                          /* Favorite fruit */
char huh[80];                            /* The last message printed */
int dnum;                                /* Dungeon number */
char *s_names[MAXSCROLLS];               /* Names of the scrolls */
char *p_colors[MAXPOTIONS];              /* Colors of the potions */
char *r_stones[MAXRINGS];                /* Stone settings of the rings */
//char *a_names[MAXARMORS];                /* Names of armor types */
char *ws_made[MAXSTICKS];                /* What sticks are made of */
char *s_guess[MAXSCROLLS];               /* Players guess at what scroll is */
char *p_guess[MAXPOTIONS];               /* Players guess at what potion is */
char *r_guess[MAXRINGS];         /* Players guess at what ring is */
char *ws_guess[MAXSTICKS];               /* Players guess at what wand is */
char *ws_type[MAXSTICKS];                /* Is it a wand or a staff */
char file_name[80];                      /* Save file name */
char home[80];                           /* User's home directory */
char prbuf[80];                          /* Buffer for sprintfs */
char outbuf[BUFSIZ];                     /* Output buffer for stdout */
int max_hp;                              /* Player's max hit points */
int ntraps;                              /* Number of traps on this level */
int max_level;                           /* Deepest player has gone */
int seed;                                /* Random number seed */

struct trap  traps[MAXTRAPS];


#define ___ 1
#define _x {1,1}
struct monster monsters[26] = {
	/* Name		 CARRY	FLAG    str, exp, lvl, amr, hpt, dmg */
	{ (char* ) "giant ant",	 0,	ISMEAN,	{ _x, 10,   2,   3, ___, (char* ) "1d6" } },
	{ (char* ) "bat",	 0,	0,	{ _x,  1,   1,   3, ___, (char* ) "1d2" } },
	{ (char* ) "centaur",	 15,	0,	{ _x, 15,   4,   4, ___, (char* ) "1d6/1d6" } },
	{ (char* ) "dragon",	 100,	ISGREED,{ _x,9000, 10,  -1, ___, (char* ) "1d8/1d8/3d10" } },
	{ (char* ) "floating eye",0,	0,	{ _x,  5,   1,   9, ___, (char* )"0d0" } },
	{ (char* ) "violet fungi",0,	ISMEAN,	{ _x, 85,   8,   3, ___, (char* )"000d0" } },
	{ (char* ) "gnome",	 10,	0,	{ _x,  8,   1,   5, ___, (char* )"1d6" } },
	{ (char* ) "hobgoblin",	 0,	ISMEAN,	{ _x,  3,   1,   5, ___, (char* )"1d8" } },
	{ (char* )"invisible stalker",0,ISINVIS,{ _x,120,   8,   3, ___, (char* ) "4d4" } },
	{ (char* ) "jackal",	 0,	ISMEAN,	{ _x,  2,   1,   7, ___, (char* ) "1d2" } },
	{ (char* )"kobold",	 0,	ISMEAN,	{ _x,  1,   1,   7, ___, (char* ) "1d4" } },
	{ (char* ) "leprechaun",	 0,	0,	{ _x, 10,   3,   8, ___, (char* )"1d1" } },
	{ (char* )"mimic",	 30,	0,	{ _x,140,   7,   7, ___, (char* ) "3d4" } },
	{ (char* ) "nymph",	 100,	0,	{ _x, 40,   3,   9, ___, (char* ) "0d0" } },
	{ (char* ) "orc",	 15,	ISBLOCK,{ _x,  5,   1,   6, ___, (char* )"1d8" } },
	{ (char* ) "purple worm", 70,	0,	{ _x,7000, 15,   6, ___, (char* ) "2d12/2d4" } },
	{ (char* )"quasit",	 30,	ISMEAN,	{ _x, 35,   3,   2, ___, (char* ) "1d2/1d2/1d4" } },
	{ (char* ) "rust monster",0,	ISMEAN,	{ _x, 25,   5,   2, ___, (char* ) "0d0/0d0" } },
	{ (char* ) "snake",	 0,	ISMEAN,	{ _x,  3,   1,   5, ___, (char* )"1d3" } },
	{ (char* ) "troll",	 50,	ISREGEN|ISMEAN,{ _x, 55,   6,   4, ___, (char* ) "1d8/1d8/2d6" } },
	{ (char* )"umber hulk",	 40,	ISMEAN,	{ _x,130,   8,   2, ___, (char* ) "3d4/3d4/2d5" } },
	{ (char* ) "vampire",	 20,	ISREGEN|ISMEAN,{ _x,380,   8,   1, ___, (char* ) "1d10" } },
	{ (char* ) "wraith",	 0,	0,	{ _x, 55,   5,   4, ___, (char* ) "1d6" } },
	{ (char* ) "xorn",	 0,	ISMEAN,	{ _x,120,   7,  -2, ___, (char* ) "1d3/1d3/1d3/4d6" } },
	{ (char* ) "yeti",	 30,	0,	{ _x, 50,   4,   6, ___, (char* ) "1d6/1d6" } },
	{ (char* ) "zombie",	 0,	ISMEAN,	{ _x,  7,   2,   8, ___, (char* ) "1d8" } }
};
#undef ___

/*
 * init_player:
 *	roll up the rogue
 */

void init_player()
{
    pstats.s_lvl = 1;
    pstats.s_exp = 0L;
    max_hp = pstats.s_hpt = 12;
    if (rnd(100) == 7)
    {
	pstats.s_str.st_str = 18;
	pstats.s_str.st_add = rnd(100) + 1;
    }
    else
    {
	pstats.s_str.st_str = 16;
	pstats.s_str.st_add = 0;
    }
    strcpy(pstats.s_dmg,"1d4");
    pstats.s_arm = 10;
    max_stats = pstats;
    pack = NULL;
}

/*
 * Contains defintions and functions for dealing with things like
 * potions and scrolls
 */

struct words rainbow[NCOLORS] = {
    "Red",
    "Blue",
    "Green",
    "Yellow",
    "Black",
    "Brown",
    "Orange",
    "Pink",
    "Purple",
    "Grey",
    "White",
    "Silver",
    "Gold",
    "Violet",
    "Clear",
    "Vermilion",
    "Ecru",
    "Turquoise",
    "Magenta",
    "Amber",
    "Topaz",
    "Plaid",
    "Tan",
    "Tangerine"
};

struct words sylls[NSYLLS] = {
    "a", "ab", "ag", "aks", "ala", "an", "ankh", "app", "arg", "arze",
    "ash", "ban", "bar", "bat", "bek", "bie", "bin", "bit", "bjor",
    "blu", "bot", "bu", "byt", "comp", "con", "cos", "cre", "dalf",
    "dan", "den", "do", "e", "eep", "el", "eng", "er", "ere", "erk",
    "esh", "evs", "fa", "fid", "for", "fri", "fu", "gan", "gar",
    "glen", "gop", "gre", "ha", "he", "hyd", "i", "ing", "ion", "ip",
    "ish", "it", "ite", "iv", "jo", "kho", "kli", "klis", "la", "lech",
    "man", "mar", "me", "mi", "mic", "mik", "mon", "mung", "mur",
    "nej", "nelg", "nep", "ner", "nes", "nes", "nih", "nin", "o", "od",
    "ood", "org", "orn", "ox", "oxy", "pay", "pet", "ple", "plu", "po",
    "pot", "prok", "re", "rea", "rhov", "ri", "ro", "rog", "rok", "rol",
    "sa", "san", "sat", "see", "sef", "seh", "shu", "ski", "sna",
    "sne", "snik", "sno", "so", "sol", "sri", "sta", "sun", "ta",
    "tab", "tem", "ther", "ti", "tox", "trol", "tue", "turs", "u",
    "ulk", "um", "un", "uni", "ur", "val", "viv", "vly", "vom", "wah",
    "wed", "werg", "wex", "whon", "wun", "xo", "y", "yot", "yu",
    "zant", "zap", "zeb", "zim", "zok", "zon", "zum",
};

struct words stones[NSTONES] = {
    "Agate",
    "Alexandrite",
    "Amethyst",
    "Carnelian",
    "Diamond",
    "Emerald",
    "Granite",
    "Jade",
    "Kryptonite",
    "Lapus lazuli",
    "Moonstone",
    "Obsidian",
    "Onyx",
    "Opal",
    "Pearl",
    "Ruby",
    "Saphire",
    "Tiger eye",
    "Topaz",
    "Turquoise",
};

struct words wood[NWOOD] = {
    "Avocado wood",
    "Balsa",
    "Banyan",
    "Birch",
    "Cedar",
    "Cherry",
    "Cinnibar",
    "Driftwood",
    "Ebony",
    "Eucalyptus",
    "Hemlock",
    "Ironwood",
    "Mahogany",
    "Manzanita",
    "Maple",
    "Oak",
    "Persimmon wood",
    "Redwood",
    "Rosewood",
    "Teak",
    "Walnut",
    "Zebra wood",
};

struct words metal[NMETAL] = {
    "Aluminium",
    "Bone",
    "Brass",
    "Bronze",
    "Copper",
    "Iron",
    "Lead",
    "Pewter",
    "Steel",
    "Tin",
    "Zinc",
};

struct magic_item things[NUMTHINGS] = {
    { (char* ) "",			27 },	/* potion */
    { (char* ) "",			27 },	/* scroll */
    { (char* ) "",			18 },	/* food */
    { (char* ) "",			 9 },	/* weapon */
    { (char* ) "",			 9 },	/* armor */
    { (char* ) "",			 5 },	/* ring */
    { (char* ) "",			 5 },	/* stick */
};

struct magic_item s_magic[MAXSCROLLS] = {
    { (char* ) "monster confusion",	 8, 170 },
    { (char* ) "magic mapping",		 5, 180 },
    { (char* ) "light",			10, 100 },
    { (char* ) "hold monster",		 2, 200 },
    { (char* ) "sleep",			 5,  50 },
    { (char* ) "enchant armor",		 8, 130 },
    { (char* ) "identify",		21, 100 },
    { (char* ) "scare monster",		 4, 180 },
    { (char* ) "gold detection",		 4, 110 },
    { (char* ) "teleportation",		 7, 175 },
    { (char* ) "enchant weapon",		10, 150 },
    { (char* ) "create monster",		 5,  75 },
    { (char* ) "remove curse",		 8, 105 },
    { (char* ) "aggravate monsters",	 1,  60 },
    { (char* ) "blank paper",		 1,  50 },
    { (char* ) "genocide",		 1, 200 },
};

struct magic_item p_magic[MAXPOTIONS] = {
    { (char* ) "confusion",		 8,  50 },
    { (char* ) "paralysis",		10,  50 },
    { (char* ) "poison",			 8,  50 },
    { (char* ) "gain strength",		15, 150 },
    { (char* ) "see invisible",		 2, 170 },
    { (char* ) "healing",		15, 130 },
    { (char* ) "monster detection",	 6, 120 },
    { (char* ) "magic detection",	 6, 105 },
    { (char* ) "raise level",		 2, 220 },
    { (char* ) "extra healing",		 5, 180 },
    { (char* ) "haste self",		 4, 200 },
    { (char* ) "restore strength",	14, 120 },
    { (char* ) "blindness",		 4,  50 },
    { (char* ) "thirst quenching",	 1,  50 },
};

struct magic_item r_magic[MAXRINGS] = {
    { (char* ) "protection",		 9, 200 },
    { (char* ) "add strength",		 9, 200 },
    { (char* ) "sustain strength",	 5, 180 },
    { (char* ) "searching",		10, 200 },
    { (char* ) "see invisible",		10, 175 },
    { (char* ) "adornment",		 1, 100 },
    { (char* ) "aggravate monster",	11, 100 },
    { (char* ) "dexterity",		 8, 220 },
    { (char* ) "increase damage",	 8, 220 },
    { (char* ) "regeneration",		 4, 260 },
    { (char* ) "slow digestion",		 9, 240 },
    { (char* ) "telportation",		 9, 100 },
    { (char* ) "stealth",		 7, 100 },
};

struct magic_item ws_magic[MAXSTICKS] = {
    { (char* ) "light",			12, 120 },
    { (char* ) "striking",		 9, 115 },
    { (char* ) "lightning",		 3, 200 },
    { (char* ) "fire",			 3, 200 },
    { (char* ) "cold",			 3, 200 },
    { (char* ) "polymorph",		15, 210 },
    { (char* ) "magic missile",		10, 170 },
    { (char* ) "haste monster",		 9,  50 },
    { (char* ) "slow monster",		11, 220 },
    { (char* ) "drain life",		 9, 210 },
    { (char* ) "nothing",		 1,  70 },
    { (char* )"teleport away",		 5, 140 },
    { (char* )"teleport to",		 5,  60 },
    { (char* ) "cancellation",		 5, 130 },
};

int a_class[MAXARMORS] = {
    8,
    7,
    7,
    6,
    5,
    4,
    4,
    3,
};

char *a_names[MAXARMORS] = {
    (char* ) "leather armor",
    (char* ) "ring mail",
    (char* ) "studded leather armor",
    (char* ) "scale mail",
    (char* ) "chain mail",
    (char* ) "splint mail",
    (char* ) "banded mail",
    (char* ) "plate mail",
};

int a_chances[MAXARMORS] = {
    20,
    35,
    50,
    63,
    75,
    85,
    95,
    100
};

/*
 * init_things
 *	Initialize the probabilities for types of things
 */
void init_things()
{
    struct magic_item *mp;

    for (mp = &things[1]; mp <= &things[NUMTHINGS-1]; mp++)
	mp->mi_prob += (mp-1)->mi_prob;
    badcheck((char* ) "things", things, NUMTHINGS);
}

/*
 * init_colors:
 *	Initialize the potion color scheme for this time
 */

void init_colors()
{
    int i;
    char *str;

    for (i = 0; i < MAXPOTIONS; i++)
    {
	do
	    str = rainbow[rnd(NCOLORS)].w_string;
	until (isupper(*str));
	*str = tolower(*str);
	p_colors[i] = str;
	p_know[i] = FALSE;
	p_guess[i] = NULL;
	if (i > 0)
		p_magic[i].mi_prob += p_magic[i-1].mi_prob;
    }
    badcheck((char* ) "potions", p_magic, MAXPOTIONS);
}

/*
 * init_names:
 *	Generate the names of the various scrolls
 */

void init_names()
{
    int nsyl;
    char *cp, *sp;
    int i, nwords;

    for (i = 0; i < MAXSCROLLS; i++)
    {
	cp = prbuf;
	nwords = rnd(4)+2;
	while(nwords--)
	{
	    nsyl = rnd(3)+1;
	    while(nsyl--)
	    {
		sp = sylls[rnd(NSYLLS)].w_string;
		while(*sp)
		    *cp++ = *sp++;
	    }
	    *cp++ = ' ';
	}
	*--cp = '\0';
	s_names[i] = (char *) new_var(strlen(prbuf)+1);
	s_know[i] = FALSE;
	s_guess[i] = NULL;
	strcpy(s_names[i], prbuf);
	if (i > 0)
		s_magic[i].mi_prob += s_magic[i-1].mi_prob;
    }
    badcheck((char* ) "scrolls", s_magic, MAXSCROLLS);
}

/*
 * init_stones:
 *	Initialize the ring stone setting scheme for this time
 */

void init_stones()
{
    int i;
    char *str;

    for (i = 0; i < MAXRINGS; i++)
    {
	do
	    str = stones[rnd(NSTONES)].w_string;
	until (isupper(*str));
	*str = tolower(*str);
	r_stones[i] = str;
	r_know[i] = FALSE;
	r_guess[i] = NULL;
	if (i > 0)
		r_magic[i].mi_prob += r_magic[i-1].mi_prob;
    }
    badcheck((char* ) "rings", r_magic, MAXRINGS);
}

/*
 * init_materials:
 *	Initialize the construction materials for wands and staffs
 */

void init_materials()
{
    int i;
    char *str;

    for (i = 0; i < MAXSTICKS; i++)
    {
	do
	    if (rnd(100) > 50)
	    {
		str = metal[rnd(NMETAL)].w_string;
		if (isupper(*str))
			ws_type[i] = (char* ) (char* ) "wand";
	    }
	    else
	    {
		str = wood[rnd(NWOOD)].w_string;
		if (isupper(*str))
			ws_type[i] = (char* ) "staff";
	    }
	until (isupper(*str));
	*str = tolower(*str);
	ws_made[i] = str;
	ws_know[i] = FALSE;
	ws_guess[i] = NULL;
	if (i > 0)
		ws_magic[i].mi_prob += ws_magic[i-1].mi_prob;
    }
    badcheck((char* ) "sticks", ws_magic, MAXSTICKS);
}

void badcheck(char *name, struct magic_item *magic, int bound)
{
    struct magic_item *end;

    if (magic[bound - 1].mi_prob == 100)
	return;
    printf("\nBad percentages for %s:\n", name);
    for (end = &magic[bound]; magic < end; magic++)
	printf("%3d%% %s\n", magic->mi_prob, magic->mi_name);
    printf("[hit RETURN to continue]");
    fflush(stdout);
    while (getchar() != '\n')
	continue;
}

struct h_list helpstr[] = {
    '?',	(char* ) "	prints help",
    '/',	(char* ) "	identify object",
    'h',	(char* ) "	left",
    'j',	(char* ) "	down",
    'k',	(char* ) "	up",
    'l',	(char* ) "	right",
    'y',	(char* ) "	up & left",
    'u',	(char* ) "	up & right",
    'b',	(char* ) "	down & left",
    'n',	(char* ) "	down & right",
    'H',	(char* ) "	run left",
    'J',	(char* ) "	run down",
    'K',	(char* ) "	run up",
    'L',	(char* ) "	run right",
    'Y',	(char* ) "	run up & left",
    'U',	(char* ) "	run up & right",
    'B',	(char* ) "	run down & left",
    'N',	(char* ) "	run down & right",
    't',	(char* ) "<dir>	throw something",
    'f',	(char* ) "<dir>	forward until find something",
    'p',	(char* ) "<dir>	zap a wand in a direction",
    'z',	(char* ) "	zap a wand or staff",
    '>',	(char* ) "	go down a staircase",
    's',	(char* ) "	search for trap/secret door",
    ' ',	(char* ) "	(space) rest for a while",
    'i',	(char* ) "	inventory",
    'I',	(char* ) "	inventory single item",
    'q',	(char* ) "	quaff potion",
    'r',	(char* ) "	read paper",
    'e',	(char* ) "	eat food",
    'w',	(char* ) "	wield a weapon",
    'W',	(char* ) "	wear armor",
    'T',	(char* ) "	take armor off",
    'P',	(char* ) "	put on ring",
    'R',	(char* ) "	remove ring",
    'd',	(char* ) "	drop object",
    'c',	(char* ) "	call object",
    'o',	(char* ) "	examine/set options",
    CTRL('L'),	(char* ) "	redraw screen",
    CTRL('R'),	(char* ) "	repeat last message",
    ESCAPE,	(char* ) "	cancel command",
    'v',	(char* ) "	print program version number",
    '!',	(char* ) "	shell escape",
    'S',	(char* ) "	save game",
    'Q',	(char* ) "	quit",
    0, 0
};
