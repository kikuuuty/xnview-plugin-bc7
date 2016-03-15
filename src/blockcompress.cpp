#include <cstdint>
#include <string>
#include "blockcompress.h"
#include "DirectXTex.h"

namespace {

	std::wstring utf8_to_unicode( const std::string& str )
	{
		std::wstring dst;
		int32_t length = ::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, NULL, 0 );
		if ( length <= 0 ) {
			return std::wstring();
		}
		dst.resize( length - 1 );
		::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, &dst[0], length );
		return dst;
	}

	std::string unicode_to_utf8( const std::wstring& str )
	{
		std::string dst;
		int32_t length = ::WideCharToMultiByte( CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL );
		if ( length <= 0 ) {
			return std::string();
		}
		dst.resize( length - 1 );
		::WideCharToMultiByte( CP_UTF8, 0, str.c_str(), -1, &dst[0], length, NULL, NULL );
		return dst;
	}

}

struct blockcompress_t
{
	DirectX::ScratchImage image;
};

BCHandle blockcompress_initialize( const char* filename )
{
	blockcompress_t* bc = new blockcompress_t;
	if ( bc == nullptr ) {
		return nullptr;
	}

	DirectX::ScratchImage bc_image;
	DirectX::TexMetadata texinfo;
	HRESULT hr = DirectX::LoadFromDDSFile( utf8_to_unicode( filename ).c_str(),
					DirectX::DDS_FLAGS_NONE, &texinfo, bc_image );
	if ( FAILED(hr) ) {
		goto failed_initialize;
	}

	hr = DirectX::Decompress( bc_image.GetImages(), bc_image.GetImageCount(), bc_image.GetMetadata(),
			DXGI_FORMAT_R8G8B8A8_UNORM, bc->image );
	if ( FAILED(hr) ) {
		goto failed_initialize;
	}
	return bc;

failed_initialize:
	delete bc;
	return nullptr;
}

void blockcompress_finalize( BCHandle handle )
{
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		delete bc;
	}
}

int32_t blockcompress_width( BCHandle handle )
{
	int32_t width = 0;
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		width = static_cast<int32_t>(bc->image.GetMetadata().width);
	}
	return width;
}

int32_t blockcompress_height( BCHandle handle )
{
	int32_t height = 0;
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		height = static_cast<int32_t>(bc->image.GetMetadata().height);
	}
	return height;
}

int32_t blockcompress_dpi( BCHandle handle )
{
	return 68;
}

int32_t blockcompress_bits_per_pixel( BCHandle handle )
{
	int32_t bits_per_pixel = 0;
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		bits_per_pixel = 4 << 3;
	}
	return bits_per_pixel;
}

int32_t blockcompress_bytes_per_line( BCHandle handle )
{
	int32_t bytes_per_line = 0;
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		bytes_per_line = static_cast<int32_t>(bc->image.GetImages()->rowPitch);
	}
	return bytes_per_line;
}

void blockcompress_copy_scanline( BCHandle handle, unsigned char* dst, int32_t line )
{
	if ( handle ) {
		blockcompress_t* bc = reinterpret_cast<blockcompress_t*>(handle);
		size_t bytes_per_line = static_cast<int32_t>(bc->image.GetImages()[0].rowPitch);
		const void* src = bc->image.GetImages()[0].pixels + (bytes_per_line * line);
		memcpy( dst, src, bytes_per_line );
	}
}
