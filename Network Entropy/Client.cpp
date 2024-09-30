#include <unistd.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <strings.h>
#include <cstring>
#include <stdexcept>

using std::size_t;
using std::cout;

struct args {
    std::string host;
    int port;
    std::string buffer;
    std::string schedInfo;
    std::vector<float> results;
};

void* make_connection(void* a) {
    /* Unpack a into an args */
    args* x = static_cast<args*>(a);
    int sockfd;
    struct sockaddr_in serv_addr;
    try {
        /* Create Socket */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            throw std::runtime_error("Socket creation failed.");
        }
        struct hostent *server;
        server = gethostbyname(x->host.c_str());
        if (server == nullptr) {
            throw std::runtime_error("No such host found.");
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
        serv_addr.sin_port = htons(x->port); /*htons values between host and network byte order*/

        /* Create the connection. */
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
            throw std::runtime_error("Unspecified error when connecting.");
        }
        /* Send the message size, then the message stored
         * in the string buffer. */
        int msgSize = sizeof(x->buffer);
        int n = write(sockfd,&msgSize,sizeof(int));
        if (n < 0) throw std::runtime_error("Writing to socket failed.");
        n = write(sockfd,x->buffer.c_str(),msgSize);
        if (n < 0) throw std::runtime_error("Writing to socket failed.");
        std::size_t recSize;

        /* Read the message size, then receive data */
        n = read(sockfd, &recSize, sizeof(recSize));
        if (n < 0) throw std::runtime_error("Reading from socket failed.");
        std::vector<float> entropy(recSize);
        n = read(sockfd, entropy.data(), msgSize);
        if (n < 0) throw std::runtime_error("Reading from socket failed.");

        std::string data = x->buffer;
        std::stringstream s(data);
        std::string _t1;
        int _t2;
        std::string schedInfo = "";
        while (s >> _t1 >> _t2) {
            schedInfo += std::string(_t1 + "(" + std::to_string(_t2) + "), ");
        }
        schedInfo.pop_back();
        schedInfo.pop_back();
        x->schedInfo = schedInfo;
        x->results = entropy;
        close(sockfd);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR, " << e.what() << std::endl;
        exit(0);
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage " << argv[0] << " hostname port" << std::endl;
        exit(0);
    }

    int port = atoi(argv[2]);

    std::vector<std::string> listOfTasks;
    std::string data;
    while(getline(std::cin, data))
        listOfTasks.push_back(data);
    const std::size_t NTHREADS = listOfTasks.size();
    pthread_t* tid = new pthread_t[NTHREADS];
    args* arg = new args[NTHREADS];

    for (std::size_t i = 0; i < NTHREADS; i++) {
        arg[i].host = argv[1];
        arg[i].port = port;
        arg[i].buffer = listOfTasks[i];
        if (pthread_create(&tid[i], NULL, make_connection, &arg[i])) {
            std::cerr << "Error creating thread!\n";
            return 1;
        }
    }
    for (std::size_t i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], nullptr);
    }
    std::cout << std::fixed << std::showpoint;
    std::cout << std::setprecision(2);
    for (std::size_t i = 0; i < NTHREADS; i++) {
        std::cout << "CPU " << i + 1 << std::endl;
        std::cout << "Task scheduling information: " << arg[i].schedInfo << std::endl;
        std::cout << "Entropy for CPU " << i + 1 << std::endl;
        for (std::size_t j = 0; j < arg[i].results.size(); j++) {
            std::cout << std::abs(arg[i].results[j]) << " ";
        }
        std::cout << std::endl << std::endl;
    }
    return 0;
}
