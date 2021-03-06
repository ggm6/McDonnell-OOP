/*
 * Functions for dealing with problems brought about by weapons
 *
 * @(#)weapons.c	3.17 (Berkeley) 6/15/81
 */

#include "curses.h"
#include <ctype.h>
#include "rogue.hpp"

#define NONE 100

char *w_names[MAXWEAPONS] = {
    (char* ) "mace",
    (char* ) "long sword",
    (char* ) "short bow",
    (char* ) "arrow",
    (char* ) "dagger",
    (char* ) "rock",
    (char* ) "two handed sword",
    (char* ) "sling",
    (char* ) "dart",
    (char* ) "crossbow",
    (char* ) "crossbow bolt",
    (char* ) "spear",
};

static struct init_weps {
    char *iw_dam;
    char *iw_hrl;
    char iw_launch;
    int iw_flags;
} init_dam[MAXWEAPONS] = {
    (char* ) "2d4", (char* ) "1d3", NONE, 0,		/* Mace */
    (char* ) "1d10", (char* ) "1d2", NONE,0,		/* Long sword */
    (char* ) "1d1", (char* ) "1d1", NONE,	0,		/* Bow */
    (char* ) "1d1", (char* ) "1d6", BOW,	ISMANY|ISMISL,	/* Arrow */
    (char* ) "1d6", (char* ) "1d4", NONE,	ISMISL,		/* Dagger */
    (char* ) "1d2", (char* ) "1d4", SLING,ISMANY|ISMISL,	/* Rock */
    (char* ) "3d6", (char* ) "1d2", NONE,	0,		/* 2h sword */
    (char* ) "0d0", (char* ) "0d0", NONE, 0,		/* Sling */
    (char* ) "1d1", (char* ) "1d3", NONE,	ISMANY|ISMISL,	/* Dart */
    (char* ) "1d1", (char* ) "1d1", NONE, 0,		/* Crossbow */
    (char* ) "1d2", (char* ) "1d10", CROSSBOW, ISMANY|ISMISL,/* Crossbow bolt */
    (char* ) "1d8", (char* ) "1d6", NONE, ISMISL,		/* Spear */
};

/*
 * missile:
 *	Fire a missile in a given direction
 */

void missile(int ydelta, int xdelta)
{
    struct object *obj;
    struct linked_list *item, *nitem;

    /*
     * Get which thing we are hurling
     */
    if ((item = get_item("throw", WEAPON)) == NULL)
	return;
    obj = (struct object *) ldata(item);
    if (!dropcheck(obj) || is_current(obj))
	return;
    /*
     * Get rid of the thing.  If it is a non-multiple item object, or
     * if it is the last thing, just drop it.  Otherwise, create a new
     * item with a count of one.
     */
    if (obj->o_count < 2)
    {
	detach(pack, item);
	inpack--;
    }
    else
    {
	obj->o_count--;
	if (obj->o_group == 0)
	    inpack--;
	nitem = (struct linked_list *) new_item(sizeof *obj);
	obj = (struct object *) ldata(nitem);
	*obj = *((struct object *) ldata(item));
	obj->o_count = 1;
	item = nitem;
    }
    do_motion(obj, ydelta, xdelta);
    /*
     * AHA! Here it has hit something.  If it is a wall or a door,
     * or if it misses (combat) the mosnter, put it on the floor
     */
    if (!isupper(mvwinch(mw, obj->o_pos.y, obj->o_pos.x))
	|| !hit_monster(unc(obj->o_pos), obj))
	    fall(item, TRUE);
    mvwaddch(cw, hero.y, hero.x, PLAYER);
}

/*
 * do the actual motion on the screen done by an object traveling
 * across the room
 */
void do_motion(struct object *obj, int ydelta, int xdelta)
{
    /*
     * Come fly with us ...
     */
    obj->o_pos = hero;
    for (;;)
    {
	int ch;

	/*
	 * Erase the old one
	 */
	if (!ce(obj->o_pos, hero) && cansee(unc(obj->o_pos)) &&
	    mvwinch(cw, obj->o_pos.y, obj->o_pos.x) != ' ')
		    mvwaddch(cw, obj->o_pos.y, obj->o_pos.x,
			    show(obj->o_pos.y, obj->o_pos.x));
	/*
	 * Get the new position
	 */
	obj->o_pos.y += ydelta;
	obj->o_pos.x += xdelta;
	if (step_ok(ch = winat(obj->o_pos.y, obj->o_pos.x)) && ch != DOOR)
	{
	    /*
	     * It hasn't hit anything yet, so display it
	     * If it alright.
	     */
	    if (cansee(unc(obj->o_pos)) &&
		mvwinch(cw, obj->o_pos.y, obj->o_pos.x) != ' ')
	    {
		mvwaddch(cw, obj->o_pos.y, obj->o_pos.x, obj->o_type);
		draw(cw);
	    }
	    continue;
	}
	break;
    }
}

/*
 * fall:
 *	Drop an item someplace around here.
 */

void fall(struct linked_list *item, bool pr)
{
    struct object *obj;
    struct room *rp;
    static coord fpos;

    obj = (struct object *) ldata(item);
    if (fallpos(&obj->o_pos, &fpos, TRUE))
    {
	mvaddch(fpos.y, fpos.x, obj->o_type);
	obj->o_pos = fpos;
	if ((rp = roomin(&hero)) != NULL && !(rp->r_flags & ISDARK))
	{
	    light(&hero);
	    mvwaddch(cw, hero.y, hero.x, PLAYER);
	}
	attach(lvl_obj, item);
	return;
    }
    if (pr)
	msg("Your %s vanishes as it hits the ground.", w_names[obj->o_which]);
    discard(item);
}

/*
 * init_weapon:
 *	Set up the initial goodies for a weapon
 */

void init_weapon(struct object *weap, char type)
{
    struct init_weps *iwp;
	for (unsigned int it=0; it!=MAXWEAPONS; ++it)
	{  // CHANGE from original code, because iwp was using "type" as an array subscript to find the correct element in init_dam
		if (init_dam[it].iw_launch==type)
		{
			iwp=&init_dam[it];
			it=MAXWEAPONS;
		}
	}
    strcpy(weap->o_damage,iwp->iw_dam);
    strcpy(weap->o_hurldmg,iwp->iw_hrl);
    weap->o_launch = iwp->iw_launch;
    weap->o_flags = iwp->iw_flags;
    if (weap->o_flags & ISMANY)
    {
	weap->o_count = rnd(8) + 8;
	weap->o_group = newgrp();
    }
    else
	weap->o_count = 1;
}

/*
 * Does the missile hit the monster
 */

bool hit_monster(int y, int x, struct object *obj)
{
    static coord mp;

    mp.y = y;
    mp.x = x;
    return fight(&mp, winat(y, x), obj, TRUE);
}

/*
 * num:
 *	Figure out the plus number for armor/weapons
 */

char * num(int n1, int n2)
{
    static char numbuf[80];

    if (n1 == 0 && n2 == 0)
	return (char* ) "+0";
    if (n2 == 0)
	sprintf(numbuf, "%s%d", n1 < 0 ? "" : "+", n1);
    else
        sprintf(numbuf, "%s%d,%s%d",
				n1 < 0 ? "" : "+", n1, n2 < 0 ? "" : "+", n2);
    return numbuf;
}    

/*
 * wield:
 *	Pull out a certain weapon
 */

void wield()
{
    struct linked_list *item;
    struct object *obj, *oweapon;

    oweapon = cur_weapon;
    if (!dropcheck(cur_weapon))
    {
	cur_weapon = oweapon;
	return;
    }
    cur_weapon = oweapon;
    if ((item = get_item("wield", WEAPON)) == NULL)
    {
bad:
	after = FALSE;
	return;
    }

    obj = (struct object *) ldata(item);
    if (obj->o_type == ARMOR)
    {
	msg("You can't wield armor");
	goto bad;
    }
    if (is_current(obj))
        goto bad;

    if (terse)
	addmsg("W");
    else
	addmsg("You are now w");
    msg("ielding %s", inv_name(obj, TRUE));
    cur_weapon = obj;
}

/*
 * pick a random position around the give (y, x) coordinates
 */
bool fallpos(coord *pos, coord *newpos, bool passages)
{
    int y, x, cnt, ch;

    cnt = 0;
    for (y = pos->y - 1; y <= pos->y + 1; y++)
	for (x = pos->x - 1; x <= pos->x + 1; x++)
	{
	    /*
	     * check to make certain the spot is empty, if it is,
	     * put the object there, set it in the level list
	     * and re-draw the room if he can see it
	     */
	    if (y == hero.y && x == hero.x)
		continue;
	    if (((ch = winat(y, x)) == FLOOR || (passages && ch == PASSAGE))
					&& rnd(++cnt) == 0)
	    {
		newpos->y = y;
		newpos->x = x;
	    }
	}
    return (cnt != 0);
}
