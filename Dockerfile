FROM fedora:latest

RUN dnf update -y && dnf install -y \
    #sudo libncurses5-dev libncursesw5-dev 
    cmake git g++ ncurses-devel

# add super user 
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
#USER docker

# Catch2 for tests
RUN git clone https://github.com/catchorg/Catch2.git && \
    cd Catch2 && \
    cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
    cmake --build build/ --target install

# build project
ADD ./ /app/
WORKDIR /app/build

RUN cmake .. && make

# final step for ncurses


ENTRYPOINT ["./MusicPlayerUI"]

