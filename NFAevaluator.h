#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <set>
#include <string>

#include "NFA.h"

namespace nfa {

class ParallelNFAevaluator {
 public:
    ParallelNFAevaluator(Node* start, Node* end, std::set<std::string>* const output) : start(start), end(end), output(output) { }

    void evaluate();

    friend void threadLoop(ParallelNFAevaluator*);

 private:
    std::atomic_uint activeThreads;
    Node* start, *end;
    std::queue<State> activeStates;
    std::set<std::string>* const output;
    std::mutex stateLock, outputLock, syncLock;
    std::condition_variable syncVar;
};

}  // namespace nfa