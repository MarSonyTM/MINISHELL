FROM ubuntu:22.04

# Update the package lists
RUN apt-get update

# Install necessary packages
RUN apt-get install -y --fix-missing \
    accountsservice \
    accountsservice-ubuntu-schemas \
    acl \
    adduser \
    adwaita-icon-theme \
    alsa-topology-conf \
    alsa-ucm-conf \
    alsa-utils \
    apache2-bin \
    apg \
    apparmor \
    apport \
    apport-symptoms \
    appstream \
    apt \
    apt-config-icons \
    apt-transport-https \
    wget \
    gnupg \
    git

# Add the LLVM package repository
RUN echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-12 main" | tee /etc/apt/sources.list.d/llvm.list

# Import the repository GPG key
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

# Update the package lists
RUN apt-get update

# Install clang-12
RUN apt-get install -y clang-12

# Copy your code into the container
COPY . /program

# Set the working directory
WORKDIR /program