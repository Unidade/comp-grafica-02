FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    sudo \
    git \
    curl \
    build-essential \
    g++ \
    pkg-config \
    freeglut3-dev \
    libglu1-mesa-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libglm-dev \
    libglew-dev \
    libxrandr-dev \
    libxi-dev \
    libxcursor-dev \
    libxinerama-dev \
    x11-apps \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
