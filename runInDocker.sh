#!/bin/bash

docker run --rm -e DISPLAY -v $XAUTHORITY:/root/.Xauthority -v /tmp/.X11-unix:/tmp/.X11-unix:ro --name particles2d-container particles2d-image
