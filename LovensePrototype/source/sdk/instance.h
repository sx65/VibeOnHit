#pragma once

#include <memory/memory.h>
#include <string/string.h>
#include <vector>
#include <functional>
#include <print>

#include "base.h"
#include "offsets.h"

namespace rbx {
	template <typename T>
	struct child_entry {
		T instance;
		std::string name;
	};

	template <typename T>
	inline bool operator==( const child_entry<T>& a, const child_entry<T>& b ) {
		return a.instance == b.instance && a.name == b.name;
	}

	class instance : public rbx::base {
	public:

		template <typename T>
		T get_parent( void ) const noexcept {
			const auto& address = get_address();
			
			if ( address <= 0 )
				return T();

			return memory.read<T>( address + Offsets::instance::Parent );
		}

		template <typename T>
		std::vector<T> get_children( void ) const noexcept {
			std::vector<T> container;

			const auto address = get_address();
			if ( !address )
				return container;

			const auto children_ptr = memory.read<std::uint64_t>( address + Offsets::instance::Children );
			if ( !children_ptr )
				return container;

			const auto instance_start = memory.read<std::uint64_t>( children_ptr );
			const auto instance_end = memory.read<std::uint64_t>( children_ptr + Offsets::Core::Self );

			if ( !instance_start || !instance_end || instance_end <= instance_start )
				return container;

			const std::uint64_t size = instance_end - instance_start;

			std::vector<std::uint8_t> buffer( size );

			if ( !memory.read_buffer( instance_start, buffer.data(), size ) )
				return container;

			const std::size_t count = size / 0x10;

			for ( std::size_t i = 0; i < size; i += 0x10 ) {
				T obj = *reinterpret_cast< T* >( &buffer[ i ] );

				if ( obj.get_address() != 0 )
					container.push_back( std::move( obj ) );
			}

			return container;
		}

		template <typename T>
		std::vector<T> get_descendants( void ) const noexcept {
			std::vector<T> container;

			const auto address = get_address();
			if ( !address )
				return container;

			std::function<void( rbx::instance )> collect_descendants = [ & ]( rbx::instance instance ) {
				auto children = instance.get_children<T>();

				for ( const auto& child : children ) {
					if ( child.get_address() )
						container.push_back( child );

					collect_descendants( child );
				}
			};

			collect_descendants( *static_cast< const rbx::instance* >( this ) );

			return container;
		}

		template <typename T>
		void get_children_into( std::vector<T>& out ) const noexcept {
			out.clear();

			const auto address = get_address();
			if ( !address )
				return;

			const auto children_ptr = memory.read<std::uint64_t>( address + Offsets::instance::Children );
			if ( !children_ptr )
				return;

			const auto instance_start = memory.read<std::uint64_t>( children_ptr );
			const auto instance_end = memory.read<std::uint64_t>( children_ptr + Offsets::Core::Self );

			if ( !instance_start || !instance_end || instance_end <= instance_start )
				return;

			const std::uint64_t size = instance_end - instance_start;

			std::vector<std::uint8_t> buffer( size );

			if ( !memory.read_buffer( instance_start, buffer.data(), size ) )
				return;

			const std::size_t count = size / 0x10;

			for ( std::size_t i = 0; i < size; i += 0x10 ) {
				T obj = *reinterpret_cast< T* >( &buffer[ i ] );
				
				if ( obj.get_address() != 0 )
					out.push_back( std::move( obj ) );
			}
		}

		template <typename T>
		std::vector<child_entry<T>> get_children_entries( void ) const noexcept {
			std::vector<child_entry<T>> child_entries;

			const auto children = get_children<T>();

			if ( children.empty() )
				return child_entries;

			for ( auto& child : children ) {
				if ( !child.is_valid() )
					continue;

				child_entry<T> entry;
				entry.instance = child;
				entry.name = child.get_name();

				child_entries.push_back( entry );
			}

			return child_entries;
		}

		template <typename T>
		T find_child_by_name( const char* name, const std::vector<T>& children ) const noexcept {
			for ( const auto& child : children ) {
				if ( child.get_name() == name )
					return child;
			}

			return T();
		}

		template <typename T>
		T find_child_by_name( const char* name ) const noexcept {
			return find_child_by_name<T>( name, get_children<T>() );
		}

		template <typename T>
		T find_child_by_class( const char* class_name, const std::vector<T>& children ) const noexcept {
			for ( const auto& child : children ) {
				if ( child.get_class_name() == class_name )
					return child;
			}

			return T();
		}

		template <typename T>
		T find_child_by_class( const char* class_name ) const noexcept {
			return find_child_by_class<T>( class_name, get_children<T>() );
		}

		std::string get_name( void ) const noexcept;
		std::string get_class_name( void ) const noexcept;
		std::string get_full_path( void ) const noexcept;
	};
}

namespace std {
	template<>
	struct hash<rbx::instance> {
		size_t operator()( const rbx::instance& instance ) const {
			return hash<string>()( "instance" ) ^ ( hash<int>()( instance.address ) << 1 );
		}
	};
}