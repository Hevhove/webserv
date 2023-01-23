FROM ubuntu:20.04

WORKDIR /app
COPY . . 
RUN apt-get update && apt-get install build-essential -y
USER root
RUN make
CMD ["./server", "a"]
EXPOSE 3490
