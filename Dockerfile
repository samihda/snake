FROM alpine:3.8

RUN apk add --update build-base ncurses-dev
WORKDIR /app
COPY . .
RUN make
ENTRYPOINT ["./snake"]
