#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <type_traits>
#include <concepts>
#include <stop_token>
#include <atomic>

namespace thread_pool {
	class thread_pool {
	public:
		explicit thread_pool( std::size_t threads = std::thread::hardware_concurrency() ) {
			for ( std::size_t i = 0; i < threads; ++i ) {
				workers.emplace_back( [ this ]( std::stop_token stoken ) {
					while ( !stoken.stop_requested() ) {
						std::function<void()> task;
						{
							std::unique_lock lock( queue_mutex );
							cond.wait( lock, stoken,
								[ this ] { return stop_requested() || !tasks.empty(); } );
							if ( stop_requested() && tasks.empty() )
								return;
							task = std::move( tasks.front() );
							tasks.pop();
						}
						task();
					}
					} );
			}
		}

		~thread_pool() {
			request_stop();
			cond.notify_all();
		}

		template<std::invocable F, typename... Args>
		auto enqueue( F&& f, Args&&... args ) {
			using return_type = std::invoke_result_t<F, Args...>;

			auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
				std::bind( std::forward<F>( f ), std::forward<Args>( args )... )
			);
			std::future<return_type> res = task_ptr->get_future();

			{
				std::scoped_lock lock( queue_mutex );
				if ( stop_requested() )
					throw std::runtime_error( "enqueue on stopped thread_pool" );
				tasks.emplace( [ task_ptr ]() { ( *task_ptr )( ); } );
			}

			cond.notify_one();
			return res;
		}

		void request_stop() noexcept {
			stop_flag.store( true, std::memory_order_relaxed );
		}

		bool stop_requested() const noexcept {
			return stop_flag.load( std::memory_order_relaxed );
		}

	private:
		std::vector<std::jthread> workers;
		std::queue<std::function<void()>> tasks;
		mutable std::mutex queue_mutex;
		std::condition_variable_any cond;
		std::atomic<bool> stop_flag{ false };
	};
}