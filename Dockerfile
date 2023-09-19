# Docker build command that 
#sudo docker build -t example.com:80/kpm_rc:1.0.0 . 



FROM nexus3.o-ran-sc.org:10002/o-ran-sc/bldr-ubuntu20-c-go:1.0.0 as build-kpm_rc
#FROM nexus3.o-ran-sc.org:10002/o-ran-sc/bldr-ubuntu18-c-go:1.9.0 as build-kpm_rc

# Install utilities
RUN apt update && apt install -y iputils-ping net-tools curl sudo ca-certificates


RUN go version

# Install RMr shared library & development header files
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr_4.8.0_amd64.deb/download.deb && dpkg -i rmr_4.8.0_amd64.deb && rm -rf rmr_4.8.0_amd64.deb
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr-dev_4.8.0_amd64.deb/download.deb && dpkg -i rmr-dev_4.8.0_amd64.deb && rm -rf rmr-dev_4.8.0_amd64.deb

# Install dependencies, compile and test the module
RUN mkdir -p /go/src/gerrit.o-ran-sc.org/r/kpm_rc/rc
COPY . /go/src/gerrit.o-ran-sc.org/r/kpm_rc/rc

WORKDIR /go/src/gerrit.o-ran-sc.org/r/kpm_rc/rc

#WORKDIR "/go/src/kpm_rc"

ENV GO111MODULE=on GO_ENABLED=0 GOOS=linux

COPY go.mod .
COPY go.sum .
#COPY cmd cmd
COPY e2ap e2ap
COPY e2sm e2sm
COPY protocol protocol
# COPY xapp-descriptor xapp-descriptor
RUN ls

#RUN go get golang.org/x/net@v0.0.0-20210614182718-04defd469f4e
RUN go get google.golang.org/grpc@v1.39.0

# "COMPILING E2AP Wrapper"
RUN cd e2ap && \
    gcc -c -fPIC -Iheaders/ lib/*.c wrapper.c && \
    gcc *.o -shared -o libe2apwrapper.so && \
    cp libe2apwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2ap && \
    cp wrapper.h headers/*.h /usr/local/include/e2ap && \
    ldconfig

# "COMPILING E2SM Wrapper"
RUN cd e2sm && \
    gcc -c -fPIC -Iheaders/ lib/*.c wrapper.c && \
    gcc *.o -shared -o libe2smwrapper.so && \
    cp libe2smwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2sm && \
    cp wrapper.h headers/*.h /usr/local/include/e2sm && \
    ldconfig

#RUN go build -a -installsuffix cgo -o kpm_rc kpm_rc.go

RUN go build .
#kpm_rc.go && pwd && ls -lat


# Final deployment container
FROM ubuntu:18.04

ENV CFG_FILE=config/config-file.json
ENV RMR_SEED_RT=config/uta_rtg.rt

RUN mkdir /config

COPY --from=build-kpm_rc /go/src/gerrit.o-ran-sc.org/r/kpm_rc/rc /
COPY --from=build-kpm_rc  /go/src/gerrit.o-ran-sc.org/r/kpm_rc/rc/config/* /config/
COPY --from=build-kpm_rc /usr/local/lib /usr/local/lib

RUN ldconfig

RUN chmod 755 /kpm_rc
CMD /kpm_rc

