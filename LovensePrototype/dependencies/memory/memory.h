#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string>

class CMemory final {
private:
	HANDLE process_handle = 0;
	HWND window_handle = 0;
	std::uint64_t process_id = 0;
	std::uint64_t base_address = 0;

public:
	bool init( void );
	bool attach( void );
	void detach( void );

	std::uint64_t get_process_id( const char* process_name );
	std::uint64_t get_base_addr( const char* process_name );
	HWND get_window_handle( const char* window_class_name );

	std::uint64_t get_process_id( void );
	std::uint64_t get_base_addr( void );
	HWND get_window_handle( void );
	HANDLE get_process_handle( void );

	void set_process_id( std::uint64_t process_id );
	void set_base_addr( std::uint64_t base_address );
	void set_window_handle( HWND window_handle );

	bool is_valid_addr( const std::uint64_t& address ) const;

	bool read_buffer( const std::uint64_t& address, void* buffer, size_t size ) const;

	template <typename T>
	T read( const std::uint64_t& address ) const {
		T value{};

		if ( !is_valid_addr( address ) || !process_handle || process_handle == INVALID_HANDLE_VALUE )
			return value;

		ReadProcessMemory( process_handle, reinterpret_cast< LPCVOID >( address ), &value, sizeof( T ), nullptr );

		return value;
	}

	template <typename T>
	bool write( const std::uint64_t& address, const T& value ) const {
		if ( !is_valid_addr( address ) || ( !process_handle || process_handle == INVALID_HANDLE_VALUE ) )
			return false;

		return WriteProcessMemory( process_handle, reinterpret_cast< LPVOID >( address ), &value, sizeof( T ), nullptr );
	}

	std::uint64_t find_pattern( const std::string& pattern_string );

	std::string read_string( const std::uint64_t& address, int length );

	const char* process_name = "RobloxPlayerBeta.exe";
	const char* window_class_name = "WINDOWSCLIENT";
};

extern CMemory memory;