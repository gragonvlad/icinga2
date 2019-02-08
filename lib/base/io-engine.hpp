/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2018 Icinga Development Team (https://icinga.com/)      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef IO_ENGINE_H
#define IO_ENGINE_H

/**
 * Boost.Coroutine2 (the successor of Boost.Coroutine)
 * (1) doesn't even exist in old Boost versions and
 * (2) isn't supported by ASIO, yet.
 */
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING 1

#include "base/lazy-init.hpp"
#include <atomic>
#include <memory>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/spawn.hpp>

/**
 * Scope lock for CPU-bound work done in an I/O thread
 *
 * @ingroup base
 */
class CpuBoundWork
{
public:
	CpuBoundWork(boost::asio::yield_context yc);
	CpuBoundWork(const CpuBoundWork&) = delete;
	CpuBoundWork(CpuBoundWork&&) = delete;
	CpuBoundWork& operator=(const CpuBoundWork&) = delete;
	CpuBoundWork& operator=(CpuBoundWork&&) = delete;
	~CpuBoundWork();
};

/**
 * Async I/O engine
 *
 * @ingroup base
 */
class IoEngine
{
	friend CpuBoundWork;

public:
	IoEngine(const IoEngine&) = delete;
	IoEngine(IoEngine&&) = delete;
	IoEngine& operator=(const IoEngine&) = delete;
	IoEngine& operator=(IoEngine&&) = delete;

	static IoEngine& Get();

	boost::asio::io_service& GetIoService();

private:
	IoEngine();

	void RunEventLoop();

	static LazyInit<std::unique_ptr<IoEngine>> m_Instance;

	boost::asio::io_service m_IoService;
	boost::asio::io_service::work m_KeepAlive;
	boost::asio::deadline_timer m_AlreadyExpiredTimer;
	std::atomic_uint_fast32_t m_CpuBoundSemaphore;
};

#endif /* IO_ENGINE_H */