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
#include "ThreadEngine.h"
#include "log.h"

// Create a thread local queue for each thread
// This is what allows us to submit functions asynchronously
// then submit them to the global job queue
#ifndef _WIN32
thread_local functions_t threadQueue;
#else
__declspec(thread) functions_t* threadQueue;
#endif

// A mutex to avoid a race condition when checking the queue.
std::mutex queueLock;

ThreadHandler::ThreadHandler() : totalConcurrentThreads(0)
{
	// NOTE: Windows thread_local stuff is not complete currently, we must work around
	// this with pointers. This will initialize a queue for the main thread.
	// Each thread will also have their own queues.
#ifdef _WIN32
	functions_t *queue = new functions_t;
	threadQueue = queue;
#endif
}

ThreadHandler::~ThreadHandler()
{
	this->Shutdown();
#ifdef _WIN32
	delete threadQueue;
#endif
}

void ThreadHandler::Initialize()
{
	Log(LOG_THREAD) << "Thread engine initializing";
	this->funcs = functions_t();
	this->totalConcurrentThreads = std::thread::hardware_concurrency();
	
	// We require at least 1 thread, if somehow C++ fails to epic porportions
	// we won't fail to run the game.
	unsigned int spawnthrds = 1;
	
	// Some older single-core CPUs will have trouble running the game
	// and rather than just crashing for the user or telling them
	// "Sorry, your computer is too shitty for me.", we'll let them
	// run the game with awful performance.
	if (this->totalConcurrentThreads <= 1)
	{
		Log(LOG_THREAD) << "Seems this CPU is a bit slow, only spawning 2 threads!";
		spawnthrds = this->totalConcurrentThreads = 2;
	}
	else // Otherwise just multiply the threads by 2, I suspect many will idle so this is fine.
	{
		spawnthrds = this->totalConcurrentThreads * 2;
	}
	
	Log(LOG_THREAD) << "Total supported threads: " << this->totalConcurrentThreads;
	
	// Spawn the threads needed.
	for (unsigned i = 0; i < spawnthrds; ++i)
		new WorkerThread(i, this);
	
	Log(LOG_THREAD) << "Spawned " << this->Threads.size() << " threads"; 
}

void ThreadHandler::Shutdown()
{
	Log(LOG_THREAD) << "Thread engine shutting down...";
	// Wake all threads, make this->funcs read only, finish work, shutdown threads
	this->JoinThreads();
	// We avoid a race condition because all threads have been joined at this point.
	Log(LOG_THREAD) << this->funcs.size() << "jobs left and will not be processed.";

	for (auto it = this->Threads.begin(), it_end = this->Threads.end(); it != it_end;)
		delete *(it++);
}

void ThreadHandler::JoinThreads()
{
	for (const auto &t : this->Threads)
		t->quitting = true;

	this->WakeThreads();

	for (const auto &t : this->Threads)
		t->Join();
}

void ThreadHandler::WakeThreads()
{
	for (const auto &t : this->Threads)
		t->Wake();
}

bool ThreadHandler::Submit(bool noStall)
{
	// Acquire a lock. if noStall, return -1 so the function can continue, otherwise wait until the lock is acquired
	if (noStall)
	{
		if (!queueLock.try_lock())
			return false;
	}
	else
		queueLock.lock();

#ifndef _WIN32
	// Submit the functions pending
	while (!threadQueue.empty())
	{
		// Remove function from thread local stack
		function_t func = threadQueue.front();
		threadQueue.pop();

		// Move to global stack so threads can pick what is needed
		this->funcs.push(func);
	}
#else
	// Submit the functions pending
	while (!threadQueue->empty())
	{
		// Remove function from thread local stack
		function_t func = threadQueue->front();
		threadQueue->pop();

		// Move to global stack so threads can pick what is needed
		this->funcs.push(func);
	}
#endif

	// release our lock
	queueLock.unlock();

	// Notify the threads that there is work
	this->WakeThreads();

	return true;
}

/******************************************************************/


WorkerThread::WorkerThread(int ThreadID, ThreadHandler *thr) : wakeThread(false),
thr(thr), threadID(ThreadID), quitting(false)
{
	Log(LOG_THREAD) << "Thread ID: " << ThreadID;
	// NOTICE: We must wait for the class to initialize before we can start our thread
	// otherwise we get uninitialized values which can cause a race condition
	// and undefined behavior when the class 
	this->th = std::thread(&WorkerThread::Main, this);
	// Add the thread to the thread list
	thr->Threads.push_back(this);
}

WorkerThread::~WorkerThread()
{
	// Remove the thread from the thread list
	this->thr->Threads.remove(this);
}

void WorkerThread::Sleep()
{
	std::unique_lock<std::mutex> lk(this->m);
	this->cv.wait(lk, [this] (){return this->wakeThread; });
	this->wakeThread = false;
}

void WorkerThread::Wake()
{
	wakeThread = true;
	this->cv.notify_all();
}

void WorkerThread::Join()
{
	Log(LOG_THREAD) << "Thread " << this->threadID << " joining to next thread...";
	this->quitting = true;
	this->th.join();
}

void WorkerThread::Main()
{
	//this->Sleep();
#ifdef _WIN32
	functions_t *queue = new functions_t;
	threadQueue = queue;
#endif
	while (!this->quitting)
	{
		// Acquire a lock on the queue and get the data
		queueLock.lock();

		// Check the funcs are in the queue and ready to be processed
		if (!this->thr->funcs.empty())
		{
			// Get a local copy of the function
			function_t func = this->thr->funcs.front();
			this->thr->funcs.pop();

			// Release our lock so other threads can use it before we run our function.
			queueLock.unlock();

			// Run the function (we released the lock in case the function takes a long time to run)
			func();
		}
		else
		{
			queueLock.unlock();
			this->Sleep();
		}
	}

#ifdef _WIN32
	// Clean up our shit.
	delete queue;
#endif

	Log(LOG_THREAD) << "Thread " << this->threadID << " exiting..."; 
}
