#include "renderer.hpp"
#include "menu.hpp"
#include "submenu_handler.hpp"

renderer g_renderer;

void renderer::render() {
	get_submenu_handler()->get_total_options() > get_menu()->m_max_options ? render_options = get_menu()->m_max_options : render_options = get_submenu_handler()->get_total_options();

	draw_string(NAME, get_menu()->scaled_x_coord + get_menu()->pos.x, 0.07533f + get_menu()->pos.y, FontHouseScript, 1.0f, title_color, true, -2);

	render_header();
	render_sub_header();
	render_background();
	render_option_highlight();
	render_footer();
	render_globe();
}

void renderer::render_header() {
	draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, 0.09733f + get_menu()->pos.y, 0.235f, 0.09000f, header_color);
}

void renderer::render_sub_header() {
	draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, 0.16027f + get_menu()->pos.y, 0.235f, 0.03753f, sub_header_color);
	draw_string(std::format("{} / {}", get_menu()->m_current_option + 1, get_submenu_handler()->get_total_options()).c_str(), get_menu()->pos.x + get_menu()->scaled_x_coord + (0.958f - 0.7f), get_menu()->pos.y + 0.14587f, 0, 0.37f, sub_menu_title_color, false, JustifyRight, 0.695f);
}

void renderer::render_background() {
	draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, (0.198f + ((0.019f - 0.00040f) * (render_options - 1))) + get_menu()->pos.y, 0.235f, (0.040f + ((0.037f + 0.00010f) * (render_options - 1))), background_color);
}

void renderer::render_option_highlight() {
	static float CurrentCoord = get_menu()->pos.y;

	get_menu()->current_rendering_scroll = g_math.Clamp(get_menu()->m_current_option - get_menu()->m_scroll_offset > get_menu()->m_max_options ? get_menu()->m_max_options : (get_menu()->m_current_option - get_menu()->m_scroll_offset) + 1, 0, get_menu()->m_max_options);
	CurrentCoord = lerp(CurrentCoord, (0.161333f + (get_menu()->current_rendering_scroll * option_height)) + get_menu()->pos.y, 0.25f);

	draw_sprite({ "commonmenu", "gradient_nav" }, get_menu()->scaled_x_coord + get_menu()->pos.x, CurrentCoord, 0.235f, 0.03753f, 0.f, { 255,255,255,255 });
}

void renderer::render_tooltip(std::string text)
{
	if (!text.empty())
	{
		tooltip = text;
		set_2d_layer(8);
		int DrawPos = get_submenu_handler()->get_total_options() > get_menu()->m_max_options ? get_menu()->m_max_options : get_submenu_handler()->get_total_options();

		HUD::SET_TEXT_FONT(FontChaletLondon);
		HUD::SET_TEXT_SCALE(0.f, (0.4f * 0.75f));
		HUD::SET_TEXT_WRAP(get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f, (get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f) + width);
		HUD::BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.c_str());
		int Lines = HUD::END_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING(get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f, ((0.1978f + ((0.03753f - 0.00030f) * (DrawPos + 2)))) + get_menu()->pos.y - 0.037f);

		float Height = 0.0315f;
		if (Lines > 1) {
			Height = (Lines * 0.022f);
		}

		draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, ((0.1978f + ((0.03753f - 0.00030f) * (DrawPos + 2)))) + get_menu()->pos.y - 0.037f + (Height * 0.5f), 0.235f, Height, { 0, 0, 0, 155 });
		draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, ((0.1978f + ((0.03753f - 0.00030f) * (DrawPos + 2)))) + get_menu()->pos.y - (0.030f / 2) - 0.025f + (0.005f * 0.5f), 0.235f, 0.005f, header_color);
		HUD::SET_TEXT_WRAP(get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f, (get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f) + width);
		draw_string(tooltip.c_str(), get_menu()->scaled_x_coord + get_menu()->pos.x - 0.115f, ((0.1978f + ((0.03753f - 0.00030f) * (DrawPos + 2)))) + get_menu()->pos.y - 0.037f + (Lines == 1 ? 0.002f : 0.f), FontChaletLondon, 0.4f, { 255, 255, 255, 255 }, false, -1, 0.38f, 0.4975f);
	}
}

void renderer::render_footer() {
	int index = get_submenu_handler()->get_total_options() > get_menu()->m_max_options ? get_menu()->m_max_options : get_submenu_handler()->get_total_options();
	draw_rect(get_menu()->scaled_x_coord + get_menu()->pos.x, ((0.1978f + ((0.03753f - 0.00030f) * index))) + get_menu()->pos.y, 0.235f, 0.03753f, sub_header_color);
	draw_sprite({ "commonmenu", "shop_arrows_upanddown" }, get_menu()->scaled_x_coord + get_menu()->pos.x, ((0.1978f + ((0.03753f - 0.00030f) * index))) + get_menu()->pos.y, 0.235f / 13.0, 0.03753f / 1.2, 0, { 255, 255, 255, 200 });
}

void renderer::render_globe() {
	static int m_GlobeHandle;
	if (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_GlobeHandle)) {
		m_GlobeHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
	}
	else {
		float Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2).z;
		Rotation -= round(Rotation / 360.0f) * 360.0f;
		if (Rotation < 0.f) {
			Rotation += 360.0f;
		}

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_GlobeHandle, "SET_DATA_SLOT");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(Rotation);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}

	GRAPHICS::DRAW_SCALEFORM_MOVIE(m_GlobeHandle, get_menu()->scaled_x_coord + get_menu()->pos.x + 0.33801f, 0.09733f + get_menu()->pos.y + 0.406f, 0.98f, 1.f, 255, 255, 255, 255, 0);
}

float renderer::get_string_width(const char* str, int font, float fontsize) {
	if (font == FontChaletLondon) fontsize *= 0.75f;
	HUD::BEGIN_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(str);
	HUD::SET_TEXT_FONT(font);
	HUD::SET_TEXT_SCALE(fontsize, fontsize);
	return HUD::END_TEXT_COMMAND_GET_SCREEN_WIDTH_OF_DISPLAY_TEXT(font);
}

float renderer::get_string_height(int font, float fontsize) {
	return HUD::GET_RENDERED_CHARACTER_HEIGHT(fontsize, font);
}

void renderer::draw_title(const char* title) {
	std::string str = title;
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	draw_string(str.c_str(), 0.742f + get_menu()->pos.x - (0.01f / 2) - 0.7f, 0.14587f + get_menu()->pos.y, 0, 0.37f, sub_menu_title_color);
}

void renderer::draw_sprite(Texture texture, float x, float y, float w, float h, float heading, RGBA color, bool renderIfNotStreamed) {
	bool streamed = GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(texture.dict);
	if (streamed || renderIfNotStreamed) GRAPHICS::DRAW_SPRITE(texture.dict, texture.id, x, y, w, h, heading, color.r, color.g, color.b, color.a, 0, 0);
	if (!streamed) GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(texture.dict, 0);
}

void renderer::draw_rect(float x, float y, float width, float height, RGBA color) {
	GRAPHICS::DRAW_RECT(x, y, width, height, color.r, color.g, color.b, color.a, 0);
}

void renderer::draw_line(float x1, float y1, float z1, float x2, float y2, float z2, RGBA color) {
	GRAPHICS::DRAW_LINE(x1, y1, z1, x2, y2, z2, color.r, color.g, color.b, color.a);
}

void renderer::draw_string(const char* text, float x, float y, int font, float scale, RGBA color, bool outline, int justify, float justifyfix, bool loginbox) {
	HUD::SET_TEXT_FONT(font);

	if (justify != -1) {
		HUD::SET_TEXT_WRAP(0.0f, 0.958f - justifyfix);
		HUD::SET_TEXT_JUSTIFICATION(justify);
	}

	if (font == FontChaletLondon && loginbox) {
		scale *= 0.75f;
		y += 0.003f;
	}
	if (font == FontChaletComprimeCologne && loginbox) {
		y += 0.003f;
	}

	HUD::SET_TEXT_SCALE(0.0f, scale);
	HUD::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);

	if (outline) HUD::SET_TEXT_OUTLINE();
	HUD::SET_TEXT_CENTRE(justify == -2);
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
}

void renderer::set_2d_layer(int layer) {
	GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(layer);
	m_2d_layer = layer;
}

void renderer::draw_in_world_box(Entity entity, RGBA color) {
	Vector3 Min;
	Vector3 Max;
	MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(entity), &Min, &Max);
	float BoxWidth = Max.x * 2;
	float BoxLength = Max.y * 2;
	float BoxHeight = Max.z * 2;

	Vector3 P1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, (BoxWidth / 2) * -1, (BoxLength / 2), BoxHeight / 2);
	Vector3 P4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, BoxWidth / 2, (BoxLength / 2), BoxHeight / 2);
	Vector3 P5 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, (BoxWidth / 2) * -1, (BoxLength / 2), (BoxHeight / 2) * -1);
	Vector3 P7 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, BoxWidth / 2, (BoxLength / 2), (BoxHeight / 2) * -1);
	Vector3 P2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, (BoxWidth / 2) * -1, (BoxLength / 2) * -1, BoxHeight / 2);
	Vector3 P3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, BoxWidth / 2, (BoxLength / 2) * -1, BoxHeight / 2);
	Vector3 P6 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, (BoxWidth / 2) * -1, (BoxLength / 2) * -1, (BoxHeight / 2) * -1);
	Vector3 P8 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, BoxWidth / 2, (BoxLength / 2) * -1, (BoxHeight / 2) * -1);

	GRAPHICS::DRAW_LINE(P1.x, P1.y, P1.z, P4.x, P4.y, P4.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P1.x, P1.y, P1.z, P2.x, P2.y, P2.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P1.x, P1.y, P1.z, P5.x, P5.y, P5.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P2.x, P2.y, P2.z, P3.x, P3.y, P3.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P3.x, P3.y, P3.z, P8.x, P8.y, P8.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P4.x, P4.y, P4.z, P7.x, P7.y, P7.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P4.x, P4.y, P4.z, P3.x, P3.y, P3.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P5.x, P5.y, P5.z, P7.x, P7.y, P7.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P6.x, P6.y, P6.z, P2.x, P2.y, P2.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P6.x, P6.y, P6.z, P8.x, P8.y, P8.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P5.x, P5.y, P5.z, P6.x, P6.y, P6.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_LINE(P7.x, P7.y, P7.z, P8.x, P8.y, P8.z, color.r, color.g, color.b, color.a);
}

renderer* get_renderer() { return &g_renderer; }