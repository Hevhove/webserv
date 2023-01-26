#!/usr/bin/env bash

set -e 

PORT=3490
CONTAINER_NAME="ciaomamma"

if [[ $(docker ps | grep tester) ]]; then
    docker kill $CONTAINER_NAME
fi

docker build -t webserv .
docker run --name $CONTAINER_NAME -dp $PORT:$PORT webserv
