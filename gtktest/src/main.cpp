/*
 * main.cpp
 *
 *       Created on: 01.09.2015
 *           Author: aleksey slovesnov
 * Copyright(c/c++): 2015-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         Homepage: slovesnov.users.sourceforge.net
 */

//#include "aslov.h"
#include <cmath>
#include <gtk/gtk.h>
#include <string>

#include <windows.h>
//#include <shellapi.h>

cairo_t* cairo;
cairo_surface_t *surface;

PangoFontDescription* m_font;

enum DRAW_TEXT{
	DRAW_TEXT_BEGIN,DRAW_TEXT_CENTER,DRAW_TEXT_END
};

bool selectFont(const char* s, PangoFontDescription*& font) {
	GtkWidget *dialog = gtk_font_chooser_dialog_new(s,
			GTK_WINDOW(0));
	gtk_font_chooser_set_font_desc(GTK_FONT_CHOOSER(dialog), font);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	bool r = result == GTK_RESPONSE_OK || result == GTK_RESPONSE_APPLY;
	if (r) {
		font = gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(dialog));
	}
	gtk_widget_destroy(dialog);
	return r;
}

PangoFontDescription* _createPangoFontDescription(){
	std::string s="Times New Roman, 84";
//	std::string s="Times New Roman, 14";
	PangoFontDescription* d=pango_font_description_from_string(s.c_str());
	//https://www.cairographics.org/FAQ/
	//pango_font_description_set_absolute_size (d, fontHeight * PANGO_SCALE);
	return d;
}

PangoLayout* _createPangoLayout(cairo_t *cr,std::string text){
	PangoLayout *layout = pango_cairo_create_layout(cr);
	PangoFontDescription*desc = _createPangoFontDescription();
	pango_layout_set_font_description(layout, desc);

	pango_layout_set_markup(layout, text.c_str(), -1);
	pango_font_description_free(desc);
	return layout;
}

void _drawMarkup(cairo_t *cr, std::string text, cairo_rectangle_int_t rect,
		DRAW_TEXT optionx, DRAW_TEXT optiony) {
	int w, h;
	PangoLayout *layout = _createPangoLayout(cr,text);
	pango_layout_get_pixel_size(layout, &w, &h);

	double px = rect.x;
	double py = rect.y;
	if (optionx!=DRAW_TEXT_BEGIN) {
		px += (rect.width - w) / (optionx==DRAW_TEXT_END?1:2);
	}
	if (optiony!=DRAW_TEXT_BEGIN) {
		py += (rect.height - h) /  (optiony==DRAW_TEXT_END?1:2);
	}

	cairo_move_to(cr, px, py);
	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);
	g_object_unref(layout);
}


static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

const GdkRGBA BLACK_COLOR = { 0., 0., 0., 1. };

void f();

int main(int argc, char *argv[]) {
	//PairDoubleDouble scale=getScaleFactor();

	gtk_init(&argc, &argv); //do not remove
	//aslovInit(argv);

//	int i;
	std::string s,sb;
	const int areaWidth=800;
	const int areaHeight=600;

	surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, areaWidth, areaHeight);
	cairo=cairo_create(surface);

//	setFont(cairo,"Times New Roman",14, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//	std::string t="Click to change parameters";

	int size=64;
	double x=100;
	double y=100;

#define cr cairo
//	cairo_rectangle(cr, x, y, size, size);
//	cairo_stroke (cr);

//	double width = 60;
//	double height = 4*width/3;
//	double radius = height / 10.0;

	const double triangle=28/64.;//(width of triangle)/size [0..1]
	const double back=26/64.;//(height of arrow back)/size [0..1]
	//cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
	cairo_new_sub_path (cr);
	cairo_move_to(cr, x, y+size/2);
	cairo_rel_line_to(cr, triangle*size, -size/2);
	cairo_rel_line_to(cr, 0, (size-size*back)/2 );
	cairo_rel_line_to(cr, (1-triangle)*size, 0 );
	cairo_rel_line_to(cr, 0, size*back);
	cairo_rel_line_to(cr, -(1-triangle)*size, 0 );
	cairo_rel_line_to(cr, 0, (size-size*back)/2 );
	cairo_close_path (cr);

	cairo_pattern_t *pat1 = cairo_pattern_create_linear(x, y+size/2, x+size, y+size/2);
	cairo_pattern_add_color_stop_rgb(pat1, 0, 1, 1, 1);
	cairo_pattern_add_color_stop_rgb(pat1, 1, 0, 0, 1);
	cairo_set_source(cr, pat1);
	cairo_fill_preserve (cr);
	cairo_pattern_destroy(pat1);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, std::max(1,size/32));
	cairo_stroke (cr);

	cairo_select_font_face(cr, "Times New Roman", CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 4);
//	setFont(cr, "Times New Roman", 22,CAIRO_FONT_SLANT_NORMAL,
//			CAIRO_FONT_WEIGHT_NORMAL);
	cairo_rectangle_int_t rect={100,100,0,0};
	_drawMarkup(cr,"Times New Roman",rect, DRAW_TEXT_BEGIN, DRAW_TEXT_BEGIN);

	s="Times New Roman, 42";
	m_font = pango_font_description_from_string(s.c_str());
	selectFont("title", m_font);
/*

	setFont(cr, "Times New Roman", 22,CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);
	auto a=getMonitorSize(true);
	auto diagonal=getMonitorDiagonal(true);
	auto diagonal1=getMonitorDiagonal(false);
	auto b=getDPI();
	//PairDoubleDouble scale=getScaleFactor();

	s =
			format(
					"monitor size(mm) %.1lf x %.1lf\ndiagonal %.1lf mm %.1lf, inch\ndpi %.1lf x %.1lf\nscale %.2lf x %.2lf",
					a.first, a.second, diagonal, diagonal1, b.first, b.second,scale.first,scale.second);
	drawMarkup(cr, s, 200, 200, DRAW_TEXT_BEGIN, DRAW_TEXT_BEGIN);
	std::ofstream file("a.txt");
	file<<s;

*/


	GtkWidget* window =  gtk_window_new(GTK_WINDOW_TOPLEVEL);
	auto area = gtk_drawing_area_new();
	gtk_widget_set_size_request (area, 800, 600);

	gtk_container_add(GTK_CONTAINER(window), area);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (area), "draw", G_CALLBACK (draw_callback), NULL);

	gtk_widget_show_all(window);

	//printl(ShellExecute(0, 0, "http://www.google.com", 0, 0 , SW_SHOW ));
	//printl(ShellExecute(0, 0, "file:///C:\\slovesno\\a.txt", 0, 0 , SW_SHOW ));
	//printl(ShellExecute(0, 0, "file:///C:/slovesno/a.txt", 0, 0 , SW_SHOW ));
	//printl(ShellExecute(0, 0, "C:/slovesno/a.txt", 0, 0 , SW_SHOW ));
	//printl(ShellExecute(0, 0, "C:\\slovesno\\a.txt", 0, 0 , SW_SHOW ));
	//printl(ShellExecute(0, 0, "C:\\slovesno\\a.html", 0, 0 , SW_SHOW ));

	gtk_main();

//	destroy(cairo);
//	destroy(surface);
}


/*
void getMarkupExtents(cairo_t *cr, std::string const &text, int height, int &sx,
		int &sy) {
	setFont(cr, height);
	PangoLayout *layout = createPangoLayout(cr, text);
	pango_layout_get_pixel_size(layout, &sx, &sy);
	g_object_unref(layout);
}

void getTextExtents(cairo_t *cr, std::string const &text, int height, int &sx,
		int &sy) {
	setFont(cr, height);

	cairo_text_extents_t e;
	cairo_text_extents(cr, text.c_str(), &e);
	sx = e.width;
	sy = e.height;
}

*/
