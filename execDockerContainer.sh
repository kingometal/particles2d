#!/bin/bash
docker build -t particles2d-image .
docker run --rm -e DISPLAY -v $XAUTHORITY:/root/.Xauthority -v /tmp/.X11-unix:/tmp/.X11-unix:ro --name particles2d-container --mount type=bind,src=$(pwd),dst=/home/ particles2d-image /bin/bash -c "cd /home/ && $@"
