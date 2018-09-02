
docker build -t particles2d-image .
docker run --rm -it -e DISPLAY -v $XAUTHORITY:/root/.Xauthority -v /tmp/.X11-unix:/tmp/.X11-unix:ro --name particles2d-container particles2d-image
