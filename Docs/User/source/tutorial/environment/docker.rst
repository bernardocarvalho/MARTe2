Docker (optional) 
-----------------

Docker is a set of platform as a service (PaaS) products that use OS-level virtualization to deliver software in packages called containers, facilitating the development and testing of applications in different environments, and allowing for automatization of its deployment.

In case you choose to use docker for the execution of the examples, you may find in the root folder of the project a Dockerfile including all the demo dependencies which will facilitate the setting-up of the environment.

To use it, run the following in a terminal: ::

    cd ~/Projects/MARTe2-demos-padova;
    docker build -t <DOCKER_IMAGE> .;

where <DOCKER_IMAGE> is the name of the image to be used, for instance, "*docker build -t marte2-directory/centos:7 .*"
(Note that you need to map your local *Projects* directory with the */root/Projects* directory in the container. Also, the docker daemon needs to be running).

To execute the image with your host folder mapped into the container, run: ::
    
    docker run -it -e DISPLAY=$DISPLAY -w /root/Projects -v ~/Projects:/root/Projects:Z -v /tmp/.X11-unix:/tmp/.X11-unix <DOCKER_IMAGE>;

Following the previous example, the command should be "*docker run -it -e DISPLAY=$DISPLAY -w /root/Projects -v ~/Projects:/root/Projects:Z -v /tmp/.X11-unix:/tmp/.X11-unix marte2-directory/centos:7*", where <DOCKER_IMAGE> is the name of the image in the form [REPOSITORY]:[TAG]. For instance, "*docker.io/centos:7*""

