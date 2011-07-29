#include "menu.h"
#include <cstring>

//menuitem
//(these guys shouldn't actually be called, since
//any instance of menuitem should be of a subclass)
menuitem::~menuitem() { }
bool menuitem::activate() { return false; }
bool menuitem::key_input(int key) { return false; }
void menuitem::key_input_non_active(int key) { }
bool menuitem::shouldMenuBeDrawn() { return false; }
void menuitem::onSelect() { }
void menuitem::onDeselect() { }

//submenuitem
submenuitem::~submenuitem() { }

submenuitem::submenuitem(menu *m, char *name) {
	this->m = m;
	this->name = name;
	isActive = false;
}

bool submenuitem::activate() {
	isActive = true;
	m->set_active(true);
	return true;
}

bool submenuitem::key_input(int key) {
	if(isActive)
		m->key_input(key);
	return (isActive = m->is_active());
}

bool submenuitem::shouldMenuBeDrawn() {return false;}

//actionmenuitem
actionmenuitem::~actionmenuitem() { }

actionmenuitem::actionmenuitem(bool (*init1)(), bool (*ki1)(int), char *name1) {
	init = init1;
	ki = ki1;
	name = name1;
}

bool actionmenuitem::activate() {
	return (*init)();
}

bool actionmenuitem::key_input(int key) {
	return (*ki)(key);
}

bool actionmenuitem::shouldMenuBeDrawn() {return true;}

//inputmenuitem
inputmenuitem::~inputmenuitem() {
	delete [] input;
}

inputmenuitem::inputmenuitem(int maxInputLen, bool (*inputValidator)(char *), char *initInput, char *iie, char *t, char *nam) {
	maxlen = maxInputLen;
	input = new char[maxlen+1];
	if(initInput == NULL) {
		len = 0;
		input[0] = '\0';
	} else {
		strcpy(input, initInput);
		len = strlen(input);
	}
	
	vali = inputValidator;
	invalidInputError = iie;
	displayError = false;

	text = t;
	name = nam;
}

char * inputmenuitem::get_input() {
	if(input == NULL)
		return NULL;
	char *ans = new char[len+1];
	strcpy(ans, input);
	return ans;
}

bool inputmenuitem::activate() {
	displayError = false;
	return true;
}

/* i see we handle characters here -- we need to handle dots for IP addresses */
bool inputmenuitem::key_input(int key) {
	if(key == MENU_KEY_BACKSPACE) {
		if(len > 0)
			input[--len] = '\0';
	}
	else if(key == MENU_KEY_ENTER) {
		if(vali == NULL || (*vali)(input)) 
			return false;
		else
			displayError = true;
	}
	else if(key >= MENU_KEY_A && key <= MENU_KEY_A + 25) {
		if(len < maxlen) {
			input[len++] = 'a' + key - MENU_KEY_A;
			input[len] = '\0';
		}
	}
	else if(key >= MENU_KEY_0 && key <= MENU_KEY_0 + 9) {
		if(len < maxlen) {
			input[len++] = '0' + key - MENU_KEY_0;
			input[len] = '\0';
		}
	}
	return true;
}

bool inputmenuitem::shouldMenuBeDrawn() {
	return true;
}

//togglemenuitem
togglemenuitem::~togglemenuitem() { }

togglemenuitem::togglemenuitem(char *name1, bool state1, void (*act)(bool)) {
	name = name1;
	state = state1;
	action = act;
}

bool togglemenuitem::activate() {
	state = !state;
	if(action != NULL)
		action(state);
	return false;
}

bool togglemenuitem::get_state() {
	return state;
}

//slidermenuitem
slidermenuitem::~slidermenuitem() { }

slidermenuitem::slidermenuitem(char *name1, char** states1, int len1, int curstate1, void (*act)(int)) {
       name = name1;
       states = states1;
       len = len1;
       curstate = curstate1;
       newcurstate = curstate1;
       action = act;
}

bool slidermenuitem::get_state() {
	return curstate;
}

void slidermenuitem::key_input_non_active(int key) {
	if(key == MENU_KEY_LEFT) {
		if(newcurstate > 0) {
			newcurstate--;
			//if(action != NULL) action(curstate); //now somewhere else
		}
	}
	else if(key == MENU_KEY_RIGHT) {
		if(newcurstate < len - 1) {
			newcurstate++;
			//if(action != NULL) action(curstate);
		}
	}
}

bool slidermenuitem::activate() {
	curstate = newcurstate;
	if(action != NULL)
		action(curstate);
	return false;
}

void slidermenuitem::onDeselect() {
	newcurstate = curstate;
}
