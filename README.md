## JITQ - Just-In-Time Query Compilation

JITQ is a dataflow framework featuring just-in-time compilation and query optimization in presence of user-defined functions.

## Installation

This is a best-effort guide to help getting started with JITQ development.
Since it may run out-of-date, you may also look into [`.gitlab-ci.yml`](.gitlab-ci.yml),
which contains the commands used to set up and run compilation and tests for every commit,
as well as the [Dockerfile](https://hub.docker.com/r/ingomuellernet/buildenv/dockerfile/)
which is used to run the CI tests.

0. Git submodules

```bash
git submodule update --recursive --init
```

1. Python requirements

```bash
sudo apt install python3 python3-pip python3-dev
pip3 install -r requirements.txt
pip3 install -r python/jitq/tests/requirements.txt
```

2. Dev requirements

```bash
sudo apt install build-essential \
        libtinfo-dev \
        wget \
        xz-utils
```

3. CMake

```bash
sudo mkdir /opt/cmake-3.13.4/ && \
cd /opt/cmake-3.13.4/ && \
wget https://cmake.org/files/v3.13/cmake-3.13.4-Linux-x86_64.tar.gz -O - \
    | sudo tar -xz --strip-components=1 && \
for file in bin/*; \
do \
    sudo ln -s $PWD/$file /usr/bin/$(basename $file)-3.13; \
done
```

4. Clang

Download and add links:

```bash
sudo mkdir /opt/clang+llvm-5.0.0/ && \
cd /opt/clang+llvm-5.0.0/ && \
wget http://releases.llvm.org/5.0.0/clang+llvm-5.0.0-linux-x86_64-ubuntu16.04.tar.xz -O - \
     | sudo tar -x -I xz --strip-components=1 && \
for file in bin/*; \
do \
    sudo ln -s $PWD/$file /usr/bin/$(basename $file)-5.0; \
done
```

5. Gold linker

```bash
sudo apt install binutils-dev ninja-build python-minimal
```

Download and build with ninja:

```bash
cd /tmp/ && \
wget http://releases.llvm.org/5.0.0/llvm-5.0.0.src.tar.xz && \
tar -xf llvm-5.0.0.src.tar.xz && \
mkdir /tmp/llvm-5.0.0.src/build && \
cd /tmp/llvm-5.0.0.src/build && \
CXX=clang++-5.0 CC=clang-5.0 cmake-3.13 -G Ninja ../ \
    -DLLVM_BINUTILS_INCDIR=/usr/include \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DLLVM_OPTIMIZED_TABLEGEN=ON && \
ninja LLVMgold
```

Copy goldlinker:

```bash
sudo cp /tmp/llvm-5.0.0.src/build/lib/LLVMgold.so /opt/clang+llvm-5.0.0/lib
```

6. cppcheck

```bash
sudo apt install build-essential \
        libpcre3-dev \
        unzip
```

Download and build:

```bash
cd /tmp/ && \
wget https://github.com/danmar/cppcheck/archive/1.81.zip && \
unzip 1.81.zip && \
cd /tmp/cppcheck-1.81 && \
make SRCDIR=build CFGDIR=/opt/cppcheck-1.81/share/cfg HAVE_RULES=yes \
     CXXFLAGS="-O2 -DNDEBUG -Wall -Wno-sign-compare -Wno-unused-function" && \
sudo mkdir -p /opt/cppcheck-1.81/bin /opt/cppcheck-1.81/share && \
sudo cp -r cfg /opt/cppcheck-1.81/share && \
sudo cp cppcheck /opt/cppcheck-1.81/bin/cppcheck-1.81;
```

Add links:

```bash
for bin in /opt/cppcheck-1.81/bin/cppcheck-1.81; do \
    sudo ln -s $bin /usr/bin/; \
done
```

7. GraphViz

```bash
sudo apt-get install libgraphviz-dev
```

8. Boost

```bash
sudo apt install python-dev autotools-dev libicu-dev libbz2-dev
cd /tmp/ && \
wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz -O - \
    | tar -xz && \
cd /tmp/boost_1_69_0 && \
echo "using clang : 5.0 : $(which clang-5.0) ; " > tools/build/src/user-config.jam && \
./bjam --toolset=clang-5.0 --python=$PYTHONVERSION -j6 --prefix=/opt/boost-1.69.0 && \
sudo ./bjam install
```

## Development

0. Setup build:

Configure CMake:

```bash
cd jitq/backend/build
CXX=clang++-5.0 CC=clang-5.0 cmake-3.13 ../src/ -DLLVM_DIR=/opt/clang+llvm-5.0.0/lib/cmake/llvm -DBOOSTROOT=/opt/boost-1.69.0
```

Configure JIT compilation:

```bash
echo -e "CC=clang-5.0\nCXX=clang++-5.0\nLIBOMPDIR=/opt/clang+llvm-5.0.0/lib" > .../src/code_gen/cpp/Makefile.local
```

1. Build:

```bash
make
```

2. Set `PYTHONPATH` and `JITQPATH`:

```bash
export JITQPATH=$PWD
export PYTHONPATH=$PWD/python
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/clang+llvm-5.0.0/lib
```

3. Run tests and format checkers before commit:

```bash
tools/check_all.sh
python3 -m unittest discover -v -s python/jitq/tests
cd backend/build
make test
(cd ../../python/jitq/tests/ && python3 -m unittest discover -v)
```

## Common Issues

1. `libruntime.so` cannot be found by python cffi.

Add `$JITQPATH/backend/build` to the environment variable `LD_LIBRARY_PATH`.
