#######################################
# 1) Build stage  (≈ 1.9 GB, discarded)
#######################################
FROM ubuntu:24.04 AS build

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        ninja-build build-essential cmake git \
        qt6-base-dev libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy the whole repo into the image
WORKDIR /src
COPY . .

# Configure + build + run unit tests
ARG BUILD_TYPE=Release
RUN cmake -S BlokchainSim/BlokchainSim -B /build -G Ninja \
          -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    && cmake --build /build --config ${BUILD_TYPE} \
    && ctest --test-dir /build -C ${BUILD_TYPE} --output-on-failure

#######################################
# 2) Runtime stage  (slim image)
#######################################
FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
        qt6-base-dev libssl3 ca-certificates \
        libx11-xcb1 libxcb-icccm4 libxcb-image0 \
        libxcb-keysyms1 libxcb-render-util0 libxcb-xinput0 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=build /build/BlokchainSim ./BlokchainSim

ENV QT_QPA_PLATFORM=xcb
ENTRYPOINT ["./BlokchainSim"]



