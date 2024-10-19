#pragma once
#include "../common.hpp"
#include "../invoker/natives.hpp"
#include "../utilities/utilities.hpp"
#include "../gta/enums.hpp"
#include "../utilities/timer.h"

typedef enum {
	FontChaletLondon = 0,
	FontHouseScript = 1,
	FontMonoSpace = 2,
	FontWindlings = 3,
	FontChaletComprimeCologne = 4,
	FontChaletComprimeCologne2 = 5,
	FontChaletComprimeCologne3 = 6,
	FontPricedown = 7
} Font;

typedef enum {
	JustifyIgnore = -1,
	JustifyCenter,
	JustifyLeft,
	JustifyRight
} Justify;

class renderer
{
public:
	void initialize();
	void update();

public:
	void render();
	void render_header();
	void render_sub_header();
	void render_background();
	void render_option_highlight();
	void render_footer();
	void render_tooltip(std::string text);
	void render_globe();

public:
	float get_string_width(const char* str, int font, float fontsize);
	float get_string_height(int font, float fontsize);

	void draw_string(const char* text, float x, float y, int font, float scale, RGBA color, bool outline = false, int justify = -1, float justifyfix = 0.0f, bool loginbox = false);
	void draw_sprite(Texture texture, float x, float y, float w, float h, float heading, RGBA color, bool renderIfNotStreamed = true);
	void draw_rect(float x, float y, float width, float height, RGBA color);
	void draw_line(float x1, float y1, float z1, float x2, float y2, float z2, RGBA color);
	void draw_title(const char* title);
	void draw_in_world_box(Entity entity, RGBA color);
	void set_2d_layer(int layer);

	RGBA header_color = { 105, 23, 195, 255 };
	RGBA sub_header_color = { 0, 0, 0, 210 };
	RGBA title_color = { 255, 255, 255, 255 };
	RGBA sub_menu_title_color = { 255, 255, 255, 255 };
	RGBA background_color = { 0, 0, 0, 190 };
	RGBA menu_text_color = { 255, 255, 255, 255 };
	RGBA highlighted_option_color = { 0, 0, 0, 255 };

	float width = 0.235f;
	float bg_opacity = 45;
	float line_Width = 0.0009f;
	float header_height = 0.083f;
	float render_options;
	float option_height = 0.037f;
	float sub_header_height = option_height;
	float footer_height = option_height;
	std::string tooltip = "";
	int m_2d_layer;
	int text_font = FontChaletLondon;
	int toggle_style;
	float* m_opacity = 0;

	void SetOpacityVariable(float& opacity) {
		m_opacity = &opacity;
	}

	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}
};

renderer* get_renderer();