#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "csapp.h"
#include "cache.h"

/*
UID: PARAS PATANGE :)

The following code is a proxy which is used to connect to a webserver. Caching was not fully completed using the LRU method,
howevere there has been a significant attempt, which can bee seen in the cache.c and cache.h files.
LFU was not completed in time (attempt below for integration and in in cache.c and cache.h files above refrences).
This proxy has the ability to handle multipe clients and requests (concurency).
Threading was done using arguments which allows the connection_file_descriptor (config if LRU) to be parsed into the thread.

My calculated ports were : 13100 and 13101

*/

// For constructing the http request for proxy server request.
static const char *get_request = "GET %s HTTP/1.0\r\n";  //gets request
static const char *formating_of_host = "Host: %s\r\n";  //What is the format of the host
static const char *connection = "Connection: close\r\n";  //Connection has closed

//combined_n_header: is a connection of the proxy, the other client access information
static const char *combined_n_header = "Proxy-Connection: close\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36\r\n";
static const char *line_end = "\r\n"; //combine combined above

// For idenifying and extraction of data and check the type of connection */ 
static const char *host_id = "Host"; //Host information
static const char *user_info_id = "User-Agent";  //Agent information
static const char *proxy_status = "Proxy-Connection";  //proxy connection status (connected)

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Retrieves the host, path and clients port from the uri
// This function was adapted from refrence link (12) at the bottom of the file you can find links.
void http_Parser(char *uri, char *host, char *path, int *client_port_num){
/////////////// ADD MORE COMMENTS TO THIS

    size_t index_ptr;
    size_t counter_1 = 0;
    size_t counter_Port = 0;
    char ptr[MAXLINE];
    char port_string[MAXLINE];
    char port[MAXLINE];

    strcpy(ptr, uri);
    if (strstr(ptr, "://")) {  //finds the https:// and removes it
        strcpy(ptr, index(ptr, ':'));
        strcpy(ptr, &ptr[3]);
    }
    
    assert (ptr != NULL);//  to get the host name by finding the first, if it exists
    while((ptr[index_ptr] != '\0') && (ptr[index_ptr] != ':') && (ptr[index_ptr] != '/')){
        index_ptr++;
        counter_1++;
    }
    
    strncpy(host, ptr, counter_1); //counts where the actual useful information is present
    if (ptr[index_ptr] == ':') {// Finds the client port part of the uri
        index_ptr += 1;
        strcpy(port_string, &ptr[index_ptr]);
        while (isdigit(ptr[index_ptr])){
            counter_Port++;
            index_ptr++;
        }
        strncpy(port, port_string, counter_Port);
        *client_port_num = atoi(port);
    } else {
        
        *client_port_num = 80; //Defualt client port (80)
    }

    strcpy(path, &ptr[index_ptr]);
    memset(ptr, 0, strlen(ptr));
    return;
}


//Extracts information from the Rio, and builds the header using a buffer
// This function was inspired from refrence link (6) at the bottom of the file you can find links.
void build_header(char *created_header, rio_t *rio, char *host, char *path, int *client_port_num){
    char buffer[MAXLINE]; //buffer for building
    char request[MAXLINE];
    char seperate_info[MAXLINE];
    char host_header[MAXLINE];

    printf("Checker build_header");

    sprintf(request, get_request, path);
    for (int i = Rio_readlineb(rio, buffer, MAXLINE); i != 0;  i = Rio_readlineb(rio, buffer, MAXLINE)) {
        if (strcmp(buffer, line_end) == 0){   
            printf("Check header 1 ");
            break; // Break from while loop if the buffer has ended
        }else if (!strncasecmp(buffer, host_id, strlen(host_id)))
        {
            printf("Check header 2 ");// no host exists
            strcpy(host_header, buffer);
            continue;
        }
    }
    
    printf("Header built"); // Saves the formatted data into created_header.
    sprintf(created_header, "%s%s%s%s", request, host_header, combined_n_header, line_end);
     //proxy_connection, user_agent_hdr, line_end merged into combined_n_header
}   


//The following function will handle the HTTP Connection, cache the send information to and from the webserver to the client.

void handle_connection(int connection_file_descriptor){
    char buffer[MAXLINE];
    char method[MAXLINE];
    char uri[MAXLINE];
    char protocol_no[MAXLINE];
    char convert_int_to_char[10];
    char server_http_header[MAXLINE]; //header
    char host[MAXLINE]; //host
    char path[MAXLINE]; //path
    char url_store[100];
    int client_port_num; //port number
    int dst_file_descriptor;
    rio_t rio;// Client rio and
    rio_t dst_rio; // server rio
    

    Rio_readinitb(&rio, connection_file_descriptor); //sets up Rio
    Rio_readlineb(&rio, buffer, MAXLINE); //reads from the Rio and puts into the buffer

    sscanf(buffer, "%s %s %s", method, uri, protocol_no); //read the client request
    if (strcmp("GET", method) != 0){
        fprintf(stderr, "Get Method Accepted Only.");
        return;
    }
    
    // strcpy(url_store, uri); // Stores the uri which can be later used ()Caching.
    // // if (get_l(url_store, connection_file_descriptor) == 1){
    // //     return;
    // // }

    http_Parser(uri, host, path, &client_port_num);// Parse uri into http header to be sent to destintion
    build_header(server_http_header, &rio, host, path, &client_port_num); //builds the header

    // Connect to server
    sprintf(convert_int_to_char, "%d", client_port_num);
    dst_file_descriptor = open_clientfd(host, convert_int_to_char); //opens the socket

    if (dst_file_descriptor == -1){
        printf("Couldn't connec : %s, %d", host, client_port_num);  //Check if this works!
        errno = ENOENT;
        // Sets errors code ENOENT if the proxy cannot connect to the server.
        return;
    } /*write and send the new the http header to end web server*/
    
    rio_connection(dst_rio, connection_file_descriptor, buffer, url_store, server_http_header, dst_file_descriptor);  //connects
    Close(dst_file_descriptor);

    printf("Valid connection");

    return;
}

// This function was inspired from refrence link (9) at the bottom of the file you can find links.
//THIS WAS USED IN A CACHING FUNCTION: (ATTEMPT)
// int get_l(char url_store[100], int connection_file_descriptor){  //returns to sender "aka get lost"
//     int cache_index = cache_find(url_store);
//     //If cached is found, perform the arrangment of LRU.
//     }
//     return 0;
// }

// This function was inspired from refrence link (1) at the bottom of the file you can find links.
//Function which handles rio actions such as reading and writing
void rio_connection(rio_t dst_rio, int connection_file_descriptor, char buffer[MAXLINE], char url_store[100], char server_http_header[MAXLINE], int dst_file_descriptor) {

    //Copy the buffer then send it to the end server to the client that was requested.
    Rio_readinitb(&dst_rio, dst_file_descriptor);
    Rio_writen(dst_file_descriptor, server_http_header, strlen(server_http_header));
    char cachebuf[MAX_OBJECT_SIZE];
    while (1) //infinte loop till done
    {
        size_t data = Rio_readlineb(&dst_rio, buffer, MAXLINE);
        if (data != 0){
            Rio_writen(connection_file_descriptor, buffer, data);
            strcat(cachebuf, buffer);
            printf("Checker rio_connection");
        }
        else{
            break;
        }
    }
    // cache_uri_data(url_store, cachebuf); //Caches the data at the end of the connection
    
}


//Handles threading opening, and closing threads 
void *threading(void *vargp){
    int connection_file_descriptor = (int)vargp;
    Pthread_detach(pthread_self());
    handle_connection(connection_file_descriptor); //establish connection
    Close(connection_file_descriptor); //close thread
}


//Main function handles input, then creates connection and initalises queue for LRU cacheing(Semi complete see Cache.c) then creates threading 
int main(int argc, char **argv){

    if(argc != 3) {
        if (argv[2] == "LRU") {
           fprintf(stderr, "./proxy # replacement_policy\n", argv[0]);
            exit(0); 
        }else if (argv[2] == "LFU") {
           printf("LFU not implemented!");
            exit(0);
        }else {
            printf("Input Not valid!");
            exit(0);
        }
    }else {
        printf("Argument 3 was invalid ");
    }

    int file_descriptor;
    int connection_file_descriptor;
    socklen_t Client_length;
    struct sockaddr_storage client_address; // Enough space for any address  // line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE]; 
    char client_port_num[MAXLINE]; //Provides information on what is the clients port
    pthread_t thread_id; //Provides information to what thread is being run
    file_descriptor = Open_listenfd(argv[1]);
    initialise_queue(); // Initialises Cache Queue  
    while (1){  
        Client_length = sizeof(client_address);
        connection_file_descriptor = Accept(file_descriptor, (SA *)&client_address, &Client_length); //accepts
        Getnameinfo((SA *)&client_address, Client_length, client_hostname, MAXLINE, client_port_num, MAXLINE, 0);  //gets information to use/display
        printf("Connected: (%s, %s)\n", client_hostname, client_port_num);
        strcmp(argv[2], "LRU") == 0;  
        Pthread_create(&thread_id, NULL, threading, (void *)connection_file_descriptor); //Create threads
    }
    return 0;
}

//Simple program to test functionality of other methods
void Test_function(){
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------GOT TILL TEST FUNCTION----------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Refrence Links Used: ->
//
// (1)https://github.com/jerrinss5/Multi-threaded-Proxy-Server/blob/master/server.py
// (2)https://softwareengineering.stackexchange.com/questions/376818/how-would-you-design-a-multithreaded-lru-cache-using-c-unordered-map-and-li
// (3)https://github.com/webbert/CSAPP-Labs
// (4)https://www.fortinet.com/resources/cyberglossary/proxy-server
// (5)https://www.youtube.com/watch?v=0PSB9y8ehbk
// (6)https://github.com/jeeschr/basic-proxy-server/blob/master/proxy.c
// (7)https://stackoverflow.com/questions/21125269/proxy-server-in-c-multiple
// (8)https://www.geeksforgeeks.org/least-frequently-used-lfu-cache-implementation/
// (9)https://www.enjoyalgorithms.com/blog/least-frequently-used-cache
// (10)https://www.enjoyalgorithms.com/blog/least-frequently-used-cache
// (11)https://www.geeksforgeeks.org/lru-cache-implementation/
// (12)https://github.com/kkudrolli/Web-Proxy
// (13)https://www.youtube.com/watch?v=10VZC3Qrm7U
// (14)http://amritapnitc.blogspot.com/2015/07/simple-proxy-server-in-c-using-multi.html
// (15)https://docs.oracle.com/cd/E19575-01/821-0047/geeie/index.html
//



