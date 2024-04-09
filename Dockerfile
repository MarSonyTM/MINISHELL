FROM debian:latest

RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    make \
    gcc \
    g++ \
    valgrind \
    libreadline-dev

RUN mkdir program
WORKDIR /program

COPY . /program/