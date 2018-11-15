#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include "library.h"
#include "global.h"

/*
 * SECTION 1: GLOBAL DATA
 * ----------------------
 * Add your own data fields below this
 *
 */

/*
 * SECTION 2: CALLBACK FUNCTIONS
 * -----------------------------
 * The following functions are called on the corresponding event
 *
 */
int timeout;
int windowSize;
int confRecep = 1;
char datosRecibidos[500];

/*
 * Creates a new connection. You should declare any variable needed in the upper section, DO NOT DECLARE
 * ANY VARIABLES HERE. This function should only make initializations as required.
 *
 * You should save the input parameters in some persistant global variable (defined in Section 1) so it can be
 * accessed later
 */
void connection_initialization(int _windowSize, long timeout_in_ns) {
  windowSize = _windowSize;
  timeout = timeout_in_ns;

}

/* This callback is called when a packet pkt of size n is received*/
void receive_callback(packet_t *pkt, size_t n) {

  if(pkt->len < n || VALIDATE_CHECKSUM(pkt)==0){
    perror("Mal paquete");
    return;
  }
    if( pkt->type == DATA ){
      ACCEPT_DATA(pkt->data,pkt->len);
      SEND_ACK_PACKET(pkt->ackno);
    }else{
      confRecep = 1;
      RESUME_TRANSMISSION();

    }


}

  char *datos;
  int numDatos;
/* Callback called when the application wants to send data to the other end*/
void send_callback() {

  if(confRecep == 0){
    PAUSE_TRANSMISSION();
  }


  datos= (char *)malloc( 500*sizeof(void) );

  numDatos = READ_DATA_FROM_APP_LAYER( datos, 500*sizeof(void) );
  if(conf <= 0){
    perror("No hay datos");
    return;
  }


  SEND_DATA_PACKET(DATA, numDatos , 1, 1 ,datos); //ACK es 1, no vale para nada
  confRecep=0;

}

/*
 * This function gets called when timer with index "timerNumber" expires.
 * The function of this timer depends on the protocol programmer
 */
void timer_callback(int timerNumber) {
  SEND_DATA_PACKET(DATA, numDatos , 1, 1 ,datos); //ACK es 1, no vale para nada
}
