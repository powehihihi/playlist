FROM fedora:latest

RUN dnf update -y && dnf install -y \
    #sudo libncurses5-dev libncursesw5-dev 
    cmake git g++ ncurses-devel

# add super user 
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
#USER docker

# build project
ADD ../ /app/
WORKDIR /app/build

RUN cmake .. && make


ENTRYPOINT ["./MusicPlayerUI"]

