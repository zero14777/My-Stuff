FROM debian:latest
MAINTAINER Peter Greer "pbgreerb@ucsc.edu"
RUN apt-get update -y
RUN apt-get install -y python-pip python-dev build-essential
COPY . /hello
WORKDIR /hello
RUN pip install -r requirements.txt

EXPOSE 8080

ENTRYPOINT ["python"]
CMD ["hello.py"]