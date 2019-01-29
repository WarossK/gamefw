#pragma once

//zlib license. https://opensource.org/licenses/Zlib
//source by https://github.com/progschj/ThreadPool

#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

namespace gamefw
{
	class cond_wait_thread
	{
	private:
		std::unique_ptr<std::thread> thread_;
		std::queue<std::function<void()>> task_queue_;
		std::mutex queue_mutex_;
		std::condition_variable condition_;
		bool is_stop_;
		bool is_wait_;

	public:
		cond_wait_thread() :
			is_stop_(false)
		{
			is_wait_ = false;
			thread_.reset(new std::thread([&]
			{
				bool& is_wait = is_wait_;
				while (true)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(queue_mutex_);
						is_wait = true;
						condition_.wait(lock, [&] { return is_stop_ || !task_queue_.empty(); });
						is_wait = false;

						if (is_stop_ && task_queue_.empty()) return;

						task = std::move(task_queue_.front());
						task_queue_.pop();
					}

					task();
				}
			}));

		}

		~cond_wait_thread()
		{
			{
				std::unique_lock<std::mutex> lock(queue_mutex_);
				is_stop_ = true;
			}

			condition_.notify_all();
			thread_->join();
		}

		template<class F, class... Args>
		auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result <F, Args...>::type>
		{
			using return_type = typename std::result_of<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>
				(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

			std::future<return_type> result = task->get_future();

			{
				std::unique_lock<std::mutex> lock(queue_mutex_);

				if (is_stop_) throw std::runtime_error("enqueue on stopped thread.");

				task_queue_.emplace([task]() { (*task)(); });
			}

			condition_.notify_one();
			return result;
		}

		void WaitProcess()
		{
			while (!IsProcessComplete());
		}

		bool IsProcessComplete()
		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			return is_wait_;
		}
	};
}