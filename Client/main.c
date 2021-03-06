#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client/client.h"
#include <pthread.h>



//Funciones utilizadas por el main
void makePathToImage(char *userInput);
void readPathToImage();
void makeIpPort(char *port, char *ip);

int main() {
    int pixelNumber = -1;
    char ip[25] = "";
    char port[25] = "";

    printf("Digite la IP a donde se enviaran las imagenes en esta ronda\n");
    scanf("%s", ip);

    printf("Digite el puerto asociado a la IP a donde se enviaran las imagenes en esta ronda\n");
    scanf("%s", port);


    //Verificar si los argumentos se pasaron coreectamente
    if (strcmp(ip, "") == 0 || strcmp(port, "") == 0) {
        printf("Fue imposible conectarse al servidor. Argumentos faltantes o erroneos\n");
        return 1;
    }
    
    
    
    makeIpPort(port, ip);

    

    //Ciclo de envio de datos con los threads
    char userInput[100];
    while(1){
        printf("En caso de querer terminar el programa digitar end. Caso contrario, favor introducir la ruta de la nueva imagen a enviar\n");
        scanf("%s", userInput);
        if(strcmp(userInput, "end") == 0){
            break;
        }
        FILE *file = fopen(userInput, "r");
        //Comprueba si el archivo existe en esa direccion. Si no vuelve al inicio del while
            if (!file) {
                printf("No fue posible abrir el archivo en la direccion %s\n", userInput);
                continue;
            }
        fclose(file);    
        makePathToImage(userInput);
        system(imageLinkToServer);
        readPathToImage();
        printf("Favor introducir el valor del pixel asociado a la imagen\n");
        scanf("%d", &pixelNumber);

        //Creacion del Json para enviar la imagen
        imageDataJson = json_object();
        json_object_set_new(imageDataJson, "image", json_string(encondedContent));
        json_object_set_new(imageDataJson, "pixelValue", json_integer(pixelNumber));
        //Se envia la imagen al servidor
        sendImageData();

        //Se liberan los datos de la memoria dinamica
        free(imageLinkToServer);
        free(encondedContent);
        json_decref(imageDataJson);

    }

    //Liberacion de memoria dinamica
    free(dynamicURL);
  
    



    return 0;
    
    

}

//Codigo que genera el comando de terminal necesario para determinar la ruta dinamica de la imagen
void makePathToImage(char *userInput){
    //Se definen las partes estaticas del comando utilizado
    char *firstPart = "base64 ";
    char *secondPartImage = " > encondedImage.txt";

    //Se reserva memoria dinamica para poder ejecutar el comando
    imageLinkToServer = malloc(strlen(firstPart) + strlen(userInput) + strlen(secondPartImage) + 1);

    //Se construye el comando por medio de concatenacion
    strcpy(imageLinkToServer, firstPart);
    strcat(imageLinkToServer, userInput);
    strcat(imageLinkToServer, secondPartImage);
}

//Codigo que toma el arhivo, lee su contenido y lo serializa desde base64 a String
void readPathToImage(){
    //Se abre el archivo con el fin de leerlo
    FILE *base64Image = fopen("encondedImage.txt", "r");
    char line[121];
    char **readData = NULL;
    int lineLenght;
    int lineAmount = 0;
    unsigned long amountOfChars = 0;
    //Se define la memoria para que se lean las lineas del archivo
    while (fgets(line, 120, base64Image)) {
        readData = realloc(readData, (lineAmount + 1) * sizeof(char*));
        line[strcspn(line, "\n")] = 0;
        lineLenght = strlen(line);
        readData[lineAmount] = calloc(sizeof(char), lineLenght + 1);
        strcpy(readData[lineAmount], line);
        lineAmount++;
        amountOfChars += lineLenght;
    }
    //Se cierra el archivo que se abrio
    fclose(base64Image);
    //Se libera memoria
    encondedContent = malloc(amountOfChars + 1);
    strcpy(encondedContent, readData[0]);
    free(readData[0]);

    for (int i = 1; i < lineAmount; i++) {
        strcat(encondedContent, readData[i]);
        free(readData[i]);
    }
    //Se libera memoria y se limpia el .txt
    free(readData);
    remove("encondedImage.txt");
}

//Codigo que arma el IP y el puerto de manera dinamica segun el input del usuario
void makeIpPort(char *port, char *ip){
    //Se definen las partes estaticas del URL del servidor
    char *firstPart = "http://";
    char *secondPartImage = "/api/imagePixels";
    char *dots = ":";

    //Se reserva la memoria dinamica en el HEAP para poder armar el mensaje
    dynamicURL = malloc(strlen(firstPart) + strlen(ip)+ strlen(dots)+ strlen(port) + strlen(secondPartImage) + 1);

    //Se arma el URL de maner dinamica
    strcpy(dynamicURL, firstPart);
    strcat(dynamicURL, ip);
    strcat(dynamicURL, dots);
    strcat(dynamicURL, port);
    strcat(dynamicURL, secondPartImage);

}    