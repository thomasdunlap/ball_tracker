#!/bin/bash

docker run -it --init --privileged --net host --ipc host --device /dev/video0 --device /dev/video1 --env "DISPLAY:$DISPLAY" -v /tmp/.X11-unix:/tmp/.X11-unix -v $(pwd):/home focal-cuda-opencv:latest /bin/bash

