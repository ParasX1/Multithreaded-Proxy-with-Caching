# Multi-Threaded Proxy with Caching

## Introduction

Welcome to the Multi-Threaded Proxy with Caching project by Paras Patange. This C program serves as a proxy, facilitating connections to web servers. It incorporates multi-threading for concurrent handling of multiple clients and requests. Additionally, attempts were made to implement caching using the Least Recently Used (LRU) method.

### Project Details

- **Author:** Paras Patange
- **UID:** PARAS PATANGE
- **Proxy Ports:** 13100 and 13101

## Implementation Overview

The proxy is designed to handle multiple clients concurrently and connect to web servers on their behalf. Key features and components include:

- **Threading:** Threads are utilized to manage concurrent connections, and each thread handles a separate client request.
- **HTTP Request Construction:** The proxy constructs HTTP requests to forward to web servers. It includes essential headers such as `GET`, `Host`, and `Connection`.
- **URI Parsing:** A function parses the URI to extract the host, path, and client port from the URI.
- **Header Building:** The proxy builds HTTP headers for communication with the destination server.
- **Caching (Attempted):** Attempts were made to implement caching using the LRU method. The cache management is in progress and is present in the `cache.c` and `cache.h` files.

## Usage

1. Compile the program: `gcc -o proxy proxy.c csapp.c cache.c -lpthread`
2. Run the proxy: `./proxy <port_number> <replacement_policy>` (e.g., `./proxy 8080 LRU`)
   - Supported replacement policies: `LRU` (Least Recently Used) and `LFU` (Least Frequently Used).

## Functions

1. **`http_Parser(char *uri, char *host, char *path, int *client_port_num)`**
   - Parses the URI to extract host, path, and client port.

2. **`build_header(char *created_header, rio_t *rio, char *host, char *path, int *client_port_num)`**
   - Builds HTTP headers for communication with the destination server.

3. **`handle_connection(int connection_file_descriptor)`**
   - Handles the entire HTTP connection process, including proxy-server and server-client interactions.

4. **`rio_connection(rio_t dst_rio, int connection_file_descriptor, char buffer[MAXLINE], char url_store[100], char server_http_header[MAXLINE], int dst_file_descriptor)`**
   - Manages Rio actions for reading and writing during the connection.

5. **`threading(void *vargp)`**
   - Handles threading, opening, and closing threads for concurrent connections.

6. **`main(int argc, char **argv)`**
   - The main function handles input, initializes the cache queue, and creates threads for concurrent connections.

## Notes

- If the proxy cannot connect to the server, it sets the error code `ENOENT`.

## References

The implementation drew inspiration from various online resources, including:

1. [Multi-threaded Proxy Server](https://github.com/jerrinss5/Multi-threaded-Proxy-Server)
2. [Basic Proxy Server](https://github.com/jeeschr/basic-proxy-server)
3. [Web Proxy](https://github.com/kkudrolli/Web-Proxy)

Please note that caching functionality is a work in progress, and the project will be updated accordingly.

## Acknowledgments

Special thanks to the open-source community and online resources contributing to the understanding and development of proxy servers and multi-threading in C.

## Author:
Paras Patange
