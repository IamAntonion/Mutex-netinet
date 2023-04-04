#include <iostream>
#include <string.h>
#include <mutex>
#include <thread>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

#define PORT     8080
#define MAXLINE 1024

string general_numbers;

int sockfd;
struct sockaddr_in servaddr;

mutex mtx;

bool is_numeric(string const& str)
{
    char* p;
    strtol(str.c_str(), &p, 10);
    return *p == 0;
}

int examination(string const& str_numbers)
{
    if (!is_numeric(str_numbers)) {
        return 1;
    }
    else if (str_numbers.size() > 64) {
        return 2;
    }
    return 0;
}

string change_of_even_values(string const& str)
{
    string change_str = str;

    for (int i = 0; i < change_str.size(); i++) {
        if (change_str[i] % 2 == 0) {
            change_str.push_back('0');

            for (int j = change_str.size() - 1; j > i; j--) {
                change_str[j] = change_str[j - 1];
            }

            change_str[i] = 'K';
            change_str[i + 1] = 'B';

            i++;
        }
    }
    return change_str;
}

union exchange_line{
	unsigned u;
	char c[4];
};

void connect_server(){;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
}

void send_data(unsigned data){
	exchange_line buffer;
	buffer.u = data;

	sendto(sockfd, (const char*)buffer.c, 4,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
			    std::cout<< buffer.u <<" : sent."<<std::endl;

	return;
}

void disconnect_server(){

    close(sockfd);

	return;
}

void first_flow() {
    while (1) {
        string numbers_str;
        bool is_verified = true;

        sleep(0.3);
        while (is_verified)
        {
            cout << "Enter a string consisting only of numbers not exceeding 64 characters\n";
            cin >> numbers_str;

            int number_error = examination(numbers_str);
            if (number_error == 0) is_verified = false;
            else if (number_error == 1) cout << "String contains non-numeric characters\n";
            else cout << "There are more than 64 characters in the string\n";
        }

        sort(numbers_str.begin(), numbers_str.end());

        numbers_str = change_of_even_values(numbers_str);
        cout << numbers_str << endl;

        mtx.lock();
        general_numbers = numbers_str;
        mtx.unlock();
    }
}

void second_flow() {
    while (1) {
        string in_numbers;
        mtx.lock();
        if (general_numbers != "") {
            in_numbers = general_numbers;
            general_numbers = "";
        }
        mtx.unlock();
        if (in_numbers != ""){
        	connect_server();
            int sum_numbers = 0;

            for (int i = 0; i < in_numbers.size(); i++) {
                if ((in_numbers[i] != 'K') && (in_numbers[i] != 'B')) {
                    sum_numbers += in_numbers[i] - 0x30;
                }
            }

            cout << "Sum of all elements: " << sum_numbers << endl;
            send_data(sum_numbers);
            disconnect_server();
        }
    }
}

int main()
{
	thread t1(first_flow);
    thread t2(second_flow);

    t1.join();
    t2.join();

    return 0;
}
