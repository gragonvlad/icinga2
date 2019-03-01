/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <cstddef>
#include <functional>
#include <memory>
#include <thread>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace icinga
{

enum SchedulerPolicy
{
	DefaultScheduler,
	LowLatencyScheduler
};

/**
 * A thread pool.
 *
 * @ingroup base
 */
class ThreadPool
{
public:
	typedef std::function<void ()> WorkFunction;

	ThreadPool(size_t threads = std::thread::hardware_concurrency() * 2u);
	~ThreadPool();

	void Start();
	void Stop();

	bool Post(const WorkFunction& callback, SchedulerPolicy policy = DefaultScheduler);

private:
	boost::shared_mutex m_Mutex;
	std::unique_ptr<boost::asio::thread_pool> m_Pool;
	size_t m_Threads;
};

}

#endif /* EVENTQUEUE_H */
