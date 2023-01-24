# web-server
A simple web server built using clang

[![forthebadge](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com)


## Requirements:
- gcc
- cmake
- git
- make or ninja

## Cloning:
```
$ git clone https://github.com/krishpranav/web-server
```

## Building:

- Building:
- ```create a build folder``` 
```
$ mkdir build
$ cd build && cmake .. 
$ make
```

- Server
```
$ ./web-server
Listening on port 9000
Waiting for connection...
```

- Client:
```
./webserver
Connected to 127.0.0.1:9000
Sent: Hi!
Received: Hi!
Done.
```

