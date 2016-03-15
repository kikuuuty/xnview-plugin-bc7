#ifndef BLOCKCOMPRESS_H__
#define BLOCKCOMPRESS_H__
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* BCHandle;

	BCHandle blockcompress_initialize( const char* filename );
	void blockcompress_finalize( BCHandle handle );

	int32_t blockcompress_width( BCHandle handle );
	int32_t blockcompress_height( BCHandle handle );
	int32_t blockcompress_dpi( BCHandle handle );
	int32_t blockcompress_bits_per_pixel( BCHandle handle );
	int32_t blockcompress_bytes_per_line( BCHandle handle );

	void blockcompress_copy_scanline( BCHandle handle, unsigned char* dst, int32_t line );

#ifdef __cplusplus
}
#endif

#endif
