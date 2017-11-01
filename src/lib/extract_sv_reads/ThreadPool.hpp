#pragma once

// TODO Check if hts.h is needed
#include <hts.h>
#include <thread_pool.h>

#include <stdexcept>

class ThreadPool {
    public:
        ThreadPool(int nthreads)
            : _thread_pool()
        {

            if (!(_thread_pool.pool = hts_tpool_init(nthreads))) {
                throw std::runtime_error("Failed to initialize thread pool");
            }
        
        }

        ~ThreadPool() {
            if (_thread_pool.pool) {
                hts_tpool_destroy(_thread_pool.pool);
            }
        }

        htsThreadPool* pool() {
            return &_thread_pool;
        }

    private:
        htsThreadPool _thread_pool;
};
