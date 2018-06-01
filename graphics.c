#include "graphics.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>


/**
 *	Lieber Leser, bitte nicht nachmachen.
 *	Die Verwendung globaler Variablen ist boese,
 *	wird im folgenden aber leider benoetigt, um die Schnittstelle
 *	moeglichst anfaengerfreundlich zu gestalten.
 *	Desweiteren wurden schlimme Hacks (sicher leicht zu finden
 *	und in eigenen Projekten vermeidbar)
 *	vorgenommen, um die Robustheit des Systems im Laborbetrieb
 *	sicherzustellen.
 * 
 *	Wer bei der Bearbeitung des Laborversuches Spass am Thema
 *	Computergraphik gefunden hat, dem seien die Tutorials unter
 *		http://lazyfoo.net/tutorials/SDL/
 *	ans Herz gelegt.
 */
static struct {
	int is_init;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	int has_mouse;
	int mouse_x;
	int mouse_y;
	int has_mouse_button;
	int has_mouse_wheel;
	int mouse_button;
	int mouse_button_state;
	int mouse_wheel;
	int has_key;
	int key;
	int no_show_cnt;
	int no_event_cnt;
} context = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static void emergency_exit() {
	if(context.renderer) {
		SDL_DestroyRenderer( context.renderer );
	}
	if(context.window) {
		SDL_DestroyWindow( context.window );
	}
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


static void check_failure(int failure, const char* message) {
	if(failure) {
		fprintf(stderr, "ERROR: %s\n", message);
		emergency_exit();
		exit(1);
	}
}

static void base_init(){
	if(!context.is_init) {
		check_failure(
			SDL_Init( SDL_INIT_VIDEO ) < 0
			, "could not SDL_INIT_VIDEO"
		);
		int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
		int img_initted = IMG_Init(img_flags);
		check_failure(
			(img_initted & img_flags) != img_flags
			, "could not IMG_Init"
		);
		check_failure(
			TTF_Init() < 0
			, "could not TTF_Init"
		);
		context.is_init = 1;
	} else {
		check_failure(1, "base_init may only called once");
	}
}

static void base_quit(){
	if(context.is_init) {
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		context.is_init = 0;
	} else {
		check_failure(1, "base_quit may only called once");
	}
}

void g_create(int width, int height, const char* title) {
	if(!context.window && !context.renderer) {
		base_init();
		context.window = SDL_CreateWindow(
			  title
			, SDL_WINDOWPOS_UNDEFINED
			, SDL_WINDOWPOS_UNDEFINED
			, width
			, height
			, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
		);
		check_failure(
			  !context.window
			, "could not SDL_CreateWindow"
		);
		context.renderer = SDL_CreateRenderer( context.window, -1, SDL_RENDERER_ACCELERATED );
		if(!context.renderer){
			context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_SOFTWARE);
			check_failure(
				!context.renderer
				, "could not SDL_CreateRenderer"
			);
		}
	} else {
		check_failure(1, "g_create may only be called once");
	}
}
	
	
void g_destroy() {
	if(context.window || context.renderer) {
		SDL_DestroyRenderer( context.renderer );
		SDL_DestroyWindow( context.window );
		base_quit();
		context.window = 0;
		context.renderer = 0;
	} else {
		check_failure(1, "g_destroy may only be called once");
	}
}


Color g_rgb(unsigned char red, unsigned char green, unsigned char blue) {
	Color color;
	color.red = red;
	color.green = green;
	color.blue = blue;
	color.alpha = 255;
	return color;
}
Color g_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
	Color color;
	color.red = red;
	color.green = green;
	color.blue = blue;
	color.alpha = alpha;
	return color;
}

static void life_saver() {
	++context.no_show_cnt;
	if(context.no_show_cnt > 1024 * 1024 * 64) {
		g_show();
	}
	if(context.no_event_cnt > 1024 * 1024 * 64) {
		g_has_quit();
	}
}

void g_clear(unsigned char r, unsigned char g,unsigned char b,unsigned char a) {
	life_saver();
	//unsigned char r = 0, g = 0, b = 0, a = 255;
	//unsigned char r = 255, g = 255, b = 255, a = 255;
	check_failure(
		  SDL_SetRenderDrawColor( context.renderer, r, g, b, a) < 0
		, "could not SDL_SetRenderDrawColor"
	);
	SDL_RenderClear(context.renderer);
}

void g_show() {
	SDL_RenderPresent(context.renderer);
	SDL_Delay(10);
	context.no_show_cnt = 0;
}

int g_has_quit() {
	context.no_event_cnt = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type){
			case SDL_QUIT:
				return 1;
				break;
			case SDL_KEYDOWN:
				context.has_key = 1;
				context.key = event.key.keysym.sym;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym == 27) {
					return 1;
				}
				break;
			case SDL_MOUSEMOTION:
				context.has_mouse = 1;
				context.mouse_x = event.motion.x;
				context.mouse_y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				context.has_mouse_button = 1;
				context.mouse_button = event.button.button;
				context.mouse_button_state = event.button.state;
				context.mouse_x = event.button.x;
				context.mouse_y = event.button.y;
				break;
			case SDL_MOUSEWHEEL:
				context.has_mouse_wheel = 1;
				context.mouse_wheel = event.wheel.y;
				break;
			default:
				;
		}
	}
	return 0;
}

void g_put_pixel(int x, int y, Color color) {
	check_failure(
		  0 != pixelRGBA(context.renderer, x, y, color.red, color.green, color.blue, color.alpha)
		, "could not g_put_pixel"
	);
	life_saver();
}
void g_draw_horizontal_line(int x1, int x2, int y, Color color) {
	check_failure(
		  0 != hlineRGBA(context.renderer, x1, x2, y, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_horizontal_line"
	);
	life_saver();
}
void g_draw_vertical_line(int x, int y1, int y2, Color color) {
	check_failure(
		  0 != vlineRGBA(context.renderer, x, y1, y2, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_vertical_line"
	);
	life_saver();
}
void g_draw_rectangle(int x1, int y1, int x2, int y2, Color color) {
	check_failure(
		  0 != rectangleRGBA(context.renderer, x1, y1, x2, y2, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_rectangle"
	);
	life_saver();
}
void g_draw_rounded_rectangle(int x1, int y1, int x2, int y2, int corner_radius, Color color) {
	check_failure(
		  0 != roundedRectangleRGBA(context.renderer, x1, y1, x2, y2, corner_radius, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_rounded_rectangle"
	);
	life_saver();
}
void g_fill_rectangle(int x1, int y1, int x2, int y2, Color color) {
	check_failure(
		  0 != boxRGBA(context.renderer, x1, y1, x2, y2, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_rectangle"
	);
	life_saver();
}
void g_fill_rounded_rectangle(int x1, int y1, int x2, int y2, int corner_radius, Color color) {
	check_failure(
		  0 != roundedBoxRGBA(context.renderer, x1, y1, x2, y2, corner_radius, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_rounded_rectangle"
	);
	life_saver();
}
void g_draw_line(int x1, int y1, int x2, int y2, Color color) {
	int anti_alias = 1;
	if(anti_alias){
		check_failure(
			  0 != aalineRGBA(context.renderer, x1, y1, x2, y2, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_line(aa)"
		);
	} else {
		check_failure(
			  0 != lineRGBA(context.renderer, x1, y1, x2, y2, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_line"
		);
	}
	life_saver();
}
void g_draw_thick_line(int x1, int y1, int x2, int y2, unsigned char line_width, Color color) {
	check_failure(
		  0 != thickLineRGBA(context.renderer, x1, y1, x2, y2, line_width, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_thick_line"
	);
	life_saver();
}
void g_draw_circle(int x, int y, int radius, Color color) {
	int anti_alias = 1;
	if(anti_alias) {
		check_failure(
			  0 != aacircleRGBA(context.renderer, x, y, radius, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_circle(aa)"
		);
	} else {
		check_failure(
			  0 != circleRGBA(context.renderer, x, y, radius, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_circle"
		);
	}
	life_saver();
}
void g_draw_arc(int x, int y, int radius, int begin_degrees, int end_degrees, Color color){
	check_failure(
		  0 != arcRGBA(context.renderer, x, y, radius, begin_degrees, end_degrees, color.red, color.green, color.blue, color.alpha)
		, "could not g_draw_arc"
	);
	life_saver();
}
void g_fill_circle(int x, int y, int radius, Color color) {
	check_failure(
		  0 != filledCircleRGBA(context.renderer, x, y, radius, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_circle"
	);
	life_saver();
}
void g_draw_ellipse(int x, int y, int radius_x, int radius_y, Color color) {
	int anti_alias = 1;
	if(anti_alias) {
		check_failure(
			  0 != aaellipseRGBA(context.renderer, x, y, radius_x, radius_y, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_ellipse(aa)"
		);
	} else {
		check_failure(
			  0 != ellipseRGBA(context.renderer, x, y, radius_x, radius_y, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_ellipse"
		);
	}
	life_saver();
}
void g_fill_ellipse(int x, int y, int radius_x, int radius_y, Color color) {
	check_failure(
		  0 != filledEllipseRGBA(context.renderer, x, y, radius_x, radius_y, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_ellipse"
	);
	life_saver();
}
void g_fill_pie(int x, int y, int radius, int begin_degrees, int end_degrees, Color color) {
	check_failure(
		  0 != filledPieRGBA(context.renderer, x, y, radius, begin_degrees, end_degrees, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_pie"
	);
	life_saver();
}
void g_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
	int anti_alias = 1;
	if(anti_alias) {
		check_failure(
			  0 != aatrigonRGBA(context.renderer, x1, y1, x2, y2, x3, y3, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_triangle"
		);
	} else {
		check_failure(
			  0 != trigonRGBA(context.renderer, x1, y1, x2, y2, x3, y3, color.red, color.green, color.blue, color.alpha)
			, "could not g_draw_triangle"
		);
	}
	life_saver();
}
void g_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
	check_failure(
		  0 != filledTrigonRGBA(context.renderer, x1, y1, x2, y2, x3, y3, color.red, color.green, color.blue, color.alpha)
		, "could not g_fill_triangle"
	);
	life_saver();
}

void g_load_font(const char* ttf_file, int point_size) {
	if(context.font) {
		TTF_CloseFont(context.font);
		context.font = 0;
	}
	context.font = TTF_OpenFont(ttf_file, point_size);
	check_failure(
		  context.font == 0
		, "unable to load font"
	);
	life_saver();
}

void g_text_dimensions(int* width, int* height, const char* text) {
	check_failure(context.font == 0, "could not g_text_dimensions on null-font");
	check_failure(
		  TTF_SizeText(context.font, text, width, height) < 0
		, "could not g_text_dimensions"
	);
	life_saver();
}

static void draw_image(int x, int y, SDL_Surface* surface) {
	SDL_Texture* texture = SDL_CreateTextureFromSurface(context.renderer, surface);
	check_failure(texture == 0, "could not create texture");
	SDL_Rect srcrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = surface->w;
	srcrect.h = surface->h;
	SDL_Rect dstrect;
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = surface->w;
	dstrect.h = surface->h;
	int e = SDL_RenderCopy(context.renderer, texture, &srcrect, &dstrect);
	SDL_DestroyTexture(texture);
	if(e != 0){
		check_failure(1, "could not SDL_RenderCopy");
	}
	life_saver();
}

void g_render_text(int x, int y, const char* text, Color text_color) {
	check_failure(context.font == 0, "could not g_render_text on null-font");
	SDL_Color fg;
	fg.r = text_color.red;
	fg.g = text_color.green;
	fg.b = text_color.blue;
	fg.a = text_color.alpha;
	SDL_Surface* surface = TTF_RenderText_Blended(context.font, text, fg);
	check_failure(surface == 0, "could not g_render_text");
	draw_image(x, y, surface);
	SDL_FreeSurface(surface);
}

void g_render_text_shaded(int x, int y, const char* text, Color text_color, Color background_color) {
	check_failure(context.font == 0, "could not g_render_text_shaded on null-font");
	SDL_Color fg;
	fg.r = text_color.red;
	fg.g = text_color.green;
	fg.b = text_color.blue;
	fg.a = text_color.alpha;
	SDL_Color bg;
	bg.r = background_color.red;
	bg.g = background_color.green;
	bg.b = background_color.blue;
	bg.a = background_color.alpha;
	SDL_Surface* surface = TTF_RenderText_Shaded(context.font, text, fg, bg);
	check_failure(surface == 0, "could not g_render_text");
	draw_image(x, y, surface);
	SDL_FreeSurface(surface);
}

int g_key(int* key) {
	if(context.has_key) {
		*key = context.key;
		context.has_key = 0;
		return 1;
	}
	return 0;
}

int g_mouse_motion(int* x, int* y) {
	if(context.has_mouse) {
		*x = context.mouse_x;
		*y = context.mouse_y;
		context.has_mouse = 0;
		return 1;
	}
	return 0;
}

int g_mouse_button(int* button, int* button_state) {
	if(context.has_mouse_button) {
		*button = context.mouse_button;
		*button_state = context.mouse_button_state;
		context.mouse_button = 0;
		context.mouse_button_state = 0;
		context.has_mouse_button = 0;
		return 1;
	}
	return 0;
}

int g_mouse_wheel(int* direction) {
	if(context.has_mouse_wheel) {
		*direction = context.mouse_wheel;
		context.has_mouse_wheel = 0;
		return 1;
	}
	return 0;
}

static double min(double a, double b) {
	return b < a ? b : a;
}
static double max(double a, double b) {
	return b < a ? a : b;
}
static double clip(double v, double min_v, double max_v) {
	return max(min(v, max_v), min_v);
}
// h, s, v in [0, 1]
Color g_hsv(double h, double s, double v) {
	s = clip(s, 0.0, 1.0);
	v = clip(v, 0.0, 1.0);
	h = fmod(h, 1.0);
	if (h < 0) {
		h += 1.0;
	}
	double H = 6.0*h;
	int hi = (int)H;
	double f = H - hi;
	double p = v * (1.0 - s);
	double q = v * (1.0 - s * f);
	double t = v * (1.0 - s * (1.0 - f));
	v = clip(256 * v, 0.0, 255.0);
	p = clip(256 * p, 0.0, 255.0);
	q = clip(256 * q, 0.0, 255.0);
	t = clip(256 * t, 0.0, 255.0);
	switch (hi) {
	case 1:  return g_rgb((unsigned char)q, (unsigned char)v, (unsigned char)p);
	case 2:  return g_rgb((unsigned char)p, (unsigned char)v, (unsigned char)t);
	case 3:  return g_rgb((unsigned char)p, (unsigned char)q, (unsigned char)v);
	case 4:  return g_rgb((unsigned char)t, (unsigned char)p, (unsigned char)v);
	case 5:  return g_rgb((unsigned char)v, (unsigned char)p, (unsigned char)q);
	default: return g_rgb((unsigned char)v, (unsigned char)t, (unsigned char)p);
	}
}

void g_delay(unsigned int delay_milli_seconds)
{
	SDL_Delay(delay_milli_seconds);
}
