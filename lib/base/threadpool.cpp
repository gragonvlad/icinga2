/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#include "base/threadpool.hpp"
#include <boost/asio/post.hpp>
#include <boost/thread/locks.hpp>

using namespace icinga;

ThreadPool::ThreadPool(size_t threads)
	: m_Threads(threads)
{
	Start();
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Start()
{
	boost::unique_lock<decltype(m_Mutex)> lock (m_Mutex);

	if (!m_Pool) {
		m_Pool = decltype(m_Pool)(new boost::asio::thread_pool(m_Threads));
	}
}

void ThreadPool::Stop()
{
	boost::unique_lock<decltype(m_Mutex)> lock (m_Mutex);

	if (m_Pool) {
		m_Pool->join();
		m_Pool = nullptr;
	}
}

/**
 * Appends a work item to the work queue. Work items will be processed in FIFO order.
 *
 * @param callback The callback function for the work item.
 * @param policy The scheduling policy
 * @returns true if the item was queued, false otherwise.
 */
bool ThreadPool::Post(const ThreadPool::WorkFunction& callback, SchedulerPolicy policy)
{
	boost::shared_lock<decltype(m_Mutex)> lock (m_Mutex);

	if (m_Pool) {
		boost::asio::post(*m_Pool, callback);
		return true;
	} else {
		return false;
	}
}
