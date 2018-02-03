FROM openjdk:8
MAINTAINER ome-devel@lists.openmicroscopy.org.uk

RUN useradd -m bf
COPY . /opt/bioformats/
RUN chown -R bf /opt/bioformats

USER bf
WORKDIR /opt/bioformats
RUN mvn install

ENV TZ "Europe/London"
