/*
 * Plugin example for XnView 1.65 or more
 *
 * When Xuser is built, put it in PluginsEx folder
 */

#include <stdio.h>
#include <windows.h>

#include "blockcompress.h"

BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

#define API __stdcall

#define GFP_RGB		0
#define GFP_BGR		1

#define GFP_READ	0x0001
#define GFP_WRITE	0x0002

typedef struct {
	unsigned char red[256];
	unsigned char green[256];
	unsigned char blue[256];
} GFP_COLORMAP;

BOOL API gfpGetPluginInfo( DWORD version, LPSTR label, INT label_max_size, LPSTR extension, INT extension_max_size, INT* support )
{
	if ( version != 0x0002 ) {
		return FALSE;
	}

	strncpy_s( label, label_max_size, "Block Compression (BC)", _TRUNCATE );
	strncpy_s( extension, extension_max_size, "dds", _TRUNCATE );

	*support = GFP_READ/* | GFP_WRITE*/;

	return TRUE;
}

void* API gfpLoadPictureInit( LPCSTR filename )
{
	BCHandle handle = blockcompress_initialize( filename );
	if ( handle == NULL ) {
		return NULL;
	}
	return handle;
}

BOOL API gfpLoadPictureGetInfo( void* ptr, INT* pictype, INT* width, INT* height, INT* dpi, INT* bits_per_pixel, INT* bytes_per_line, BOOL* has_colormap, LPSTR label, INT label_max_size )
{
	*pictype = GFP_RGB;
	*width = blockcompress_width( ptr );
	*height = blockcompress_height( ptr );
	*dpi = blockcompress_dpi( ptr );
	*bits_per_pixel = blockcompress_bits_per_pixel( ptr );
	*bytes_per_line = blockcompress_bytes_per_line( ptr );
	*has_colormap = FALSE;
	strncpy_s( label, label_max_size, "Block Compression (BC)", _TRUNCATE );
	return TRUE;
}

BOOL API gfpLoadPictureGetLine( void* ptr, INT line, unsigned char* buffer )
{
	blockcompress_copy_scanline( ptr, buffer, line );
	return TRUE;
}

BOOL API gfpLoadPictureGetColormap( void* ptr, GFP_COLORMAP* cmap )
{
	return FALSE;
}

void API gfpLoadPictureExit( void* ptr )
{
	blockcompress_finalize( ptr );
}

// bits_per_pixel can be 1 to 8, 24, 32
BOOL API gfpSavePictureIsSupported( INT width, INT height, INT bits_per_pixel, BOOL has_colormap )
{
	return FALSE;
}

void* API gfpSavePictureInit( LPCSTR filename, INT width, INT height, INT bits_per_pixel, INT dpi, INT* picture_type, LPSTR label, INT label_max_size )
{
	return NULL;
}

BOOL API gfpSavePicturePutLine( void* ptr, INT line, const unsigned char* buffer )
{
	return FALSE;
}

void API gfpSavePictureExit( void* ptr )
{
}

