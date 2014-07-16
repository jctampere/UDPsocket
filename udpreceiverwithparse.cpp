#include "local_sock.h"
//#include <ctime>
#include <vector>


std::string fname = "trafficlightdata";
int parseData();
struct DsgMap{
    uint8_t d_index; //index of detector
    uint8_t sg_index; //index of signal group
};
struct DataRow{
    short tick_count;
    vector<string> grint;
    vector<string> dint;
    vector<unsigned short> detectorSignal;

};

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
  server.sin_port = htons(65400);

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
    
    //save data to a file
    //std::time_t result = std::time(NULL);
    
    /*struct tm* localtm = std::localtime(&result);
    std::string t; 
    std::string y = std::to_string(1900+localtm->tm_year);
    std::string m = std::to_string(1+localtm->tm_mon);
    std::string d = std::to_string(localtm->tm_mday);
    t.append(y);
    t.append(m);
    t.append(d);
    name.append(t);*/
    
    FILE *fp = fopen(fname.c_str(),"ab");
    if (fp != NULL){
      //everytime write 1024 bytes
      fwrite(buf,1,1024,fp);
      fclose(fp);
     }
     
      parseData();
  }
   
}

int parseData(){
    cout<<"start parse "<<endl;
    FILE *datafile = fopen(fname.c_str(),"r+");
    char *buffer;
    size_t result;
    
    if(datafile!=NULL){
        buffer = (char*) malloc(1024);
        if(buffer == NULL) {fputs("Memory error",stderr);return 2;}
        result = fread(buffer,1,1024,datafile);
        if(result !=1024){fputs("Reading error",stderr);return 3;}
        
        //parse header
        char messageType = buffer[0];
        cout<<messageType<<endl;
        unsigned short messageLength = buffer[2];
        cout<<(int)messageLength<<endl;
        uint8_t locationLength = buffer[3];
        cout<<(int)locationLength<<endl;
        string location((buffer+4), locationLength);
        cout<<location<<endl;
        
        //parse message body
        char* mbodyStart = buffer+4+locationLength;
        switch(messageType){
                //check message type and parse
            case 0x63:{ //configuration message
                uint8_t numSG = *mbodyStart;
                cout<<(int)numSG<<endl;
                uint8_t numD = *(mbodyStart+1);
                cout<<(int)numD<<endl;
                
                //signal group names
                vector<string> signalGroupNames;
                int i=0;
                int l=0;
                char* p = mbodyStart+2;
                char* c = mbodyStart+2;
                string sn;
        
                while(*(c+i) != 0x0a){//ASCII Line Feed "0a"
                    if(*(c+i)==','){
                        string sn(p,l);
                        signalGroupNames.push_back(sn);
                        l=0;
                        p=c+i+1;
                    
                    } else{
                        l++;
                      }
                    i++;
                }
                p=c+i;
                //print
                for( std::vector<string>::const_iterator iter = signalGroupNames.begin(); iter != signalGroupNames.end(); ++iter)
                    std::cout << *iter << ' ';
                
                //detector names
                vector<string> detectorNames;
                i =0;
                l=0;
                c = p;
                
                while(*(c+i) !=0x0a){ //ASCII LINE FEED
                    if(*(c+i)==','){
                        string sn(p,l);
                        detectorNames.push_back(sn);
                        l=0;
                        p=c+i+1;
                    
                    }else {
                        l++;
                    }
                    i++;
                
                }
                p=c+i;
                //print
                for( std::vector<string>::const_iterator iter2 = detectorNames.begin(); iter2 != detectorNames.end(); ++iter2)
                    std::cout << *iter2 << ' ';
                
                //dsgMap, detector--signal group associations 2*numD bytes
                c=p;
                vector<DsgMap> dsgMap;
                for (i=0;i<2*((int)numD);i+=2){
                    DsgMap dm = {*p,*(p+1)};
                    dsgMap.push_back(dm);
                    p=c+i;
                }
                
                //detSens, Detector sensitivity setting, numD bytes
                vector<uint8_t> detSens;
                c=p;
                for (i=0;i<(int)numD;i++){
                    detSens.push_back(*p);
                    p=c+i;
                }
                //print
                for( std::vector<uint8_t>::const_iterator iter3 = detSens.begin(); iter3 != detSens.end(); ++iter3)
                    std::cout << (int)*iter3 << ' ';
                break;
            }
            case 0x64: { //keep alive message
                unsigned long time = (*mbodyStart)<<24|(*(mbodyStart+1))<<16|(*(mbodyStart+2))<<8|(*(mbodyStart+3));
                cout<<time<<endl;
                break;
            }
            case 0x65:{ //Data message
                char reserved = *mbodyStart;
                cout<<reserved<<endl;
                short sequence = (*(mbodyStart+1))<<8|*(mbodyStart+2);
                cout<<sequence<<endl;
                unsigned long dtime = (*(mbodyStart+3))<<24|(*(mbodyStart+4))<<16|(*(mbodyStart+5))<<8|*(mbodyStart+6);
                cout<<dtime<<endl;
                short rowCount =(*(mbodyStart+7))<<8|*(mbodyStart+8);
                cout<<rowCount<<endl;
                uint8_t dnumSG =*(mbodyStart+9);
                uint8_t dnumD = *(mbodyStart+10);
                cout<<dnumSG<<" "<<dnumD<<endl;
                
                break;}
            }
     
        
        
    }else {
        fputs("File error",stderr);
        return 1;
    }
}


