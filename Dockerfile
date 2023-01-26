FROM ubuntu:20.04

WORKDIR /app
COPY . . 
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install build-essential php php-cli php-fpm -y
USER root
RUN make re
CMD ["./webserv", "a"]
EXPOSE 3490
