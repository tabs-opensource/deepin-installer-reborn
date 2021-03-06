#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Install package dependencies for development environment.

sudo aptitude install \
    cmake \
    g++ \
    gettext \
    libattr1-dev \
    libparted-dev \
    libqt5x11extras5-dev \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxss-dev \
    libxtst-dev \
    pkg-config \
    qt5-qmake \
    qt5-default \
    qtbase5-dev \
    qttools5-dev-tools \
    zlib1g-dev \
    btrfs-progs \
    dosfstools \
    e2fsprogs \
    jfsutils \
    mtools \
    ntfs-3g \
    os-prober \
    parted \
    reiserfsprogs \
    squashfs-tools \
    tshark \
    udev \
    util-linux \
    x11-xserver-utils \
    xfsprogs \
    xkb-data
