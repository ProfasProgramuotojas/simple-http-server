#include <winsock2.h>
#include <iostream>
#include <string>
#include <regex>

#pragma comment(lib, "ws2_32.lib")

std::string getRouteFromClient(char buffer[1024])
{
  if (!buffer)
    return "";

  std::string request(buffer);

  std::regex regex_pattern(R"(^GET\s+(/\S*)\s+HTTP/1\.1)");

  std::smatch match;
  if (std::regex_search(request, match, regex_pattern))
  {
    return match[1].str();
  }

  return "";
}

int main()
{
  WSAData wsa;
  SOCKADDR_IN server_addr, client_addr;
  int addr_len = sizeof(client_addr);
  char buffer[1024];

  WSAStartup(MAKEWORD(2, 2), &wsa);
  SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server, 1);

  while (true)
  {
    SOCKET client = accept(server, (struct sockaddr *)&client_addr, &addr_len);
    recv(client, buffer, sizeof(buffer), 0);

    std::string route = getRouteFromClient(buffer);

    if (route == "/")
      route = "Home";

    std::string htmlBody =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head><title>My HTTP Server</title></head>\n"
        "<body>\n"
        "<h1>Hello, from route " +
        route + "!</h1>\n"
                "<p>This is a response from my HTTP server</p>\n"
                "</body>\n"
                "</html>";

    int body_length = htmlBody.length();

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Length: " + std::to_string(body_length) + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += htmlBody;

    send(client, response.c_str(), response.length(), 0);
    closesocket(client);
  }

  closesocket(server);
  WSACleanup();
  return 0;
}