FROM gcc:latest

# update and install packages
RUN apt-get update && apt-get install -y \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Copy the source code and Makefile into the container
COPY . .

# run project using make
RUN make

# ENTRYPOINT ["./lab6"]

# CMD []