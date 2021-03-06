:: Color client image building automatic batch entrypoint
@ECHO OFF

:: Remove previous image of color-client-image
ECHO ====== WELCOME TO COLOR CLIENT IMAGE BUILDING ======
ECHO 1. Deleting previous image...
docker image rm color-client-image
ECHO - PREVIOUS IMAGE DELETED -
ECHO =====================================

:: Build docker image
ECHO 2. Building new Docker image from source...
docker build -t color-client-image .
ECHO - DOCKER IMAGE BUILT -
ECHO =====================================