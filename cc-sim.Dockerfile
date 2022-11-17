FROM gcc:latest

RUN apt-get update 
# RUN apt-get install -y clang-11
RUN apt-get install -y cmake ninja-build python3 python3-pip
RUN apt-get install -y clang-format vim git
RUN pip install cmake-format compdb conan

WORKDIR /simulator
ENV CONAN_USER_HOME=/simulator/build
COPY requirements.txt .
RUN pip install -r requirements.txt

# COPY . .
# RUN bash setup_build.sh --build
# RUN cmake -DBUILD_PROJECT="no_tests" -G Ninja -B build
# RUN cd build && ninja
CMD [ "bash" ]
