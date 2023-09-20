module gerrit.o-ran-sc.org/r/hw-go

go 1.15

require (
	gerrit.o-ran-sc.org/r/ric-plt/alarm-go.git/alarm v0.5.0
	gerrit.o-ran-sc.org/r/ric-plt/xapp-frame v0.9.3
	github.com/golang/protobuf v1.5.2 // indirect
	github.com/spf13/viper v1.4.0
	golang.org/x/net v0.0.0-20210614182718-04defd469f4e // indirect
	k8s.io/utils v0.0.0-20210629042839-4a2b36d8d73f // indirect

)

require (
	gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc v0.0.0-00010101000000-000000000000
	github.com/google/uuid v1.1.2 // indirect
	github.com/prometheus/client_model v0.0.0-20190812154241-14fe0d1b01d4 // indirect
	golang.org/x/xerrors v0.0.0-20200804184101-5ec99f83aff1 // indirect
)

replace gerrit.o-ran-sc.org/r/ric-plt/xapp-frame => gerrit.o-ran-sc.org/r/ric-plt/xapp-frame.git v0.9.3

replace gerrit.o-ran-sc.org/r/ric-plt/sdlgo => gerrit.o-ran-sc.org/r/ric-plt/sdlgo.git v0.8.0

replace gerrit.o-ran-sc.org/r/com/golog => gerrit.o-ran-sc.org/r/com/golog.git v0.0.2

replace gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc => /go/src/hw-go/protocol/grpc/ricmsgcommrpc/rc

//replace gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc => /home/vboxuser/wrk/hw-go/protocol/grpc/ricmsgcommrpc/rc
