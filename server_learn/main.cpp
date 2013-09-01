#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

void start_server() {
  SOCKET sock;
  //struct sockaddr local_sa;
  struct sockaddr_in local_sin;

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    printf("error socket\n");
    return;
  }
  printf("socket created\n");

  // bind
  local_sin.sin_family = AF_INET;
  local_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  local_sin.sin_port = htons(8080);
  int result = bind(sock, reinterpret_cast<sockaddr*>(&local_sin), sizeof(local_sin));
  if (result == SOCKET_ERROR) {
    printf("error bind");
    closesocket(sock);
    return;
  }
  printf("socket binded\n");

  // listen
  result = listen(sock, SOMAXCONN);
  if (result == SOCKET_ERROR) {
    printf("error listen");
    closesocket(sock);
    return;
  }
  printf("socket listening\n");

  // receive and send
  char buf[4096];
  while (true) {
    // accept
    struct sockaddr_in remote_sin;
    int len = sizeof(remote_sin);
    SOCKET accept_sock = accept(sock, reinterpret_cast<sockaddr*>(&remote_sin), &len);
    if (accept_sock == INVALID_SOCKET) {
      printf("error accept");
      closesocket(sock);
      return;
    }
    printf("socket accepted\n");

    int received_len = recv(accept_sock, buf, sizeof(buf) - 1, 0);
    if (received_len > 0) {
      buf[received_len] = '\0';
      printf("received: %s\n", buf);
    }

    // TODO: send only when recv finished
    char message[] = "hello";
    result = send(accept_sock, message, strlen(message), 0);
    if (result == INVALID_SOCKET) {
      printf("error send\n");
      closesocket(sock);
      return;
    }    
    printf("socket send message: %s\n", message);
    closesocket(accept_sock);
  }

  closesocket(sock);
}

int main() {

  WSADATA data;
  WSAStartup(MAKEWORD(2,2), &data);
  start_server();

  WSACleanup();
  return 0;
}
