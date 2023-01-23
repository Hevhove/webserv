#!/usr/bin/env bash

set -e 

PORT=3490
docker build -t webserv .
docker run -dp $PORT:$PORT webserv
