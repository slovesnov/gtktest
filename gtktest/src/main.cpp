/*
 * main.cpp
 *
 *       Created on: 01.09.2015
 *           Author: aleksey slovesnov
 * Copyright(c/c++): 2015-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         Homepage: slovesnov.users.sourceforge.net
 */

#include "aslov.h"
#include <cmath>
//#include <gtk/gtk.h>
//#include <string>
//
//#include <windows.h>
//#include <shellapi.h>

#include <filesystem>
using namespace std::filesystem;

int main(int argc, char *argv[]) {
	//PairDoubleDouble scale=getScaleFactor();

	gtk_init(&argc, &argv); //do not remove
	aslovInit(argv);

	std::string path="c:\\slovesno\\site\\img\\bridge";
	std::string s,e,q,s1;
	int w,nw;
	Pixbuf p;

//	e="1.png";
//	printl(endsWith(e, "en.png"));
//	return 0;

	for (auto& pa : recursive_directory_iterator(path)) {
		s = pa.path().string();
		p.set(s);
		w=p.width();
		nw=int(round(w*.8));//better than just int(w*.8)
		e = getFileInfo(s,FILEINFO::NAME);
//		if(w<600){
		if(!startsWith(e, "en") ){
			continue;
		}
		if(nw>800){
			 s1=format("style=\"position:relative;left:%dpx;\"",(793-nw)/2);
		}
		else{
			s1="";
		}
		q=format("<img %ssrc=\"img/bridge/%s\" width=\"%d\">" ,s1.c_str(),e.c_str(),nw);
		//printl(e,w)
		printzn(q);
		//<img src="img/bridge/barbakaru3ru.png" width="614">
	}
}
