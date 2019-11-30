#include "sample_hardware.hpp"
#include "Networkbits.hpp"

//Network thread - responsible for listening for connectinos and responding with updated tempature values
void networktest()
{
    //This only runs when BOTH switches are pressed down
    if ((SW1 == 0) || (SW2 == 0)) return;
	printf("\r\nBasic HTTP server example\r\n");

    //generic error code handle
    int err=0;

    // Ethernet Interface
    EthernetInterface eth;
    eth.set_network(IP, NETMASK, GATEWAY); // comment this line out if you wish to use DHCP
    eth.connect();
    printf("\r\nThe target IP address is '%s'\r\n", eth.get_ip_address());

    // Use to be :- TCPServer srv;!!!
    TCPSocket srv;
    TCPSocket *clt_sock;

    // Open the server on ethernet stack
    srv.open(&eth);

    // BIND the server to the HTTP port (TCP 80)
    err=srv.bind(eth.get_ip_address(), 80);

    printf("IP Address confirm at : %s\n\r",eth.get_ip_address());

    if(err==0) {
        printf("Bind OK\n\r");
    } else {
        printf("Bind error=%d\n\r",err);
    }

    // LISTEN Can handle 5 simultaneous connections
    err=srv.listen(5);
    if(err==0) {
        printf("Listening OK\n\r");
    } else {
        printf("Listen error=%d\n\r",err);
    }

    
    //KEEP RESPONDING WHILE THE SWITCHES ARE PRESSED
    while ((SW1 == 1) && (SW2 == 1)) 
		{
			// ACCEPT Accepting connections now.....
        clt_sock=srv.accept();

        printf("Waiting.....\n\r");

			
			 //Uses a C++ string to make it easier to concatinate
        string response;
        //This is a C string
        char temp_str[64];
        
        //Read the temperature value - note that getParameters() is thread safe
        float temp = sensor.getTemperature();
        
        //Convert to a C String
        sprintf(temp_str, "%5.3f", temp );
        printf("Tempeture: %5.3f\n\r", temp);
        
        //Build the C++ string response
			
			response = HTTP_START;
			
			response += HTTP_HEADER_START;
			response += "    <title>ELEC350 Coursework</title>\r\n";
			response += HTTP_HEADER_END;
			
			response += HTTP_BODY_START;
			response += "style=\"display:flex;text-align:center;\">\r\n";
			response += "    <div style=\"margin:auto;\">\r\n";
			response += "      <h1>Hello World</h1>\r\n";
			response += "      <p> The tempeture is ";
			response += temp_str;
			response += "\r\n    </div>\r\n";
			response += HTTP_BODY_END;
			
			response += HTTP_BOTTOM;

			
//        response = HTTP_MESSAGE_BODY1;
//        response += ldr_str;
//        response += HTTP_MESSAGE_BODY2;
        
        //Send static HTML response (as a C string)
        clt_sock->send(response.c_str(), response.size()+6); 
				printf("%s",response.c_str());

        clt_sock->close();
        wait_us(1000000);//Delay 1 second
				
				
		}
	}
