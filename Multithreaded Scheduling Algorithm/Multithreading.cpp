#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iomanip>

struct arguments {
    std::string str;
    std::vector<float> results;
    std::string schedInfo;
};

/* This is the entropy scheduling program as described by the algorithm, with almost no modifications.
 * It even returns two values! */
std::pair<float, int> ParseMap(std::map<std::string, int> &freq, int currFreq, float currH, std::string selectedTask, int extraFreq) {
    float H, currentTerm, newTerm;
    int NFreq = currFreq + extraFreq;
    if (NFreq == extraFreq) {
        H = 0;
    }
    else {
        if ( freq[selectedTask] == 0 ) {
            currentTerm = 0;
        }
        else {
            currentTerm = freq[selectedTask] * log2(freq[selectedTask]);
        }
        newTerm = (freq[selectedTask] + extraFreq) * log2(freq[selectedTask] + extraFreq);
        H = log2(NFreq) - ((log2(currFreq) - currH) * (currFreq) - currentTerm + newTerm) / NFreq;
    }
    freq[selectedTask] += extraFreq;
    return std::pair<float, int>(H, NFreq);
}

void* ParseString(void* arg_ptr) {
    /* This code parses the arguments from arg_ptr */
    arguments *arg = static_cast<arguments*>(arg_ptr);
    std::string data = arg->str;
    std::stringstream s(data);

    /* The various variables we use.
     * _t1 and _t2 are temporary variables for when we create the queue.
     * q is a queue of tasks to be done, comprised of a string and the scheduling info for each task.
     * freq maps strings (e.g. "A") to their frequency (e.g. 4).
     * entropy is where we store our results for returning. only necessary in multithreaded ver. */

    std::string _t1;
    int _t2;
    std::queue<std::pair<std::string, int>> q;
    std::map<std::string, int> freq;
    std::vector<float> entropy;
    /* In the single threaded version I used a queue to hold
     * info about the list of tasks. Because I pop from the queue however,
     * I need a way to create that little bit of text that holds displays scheduling information
     * after I already deleted it. Instead of changing my old code, which would make sense,
     * I just make the string myself and return it too. */
    std::string schedInfo = "";

    /* This code creates the actual queue of tasks, as well as constructing the
     * schedInfo string. Finally it also sets up the frequency map. */
    while (s >> _t1 >> _t2) {
        q.emplace(std::pair<std::string, int>(_t1, _t2));
        schedInfo += std::string(_t1 + "(" + std::to_string(_t2) + "), ");
        freq[_t1] = 0;
    }
    /* This removes the last two characters from the string, i.e. the ) */
    schedInfo.pop_back();
    schedInfo.pop_back();
    float currH = 0;
    int currFreq = 0;
    /* Simply iterate through the queue, passing our values to the algorithm.  */
    while (!q.empty()) {
        std::tie(currH, currFreq) = ParseMap(freq, currFreq, currH, q.front().first, q.front().second);
        entropy.push_back(currH);
        q.pop();
    }
    /* These are the results of our calculations. Because arg is a pointer
     * that main already has access to, we don't have to return it, and can
     * instead just set its values so that when we get to main
     * it'll be able to use it.*/
    arg->results = entropy;
    arg->schedInfo = schedInfo;
    return nullptr;
}

int main() {

    /* This code creates a list of tasks, i.e. strings to process. */
    /* Then, when we know how many strings we need to process
     * we can start making that number of threads. */
    std::string data;
    std::vector<std::string> listOfTasks;
    while(getline(std::cin, data)) {
        listOfTasks.push_back(data);
    }
    const std::size_t NTHREADS = listOfTasks.size();
    pthread_t* tid = new pthread_t[NTHREADS];
    arguments* arg = new arguments[NTHREADS];

    /* This code makes threads. Most importantly, it updates an arg for each thread that you're passing as input.  */
    for (int i = 0; i < NTHREADS; i++) {
        arg[i].str = listOfTasks[i];
        if (pthread_create(&tid[i], NULL, ParseString, &arg[i])) {
            std::cerr << "Error creating thread!\n";
            return 1;
        }
    }
    for (int i = 0; i < NTHREADS ; i++ ) {
        pthread_join(tid[i], nullptr);
    }

    /* Output to two places. Also, absolute value it for that one annoying case where you get -0.00 */
    std::cout << std::fixed << std::showpoint;
    std::cout << std::setprecision(2);
    for (int i = 0; i < NTHREADS; i++) {
        std::cout << "CPU " << i + 1 << std::endl;
        std::cout << "Task scheduling information: " << arg[i].schedInfo << std::endl;
        std::cout << "Entropy for CPU " << i + 1 << std::endl;
        for (int j = 0; j < arg[i].results.size(); j++) {
            std::cout << std::abs(arg[i].results[j]) << " ";
        }
        std::cout << std::endl << std::endl;
    }
    delete[] tid;
    delete[] arg;
}
