#   Copyright (c) 2020 Samsung.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

#----------------------------------------------------------
#
#----------------------------------------------------------
# docker build -t example.com:80/hw-go:1.2 .

FROM nexus3.o-ran-sc.org:10002/o-ran-sc/bldr-ubuntu20-c-go:1.0.0 as build-hw-go

# Install utilities
RUN apt update && apt install -y iputils-ping net-tools curl sudo ca-certificates

# Install RMr shared library & development header files
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr_4.7.0_amd64.deb/download.deb && dpkg -i rmr_4.7.0_amd64.deb && rm -rf rmr_4.7.0_amd64.deb
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr-dev_4.7.0_amd64.deb/download.deb && dpkg -i rmr-dev_4.7.0_amd64.deb && rm -rf rmr-dev_4.7.0_amd64.deb

# Install dependencies, compile and test the module
RUN mkdir -p /go/src/hw-go
COPY . /go/src/hw-go

WORKDIR "/go/src/hw-go"

ENV GO111MODULE=on GO_ENABLED=0 GOOS=linux

COPY go.mod .
COPY go.sum .
#COPY cmd cmd
COPY e2ap e2ap
COPY e2sm e2sm
COPY protocol protocol
# COPY xapp-descriptor xapp-descriptor
RUN ls


#RUN go get google.golang.org/grpc@v1.39.0

# "COMPILING E2SM Wrapper"
RUN cd e2sm && \
    gcc -c -fPIC  -fvisibility=hidden -Iheaders/ lib/*.c wrapper.c  && \
    gcc *.o -shared  -fvisibility=hidden   -o libe2smwrapper.so && \
    cp libe2smwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2sm && \
    cp wrapper.h headers/*.h /usr/local/include/e2sm && \
    ldconfig

# "COMPILING KPM V2"
RUN cd kpmv2 && \
    gcc -c -fPIC -DASN_DISABLE_OER_SUPPORT -DASN_DISABLE_JER_SUPPORT -Ihdr/ lib/*.c wrapper.c -fvisibility=hidden && \
    gcc -DASN_DISABLE_OER_SUPPORT -DASN_DISABLE_JER_SUPPORT  *.o -shared -fvisibility=hidden -o libkpmv2wrapper.so && \
    cp  libkpmv2wrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/kpmv2 && \
    cp wrapper.h hdr/*.h /usr/local/include/kpmv2 && \
    ldconfig


# "COMPILING E2AP Wrapper"
RUN cd e2ap && \
    gcc -c -fPIC  -fvisibility=hidden  -Iheaders/ lib/*.c  wrapper.c && \
    gcc *.o -shared -fvisibility=hidden  -o libe2apwrapper.so && \
    cp libe2apwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2ap && \
    cp wrapper.h headers/*.h /usr/local/include/e2ap && \
    ldconfig


# "COMPILING RC V1"
RUN cd rcv1 && \
    gcc -c -fPIC -DASN_DISABLE_OER_SUPPORT -DASN_DISABLE_JER_SUPPORT -Ihdr/ lib/*.c wrapper.c -fvisibility=hidden && \
    gcc -DASN_DISABLE_OER_SUPPORT -DASN_DISABLE_JER_SUPPORT *.o -shared -fvisibility=hidden -o librcv1wrapper.so && \
    cp librcv1wrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/rcv1 && \
    cp wrapper.h hdr/*.h /usr/local/include/rcv1 && \
    ldconfig


RUN go build -a -installsuffix cgo -o hw-go hwApp.go


# Final deployment container
FROM ubuntu:18.04

ENV CFG_FILE=config/config-file.json
ENV RMR_SEED_RT=config/uta_rtg.rt

RUN mkdir /config

COPY --from=build-hw-go /go/src/hw-go/hw-go /
COPY --from=build-hw-go /go/src/hw-go/config/* /config/
COPY --from=build-hw-go /usr/local/lib /usr/local/lib

RUN ldconfig

RUN chmod 755 /hw-go
CMD /hw-go
