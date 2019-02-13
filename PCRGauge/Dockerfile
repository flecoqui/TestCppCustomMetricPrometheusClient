# Get the GCC preinstalled image from Docker Hub
FROM gcc:8

RUN apt-get update && apt-get install -y curl git
RUN apt-get install libcurl4-openssl-dev
RUN apt-get install zlib1g-dev

#  install cmake
RUN curl https://cmake.org/files/v3.13/cmake-3.13.2-Linux-x86_64.sh -o /tmp/curl-install.sh \
      && chmod u+x /tmp/curl-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/curl-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/curl-install.sh

ENV PATH="/usr/bin/cmake/bin:${PATH}"

# install library 
WORKDIR /usr/src
RUN git clone https://github.com/jupp0r/prometheus-cpp.git

WORKDIR /usr/src/prometheus-cpp

RUN git submodule init
RUN git submodule update

RUN mkdir /usr/src/prometheus-cpp/3rdparty/civetweb/_build
WORKDIR /usr/src/prometheus-cpp/3rdparty/civetweb/_build
RUN cmake ..  -DCIVETWEB_ENABLE_CXX=ON -DCIVETWEB_ENABLE_SSL=OFF -DCIVETWEB_BUILD_TESTING=OFF
RUN make -j 4 
RUN make install

RUN mkdir /usr/src/prometheus-cpp/3rdparty/googletest/_build
WORKDIR /usr/src/prometheus-cpp/3rdparty/googletest/_build
RUN cmake ..  
RUN make -j 4 
RUN make install

RUN mkdir /usr/src/prometheus-cpp/_build
WORKDIR /usr/src/prometheus-cpp/_build
RUN cmake ..  -DBUILD_SHARED_LIB=ON
RUN make -j 4 
RUN make DESTDIR=/ install

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/dockertest1

# Specify the working directory
WORKDIR /usr/src/dockertest1

# Use GCC to compile the Test.cpp source file
RUN g++ -o PCRGauge PCRGauge.cpp -std=c++11 -lz -I/usr/local/include/  /usr/local/lib/libprometheus-cpp-pull.a /usr/local/lib/libprometheus-cpp-core.a /usr/local/lib/libprometheus-cpp-push.a /usr/local/lib/libcivetweb.a  /usr/local/lib/libcivetweb-cpp.a  /usr/local/lib/libgmock.a  /usr/local/lib/libgmock_main.a  /usr/local/lib/libgtest.a  /usr/local/lib/libgtest_main.a -pthread

ENV LISTENING_PORT 8080
# Run the program output from the previous step
CMD ["./PCRGauge"]