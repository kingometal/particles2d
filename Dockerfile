#FROM alpine:3.7
FROM ubuntu:bionic

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y make
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends libsdl2-dev
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends g++
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends libsdl2-ttf-dev 
RUN rm -rf /var/lib/apt/lists/*  # clean up apt-get cache

#RUN apk add --no-cache make g++ libx11-dev sdl2-dev sdl2_ttf-dev libc6-compat

ENV SRCFOLDER /home/developer

VOLUME ['$SRCFOLDER']

COPY source/*.cpp $SRCFOLDER/source/
COPY source/*.h $SRCFOLDER/source/
COPY source/interfaces/*.h $SRCFOLDER/source/interfaces/
COPY *.ttf $SRCFOLDER/
COPY Makefile $SRCFOLDER/

CMD cd /home/developer; make; ./particles2d.x
