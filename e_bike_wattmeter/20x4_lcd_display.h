#pragma once

#define LINECOUNT 4
#define COLUMNCOUNT 20

#include "lib/LiquidCrystal_I2C.h"
/*

16x4 LCD Display Interface

Param - Val		Param - Val
Param - Val		Param - Val
Param - Val		Param - Val
Param - Val		Param - Val

[P][P][P][][V][V][V][V][][][P][P][P][][V][V][V][V]
[P][P][P][][V][V][V][V][][][P][P][P][][V][V][V][V]
[P][P][P][][V][V][V][V][][][P][P][P][][V][V][V][V]
[P][P][P][][V][V][V][V][][][P][P][P][][V][V][V][V]

*/



/*
	This function works on nearly the lowerest layer, functions like showPage should call this
	We need to hold all values in an array, so that we can check if a value has changed
	The best solution would be to
		1. build a char array for a whole line
		2. check for each char if it needs to be updated and do so if neccessary
	In order to do so we need a [4][16] array to hold all the chars
*/

/*
	compare the given strings with the chars in allchars[][]
	if single chars differ, update them in the array and refresh the position in the display
	return true if value changed, else false
	int col, column 1 or 2
	int line lines 1-4
	char* param, 2 chars
	char* value, 4 chars
*/
void lcdSetValue(int col, int line, char* param, char* value);


/*
	return the value at the position as char*, so that can be interpreted by the caller
	int col ,column 1 or 2
	int line, lines 1-4
*/
char* lcdGetValue(int col, int line);

/*
	return the parameter string at the position
	int col ,column 1 or 2
	int line, lines 1-4
*/
char* lcdGetParam(int col, int line);


/*
	The above functions make it possible to show some values on the display,
	but additionally we want to have multible pages to show even more values.
	This requires a page class. The page should have 8 lcdValues.
	A lcdValue consists of the parameter name, that is shown in 2 chars and a getValue function pointer,
	which returns the value as float.
*/

class lcdValue {
public:
	char* param;
	measurement *mp;
	char* getWholeString(char string[]);
private:
};

/*
* returns the value as printable string on lcd display (7 chars length)
*/
char* lcdValue::getWholeString(char string[]) {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	dprintf(lvl_info, "lcdValue has param: " + String(param) + " .\n");
	//char string[8];
#ifdef _debug
	//if everything works we shouldn't see a 'd'
	for (int i = 0; i < (COLUMNCOUNT / 2 - 1); i++)
		string[i] = 'd';
	string[(COLUMNCOUNT / 2)-1] = '\0';
#endif
	//temp = &string;
	string[0] = param[0];
	string[1] = param[1];
	string[2] = param[2];
	string[3] = ' ';

	float val = mp->getValue();
	while (val > 999) {
		dprintf(lvl_err, String(__func__) + "unhandled overflow, value too large!!");
		val = val / 10;
	}
	char result[10]; 
	dtostrf(val, 5, 2, result);
	for (int i = 0; i < 5; i++)
		string[i+4] = result[i];
	dprintf(lvl_verbose, String(__func__) + " build up string "+String(string)+" \n");
	
	dprintf(lvl_trace, String(__func__) + " leave\n");
	return NULL;
}
/*

Arrangement in the LCD display
lcdval[0] - lcdval[1]
lcdval[2] - lcdval[3]
lcdval[4] - lcdval[5]
lcdval[6] - lcdval[7]
*/
class page {
private:
public:
	char* getWholeLine(int line, char strLine[]);

	lcdValue *lcdval[8];
	/*
		This function should initialize all lcdValues with name and function pointer.
	*/
	void init();
	/*
		This function should go through all lcdValues and call lcdSetValue() for each one
		lcdSetValue() will determine if the display needs to be updated or not.
	*/
	void refresh();

	//we should hold pointers to the next and previous page, optimally in a circle (last element points to the first and the other way round)
	page* prev;
	page* next;
};


// page functions

char* page::getWholeLine(int line, char strLine[]) {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	dprintf(lvl_verbose, "get val" +String(line*2)+" and val" + String(line*2+1) + "\n")
	lcdValue *val1 = lcdval[line * 2];
	lcdValue *val2 = lcdval[line * 2 + 1];
	char strVal1[(COLUMNCOUNT / 2)-1];
	char strVal2[(COLUMNCOUNT / 2)-1];

	//char strLine[COLUMNCOUNT];

#ifdef _debug
	//if everything works, all the A's should be overwritten
	for (int i = 0; i < COLUMNCOUNT; i++)
		strLine[i] = 'A';
#endif
	

	if (val1) {
		val1->getWholeString(strVal1);
		dprintf(lvl_verbose, ": got strVal1: " + String(strVal1) + "\n");
	}
	if (val2) {
		val2->getWholeString(strVal2);
		dprintf(lvl_verbose, " got strVal2 " + String(strVal2) + "\n");		
	}

	
	for (int i = 0; i < (COLUMNCOUNT/2)-1; i++) {
		if (val1)
			strLine[i] = strVal1[i];
		else
			strLine[i] = 'a';
	}
	strLine[(COLUMNCOUNT / 2)-1] = 'b';
	strLine[(COLUMNCOUNT / 2)] = 'b';
	for (int i = 0; i < (COLUMNCOUNT / 2)-1; i++) {
		if (val2)
			strLine[i + (COLUMNCOUNT / 2) + 1] = strVal2[i]; //believe me  it is the right offset
		else
			strLine[i + (COLUMNCOUNT / 2) + 1] = 'c';
	}
	strLine[COLUMNCOUNT] = '\0';

	dprintf(lvl_verbose, "build line " + String(strLine) + " \n");
	dprintf(lvl_trace, String(__func__) + " leave\n");
	return NULL;
}

/*
	On the highest level we have the LCD display with several pages, menues and so on
*/

class lcdDisplay {
private:
	LiquidCrystal_I2C *lcd;
	void writeChar(char c, int line, int column) {
		lcd->setCursor((uint8_t)column, (uint8_t)line);
		lcd->print(c);
	};
public:
	char allchars[LINECOUNT][COLUMNCOUNT]; //in memory representation of the chars shown on the lcd display
	page *pages;
	page *curPage;
	void init();
	int insertPage(page *pPage);
	int deletePage(page *pPage);
	int showPage(page *pPage);
	int switchNextPage(page *pPage) {
		if (!curPage->next) return -1;
		curPage = curPage->next;
		return 0;
	};

	int switchPrevPage(page *pPage) {
		if (!curPage->prev) return -1;
		curPage = curPage->next;
		return 0;
	};
	int refreshCurrentPage();
	bool pageListEmpty() {
		return (pages == pages->next && pages == pages->prev);
	};
};

// lcd display functions

void lcdDisplay::init() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	lcd = new LiquidCrystal_I2C(0x27, COLUMNCOUNT, LINECOUNT);
	lcd->init();
	lcd->backlight();
	pages = new page();
	pages->next = pages;
	pages->prev = pages;
	for (int i = 0; i < LINECOUNT; i++) {
		for (int a = 0; a < COLUMNCOUNT; a++) {
			allchars[i][a] = 'x';
		}
	}
	dprintf(lvl_trace, String(__func__) + " leave\n");
}

int lcdDisplay::insertPage(page *pPage) {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	/*
		insert a page after the last page in the list
				->
		page1	<-	page 2

				->			->
		page1	<-	page 3	<- page 2
	*/
	page *lastPage;
	if (pages && pages->prev)
		lastPage = pages->prev;

	lastPage->next = pPage;
	pPage->prev = lastPage;

	pages->prev = pPage;
	pPage->next = pages;
	dprintf(lvl_trace, String(__func__) + " leave\n");
	return 0;
}

/*
	deletes the given page from the list
	return values:
		0: success
		1: list not empty
		2: element not found
*/
int lcdDisplay::deletePage(page *pPage) {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	short ret = 0;
	if (!pageListEmpty()) {
		page *curPage;
		dprintf(lvl_info, String(__func__) + " start searching for page");
		do {
			curPage = pages->next;
			if (curPage == pPage) { //we found the page to delete, need to modify the pointers
				dprintf(lvl_info, String(__func__) + " found page to delete, do so now\n");
				page *prev = curPage->prev;
				page *next = curPage->next;
				prev->next = next;
				next->prev = prev;
				delete curPage;
				ret = 0;
				goto end;
			}
		} while (curPage != pages);
		dprintf(lvl_err, String(__func__) + " the given page was not found\n");
		ret = 2;
		goto end;
	}
	else {
		dprintf(lvl_err, String(__func__) + " cannot delete anything, the list is empty!\n");
		ret = 1;
		goto end;
	}
end:
	dprintf(lvl_trace, String(__func__) + " leave\n");
	return ret;
}

/*
	return:
		0: success
		1: invalid page
*/
int lcdDisplay::showPage(page *pPage) {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	short ret = 0;
	if (pPage == pages) {
		dprintf(lvl_err, String(__func__) + "not a valid page!\n");
		ret = 1;
		goto end;
	}

	curPage = pPage;
	refreshCurrentPage();
end:
	dprintf(lvl_trace, String(__func__) + " leave\n");
	return ret;
}

int lcdDisplay::refreshCurrentPage() {
	dprintf(lvl_trace, String(__func__) + " enter\n");
	for (int i = 0;
		i < LINECOUNT;
		i++) {
		char line[COLUMNCOUNT+1];
		curPage->getWholeLine(i, line);
		dprintf(lvl_verbose, String(__func__) + ": got line " + String(line) +"\n");
		for (int a = 0;
			a < COLUMNCOUNT;
			a++) {
			if (line[a] != allchars[i][a]) {
				dprintf(lvl_info, "found non equal char, replace " + String(allchars[i][a]) + " with " + String(line[a]) + " on " + String(i) + "," + String(a) + "\n");
				allchars[i][a] = line[a];
				writeChar(line[a], i, a);
			}
		}
	}
	dprintf(lvl_trace, String(__func__) + " leave\n");
}








