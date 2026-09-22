FROM gentoo/portage:latest as portage-base
FROM gentoo/stage3:latest as gentoo-base
COPY --from=portage-base /var/db/repos/gentoo /var/db/repos/gentoo

RUN chown -R portage:portage /var/cache/distfiles
# Create the necessary directories for Portage environment files
RUN install -m 0750 -o portage -g portage -d /etc/portage/{env,repos.conf} /etc/portage/package.{accept_keywords,env,unmask,use}

# Test config
RUN printf 'COMMON_FLAGS="-O2 -pipe -frecord-gcc-switches"\nCFLAGS="${COMMON_FLAGS}"\nCXXFLAGS="${COMMON_FLAGS}"\nFCFLAGS="${COMMON_FLAGS}"\nFFLAGS="${COMMON_FLAGS}"\nLDFLAGS="${LDFLAGS} -Wl,--defsym=__gentoo_check_ldflags__=0"\nFEATURES="collision-protect ipc-sandbox network-sandbox sandbox split-log split-elog strict test userfetch userpriv usersandbox"\nFEATURES="${FEATURES} -parallel-install"\nPORTAGE_ELOG_CLASSES="log warn error qa"\nPORTAGE_ELOG_SYSTEM="echo save"\nDISTUTILS_STRICT_ENTRY_POINTS=1\nALLOW_TEST="network"\nIWDT_ALL=y\nQA_CMP=y\nQA_CMP_ARGS="-xS"\nQA_SED=y\nQA_VDB=y' > /etc/portage/env/test.conf


# Add the package.accept_keywords entries 
RUN printf '%s\n' '=app-portage/pkg-testing-tools-9999 **' > /etc/portage/package.accept_keywords/pkg-testing-tools && \
 printf '%s\n' '=sci-physics/hepmc-9999 **' > /etc/portage/package.accept_keywords/hepmc && \
 printf '%s\n' 'sci-mathematics/unuran ~*' > /etc/portage/package.accept_keywords/unuran && \
 printf '%s\n' 'sci-physics/root -X -qt5 -asimage -opengl -jupyter' > /etc/portage/package.use/root

# Install tools
RUN emerge -q -g \
 dev-util/ccache \
 =app-portage/pkg-testing-tools-9999
# missing root deps: https://github.com/gentoo/gentoo/pull/46793/changes
RUN emerge -q -g media-libs/giflib media-libs/libjpeg-turbo
# Get HepMC dependencies
RUN pkg-testing-tool -muc 1 -adu --fail-fast --append-emerge "-q -g --usepkg-exclude 'sci-physics/hepmc' --autounmask=y --autounmask-write --autounmask-continue --autounmask-use=y --autounmask-backtrack=y" --test-feature-scope force --extra-env-file 'test.conf' -p =sci-physics/hepmc-9999
# Unmerge HepMC
RUN emerge --unmerge hepmc