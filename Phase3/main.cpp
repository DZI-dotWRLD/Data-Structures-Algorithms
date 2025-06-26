//phase 3 Ishchanka

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <random>

// Job class definition
class Job {
public:
    std::string jobID;
    char jobClass;
    int priority;
    double runtime;     // in seconds
    double startTime;   // in seconds
    double endTime;     // in seconds
    double elapsedTime; // in seconds

    Job()
        : jobID(""), jobClass(' '), priority(0), runtime(0),
        startTime(-1), endTime(-1), elapsedTime(-1) {}

    Job(const std::string& id, char cls, int prio)
        : jobID(id), jobClass(cls), priority(prio),
        runtime(15 + rand() % 11), // Random runtime between 15 and 25 seconds
        startTime(-1), endTime(-1), elapsedTime(-1) {}

    bool operator<(const Job& other) const {
        // Higher priority jobs should come first
        return priority < other.priority;
    }

    void resetTimes() {
        startTime = -1;
        endTime = -1;
        elapsedTime = -1;
    }
};

// Global variables and synchronization primitives
std::mutex queueMutex;
std::mutex processorMutex;
std::condition_variable cv;
std::atomic<bool> keepRunning(true);
int jobCounter = 1; // Start from 1

// Function to populate input queue from a file
std::queue<Job> populateInputQueue(const std::string& filename) {
    std::queue<Job> inputQueue;
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return inputQueue;
    }

    std::getline(file, line); // Skip header line if present
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string jobID;
        char jobClass;
        int priority;
        iss >> jobID >> jobClass >> priority;

        if (!iss.fail()) {
            Job job(jobID, jobClass, priority);
            inputQueue.push(job);
        }
    }
    return inputQueue;
}

// Function to handle user input in a separate thread
void handleUserInput(std::queue<Job>& inputQueue) {
    while (keepRunning) {
        std::string line;
        char jobClass;
        int priority;
        std::cout << "Enter job class (A-Z) or 'Q' to quit adding jobs: ";
        std::cin >> jobClass;
        if (jobClass == 'Q' || jobClass == 'q') {
            keepRunning = false;
            cv.notify_all(); // Notify other threads
            break;
        }

        std::cout << "Enter job priority (higher value = higher priority): ";
        std::cin >> priority;

        if (std::cin.fail() || priority <= 0) {
            std::cerr << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            std::ostringstream jobID;
            jobID << "J" << std::setw(3) << std::setfill('0') << jobCounter++;
            Job newJob(jobID.str(), jobClass, priority);
            inputQueue.push(newJob);
            std::cout << "Added new job: " << newJob.jobID << " | Class: " << newJob.jobClass
                << " | Priority: " << newJob.priority << "\n";
        }
        cv.notify_all(); // Notify other threads that new job is added
    }
}

// Function to move jobs from input queue to class queues
void moveJobsToClassQueues(std::queue<Job>& inputQueue,
    std::unordered_map<char, std::priority_queue<Job>>& jobClassQueues) {
    while (keepRunning) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&]() { return !inputQueue.empty() || !keepRunning; });

        if (!keepRunning && inputQueue.empty()) break;

        while (!inputQueue.empty()) {
            Job job = inputQueue.front();
            inputQueue.pop();
            jobClassQueues[job.jobClass].push(job);
            std::cout << "Moved job " << job.jobID << " to class queue " << job.jobClass << "\n";
        }
        lock.unlock();
        cv.notify_all(); // Notify dispatch thread that jobs are available
    }
}

// Function to dispatch jobs to processors and handle job execution
void dispatchJobs(std::unordered_map<char, std::priority_queue<Job>>& jobClassQueues,
    const std::unordered_map<char, int>& processors,
    std::vector<Job>& outputQueue) {
    std::unordered_map<char, std::vector<Job>> processorStatus;
    double currentTime = 0.0;

    // Initialize processor status
    for (const auto& p : processors) {
        processorStatus[p.first] = std::vector<Job>(p.second, Job());
    }

    while (keepRunning) {
        std::unique_lock<std::mutex> lock(processorMutex);

        // Check if all queues and processors are empty
        bool allQueuesEmpty = true;
        for (const auto& entry : jobClassQueues) {
            if (!entry.second.empty()) {
                allQueuesEmpty = false;
                break;
            }
        }
        if (allQueuesEmpty) {
            bool processorsBusy = false;
            for (const auto& entry : processorStatus) {
                for (const auto& job : entry.second) {
                    if (!job.jobID.empty()) {
                        processorsBusy = true;
                        break;
                    }
                }
                if (processorsBusy) break;
            }
            if (!processorsBusy && !keepRunning) {
                break;
            }
        }

        // Dispatch jobs to processors
        for (auto& entry : processorStatus) {
            char classType = entry.first;
            for (size_t i = 0; i < entry.second.size(); ++i) {
                if (entry.second[i].jobID.empty()) {
                    std::lock_guard<std::mutex> queueLock(queueMutex);
                    if (!jobClassQueues[classType].empty()) {
                        Job job = jobClassQueues[classType].top();
                        jobClassQueues[classType].pop();
                        job.startTime = currentTime;
                        job.endTime = currentTime + job.runtime;
                        entry.second[i] = job;
                        std::cout << "Dispatching job: " << job.jobID << " | Class: " << job.jobClass
                            << " to Processor " << classType << i << "\n";
                    }
                }
            }
        }

        // Process jobs
        for (auto& entry : processorStatus) {
            for (auto& job : entry.second) {
                if (!job.jobID.empty()) {
                    // Check if job has completed
                    if (job.endTime <= currentTime) {
                        job.elapsedTime = job.endTime - job.startTime;
                        outputQueue.push_back(job);
                        std::cout << "Completed job: " << job.jobID
                            << " | Start Time: " << job.startTime
                            << " | End Time: " << job.endTime
                            << " | Runtime: " << job.runtime << "s\n";
                        job = Job(); // Reset processor slot
                    }
                }
            }
        }

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate time passing
        currentTime += 0.1; // Increment current time by 0.1 seconds
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    std::queue<Job> inputQueue = populateInputQueue("jobs.txt");

    // Output jobs read from the file
    std::cout << "Jobs read from file:\n";
    std::queue<Job> tempQueue = inputQueue; // Temporary copy to preserve the inputQueue
    int fileJobCount = 0; // Counter for the number of jobs in the file
    while (!tempQueue.empty()) {
        const Job& job = tempQueue.front();
        std::cout << "Job ID: " << job.jobID
            << " | Class: " << job.jobClass
            << " | Priority: " << job.priority
            << " | Runtime: " << job.runtime << " seconds\n";
        tempQueue.pop();
        fileJobCount++;
    }

    jobCounter = fileJobCount + 1; // Set counter to continue after file jobs

    std::unordered_map<char, std::priority_queue<Job>> jobClassQueues;

    std::unordered_map<char, int> processors = {
        {'A', 2}, {'B', 1}, {'C', 1}, {'D', 1}, {'L', 1}, {'Z', 1}
    };

    std::vector<Job> outputQueue;

    // Start threads
    std::thread inputThread(handleUserInput, std::ref(inputQueue));
    std::thread moveJobsThread(moveJobsToClassQueues, std::ref(inputQueue), std::ref(jobClassQueues));
    std::thread dispatchThread(dispatchJobs, std::ref(jobClassQueues), std::ref(processors), std::ref(outputQueue));

    // Wait for input thread to finish
    inputThread.join();
    // After input thread finishes, set keepRunning to false to stop other threads
    keepRunning = false;
    cv.notify_all(); // Notify all waiting threads
    moveJobsThread.join();
    dispatchThread.join();

    std::cout << "\nOutput Queue Jobs:\n";
    for (const Job& job : outputQueue) {
        std::cout << "Job ID: " << job.jobID
            << " | Class: " << job.jobClass
            << " | Start Time: " << job.startTime
            << " | End Time: " << job.endTime
            << " | Elapsed Time: "
            << std::fixed << std::setprecision(1) << job.elapsedTime
            << " seconds\n";
    }

    std::cout << "System shutting down...\n";
    return 0;
}

