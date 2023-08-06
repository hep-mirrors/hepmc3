FROM archlinux:latest
RUN  pacman -Syu --noconfirm \
                      bc make  cmake binutils git wget  file sed gawk grep which  \
                      pkgconf  root protobuf   \
                      gcc gcc-fortran clang  \
                      libzip   ccache  && pacman -Scc --noconfirm
