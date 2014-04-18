/*
 * Copyright (c) 2013-2014, Justin Crawford <Justasic@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <list>
#include <condition_variable>

typedef std::queue<std::function<void ()>> functions_t;
typedef std::function<void()> function_t;

class ThreadHandler;

class WorkerThread
{
	// Mutex used for sleeping the thread
	std::mutex m;
	std::condition_variable cv;
	bool wakeThread;
	std::thread th; // Inherit std::thread instead?
	ThreadHandler *thr;
protected:
	// Thread ID
	int threadID;
public:
	WorkerThread(int, ThreadHandler*);
	~WorkerThread();

	friend class ThreadHandler;

	// Put the thread into an idle state
	void Sleep();
	// Wake the thread from idle state
	void Wake();
	// Execute jobs - This is called by the Job Handler directly.
	void Main();
	// Join the thread to the main thread (used for shutdown)
	void Join();

	// Get the thread ID
	inline int GetID() const { return this->threadID; }

	// Check whether the thread is quitting
	std::atomic<bool> quitting;
};

class ThreadHandler
{
protected:
	void JoinThreads();
	void WakeThreads();
	std::list<WorkerThread*> Threads;
public:
	ThreadHandler();
	~ThreadHandler();

	// Let the threads access members in this class
	friend class WorkerThread;

	// Waiting jobs, This is global among all threads.
	functions_t funcs;

	// Defines how many threads the system is designed to use
	unsigned int totalConcurrentThreads;

	// This makes it easy to specify functions and arguments
	// in a readable manner
	template< class _Function, class... _Args >
	void AddQueue(_Function&& __f, _Args&&... __args)
	{
		// Create a thread local queue for each thread
		// This is what allows us to submit functions asynchronously
		// then submit them to the global job queue

		// Make a bound std::function for storage
		function_t func = std::bind(std::forward<_Function>(__f), std::forward<_Args>(__args)...);
		// Push to thread local storage container
#ifndef _WIN32
		extern thread_local functions_t threadQueue;
		threadQueue.push(func);
#else
		extern __declspec(thread) functions_t* threadQueue;
		threadQueue->push(func);
#endif
	}

	bool Submit(bool noStall = false);

	void Initialize();
	void Shutdown();
};
