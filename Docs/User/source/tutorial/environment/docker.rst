Docker (optional) 
-----------------
-----------------

In the root folder of this project there is a Dockerfile which includes all the demo dependencies. ::

    cd ~/Projects/MARTe2-demos-padova
    docker build -t <DOCKER_IMAGE> .

For instance, you can use "docker build -t marte2-demos-padova/centos:7 ."
Note that you need to map your local Projects directory with the /root/Projects directory in the container.
Also, the docker daemon needs to be running.

To execute the image with your host folder mapped into the container, run: ::
    
    docker run -it -e DISPLAY=$DISPLAY -w /root/Projects -v ~/Projects:/root/Projects:Z -v /tmp/.X11-unix:/tmp/.X11-unix <DOCKER_IMAGE>

Following the previous example, the command should be "docker run -it -e DISPLAY=$DISPLAY -w /root/Projects -v ~/Projects:/root/Projects:Z -v /tmp/.X11-unix:/tmp/.X11-unix marte2-demos-padova/centos:7"
where <DOCKER_IMAGE> is the name of the image in the form [REPOSITORY]:[TAG]. For instance, "docker.io/centos:7"

