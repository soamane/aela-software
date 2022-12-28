/*coded by https://github.com/soamane*/

#include "stdafx.hpp"
#include "cheat.h"

hooks::ivmt vmt_hook;
using prototype = long( __stdcall * )( IDXGISwapChain *, UINT, UINT );
prototype type{};

long __stdcall PresentScene( IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags ) {
	functions::Base( );
	return type( pSwapChain, SyncInterval, Flags );
}

void method( void *ptr, int index, void *function ) {
	type = *(decltype( type ) *)( *(uintptr_t *)ptr + index * sizeof( void * ) );
	vmt_hook.setup_hook( ptr );
	vmt_hook.hk_function( index, function );
}

unsigned __stdcall hooking( LPVOID lpParam ) {
//	CheckRegedit();
	auto hook_ptr = CSystemGlobalEnvironment::Instance( )->GetRenderer->GetSwapChain;
	method( hook_ptr, 8, PresentScene );
	return 0L;
}

BOOL WINAPI AELA( HMODULE handle, DWORD reason, LPVOID reseve ) {
	UNREFERENCED_PARAMETER( handle );
	UNREFERENCED_PARAMETER( reseve );

	switch ( reason ) {
		case DLL_PROCESS_ATTACH:

			hooking( reseve );
			break;

		case DLL_PROCESS_DETACH: break;
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
	}
	return TRUE;
}