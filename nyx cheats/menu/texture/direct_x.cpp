#include "direct_x.hpp"
#include "../../pointers/pointers.hpp"
#include "shader/stb_image.h"
#include "shader/stb_image_write.h"
#include "../../utilities/log/log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

using namespace VX;

direct_x g_direct_x;


void direct_x::initialize()
{
	m_swapchain = *get_pointers()->m_swapchain;
	if (FAILED(m_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)m_device.GetAddressOf())))
		throw std::runtime_error("Failed to create a ID3D11Device");
	m_device->GetImmediateContext(m_context.GetAddressOf());

	get_log()->send("Direct X", "Create Direct X");
}

std::pair<direct_x_shader::shader_data, direct_x_shader::vec2> direct_x::create_texture(std::filesystem::path path) {
	direct_x_shader::image image{ read_image_file(path) };
	return std::make_pair(direct_x::create_shader_data(image), image.size);
}

std::vector<direct_x_shader::frame_data> direct_x::create_gif_texture(std::filesystem::path path) {
	std::vector<direct_x_shader::frame_data> frames{};
	std::vector<direct_x_shader::image_data> gif{ split_gif(path) };
	for (auto& fr : gif) {
		direct_x_shader::image image{ read_memory(fr) };
		frames.push_back({ fr.delay, direct_x::create_shader_data(image) });
	}
	return frames;
}

direct_x_shader::shader_data direct_x::create_shader_data(direct_x_shader::image image) {
	if (!image.data) {
		return {};
	}

	ID3D11ShaderResourceView* resourceView{};
	ID3D11Texture2D* texture{};
	D3D11_TEXTURE2D_DESC desc{ image.size.x.u32, image.size.y.u32, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, { 1 }, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0 };
	D3D11_SUBRESOURCE_DATA subResource{ image.data, desc.Width * 4, 0 };
	m_device->CreateTexture2D(&desc, &subResource, &texture);

	if (!texture) {
		return {};
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{ DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D, { 0, desc.MipLevels } };
	m_device->CreateShaderResourceView(texture, &srvDesc, std::add_pointer_t<decltype(resourceView)>(&resourceView));

	if (!texture || !resourceView) {
		return {};
	}
	else {
		return { texture, resourceView };
	}
}

direct_x_shader::image_data direct_x::write_memory(direct_x_shader::vec2& size, int comp, const void* data, int strideBytes, uint32_t delay) {
	int imageSize;
	stbi_uc* image{ stbi_write_png_to_mem((const unsigned char*)data, strideBytes, size.x.i32, size.y.i32, comp, &imageSize) };
	if (!image) {
		return {};
	}
	else {
		return { image, imageSize, delay };
	}
}
direct_x_shader::image direct_x::read_memory(direct_x_shader::image_data data) {
	direct_x_shader::vec2 size{};
	stbi_uc* image{ stbi_load_from_memory(data.bytes, data.size, &size.x.i32, &size.y.i32, NULL, 0) };
	if (!image) {
		return {};
	}
	else {
		return { image, size };
	}
}
direct_x_shader::gif direct_x::read_gif_memory(direct_x_shader::image_data data) {
	direct_x_shader::vec2 size{};
	int* delays{};
	int frameCount{}, comp{};
	stbi_uc* image{ stbi_load_gif_from_memory(data.bytes, data.size, &delays, &size.x.i32, &size.y.i32, &frameCount, &comp, NULL) };
	if (!image) {
		return {};
	}
	else {
		return { image, data, delays,frameCount,size,comp };
	}
}


direct_x_shader::image direct_x::read_image_file(std::filesystem::path path) {
	direct_x_shader::vec2 size{};
	stbi_uc* image{ stbi_load(path.string().c_str(), &size.x.i32, &size.y.i32, NULL, 4) };
	if (!image) {
		return {};
	}
	else {
		return { image, size };
	}
}

direct_x_shader::gif direct_x::read_gif_file(std::filesystem::path path) {
	FILE* f{ stbi__fopen(path.string().c_str(), "rb") };
	if (!f) {
		return {};
	}

	direct_x_shader::image_data data{};
	fseek(f, 0L, SEEK_END);
	data.size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	data.bytes = decltype(data.bytes)(malloc(data.size));
	fread(data.bytes, data.size, 1, f);
	fclose(f);

	direct_x_shader::gif gif_result = read_gif_memory(data);
	free(data.bytes);

	if (!gif_result.bytes) {
		return {};
	}
	else {
		return gif_result;
	}

}

std::vector<direct_x_shader::image_data> direct_x::split_gif(std::filesystem::path path) {
	direct_x_shader::gif gif{ read_gif_file(path) };
	std::vector<direct_x_shader::image_data> frames{};
	size_t bytes{ size_t(gif.size.x.i32 * gif.comp) };
	for (size_t i{}; i != gif.frame_count; ++i)
		frames.push_back(write_memory(gif.size, gif.comp, gif.bytes + bytes * gif.size.y.i32 * i, 0, gif.delays[i]));
	stbi_image_free(gif.bytes);
	return frames;
}

direct_x* get_direct_x() { return &g_direct_x; }