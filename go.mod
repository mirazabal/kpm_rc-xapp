module kpm-rc

go 1.18

require (
	gerrit.o-ran-sc.org/r/ric-plt/xapp-frame v0.9.3
	github.com/golang/protobuf v1.5.2 // indirect
	github.com/spf13/viper v1.4.0
	golang.org/x/net v0.0.0-20210614182718-04defd469f4e // indirect
	k8s.io/utils v0.0.0-20210629042839-4a2b36d8d73f // indirect
)

require (
	gerrit.o-ran-sc.org/r/com/golog v0.0.2 // indirect
	gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc v0.0.0-00010101000000-000000000000 // indirect
	gerrit.o-ran-sc.org/r/ric-plt/alarm-go.git/alarm v0.5.0 // indirect
	gerrit.o-ran-sc.org/r/ric-plt/nodeb-rnib.git/common v1.2.1 // indirect
	gerrit.o-ran-sc.org/r/ric-plt/nodeb-rnib.git/entities v1.2.1 // indirect
	gerrit.o-ran-sc.org/r/ric-plt/nodeb-rnib.git/reader v1.2.1 // indirect
	gerrit.o-ran-sc.org/r/ric-plt/sdlgo v0.7.0 // indirect
	github.com/PuerkitoBio/purell v1.1.1 // indirect
	github.com/PuerkitoBio/urlesc v0.0.0-20170810143723-de5bf2ad4578 // indirect
	github.com/asaskevich/govalidator v0.0.0-20190424111038-f61b66f89f4a // indirect
	github.com/beorn7/perks v1.0.0 // indirect
	github.com/davecgh/go-spew v1.1.1 // indirect
	github.com/docker/go-units v0.4.0 // indirect
	github.com/fsnotify/fsnotify v1.4.9 // indirect
	github.com/go-openapi/analysis v0.19.5 // indirect
	github.com/go-openapi/errors v0.19.3 // indirect
	github.com/go-openapi/jsonpointer v0.19.3 // indirect
	github.com/go-openapi/jsonreference v0.19.3 // indirect
	github.com/go-openapi/loads v0.19.4 // indirect
	github.com/go-openapi/runtime v0.19.4 // indirect
	github.com/go-openapi/spec v0.19.3 // indirect
	github.com/go-openapi/strfmt v0.19.4 // indirect
	github.com/go-openapi/swag v0.19.7 // indirect
	github.com/go-openapi/validate v0.19.6 // indirect
	github.com/go-redis/redis v6.15.9+incompatible // indirect
	github.com/go-stack/stack v1.8.0 // indirect
	github.com/google/uuid v1.1.2 // indirect
	github.com/gorilla/mux v1.7.1 // indirect
	github.com/hashicorp/hcl v1.0.0 // indirect
	github.com/jessevdk/go-flags v1.4.0 // indirect
	github.com/magiconair/properties v1.8.0 // indirect
	github.com/mailru/easyjson v0.7.0 // indirect
	github.com/matttproud/golang_protobuf_extensions v1.0.1 // indirect
	github.com/mitchellh/mapstructure v1.1.2 // indirect
	github.com/pelletier/go-toml v1.2.0 // indirect
	github.com/pmezard/go-difflib v1.0.0 // indirect
	github.com/prometheus/client_golang v0.9.3 // indirect
	github.com/prometheus/client_model v0.0.0-20190812154241-14fe0d1b01d4 // indirect
	github.com/prometheus/common v0.4.0 // indirect
	github.com/prometheus/procfs v0.0.0-20190507164030-5867b95ac084 // indirect
	github.com/spf13/afero v1.2.2 // indirect
	github.com/spf13/cast v1.3.0 // indirect
	github.com/spf13/jwalterweatherman v1.0.0 // indirect
	github.com/spf13/pflag v1.0.3 // indirect
	github.com/stretchr/objx v0.2.0 // indirect
	github.com/stretchr/testify v1.5.1 // indirect
	go.mongodb.org/mongo-driver v1.1.2 // indirect
	golang.org/x/sys v0.0.0-20210423082822-04245dca01da // indirect
	golang.org/x/text v0.3.6 // indirect
	golang.org/x/xerrors v0.0.0-20200804184101-5ec99f83aff1 // indirect
	google.golang.org/genproto v0.0.0-20180817151627-c66870c02cf8 // indirect
	google.golang.org/grpc v1.21.0 // indirect
	google.golang.org/protobuf v1.26.0 // indirect
	gopkg.in/yaml.v2 v2.3.0 // indirect
)

replace gerrit.o-ran-sc.org/r/ric-plt/xapp-frame => gerrit.o-ran-sc.org/r/ric-plt/xapp-frame.git v0.9.3

replace gerrit.o-ran-sc.org/r/ric-plt/sdlgo => gerrit.o-ran-sc.org/r/ric-plt/sdlgo.git v0.8.0

replace gerrit.o-ran-sc.org/r/com/golog => gerrit.o-ran-sc.org/r/com/golog.git v0.0.2

// replace gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc => /home/mir/workspace/osc-ric-install-guide/kpm-rc-xapp/protocol/grpc/ricmsgcommrpc/rc

replace gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc => /go/src/gerrit.o-ran-sc.org/r/kpm-rc/rc/protocol/grpc/ricmsgcommrpc/rc


