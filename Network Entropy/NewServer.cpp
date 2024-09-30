#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <map>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
#include <pthread.h>
#include <cstring>
#include <iomanip>

/* Fireman function */
void fireman(int) {
   while (waitpid(-1, NULL, WNOHANG) > 0) {
      // std::cout << "A child process ended" << std::endl;
   }
}

/* Calculate Entropy Function. */
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

/* Function to receive data */
void ParseString(std::string data, int cli_socket) {
    /* Parse values, placing the tasks in the queue "q". */
    std::stringstream s(data);
    std::string _t1;
    int _t2;
    std::queue<std::pair<std::string, int>> q;
    std::map<std::string, int> freq;
    std::vector<float> entropy;
    while (s >> _t1 >> _t2) {
        q.emplace(std::pair<std::string, int>(_t1, _t2));
        freq[_t1] = 0;
    }
    float currH = 0;
    int currFreq = 0;
    while (!q.empty()) {
        std::tie(currH, currFreq) = ParseMap(freq, currFreq, currH, q.front().first, q.front().second);
        entropy.push_back(currH);
        q.pop();        
    }

    std::size_t msgSize = entropy.size();
    int n = write(cli_socket, &msgSize, sizeof(msgSize));
    if (n < 0) {
        std::cerr << "Error writing to socket" << std::endl;
        _exit(0);
    }
    std::cout << "Writing entropy.data()" << std::endl;
    n = write(cli_socket, entropy.data(), sizeof(entropy));
    if (n < 0) {
        std::cerr << "Error writing to socket" << std::endl;
        exit(0);
    }
    close(cli_socket);
}

/* Receive multiple requests using sockets; create child process per request */
int main(int argc, char *argv[]) {
   //Declare variables
   signal(SIGCHLD, fireman);
   int sockfd, newsockfd, portno, clilen;
   struct sockaddr_in serv_addr, cli_addr;

   // Check the commandline arguments
   if (argc != 2) {
      std::cerr << "Port not provided" << std::endl;
      exit(0);
   }

   // Create the socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      std::cerr << "Error opening socket" << std::endl;
      exit(0);
   }

   // Populate the sockaddr_in structure
   bzero((char *)&serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   // Bind the socket with the sockaddr_in structure
   if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      std::cerr << "Error binding" << std::endl;
      exit(0);
   }

   // Set the max number of concurrent connections
   listen(sockfd, 5);
   clilen = sizeof(cli_addr);

   // Accept a new connection
   while(true) {
       std::cout << "Accepting a connection..." << std::endl;

       newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
       if (newsockfd < 0) {
           std::cerr << "Error accepting new connection!" << std::endl;
           exit(0);
       }

       int n, msgSize = 0;
       n = read(newsockfd, &msgSize, sizeof(int));
       if (n < 0) {
           std::cerr << "Error reading from socket." << std::endl;
           exit(0);
       }
       char *tempBuffer = new char[msgSize + 1];
       bzero(tempBuffer, msgSize + 1);
       n = read(newsockfd, tempBuffer, msgSize + 1);
       if (n < 0) {
           std::cerr << "Error reading from socket" << std::endl;
           exit(0);
       }
       std::string buffer = tempBuffer;
       delete[] tempBuffer;

       std::cout << "Parsing this string: " << buffer << "\t Message size: " << msgSize << std::endl;
       if (fork() == 0) {
           close(sockfd);
           ParseString(buffer, newsockfd);
           _exit(0);
       }
   }
   close(sockfd);
   return 0;
}
