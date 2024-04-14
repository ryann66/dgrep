#include <thread>
#include <vector>
#include <chrono>

#include "NFAevaluator.h"
#include "truncation_error.h"

using std::thread;
using std::vector;
using std::stack;
using std::unique_lock;
using std::mutex;
using std::chrono::milliseconds;
using std::chrono::duration;

using metastring::Metastring;

namespace nfa {

void threadLoop(ParallelNFAevaluator* eval) {
    unique_lock<mutex> lock(eval->syncLock);
    while (eval->activeThreads != 0 || !eval->activeStates.empty()) {
        eval->stateLock.lock();
        if (!eval->activeStates.empty()) {
            eval->activeThreads++;

            // get state
            State s(eval->activeStates.top());
            eval->activeStates.pop();
            eval->stateLock.unlock();

            // check for end state
            if (s.state == eval->end) {
                // add string to out
                eval->outputLock.lock();
                eval->output->insert(s.string.toString());
                eval->outputLock.unlock();
            } else {
                // evaluate state
                stack<State> newStates;
                for (Edge* out : s.state->outgoing) {
                    try {
                        out->traverse(s, newStates);
                    } catch (truncation_error*) { }
                }

                if (!newStates.empty()) {
                    // add states to state queue
                    eval->stateLock.lock();
                    do {
                        eval->activeStates.push(newStates.top());
                        newStates.pop();
                    } while (!newStates.empty());
                    eval->stateLock.unlock();
                }
                
            }

            eval->activeThreads--;
            eval->syncVar.notify_all();
        } else {
            eval->stateLock.unlock();
            
            // wait for a thread to terminate
            duration d(milliseconds(1));
            eval->syncVar.wait_for(lock, d);
        }
    }
}

void ParallelNFAevaluator::evaluate() {
    // setup background
    unsigned int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 8;
    }
    activeThreads = 0;
    Metastring m("");
    activeStates.emplace(start, m);
    
    // launch threads
    vector<thread> threads;
    threads.reserve(numThreads - 1);
    for (unsigned int i = 1; i < numThreads; i++)
        threads.emplace_back(threadLoop, this);
    threadLoop(this);
    
    // cleanup
    for (thread& t : threads) t.join();
}

}  // namespace nfa