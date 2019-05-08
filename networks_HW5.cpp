#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

 
#define PORT 3800
#define IP "64.183.98.170"
#define DISCONNECT 2
#define EXIT_MSG "Disconnecting"


int main() {

	int CreateSocket = 0,n = 0;
    char dataReceived[1024] = {'0'};
    char sendData[1024] = {'0'};
    struct sockaddr_in ipOfServer;
    bool keepTalking = true;
    
    //create an endpoint for communication on host end
    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		
        cout << "Socket not created \n";
        return -1;
    }
    
    //set address family, port, and IP
    ipOfServer.sin_family = AF_INET;//AF_NET - designates what type of addresses my socket can communicate with (IPv4)
    ipOfServer.sin_port = htons(PORT);
    ipOfServer.sin_addr.s_addr = inet_addr(IP);
    
    //establish connection to the specified socket
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer)) < 0) {
		cout << "could not connect\n";
		return -1;
	}
	
	//server-client communication
	while(keepTalking) {
		
		//read information
		while((n = read(CreateSocket, dataReceived, sizeof(dataReceived))) > 0) {
			
			if(n < 0) {
				printf("read error\n");
				return -1;
			}
			else if(n == 0) {
				break;
			}
			
			//display server output to stdout
			dataReceived[n] = '\0';
			cout << dataReceived << endl;
			
			if(strcmp(dataReceived, EXIT_MSG)) {
				keepTalking = false;
				break;	
			}
		}
		
		//send information
		if(keepTalking) {
			
			cin >> sendData;
			
		    if(send(CreateSocket, sendData, sizeof(sendData), MSG_EOR) < 0) {
			    printf("send failed\n");
		    }
		}
		else {
			break;
		}	
	}

	//shutdown the socket 
    if(shutdown(CreateSocket, DISCONNECT) < 0) {
        cout << "shutdown fail\n";
    }
    
    
    return 0;
}
