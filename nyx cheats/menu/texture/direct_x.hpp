#pragma once
#include "../../common.hpp"
#include <wrl/internal.h>
#include <wrl/client.h>
#include <d3d11.h>

namespace direct_x_shader {
	union vector_coord {
		int16_t i16;
		uint16_t u16;
		int32_t i32;
		uint32_t u32;
		float f_point;
	};
	struct vec2 {
		vector_coord x{}, y{};
	};
	struct image { unsigned char* data{}; vec2 size{}; };
	struct image_data { unsigned char* bytes{}; int size{}; uint32_t delay{}; };
	struct shader_data { ID3D11Resource* resource{}; ID3D11ShaderResourceView* resource_view{}; };
	struct frame_data { uint32_t delay{}; shader_data shader{}; vec2 size{}; };
	struct gif { unsigned char* bytes{}; image_data data{}; int* delays{}; int frame_count{}; vec2 size{}; int comp{}; };
}

class direct_x
{
public:
	void initialize();

public:
	std::pair<direct_x_shader::shader_data, direct_x_shader::vec2> create_texture(std::filesystem::path path);
	std::vector<direct_x_shader::frame_data> create_gif_texture(std::filesystem::path path);
	direct_x_shader::shader_data create_shader_data(direct_x_shader::image image);

	direct_x_shader::image_data write_memory(direct_x_shader::vec2& size, int comp, const void* data, int strideBytes, uint32_t delay);
	direct_x_shader::image read_memory(direct_x_shader::image_data data);
	direct_x_shader::gif read_gif_memory(direct_x_shader::image_data data);
	direct_x_shader::image read_image_file(std::filesystem::path path);
	direct_x_shader::gif read_gif_file(std::filesystem::path path);
	std::vector<direct_x_shader::image_data> split_gif(std::filesystem::path path);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
};

direct_x* get_direct_x();