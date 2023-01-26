#!/usr/bin/env bash

set -e 

PORT=3490

if [[ $(docker ps | grep tester) ]]; then
    docker kill tester
fi

docker build -t webserv .
docker run -dp $PORT:$PORT webserv
