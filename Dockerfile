from centos

RUN  yum -y install gcc
RUN  yum -y install gcc-c++
RUN  yum -y install cmake
RUN  yum -y install automake autoconf libtool make

RUN mkdir /root/dev

WORKDIR /root/dev