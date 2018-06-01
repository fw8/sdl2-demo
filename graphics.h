#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Beim Umgang mit vorliegendem Dokument sollte folgende Programmstruktur eingehalten werden.
 */
#if 0
int main(int argc, char** argv) {
	/* Breite des Fensters */
	int width  = 1000;
	/* Hoehe des Fensters */
	int height = 1000;
	
	/* Erzeugung der Graphikumgebung */
	g_create(width, height, "title");
	
	/* Laden einer Schriftart des "ttf-Formats" aus der Datei
	 * "VeraMono.ttf" im Verzeichnis "ttf".
	 * Die Schriftgroesse ist 32
	 */
	g_load_font("ttf/VeraMono.ttf", 32);
	
	/* Solange das Fenster nicht geschlossen wurde
	 * (Fenster schliessen durch
	 *    - x-Symbol anklicken,
	 *    - ESC oder
	 *    - ALT + F4
	 * )
	 * Die Funktion g_has_quit hat den Seiteneffekt, dass sie die Maus- und Tastaturereignisse verarbeitet.
	 * Daher MUSS sie unbedingt periodisch aufgerufen werden, selbst wenn das Ergebnis nicht verwendet wird.
	 */
	while(!g_has_quit()) {
		/* Loeschen des Fensterinhalts */
		g_clear();
		
		/* Fensterinhalt zeichnen */
			/* Bildpunkt in der Fenstermitte schwarz faerben */
		g_put_pixel(width/2, height/2, g_rgb(0,0,0));
			/* Text "Hello, world!" in der Farbe Lila an die Stelle (20, 100) in das Graphikfenster */
		g_render_text(20, 100, "Hello, world!", g_rgb(255,0,255));
		
		/* Fensterinhalt anzeigen
		 * Diese Funktion muss unbedingt periodisch aufgerufen werden.
		 * Andernfalls kann es zur Ueberlastung des Graphiktreibers mit anschliessendem Absturz
		 * des Betriebssystems kommen.
		 */
		g_show();
	}
	
	/* Graphikumgebung vor dem Programmende freigeben */
	g_destroy();
	return 0;
}
#endif


/**
 * Farbaussteuerungen
 *  - red fuer den Rot-,
 *  - green fuer den Gruen- und
 *  - blue fuer den Blauanteil
 * einer Farbe.
 * Farben werden additiv gemischt, beispielsweise:
 *     red =   0, green =   0, blue =   0 ==> Schwarz
 *     red = 128, green = 128, blue = 128 ==> Grau
 *     red = 255, green = 255, blue = 255 ==> Weiss
 *     red = 255, green =   0, blue =   0 ==> Rot
 *     red = 255, green = 255, blue =   0 ==> Gelb
 *     red =   0, green = 255, blue =   0 ==> Gruen
 *     red =   0, green = 255, blue = 255 ==> Tuerkis
 *     red =   0, green =   0, blue = 255 ==> Blau
 *     red = 255, green =   0, blue = 255 ==> Lila
 *
 * Dabei ist alpha die Deckkraft der Farbe.
 *     alpha =   0  ==> unsichtbar
 *     alpha = 128  ==> halb transparent
 *     alpha = 255  ==> volle Deckkraft
 */
typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
} Color;

/**
 * Erzeugung einer Farbe mit voller Deckkraft.
 */
Color g_rgb(unsigned char red, unsigned char green, unsigned char blue);

/**
 * Erzeugung einer Farbe mit beliebiger Deckkraft.
 */
Color g_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

/**
 * Erzeugung der Graphikumgebung.
 * Hierbei wird ein Fenster mit der Breite width und der Hoehe height angelegt.
 * Der Titel des Fensters wird durch den Parameter title festgelegt.
 * 
 * Die einzelnen Bildpunkte des Fensters werden mit dem Fensterkoordinatensystem adressiert.
 * Siehe hierzu folgende Skizze:
 * ---- x-Achse--------------------------------->
 * |D                                          C|
 * |                                            |
 * |              -----------Winkel--           |
 * |               \ phi   )von                 y-Achse
 * |                \     )                     |
 * |                 \   )                      |
 * |                  \ )nach                   |
 * |A                  \                       B|
 * ---------------------------------------------V
 * 
 * Dabei haben die Eckpunkte A, B, C und D des Fensters folgende Koordinaten:
 *     D: (      0,        0)
 *     C: (width-1,        0)
 *     A: (      0, height-1)
 *     B: (width-1, height-1)
 */
void g_create(int width, int height, const char* title);

/**
 * Freigeben der Graphikumgebung.
 */
void g_destroy();

/**
 * Loeschen des Fensterinhalts.
 */
void g_clear(unsigned char r, unsigned char g,unsigned char b,unsigned char a);

/**
 * Anzeigen des Fensterinhalts.
 * Diese Funktion muss unbedingt periodisch aufgerufen werden.
 * Andernfalls kann es zur Ueberlastung des Graphiktreibers mit anschliessendem Absturz
 * des Betriebssystems kommen.
 */
void g_show();

/**
 * Ueberpruefen, ob das Fenster geschlossen wurde.
 * Die Funktion g_has_quit hat den Seiteneffekt, dass sie die Maus- und Tastaturereignisse verarbeitet.
 * Daher MUSS sie unbedingt periodisch aufgerufen werden, selbst wenn das Ergebnis nicht verwendet wird.
 * 
 * Liefert 1 zurueck, falls das Fenster geschlossen wurde.
 * Liefert 0 zurueck, sonst.
 */
int g_has_quit();

/**
 * Faerbt den Bildpunkt (x,y) mit der Farbe color.
 */
void g_put_pixel(int x, int y, Color color);

/**
 * Faerbt die Bildpunkte der Strecke von (x1,y) bis (x2,y) mit der Farbe color.
 */
void g_draw_horizontal_line(int x1, int x2, int y, Color color);

/**
 * Faerbt die Bildpunkte der Strecke von (x,y1) bis (x,y2) mit der Farbe color.
 */
void g_draw_vertical_line(int x, int y1, int y2, Color color);

/**
 * Faerbt die Bildpunkte der Strecke von (x1,y1) bis (x2,y2) mit der Farbe color.
 */
void g_draw_line(int x1, int y1, int x2, int y2, Color color);

/**
 * Faerbt die Bildpunkte der Strecke von (x1,y1) bis (x2,y2) mit der Farbe color.
 * Dabei wird die Strichstaerke line_width verwendet.
 */
void g_draw_thick_line(int x1, int y1, int x2, int y2, unsigned char line_width, Color color);

/**
 * Faerbt die Bildpunkte des Randes eines achsparallelen Rechtecks mit der Farbe color.
 * Dabei sind (x1,y1) und (x2,y2) die Koordinaten zweier sich gegenueber liegender Eckpunkte
 * des Rechtecks.
 */
void g_draw_rectangle(int x1, int y1, int x2, int y2, Color color);

/**
 * Faerbt die Bildpunkte des Randes eines achsparallelen Rechtecks mit der Farbe color.
 * Dabei sind (x1,y1) und (x2,y2) die Koordinaten zweier sich gegenueber liegender Eckpunkte
 * des Rechtecks.
 * Das Rechteck wird mit abgerundeten Ecken gezeichnet, dabei ist corner_radius der Radius
 * der Abrundung.
 */
void g_draw_rounded_rectangle(int x1, int y1, int x2, int y2, int corner_radius, Color color);

/**
 * Faerbt die Bildpunkte eines Kreisumfangs mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und radius der Radius des Kreises.
 */
void g_draw_circle(int x, int y, int radius, Color color);

/**
 * Faerbt die Bildpunkte des Umfangs einer Ellipse mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und (radius_x,radius_y) die beiden Halbachsen der Ellipse.
 */
void g_draw_ellipse(int x, int y, int radius_x, int radius_y, Color color);

/**
 * Faerbt die Bildpunkte eines Kreisbogens mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und radius der Radius des Kreisbogens.
 * Die Parameter begin_degrees und end_degrees legen Start- bzw. Endwinkel des Kreisbogens fest.
 */
void g_draw_arc(int x, int y, int radius, int begin_degrees, int end_degrees, Color color);

/**
 * Faerbt die Bildpunkte des Randes eines Dreiecks mit der Farbe color.
 * Dabei sind (x1,y1), (x2,y2) und (x3,y3) die Eckpunkte des Dreiecks.
 */
void g_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);

/**
 * Faerbt die Bildpunkte der Flaeche eines achsparallelen Rechtecks mit der Farbe color.
 * Dabei sind (x1,y1) und (x2,y2) die Koordinaten zweier sich gegenueber liegender Eckpunkte
 * des Rechtecks.
 */
void g_fill_rectangle(int x1, int y1, int x2, int y2, Color color);

/**
 * Faerbt die Bildpunkte der Flaeche eines achsparallelen Rechtecks mit der Farbe color.
 * Dabei sind (x1,y1) und (x2,y2) die Koordinaten zweier sich gegenueber liegender Eckpunkte
 * des Rechtecks.
 * Das Rechteck wird mit abgerundeten Ecken gezeichnet, dabei ist corner_radius der Radius
 * der Abrundung.
 */
void g_fill_rounded_rectangle(int x1, int y1, int x2, int y2, int corner_radius, Color color);

/**
 * Faerbt die Bildpunkte einer Kreisflaeche mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und radius der Radius des Kreises.
 */
void g_fill_circle(int x, int y, int radius, Color color);

/**
 * Faerbt die Bildpunkte der Flaeche einer Ellipse mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und (radius_x,radius_y) die beiden Halbachsen der Ellipse.
 */
void g_fill_ellipse(int x, int y, int radius_x, int radius_y, Color color);

/**
 * Faerbt die Bildpunkte der Flaeche eines Kreissektors ("Tortenstueck") mit der Farbe color.
 * Dabei sind (x,y) der Mittelpunkt und radius der Radius des Kreissektors.
 * Die Parameter begin_degrees und end_degrees legen Start- bzw. Endwinkel des Kreissektors fest.
 */
void g_fill_pie(int x, int y, int radius, int begin_degrees, int end_degrees, Color color);

/**
 * Faerbt die Bildpunkte der Flaeche eines Dreiecks mit der Farbe color.
 * Dabei sind (x1,y1), (x2,y2) und (x3,y3) die Eckpunkte des Dreiecks.
 */
void g_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);

/**
 * Laed eine Schriftart in der Schriftgroesse point_size.
 * Die Schriftart wird aus der Datei ttf_file geladen.
 * Die Schriftart ist im True-Type-Font Dateiformat (TTF) hinterlegt.
 * Diese Funktion ist vor der Verwendung von
 *    - g_text_dimensions,
 *    - g_render_text oder
 *    - g_render_text_shaded
 * aufzurufen.
 */
void g_load_font(const char* ttf_file, int point_size);

/**
 * Bestimmt die Ausdehnung (Breite und Hoehe in Bildpunkten) des Textes text mit der aktuell
 * eingestellten Schriftart.
 * Die Breite des Textes wird am Ort width und die Hoehe des Textes wird am Ort height gespeichert.
 * Vor Aufruf dieser Funktion muss zunaechst eine Schriftart durch Aufruf der Funktion g_load_font
 * geladen werden.
 */
void g_text_dimensions(int* width, int* height, const char* text);

/**
 * Schreibt den Text text in der Farbe text_color an der Position (x,y) in der eingestellten
 * Schriftart in das Graphikfenster.
 * Vor Aufruf dieser Funktion muss zunaechst eine Schriftart durch Aufruf der Funktion g_load_font
 * geladen werden.
 */
void g_render_text(int x, int y, const char* text, Color text_color);

/**
 * Schreibt den Text text in der Farbe text_color an der Position (x,y) in der eingestellten
 * Schriftart in das Graphikfenster. Der Hintergrund des Textes wird mit der Farbe background_color gefaerbt.
 * Vor Aufruf dieser Funktion muss zunaechst eine Schriftart durch Aufruf der Funktion g_load_font
 * geladen werden.
 */
void g_render_text_shaded(int x, int y, const char* text, Color text_color, Color background_color);

/**
 * Abfrage, ob eine Taste gedrueckt wurde.
 * Falls eine Taste gedrueckt wurde, so wird der entsprechende Tastaturkode
 * (entspricht fuer Ziffern und Buchstaben dem jeweiligen ASCII-Wert)
 * am Ort key gespeichert und eine 1 zurueck gegeben.
 * Sonst: Rueckgabe von 0.
 */
int g_key(int* key);

/**
 * Abfrage, ob eine Mausbewegung erfolgt ist.
 * Falls eine Mausbewegung erfolgt ist, so wird die neue Mauspositon
 * an den Orten x bzw. y gespeichert und eine 1 zurueck gegeben.
 * Sonst: Rueckgabe von 0.
 * 
 * Die Mauspositon (*x, *y) gibt die Koordinaten des Bildpunkts unter
 * dem Mauszeiger im Graphikfenster an.
 */
int g_mouse_motion(int* x, int* y);

/**
 * Abfrage, ob ein Maustastenereignis erfolgt ist.
 * Falls ein Maustastenereignis erfolgt ist, so wird die verursachende Maustaste
 * am Ort button und die Art des Ereignisses (gedrueckt -> 1, losgelassen -> 0)
 * am Ort button_state gespeichert. Anschliessend wird eine 1 zurueck gegeben.
 * Sonst: Rueckgabe von 0.
 */
int g_mouse_button(int* button, int* button_state);

/**
 * Abfrage, ob ein Mausradereignis erfolgt ist.
 * Falls ein Mausradereignis erfolgt ist, so wird die Richtung der Mausradbewegung
 * am Ort direction gespeichert. Anschliessend wird eine 1 zurueck gegeben.
 * Sonst: Rueckgabe von 0.
 */
int g_mouse_wheel(int* direction);

/**
 * Erzeugung einer Farbe im HSV-System: 
 * h ist der Farbwinkel von 0...1
 *
 * h		|	Farbton
 * ----------------------------
 * 0./6		|	rot
 * 1./6		|	gelb
 * 2./6		|	gruen
 * 3./6		|	cyan (tuerkis)
 * 4./6		|	blau
 * 5./6		|	magenta (pink)
 * 6./6		|	rot
 *
 * s ist die Saettigung von 0...1
 * s = 0 => Von schwarz ueber grau bis weiss
 * s = 1 => Maximal gesaettigte Farbe
 *
 * v ist die Helligkeit
 * v = 0 => schwarz
 * v = 1 => maximal helle Farbe
 * Weitere Informationen: https://de.wikipedia.org/wiki/HSV-Farbraum
 */
Color g_hsv(double h, double s, double v);

/**
 * Pausieren des Programmablaufs fuer delay_milli_seconds Millisekunden
 */
void g_delay(unsigned int delay_milli_seconds);

#ifdef __cplusplus
}
#endif
