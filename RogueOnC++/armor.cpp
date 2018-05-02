/*
 * This file contains misc functions for dealing with armor
 * @(#)armor.c	3.9 (Berkeley) 6/15/81
 */

#include <iostream>
#include "curses.h"
#include "rogue.hpp"

using std::cout;

void wear();
void waste_time();
void take_off();
bool dropcheck(register struct object *op);
void do_daemons(register int flag);
void do_fuses(register int flag);

/*
 * wear:
 *	The player wants to wear something, so let him/her put it on.
 */

void wear()
{
    register struct linked_list *item;
    register struct object *obj;
// GARRETT CHANGE: as far as I can tell, cout can be used for standard out to replace all of the msg stuff, as this is simple output for a text based game
    if (cur_armor != NULL)
    {
	cout << "You are already wearing some";
	if (!terse)
	    cout << ".  You'll have to take it off first";
	endmsg();
	after = FALSE;
	return;
    }
    if ((item = get_item("wear", ARMOR)) == NULL)
	return;
    obj = (struct object *) ldata(item);
    if (obj->o_type != ARMOR)
    {
	msg("You can't wear that.");
	return;
    }
    waste_time();
    if (!terse)
	addmsg("You are now w");
    else
	addmsg("W");
    msg("earing %s.", a_names[obj->o_which]);
    cur_armor = obj;
    obj->o_flags |= ISKNOW;
}

/*
 * take_off:
 *	Get the armor off of the players back
 */

void take_off()
{
	register struct object *obj;
    if ((obj = cur_armor) == NULL)
    {
	if (terse)
		msg("Not wearing armor");
	else
		msg("You aren't wearing any armor");
	return;
    }
    if (!dropcheck(cur_armor))
	return;
    cur_armor = NULL;
    if (terse)
	addmsg("Was");
    else
	addmsg("You used to be ");
    msg(" wearing %c) %s", pack_char(obj), inv_name(obj, TRUE));
}

/*
 * waste_time:
 *	Do nothing but let other things happen
 */

void waste_time()
{
    do_daemons(BEFORE);
    do_fuses(BEFORE);
    do_daemons(AFTER);
    do_fuses(AFTER);
}
