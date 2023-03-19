FROM ubuntu:20.04

WORKDIR /app
COPY . . 
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install build-essential php php-cli php-fpm -y
USER root
RUN make re
ENTRYPOINT ["sh", "-c", "./webserv /conf/default.conf"]
EXPOSE 81 85 3490 3491
