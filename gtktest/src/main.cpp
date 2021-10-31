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
//#include "TextWithAttributes.h"
//#include <glib/gstdio.h>

//450
const int areaWidth=1000;
const int areaHeight=3*areaWidth/4;


const GdkRGBA BLACK_COLOR = { 0., 0., 0., 1. };

void draw(GtkWidget *widget, cairo_t *cr){
//	const GdkRGBA bg = {0xF6/255. , 0xF4/255., 0xBA/255.,1};
//	const GdkRGBA linecolor = {0 , 0, 0,1};

	int i;
	double x,y;
	std::string s;
	cairo_text_extents_t extents;

	const char *string = "j6()J";
//	const char *utf8 = "66.()";

	const int fontSize=40;

	setFont(cr,"Times New Roman",fontSize, CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);

	s=string+std::string("\n<i>abc123</i>");

	const int wi=300;
	cairo_rectangle_int_t rect={0,0,wi,200};

	for(i=0;i<9;i++){
		rect.x = 30 + (wi+30) * (i%3);
		cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
		cairo_set_line_width (cr, 6.0);
		cairo_arc (cr, rect.x, rect.y, 10.0, 0, 2*G_PI);
		cairo_fill (cr);

		cairo_rectangle(cr,rect.x,rect.y,rect.width,rect.height);
		cairo_stroke (cr);

		gdk_cairo_set_source_rgba(cr, &BLACK_COLOR);
		drawMarkup(cr, s, rect,
				DRAW_TEXT(i%3), DRAW_TEXT(i/3));
		if((i+1)%3==0){
			rect.y+=220;
		}
	}

//	x=25;
//	y=150;
	x=700;
	y=700;

	cairo_text_extents (cr, string, &extents);
	cairo_move_to (cr, x,y);
	cairo_show_text (cr, string);

	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);
	cairo_arc (cr, x, y, 10.0, 0, 2*G_PI);
	cairo_fill (cr);
	cairo_move_to (cr, x,y);
	cairo_rel_line_to (cr, 0, extents.y_bearing);
	cairo_rel_line_to (cr, extents.width, 0);
	cairo_rel_line_to (cr, extents.x_bearing, extents.height);
	cairo_rel_line_to (cr, -extents.width, 0);
	cairo_stroke (cr);
/*


	const int len=150;
	DRAW_TEXT d[]={DRAW_TEXT_BEGIN,DRAW_TEXT_CENTER,DRAW_TEXT_END};
	for(i=0;i<3;i++){
		x = 30 + 350 * i;
		y = 100;

		cairo_set_source_rgba(cr, 1, 0.2, 0.2, 0.6);
		cairo_set_line_width(cr, 6.0);
		cairo_arc(cr, x, y, 10.0, 0, 2 * G_PI);
		cairo_fill(cr);
		cairo_move_to(cr, x - len, y);
		cairo_rel_line_to(cr, 2 * len, 0);
		cairo_move_to(cr, x, y - len);
		cairo_rel_line_to(cr, 0, 2 * len);
		cairo_stroke(cr);

		gdk_cairo_set_source_rgba(cr, &BLACK_COLOR);
		drawText(cr, string, x, y, d[i],DRAW_TEXT_CENTER);
	}
*/

}

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	draw(widget,cr);
	return FALSE;
}


int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv); //do not remove
	aslovInit(argv);

	auto window =  gtk_window_new(GTK_WINDOW_TOPLEVEL);;
	auto area = gtk_drawing_area_new();
	gtk_widget_set_size_request (area, areaWidth, areaHeight);

	gtk_container_add(GTK_CONTAINER(window), area);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (area), "draw", G_CALLBACK (draw_callback), NULL);

	gtk_widget_show_all(window);

	gtk_main();
}
