FROM ubuntu:20.04
LABEL MAINTAINER="Lucaslah <lucaslah@cssudii.tk>"

ARG apt_username
ARG apt_password

RUN apt-get install -yq --no-install-recommends \
    git \
    curl \
    vim

RUN echo "deb [trusted=yes] https://apt.cssudii.xyz main main" | sudo tee -a /etc/apt/sources.list
RUN echo "deb [trusted=yes] https://apt.cssudii.xyz trusty main" | sudo tee -a /etc/apt/sources.list

RUN apt update
RUN apt-get install we-bt16
RUN apt-get install turbod

EXPOSE 80 8080 577

VOLUME ["/var/we-bt16/config", "/var/we-bt16/logs", "/var/we-bt16/data"]

CMD ["turbod" "--work-dir=/var/we-bt16"]