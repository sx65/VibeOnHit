#include <Windows.h>
#include <TlHelp32.h>

#include "memory.h"
#include <vector>
#include <sstream>
#include <cstring>

static std::wstring ToWide( const char* str ) {
	if ( !str || !*str ) return {};
	int len = MultiByteToWideChar( CP_ACP, 0, str, -1, nullptr, 0 );
	if ( len <= 0 ) return {};
	std::wstring out( len, 0 );
	MultiByteToWideChar( CP_ACP, 0, str, -1, &out[ 0 ], len );
	return out;
}

std::uint64_t CMemory::get_process_id( const char* process_name ) {
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof( entry );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( !snapshot || snapshot == INVALID_HANDLE_VALUE )
		return 0;

	std::uint64_t pid = 0;
	std::wstring target = ToWide( process_name );

	if ( Process32FirstW( snapshot, &entry ) ) {
		do {
			if ( _wcsicmp( entry.szExeFile, target.c_str() ) == 0 ) {
				pid = entry.th32ProcessID;
				break;
			}
		} while ( Process32NextW( snapshot, &entry ) );
	}

	CloseHandle( snapshot );
	return pid;
}

std::uint64_t CMemory::get_base_addr( const char* process_name ) {
	MODULEENTRY32W entry;
	entry.dwSize = sizeof( entry );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, ( DWORD )process_id );
	if ( !snapshot || snapshot == INVALID_HANDLE_VALUE )
		return 0;

	std::uint64_t base = 0;
	std::wstring target = ToWide( process_name );

	if ( Module32FirstW( snapshot, &entry ) ) {
		do {
			if ( _wcsicmp( entry.szModule, target.c_str() ) == 0 ) {
				base = reinterpret_cast< std::uint64_t >( entry.modBaseAddr );
				break;
			}
		} while ( Module32NextW( snapshot, &entry ) );
	}

	CloseHandle( snapshot );
	return base;
}

HWND CMemory::get_window_handle( const char* window_class_name ) {
	return FindWindowA( window_class_name, NULL );
}

bool CMemory::is_valid_addr( const std::uint64_t& address ) const {
	return address >= 0x10000 && address <= 0x7FFFFFFFFFFF;
}

bool CMemory::read_buffer( const std::uint64_t& address, void* buffer, size_t size ) const {
	if ( !is_valid_addr( address ) || ( !process_handle || process_handle == INVALID_HANDLE_VALUE ) )
		return false;

	return ReadProcessMemory( process_handle, reinterpret_cast< LPCVOID >( address ), buffer, size, nullptr );
}

bool CMemory::init( void ) {
	process_id = get_process_id( process_name );

	if ( !process_id )
		return false;

	base_address = get_base_addr( process_name );

	if ( !base_address )
		return false;

	window_handle = get_window_handle( window_class_name );

	if ( !window_handle || window_handle == INVALID_HANDLE_VALUE )
		return false;

	return true;
}

bool CMemory::attach( void ) {
	if ( !process_id )
		return false;

	if ( process_handle && process_handle != INVALID_HANDLE_VALUE )
		return true;

	process_handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, ( DWORD )process_id );

	return process_handle && process_handle != INVALID_HANDLE_VALUE;
}

void CMemory::detach( void ) {
	if ( process_handle && process_handle != INVALID_HANDLE_VALUE ) {
		CloseHandle( process_handle );
		process_handle = nullptr;
	}
}

std::uint64_t CMemory::get_process_id( void ) {
	return process_id;
}

std::uint64_t CMemory::get_base_addr( void ) {
	return base_address;
}

HWND CMemory::get_window_handle( void ) {
	return window_handle;
}

HANDLE CMemory::get_process_handle( void ) {
	return process_handle;
}

void CMemory::set_process_id( std::uint64_t process_id ) {
	this->process_id = process_id;
}

void CMemory::set_base_addr( std::uint64_t base_address ) {
	this->base_address = base_address;
}

void CMemory::set_window_handle( HWND window_handle ) {
	this->window_handle = window_handle;
}

std::vector<BYTE> ParsePattern( const std::string& pattern_string ) {
	std::vector<BYTE> bytes;
	std::stringstream ss( pattern_string );
	std::string byte_str;

	while ( ss >> byte_str ) {
		if ( byte_str == "?" || byte_str == "??" ) bytes.push_back( 0x00 ); // wildcard
		else bytes.push_back( static_cast< BYTE >( std::stoul( byte_str, nullptr, 16 ) ) );
	}

	return bytes;
}

bool ComparePattern( const BYTE* data, const std::vector<BYTE>& pattern ) {
	for ( std::size_t i = 0; i < pattern.size(); ++i )
		if ( pattern[ i ] != 0x00 && data[ i ] != pattern[ i ] )
			return false;

	return true;
}

std::uint64_t CMemory::find_pattern( const std::string& pattern_string ) {
	if ( !process_handle || process_handle == INVALID_HANDLE_VALUE )
		return 0;

	std::vector<BYTE> pattern = ParsePattern( pattern_string );
	const std::size_t pattern_size = pattern.size();

	SYSTEM_INFO system_info;
	GetSystemInfo( &system_info );

	std::uint64_t start_address = reinterpret_cast< std::uint64_t >( system_info.lpMinimumApplicationAddress );
	std::uint64_t end_address = reinterpret_cast< std::uint64_t >( system_info.lpMaximumApplicationAddress );

	MEMORY_BASIC_INFORMATION mbi;
	std::vector<BYTE> buffer;

	for ( std::uint64_t address = start_address; address < end_address; address += mbi.RegionSize ) {
		if ( !VirtualQueryEx( process_handle, reinterpret_cast< LPCVOID >( address ), &mbi, sizeof( mbi ) ) ) continue;
		if ( mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS || mbi.Protect & PAGE_GUARD ) continue;

		buffer.resize( mbi.RegionSize );
		SIZE_T bytes_read = 0;

		if ( ReadProcessMemory( process_handle, reinterpret_cast< LPCVOID >( address ), buffer.data(), mbi.RegionSize, &bytes_read ) )
			for ( SIZE_T i = 0; i < bytes_read - pattern_size; i++ )
				if ( ComparePattern( &buffer[ i ], pattern ) ) return address + i;
	}

	return 0;
}

std::string CMemory::read_string( const std::uint64_t& address, int length ) {
	if ( !is_valid_addr( address ) || length <= 0 || length > 200 )
		return std::string();

	std::vector<char> buffer( length + 1 );

	if ( !read_buffer( address, buffer.data(), length ) )
		return std::string();

	buffer[ length ] = '\0';

	return std::string( buffer.data() );
}

CMemory memory;