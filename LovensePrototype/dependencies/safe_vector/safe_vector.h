#pragma once

#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>

namespace safe {
	namespace detail {
		template<typename T, typename Hash = std::hash<T>>
		inline void sync_lists(std::vector<T>& v1, const std::vector<T>& v2, bool dont_check_valid = false) {
			(void)dont_check_valid;
			std::unordered_set<T, Hash> target(v2.begin(), v2.end());
			v1.erase(std::remove_if(v1.begin(), v1.end(),
				[&target](const T& e) { return target.find(e) == target.end(); }), v1.end());
			std::unordered_set<T, Hash> current(v1.begin(), v1.end());
			for (const T& e : v2) {
				if (current.find(e) == current.end()) {
					v1.push_back(e);
					current.insert(e);
				}
			}
		}
	}

	template<typename T>
	class vector {
	private:
		std::atomic<std::shared_ptr<std::vector<T>>> list_ptr;

		std::mutex write_mutex;

	public:
		vector() {
			list_ptr.store( std::make_shared<std::vector<T>>() );
		}

		std::shared_ptr<std::vector<T>> get_snapshot() const {
			return list_ptr.load();
		}

		void update( const std::vector<T>& new_items, bool dont_check_valid = false ) {
			std::lock_guard<std::mutex> lock( write_mutex );

			auto new_list = std::make_shared<std::vector<T>>( *list_ptr.load() );
			detail::sync_lists( *new_list, new_items, dont_check_valid );
			list_ptr.store( new_list );
		}

		template <typename init_func>
		void update_and_init( std::vector<T>& new_items, init_func initializer, bool dont_check_valid = false ) {
			std::lock_guard<std::mutex> lock( write_mutex );

			std::vector<T> working_list;

			auto current_list = list_ptr.load();

			working_list = *current_list;
			detail::sync_lists( working_list, new_items, dont_check_valid );

			for ( auto& player : working_list )
				initializer( player );

			auto new_list = std::make_shared<std::vector<T>>( std::move( working_list ) );
			list_ptr.store( new_list );
		}

		template <typename update_func>
		void update_data( update_func updater ) {
			std::lock_guard<std::mutex> lock( write_mutex );

			auto current = list_ptr.load();
			auto new_list = std::make_shared<std::vector<T>>( *current );

			for ( auto& player : *new_list )
				updater( player );

			list_ptr.store( new_list );
		}

		void replace( const std::vector<T>& new_items ) {
			std::lock_guard<std::mutex> lock( write_mutex );

			auto new_list = std::make_shared<std::vector<T>>( new_items );
			list_ptr.store( new_list );
		}

		void clear() {
			std::lock_guard<std::mutex> lock( write_mutex );

			auto new_list = std::make_shared<std::vector<T>>();
			list_ptr.store( new_list );
		}

		T front() {
			return ( *list_ptr.load() ).front();
		}
	};
}