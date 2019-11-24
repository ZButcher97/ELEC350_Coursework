#include "sample_hardware.hpp"
#include "Networkbits.hpp"

//Network thread - responsible for listening for connectinos and responding with updated tempature values
void networktest()
{
    //This only runs when BOTH switches are pressed down
    if ((SW1 == 0) || (SW2 == 0)) return;
    
    lcd.cls();
    lcd.printf("Basic HTTP server example\n\r");
		printf("Basic HTTP server example\n\r");
    
    //Configure an ethernet connection
    EthernetInterface eth;
    eth.set_network(IP, NETMASK, GATEWAY);
		printf("Connecting\n\r");
    eth.connect();
    lcd.printf("The target IP address is '%s'\n\r", eth.get_ip_address());
    printf("The target IP address is '%s'\n\r", eth.get_ip_address());
    //Now setup a web server
    TCPServer srv;           //TCP/IP Server
    TCPSocket clt_sock;      //Socket for communication
    SocketAddress clt_addr;  //Address of incoming connection
    
		
    /* Open the server on ethernet stack */
		printf("Server open\n\r");
    srv.open(&eth);
    
    /* Bind the HTTP port (TCP 80) to the server */
		printf("Binding ip port\n\r");
    srv.bind(eth.get_ip_address(), 80);
    
    /* Can handle 5 simultaneous connections */
    srv.listen(5);
    
    //KEEP RESPONDING WHILE THE SWITCHES ARE PRESSED
    while ((SW1 == 1) && (SW2 == 1)) {
        using namespace std;
        lcd.cls();
        lcd.printf("Open 10.0.0.1 in a browser\n\r");
        printf("Open 10.0.0.1 in a browser\n\r");
			
        //Block and wait on an incoming connection
			printf("waiting to accept incomming connections\n\r");
        srv.accept(&clt_sock, &clt_addr);
        
				printf("accept %s:%d\n\r", clt_addr.get_ip_address(), clt_addr.get_port());
			
        //Uses a C++ string to make it easier to concatinate
        string response;
        //This is a C string
        char ldr_str[64];
        
        //Read the temperature value - note that getParameters() is thread safe
        float temp = sensor.getTemperature();
        
        //Convert to a C String
        sprintf(ldr_str, "%5.3f", temp );
        printf("LDR: %5.3f\n\r", temp);
        
        //Build the C++ string response
        response = HTTP_MESSAGE_BODY1;
        response += ldr_str;
        response += HTTP_MESSAGE_BODY2;
        
        //Send static HTML response (as a C string)
        clt_sock.send(response.c_str(), response.size()+6);    
    }
    
    printf("Release BOTH switches\n\r");
    lcd.printf("Release BOTH switches\n\r");
    while ((SW1 != 0) && (SW2 != 0));
    wait(0.5); //debounce
}