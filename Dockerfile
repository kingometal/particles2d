FROM alpine:3.7

RUN apk add --no-cache make g++ libx11-dev

ENV SRCFOLDER /home/developer

VOLUME ['$SRCFOLDER']

COPY *.cpp $SRCFOLDER/
COPY *.h $SRCFOLDER/
COPY include/rnd.h $SRCFOLDER/include/
COPY makefile $SRCFOLDER/

CMD cd /home/developer; make; ./particles2d.x

