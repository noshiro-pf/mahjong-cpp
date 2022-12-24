FROM ubuntu:20.04
LABEL maintainer "pystyle"

WORKDIR /app

ENV LC_ALL C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    git \
    libboost-all-dev

# build mahjong-cpp
RUN git clone https://github.com/nekobean/mahjong-cpp.git
WORKDIR /app/mahjong-cpp/build

RUN cmake -DBUILD_SERVER=ON ..
RUN make -j$(nproc)


# Install SSH server
RUN apt-get install -y --no-install-recommends openssh-server
RUN echo "root:root" | chpasswd
RUN sed -i "s/#PermitRootLogin prohibit-password/PermitRootLogin yes/" /etc/ssh/sshd_config

# RUN mkdir /var/run/sshd
# # # SSH login fix. Otherwise user is kicked off after login
# RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

# ENV NOTVISIBLE "in users profile"
# RUN echo "export VISIBLE=now" >> /etc/profile


RUN rm -rf /var/lib/apt/lists/*


EXPOSE 22

CMD service ssh start && /bin/bash

WORKDIR /app/mahjong-cpp/build/src/server
