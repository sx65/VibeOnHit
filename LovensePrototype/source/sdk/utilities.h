#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace rbx::utils {
	template <typename T>
	bool is_valid_instance( const T& instance1, const T& instance2 ) {
		const auto address1 = instance1.get_address();

		if ( !address1 )
			return false;

		const auto address2 = instance2.get_address();

		if ( !address2 )
			return false;

		return address1 == address2;
	}

	template <typename T>
	bool update_instance( T& curr, T&& n ) {
		if ( !is_valid_instance( curr, n ) ) {
			curr = std::move( n );

			if ( !is_valid_instance( curr, n ) )
				return false;
		}

		return true;
	}

	template<typename T, typename Hash = std::hash<T>>
	void sync_lists( std::vector<T>& vector1, const std::vector<T>& vector2, bool dont_check_valid = false ) {
		std::unordered_set<T, Hash> target_set( vector2.begin(), vector2.end() );

		vector1.erase(
			std::remove_if( vector1.begin(), vector1.end(),
				[ &target_set ]( const T& element ) {
					return target_set.find( element ) == target_set.end();
				} ), vector1.end()
					);

		std::unordered_set<T, Hash> current_set( vector1.begin(), vector1.end() );

		for ( const T& element : vector2 ) {
			if ( current_set.find( element ) == current_set.end() ) {
				vector1.push_back( element );
				current_set.insert( element );
			}
		}

		// remove duplicates
		//std::sort( vector1.begin(), vector1.end() );
		//vector1.erase( std::unique( vector1.begin(), vector1.end() ), vector1.end() );
	}
}