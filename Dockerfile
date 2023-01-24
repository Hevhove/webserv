FROM ubuntu:20.04

WORKDIR /app
COPY . . 
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install build-essential -y && apt-get install -y php php-cli php-fpm
USER root
RUN make
CMD ["./webserv", "a"]
EXPOSE 3490
