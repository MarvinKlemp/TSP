FROM gcc:10

RUN apt-get update && apt-get -y install cmake protobuf-compiler python3-matplotlib
