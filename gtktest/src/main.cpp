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

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	double x, y;
	const char string[] = "j6()J";
	std::string s = string + std::string("\n<i>abc123</i>");
	const int fontSize = 40;
	const GdkRGBA BLACK_COLOR = { 0., 0., 0., 1. };
	const GdkRGBA LINE_COLOR = { 1, 0.2, 0.2, 0.6 };

	//set for both drawMarkup & drawText
	setFont(cr, "Times New Roman", fontSize, CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);

	cairo_rectangle_int_t rect = { 30, 30, 200, 200 };
	x = rect.x;
	y = rect.y;
	gdk_cairo_set_source_rgba(cr, &LINE_COLOR);
	cairo_set_line_width(cr, 6.0);
	cairo_arc(cr, x, y, 10.0, 0, 2 * G_PI);
	cairo_fill(cr);
	cairo_rectangle(cr, x, y, rect.width, rect.height);
	cairo_stroke(cr);

	gdk_cairo_set_source_rgba(cr, &BLACK_COLOR);
	drawMarkup(cr, s, rect, DRAW_TEXT_BEGIN, DRAW_TEXT_CENTER);

	const int len = 100;
	x = 350;
	y = rect.y+rect.height/2;
	gdk_cairo_set_source_rgba(cr, &LINE_COLOR);
	cairo_set_line_width(cr, 6.0);
	cairo_arc(cr, x, y, 10.0, 0, 2 * G_PI);
	cairo_fill(cr);
	cairo_move_to(cr, x - len, y);
	cairo_rel_line_to(cr, 2 * len, 0);
	cairo_move_to(cr, x, y - len);
	cairo_rel_line_to(cr, 0, 2 * len);
	cairo_stroke(cr);

	gdk_cairo_set_source_rgba(cr, &BLACK_COLOR);
	drawText(cr, string, x, y, DRAW_TEXT_CENTER, DRAW_TEXT_CENTER);

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
