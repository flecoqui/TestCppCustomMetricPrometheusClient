# Custom Prometheus Metric PCR Gauge
This sample application implements a sample PCR Gauge based on a Custom Prometheus Metric running in Container in Azure Kubernetes Service.


## Using the Application

PCR Gauge Custom Prometheus Client:
Syntax:
PCRGauge --prometheusport <local TCP port used to receive Prometheus request>
         --tsport <local UDP port used to receive the TS stream>
        [--verbose] 


For instance:
Launch PCRGauge to receive the http request on port 8080 and receiving the TS Stream on multicast address 239.0.0.1 and UDP port 1234:

        PCRGauge --prometheusport 8080 --tsport 239.0.0.1:1234 

Launch PCRGauge to receive the http request on port 8080 and receiving the TS Stream on unicast address 127.0.0.1 and UDP port 1234

        PCRGauge --prometheusport 8080 --tsport 127.0.0.1:1234 



## Building the container image with Azure CLI

az acr build --registry "ACRName"   --image pcrgauge:v1 .


The image is built using the DockerFile below:



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


## Deploying the container image  with Kubectl

kubectl apply -f pcrgauge.aks.yaml -n monitoring


The yaml file used to deploy the solution on AKS:


            apiVersion: v1
            kind: Service
            metadata:
            name: pcrgauge
            labels:
                app: pcrgauge
            spec:
            ports:
            - name: http
                port: 8080
                targetPort: http
            - name: ts
                protocol: UDP
                port: 1234
                targetPort: 1234
            selector:
                app: pcrgauge
            ---
            apiVersion: apps/v1
            kind: Deployment
            metadata:
            name: pcrgauge
            labels:
                app: pcrgauge
            spec:
            replicas: 1
            selector:
                matchLabels:
                app: pcrgauge
            template:
                metadata:
                labels:
                    app: pcrgauge
                spec:
                containers:
                - name: pcrgauge
                    image: testacreu2.azurecr.io/pcrgauge:v1
                    command: ["./PCRGauge","--prometheusport", "8080", "--tsport", "127.0.0.1:1234" ]
                    imagePullPolicy: IfNotPresent
                    ports:
                    - containerPort: 8080
                        name: http       
                    - containerPort: 1234
                        name: ts                 
                    env:
                    - name: LISTENING_PORT
                        value: "8080"
            ---
            apiVersion: monitoring.coreos.com/v1
            kind: ServiceMonitor
            metadata:
            name: pcrgauge
            labels:
                app: pcrgauge
            spec:
            selector:
                matchLabels:
                app: pcrgauge
            endpoints:
            - port: http

## Checking the deployment of the  container image

### Checking if the pod is deployed with kubectl

Use the following command:

        kubectl get pods -n monitoring

### Checking if the Custom Metric is visible with kubectl

Use the following command:

    kubectl port-forward -n monitoring prometheus-prometheus-operator-prometheus-0 9090

### Checking the counter values with kubectl

Use the following command:

    kubectl exec pcrgauge-c58fdc84b-p49b4 -n monitoring -- curl http://127.0.0.1:8080/metrics

