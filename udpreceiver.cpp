#include "local_sock.h"
#include <ctime>

int main(){
  int sock, n;
  socklen_t server_len,client_len;
  struct sockaddr_in server, client;

  if((sock=socket(PF_INET, SOCK_DGRAM,0))<0){
    perror("socket open error ");
    return 1;
    }
  
  memset(&server,0,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(65300);

  if (bind(sock, (struct sockaddr *) &server, sizeof(server))<0){
    perror("binding error");
    return 2;

    } 

  server_len = sizeof(server);

  if(getsockname(sock, (struct sockaddr *) &server, &server_len) <0){
    perror("getsocketname "); 
    return 3;

 }
  cout <<"using port " <<ntohs(server.sin_port)<<endl;
  while(1){
    if ((n= recvfrom(sock, buf, BUFSIZ, 0, (struct sockaddr *) &client, &client_len))<0){
      perror("recvfrom ");
      close(sock);
      return 4;
       }
    //handle buf
    std::time_t result = std::time(NULL);
    std::string name = "data";
    struct tm* localtm = std::localtime(&result);
    std::string t; 
    std::string y = std::to_string(1900+localtm->tm_year);
    std::string m = std::to_string(1+localtm->tm_mon);
    std::string d = std::to_string(localtm->tm_mday);
    std::string h = std::to_string(localtm->tm_hour);
    t.append(y);
    t.append(m);
    t.append(d);
    t.append(h);
    name.append(t);

    FILE *fp = fopen(name.c_str(),"ab");
    if (fp != NULL){
      fputs(buf, fp);
      fclose(fp);
     }
  }
}
