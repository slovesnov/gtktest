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
#include <fstream>

const bool firstdeck=false;
const bool storefile=1;

using VGdkPixbufPtr = std::vector<GdkPixbuf*>;

cairo_t* cairo;
cairo_surface_t *surface;
std::vector<std::pair<int,int>> points[2];

//alpha - high byte
uint32_t black=0xff000000;
uint32_t transparent=0;//transparent

VGdkPixbufPtr vp;
GtkWidget* area;
const int c[]={6, 20, 101, 183-38};
const int dx=123-8;
const int stepy=15;
const int width = c[2] - c[0];
const int height = c[3] - c[1];
const int marginx=10;
const int marginy=10;
const int areaWidth=width*13+2*marginx;
const int areaHeight=height*4+2*marginy;

void copyCards();

void printHelp(){
	int i,j;
	const char SUITS_CHAR[] = "shdcn";
	const char RANK[] = "akqjt98765432";
	for(i=0;i<4;i++){
		for(j=0;j<13;j++){
			g_print("/*%c%c*/,, ",RANK[12-j],SUITS_CHAR[i]);
		}
		printan("");
	}
}

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv); //do not remove
	aslovInit(argv);

	int i,j;
	GdkPixbuf*p;
	std::string s,add;
	if (!firstdeck) {
		add = "a";
	}
	for(i=1;;i++){
		s="decks/"+add+std::to_string(i)+".png";
		std::ifstream f(s);
		if(!f.good()){
			break;
		}
		p = gdk_pixbuf_new_from_file(s.c_str(), NULL);
		vp.push_back( gdk_pixbuf_new_subpixbuf(p, 25, 185, 1140, 150));
		g_object_unref(p);
	}
	//printl(vp.size())

	const int N = firstdeck ? 2 : 5;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N-i; j++) {
			points[0].push_back( { i, j });
		}
	}

	for (i = 1; i < N; i++) {
		points[1].push_back( { i, N-i });
	}

	surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, areaWidth, areaHeight);
	cairo=cairo_create(surface);

	copyCards();


	if (storefile) {
		GdkPixbuf *pb = gdk_pixbuf_get_from_surface(surface, marginx, marginy,
				width * 13, height * 4);
		copy(pb, cairo, marginx, marginy, width * 13, height * 4, 0, 0);
		s = "deck" + std::to_string(6 + !firstdeck) + ".png";
		gdk_pixbuf_save(pb, s.c_str(), "png", NULL, NULL);
		g_object_unref(pb);
	}

	GtkWidget* window =  gtk_window_new(GTK_WINDOW_TOPLEVEL);
	area = gtk_drawing_area_new();
	gtk_widget_set_size_request (area, areaWidth, areaHeight);

	gtk_container_add(GTK_CONTAINER(window), area);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (area), "draw", G_CALLBACK (draw_callback), NULL);

	gtk_widget_show_all(window);
//	printHelp();
	gtk_main();

	destroy(cairo);
	destroy(surface);
}

void copyCards() {
	const int deckdata[][52*2]={{
		//2  3     4     5    6    7      8   9      10   j     q    k    a
		1,8, 9,7, 1,10, 1,1, 1,6, 10,10, 8,3, 13,6, 14,4, 12,6, 5,7, 3,3, 11,6,
		2,1, 8,1, 13,10, 11,4, 7,6, 13,3, 4,4, 6,2,  5,1, 13,5, 6,7, 6,6, 6,8,
		11,8, 10,3, 11,3,10,1,12,7, 1,4, 1,9,  1,2, 10,5, 10,7, 12,3,   12,2,     8,9,
		11,7, 1,5, 9,6,  1,3, 5,10,3,2, 10,8,  5,5, 7,1, 2,2,  14,3,   3,9,     14,1
		},{
		/*2s*/14,1, /*3s*/2,1, /*4s*/2,8, /*5s*/1,5, /*6s*/13,6, /*7s*/1,9, /*8s*/3,3, /*9s*/3,6, /*ts*/3,1, /*js*/1,4, /*qs*/2,9, /*ks*/8,4, /*as*/6,4,
		/*2h*/2,5, /*3h*/5,6, /*4h*/3,5, /*5h*/10,10, /*6h*/5,7, /*7h*/9,4, /*8h*/10,3, /*9h*/4,4, /*th*/9,7, /*jh*/6,10, /*qh*/10,9, /*kh*/14,8, /*ah*/8,10,
		/*2d*/2,4, /*3d*/2,7, /*4d*/6,7, /*5d*/6,1, /*6d*/3,4, /*7d*/2,6, /*8d*/8,1, /*9d*/10,1, /*td*/1,7, /*jd*/1,1, /*qd*/5,8, /*kd*/1,6, /*ad*/15,6,
		/*2c*/10,2, /*3c*/1,8, /*4c*/4,9, /*5c*/4,10, /*6c*/12,4, /*7c*/8,8, /*8c*/1,3, /*9c*/1,2, /*tc*/2,3, /*jc*/9,8, /*qc*/1,10, /*kc*/3,7, /*ac*/11,8,
		}
	};

	//need to create new image with Alpha support
	GdkPixbuf *pixbuf=gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8,
			width, height);
	int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

	auto setPixel = [&](int x, int y, uint32_t color) {
		guchar *p = pixels + (y) * rowstride + (x) * n_channels;
		uint32_t *p1 = (uint32_t*) p;
		*p1 = color;
	};

	int destx, desty, sourcex, sourcey;
	int i, j;
	int index = 0;

	auto a=deckdata[!firstdeck];
	for (int ii = 0; ii < 52*2; ii += 2, index++) {
		int picture = a[ii];
		int card = a[ii + 1] - 1;
		destx = marginx + width * (index % 13);
		desty = marginy + height * (index / 13);
		sourcex = c[0] + dx * card;
		sourcey = c[1];
		if (card > 3) {
			sourcey -= stepy;
		}
		if (picture - 1 >= int(vp.size())) {
			printl("error")
			;
			exit(1);
		}
		assert(picture<vp.size()+1);

		gdk_pixbuf_copy_area(vp[picture - 1], sourcex, sourcey, width, height, pixbuf, 0, 0);

		//black frame
		for (i = 0; i < width; i++) {
			setPixel(i, 0, black);
			setPixel(i, height - 1, black);
		}

		for (i = 0; i < height; i++) {
			setPixel(0, i, black);
			setPixel(width - 1, i, black);
		}

		i = 0;
		for (auto &a : points) {
			auto c = i == 0 ? transparent : black;
			for (auto&& [x,y] : a) {
				for (j = 0; j < 4; j++) {
					setPixel(j % 2 ? x : width - 1 - x, j / 2 ? y : height - 1 - y, c);
				}
			}
			i++;
		}

		copy(pixbuf, cairo, destx, desty, width, height, 0, 0);
	}
}
