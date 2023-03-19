#!/usr/bin/env bash

set -e

PORTS=("81:81" "85:85" "3490:3490" "3491:3491")
CONTAINER_NAME="ciaomamma"

if [[ $(docker ps -a --filter "name=^/${CONTAINER_NAME}$" --format '{{.Names}}') == $CONTAINER_NAME ]]; then
    docker rm $CONTAINER_NAME
fi

if [[ $(docker ps -a | grep tester) ]]; then
    docker kill $CONTAINER_NAME
fi

docker build -t webserv .

# Build the ports arguments string
PORTS_ARGS=""
for port in "${PORTS[@]}"; do
  PORTS_ARGS+=" -p ${port}"
done

docker run --name $CONTAINER_NAME -d ${PORTS_ARGS} webserv
