FROM fedora:35
RUN  yum -y  install  dnf-plugins-core \
                      bc make doxygen cmake binutils git wget diffutils file sed gawk grep which \
                      gcc-gfortran gcc-c++ clang bzip2   openssl-devel openssl \
                      libzip-devel  zlib zlib-devel pythia8-devel pythia8 \
                      valgrind HepMC HepMC-devel xz xz-devel bzip2 bzip2-devel zlib zlib-devel zlib-static libzstd \
                      libzstd-devel graphviz graphviz-devel pythia8 pythia8-devel \
                      ccache  \
                      protobuf protobuf-devel  \
                      python3 python3-devel  python3-numpy   \
                      && yum -y clean all

RUN                   if [ $(cat /etc/fedora-release  | grep -Eo '[0-9]+') -gt  23 ]; then yum -y  install protobuf protobuf-devel root-core root root-physics python3 python3-devel python2 python2-devel; fi  && yum -y clean all
RUN                   if [ $(cat /etc/fedora-release  | grep -Eo '[0-9]+') -eq  24 ]; then sed -i 's@message(FATAL_ERROR \"The imported @message(STATUS \"The imported@g' /usr/share/root/cmake/ROOTConfig-targets.cmake ; fi
RUN                   if [ $(cat /etc/fedora-release  | grep -Eo '[0-9]+') -eq  33 ]; then yum -y  install pypy pypy-devel; fi && yum -y clean all

# cat /etc/fedora-release | grep -Eo '[0-9]+'
# 22 -- cmake is too old, no libzstd libzstd-devel
# 23 -- no protobuf no root and no python
# 24 -- sed on ROOT is needed.
# 33 -- added pypy pypy-devel

